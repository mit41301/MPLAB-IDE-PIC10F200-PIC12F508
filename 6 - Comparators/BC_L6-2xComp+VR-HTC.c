/************************************************************************
*                                                                       *
*   Filename:      BC_L6-2xComp+VR-HTC.c                                *
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
*   Description:    Lesson 6, example 5                                 *
*                                                                       *
*   Demonstrates use of comparators 1 and 2                             *
*   with the programmable voltage reference                             *
*                                                                       *
*   Turns on: LED 1 when C1IN+ > 2.5 V                                  *
*         and LED 2 when C2IN+ > 2.5 V                                  *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = input 1 (LDR/resistor divider)                          *
*       C1IN- = connected to CVref                                      *
*       C2IN+ = input 2 (LDR/resistor divider)                          *
*       CVref = connected to C1IN-                                      *
*       RC1   = indicator LED 2                                         *
*       RC3   = indicator LED 1                                         *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>

                           
/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);

// Pin assignments
#define sLED1   sPORTC.RC3      // indicator LED 1
#define sLED2   sPORTC.RC1      // indicator LED 2


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

    // configure comparator 1
    CM1CON0bits.C1PREF = 1;         // +ref is C1IN+
    CM1CON0bits.C1NREF = 1;         // -ref is C1IN- (= CVref)
    CM1CON0bits.C1POL = 1;          // normal polarity (C1IN+ > C1IN-)
    CM1CON0bits.C1ON = 1;           // turn comparator on   
                                    // -> C1OUT = 1 if C1IN+ > C1IN- (= CVref)
            
    // configure comparator 2
    CM2CON0bits.C2PREF1 = 1;        // +ref is C2IN+
    CM2CON0bits.C2NREF = 0;         // -ref is CVref
    CM2CON0bits.C2POL = 1;          // normal polarity (C2IN+ > CVref)
    CM2CON0bits.C2ON = 1;           // turn comparator on  
                                    // -> C2OUT = 1 if C2IN+ > CVref  
    
    // configure voltage reference
    VRCONbits.VRR = 1;              // select low range
    VRCONbits.VR = 12;              // CVref = 0.500*Vdd
    VRCONbits.VROE = 1;             // enable CVref output pin
    VRCONbits.VREN = 1;             // turn voltage reference on
                                    // -> CVref = 2.50 V (if Vdd = 5 V),
                                    //    CVref output pin enabled

    
    //*** Main loop
    for (;;)
    {
        // start with shadow PORTC clear
        sPORTC.port = 0;
        
        // test comparator inputs
        sLED1 = CM1CON0bits.C1OUT;  // turn on LED 1 if C1IN+ > CVref
        sLED2 = CM2CON0bits.C2OUT;  // turn on LED 2 if C2IN+ > CVref
        
        // display test results
        PORTC = sPORTC.port;        // copy shadow to PORTC        
    }
}
