/* ************************************************************************** */


#ifndef _APP_CONFIG_H    /* Guard against multiple inclusion */
#define _APP_CONFIG_H


#include <stdint.h>
#include <stdbool.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        APP_CONFIG_ALL = 0x000,
        APP_CONFIG_INITIALIZED = 0x001,
        APP_CONFIG_FAN_CURVE = 0x010,
        APP_CONFIG_PUMP_CURVE = 0x020,
        APP_CONFIG_WATCHDOG = 0x030,
        APP_CONFIG_COOLING_ACCEPTABLE = 0x038,
        APP_CONFIG_COOLING_NOMINAL = 0x040,
        APP_CONFIG_ARGB_EFFECT = 0x100,
    } APP_CONFIG_SECTION;
    
    
    uint32_t config_fan_curve_point_count;
    float config_fan_curve_temperatures[12];
    uint8_t config_fan_curve_values[12];
    
    uint32_t config_pump_curve_point_count;
    float config_pump_curve_temperatures[12];
    uint8_t config_pump_curve_values[12];
    
    uint32_t config_watchdog_interval_seconds;
    uint32_t config_watchdog_runtime_seconds;
    
    //order to fit coolctl: flow - pump - fan - temp
    float config_cooling_acceptable_min[4];
    float config_cooling_acceptable_max[4];
    float config_cooling_nominal_min[4];
    float config_cooling_nominal_max[4];
    
    uint32_t config_argb_effect_index;
    uint32_t config_argb_effect_params[7];
    
    
    void APP_CONFIG_Init();
    void APP_CONFIG_Write(APP_CONFIG_SECTION section);
    
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
