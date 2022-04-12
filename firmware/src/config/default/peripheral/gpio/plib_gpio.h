/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for PUMP_WD_RUN pin ***/
#define PUMP_WD_RUN_Set()               (LATJSET = (1<<9))
#define PUMP_WD_RUN_Clear()             (LATJCLR = (1<<9))
#define PUMP_WD_RUN_Toggle()            (LATJINV= (1<<9))
#define PUMP_WD_RUN_OutputEnable()      (TRISJCLR = (1<<9))
#define PUMP_WD_RUN_InputEnable()       (TRISJSET = (1<<9))
#define PUMP_WD_RUN_Get()               ((PORTJ >> 9) & 0x1)
#define PUMP_WD_RUN_PIN                  GPIO_PIN_RJ9

/*** Macros for BUZZER pin ***/
#define BUZZER_Set()               (LATJSET = (1<<12))
#define BUZZER_Clear()             (LATJCLR = (1<<12))
#define BUZZER_Toggle()            (LATJINV= (1<<12))
#define BUZZER_OutputEnable()      (TRISJCLR = (1<<12))
#define BUZZER_InputEnable()       (TRISJSET = (1<<12))
#define BUZZER_Get()               ((PORTJ >> 12) & 0x1)
#define BUZZER_PIN                  GPIO_PIN_RJ12

/*** Macros for EMERGENCY_SD pin ***/
#define EMERGENCY_SD_Set()               (LATJSET = (1<<10))
#define EMERGENCY_SD_Clear()             (LATJCLR = (1<<10))
#define EMERGENCY_SD_Toggle()            (LATJINV= (1<<10))
#define EMERGENCY_SD_OutputEnable()      (TRISJCLR = (1<<10))
#define EMERGENCY_SD_InputEnable()       (TRISJSET = (1<<10))
#define EMERGENCY_SD_Get()               ((PORTJ >> 10) & 0x1)
#define EMERGENCY_SD_PIN                  GPIO_PIN_RJ10

/*** Macros for TACH_OUT pin ***/
#define TACH_OUT_Get()               ((PORTC >> 2) & 0x1)
#define TACH_OUT_PIN                  GPIO_PIN_RC2

/*** Macros for PUMP_PWM pin ***/
#define PUMP_PWM_Get()               ((PORTC >> 3) & 0x1)
#define PUMP_PWM_PIN                  GPIO_PIN_RC3

/*** Macros for PUMP_TACH pin ***/
#define PUMP_TACH_Get()               ((PORTG >> 6) & 0x1)
#define PUMP_TACH_PIN                  GPIO_PIN_RG6

/*** Macros for FAN_PWM pin ***/
#define FAN_PWM_Get()               ((PORTG >> 7) & 0x1)
#define FAN_PWM_PIN                  GPIO_PIN_RG7

/*** Macros for FAN_TACH pin ***/
#define FAN_TACH_Get()               ((PORTG >> 8) & 0x1)
#define FAN_TACH_PIN                  GPIO_PIN_RG8

/*** Macros for FLOW_TACH pin ***/
#define FLOW_TACH_Get()               ((PORTE >> 8) & 0x1)
#define FLOW_TACH_PIN                  GPIO_PIN_RE8

/*** Macros for WATER_TEMP pin ***/
#define WATER_TEMP_Get()               ((PORTB >> 5) & 0x1)
#define WATER_TEMP_PIN                  GPIO_PIN_RB5

/*** Macros for PWR_DETECT pin ***/
#define PWR_DETECT_Set()               (LATHSET = (1<<3))
#define PWR_DETECT_Clear()             (LATHCLR = (1<<3))
#define PWR_DETECT_Toggle()            (LATHINV= (1<<3))
#define PWR_DETECT_OutputEnable()      (TRISHCLR = (1<<3))
#define PWR_DETECT_InputEnable()       (TRISHSET = (1<<3))
#define PWR_DETECT_Get()               ((PORTH >> 3) & 0x1)
#define PWR_DETECT_PIN                  GPIO_PIN_RH3
#define PWR_DETECT_InterruptEnable()   (CNENHSET = (1<<3))
#define PWR_DETECT_InterruptDisable()  (CNENHCLR = (1<<3))

