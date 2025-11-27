/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp2_2LEDs-HTC.c                              *
*   Date:          8/8/12                                               *
*   File Version:  3.2                                                  *
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
*   Description:    Lesson 6, example 4                                 *
*                                                                       *
*   Demonstrates use of comparator 2 and programmable voltage reference *
*                                                                       *
*   Turns on Low LED  when C2IN+ < 2.0 V (low light level)              *
*         or High LED when C2IN+ > 3.0 V (high light level)             *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C2IN+ = voltage to be measured (LDR/resistor divider)           *
*       RC3   = "Low" LED                                               *
*       RC1   = "High" LED                                              *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ  4000000     // oscillator frequency for delay functions


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);

// Pin assignments
#define sLO     sPORTC.RC3          // "Low" LED (shadow)
#define sHI     sPORTC.RC1          // "High" LED (shadow)


/***** GLOBAL VARIABLES *****/
union {                             // shadow copy of PORTC
    uint8_t         port;
    struct {
        unsigned    RC0     : 1;
        unsigned    RC1     : 1;
        unsigned    RC2     : 1;
        unsigned    RC3     : 1;
        unsigned    RC4     : 1;
        unsigned    RC5     : 1;
    };
} sPORTC;


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure ports
    TRISC = 0b110101;               // configure RC1 and RC3 (only) as outputs
    
    // configure comparator 2
    CM2CON0bits.C2PREF1 = 1;        // +ref is C2IN+
    CM2CON0bits.C2NREF = 0;         // -ref is CVref
    CM2CON0bits.C2POL = 1;          // normal polarity (C2IN+ > CVref)
    CM2CON0bits.C2ON = 1;           // turn comparator on  
                                    // -> C2OUT = 1 if C2IN+ > CVref  
    
    // configure voltage reference
    VRCONbits.VRR = 0;              // CVref = 0.406*Vdd (2.03V) if VR = 5
                                    //      or 0.594*Vdd (2.97V) if VR = 11
    VRCONbits.VREN = 1;             // turn voltage reference on

    
    //*** Main loop
    for (;;)
    {
        // Test for low illumination
        VRCONbits.VR = 5;           // select low CVref (2.03 V)
        __delay_us(10);             // wait 10 us to settle
        sLO = !CM2CON0bits.C2OUT;   // if C2IN+ < CVref turn on Low LED
        
        // Test for high illumination
        VRCONbits.VR = 11;          // select high CVref (2.97 V)
        __delay_us(10);             // wait 10 us to settle
        sHI = CM2CON0bits.C2OUT;    // if C2IN+ > CVref turn on High LED
        
        // Display test results
        PORTC = sPORTC.port;        // copy shadow to PORTC
    }
}
