/* ************************************************************************** */


#include "emergency.h"
#include "peripheral/gpio/plib_gpio.h"
#include "system/debug/sys_debug.h"



static uint32_t _emergency_warning_sources = 0;


void EMERGENCY_Init() {
    EMERGENCY_Reset();
}

void EMERGENCY_WarningEnable(EMERGENCY_SOURCE source) {
    _emergency_warning_sources |= (uint32_t)source;
    BUZZER_Set();
    SYS_DEBUG_PRINT(SYS_ERROR_WARNING, "\nEmergency warning signal from source %x\n", source);
}

void EMERGENCY_WarningDisable(EMERGENCY_SOURCE source) {
    _emergency_warning_sources &= ~((uint32_t)source);
    if (_emergency_warning_sources == 0) BUZZER_Clear();
}

void EMERGENCY_Shutdown() {
    EMERGENCY_SD_Set();
    BUZZER_Set();
    SYS_DEBUG_MESSAGE(SYS_ERROR_FATAL, "\n---- EMERGENCY SHUTDOWN TRIGGERED ----\n");
}

void EMERGENCY_Reset() {
    _emergency_warning_sources = 0;
    BUZZER_Clear();
    EMERGENCY_SD_Clear();
}



/* *****************************************************************************
 End of File
 */
