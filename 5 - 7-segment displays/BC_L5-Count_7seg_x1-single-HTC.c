/************************************************************************
*                                                                       *
*   Filename:      BC_15-Count_7seg_x1-single-HTC.c                     *
*   Date:          8/8/12                                               *
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
*   Description:    Lesson 5, example 1b                                *
*                                                                       *
*   Demonstrates use of lookup tables to drive a 7-segment display      *
*                                                                       *
*   Single digit 7-segment display counts repeating 0 -> 9              *
*   1 second per count, with timing derived from int 4 MHz oscillator   *
*   (single pattern lookup array)                                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       RB0-1,RB4, RC1-4 = 7-segment display bus (common cathode)       *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ  4000000     // oscillator frequency for delay functions


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);


/***** LOOKUP TABLES *****/

// pattern table for 7 segment display on ports B and C
const uint8_t pat7seg[10] = {
    // RC4:1,RB4,RB1:0 = CDBAEFG
    0b1111110,  // 0
    0b1010000,  // 1
    0b0111101,  // 2
    0b1111001,  // 3
    0b1010011,  // 4
    0b1101011,  // 5
    0b1101111,  // 6
    0b1011000,  // 7
    0b1111111,  // 8
    0b1111011   // 9
}; 


/***** MAIN PROGRAM *****/
void main()
{
    uint8_t     digit;              // digit to be displayed
    
    //*** Initialisation
    
    // configure ports
    TRISB = 0;                      // configure PORTB and PORTC as all outputs
    TRISC = 0;
    ADCON0 = 0;                     // disable AN0, AN1, AN2 inputs
    CM1CON0bits.C1ON = 0;           //     and comparator 1 -> RB0,RB1 digital
    CM2CON0bits.C2ON = 0;           // disable comparator 2 -> RC1 digital

        
    //*** Main loop
    for (;;)
    {
        // display each digit from 0 to 9 for 1 sec
        for (digit = 0; digit < 10; digit++)
        {
            // display digit by extracting pattern bits for all pins
            PORTB = (pat7seg[digit] & 1<<2) << 2 |      // RB4 
                    (pat7seg[digit] & 0b00000011);      // RB0-1  
            PORTC = (pat7seg[digit] >> 2) & 0b011110;   // RC1-4  
        
            // delay 1 sec
            __delay_ms(1000);
        }  
    }      
}
