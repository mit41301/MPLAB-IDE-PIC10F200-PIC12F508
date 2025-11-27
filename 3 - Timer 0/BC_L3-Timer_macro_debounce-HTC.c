/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Timer_macro_debounce-HTC.c                     *
*   Date:          5/8/12                                               *
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
*   Files required: stdmacros.h     (provides debounce macros)          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 3, example 3b                                *
*                                                                       *
*   Demonstrates use of Timer0-based debounce macros                    *
*                                                                       *
*   Toggles LED when pushbutton is pressed then released                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = flashing LED                                              *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>

#include "stdmacros-HTC.h"  // DbnceHi() - debounce switch, wait for high
                            // DbnceLo() - debounce switch, wait for low
                            // Requires: TMR0 at 256 us/tick

/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_OFF & CP_OFF & WDT_OFF & OSC_IntRC);

// Pin assignments
#define sFLASH  sGPIO.GP1           // flashing LED (shadow)
#define BUTTON  GPIObits.GP3        // pushbutton


/***** GLOBAL VARIABLES *****/
union {                             // shadow copy of GPIO
    uint8_t         port;
    struct {
        unsigned    GP0     : 1;
        unsigned    GP1     : 1;
        unsigned    GP2     : 1;
        unsigned    GP3     : 1;
        unsigned    GP4     : 1;
        unsigned    GP5     : 1;
    };
} sGPIO;


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure port
    GPIO = 0;                       // start with LED off
    sGPIO.port = 0;                 //   update shadow
    TRIS = 0b111101;                // configure GP1 (only) as an output
    
    // configure timer
    OPTION = 0b11010111;            // configure Timer0:
             //--0-----                 timer mode (T0CS = 0)
             //----0---                 prescaler assigned to Timer0 (PSA = 0)
             //-----111                 prescale = 256 (PS = 111)
             //                         -> increment every 256 us

    
    //*** Main loop
    for (;;)
    {
        // wait for button press 
        while (BUTTON == 1)         // wait until button low
            ;
        
        // toggle LED
        sFLASH = !sFLASH;           // toggle flashing LED (shadow)
        GPIO = sGPIO.port;          // write to GPIO

        // wait for button release
        DbnceHi(BUTTON);            // wait until button high (debounced)
    } 
}
