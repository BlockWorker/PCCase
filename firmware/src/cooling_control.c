/* ************************************************************************** */


#include "cooling_control.h"
#include "peripheral/tmr/plib_tmr4.h"
#include "peripheral/tmr/plib_tmr6.h"
#include "app_power.h"
#include "peripheral/ocmp/plib_ocmp7.h"
#include "peripheral/icap/plib_icap1.h"
#include "peripheral/icap/plib_icap2.h"
#include "peripheral/icap/plib_icap3.h"
#include "system/time/sys_time.h"
#include "app.h"
#include "system/debug/sys_debug.h"
#include "peripheral/ocmp/plib_ocmp8.h"
#include "peripheral/adchs/plib_adchs.h"
#include "emergency.h"
#include "app_config.h"
#include "config_hid.h"
#include <math.h>


float coolctl_fan_rpm = 0.0f;
float coolctl_pump_rpm = 0.0f;
float coolctl_flow_lph = 0.0f;
float coolctl_temp = 0.0f;


//order: flow - pump - fan (same as IC modules)
static uint16_t _coolctl_lastpulses[3] = { 0 };
static uint32_t _coolctl_period_sums[3] = { 0 };
static uint16_t _coolctl_pulse_counts[3] = { 0 };

static const float _coolctl_conversion_factors[3] = { COOLCTL_FLOW_LPH_PER_HZ, COOLCTL_PUMP_RPM_PER_HZ, COOLCTL_FAN_RPM_PER_HZ };
static float* const _coolctl_value_pointers[4] = { &coolctl_flow_lph, &coolctl_pump_rpm, &coolctl_fan_rpm, &coolctl_temp };

static uint64_t _coolctl_next_update = 0;
static uint32_t _coolctl_update_interval = 0;

static uint64_t _coolctl_next_watchdog_run = 0;
static uint64_t _coolctl_curr_watchdog_end = 0;
static uint64_t _coolctl_one_second_time = 0;

static bool _coolctl_running = false;
static bool _coolctl_wd_running = false;
static bool _coolctl_updating = false;
static bool _coolctl_wd_pump_on = false;

static int16_t _coolctl_startup_count = -1;

static uint8_t _coolctl_unacceptable_counter = 0;


void _coolctl_icap_callback(uintptr_t context) {    
    if (!_coolctl_running) {
        while (IC1CONbits.ICBNE) ICAP1_CaptureBufferRead();
        while (IC2CONbits.ICBNE) ICAP2_CaptureBufferRead();
        while (IC3CONbits.ICBNE) ICAP3_CaptureBufferRead();
        return;
    }
    
    uint16_t cap_value;
    switch (context) {
        case 0:
            cap_value = ICAP1_CaptureBufferRead();
            break;
        case 1:
            cap_value = ICAP2_CaptureBufferRead();
            break;
        case 2:
            cap_value = ICAP3_CaptureBufferRead();
            break;
        default:
            return;
    }
    
    if (_coolctl_lastpulses[context] > 0 && !_coolctl_updating) {
        uint16_t diff = cap_value - _coolctl_lastpulses[context];
        _coolctl_period_sums[context] += diff;
        _coolctl_pulse_counts[context]++;
    }
    _coolctl_lastpulses[context] = cap_value;
    
    while (IC1CONbits.ICBNE) ICAP1_CaptureBufferRead();
    while (IC2CONbits.ICBNE) ICAP2_CaptureBufferRead();
    while (IC3CONbits.ICBNE) ICAP3_CaptureBufferRead();
}

void _coolctl_adcfilter_callback(uintptr_t context) {
    uint16_t data = ADCHS_Filter1DataGet();
    
    float voltage = (float)data * COOLCTL_VOLTS_PER_COUNT;
    if (voltage < 0.3f || voltage > 2.7f) return;
    
    float resistance = COOLCTL_PULLUP_VALUE_OHMS / ((3.0f / voltage) - 1.0f);
    if (resistance < 1000.0f || resistance > 30000.0f) return;
    
    coolctl_temp = COOLCTL_NTC_B / logf(resistance / COOLCTL_NTC_RINF) - 273.15f;
}

