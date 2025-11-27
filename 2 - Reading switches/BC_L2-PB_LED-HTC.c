/************************************************************************
*                                                                       *
*   Filename:      BC_L2-PB_LED-HTC.c                                   *
*   Date:          4/8/12                                               *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        David Meiklejohn                                     *
*   Company:       Gooligum Electronics                                 *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F508/509                                           *
*   Compiler:      MPLAB XC8 v1.01 (Free mode)                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 2, example 1                                 *
*                                                                       *
*   Demonstrates reading a switch                                       *
*                                                                       *
*   Turns on LED when pushbutton is pressed                             *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <xc.h>


/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_OFF & CP_OFF & WDT_OFF & OSC_IntRC);


/***** MAIN PROGRAM *****/
void main()
{
    // Initialisation
    TRIS = 0b111101;            // configure GP1 (only) as an output

    // Main loop
    for (;;)
    {
        // turn on LED only if button pressed
        GPIO = GPIObits.GP3 ? 0 : 0b000010;     // if GP3 high, clear GP1
                                                //else set GP1
    }  
}
