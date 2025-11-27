/************************************************************************
*                                                                       *
*   Filename:      BC_L6-Comp1_TMR-HTC.c                                *
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
*   Description:    Lesson 6, example 3                                 *
*                   Crystal-based (degraded signal) LED flasher         *
*                                                                       *
*   Demonstrates use of comparator 1 to clock TMR0                      *
*                                                                       *
*   LED flashes at 1 Hz (50% duty cycle),                               *
*   with timing derived from 32.768 kHz input on C1IN+                  *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       C1IN+ = 32.768 kHz signal                                       *
*       RC3   = flashing LED                                            *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);

// Pin assignments
#define sFLASH  sPORTC.RC3          // flashing LED (shadow)


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
    TRISC = 0b110111;               // configure RC3 (only) as an output
    
    // configure timer
    OPTION = T0CS & ~PSA | 0b110;   // configure Timer0:
                                    //   counter mode (T0CS = 1)
                                    //   prescaler assigned to Timer0 (PSA = 0)
                                    //   prescale = 128 (PS = 110)
                                    //   -> incr at 256 Hz with 32.768 kHz input
                                    
    // configure comparator 1
    CM1CON0bits.C1PREF = 1;         // +ref is C1IN+
    CM1CON0bits.C1NREF = 0;         // -ref is 0.6 V internal ref
    CM1CON0bits.C1POL = 1;          // normal polarity (C1IN+ > 0.6 V)
    CM1CON0bits.nC1T0CS = 0;        // enable TMR0 clock source
    CM1CON0bits.C1ON = 1;           // turn comparator on
                                    // -> C1OUT = 1 if C1IN+ > 0.6 V,
                                    //    TMR0 clock from C1
    
      
    //*** Main loop
    for (;;)
    {
        // TMR0<7> cycles at 1 Hz, so continually copy to LED
        sFLASH = (TMR0 & 1<<7) != 0;    // sFLASH = TMR0<7>
        
        PORTC = sPORTC.port;            // copy shadow to PORTC
    }
}
