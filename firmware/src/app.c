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


/* TODO:  Add any necessary local functions.
*/


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

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    iter_start_time = 0;
    frame_ended = false;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void cb(uintptr_t context) {
    static int16_t r = 2047;
    static int16_t g = 0;
    static int16_t b = 0;
    
    if(r > 0 && b == 0){
        r--;
        g++;
    }
    if(g > 0 && r == 0){
        g--;
        b++;
    }
    if(b > 0 && g == 0){
        r++;
        b--;
    }
    
    OCMP4_CompareSecondaryValueSet(r);
    OCMP5_CompareSecondaryValueSet(g);
    OCMP6_CompareSecondaryValueSet(b);
}

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
        argb_a_colors[i] = argb_a_colors[i - 1];
    }
    argb_a_colors[0] = (g << 16) | (r << 8) | b;
}

void APP_Tasks ( void )
{

    iter_start_time = SYS_TIME_Counter64Get();
    
    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;

            /*TMR3_Start();
            OCMP4_Enable();
            OCMP5_Enable();
            OCMP6_Enable();
            SYS_TIME_CallbackRegisterMS(cb, 0, 1, SYS_TIME_PERIODIC);*/
            
            APP_POWER_Init();
            ARGB_Init();
            ARGB_SetFrameCallback(frame_end_cb, NULL);

            if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {

            APP_POWER_Tasks();
            
            if (frame_ended) {
                frame_ended = false;
                newframe();
            }
            
            ARGB_Tasks();
            
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
