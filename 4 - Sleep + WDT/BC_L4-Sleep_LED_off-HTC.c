/************************************************************************
*                                                                       *
*   Filename:      BC_L4-Sleep_LED_off-HTC.c                            *
*   Date:          8/8/12                                               *
*   File Version:  1.3                                                  *
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
*   Description:    Lesson 4, example 1                                 *
*                                                                       *
*   Demonstrates sleep mode                                             *
*                                                                       *
*   Turn on LED, wait for button pressed, turn off LED, then sleep      *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*       GP3 = pushbutton (active low)                                   *
*                                                                       *
************************************************************************/

#include <xc.h>


/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_OFF & CP_OFF & WDT_OFF & OSC_IntRC);

// Pin assignments
#define LED     GPIObits.GP1    // Indicator LED on GP1
#define nLED    1               //   (port bit 1)
#define BUTTON  GPIObits.GP3    // Pushbutton (active low)


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure port
    TRIS = ~(1<<nLED);          // configure LED pin (only) as an output


    //*** Main code 
     
    // turn on LED
    LED = 1;                    

    // wait for button press
    while (BUTTON == 1)         // wait until button low
        ;

    // go into standby (low power) mode
    LED = 0;                    // turn off LED
    SLEEP();                    // enter sleep mode

    for (;;)                    // (this loop should never execute)
        ;
}
