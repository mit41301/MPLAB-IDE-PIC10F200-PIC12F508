/************************************************************************
*                                                                       *
*   Filename:      BC_L5-Count_7seg_x1-HTC.c                            *
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
*   Description:    Lesson 5, example 1a                                *
*                                                                       *
*   Demonstrates use of lookup tables to drive a 7-segment display      *
*                                                                       *
*   Single digit 7-segment display counts repeating 0 -> 9              *
*   1 second per count, with timing derived from int 4 MHz oscillator   *
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

// pattern table for 7 segment display on port B
const uint8_t pat7segB[10] = {
    // RB4 = E, RB1:0 = FG
    0b010010,   // 0
    0b000000,   // 1
    0b010001,   // 2
    0b000001,   // 3
    0b000011,   // 4
    0b000011,   // 5
    0b010011,   // 6
    0b000000,   // 7
    0b010011,   // 8
    0b000011    // 9    
}; 

// pattern table for 7 segment display on port C
const uint8_t pat7segC[10] = {
    // RC4:1 = CDBA
    0b011110,   // 0
    0b010100,   // 1
    0b001110,   // 2
    0b011110,   // 3
    0b010100,   // 4
    0b011010,   // 5
    0b011010,   // 6
    0b010110,   // 7
    0b011110,   // 8
    0b011110    // 9
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
            // display digit
            PORTB = pat7segB[digit];    // lookup port B and C patterns
            PORTC = pat7segC[digit];
        
            // delay 1 sec
            __delay_ms(1000);
        }  
    }      
}
