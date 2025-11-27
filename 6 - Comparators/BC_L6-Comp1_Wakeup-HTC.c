/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_Wakeup-HTC.c                             *
*   Date:          9/8/12                                               *
*   File Version:  1.3                                                  *
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
*   Description:    Lesson 6, example 2                                 *
*                                                                       *
*   Demonstrates wake-up on comparator change                           *
*                                                                       *
*   Turns on LED for 1 s when comparator 1 output changes,              *
*   then sleeps until the next change                                   *
*   (internal 0.6 V reference with hysteresis)                          *
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

#define _XTAL_FREQ  4000000     // oscillator frequency for delay functions


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
    LED = 0;                    // start with LED off
    TRISC = ~(1<<nLED);         // configure LED pin (only) as an output
    ADCON0 = 0;                 // disable analog inputs -> C1OUT usable    
    
    // check for wake-up on comparator change
    if (!STATUSbits.CWUF)
    {
        // power-on reset has occurred:
        
        // configure comparator 1
        CM1CON0bits.C1PREF = 1;     // +ref is C1IN+
        CM1CON0bits.C1NREF = 0;     // -ref is 0.6 V internal ref
        CM1CON0bits.C1POL = 1;      // normal polarity (C1IN+ > 0.6 V)
        CM1CON0bits.nC1OUTEN = 0;   // enable C1OUT (for hysteresis feedback)
        CM1CON0bits.nC1WU = 0;      // enable wake-up on change
        CM1CON0bits.C1ON = 1;       // turn comparator on
        
        // delay 10 ms to allow comparator to settle
        __delay_ms(10);
    }
    else
    {
        // wake-up on comparator change occurred:
        
        // flash LED
        LED = 1;                // turn on LED
        __delay_ms(1000);       // delay 1 sec
    }
    
    //*** Sleep until comparator change
    LED = 0;                    // turn off LED
    CM1CON0;                    // read comparator to clear mismatch condition
    SLEEP();                    // enter sleep mode
}
