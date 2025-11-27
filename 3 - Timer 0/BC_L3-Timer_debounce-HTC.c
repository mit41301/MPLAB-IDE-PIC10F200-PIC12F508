/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Timer_debounce-HTC.c                           *
*   Date:          5/8/12                                               *
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
*   Description:    Lesson 3, example 3a                                *
*                                                                       *
*   Demonstrates use of Timer0 to implement debounce counting algorithm *
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
    OPTION = 0b11010101;            // configure Timer0:
             //--0-----                 timer mode (T0CS = 0)
             //----0---                 prescaler assigned to Timer0 (PSA = 0)
             //-----101                 prescale = 64 (PS = 101)
             //                         -> increment every 64 us


    //*** Main loop 
    for (;;)
	{
        // wait for button press, debounce using timer0:
        TMR0 = 0;                   // reset timer
        while (TMR0 < 157)          // wait at least 10 ms (157 x 64 us = 10 ms)
            if (BUTTON == 1)        //   if button up,
                TMR0 = 0;           //     restart wait
        
        // toggle LED
        sFLASH = !sFLASH;           // toggle flashing LED (shadow)
        GPIO = sGPIO.port;          // write to GPIO

        // wait for button release, debounce using timer0:
        TMR0 = 0;                   // reset timer
        while (TMR0 < 157)          // wait at least 10ms (157 x 64us = 10 ms)
            if (BUTTON == 0)        //   if button down,
                TMR0 = 0;           //     restart wait
    }   
}

