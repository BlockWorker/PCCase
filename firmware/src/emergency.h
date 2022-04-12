/* ************************************************************************** */


#ifndef _EMERGENCY_H    /* Guard against multiple inclusion */
#define _EMERGENCY_H


#include <stdint.h>
#include <stdbool.h>



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        EMERGENCY_SOURCE_COOLCTL = 0x01,
    } EMERGENCY_SOURCE;
    
    
    void EMERGENCY_Init();
    void EMERGENCY_WarningEnable(EMERGENCY_SOURCE source);
    void EMERGENCY_WarningDisable(EMERGENCY_SOURCE source);
    void EMERGENCY_Shutdown();
    void EMERGENCY_Reset();


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
