/************************************************************************
*                                                                       *
*   Filename:      BC_L4-WDTsleep-HTC.c                                 *
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
*   Description:    Lesson 4, example 3c                                *
*                                                                       *
*   Demonstrates periodic wake from sleep                               *
*                                                                       *
*   Turn on LED for 1 s, turn off, then sleep                           *
*   LED stays off if watchdog not enabled,                              *
*   flashes if WDT enabled and period set to 2.3 s                      *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = indicator LED                                             *
*                                                                       *
************************************************************************/

#include <xc.h>

#define _XTAL_FREQ  4000000     // oscillator frequency for _delay()


/***** CONFIGURATION *****/
#define     WATCHDOG            // define to enable watchdog timer

#ifdef WATCHDOG
    // ext reset, no code protect, watchdog, int RC clock
    __CONFIG(MCLRE_ON & CP_OFF & WDT_ON & OSC_IntRC);
#else
     // ext reset, no code protect, no watchdog, int RC clock
    __CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & OSC_IntRC);
#endif   

// Pin assignments
#define LED     GPIObits.GP1    // indicator LED
#define nLED    1               //   (port bit 1)


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure port
    TRIS = ~(1<<nLED);          // configure LED pin (only) as an output
    
    // configure watchdog timer
    OPTION = PSA | 0b111;       // prescaler assigned to WDT (PSA = 1)
                                // prescale = 128 (PS = 111)
                                // -> WDT period = 2.3 s


    //*** Main code
    LED = 1;                    // turn on LED
    
    __delay_ms(1000);           // delay 1 sec
   
    LED = 0;                    // turn off LED 
    
    SLEEP();                    // enter sleep mode
}
