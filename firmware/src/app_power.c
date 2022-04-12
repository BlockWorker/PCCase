/* ************************************************************************** */


#include "app_power.h"
#include "system/time/sys_time.h"
#include "app.h"
#include "system/debug/sys_debug.h"
#include "emergency.h"

bool app_power_main = false;
bool app_power_watchdog = false;

static uint64_t app_power_main_timeout = 0;
static uint64_t app_power_watchdog_timeout = 0;

static uint32_t app_power_timeout_delay = 0;


void _app_power_pin_callback(GPIO_PIN pin, uintptr_t context) {
    switch (pin) {
        case PWR_DETECT_PIN:
            if (app_power_main_timeout == 0 && PWR_DETECT_Get() != app_power_main) {
                app_power_main_timeout = iter_start_time + app_power_timeout_delay;
            }
            break;
        case WD_DETECT_PIN:
            if (app_power_watchdog_timeout == 0 && WD_DETECT_Get() != app_power_watchdog) {
                app_power_watchdog_timeout = iter_start_time + app_power_timeout_delay;
            }
            break;
        default:
            break;
    }
}


void APP_POWER_Init() {
    app_power_main = PWR_DETECT_Get();
    app_power_watchdog = WD_DETECT_Get();
    
    GPIO_PinInterruptCallbackRegister(PWR_DETECT_PIN, _app_power_pin_callback, NULL);
    GPIO_PinInterruptCallbackRegister(WD_DETECT_PIN, _app_power_pin_callback, NULL);
    
    app_power_main_timeout = 0;
    app_power_watchdog_timeout = 0;
    app_power_timeout_delay = SYS_TIME_USToCount(2000);
}

void APP_POWER_Tasks() {    
    if (app_power_main_timeout == 0) {
        if (PWR_DETECT_Get() != app_power_main) {
            app_power_main_timeout = iter_start_time + app_power_timeout_delay;
        }
    } else if (app_power_main_timeout < iter_start_time) {
        app_power_main = PWR_DETECT_Get();
        app_power_main_timeout = 0;
        if (!app_power_main) EMERGENCY_Reset();
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Power: Main %d\n", app_power_main);
    }
    
    if (app_power_watchdog_timeout == 0) {
        if (WD_DETECT_Get() != app_power_watchdog) {
            app_power_watchdog_timeout = iter_start_time + app_power_timeout_delay;
        }
    } else if (app_power_watchdog_timeout < iter_start_time) {
        app_power_watchdog = WD_DETECT_Get();
        app_power_watchdog_timeout = 0;
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Power: Watchdog %d\n", app_power_main);
    }
}



/* *****************************************************************************
 End of File
 */
