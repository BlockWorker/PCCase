/* ************************************************************************** */


#include <stddef.h>
#include <string.h>
#include <math.h>

#include "argb_effect.h"
#include "argb_hid.h"
#include "app_config.h"


bool argb_effect_update_requested = false;


static bool _argb_effect_frame_ended = false;
static uint32_t _argb_effect_frame_counter = 0;
static float _argb_effect_color_diff[3] = { 0.0f };


uint8_t _argb_get_multiple(uint8_t max, uint8_t divisor) {
    uint8_t quotient = max / divisor;
    return divisor * quotient;
}

void _argb_next_rainbow(uint32_t* buffer, uint8_t step) {
    uint8_t* buf8 = (uint8_t*)buffer;
    
    if(buf8[2] > 0 && buf8[0] == 0){
        buf8[2] -= step;
        buf8[1] += step;
    }
    if(buf8[1] > 0 && buf8[2] == 0){
        buf8[1] -= step;
        buf8[0] += step;
    }
    if(buf8[0] > 0 && buf8[1] == 0){
        buf8[2] += step;
        buf8[0] -= step;
    }
}

void _argb_effect_frame_callback(uintptr_t context) {
    _argb_effect_frame_ended = true;
}

void _argb_effect_draw_frame() {
    uint32_t* colors = argb_colors[0];
    uint8_t* temp_ptr8;
    float temp_valuef;
    uint8_t temp_value8;
    uint32_t temp_value32;
    uint32_t i;
    
    switch (config_argb_effect_index) {
        case ARGB_EFFECT_RAINBOWCYCLE:
            if (++_argb_effect_frame_counter >= config_argb_effect_params[2]) { //update frequency based on slowdown factor parameter
                _argb_effect_frame_counter = 0;
                _argb_next_rainbow(colors, config_argb_effect_params[0]); //step forward in rainbow cycle according to speed parameter
                for (i = 1; i < ARGB_HID_LAMPCOUNT; i++) { //apply color to all pixels
                    colors[i] = colors[0];
                }
            }
            break;
        case ARGB_EFFECT_TWOCOLORCYCLE:
            temp_valuef = 0.5f * (1.0f - cosf(2.0f * (float)M_PI * (float)(_argb_effect_frame_counter++) / (float)config_argb_effect_params[2])); //position in cycle
            if (_argb_effect_frame_counter >= config_argb_effect_params[2]) _argb_effect_frame_counter = 0; //reset frame counter to 0 at end of period
            temp_ptr8 = (uint8_t*)config_argb_effect_params; //bytes of first color parameter
            temp_value32 = 0x01; //highest byte: "intensity" = 1
            for (i = 2; i < 3; i--) { //interpolate each channel
                temp_value8 = temp_ptr8[i]; //start with first color value
                temp_value8 += (uint8_t)(temp_valuef * _argb_effect_color_diff[i]); //add scaled difference between color values
                temp_value32 <<= 8; //shift color by one byte
                temp_value32 |= temp_value8; //add channel in lower byte
            }
            for (i = 0; i < ARGB_HID_LAMPCOUNT; i++) { //apply color to all pixels
                colors[i] = temp_value32;
            }
            break;
        case ARGB_EFFECT_RAINBOWWAVE:
            if (++_argb_effect_frame_counter >= config_argb_effect_params[3]) { //update frequency based on slowdown factor parameter
                _argb_effect_frame_counter = 0;
                for (i = 0; i < ARGB_HID_LAMPCOUNT; i++) { //step each pixel forward in rainbow cycle according to speed parameter
                    _argb_next_rainbow(colors + i, config_argb_effect_params[0]);
                }
            }
            break;
        case ARGB_EFFECT_NONE:
        case ARGB_EFFECT_STATIC:
        case ARGB_EFFECT_PANELONLYSTATIC:
        default:
            break;
    }
}


void ARGB_EFFECT_Init() {
    ARGB_SetFrameCallback(_argb_effect_frame_callback, NULL);
    ARGB_EFFECT_Update();
}

void _argb_swap_buffers();

