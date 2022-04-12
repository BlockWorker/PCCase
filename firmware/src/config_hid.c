/* ************************************************************************** */


#include <string.h>

#include "config_hid.h"
#include "usb/usb_device_hid.h"
#include "app.h"
#include "app_config.h"
#include "cooling_control.h"
#include "system/debug/sys_debug.h"



static uint8_t _config_hid_ctrl_send_buffer[256] = { 0 };

static uint8_t _config_hid_interrupt_send_buffer[32] = { 0 };

static USB_DEVICE_HID_TRANSFER_HANDLE _config_hid_interrupt_receive_handles[CONFIG_HID_REQUEST_COUNT] = { 0 };
static uint8_t _config_hid_interrupt_receive_buffers[CONFIG_HID_REQUEST_COUNT][256] = { { 0 } };

static uint32_t _config_hid_requested_writes = 0;

static bool _config_hid_initialized = false;

static uint16_t _config_hid_idle = 0;
static USB_HID_PROTOCOL_CODE _config_hid_protocolcode = 1;


void _config_hid_handle_get_report(USB_DEVICE_HID_EVENT_DATA_GET_REPORT* data) {
    if (data->reportType == 0x01) { //requesting input report
        float* sendbuf_flt = (float*)_config_hid_ctrl_send_buffer;
        
        sendbuf_flt[0] = coolctl_fan_rpm;
        sendbuf_flt[1] = coolctl_pump_rpm;
        sendbuf_flt[2] = coolctl_flow_lph;
        sendbuf_flt[3] = coolctl_temp;
        
        USB_DEVICE_ControlSend(appData.usbHandle, _config_hid_ctrl_send_buffer, 4 * sizeof(float));
        return;
    } else if (data->reportType == 0x03) { //requesting feature report
        uint32_t* sendbuf_32b = (uint32_t*)_config_hid_ctrl_send_buffer;
        
        sendbuf_32b[0] = 0xff;
        
        sendbuf_32b[1] = config_fan_curve_point_count;
        memcpy(sendbuf_32b + 2, config_fan_curve_temperatures, 12 * sizeof(float));
        memcpy(sendbuf_32b + 14, config_fan_curve_values, 12);
        
        sendbuf_32b[17] = config_pump_curve_point_count;
        memcpy(sendbuf_32b + 18, config_pump_curve_temperatures, 12 * sizeof(float));
        memcpy(sendbuf_32b + 30, config_pump_curve_values, 12);
        
        sendbuf_32b[33] = config_watchdog_interval_seconds;
        sendbuf_32b[34] = config_watchdog_runtime_seconds;
        
        memcpy(sendbuf_32b + 35, config_cooling_acceptable_min, 4 * sizeof(float));
        memcpy(sendbuf_32b + 39, config_cooling_acceptable_max, 4 * sizeof(float));
        memcpy(sendbuf_32b + 43, config_cooling_nominal_min, 4 * sizeof(float));
        memcpy(sendbuf_32b + 47, config_cooling_nominal_max, 4 * sizeof(float));
        
        USB_DEVICE_ControlSend(appData.usbHandle, _config_hid_ctrl_send_buffer, 204);
        return;
    }
    USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR); //fallback for all other cases: error
}

void _config_hid_handle_report_received(USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED* data) {
    uint8_t i;
    bool found = false;
    for (i = 0; i < CONFIG_HID_REQUEST_COUNT; i++) { //search for relevant handle in array
        if (_config_hid_interrupt_receive_handles[i] == data->handle) {
            found = true;
            _config_hid_interrupt_receive_handles[i] = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID; //slot is now empty
            break;
        }
    }
    if (!found || data->status != USB_DEVICE_HID_RESULT_OK || data->length != 204) return;
    
    uint32_t* rcvbuf_32b = (uint32_t*)_config_hid_interrupt_receive_buffers[i];
    
    uint32_t active_fields = rcvbuf_32b[0];
    
    if (active_fields & 0x01) {
        config_fan_curve_point_count = rcvbuf_32b[1];
        memcpy(config_fan_curve_temperatures, rcvbuf_32b + 2, 12 * sizeof(float));
        memcpy(config_fan_curve_values, rcvbuf_32b + 14, 12);
    }
    
    if (active_fields & 0x02) {
        config_pump_curve_point_count = rcvbuf_32b[17];
        memcpy(config_pump_curve_temperatures, rcvbuf_32b + 18, 12 * sizeof(float));
        memcpy(config_pump_curve_values, rcvbuf_32b + 30, 12);
    }
    
    if (active_fields & 0x04) config_watchdog_interval_seconds = rcvbuf_32b[33];
    if (active_fields & 0x08) config_watchdog_runtime_seconds = rcvbuf_32b[34];
    
    if (active_fields & 0x10) memcpy(config_cooling_acceptable_min, rcvbuf_32b + 35, 4 * sizeof(float));
    if (active_fields & 0x20) memcpy(config_cooling_acceptable_max, rcvbuf_32b + 39, 4 * sizeof(float));
    
    if (active_fields & 0x40) memcpy(config_cooling_nominal_min, rcvbuf_32b + 43, 4 * sizeof(float));
    if (active_fields & 0x80) memcpy(config_cooling_nominal_max, rcvbuf_32b + 47, 4 * sizeof(float));
    
    _config_hid_requested_writes |= active_fields;
    
    USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_OK);
}