//both arguments: 0 = off, 255 = maximum
void _coolctl_set_pwm(uint8_t fan_speed, uint8_t pump_speed) {
    uint32_t inverse_fan = 255 - fan_speed;
    uint32_t inverse_pump = 255 - pump_speed;
    OCMP7_CompareSecondaryValueSet((uint16_t)((inverse_fan * 3999) / 255));
    OCMP7_CompareSecondaryValueSet((uint16_t)((inverse_pump * 3999) / 255));
}

uint8_t _coolctl_lerp_curve(float temp, uint32_t point_count, float* point_temps, uint8_t* point_values) {
    uint8_t result = 255;
    int8_t rightpoint;
    switch (point_count) {
        case 0: //empty curve: always at 100%
            result = 255;
            break;
        case 1: //only one point: always at that value
            result = point_values[0];
            break;
        default: //otherwise: general curve linear interpolation
            for (rightpoint = 0; rightpoint < point_count; rightpoint++) { //find first point to the right of temp on the curve
                if (temp < point_temps[rightpoint]) break;
            }
            
            if (temp >= point_temps[point_count - 1]) { //right of rightmost point,: use max point value
                result = point_values[point_count - 1];
            } else if (rightpoint == 0) { //left of leftmost point: use min point value
                result = point_values[0];
            } else { //between points: interpolate linearly
                result = point_values[rightpoint - 1]
                        + (uint8_t)((temp - point_temps[rightpoint - 1])
                                    * (float)(point_values[rightpoint] - point_values[rightpoint - 1])
                                    / (point_temps[rightpoint] - point_temps[rightpoint - 1]));
            }
            break;
    }
    return result;
}


void COOLCTL_Init() {
    coolctl_fan_rpm = 0.0f;
    coolctl_pump_rpm = 0.0f;
    coolctl_flow_lph = 0.0f;
    _coolctl_next_update = 0;
    _coolctl_update_interval = SYS_TIME_MSToCount(COOLCTL_UPDATE_INTERVAL_MS);
    _coolctl_curr_watchdog_end = 0;
    _coolctl_one_second_time = (uint64_t)SYS_TIME_MSToCount(1000);
    _coolctl_running = APP_POWER_Main();
    _coolctl_wd_running = APP_POWER_Watchdog() && !_coolctl_running;
    _coolctl_updating = false;
    PUMP_WD_RUN_Clear();
    _coolctl_wd_pump_on = false;
    _coolctl_unacceptable_counter = 0;
    
    uint8_t i;
    for (i = 0; i < 3; i++) {
        _coolctl_lastpulses[i] = 0;
        _coolctl_period_sums[i] = 0;
        _coolctl_pulse_counts[i] = 0;
    }
    
    ADCHS_Filter1CallbackRegister(_coolctl_adcfilter_callback, NULL);
    
    TMR4_Start();
    TMR6_Start();
    
    OCMP7_Enable();
    OCMP8_Enable();
    
    ICAP1_CallbackRegister(_coolctl_icap_callback, 0);
    ICAP2_CallbackRegister(_coolctl_icap_callback, 1);
    ICAP3_CallbackRegister(_coolctl_icap_callback, 2);
    
    if (_coolctl_running) {
        ICAP1_Enable();
        ICAP2_Enable();
        ICAP3_Enable();
        _coolctl_startup_count = 0;
        _coolctl_unacceptable_counter = 0;
    }
    
    if (_coolctl_wd_running) {
        _coolctl_next_watchdog_run = iter_start_time + _coolctl_one_second_time * (uint64_t)config_watchdog_interval_seconds;
    } else {
        _coolctl_next_watchdog_run = 0;
    }
    
    OCMP7_CompareSecondaryValueSet(0);
    OCMP8_CompareSecondaryValueSet(0);
}