void ARGB_EFFECT_Update() {
    argb_effect_update_requested = false;
    _argb_effect_frame_ended = false;
    
    if (!argb_hid_autonomous_mode) return;
    
    uint32_t* colors = argb_colors[0];
    
    uint8_t temp_value8;
    uint32_t temp_value32;
    uint8_t* temp_ptr8;
    uint32_t i, j;
    
    switch (config_argb_effect_index) {
        case ARGB_EFFECT_NONE:
            memset(colors, 0, ARGB_HID_LAMPCOUNT * sizeof(uint32_t)); //turn off all pixels
            break;
        case ARGB_EFFECT_STATIC:
            temp_value32 = 0x01000000 | (config_argb_effect_params[0] & 0xffffff); //the static color value
            for (i = 0; i < ARGB_HID_LAMPCOUNT; i++) { //apply static color to all pixels
                colors[i] = temp_value32;
            }
            break;
        case ARGB_EFFECT_PANELONLYSTATIC:
            memset(colors, 0, (ARGB_HID_LAMPCOUNT - 1) * sizeof(uint32_t)); //turn off all pixels except for last (front panel)
            colors[ARGB_HID_LAMPCOUNT - 1] = 0x01000000 | (config_argb_effect_params[0] & 0xffffff); //apply static color to front panel
            break;
        case ARGB_EFFECT_RAINBOWCYCLE:
            temp_value8 = _argb_get_multiple(config_argb_effect_params[1], config_argb_effect_params[0]); //initial red value
            temp_value32 = 0x01000000 | (((uint32_t)temp_value8) << 16); //initial color value (with this red value)
            _argb_effect_frame_counter = 0;
            for (i = 0; i < ARGB_HID_LAMPCOUNT; i++) { //apply initial color to all pixels
                colors[i] = temp_value32;
            }
            break;
        case ARGB_EFFECT_TWOCOLORCYCLE:
            temp_ptr8 = (uint8_t*)config_argb_effect_params; //bytes of the two color parameters
            _argb_effect_frame_counter = 0;
            _argb_effect_color_diff[0] = (float)temp_ptr8[4] - (float)temp_ptr8[0]; //difference of blue values
            _argb_effect_color_diff[1] = (float)temp_ptr8[5] - (float)temp_ptr8[1]; //difference of green values
            _argb_effect_color_diff[2] = (float)temp_ptr8[6] - (float)temp_ptr8[2]; //difference of red values
            temp_value32 = 0x01000000 | (config_argb_effect_params[0] & 0xffffff); //initial (first) color value
            for (i = 0; i < ARGB_HID_LAMPCOUNT; i++) { //apply initial color to all pixels
                colors[i] = temp_value32;
            }
            break;
        case ARGB_EFFECT_RAINBOWWAVE:
            temp_value8 = _argb_get_multiple(config_argb_effect_params[2], config_argb_effect_params[0]); //initial red value of last pixel
            temp_value32 = 0x01000000 | (((uint32_t)temp_value8) << 16); //initial color value of last pixel (with this red value)
            temp_value8 = config_argb_effect_params[1] / config_argb_effect_params[3]; //scaled frequency parameter
            _argb_effect_frame_counter = 0;
            colors[ARGB_HID_LAMPCOUNT - 1] = temp_value32; //set last pixel initial color
            for (i = ARGB_HID_LAMPCOUNT - 2; i < ARGB_HID_LAMPCOUNT; i--) { //calculate and apply initial color for each remaining pixel
                for (j = 0; j < temp_value8; j++) { //step forward in rainbow cycle according to frequency and speed parameters
                    _argb_next_rainbow(&temp_value32, config_argb_effect_params[0]);
                }
                colors[i] = temp_value32;
            }
            break;
        default:
            return;
    }
    
    _argb_swap_buffers();
}

void ARGB_EFFECT_Tasks() {
    if (argb_effect_update_requested) {
        ARGB_EFFECT_Update();
    } else if (_argb_effect_frame_ended) {
        _argb_effect_frame_ended = false;
        if (argb_hid_autonomous_mode) _argb_effect_draw_frame();
    }
}


/* *****************************************************************************
 End of File
 */
