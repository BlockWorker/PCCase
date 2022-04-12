/* ************************************************************************** */


#ifndef _CONFIG_HID_H    /* Guard against multiple inclusion */
#define _CONFIG_HID_H


#include <stdint.h>
#include <stdbool.h>


#define CONFIG_HID_REQUEST_COUNT 10

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    void CONFIG_HID_SendUpdate();
    
    void CONFIG_HID_Init();
    void CONFIG_HID_Tasks();
    

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
