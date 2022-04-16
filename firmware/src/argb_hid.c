/* ************************************************************************** */


#include <string.h>

#include "argb_hid.h"
#include "argb_positions.h"
#include "usb/usb_device_hid.h"
#include "app.h"
#include "system/debug/sys_debug.h"
#include "system/time/sys_time.h"
#include "argb_effect.h"



static const uint8_t _argb_hid_array_attrib_report[] = {
    (uint8_t)ARGB_HID_REPORT_ARRAY_ATTRIB,              //Report ID
    (uint8_t)(ARGB_HID_LAMPCOUNT & 0xff),
    (uint8_t)((ARGB_HID_LAMPCOUNT >> 8) & 0xff),        //Lamp Count
    0xB0, 0x1E, 0x04, 0x00,                             //BB Width (270000 um)
    0x20, 0xA1, 0x07, 0x00,                             //BB Height (500000 um)
    0x20, 0xA1, 0x07, 0x00,                             //BB Depth (500000 um)
    0x07, 0x00, 0x00, 0x00,                             //Array kind: Chassis
    (uint8_t)((2 * ARGB_FRAMETIME_US) & 0xff),
    (uint8_t)(((2 * ARGB_FRAMETIME_US) >> 8) & 0xff),
    (uint8_t)(((2 * ARGB_FRAMETIME_US) >> 16) & 0xff),
    (uint8_t)(((2 * ARGB_FRAMETIME_US) >> 24) & 0xff)   //Min update interval = frame time * 2 (otherwise overloaded it seems)
};

static const uint8_t _argb_hid_lamp_attrib_template[] = {
    (uint8_t)ARGB_HID_REPORT_LAMP_ATTRIB_RES,   //Report ID
    0x00, 0x00,                                 //Placeholder (+1): Lamp ID
    0x00, 0x00, 0x00, 0x00,                     //Placeholder (+3): X Position
    0x00, 0x00, 0x00, 0x00,                     //Placeholder (+7): Y Position
    0x00, 0x00, 0x00, 0x00,                     //Placeholder (+11): Z Position
    0x02, 0x00, 0x00, 0x00,                     //Lamp purposes: Accent
    0x10, 0x27, 0x00, 0x00,                     //Update Latency (10000us)
    0xff,                                       //Red level count: 255
    0xff,                                       //Green level count: 255
    0xff,                                       //Blue level count: 255
    0x01,                                       //Intensity level count: 1
    0x01,                                       //Is programmable: Yes
    0x00                                        //Input binding: None
};

static const uint8_t _argb_hid_array_control_template[] = {
    (uint8_t)ARGB_HID_REPORT_ARRAY_CONTROL, //Report ID
    0x00                                    //Placeholder (+1): Autonomous Mode
};


bool argb_hid_autonomous_mode = true;


static uint8_t _argb_hid_ctrl_send_buffer[32] = { 0 };
static uint8_t _argb_hid_ctrl_receive_buffer[ARGB_HID_RECEIVE_BUFFER_SIZE] = { 0 };

static USB_DEVICE_HID_TRANSFER_HANDLE _argb_hid_interrupt_receive_handles[ARGB_HID_REQUEST_COUNT] = { 0 };
static uint8_t _argb_hid_interrupt_receive_buffers[ARGB_HID_REQUEST_COUNT][ARGB_HID_RECEIVE_BUFFER_SIZE] = { { 0 } };

static uint16_t _argb_hid_lamp_attrib_id = 0;

static ARGB_HID_REPORT_ID _argb_hid_ctrl_report_receiving = ARGB_HID_REPORT_UNDEFINED;
static uint8_t _argb_hid_ctrl_report_receiving_length = 0;

static USB_HID_PROTOCOL_CODE _argb_hid_protocolcode = 1;

static uint16_t _argb_hid_update_queue_length = 0;
static uint16_t _argb_hid_update_queue_indices[ARGB_HID_QUEUE_MAX_LENGTH] = { 0 };
static uint32_t _argb_hid_update_queue_colors[ARGB_HID_QUEUE_MAX_LENGTH] = { 0 };

static uint32_t _argb_hid_update_bulk_array[ARGB_HID_LAMPCOUNT] = { 0 };


