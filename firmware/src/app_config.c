/* ************************************************************************** */


#include "app_config.h"
#include "dee_emulation_pic32.h"
#include "system/debug/sys_debug.h"



uint32_t config_fan_curve_point_count = 0;
float config_fan_curve_temperatures[12] = { 0.0f };
uint8_t config_fan_curve_values[12] = { 0 };

uint32_t config_pump_curve_point_count = 0;
float config_pump_curve_temperatures[12] = { 0.0f };
uint8_t config_pump_curve_values[12] = { 0 };

uint32_t config_watchdog_interval_seconds = 0;
uint32_t config_watchdog_runtime_seconds = 0;

//order to fit coolctl: flow - pump - fan - temp
float config_cooling_acceptable_min[4] = { 0.0f };
float config_cooling_acceptable_max[4] = { 0.0f };
float config_cooling_nominal_min[4] = { 0.0f };
float config_cooling_nominal_max[4] = { 0.0f };

uint32_t config_argb_effect_index = 0;
uint32_t config_argb_effect_params[7] = { 0 };


void APP_CONFIG_Init() {
    uint32_t initReadBuffer = 0;
    uint32_t error = 0;
    uint8_t i;
    
    error = DataEEInit();
    if (!error) error = DataEERead(&initReadBuffer, APP_CONFIG_INITIALIZED);
    else SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Data EE Init error %u\n", error);
    
    if (!error && initReadBuffer == 0x42) { //attempt read
        do { //fake loop to allow break
            if ((error = DataEERead(&config_fan_curve_point_count, APP_CONFIG_FAN_CURVE))) break;
            if ((error = DataEERead(&config_pump_curve_point_count, APP_CONFIG_PUMP_CURVE))) break;
            for (i = 0; i < 12; i++) {
                if ((error = DataEERead((uint32_t*)(config_fan_curve_temperatures + i), APP_CONFIG_FAN_CURVE + i + 1))) break;
                if ((error = DataEERead((uint32_t*)(config_pump_curve_temperatures + i), APP_CONFIG_PUMP_CURVE + i + 1))) break;
            }
            if (error) break;
            if ((error = DataEEReadArray(config_fan_curve_values, APP_CONFIG_FAN_CURVE + 13, 12))) break;
            if ((error = DataEEReadArray(config_pump_curve_values, APP_CONFIG_PUMP_CURVE + 13, 12))) break;
            
            if ((error = DataEERead(&config_watchdog_interval_seconds, APP_CONFIG_WATCHDOG))) break;
            if ((error = DataEERead(&config_watchdog_runtime_seconds, APP_CONFIG_WATCHDOG + 1))) break;
            
            for (i = 0; i < 4; i++) {
                if ((error = DataEERead((uint32_t*)(config_cooling_acceptable_min + i), APP_CONFIG_COOLING_ACCEPTABLE + i))) break;
                if ((error = DataEERead((uint32_t*)(config_cooling_acceptable_max + i), APP_CONFIG_COOLING_ACCEPTABLE + i + 4))) break;
                if ((error = DataEERead((uint32_t*)(config_cooling_nominal_min + i), APP_CONFIG_COOLING_NOMINAL + i))) break;
                if ((error = DataEERead((uint32_t*)(config_cooling_nominal_max + i), APP_CONFIG_COOLING_NOMINAL + i + 4))) break;
            }
            
            if ((error = DataEERead(&config_argb_effect_index, APP_CONFIG_ARGB_EFFECT))) break;
            for (i = 0; i < 7; i++) {
                if ((error = DataEERead(config_argb_effect_params + i, APP_CONFIG_ARGB_EFFECT + i + 1))) break;
            }
        } while (false);
        
        if (error) SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Data EE initial full read error %u\n", error);
    }
    
    if (error || initReadBuffer != 0x42) { //error or not yet initialized: use and write default values
        SYS_DEBUG_PRINT(SYS_ERROR_WARNING, "Data EE writing default values, error code %u\n", error);
        
        config_fan_curve_point_count = 0;
        config_pump_curve_point_count = 0;
        for (i = 0; i < 12; i++) {
            config_fan_curve_temperatures[i] = 0.0f;
            config_fan_curve_values[i] = 0;
            config_pump_curve_temperatures[i] = 0.0f;
            config_pump_curve_values[i] = 0;
        }
        config_watchdog_interval_seconds = 28800;
        config_watchdog_runtime_seconds = 120;
        config_cooling_acceptable_min[0] = 10.0f;
        config_cooling_acceptable_min[1] = 100.0f;
        config_cooling_acceptable_min[2] = 100.0f;
        config_cooling_acceptable_min[3] = 4.0f;
        config_cooling_acceptable_max[0] = 1000.0f;
        config_cooling_acceptable_max[1] = 5000.0f;
        config_cooling_acceptable_max[2] = 4000.0f;
        config_cooling_acceptable_max[3] = 60.0f;
        config_cooling_nominal_min[0] = 20.0f;
        config_cooling_nominal_min[1] = 300.0f;
        config_cooling_nominal_min[2] = 300.0f;
        config_cooling_nominal_min[3] = 10.0f;
        config_cooling_nominal_max[0] = 800.0f;
        config_cooling_nominal_max[1] = 4000.0f;
        config_cooling_nominal_max[2] = 3000.0f;
        config_cooling_nominal_max[3] = 50.0f;
        config_argb_effect_index = 0;
        for (i = 0; i < 7; i++) {
            config_argb_effect_params[i] = 0;
        }
        
        APP_CONFIG_Write(APP_CONFIG_ALL);
    }
}

