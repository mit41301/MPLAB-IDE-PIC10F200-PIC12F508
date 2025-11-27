/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Flash_LED_XTAL-HTC.c                           *
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
*   Description:    Lesson 3, example 4                                 *
*                                                                       *
*   Demonstrates use of Timer0 in counter mode                          *
*                                                                       *
*   LED flashes at 1 Hz (50% duty cycle),                               *
*   with timing derived from 32.768 kHz input on T0CKI                  *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1   = flashing LED                                            *
*       T0CKI = 32.768 kHz signal                                       *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, int RC clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & OSC_IntRC);

// Pin assignments
#define sFLASH  sGPIO.GP1           // flashing LED (shadow)


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
    TRIS = 0b111101;                // configure GP1 (only) as an output
    
    // configure timer    
    OPTION = 0b11110110;            // configure Timer0:
             //--1-----                 counter mode (T0CS = 1)
             //----0---                 prescaler assigned to Timer0 (PSA = 0)
             //-----110                 prescale = 128 (PS = 110)
             //                         -> increment at 256 Hz with 32.768 kHz input
                 
    //*** Main loop
    for (;;)
    {
        // TMR0<7> cycles at 1 Hz, so continually copy to LED
        sFLASH = (TMR0 & 1<<7) != 0;    // sFLASH = TMR0<7>
        
        GPIO = sGPIO.port;              // copy shadow to GPIO
        
    }   // repeat forever
}
