/* ************************************************************************** */


#ifndef _COOLING_CONTROL_H    /* Guard against multiple inclusion */
#define _COOLING_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

#define COOLCTL_UPDATE_INTERVAL_MS 2000

#define COOLCTL_SECONDS_PER_TICK (1.0f / 1953.0f)

#define COOLCTL_FAN_RPM_PER_HZ (60.0f / 2.0f)
#define COOLCTL_PUMP_RPM_PER_HZ (60.0f / 2.0f)
#define COOLCTL_FLOW_LPH_PER_HZ (3600.0f / 169.0f)

#define COOLCTL_VOLTS_PER_COUNT (3.0f / 65535.0f)
#define COOLCTL_PULLUP_VALUE_OHMS 4697.0f
#define COOLCTL_NTC_B 3450.0f
#define COOLCTL_NTC_RINF 0.09432431f

#define COOLCTL_FAN_STARTUP_NOMINAL_RPM_MIN 1000.0f
#define COOLCTL_FAN_STARTUP_NOMINAL_RPM_MAX 4000.0f
#define COOLCTL_PUMP_STARTUP_NOMINAL_RPM_MIN 1000.0f
#define COOLCTL_PUMP_STARTUP_NOMINAL_RPM_MAX 6000.0f
#define COOLCTL_FLOW_STARTUP_NOMINAL_LPH_MIN 15.0f
#define COOLCTL_FLOW_STARTUP_NOMINAL_LPH_MAX 1000.0f
#define COOLCTL_TEMP_STARTUP_NOMINAL_C_MIN 4.0f
#define COOLCTL_TEMP_STARTUP_NOMINAL_C_MAX 60.0f
#define COOLCTL_STARTUP_MAX_COUNT 5

#define COOLCTL_UNACCEPTABLE_MAX_COUNT 4
#define COOLCTL_WARN_MAX_COUNT 4

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    float coolctl_fan_rpm;
    float coolctl_pump_rpm;
    float coolctl_flow_lph;
    float coolctl_temp;
    
    
    void COOLCTL_Init();
    void COOLCTL_Tasks();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
