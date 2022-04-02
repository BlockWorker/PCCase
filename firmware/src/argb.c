/* ************************************************************************** */

#include "argb.h"
#include "peripheral/dmac/plib_dmac.h"
#include "system/time/sys_time.h"
#include "app_power.h"
#include "system/time/src/sys_time_local.h"
#include "peripheral/tmr/plib_tmr5.h"
#include "peripheral/ocmp/plib_ocmp1.h"
#include "peripheral/ocmp/plib_ocmp2.h"
#include "peripheral/ocmp/plib_ocmp3.h"
#include "app.h"

//Raw color values, format: 00GGRRBB
uint32_t argb_a_colors[ARGB_MAX_LENGTH] = { 0 };
uint32_t argb_b_colors[ARGB_MAX_LENGTH] = { 0 };
uint32_t argb_c_colors[ARGB_MAX_LENGTH] = { 0 };

uint32_t* argb_colors[3] = { argb_a_colors, argb_b_colors, argb_c_colors };

//Converted color values (bits encoded as PWM duty cycles)
static uint8_t __attribute__((coherent)) argb_a_data[ARGB_MAX_LENGTH * 24 + 1] = { 0 };
static uint8_t __attribute__((coherent)) argb_b_data[ARGB_MAX_LENGTH * 24 + 1] = { 0 };
static uint8_t __attribute__((coherent)) argb_c_data[ARGB_MAX_LENGTH * 24 + 1] = { 0 };

static uint8_t* argb_data[3] = { argb_a_data, argb_b_data, argb_c_data };

static volatile uint32_t* argb_registers[3] = { &OC1RS, &OC2RS, &OC3RS };

//SYS_TIME timestamps for next send
static uint64_t argb_nextsend = 0;

static uint32_t argb_frame_interval = 0;
static uint32_t argb_retry_interval = 0;

static bool argb_running = false;

static SIMPLE_CALLBACK argb_callback = NULL;
static uintptr_t argb_callback_context = NULL;


void _argb_convert_colors(argb_channel_t channel) {
    uint32_t* colors = argb_colors[channel];
    uint8_t* data = argb_data[channel];
    
    uint32_t i;
    for (i = 0; i < ARGB_MAX_LENGTH; i++) {
        uint32_t color = colors[i];
        uint32_t data_offset = 24 * i;
        
        uint32_t mask = 0x800000;
        uint32_t j;        
        for (j = 0; j < 24; j++) {
            data[data_offset + j] = (color & mask) > 0 ? ARGB_ONE_DUTY : ARGB_ZERO_DUTY;
            mask >>= 1;
        }
    }
}

bool _argb_start_send(argb_channel_t channel) {    
    _argb_convert_colors(channel);
    
    DMAC_CHANNEL dma_channel = (DMAC_CHANNEL)channel;
    void* src = argb_data[channel];
    void* dest = (void*)argb_registers[channel];
    
    bool result = DMAC_ChannelTransfer(dma_channel, src, ARGB_MAX_LENGTH * 24 + 1, dest, 1, 1);
    
    if (!result) {
        DMAC_ChannelDisable(dma_channel);
    }
    
    return result;
}

void _argb_dma_callback(DMAC_TRANSFER_EVENT status, uintptr_t context) {
    if (context == 2 && status == DMAC_TRANSFER_EVENT_COMPLETE) {
        if (argb_callback != NULL) argb_callback(argb_callback_context);
    }
}



void ARGB_Init() {
    DMAC_ChannelDisable(DMAC_CHANNEL_0);
    DMAC_ChannelDisable(DMAC_CHANNEL_1);
    DMAC_ChannelDisable(DMAC_CHANNEL_2);
    
    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, _argb_dma_callback, 0);
    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_1, _argb_dma_callback, 1);
    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_2, _argb_dma_callback, 2);
    
    argb_frame_interval = SYS_TIME_USToCount(ARGB_FRAMETIME_US);
    argb_retry_interval = SYS_TIME_USToCount(1000);
    
    argb_callback = NULL;
    argb_callback_context = NULL;
    
    TMR5_Start();
    OCMP1_Enable();
    OCMP2_Enable();
    OCMP3_Enable();
    
    argb_running = false;
    ALED_OE_Clear();
}

void ARGB_Tasks() {
    if (argb_running) {
        if (!APP_POWER_Main()) {
            DMAC_ChannelDisable(DMAC_CHANNEL_0);
            DMAC_ChannelDisable(DMAC_CHANNEL_1);
            DMAC_ChannelDisable(DMAC_CHANNEL_2);
            
            OC1RS = 0;
            OC2RS = 0;
            OC3RS = 0;
            
            argb_running = false;
            ALED_OE_Clear();
        }
        
        if (iter_start_time >= argb_nextsend) {
            argb_channel_t ch;
            for (ch = ARGB_CHANNEL_A; ch <= ARGB_CHANNEL_C; ch++) {
                if (!_argb_start_send(ch)) break;
            }
            argb_nextsend = iter_start_time + argb_frame_interval;
        }
    } else if (APP_POWER_Main()) {
        ALED_OE_Set();
        argb_running = true;
        argb_nextsend = iter_start_time;
    }
}

void ARGB_SetFrameCallback(SIMPLE_CALLBACK callback, uintptr_t context) {
    argb_callback = callback;
    argb_callback_context = context;
}


/* *****************************************************************************
 End of File
 */