void COOLCTL_Tasks() {
    uint8_t i;
    
    if (_coolctl_running) {
        if (!APP_POWER_Main()) { //power down if needed
            ICAP1_Disable();
            ICAP2_Disable();
            ICAP3_Disable();
            coolctl_fan_rpm = 0.0f;
            coolctl_pump_rpm = 0.0f;
            coolctl_flow_lph = 0.0f;
            coolctl_temp = 0.0f;
            _coolctl_running = false;
            _coolctl_updating = false;
            PUMP_WD_RUN_Clear();
            _coolctl_wd_pump_on = false;
            _coolctl_curr_watchdog_end = 0;
            if (APP_POWER_Watchdog()) { //go to watchdog operation if needed
                _coolctl_next_watchdog_run = iter_start_time + _coolctl_one_second_time * (uint64_t)config_watchdog_interval_seconds;
                _coolctl_wd_running = true;
                OCMP7_CompareSecondaryValueSet(0);
                OCMP8_CompareSecondaryValueSet(0);
            } else _coolctl_wd_running = false;
            return;
        }
        
        if (iter_start_time >= _coolctl_next_update) { //regular updates
            ADCHS_ChannelConversionStart(ADCHS_CH0); //measure temperature
            _coolctl_updating = true;
            for (i = 0; i < 3; i++) { //get RPM/Flow measurements
                float avg_freq;
                if (_coolctl_pulse_counts[i] > 0) {
                    avg_freq = (float)_coolctl_pulse_counts[i] / ((float)_coolctl_period_sums[i] * COOLCTL_SECONDS_PER_TICK); //inverse of average period in seconds
                } else {
                    avg_freq = 0.0f;
                }
                *_coolctl_value_pointers[i] = avg_freq * _coolctl_conversion_factors[i];
                
                _coolctl_period_sums[i] = 0;
                _coolctl_pulse_counts[i] = 0;
            }
            _coolctl_updating = false;
            
            if (_coolctl_startup_count < 0) { //normal operation
                bool outside_nominal = false;
                bool outside_acceptable = false;
                for (i = 0; i < 4; i++) { //check nominal and acceptable ranges
                    float val = *_coolctl_value_pointers[i];
                    if (val < config_cooling_nominal_min[i] || val > config_cooling_nominal_max[i]) outside_nominal = true;
                    if (val < config_cooling_acceptable_min[i] || val > config_cooling_acceptable_max[i]) outside_acceptable = true;
                }
                
                //outside of nominal: produce warning (buzzer)
                if (outside_nominal) EMERGENCY_WarningEnable(EMERGENCY_SOURCE_COOLCTL);
                else EMERGENCY_WarningDisable(EMERGENCY_SOURCE_COOLCTL);
                
                if (outside_acceptable) { //outside of acceptable: tolerate for a few cycles, then force shutdown
                    if (++_coolctl_unacceptable_counter >= COOLCTL_UNACCEPTABLE_MAX_COUNT) {
                        EMERGENCY_Shutdown();
                        _coolctl_next_update = UINT64_MAX;
                        return;
                    }
                } else _coolctl_unacceptable_counter = 0;
                
                //calculate fan and pump speeds according to curves and set PWM accordingly
                uint8_t fan_speed = _coolctl_lerp_curve(coolctl_temp, config_fan_curve_point_count, config_fan_curve_temperatures, config_fan_curve_values);
                uint8_t pump_speed = _coolctl_lerp_curve(coolctl_temp, config_pump_curve_point_count, config_pump_curve_temperatures, config_pump_curve_values);
                _coolctl_set_pwm(fan_speed, pump_speed);
                
                CONFIG_HID_SendUpdate();
            } else { //startup operation
                if (_coolctl_startup_count++ >= COOLCTL_STARTUP_MAX_COUNT) { //maximum startup duration: force shutdown if exceeded (cooling system failed to start)
                    EMERGENCY_Shutdown();
                    _coolctl_next_update = UINT64_MAX;
                    return;
                }
                
                //check for each measurement to be within acceptable start range
                bool fan_started = (coolctl_fan_rpm >= COOLCTL_FAN_STARTUP_NOMINAL_RPM_MIN && coolctl_fan_rpm <= COOLCTL_FAN_STARTUP_NOMINAL_RPM_MAX);
                bool pump_started = (coolctl_pump_rpm >= COOLCTL_PUMP_STARTUP_NOMINAL_RPM_MIN && coolctl_pump_rpm <= COOLCTL_PUMP_STARTUP_NOMINAL_RPM_MAX);
                bool flow_started = (coolctl_flow_lph >= COOLCTL_FLOW_STARTUP_NOMINAL_LPH_MIN && coolctl_flow_lph <= COOLCTL_FLOW_STARTUP_NOMINAL_LPH_MAX);
                bool temp_started = (coolctl_temp >= COOLCTL_TEMP_STARTUP_NOMINAL_C_MIN && coolctl_temp <= COOLCTL_TEMP_STARTUP_NOMINAL_C_MAX);
                
                //if all are within acceptable range: startup successful
                if (fan_started && pump_started && flow_started && temp_started) {
                    _coolctl_startup_count = -1; //start normal operation
                }
            }
            
            //SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Fan: %.1f RPM, Pump: %.1f RPM, Flow: %.1f l/h, Temp: %.1f C, Count: %d\n", coolctl_fan_rpm, coolctl_pump_rpm, coolctl_flow_lph, coolctl_temp, _coolctl_startup_count);
            
            _coolctl_next_update = iter_start_time + _coolctl_update_interval;
        }
    } else if (APP_POWER_Main()) { //power up if needed
        ICAP1_Enable();
        ICAP2_Enable();
        ICAP3_Enable();
        coolctl_fan_rpm = 0.0f;
        coolctl_pump_rpm = 0.0f;
        coolctl_flow_lph = 0.0f;
        coolctl_temp = 0.0f;
        for (i = 0; i < 3; i++) {
            _coolctl_lastpulses[i] = 0;
            _coolctl_period_sums[i] = 0;
            _coolctl_pulse_counts[i] = 0;
        }
        OCMP7_CompareSecondaryValueSet(0);
        OCMP8_CompareSecondaryValueSet(0);
        _coolctl_running = true;
        _coolctl_wd_running = false; //main power automatically disables watchdog mode
        _coolctl_updating = false;
        PUMP_WD_RUN_Clear();
        _coolctl_wd_pump_on = false;
        _coolctl_startup_count = 0;
        _coolctl_next_update = iter_start_time;
        _coolctl_unacceptable_counter = 0;
    } else if (_coolctl_wd_running) {
        if (!APP_POWER_Watchdog()) { //power down if needed
            _coolctl_running = false;
            _coolctl_wd_running = false;
            _coolctl_updating = false;
            PUMP_WD_RUN_Clear();
            _coolctl_wd_pump_on = false;
            return;
        }
        
        if (!_coolctl_wd_pump_on && iter_start_time >= _coolctl_next_watchdog_run) { //turn on pump at specified time, calculate next times
            _coolctl_next_watchdog_run = iter_start_time + _coolctl_one_second_time * (uint64_t)config_watchdog_interval_seconds;
            _coolctl_curr_watchdog_end = iter_start_time + _coolctl_one_second_time * (uint64_t)config_watchdog_runtime_seconds;
            PUMP_WD_RUN_Set();
            _coolctl_wd_pump_on = true;
        } else if (_coolctl_wd_pump_on && iter_start_time >= _coolctl_curr_watchdog_end) { //turn off pump at specified time
            PUMP_WD_RUN_Clear();
            _coolctl_wd_pump_on = false;
        }
    } else if (APP_POWER_Watchdog()) { //power up watchdog mode if needed
        PUMP_WD_RUN_Clear();
        _coolctl_wd_pump_on = false;
        _coolctl_next_watchdog_run = iter_start_time + _coolctl_one_second_time * (uint64_t)config_watchdog_interval_seconds;
        _coolctl_curr_watchdog_end = 0;
        _coolctl_wd_running = true;
        OCMP7_CompareSecondaryValueSet(0);
        OCMP8_CompareSecondaryValueSet(0);
    }
}


/* *****************************************************************************
 End of File
 */
