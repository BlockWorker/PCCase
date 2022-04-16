/* ************************************************************************** */


#ifndef _ARGB_EFFECT_H    /* Guard against multiple inclusion */
#define _ARGB_EFFECT_H


#include "argb.h"



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    typedef enum {
        ARGB_EFFECT_NONE = 0,
        ARGB_EFFECT_STATIC = 1,
        ARGB_EFFECT_RAINBOWCYCLE = 2,
        ARGB_EFFECT_TWOCOLORCYCLE = 3,
        ARGB_EFFECT_RAINBOWWAVE = 4,
        ARGB_EFFECT_PANELONLYSTATIC = 5
    } ARGB_EFFECT_TYPE;
    
    
    bool argb_effect_update_requested;
    
    
    void ARGB_EFFECT_Init();
    void ARGB_EFFECT_Update();
    void ARGB_EFFECT_Tasks();
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
