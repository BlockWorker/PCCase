/*******************************************************************************
  System Initialization File

  File Name:
    usb_device_init_data.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "config_hid.h"
/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt1[] =
{
	0x06, 0x20, 0xff,              // USAGE_PAGE (Vendor Defined Page 32)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x19, 0x06,                    //   USAGE_MINIMUM (Vendor Usage 6)
    0x29, 0x0e,                    //   USAGE_MAXIMUM (Vendor Usage 14)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x20,                    //   REPORT_COUNT (32)
    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
    0x19, 0x0f,                    //   USAGE_MINIMUM (Vendor Usage 15)
    0x2a,                          //   USAGE_MAXIMUM (Vendor Usage as required)
    ((0x0f + CONFIG_HID_DATA_SIZE - 1) & 0xff),
    (((0x0f + CONFIG_HID_DATA_SIZE - 1) >> 8) & 0xff),
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95,                          //   REPORT_COUNT (As required)
    CONFIG_HID_DATA_SIZE,
    0xb1, 0x02,                    //   FEATURE (Data,Var,Abs)
    0x19, 0x06,                    //   USAGE_MINIMUM (Vendor Usage 6)
    0x29, 0x0e,                    //   USAGE_MAXIMUM (Vendor Usage 14)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x20,                    //   REPORT_COUNT (32)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x19, 0x0f,                    //   USAGE_MINIMUM (Vendor Usage 15)
    0x2a,                          //   USAGE_MAXIMUM (Vendor Usage as required)
    ((0x0f + CONFIG_HID_DATA_SIZE - 1) & 0xff),
    (((0x0f + CONFIG_HID_DATA_SIZE - 1) >> 8) & 0xff),
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95,                          //   REPORT_COUNT (As required)
    CONFIG_HID_DATA_SIZE,
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x19, 0x02,                    //   USAGE_MINIMUM (Vendor Usage 2)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Vendor Usage 5)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x27, 0xff, 0xff, 0xff, 0x7f,  //   LOGICAL_MAXIMUM (2147483647)
    0x75, 0x20,                    //   REPORT_SIZE (32)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
};

/**************************************************
 * USB Device HID Function Init Data
 **************************************************/
const USB_DEVICE_HID_INIT hidInit1 =
{
	 .hidReportDescriptorSize = sizeof(hid_rpt1),
	 .hidReportDescriptor = (void *)&hid_rpt1,
	 .queueSizeReportReceive = 20,
	 .queueSizeReportSend = 20
};