void _argb_hid_handle_get_report(USB_DEVICE_HID_EVENT_DATA_GET_REPORT* data) {
    if (data->reportType == 0x03) { //only feature reports can be requested
        switch (data->reportID) {
            case ARGB_HID_REPORT_ARRAY_ATTRIB:
                USB_DEVICE_ControlSend(appData.usbHandle, (uint8_t*)_argb_hid_array_attrib_report, sizeof(_argb_hid_array_attrib_report));
                return;
            case ARGB_HID_REPORT_LAMP_ATTRIB_RES:
                memcpy(_argb_hid_ctrl_send_buffer, _argb_hid_lamp_attrib_template, sizeof(_argb_hid_lamp_attrib_template));
                memcpy(_argb_hid_ctrl_send_buffer + 1, &_argb_hid_lamp_attrib_id, sizeof(uint16_t));
                uint16_t section = _argb_hid_lamp_attrib_id / ARGB_MAX_LENGTH;
                uint16_t id_in_section = _argb_hid_lamp_attrib_id % ARGB_MAX_LENGTH;
                memcpy(_argb_hid_ctrl_send_buffer + 3, argb_positions[section] + (3 * id_in_section), 3 * sizeof(uint32_t));
                _argb_hid_lamp_attrib_id = (_argb_hid_lamp_attrib_id + 1) % ARGB_HID_LAMPCOUNT;
                USB_DEVICE_ControlSend(appData.usbHandle, _argb_hid_ctrl_send_buffer, sizeof(_argb_hid_lamp_attrib_template));
                return;
            case ARGB_HID_REPORT_ARRAY_CONTROL:
                memcpy(_argb_hid_ctrl_send_buffer, _argb_hid_array_control_template, sizeof(_argb_hid_array_control_template));
                memcpy(_argb_hid_ctrl_send_buffer + 1, &argb_hid_autonomous_mode, sizeof(uint8_t));
                USB_DEVICE_ControlSend(appData.usbHandle, _argb_hid_ctrl_send_buffer, sizeof(_argb_hid_array_control_template));
                return;
            default:
                break;
        }
    }
    USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR); //fallback for all other cases: error
}

void _argb_hid_handle_set_report(USB_DEVICE_HID_EVENT_DATA_SET_REPORT* data) {
    if (data->reportType == 0x03) { //feature report
        if (data->reportID == ARGB_HID_REPORT_LAMP_ATTRIB_REQ || data->reportID == ARGB_HID_REPORT_ARRAY_CONTROL) {
            _argb_hid_ctrl_report_receiving = data->reportID;
            _argb_hid_ctrl_report_receiving_length = data->reportLength;
            USB_DEVICE_ControlReceive(appData.usbHandle, _argb_hid_ctrl_receive_buffer, min(data->reportLength, sizeof(_argb_hid_ctrl_receive_buffer)));
            return;
        }
    } else if (data->reportType == 0x02) { //output report
        if (data->reportID == ARGB_HID_REPORT_LAMP_MULTI_UPDATE || data->reportID == ARGB_HID_REPORT_LAMP_RANGE_UPDATE || data->reportID == ARGB_HID_REPORT_LAMP_BULK_UPDATE) {
            _argb_hid_ctrl_report_receiving = data->reportID;
            _argb_hid_ctrl_report_receiving_length = data->reportLength;
            USB_DEVICE_ControlReceive(appData.usbHandle, _argb_hid_ctrl_receive_buffer, min(data->reportLength, sizeof(_argb_hid_ctrl_receive_buffer)));
            return;
        }
    }
    USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR); //fallback for all other cases: error
}

void _argb_hid_apply_queued_updates() {
    uint32_t* color_buffer = argb_colors[0]; //use "first" buffer pointer, as the buffer is contiguous, so indices just continue
    
    if (!argb_hid_autonomous_mode) { //do not allow updates in autonomous mode
        uint16_t i;
        for (i = 0; i < _argb_hid_update_queue_length; i++) { //apply updates themselves
            color_buffer[_argb_hid_update_queue_indices[i]] = _argb_hid_update_queue_colors[i];
        }
    }
    
    _argb_hid_update_queue_length = 0; //reset queue to empty
}

