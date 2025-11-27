/************************************************************************
*                                                                       *
*   Filename:      BC_L7-ADC_hex-out-HTC.c                              *
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
*   Description:    Lesson 7, example 2                                 *
*                                                                       *
*   Displays ADC output in hexadeximal on 7-segment LED displays        *
*                                                                       *
*   Continuously samples analog input,                                  *
*   displaying result as 2 x hex digits on multiplexed 7-seg displays   *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       AN2             = voltage to be measured (e.g. pot or LDR)      *
*       RB0-1,RB4,RC1-4 = 7-segment display bus (common cathode)        *
*       RC5             = "tens" digit enable (active high)             *
*       RB5             = ones digit enable                             *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>


/***** CONFIGURATION *****/
// ext reset, no code protect, no watchdog, 4 MHz int clock
__CONFIG(MCLRE_ON & CP_OFF & WDT_OFF & IOSCFS_OFF & OSC_IntRC_RB4EN);

// Pin assignments
#define TENS_EN     PORTCbits.RC5   // "tens" (high nybble) digit enable
#define ONES_EN     PORTBbits.RB5   // ones digit enable


/***** PROTOTYPES *****/
void set7seg(uint8_t digit);    // display digit on 7-segment display


/***** MACROS *****/
#define TMR0_2  (TMR0 & 1<<2)   // access to TMR0<2>


/***** MAIN PROGRAM *****/
void main()
{
    //*** Initialisation
    
    // configure ports
    TRISB = 0;                      // configure PORTB and PORTC as all outputs
    TRISC = 0;
    CM1CON0 = 0;                    // disable comparator 1 -> RB0, RB1 digital
    CM2CON0 = 0;                    // disable comparator 2 -> RC0, RC1 digital
    VRCON = 0;                      // disable CVref -> RC2 usable
    
    // configure ADC    
    ADCON0bits.ADCS = 0b11;         // clock = INTOSC/4  
    ADCON0bits.ANS  = 0b01;         // AN2 (only) analog
    ADCON0bits.CHS  = 0b10;         // select channel AN2 
    ADCON0bits.ADON = 1;            // turn ADC on
                                    // -> AN2 ready for sampling
                                    
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
        // sample input
        ADCON0bits.GO = 1;          // start conversion
        while (ADCON0bits.nDONE)    // wait until done
            ;

        // display high nybble for 2.048 ms
        while (!TMR0_2)         // wait for TMR0<2> to go high
            ;
        set7seg(ADRES >> 4);    // output high nybble of result
        TENS_EN = 1;            // enable "tens" digit 
        while (TMR0_2)          // wait for TMR0<2> to go low
            ;
                        
        // display low nybble for 2.048 ms
        while (!TMR0_2)         // wait for TMR0<2> to go high
            ;
        set7seg(ADRES & 0x0F);  // output low nybble of result 
        ONES_EN = 1;            // enable ones digit
        while (TMR0_2)          // wait for TMR0<2> to go low
            ;
    }      
}


/***** FUNCTIONS *****/

/***** Display digit on 7-segment display *****/
void set7seg(uint8_t digit)
{
    // pattern table for 7 segment display on port B
    const uint8_t pat7segB[16] = {
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
        0b000011,   // 9 
        0b010011,   // A
        0b010011,   // b
        0b010010,   // C
        0b010001,   // d
        0b010011,   // E
        0b010011    // F           
    }; 

    // pattern table for 7 segment display on port C
    const uint8_t pat7segC[16] = {
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
        0b011110,   // 9
        0b010110,   // A
        0b011000,   // b
        0b001010,   // C
        0b011100,   // d
        0b001010,   // E
        0b000010    // F        
    };
    
    // disable displays
    PORTB = 0;                  // clear all digit enable lines on PORTB
    PORTC = 0;                  //  and PORTC
    
    // output digit pattern
    PORTB = pat7segB[digit];    // lookup and output port B and C patterns
    PORTC = pat7segC[digit];
}