/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt0[] =
{
	//0x05, 0x59,                    // USAGE_PAGE (Lighting And Illumination)
	0x06, 0x59, 0xff,              // USAGE_PAGE (Lighting And Illumination) (vendor because of windows bug)
    0x09, 0x01,                    // USAGE (LampArray)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x02,                    //   USAGE (LampArrayAttributesReport)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x01,                    //     REPORT_ID (1)
    0x09, 0x03,                    //     USAGE (LampCount)
    0x15, 0x01,                    //     LOGICAL_MINIMUM (1)
    0x27, 0xff, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65535)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x19, 0x04,                    //     USAGE_MINIMUM (BoundingBoxWidthInMicrometers)
    0x29, 0x08,                    //     USAGE_MAXIMUM (MinUpdateIntervalInMicroseconds)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xff, 0xff, 0xff, 0x7f,  //     LOGICAL_MAXIMUM (2147483647)
    0x75, 0x20,                    //     REPORT_SIZE (32)
    0x95, 0x05,                    //     REPORT_COUNT (5)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x20,                    //   USAGE (LampAttributesRequestReport)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x02,                    //     REPORT_ID (2)
    0x09, 0x21,                    //     USAGE (LampId)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x22,                    //   USAGE (LampAttributesResponseReport)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x03,                    //     REPORT_ID (3)
    0x09, 0x21,                    //     USAGE (LampId)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x19, 0x23,                    //     USAGE_MINIMUM (PositionXInMicrometers)
    0x29, 0x27,                    //     USAGE_MAXIMUM (UpdateLatencyInMicroseconds)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xff, 0xff, 0xff, 0x7f,  //     LOGICAL_MAXIMUM (2147483647)
    0x75, 0x20,                    //     REPORT_SIZE (32)
    0x95, 0x05,                    //     REPORT_COUNT (5)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x19, 0x28,                    //     USAGE_MINIMUM (RedLevelCount)
    0x29, 0x2b,                    //     USAGE_MAXIMUM (IntensityLevelCount)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x2c,                    //     USAGE (IsProgrammable)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x09, 0x2d,                    //     USAGE (InputBinding)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x00,                    //     LOGICAL_MAXIMUM (0)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x50,                    //   USAGE (LampMultiUpdateReport)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x04,                    //     REPORT_ID (4)
    0x09, 0x03,                    //     USAGE (LampCount)
    0x15, 0x01,                    //     LOGICAL_MINIMUM (1)
    0x25, 0x10,                    //     LOGICAL_MAXIMUM (16)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x21,                    //     USAGE (LampId)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x55,                    //     USAGE (LampUpdateFlags)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x60,                    //   USAGE (LampRangeUpdateReport)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x05,                    //     REPORT_ID (5)
    0x09, 0x61,                    //     USAGE (LampIdStart)
    0x09, 0x62,                    //     USAGE (LampIdEnd)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x19, 0x51,                    //     USAGE_MINIMUM (RedUpdateChannel)
    0x29, 0x54,                    //     USAGE_MAXIMUM (IntensityUpdateChannel)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x55,                    //     USAGE (LampUpdateFlags)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x70,                    //   USAGE (LampArrayControlReport)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x06,                    //     REPORT_ID (6)
    0x09, 0x71,                    //     USAGE (AutonomousMode)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0xb1, 0x02,                    //     FEATURE (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x0a, 0x01, 0x7f,              //   USAGE (Undefined)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, 0x07,                    //     REPORT_ID (7)
    0x09, 0x61,                    //     USAGE (LampIdStart)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x27, 0xfe, 0xff, 0x00, 0x00,  //     LOGICAL_MAXIMUM (65534)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x03,                    //     USAGE (LampCount)
    0x15, 0x01,                    //     LOGICAL_MINIMUM (1)
    0x25, 0x20,                    //     LOGICAL_MAXIMUM (32)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x1a, 0x04, 0x7f,              //     USAGE_MINIMUM (Undefined)
    0x2a, 0x43, 0x7f,              //     USAGE_MAXIMUM (Undefined)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x40,                    //     REPORT_COUNT (64)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x09, 0x55,                    //     USAGE (LampUpdateFlags)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};

/**************************************************
 * USB Device HID Function Init Data
 **************************************************/
const USB_DEVICE_HID_INIT hidInit0 =
{
	 .hidReportDescriptorSize = sizeof(hid_rpt0),
	 .hidReportDescriptor = (void *)&hid_rpt0,
	 .queueSizeReportReceive = 60,
	 .queueSizeReportSend = 1
};



/**************************************************
 * USB Device Layer Function Driver Registration 
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[2] =
{

	/* HID Function 1 */
    { 
        .configurationValue = 1,    /* Configuration value */ 
        .interfaceNumber = 1,       /* First interfaceNumber of this function */ 
        .speed = USB_SPEED_HIGH|USB_SPEED_FULL,    /* Function Speed */ 
        .numberOfInterfaces = 1,    /* Number of interfaces */
        .funcDriverIndex = 1,  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit1    /* Function driver init data */
    },



	/* HID Function 0 */
    { 
        .configurationValue = 1,    /* Configuration value */ 
        .interfaceNumber = 0,       /* First interfaceNumber of this function */ 
        .speed = USB_SPEED_HIGH|USB_SPEED_FULL,    /* Function Speed */ 
        .numberOfInterfaces = 1,    /* Number of interfaces */
        .funcDriverIndex = 0,  /* Index of HID Function Driver */
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit0    /* Function driver init data */
    },



};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor 
 *******************************************/

