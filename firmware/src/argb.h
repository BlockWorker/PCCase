/* ************************************************************************** */


#ifndef _ARGB_H    /* Guard against multiple inclusion */
#define _ARGB_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include "app_util.h"

#define ARGB_MAX_LENGTH 256
#define ARGB_FRAMETIME_US 16000

//Frame time must be at least 80us longer than send time
#if (ARGB_FRAMETIME_US - 30 * ARGB_MAX_LENGTH) < 100
#error "ARGB frame time too short!"
#endif

#define ARGB_ZERO_DUTY 31
#define ARGB_ONE_DUTY 62


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    
    typedef enum {
        ARGB_CHANNEL_A = 0,
        ARGB_CHANNEL_B = 1,
        ARGB_CHANNEL_C = 2
    } argb_channel_t;    
    
    
    uint32_t argb_a_colors[ARGB_MAX_LENGTH];
    uint32_t argb_b_colors[ARGB_MAX_LENGTH];
    uint32_t argb_c_colors[ARGB_MAX_LENGTH];

    uint32_t* argb_colors[3];
    
    
    void ARGB_Init();
    void ARGB_Tasks();
    void ARGB_SetFrameCallback(SIMPLE_CALLBACK callback, uintptr_t context);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
