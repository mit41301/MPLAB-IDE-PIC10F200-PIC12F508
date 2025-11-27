/************************************************************************
*                                                                       *
*   Filename:      BC_L1-Turn_on_LED-12F508-HTC.c                       *
*   Date:          7/6/12                                               *
*   File Version:  1.1                                                  *
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
*   Description:    Lesson 1, example 1                                 *
*                                                                       *
*   Turns on LED.  LED remains on until power is removed.               *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*                                                                       *
************************************************************************/

#include <xc.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & OSC_IntRC);


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation
    TRIS = 0b111101;        // configure GP1 (only) as an output

    GPIO = 0b000010;        // set GP1 high

    // Main loop
    for (;;)
    {                       // loop forever
        ;
    }
}
