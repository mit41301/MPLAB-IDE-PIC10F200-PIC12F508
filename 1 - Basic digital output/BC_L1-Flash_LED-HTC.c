/************************************************************************
*                                                                       *
*   Filename:      BC_L1-Flash_LED-HTC.c                                *
*   Date:          7/6/12                                               *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        David Meiklejohn                                     *
*   Company:       Gooligum Electronics                                 *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F508/509                                           *
*   Compiler:      MPLAB XC8 v1.00 (Free mode)                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 1, example 3                                 *
*                                                                       *
*   Flashes an LED at approx 1 Hz.                                      *
*   LED continues to flash until power is removed.                      *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = flashing LED                                              *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & OSC_IntRC);

#define _XTAL_FREQ  4000000     // oscillator frequency for _delay()


/***** GLOBAL VARIABLES *****/
uint8_t     sGPIO = 0;          // shadow copy of GPIO


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation
    TRIS = 0b111101;            // configure GP1 (only) as an output

    // Main loop
    for (;;)
    {
        // toggle LED on GP1
        sGPIO ^= 0b000010;      // toggle shadow bit corresponding to GP1
        GPIO = sGPIO;           // write to GPIO

        // delay 500 ms
        __delay_ms(500);

    }   // repeat forever
}
