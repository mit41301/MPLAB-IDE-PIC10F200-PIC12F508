/************************************************************************
*                                                                       *
*   Filename:      BC_L3-Flash+PB_LED_Sym-HTC                           *
*   Date:          5/8/12                                               *
*   File Version:  1.4                                                  *
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
*   Description:    Lesson 3, example 2a                                *
*                                                                       *
*   Demonstrates use of Timer0 to maintain timing of background actions *
*   while performing other actions in response to changing inputs       *
*                                                                       *
*   One LED simply flashes at 1 Hz (50% duty cycle).                    *
*   The other LED is only lit when the pushbutton is pressed            *
*                                                                       *
*   Uses symbols to represent port bit positions                        *
*                                                                       *
*************************************************************************
*                                                                       *
*   Pin assignments:                                                    *
*       GP1 = "button pressed" indicator LED                            *
*       GP2 = flashing LED                                              *
*       GP3 = pushbutton switch (active low)                            *
*                                                                       *
************************************************************************/

#include <xc.h>
#include <stdint.h>


/***** CONFIGURATION *****/
// int reset, no code protect, no watchdog, int RC clock 
__CONFIG(MCLRE_OFF & CP_OFF & WDT_OFF & OSC_IntRC);

// Pin assignments
#define nFLASH  2               // flashing LED on GP2
#define nPRESS  1               // "button pressed" indicator LED on GP1

#define BUTTON  GPIObits.GP3    // pushbutton


/***** GLOBAL VARIABLES *****/
uint8_t     sGPIO;                  // shadow copy of GPIO


/***** MAIN PROGRAM *****/
void main()
{
    uint8_t   dc;                   // delay counter

    //*** Initialisation  
    
    // configure port
    GPIO = 0;                       // start with all LEDs off
    sGPIO = 0;                      //   update shadow
    TRIS = ~(1<<nFLASH|1<<nPRESS);  // configure LEDs (only) as outputs
    
    // configure Timer0    
    OPTION = 0b11010100;            // configure Timer0:
             //--0-----                 timer mode (T0CS = 0)
             //----0---                 prescaler assigned to Timer0 (PSA = 0)
             //-----100                 prescale = 32 (PS = 100)
             //                         -> increment every 32 us

    
    //*** Main loop  
    for (;;) 
    {
        // delay 500 ms while responding to button press
        for (dc = 0; dc < 125; dc++)     // repeat 125 times (125 x 4 ms = 500 ms)
        {  
            TMR0 = 0;                    //   clear timer0
            while (TMR0 < 125)           //   repeat for 4 ms (125 x 32 us)
            {                            //     check and respond to button press
                sGPIO &= ~(1<<nPRESS);   //       assume button up -> indicator LED off
                if (BUTTON == 0)         //       if button pressed (low)
                    sGPIO |= 1<<nPRESS;  //         turn on indicator LED
                GPIO = sGPIO;            //     update port (copy shadow to GPIO)
            }
        }
        // toggle flashing LED       
        sGPIO ^= 1<<nFLASH;              // toggle flashing LED, using shadow register
    } 
}
