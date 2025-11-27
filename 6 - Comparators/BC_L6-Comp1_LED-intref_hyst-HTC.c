/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_LED-intref_hyst-HTC.c                    *
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
*   Description:    Lesson 6, example 1d                                *
*                                                                       *
*   Demonstrates use of hysteresis with C1 internal 0.6 V reference     *
*                                                                       *
*   Turns on LED when voltage on C1IN+ < 0.6 V                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = voltage to be measured (e.g. pot output or LDR)         *
*       C1OUT = comparator output (fed back to input via resistor)      *
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
    ADCON0 = 0;                 // disable analog inputs -> C1OUT usable
    
    // configure comparator 1
    CM1CON0bits.C1PREF = 1;     // +ref is C1IN+
    CM1CON0bits.C1NREF = 0;     // -ref is 0.6 V internal ref
    CM1CON0bits.C1POL = 1;      // normal polarity (C1IN+ > 0.6 V)
    CM1CON0bits.nC1OUTEN = 0;   // enable C1OUT (for hysteresis feedback)
    CM1CON0bits.C1ON = 1;       // turn comparator on

    
    //*** Main loop
    for (;;)
    {
        // continually display comparator output (inverted)
        LED = !CM1CON0bits.C1OUT;  
    }
}
