

/* 
 * File: Interrupt_IRS.h
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTERRUPT_ISR_H
#define	INTERRUPT_ISR_H

//#include <xc.h> // include processor files - each processor file is guarded.  
#include "INTInit.h"
#include "Misc_Macro.h"
#include "MyFunctions.h"
#include "PortInit.h"
#include "TCA9548A_I2CSwitch_Init.h"
#include "Temp_TC74_Init.h"
#include "TFT_ILI9341.h"

// *** Local define statements here ***

// **********************
// *** ISR Prototypes ***
// **********************

//void __attribute__((interrupt)) _ADC1Interrupt(void);
//void __attribute__((interrupt)) _CNInterrupt(void);
void __attribute__((interrupt)) _INT0Interrupt(void);
//void __attribute__((interrupt)) _INT1Interrupt(void);
//void __attribute__((interrupt)) _INT2Interrupt(void);
//void __attribute__((interrupt)) _T1Interrupt(void);
//void __attribute__((interrupt)) _T4Interrupt(void);


//*** Extern statements go here ***


// ***********************
// *** ISR Definitions ***
// ***********************


// *** External INT0 Interrupt - RESET INTERRUPT ***

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {
    //IEC0bits.INT0IE = 0;
    //ONLED = ~ONLED;
    if (_SLEEP == 1) { // Coming out of sleep mode, device was in sleep mode.
        _SLEEP = 0;
        System_Wake();  // Wake up from shutdown.
    } else {
        SLEEP_FLG = 1;
    }
    IFS0bits.INT0IF = 0; // reset INT0 interrupt flag
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

