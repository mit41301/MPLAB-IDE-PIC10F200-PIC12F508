/************************************************************************
*                                                                       *
*   Filename:      BC_L4-Wakeup+LED-HTC.c                               *
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
*   Files required: stdmacros-HTC.h     (provides DbnceHi macro)        *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 4, example 2                                 *
*                                                                       *
*   Demonstrates wake-up on change                                      *
*       plus differentiation from POR reset                             *
*                                                                       *
*   Turn on LED after each reset                                        *
*   Turn on WAKE LED only if reset was due to wake on change            *
*   then wait for button press, turn off LEDs, debounce, then sleep     *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = on/off indicator LED                                      *
*       GP2 = wake-on-change indicator LED                              *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <xc.h>

#include "stdmacros-HTC.h"  // DbnceHi() - debounce switch, wait for high
                            // Requires: TMR0 at 256us/tick

/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_OFF & CP_OFF & WDT_OFF & OSC_IntRC);

// Pin assignments
#define LED     GPIObits.GP1    // LED to turn on/off
#define nLED    1               //   (port bit 1)
#define WAKE    GPIObits.GP2    // indicates wake on change condition
#define nWAKE   2               //   (port bit 2)
#define BUTTON  GPIObits.GP3    // Pushbutton (active low)


/***** MAIN PROGRAM *****/
void main()
{
    //***** Initialisation
    
    // configure port
    GPIO = 0;                   // start with both LEDs off    
    TRIS = ~(1<<nLED|1<<nWAKE); // configure LED pins as outputs
    
    // configure wake-on-change and timer
    OPTION = 0b01000111;        // configure wake-up on change and Timer0:
             //0-------             enable wake-up on change (/GPWU = 0)
             //--0-----             timer mode (T0CS = 0)
             //----0---             prescaler assigned to Timer0 (PSA = 0)
             //-----111             prescale = 256 (PS = 111)
             //                     -> increment every 256 us

    
    //***** Main code  
    
    // turn on LED
    LED = 1;    

    // test for wake-on-change reset    
    if (STATUSbits.GPWUF)       // if wake on change has occurred, 
    {                
        WAKE = 1;               //   turn on wake-up indicator
        DbnceHi(BUTTON);        //   wait for button to stop bouncing
    }
    
    // wait for button press
    while (BUTTON == 1)         // wait until button low
        ;
    
    // go into standby (low power) mode
    GPIO = 0;                   // turn off both LEDs
    
    DbnceHi(BUTTON);            // wait for stable button release
        
    SLEEP();                    // enter sleep mode
}
