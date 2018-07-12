/* 
 * File:                TCA9548A_I2CSwitch_Init.h
 * Author:              James Vlasblom
 * Date:                June 26, 2018.
 * 
 * Comments:            This file contains the function for initializing
 *                      and reading/writing through the TCA9548A I2C switch.
 * 
 * Revision history:    
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  

#ifndef TCA9548A_I2CSwitch_H
#define	TCA9548A_I2CSwitch_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "TFT_ILI9341.h"

#define TCA9548A_I2CSwitch_Reset _LATA8 // Set the reset pin  for the TCA9548A.
#define TCA9548A_I2CSwitch_A0 _LATB4    // A0 address pin for the TCA9548A.
#define TCA9548A_Write 0xE0
#define TCA9548A_I2CSwitch_0 0xE0
#define TCA9548A_I2CSwitch_1 0xE2

// Opens communication with the switch and sets the channels to open. The 
// 'control' is an 8-bit work with each of the 8 bits representing 1 of 8 
// I2C channels.
void TCA9548A_I2CSwitch_Open(int con_Reg, int address);

void TCA9548A_I2CSwitch_Open(int con_Reg, int address) {

    if (address == TCA9548A_I2CSwitch_0) {
        TCA9548A_I2CSwitch_A0 = 0;
    } else if (address == TCA9548A_I2CSwitch_1) {
        TCA9548A_I2CSwitch_A0 = 1;
    }

    //TCA9548A_I2CSwitch_Reset = 0; // Reset TCA9548A.
    // DelayUs(1);
      TCA9548A_I2CSwitch_Reset = 1; // Take TCA9548A out of reset.
    // DelayUs(1);

    I2C2CONbits.SEN = 1; // Start bit.
    while (I2C2CONbits.SEN) { // Wait for Start bit to clear.
    }

    I2C2TRN = address; // Write to switch address with WRITE request.

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("SACK1 = 1", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("SACK1 = 0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    //DelayMs(3000);

    I2C2TRN = con_Reg; // Write control register.

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("SACK2 = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("SACK2 = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    I2C2CONbits.PEN = 1; // Generate stop bit.
    while (I2C2CONbits.PEN) { // Wait for Start bit to clear.
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