const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                                  // DEVICE descriptor type
    0x0200,                                                 // USB Spec Release Number in BCD format
    0xEF,                                                   // Class Code
    0x02,                                                   // Subclass code
    0x01,                                                   // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,                             // Max packet size for EP0, see configuration.h
    0x1209,                                                 // Vendor ID
    0x5001,                                                 // Product ID				
    0x0100,                                                 // Device release number in BCD format
    0x01,                                                   // Manufacturer string index
    0x02,                                                   // Product string index
	0x00,                                                   // Device serial number string index
    0x01                                                    // Number of possible configurations
};

/*******************************************
 *  USB Device Qualifier Descriptor for this
 *  demo.
 *******************************************/
const USB_DEVICE_QUALIFIER deviceQualifierDescriptor1 =
{
    0x0A,                                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE_QUALIFIER,                        // Device Qualifier Type
    0x0200,                                                 // USB Specification Release number
	0xEF,                                                   // Class Code
    0x02,                                                   // Subclass code
    0x01,                                                   // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,                             // Maximum packet size for endpoint 0
    0x01,                                                   // Number of possible configurations
    0x00                                                    // Reserved for future use.
};


/*******************************************
 *  USB High Speed Configuration Descriptor
 *******************************************/
 
const uint8_t highSpeedConfigurationDescriptor[]=
{
	/* Configuration Descriptor */

    0x09,                                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                           // Descriptor Type
    USB_DEVICE_16bitTo8bitArrange(73),                      //(73 Bytes)Size of the Configuration descriptor
    2,                                                      // Number of interfaces in this configuration
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT, // Attributes
    250,
	

	/* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // Descriptor Type is Interface descriptor
    1,                                  // Interface Number
    0x00,                                  // Alternate Setting Number
    0x02,                                  // Number of endpoints in this interface
    USB_HID_CLASS_CODE,                 // Class code
    USB_HID_SUBCLASS_CODE_NO_SUBCLASS , // Subclass code
    USB_HID_PROTOCOL_CODE_NONE,         // No Protocol
    0x00,                                  // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    1,                              // Number of class descriptors
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    USB_DEVICE_16bitTo8bitArrange(sizeof(hid_rpt1)),   // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    2 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP2 IN )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    2 | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP2 OUT )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // size
    0x01,                           // Interval
    
    



	/* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // Descriptor Type is Interface descriptor
    0,                                  // Interface Number
    0x00,                                  // Alternate Setting Number
    0x02,                                  // Number of endpoints in this interface
    USB_HID_CLASS_CODE,                 // Class code
    USB_HID_SUBCLASS_CODE_NO_SUBCLASS , // Subclass code
    USB_HID_PROTOCOL_CODE_NONE,         // No Protocol
    0x00,                                  // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    1,                              // Number of class descriptors
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    USB_DEVICE_16bitTo8bitArrange(sizeof(hid_rpt0)),   // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP1 IN )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP1 OUT )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // size
    0x01,                           // Interval
    
    



};

/*******************************************
 * Array of High speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE highSpeedConfigDescSet[1] =
{
    highSpeedConfigurationDescriptor
};

/*******************************************
 *  USB Full Speed Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[]=
{
	/* Configuration Descriptor */

    0x09,                                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                           // Descriptor Type
    USB_DEVICE_16bitTo8bitArrange(73),                      //(73 Bytes)Size of the Configuration descriptor
    2,                                                      // Number of interfaces in this configuration
    0x01,                                                   // Index value of this configuration
    0x00,                                                   // Configuration string index
    USB_ATTRIBUTE_DEFAULT, // Attributes
    250,
	
	
	/* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // Descriptor Type is Interface descriptor
    1,                                  // Interface Number
    0x00,                                  // Alternate Setting Number
    0x02,                                  // Number of endpoints in this interface
    USB_HID_CLASS_CODE,                 // Class code
    USB_HID_SUBCLASS_CODE_NO_SUBCLASS , // Subclass code
    USB_HID_PROTOCOL_CODE_NONE,         // No Protocol
    0x00,                                  // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    1,                              // Number of class descriptors
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    USB_DEVICE_16bitTo8bitArrange(sizeof(hid_rpt1)),   // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    2 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP2 IN )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    2 | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP2 OUT )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // size
    0x01,                           // Interval
    
    



	/* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // Descriptor Type is Interface descriptor
    0,                                  // Interface Number
    0x00,                                  // Alternate Setting Number
    0x02,                                  // Number of endpoints in this interface
    USB_HID_CLASS_CODE,                 // Class code
    USB_HID_SUBCLASS_CODE_NO_SUBCLASS , // Subclass code
    USB_HID_PROTOCOL_CODE_NONE,         // No Protocol
    0x00,                                  // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    1,                              // Number of class descriptors
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    USB_DEVICE_16bitTo8bitArrange(sizeof(hid_rpt0)),   // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP1 IN )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP1 OUT )
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40,0x00,                      // size
    0x01,                           // Interval
    
    



};

/*******************************************
 * Array of Full speed Configuration 
 * descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[1] =
{
    fullSpeedConfigurationDescriptor
};


/**************************************
 *  String descriptors.
 *************************************/
 /*******************************************
 *  Language code string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;
        uint8_t bDscType;
        uint16_t string[1];
    }
    sd000 =
    {
        sizeof(sd000),                                      // Size of this descriptor in bytes
        USB_DESCRIPTOR_STRING,                              // STRING descriptor type
        {0x0409}                                            // Language ID
    };
/*******************************************
 *  Manufacturer string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;                                    // Size of this descriptor in bytes
        uint8_t bDscType;                                   // STRING descriptor type
        uint16_t string[11];                                // String
    }
    sd001 =
    {
        sizeof(sd001),
        USB_DESCRIPTOR_STRING,
        {'B','l','o','c','k','W','o','r','k','e','r'}
		
    };

/*******************************************
 *  Product string descriptor
 *******************************************/
	const struct
    {
        uint8_t bLength;                                    // Size of this descriptor in bytes
        uint8_t bDscType;                                   // STRING descriptor type
        uint16_t string[18];                                // String
    }
    sd002 =
    {
        sizeof(sd002),
        USB_DESCRIPTOR_STRING,
		{'P','C',' ','C','a','s','e',' ','C','o','n','t','r','o','l','l','e','r'}
    }; 

/***************************************
 * Array of string descriptors
 ***************************************/
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002,
};

/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
 
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,                                      // Full speed descriptor
    1,                                                      // Total number of full speed configurations available
    fullSpeedConfigDescSet,                                 // Pointer to array of full speed configurations descriptors
    &deviceDescriptor,                                      // High speed device descriptor
    1,                                                      // Total number of high speed configurations available
    highSpeedConfigDescSet,                                 // Pointer to array of high speed configurations descriptors
	3,  													// Total number of string descriptors available.
    stringDescriptors,                                      // Pointer to array of string descriptors.
    &deviceQualifierDescriptor1,                            // Pointer to full speed dev qualifier.
    &deviceQualifierDescriptor1                             // Pointer to high speed dev qualifier.
};


/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/

const USB_DEVICE_INIT usbDevInitData =
{
    /* Number of function drivers registered to this instance of the
       USB device layer */
    .registeredFuncCount = 2,
	
    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
    .deviceSpeed =  USB_SPEED_HIGH,   
	
	/* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = DRV_USBHS_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = DRV_USBHS_DEVICE_INTERFACE,
	
	/* Specify queue size for vendor endpoint read */
    .queueSizeEndpointRead = 115,
    
    /* Specify queue size for vendor endpoint write */
    .queueSizeEndpointWrite= 115,
};
// </editor-fold>
