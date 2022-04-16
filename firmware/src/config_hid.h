/* ************************************************************************** */


#ifndef _CONFIG_HID_H    /* Guard against multiple inclusion */
#define _CONFIG_HID_H


#include <stdint.h>
#include <stdbool.h>


#define CONFIG_HID_REQUEST_COUNT 10
#define CONFIG_HID_BUFFER_SIZE 256

#define CONFIG_HID_DATA_SIZE (58 * 4)
#define CONFIG_HID_REPORT_SIZE (CONFIG_HID_DATA_SIZE + 4)

#if (CONFIG_HID_REPORT_SIZE > CONFIG_HID_BUFFER_SIZE)
#error "Config HID buffer size too small!"
#endif

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