/*** Macros for WD_DETECT pin ***/
#define WD_DETECT_Set()               (LATHSET = (1<<8))
#define WD_DETECT_Clear()             (LATHCLR = (1<<8))
#define WD_DETECT_Toggle()            (LATHINV= (1<<8))
#define WD_DETECT_OutputEnable()      (TRISHCLR = (1<<8))
#define WD_DETECT_InputEnable()       (TRISHSET = (1<<8))
#define WD_DETECT_Get()               ((PORTH >> 8) & 0x1)
#define WD_DETECT_PIN                  GPIO_PIN_RH8
#define WD_DETECT_InterruptEnable()   (CNENHSET = (1<<8))
#define WD_DETECT_InterruptDisable()  (CNENHCLR = (1<<8))

/*** Macros for FPLED_G pin ***/
#define FPLED_G_Get()               ((PORTD >> 9) & 0x1)
#define FPLED_G_PIN                  GPIO_PIN_RD9

/*** Macros for FPLED_B pin ***/
#define FPLED_B_Get()               ((PORTD >> 10) & 0x1)
#define FPLED_B_PIN                  GPIO_PIN_RD10

/*** Macros for ALED_OE pin ***/
#define ALED_OE_Set()               (LATHSET = (1<<15))
#define ALED_OE_Clear()             (LATHCLR = (1<<15))
#define ALED_OE_Toggle()            (LATHINV= (1<<15))
#define ALED_OE_OutputEnable()      (TRISHCLR = (1<<15))
#define ALED_OE_InputEnable()       (TRISHSET = (1<<15))
#define ALED_OE_Get()               ((PORTH >> 15) & 0x1)
#define ALED_OE_PIN                  GPIO_PIN_RH15

/*** Macros for ALED_A_DATA pin ***/
#define ALED_A_DATA_Get()               ((PORTD >> 0) & 0x1)
#define ALED_A_DATA_PIN                  GPIO_PIN_RD0

/*** Macros for ALED_B_DATA pin ***/
#define ALED_B_DATA_Get()               ((PORTD >> 1) & 0x1)
#define ALED_B_DATA_PIN                  GPIO_PIN_RD1

/*** Macros for ALED_C_DATA pin ***/
#define ALED_C_DATA_Get()               ((PORTD >> 2) & 0x1)
#define ALED_C_DATA_PIN                  GPIO_PIN_RD2


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2,
    GPIO_PORT_D = 3,
    GPIO_PORT_E = 4,
    GPIO_PORT_F = 5,
    GPIO_PORT_G = 6,
    GPIO_PORT_H = 7,
    GPIO_PORT_J = 8,
    GPIO_PORT_K = 9,
} GPIO_PORT;

