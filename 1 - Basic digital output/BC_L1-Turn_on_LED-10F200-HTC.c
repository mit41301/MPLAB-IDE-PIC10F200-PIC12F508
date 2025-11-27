/************************************************************************
*                                                                       *
*   Filename:      BC_L1-Turn_on_LED-10F200-HTC.c                       *
*   Date:          19/9/12                                              *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        David Meiklejohn                                     *
*   Company:       Gooligum Electronics                                 *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     10F200                                               *
*   Compiler:      MPLAB XC8 v1.10 (Free mode)                          *
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
// ext reset, no code protect, no watchdog
__CONFIG(MCLRE_ON & CP_OFF & WDTE_OFF);


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation
    TRIS = 0b1101;          // configure GP1 (only) as an output

    GPIO = 0b0010;          // set GP1 high

    // Main loop
    for (;;)
    {                       // loop forever
        ;
    }
}
