/************************************************************************
*                                                                       *
*   Filename:      BC_L4-WDTdemo+LED-HTC.c                              *
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
*   Description:    Lesson 4, example 3a                                *
*                                                                       *
*   Demonstrates watchdog timer                                         *
*       plus differentiation from POR reset                             *
*                                                                       *
*   Turn on LED for 1 s, turn off, then enter endless loop              *
*   If enabled, WDT timer restarts after 2.3 s -> LED flashes           *
*   Turns on WDT LED to indicate WDT reset                              *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = flashing LED                                              *
*       GP2 = WDT-reset indicator LED                                   *
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
#define LED     GPIObits.GP1    // LED to flash
#define nLED    1               //   (port bit 1)
#define WDT     GPIObits.GP2    // watchdog timer reset indicator
#define nWDT    2               //   (port bit 2)


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure port
    GPIO = 0;                   // start with all LEDs off
    TRIS = ~(1<<nLED|1<<nWDT);  // configure LED pins as outputs
    
    // configure watchdog timer
    OPTION = PSA | 0b111;       // prescaler assigned to WDT (PSA = 1)
                                // prescale = 128 (PS = 111)
                                // -> WDT period = 2.3 s

    
    //*** Main code  
    
    // test for WDT-timeout reset
    if (!STATUSbits.nTO)        // if WDT timeout has occurred,
        WDT = 1;                //   turn on "error" LED

    // flash LED
    LED = 1;                    // turn on "flash" LED
    __delay_ms(1000);           // delay 1 sec
    LED = 0;                    // turn off "flash" LED 
 
    // wait forever
    for (;;)                    
        ;
}
