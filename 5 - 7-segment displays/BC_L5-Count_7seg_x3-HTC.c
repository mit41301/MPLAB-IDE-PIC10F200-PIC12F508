/************************************************************************
*                                                                       *
*   Filename:      BC_L5-Count_7seg_x3-HTC.c                            *
*   Date:          8/8/12                                               *
*   File Version:  1.5                                                  *
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
*   Description:    Lesson 5, example 2                                 *
*                                                                       *
*   Demonstrates use of multiplexing to drive multiple 7-seg displays   *
*                                                                       *
*   3-digit 7-segment LED display: 1 digit minutes, 2 digit seconds     *
*   counts in seconds 0:00 to 9:59 then repeats,                        *
*   with timing derived from int 4 MHz oscillator                       *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       RB0-1,RB4,RC1-4 = 7-segment display bus (common cathode)        *
*       RC5             = minutes enable (active high)                  *
*       RB5             = tens enable                                   *
*       RC0             = ones enable                                   *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);

// Pin assignments
#define MINUTES PORTCbits.RC5   // minutes enable
#define TENS    PORTBbits.RB5   // tens enable
#define ONES    PORTCbits.RC0   // ones enable


/***** PROTOTYPES *****/
void set7seg(uint8_t digit);    // display digit on 7-segment display


/***** MACROS *****/
#define TMR0_2  (TMR0 & 1<<2)   // access to TMR0<2>


/***** MAIN PROGRAM *****/
void main()
{
    uint8_t     mpx_cnt;            // multiplex counter
    uint8_t     mins, secs;         // time counters
    
    //*** Initialisation
    
    // configure ports
    TRISB = 0;                      // configure PORTB and PORTC as all outputs
    TRISC = 0;
    ADCON0 = 0;                     // disable AN0, AN1, AN2 inputs
    CM1CON0bits.C1ON = 0;           //     and comparator 1 -> RB0,RB1 digital
    CM2CON0bits.C2ON = 0;           // disable comparator 2 -> RC1 digital
    
    // configure timer
    OPTION = 0b11010111;            // configure Timer0:
             //--0-----                 timer mode (T0CS = 0) -> RC5 usable
             //----0---                 prescaler assigned to Timer0 (PSA = 0)
             //-----111                 prescale = 256 (PS = 111)
             //                         -> increment every 256 us
             //                            (TMR0<2> cycles every 2.048 ms) 

        
    //*** Main loop
    for (;;)
    {
        // count in seconds from 0:00 to 9:59
        for (mins = 0; mins < 10; mins++)
        {
            for (secs = 0; secs < 60; secs++)
            {
                // for each time count, multiplex display for 1 second
                // (display each of 3 digits for 2.048 ms each,
                //  so repeat 1000000/2048/3 times to make 1 second) 
                for (mpx_cnt = 0; mpx_cnt < 1000000/2048/3; mpx_cnt++)
                {
                    // display minutes for 2.048 ms
                    while (!TMR0_2)         // wait for TMR0<2> to go high
                        ;
                    set7seg(mins);          // output minutes digit
                    MINUTES = 1;            // enable minutes display
                    while (TMR0_2)          // wait for TMR0<2> to go low
                        ;
                        
                    // display tens for 2.048 ms
                    while (!TMR0_2)         // wait for TMR0<2> to go high
                        ;
                    set7seg(secs/10);       // output tens digit
                    TENS = 1;               // enable tens display
                    while (TMR0_2)          // wait for TMR0<2> to go low
                        ;
                        
                    // display ones for 2.048 ms
                    while (!TMR0_2)         // wait for TMR0<2> to go high
                        ;
                    set7seg(secs%10);       // output ones digit
                    ONES = 1;               // enable ones display
                    while (TMR0_2)          // wait for TMR0<2> to go low
                        ;
                }
            }
        }  
    }      
}


/***** FUNCTIONS *****/

/***** Display digit on 7-segment display *****/
void set7seg(uint8_t digit)
{
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
    
    // Disable displays
    PORTB = 0;                  // clear all digit enable lines on PORTB
    PORTC = 0;                  //  and PORTC
    
    // Output digit pattern
    PORTB = pat7segB[digit];    // lookup and output port B and C patterns
    PORTC = pat7segC[digit];
}