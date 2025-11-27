/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_LED-HTC.c                                *
*   Date:          8/8/12                                               *
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
*   Description:    Lesson 6, example 1a                                *
*                                                                       *
*   Demonstrates basic use of comparator 1                              *
*                                                                       *
*   Turns on LED when voltage on C1IN+ > voltage on C1IN-               *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = voltage to be measured (e.g. pot output or LDR)         *
*       C1IN- = threshold voltage (set by voltage divider resistors)    *
*       RC3   = indicator LED                                           *
*                                                                       *
************************************************************************/

#include <xc.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);

// Pin assignments
#define LED     PORTCbits.RC3   // indicator LED on RC3
#define nLED    3               //   (port bit 3)


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure ports
    TRISC = ~(1<<nLED);         // configure LED pin (only) as an output
    
    // configure comparator 1
    CM1CON0bits.C1PREF = 1;     // +ref is C1IN+
    CM1CON0bits.C1NREF = 1;     // -ref is C1IN-
    CM1CON0bits.C1POL = 1;      // normal polarity (C1IN+ > C1IN-)
    CM1CON0bits.C1ON = 1;       // turn comparator on

    
    //*** Main loop
    for (;;)
    {
        // continually display comparator output
        LED = CM1CON0bits.C1OUT;            
    }
}