void _argb_hid_handle_report_received(uint8_t* buffer, uint8_t length, bool from_ctrl) {
    uint16_t buffer_16b[16];
    bool success = false;
    uint16_t i;
    
    if (length == 0) return;
    uint8_t reportId = buffer[0];

    if (from_ctrl && (_argb_hid_ctrl_report_receiving == ARGB_HID_REPORT_UNDEFINED
            || _argb_hid_ctrl_report_receiving != reportId || _argb_hid_ctrl_report_receiving_length != length)) {
        USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
        SYS_DEBUG_PRINT(SYS_ERROR_WARNING, "ARGB HID received ctrl report %u (%u B) instead of %u (%u B)", reportId, length, _argb_hid_ctrl_report_receiving, _argb_hid_ctrl_report_receiving_length);
        return;
    }
    
    switch (reportId) {
        case ARGB_HID_REPORT_LAMP_ATTRIB_REQ:
            if (length != 3) break;
            memcpy(buffer_16b, buffer + 1, 2);
            if (buffer_16b[0] < ARGB_HID_LAMPCOUNT) {
                _argb_hid_lamp_attrib_id = buffer_16b[0]; //set lamp attribute index (for next attribute request)
                success = true;
            }
            break;
        case ARGB_HID_REPORT_ARRAY_CONTROL:
            if (length != 2) break;
            if (buffer[1] < 2) {
                argb_hid_autonomous_mode = (bool)buffer[1]; //update autonomous mode status
                if (argb_hid_autonomous_mode) argb_effect_update_requested = true;
                success = true;
            }
            break;
        case ARGB_HID_REPORT_LAMP_MULTI_UPDATE:
            if (length != 99) break;
            uint8_t count = buffer[1]; //number of updated lights
            
            uint8_t* buffer_color = buffer + 34; //offset receive buffer pointer for colors
            uint8_t* queue_color_8 = (uint8_t*)(_argb_hid_update_queue_colors + _argb_hid_update_queue_length); //offset queue buffer pointer for colors, in bytes
            
            success = true; //failure after this point should result in an ignored message, not an error response
            bool ignore = false; //assume success
            
            if (_argb_hid_update_queue_length >= ARGB_HID_QUEUE_MAX_LENGTH - 16) {
                _argb_hid_apply_queued_updates();
            }
            
            uint16_t queue_len_at_start = _argb_hid_update_queue_length; //starting queue length, for potentially ignoring message
            
            if (count >= 1 && count <= 16) { //only proceed with legal update count
                memcpy(buffer_16b, buffer + 2, count * 2); //get 16-bit light indices
                
                for (i = 0; i < count; i++) { //add all updates to queue
                    if (buffer_16b[i] >= ARGB_HID_LAMPCOUNT) { //illegal index: stop and ignore entire message
                        ignore = true;
                        break;
                    }
                    
                    _argb_hid_update_queue_indices[_argb_hid_update_queue_length++] = buffer_16b[i]; //save index in queue
                    
                    queue_color_8[0] = buffer_color[2]; //blue byte
                    queue_color_8[1] = buffer_color[1]; //green byte
                    queue_color_8[2] = buffer_color[0]; //red byte
                    queue_color_8[3] = buffer_color[3]; //intensity byte
                    
                    buffer_color += 4; //increment pointers for colors
                    queue_color_8 += 4;
                }
            }
            
            if (ignore) { //if should be ignored: reset queue to ignore entire message
                _argb_hid_update_queue_length = queue_len_at_start;
                break;
            }
            
            if (buffer[98] > 0 || _argb_hid_update_queue_length >= ARGB_HID_QUEUE_MAX_LENGTH - 16) { //update finished or queue (almost) full: apply update
                _argb_hid_apply_queued_updates();
            }
            break;
        case ARGB_HID_REPORT_LAMP_RANGE_UPDATE:
            if (length != 10) break;
            memcpy(buffer_16b, buffer + 1, 4); //get 16-bit start and end indices
            
            success = true; //failure after this point should result in an ignored message, not an error response
            
            if (buffer_16b[0] >= ARGB_HID_LAMPCOUNT || buffer_16b[1] >= ARGB_HID_LAMPCOUNT || buffer_16b[0] > buffer_16b[1]) break; //ignore illegal update
            
            uint32_t update_color_value = //construct IIRRGGBB color value
                    ((uint32_t)buffer[8] << 24) |
                    ((uint32_t)buffer[5] << 16) |
                    ((uint32_t)buffer[6] << 8) |
                    (uint32_t)buffer[7];
            
            if (_argb_hid_update_queue_length >= ARGB_HID_QUEUE_MAX_LENGTH - (buffer_16b[1] - buffer_16b[0] + 1)) { //if queue too full: apply previous update
                _argb_hid_apply_queued_updates();
            }
            
            uint16_t i;
            for (i = buffer_16b[0]; i <= buffer_16b[1]; i++) { //update entire range
                _argb_hid_update_queue_indices[_argb_hid_update_queue_length] = i;
                _argb_hid_update_queue_colors[_argb_hid_update_queue_length++] = update_color_value;
            }
            
            if (buffer[9] > 0 || _argb_hid_update_queue_length >= ARGB_HID_QUEUE_MAX_LENGTH - 16) { //update finished or queue (almost) full: apply update
                _argb_hid_apply_queued_updates();
            }
            break;
        case ARGB_HID_REPORT_LAMP_BULK_UPDATE:
            if (length != 69) break;
            success = true; //failure after this point should result in an ignored message, not an error response
            
            memcpy(buffer_16b, buffer + 1, 2); //get start ID
            uint16_t start_id = buffer_16b[0];
            uint16_t count2 = buffer[3];
            
            if (count2 == 0 || count2 > 16 || start_id + count2 > ARGB_HID_LAMPCOUNT) break; //invalid ID(s) or count
            
            memcpy(_argb_hid_update_bulk_array + start_id, buffer + 4, count2 * sizeof(uint32_t));
            
            if (buffer[68] > 0) memcpy(argb_colors[0], _argb_hid_update_bulk_array, ARGB_HID_LAMPCOUNT * sizeof(uint32_t));
            break;
        default:
            break;
    }
    
    if (from_ctrl) {
        if (success) USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_OK);
        else USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
        
        _argb_hid_ctrl_report_receiving = ARGB_HID_REPORT_UNDEFINED;
        _argb_hid_ctrl_report_receiving_length = 0;
    }
}

