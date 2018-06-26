/* 
 * File:                Temp_TC74_Init.h
 * Author:              James Vlasblom
 * Date:                June 26, 2018.
 * 
 * Comments:            This file contains the function Temp_TC74_Init(void)
 *                      which initializes the temperature sensor for 
 *                      communication with the MCU.
 * 
 * Revision history:    
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  

#ifndef Temp_TC74_Init_H
#define	Temp_TC74_Init_H

#include "TFT_ILI9341.h"




int Read_Temp_TC74(void); // Read Temperature sensor.
void Write_Config_TC74(int command); // 

int Read_Temp_TC74(void) {
    // I2C2RCV = 0;    // *** Clear the register for testing

    I2C2CONbits.SEN = 1; // Start bit.
    while (I2C2CONbits.SEN) { // Wait for Start bit to clear.
    }

    I2C2TRN = TEMPSENSE_WRITE; // Write to slaved address with WRITE request.

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK1 = 1", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK1 = 0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayMs(3000);

    I2C2TRN = RTR; // write command register

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK2 = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK2 = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2CONbits.RSEN = 1; // Sending a restart to change direction of bus.
    while (I2C2CONbits.RSEN) { // Wait for Start bit to clear.
    }

    DelayMs(3000);

    I2C2TRN = TEMPSENSE_READ; // Write to slaved address with READ request

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK3 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayMs(3000);

    I2C2CONbits.RCEN = 1; // Receiving DATA from of bus.
    while (I2C2CONbits.RCEN) { // Wait for Start bit to clear.
    }

    I2C2CONbits.PEN = 1; // Generate stop bit.
    while (I2C2CONbits.PEN) { // Wait for Start bit to clear.
    }
    return I2C2RCV; // return the contents of the RCV Register.
}

void Write_Config_TC74(int command) {
    I2C2CONbits.SEN = 1; // Start bit.
    while (I2C2CONbits.SEN) { // Wait for Start bit to clear.
    }

    I2C2TRN = TEMPSENSE_WRITE; // Write to slaved address with WRITE request.

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK1 = 1", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK1 = 0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2TRN = RWCR; // write command register

    // while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    // } // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK2 = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK2 = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2TRN = command; // One of two DATA commands, NORMAL or STANDBY

    //while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    //} // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK3 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

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

