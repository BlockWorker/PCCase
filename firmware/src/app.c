/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "system/time/sys_time.h"
#include "peripheral/gpio/plib_gpio.h"
#include "peripheral/tmr/plib_tmr3.h"
#include "peripheral/ocmp/plib_ocmp4.h"
#include "peripheral/ocmp/plib_ocmp5.h"
#include "peripheral/ocmp/plib_ocmp6.h"
#include "argb.h"
#include "peripheral/ocmp/plib_ocmp1.h"
#include "peripheral/ocmp/plib_ocmp2.h"
#include "peripheral/ocmp/plib_ocmp3.h"
#include "peripheral/tmr/plib_tmr5.h"
#include "app_power.h"
#include "usb/usb_device_hid.h"
#include "argb_hid.h"
#include "system/console/sys_console.h"
#include "system/debug/sys_debug.h"
#include "peripheral/tmr/plib_tmr4.h"
#include "cooling_control.h"
#include "app_config.h"
#include "config_hid.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

uint64_t iter_start_time = 0;


static bool frame_ended = false;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

void _argb_hid_USBDeviceHIDEventHandler(USB_DEVICE_HID_INDEX index, USB_DEVICE_HID_EVENT event, void * pData, uintptr_t context);
void _config_hid_USBDeviceHIDEventHandler(USB_DEVICE_HID_INDEX index, USB_DEVICE_HID_EVENT event, void * pData, uintptr_t context);

void _app_USBDeviceEventCallBack(USB_DEVICE_EVENT event, void * eventData, uintptr_t context) {
    switch (event) {
        case USB_DEVICE_EVENT_POWER_DETECTED:
            USB_DEVICE_Attach(appData.usbHandle);
            break;
        case USB_DEVICE_EVENT_POWER_REMOVED:
            USB_DEVICE_Detach(appData.usbHandle);
            break;
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:
            break;
        case USB_DEVICE_EVENT_CONFIGURED:
            if (*(uint8_t*)eventData == 1) {
                USB_DEVICE_HID_EventHandlerSet(USB_DEVICE_HID_INDEX_0, _argb_hid_USBDeviceHIDEventHandler, NULL);
                USB_DEVICE_HID_EventHandlerSet(USB_DEVICE_HID_INDEX_1, _config_hid_USBDeviceHIDEventHandler, NULL);
                ARGB_HID_Init();
                CONFIG_HID_Init();
                CONFIG_HID_SendUpdate();
            }
            break;
        case USB_DEVICE_EVENT_SUSPENDED:
            break;
        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize() {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT_START;

    iter_start_time = 0;
    frame_ended = false;
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void frame_end_cb(uintptr_t context) {
    frame_ended = true;
}

void newframe() {
    static int32_t r = 250;
    static int32_t g = 0;
    static int32_t b = 0;
    
    if(r > 0 && b == 0){
        r -= 10;
        g += 10;
    }
    if(g > 0 && r == 0){
        g -= 10;
        b += 10;
    }
    if(b > 0 && g == 0){
        r += 10;
        b -= 10;
    }
    
    uint32_t i;
    for (i = ARGB_MAX_LENGTH - 1; i > 0; i--) {
        argb_colors[0][i] = argb_colors[0][i - 1];
    }
    argb_colors[0][0] = 0x01000000u | (g << 16) | (r << 8) | b;
}

void APP_Tasks() {
    iter_start_time = SYS_TIME_Counter64Get();
    
    /* Check the application's current state. */
    switch (appData.state) {
        /* Application's initial state. */
        case APP_STATE_INIT_START:
            APP_CONFIG_Init();
            
            APP_POWER_Init();
            ARGB_Init();
            ARGB_SetFrameCallback(frame_end_cb, NULL);
            COOLCTL_Init();

            appData.state = APP_STATE_INIT_USB;
            break;
        case APP_STATE_INIT_USB:
            appData.usbHandle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);
            
            if (appData.usbHandle != USB_DEVICE_HANDLE_INVALID) {
                USB_DEVICE_EventHandlerSet(appData.usbHandle, _app_USBDeviceEventCallBack, NULL);
                USB_DEVICE_Attach(appData.usbHandle);
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            
            break;
        case APP_STATE_SERVICE_TASKS:
            APP_POWER_Tasks();
            
            if (frame_ended) {
                frame_ended = false;
                if (argb_hid_autonomous_mode) newframe();
            }
            
            ARGB_Tasks();
            
            COOLCTL_Tasks();
            
            CONFIG_HID_Tasks();
            
            break;
        default:
            break;
    }
}


/*******************************************************************************
 End of File
 */
