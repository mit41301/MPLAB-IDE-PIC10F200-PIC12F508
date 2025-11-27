/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Reaction_timer-HTC.c                           *
*   Date:          5/8/12                                               *
*   File Version:  1.2                                                  *
*                                                                       *
*   Author:        David Meiklejohn                                     *
*   Company:       Gooligum Electronics                                 *
*                                                                       *
*************************************************************************
*                                                                       *
*   Architecture:  Baseline PIC                                         *
*   Processor:     12F508/509                                           *
*   Compiler:      MPLAB XC8 v1.01 (Free mode)                          *
*                                                                       *
*************************************************************************
*                                                                       *
*   Files required: none                                                *
*                                                                       *
*************************************************************************
*                                                                       *
*   Description:    Lesson 3, example 1                                 *
*                   Reaction Timer game.                                *
*                                                                       *
*   User must attempt to press button within defined reaction time      *
*   after "start" LED lights.  Success is indicated by "success" LED.   *
*                                                                       *
*       Starts with both LEDs unlit.                                    *
*       2 sec delay before lighting "start"                             *
*       Waits up to 1 sec for button press                              *
*       (only) on button press, lights "success"                        *
*       1 sec delay before repeating from start                         *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = success LED                                               *
*       GP2 = start LED                                                 *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ  4000000     // oscillator frequency for _delay()


/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_OFF & CP_OFF & WDT_OFF & OSC_IntRC);


// Pin assignments
#define START   GPIObits.GP2        // LEDs
#define SUCCESS GPIObits.GP1

#define BUTTON  GPIObits.GP3        // pushbutton


/***** CONSTANTS *****/
#define MAXRT   200             // Maximum reaction time (in ms)


/***** MAIN PROGRAM *****/
void main()
{
    uint8_t     cnt_8ms;            // counter: increments every 8 ms

    //*** Initialisation  
    
    // configure port
    TRIS = 0b111001;                // configure GP1 and GP2 (only) as outputs
    
    // configure Timer0    
    OPTION = 0b11010100;            // configure Timer0:
             //--0-----                 timer mode (T0CS = 0)
             //----0---                 prescaler assigned to Timer0 (PSA = 0)
             //-----100                 prescale = 32 (PS = 100)
             //                         -> increment every 32 us


    //*** Main loop                                 
    for (;;)
    {
        // start with both LEDs off
        GPIO = 0;                   

        // delay 2 sec
        __delay_ms(2000);           // delay 2000 ms             

        // indicate start 
        START = 1;                  // turn on start LED

        // wait up to 1 sec for button press
        cnt_8ms = 0;
        while (BUTTON == 1 && cnt_8ms < 1000/8)
        {
            TMR0 = 0;               // clear timer0
            while (TMR0 < 8000/32)  // wait for 8 ms (32 us/tick)
                ;
            ++cnt_8ms;              // increment 8 ms counter
        }
        
        // check elapsed time
        if (cnt_8ms < MAXRT/8)      // if time < max reaction time (8 ms/count)
            SUCCESS = 1;            //   turn on success LED

        // delay 1 sec
        __delay_ms(1000);           // delay 1000 ms     
    }  
}