void _argb_hid_queue_transfers() {
    uint8_t i;
    USB_DEVICE_HID_RESULT result;
    for (i = 0; i < ARGB_HID_REQUEST_COUNT; i++) {
        if (_argb_hid_interrupt_receive_handles[i] != USB_DEVICE_HID_TRANSFER_HANDLE_INVALID) continue; //leave valid handles, request already present
        result = USB_DEVICE_HID_ReportReceive(USB_DEVICE_HID_INDEX_0, _argb_hid_interrupt_receive_handles + i, _argb_hid_interrupt_receive_buffers + i, ARGB_HID_RECEIVE_BUFFER_SIZE);
        if (result != USB_DEVICE_HID_RESULT_OK) break; //stop requesting once an error is encountered
    }
}

void _argb_hid_USBDeviceHIDEventHandler(USB_DEVICE_HID_INDEX index, USB_DEVICE_HID_EVENT event, void* pData, uintptr_t context) {
    USB_DEVICE_HID_EVENT_DATA_SET_PROTOCOL* set_protocol_data;
    USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED* report_received_data;
    
    switch (event) {
        case USB_DEVICE_HID_EVENT_GET_REPORT:
            _argb_hid_handle_get_report((USB_DEVICE_HID_EVENT_DATA_GET_REPORT*)pData);
            break;
        case USB_DEVICE_HID_EVENT_SET_REPORT:
            _argb_hid_handle_set_report((USB_DEVICE_HID_EVENT_DATA_SET_REPORT*)pData);
            break;
        case USB_DEVICE_HID_EVENT_GET_PROTOCOL:
            USB_DEVICE_ControlSend(appData.usbHandle, &_argb_hid_protocolcode, 1);
            break;
        case USB_DEVICE_HID_EVENT_SET_PROTOCOL:
            set_protocol_data = (USB_DEVICE_HID_EVENT_DATA_SET_PROTOCOL*)pData;
            USB_DEVICE_ControlStatus(appData.usbHandle, set_protocol_data->protocolCode == _argb_hid_protocolcode ? USB_DEVICE_CONTROL_STATUS_OK : USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
        case USB_DEVICE_HID_EVENT_GET_IDLE:
            USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
        case USB_DEVICE_HID_EVENT_SET_IDLE:
            USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_ERROR);
            break;
        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT:
            USB_DEVICE_ControlStatus(appData.usbHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;
        case USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:
            _argb_hid_handle_report_received(_argb_hid_ctrl_receive_buffer, _argb_hid_ctrl_report_receiving_length, true);
            break;
        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:
            report_received_data = (USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED*)pData;
            
            uint8_t i;
            bool found = false;
            for (i = 0; i < ARGB_HID_REQUEST_COUNT; i++) { //search for relevant handle in array
                if (_argb_hid_interrupt_receive_handles[i] == report_received_data->handle) {
                    found = true;
                    _argb_hid_interrupt_receive_handles[i] = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID; //slot is now empty
                    break;
                }
            }
            if (!found) break;
            
            if (report_received_data->status == USB_DEVICE_HID_RESULT_OK) { //handle received data if successful
                _argb_hid_handle_report_received(_argb_hid_interrupt_receive_buffers[i], report_received_data->length, false);
            }
            
            _argb_hid_queue_transfers(); //queue more transfers
            break;
        default:
            break;
    }
}

void ARGB_HID_Init() {
    argb_hid_autonomous_mode = true;
    _argb_hid_lamp_attrib_id = 0;
    _argb_hid_ctrl_report_receiving = ARGB_HID_REPORT_UNDEFINED;
    _argb_hid_ctrl_report_receiving_length = 0;
    
    uint8_t i;
    for (i = 0; i < ARGB_HID_REQUEST_COUNT; i++) { //initialize transfer arrays
        memset(_argb_hid_interrupt_receive_buffers[i], 0, ARGB_HID_RECEIVE_BUFFER_SIZE);
        _argb_hid_interrupt_receive_handles[i] = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
    }
    
    _argb_hid_queue_transfers(); //queue as many transfers as possible
}




/* *****************************************************************************
 End of File
 */
