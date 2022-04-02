/* ************************************************************************** */


#ifndef _APP_POWER_H    /* Guard against multiple inclusion */
#define _APP_POWER_H

#include "config/default/peripheral/gpio/plib_gpio.h"



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdint.h>


#define APP_POWER_Main() app_power_main
#define APP_POWER_Watchdog() app_power_watchdog
#define APP_POWER_Pump() (APP_POWER_Main() || APP_POWER_Watchdog())


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    bool app_power_main;
    bool app_power_watchdog;
    
    void APP_POWER_Init();
    void APP_POWER_Tasks();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