typedef enum
{
    GPIO_INTERRUPT_ON_MISMATCH,
    GPIO_INTERRUPT_ON_RISING_EDGE,
    GPIO_INTERRUPT_ON_FALLING_EDGE,
    GPIO_INTERRUPT_ON_BOTH_EDGES,
}GPIO_INTERRUPT_STYLE;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RA0 = 0,
    GPIO_PIN_RA1 = 1,
    GPIO_PIN_RA2 = 2,
    GPIO_PIN_RA3 = 3,
    GPIO_PIN_RA4 = 4,
    GPIO_PIN_RA5 = 5,
    GPIO_PIN_RA6 = 6,
    GPIO_PIN_RA7 = 7,
    GPIO_PIN_RA9 = 9,
    GPIO_PIN_RA10 = 10,
    GPIO_PIN_RA14 = 14,
    GPIO_PIN_RA15 = 15,
    GPIO_PIN_RB0 = 16,
    GPIO_PIN_RB1 = 17,
    GPIO_PIN_RB2 = 18,
    GPIO_PIN_RB3 = 19,
    GPIO_PIN_RB4 = 20,
    GPIO_PIN_RB5 = 21,
    GPIO_PIN_RB6 = 22,
    GPIO_PIN_RB7 = 23,
    GPIO_PIN_RB8 = 24,
    GPIO_PIN_RB9 = 25,
    GPIO_PIN_RB10 = 26,
    GPIO_PIN_RB11 = 27,
    GPIO_PIN_RB12 = 28,
    GPIO_PIN_RB13 = 29,
    GPIO_PIN_RB14 = 30,
    GPIO_PIN_RB15 = 31,
    GPIO_PIN_RC1 = 33,
    GPIO_PIN_RC2 = 34,
    GPIO_PIN_RC3 = 35,
    GPIO_PIN_RC4 = 36,
    GPIO_PIN_RC12 = 44,
    GPIO_PIN_RC13 = 45,
    GPIO_PIN_RC14 = 46,
    GPIO_PIN_RC15 = 47,
    GPIO_PIN_RD0 = 48,
    GPIO_PIN_RD1 = 49,
    GPIO_PIN_RD2 = 50,
    GPIO_PIN_RD3 = 51,
    GPIO_PIN_RD4 = 52,
    GPIO_PIN_RD5 = 53,
    GPIO_PIN_RD6 = 54,
    GPIO_PIN_RD7 = 55,
    GPIO_PIN_RD9 = 57,
    GPIO_PIN_RD10 = 58,
    GPIO_PIN_RD11 = 59,
    GPIO_PIN_RD12 = 60,
    GPIO_PIN_RD13 = 61,
    GPIO_PIN_RD14 = 62,
    GPIO_PIN_RD15 = 63,
    GPIO_PIN_RE0 = 64,
    GPIO_PIN_RE1 = 65,
    GPIO_PIN_RE2 = 66,
    GPIO_PIN_RE3 = 67,
    GPIO_PIN_RE4 = 68,
    GPIO_PIN_RE5 = 69,
    GPIO_PIN_RE6 = 70,
    GPIO_PIN_RE7 = 71,
    GPIO_PIN_RE8 = 72,
    GPIO_PIN_RE9 = 73,
    GPIO_PIN_RF0 = 80,
    GPIO_PIN_RF1 = 81,
    GPIO_PIN_RF2 = 82,
    GPIO_PIN_RF3 = 83,
    GPIO_PIN_RF4 = 84,
    GPIO_PIN_RF5 = 85,
    GPIO_PIN_RF8 = 88,
    GPIO_PIN_RF12 = 92,
    GPIO_PIN_RF13 = 93,
    GPIO_PIN_RG0 = 96,
    GPIO_PIN_RG1 = 97,
    GPIO_PIN_RG6 = 102,
    GPIO_PIN_RG7 = 103,
    GPIO_PIN_RG8 = 104,
    GPIO_PIN_RG9 = 105,
    GPIO_PIN_RG12 = 108,
    GPIO_PIN_RG13 = 109,
    GPIO_PIN_RG14 = 110,
    GPIO_PIN_RG15 = 111,
    GPIO_PIN_RH0 = 112,
    GPIO_PIN_RH1 = 113,
    GPIO_PIN_RH2 = 114,
    GPIO_PIN_RH3 = 115,
    GPIO_PIN_RH4 = 116,
    GPIO_PIN_RH5 = 117,
    GPIO_PIN_RH6 = 118,
    GPIO_PIN_RH7 = 119,
    GPIO_PIN_RH8 = 120,
    GPIO_PIN_RH9 = 121,
    GPIO_PIN_RH10 = 122,
    GPIO_PIN_RH11 = 123,
    GPIO_PIN_RH12 = 124,
    GPIO_PIN_RH13 = 125,
    GPIO_PIN_RH14 = 126,
    GPIO_PIN_RH15 = 127,
    GPIO_PIN_RJ0 = 128,
    GPIO_PIN_RJ1 = 129,
    GPIO_PIN_RJ2 = 130,
    GPIO_PIN_RJ3 = 131,
    GPIO_PIN_RJ4 = 132,
    GPIO_PIN_RJ5 = 133,
    GPIO_PIN_RJ6 = 134,
    GPIO_PIN_RJ7 = 135,
    GPIO_PIN_RJ8 = 136,
    GPIO_PIN_RJ9 = 137,
    GPIO_PIN_RJ10 = 138,
    GPIO_PIN_RJ11 = 139,
    GPIO_PIN_RJ12 = 140,
    GPIO_PIN_RJ13 = 141,
    GPIO_PIN_RJ14 = 142,
    GPIO_PIN_RJ15 = 143,
    GPIO_PIN_RK0 = 144,
    GPIO_PIN_RK1 = 145,
    GPIO_PIN_RK2 = 146,
    GPIO_PIN_RK3 = 147,
    GPIO_PIN_RK4 = 148,
    GPIO_PIN_RK5 = 149,
    GPIO_PIN_RK6 = 150,
    GPIO_PIN_RK7 = 151,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;

typedef  void (*GPIO_PIN_CALLBACK) ( GPIO_PIN pin, uintptr_t context);

void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    GPIO_PIN                 pin;

    /* Callback for event on target pin*/
    GPIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} GPIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

#define GPIO_PinInterruptEnable(pin)       GPIO_PinIntEnable(pin, GPIO_INTERRUPT_ON_MISMATCH)
#define GPIO_PinInterruptDisable(pin)      GPIO_PinIntDisable(pin)

void GPIO_PinIntEnable(GPIO_PIN pin, GPIO_INTERRUPT_STYLE style);
void GPIO_PinIntDisable(GPIO_PIN pin);

bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const   GPIO_PIN_CALLBACK callBack,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