void _config_hid_queue_transfers() {
    uint8_t i;
    USB_DEVICE_HID_RESULT result;
    for (i = 0; i < CONFIG_HID_REQUEST_COUNT; i++) {
        if (_config_hid_interrupt_receive_handles[i] != USB_DEVICE_HID_TRANSFER_HANDLE_INVALID) continue; //leave valid handles, request already present
        result = USB_DEVICE_HID_ReportReceive(USB_DEVICE_HID_INDEX_1, _config_hid_interrupt_receive_handles + i, _config_hid_interrupt_receive_buffers + i, 256);
        if (result != USB_DEVICE_HID_RESULT_OK) break; //stop requesting once an error is encountered
    }
}

void _config_hid_USBDeviceHIDEventHandler(USB_DEVICE_HID_INDEX index, USB_DEVICE_HID_EVENT event, void* pData, uintptr_t context) {
    USB_DEVICE_HID_EVENT_DATA_SET_PROTOCOL* set_protocol_data;
    USB_DEVICE_HID_EVENT_DATA_SET_IDLE* set_idle_data;
    
    switch (event) {
        case USB_DEVICE_HID_EVENT_GET_REPORT:
            _config_hid_handle_get_report((USB_DEVICE_HID_EVENT_DATA_GET_REPORT*)pData);
            break;
        case USB_DEVICE_HID_EVENT_SET_REPORT:
            USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
        case USB_DEVICE_HID_EVENT_GET_PROTOCOL:
            USB_DEVICE_ControlSend(appData.usbHandle, &_config_hid_protocolcode, 1);
            break;
        case USB_DEVICE_HID_EVENT_SET_PROTOCOL:
            set_protocol_data = (USB_DEVICE_HID_EVENT_DATA_SET_PROTOCOL*)pData;
            USB_DEVICE_ControlStatus(appData.usbHandle, set_protocol_data->protocolCode == _config_hid_protocolcode ? USB_DEVICE_CONTROL_STATUS_OK : USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
        case USB_DEVICE_HID_EVENT_GET_IDLE:
            USB_DEVICE_ControlSend(appData.usbHandle, &_config_hid_idle, 1);
            break;
        case USB_DEVICE_HID_EVENT_SET_IDLE:
            set_idle_data = (USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)pData;
            USB_DEVICE_ControlStatus(appData.usbHandle, set_idle_data->duration == _config_hid_idle ? USB_DEVICE_CONTROL_STATUS_OK : USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT:
            USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;
        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:
            _config_hid_handle_report_received((USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED*)pData);
            _config_hid_queue_transfers();
            break;
        default:
            break;
    }
}


void CONFIG_HID_SendUpdate() {
    float* sendbuf_flt = (float*)_config_hid_interrupt_send_buffer;
    
    sendbuf_flt[0] = coolctl_fan_rpm;
    sendbuf_flt[1] = coolctl_pump_rpm;
    sendbuf_flt[2] = coolctl_flow_lph;
    sendbuf_flt[3] = coolctl_temp;
    
    USB_DEVICE_HID_TRANSFER_HANDLE _discard_handle;
    USB_DEVICE_HID_ReportSend(USB_DEVICE_INDEX_1, &_discard_handle, _config_hid_interrupt_send_buffer, 4 * sizeof(float));
}

void CONFIG_HID_Init() {
    uint8_t i;
    for (i = 0; i < CONFIG_HID_REQUEST_COUNT; i++) { //initialize transfer arrays
        memset(_config_hid_interrupt_receive_buffers[i], 0, 256);
        _config_hid_interrupt_receive_handles[i] = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
    }
    
    _config_hid_queue_transfers(); //queue as many transfers as possible
    
    _config_hid_initialized = true;
}

void CONFIG_HID_Tasks() {
    if (!_config_hid_initialized) return;
    
    if (_config_hid_requested_writes > 0) {
        if (_config_hid_requested_writes & 0x01) APP_CONFIG_Write(APP_CONFIG_FAN_CURVE);
        if (_config_hid_requested_writes & 0x02) APP_CONFIG_Write(APP_CONFIG_PUMP_CURVE);
        if (_config_hid_requested_writes & 0x0c) APP_CONFIG_Write(APP_CONFIG_WATCHDOG);
        if (_config_hid_requested_writes & 0x30) APP_CONFIG_Write(APP_CONFIG_COOLING_ACCEPTABLE);
        if (_config_hid_requested_writes & 0xc0) APP_CONFIG_Write(APP_CONFIG_COOLING_NOMINAL);
        
        _config_hid_requested_writes = 0;
    }
}


/* *****************************************************************************
 End of File
 */
