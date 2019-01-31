
/* 
 * File:                    MyFunctions.h
 * Author:                  James Vlasblom
 * Date:                    June 14, 2018
 * 
 * Comment:     This file contains misc functions that may have broad 
 *              appeal
 
 *              Function List:  DelayUs()
 *                              DelayMs()
 * 
 * Revision history:    
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MYFUNCTIONS_50_Shot_H
#define	MYFUNCTIONS_50_Shot_H

#define OSC_CLOCK 16000000              // 8 MHz oscillator
// divide OSC_CLOCK / 4 for FRCDIV, divide OSC_CLOCK / 1 for FRPLL clock setting
#define INSTRUCT_CLOCK (OSC_CLOCK / 1) // 4 OSC per instruction cycle

#define CLOCKMS (0.001 * INSTRUCT_CLOCK)     // number represents clk / 2000 (ie 4MHz / 2000)
//#define CLOCKMS 2000
#define CLOCKUS 2
#define KLUNK_CH 11
#define _POS 0
#define _NEG 1
#define _OUTPUT 0
#define _INPUT 1


#include <xc.h> // include processor files - each processor file is guarded. 

// *** Declarations start here ***
void DelayMs(long time);
void DelayUs(long time);


// *** Function definitions start here, all declarations above this point

void DelayMs(long time) {
    long Temp = 0; // temp 32 bit variable]
    Temp = time * CLOCKMS;
    PR2 = Temp; // copy LSW into PR2
    PR3 = Temp >> 16; // Shift 16 bits and copy MSW into PR3
    T2CON = 0x0000; // stops Timer1
    T3CON = 0x00; // stops any 16-bit Timer3 operation
    TMR3 = 0x00; // clears contents of Timer3 Register 
    TMR2 = 0x00; // clears contents of Timer2 register
    T2CONbits.TCKPS = 0; // set clock prescaler to 1:1
    IFS0bits.T3IF = 0; // clears timer3 interrupt status flag
    T2CONbits.T32 = 1; // enable 32bit timer operation
    T2CONbits.TON = 1; // start 32 bit timer

    while (IFS0bits.T3IF == 0) {
    }
}


// This Us routine is only accurate above 100Us due to system clock

void DelayUs(long time) {
    PR2 = time * CLOCKUS; // copy LSW into PR2
    T2CON = 0x0000; // stops Timer1
    TMR2 = 0x00; // clears contents of Timer2 register
    //T2CONbits.TCKPS = 0;    // set clock prescaler to 1:1
    IFS0bits.T2IF = 0; // clears timer3 interrupt status flag
    T2CONbits.TON = 1; // start 32 bit timer

    while (IFS0bits.T2IF == 0) {
    }
}


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

