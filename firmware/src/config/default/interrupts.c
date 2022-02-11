/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
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
#include "interrupts.h"
#include "definitions.h"


// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


void CORE_TIMER_InterruptHandler( void );
void INPUT_CAPTURE_1_InterruptHandler( void );
void TIMER_2_InterruptHandler( void );
void INPUT_CAPTURE_2_InterruptHandler( void );
void TIMER_3_InterruptHandler( void );
void INPUT_CAPTURE_3_InterruptHandler( void );
void TIMER_5_InterruptHandler( void );
void TIMER_6_InterruptHandler( void );
void TIMER_7_InterruptHandler( void );
void ADC_DF1_InterruptHandler( void );
void CHANGE_NOTICE_H_InterruptHandler( void );
void DRV_USBHS_InterruptHandler( void );
void DRV_USBHS_DMAInterruptHandler( void );
void DMA0_InterruptHandler( void );
void DMA1_InterruptHandler( void );
void DMA2_InterruptHandler( void );
void NVM_InterruptHandler( void );



/* All the handlers are defined here.  Each will call its PLIB-specific function. */
void __ISR(_CORE_TIMER_VECTOR, ipl1SRS) CORE_TIMER_Handler (void)
{
    CORE_TIMER_InterruptHandler();
}

void __ISR(_INPUT_CAPTURE_1_VECTOR, ipl1SRS) INPUT_CAPTURE_1_Handler (void)
{
    INPUT_CAPTURE_1_InterruptHandler();
}

void __ISR(_TIMER_2_VECTOR, ipl1SRS) TIMER_2_Handler (void)
{
    TIMER_2_InterruptHandler();
}

void __ISR(_INPUT_CAPTURE_2_VECTOR, ipl1SRS) INPUT_CAPTURE_2_Handler (void)
{
    INPUT_CAPTURE_2_InterruptHandler();
}

void __ISR(_TIMER_3_VECTOR, ipl1SRS) TIMER_3_Handler (void)
{
    TIMER_3_InterruptHandler();
}

void __ISR(_INPUT_CAPTURE_3_VECTOR, ipl1SRS) INPUT_CAPTURE_3_Handler (void)
{
    INPUT_CAPTURE_3_InterruptHandler();
}

void __ISR(_TIMER_5_VECTOR, ipl1SRS) TIMER_5_Handler (void)
{
    TIMER_5_InterruptHandler();
}

void __ISR(_TIMER_6_VECTOR, ipl1SRS) TIMER_6_Handler (void)
{
    TIMER_6_InterruptHandler();
}

void __ISR(_TIMER_7_VECTOR, ipl1SRS) TIMER_7_Handler (void)
{
    TIMER_7_InterruptHandler();
}

void __ISR(_ADC_DF1_VECTOR, ipl1SRS) ADC_DF1_Handler (void)
{
    ADC_DF1_InterruptHandler();
}

void __ISR(_CHANGE_NOTICE_H_VECTOR, ipl1SRS) CHANGE_NOTICE_H_Handler (void)
{
    CHANGE_NOTICE_H_InterruptHandler();
}

void __ISR(_USB_VECTOR, ipl1SRS) USB_Handler (void)
{
    DRV_USBHS_InterruptHandler();
}

void __ISR(_USB_DMA_VECTOR, ipl1SRS) USB_DMA_Handler (void)
{
    DRV_USBHS_DMAInterruptHandler();
}

void __ISR(_DMA0_VECTOR, ipl1SRS) DMA0_Handler (void)
{
    DMA0_InterruptHandler();
}

void __ISR(_DMA1_VECTOR, ipl1SRS) DMA1_Handler (void)
{
    DMA1_InterruptHandler();
}

void __ISR(_DMA2_VECTOR, ipl1SRS) DMA2_Handler (void)
{
    DMA2_InterruptHandler();
}

void __ISR(_FLASH_CONTROL_VECTOR, ipl1SRS) FLASH_CONTROL_Handler (void)
{
    NVM_InterruptHandler();
}





/*******************************************************************************
 End of File
*/