void APP_CONFIG_Write(APP_CONFIG_SECTION section) {
    bool all = (section == APP_CONFIG_ALL);
    uint32_t error = 0;
    uint8_t i;
    
    switch (section) {
        case APP_CONFIG_ALL:
        case APP_CONFIG_INITIALIZED:
            error = DataEEWrite(0x42, APP_CONFIG_INITIALIZED);
            if (!all || error) break;
        case APP_CONFIG_FAN_CURVE:
            error = DataEEWrite(config_fan_curve_point_count, APP_CONFIG_FAN_CURVE);
            for (i = 0; i < 12; i++) {
                if (error) break;
                error = DataEEWrite(*((uint32_t*)(config_fan_curve_temperatures + i)), APP_CONFIG_FAN_CURVE + i + 1);
            }
            if (!error) error = DataEEWriteArray(config_fan_curve_values, APP_CONFIG_FAN_CURVE + 13, 12);
            if (!all || error) break;
        case APP_CONFIG_PUMP_CURVE:
            error = DataEEWrite(config_pump_curve_point_count, APP_CONFIG_PUMP_CURVE);
            for (i = 0; i < 12; i++) {
                if (error) break;
                error = DataEEWrite(*((uint32_t*)(config_pump_curve_temperatures + i)), APP_CONFIG_PUMP_CURVE + i + 1);
            }
            if (!error) error = DataEEWriteArray(config_pump_curve_values, APP_CONFIG_PUMP_CURVE + 13, 12);
            if (!all || error) break;
        case APP_CONFIG_WATCHDOG:
            error = DataEEWrite(config_watchdog_interval_seconds, APP_CONFIG_WATCHDOG);
            if (!error) error = DataEEWrite(config_watchdog_runtime_seconds, APP_CONFIG_WATCHDOG + 1);
            if (!all || error) break;
        case APP_CONFIG_COOLING_ACCEPTABLE:
            for (i = 0; i < 4; i++) {
                if (error) break;
                error = DataEEWrite(*((uint32_t*)(config_cooling_acceptable_min + i)), APP_CONFIG_COOLING_ACCEPTABLE + i);
                if (error) break;
                error = DataEEWrite(*((uint32_t*)(config_cooling_acceptable_max + i)), APP_CONFIG_COOLING_ACCEPTABLE + i + 4);
            }
            if (!all || error) break;
        case APP_CONFIG_COOLING_NOMINAL:
            for (i = 0; i < 4; i++) {
                if (error) break;
                error = DataEEWrite(*((uint32_t*)(config_cooling_nominal_min + i)), APP_CONFIG_COOLING_NOMINAL + i);
                if (error) break;
                error = DataEEWrite(*((uint32_t*)(config_cooling_nominal_max + i)), APP_CONFIG_COOLING_NOMINAL + i + 4);
            }
            if (!all || error) break;
        case APP_CONFIG_ARGB_EFFECT:
            error = DataEEWrite(config_argb_effect_index, APP_CONFIG_ARGB_EFFECT);
            for (i = 0; i < 7; i++) {
                if (error) break;
                error = DataEEWrite(config_argb_effect_params[i], APP_CONFIG_ARGB_EFFECT + i + 1);
            }
            break;
        default:
            break;
    }
    
    if (error) SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Data EE write error %u\n", error);
}



/* *****************************************************************************
 End of File
 */
