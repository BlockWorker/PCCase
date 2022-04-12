/* ************************************************************************** */


#ifndef _ARGB_HID_H    /* Guard against multiple inclusion */
#define _ARGB_HID_H

#include <argb.h>


//total lamp count: three ARGB channels plus one power/hdd light
#define ARGB_HID_LAMPCOUNT (3 * ARGB_MAX_LENGTH + 1)
#define ARGB_HID_QUEUE_MAX_LENGTH 1024
#define ARGB_HID_REQUEST_COUNT 60
#define ARGB_HID_RECEIVE_BUFFER_SIZE 128

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    typedef enum {
        ARGB_HID_REPORT_UNDEFINED = 0x00,
        ARGB_HID_REPORT_ARRAY_ATTRIB = 0x01,
        ARGB_HID_REPORT_LAMP_ATTRIB_REQ = 0x02,
        ARGB_HID_REPORT_LAMP_ATTRIB_RES = 0x03,
        ARGB_HID_REPORT_LAMP_MULTI_UPDATE = 0x04,
        ARGB_HID_REPORT_LAMP_RANGE_UPDATE = 0x05,
        ARGB_HID_REPORT_ARRAY_CONTROL = 0x06,
        ARGB_HID_REPORT_LAMP_BULK_UPDATE = 0x07
    } ARGB_HID_REPORT_ID;
    

    bool argb_hid_autonomous_mode;
    
    
    void ARGB_HID_Init();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
