/************************************************************************
*                                                                       *
*   Filename:      BC_L7-ADC_4LEDs-HTC.c                                *
*   Date:          9/8/12                                               *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        David Meiklejohn                                     *
*   Company:       Gooligum Electronics                                 *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     16F506                                               *
*   Compiler:      MPLAB XC8 v1.01 (Free mode)                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 7, example 1                                 *
*                                                                       *
*   Demonstrates basic use of ADC                                       *
*                                                                       *
*   Continuously samples analog input, copying value to 4 x LEDs        *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       AN0     = voltage to be measured (e.g. pot output)              *
*       RC0-3   = output LEDs (RC3 is MSB)                              *
*                                                                       *
************************************************************************/

#include <xc.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);

// Pin assignments
#define LEDS    PORTC           // output LEDs on RC0-RC3


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure ports
    TRISC = 0b110000;           // configure RC0-RC3 as outputs
    CM2CON0 = 0;                // disable comparator 2 -> RC0, RC1 digital
    VRCON = 0;                  // disable CVref -> RC2 usable
    
    // configure ADC    
    ADCON0bits.ADCS = 0b11;     // clock = INTOSC/4  
    ADCON0bits.ANS  = 0b10;     // AN0, AN2 analog
    ADCON0bits.CHS  = 0b00;     // select channel AN0 
    ADCON0bits.ADON = 1;        // turn ADC on
                                // -> AN0 ready for sampling

    
    //*** Main loop
    for (;;)
    {
        // sample analog input
        ADCON0bits.GO = 1;          // start conversion
        while (ADCON0bits.nDONE)    // wait until done
            ;
        
        // display result on 4 x LEDs
        LEDS = ADRES >> 4;      // copy high nybble of result to LEDs
    }
}
