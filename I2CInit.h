
/* 
 * File:                I2CInit.h
 * Author:              James Vlasblom
 * Date:                June 18, 2018.
 * 
 * Comments:            This file contains the function I2CInit(void)
 *                      which initializes the I2C module for communicating 
 *                      with the temperature sensors.
 * 
 * Revision history:    
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#define ON 1
#define OFF 0
#define TEMPSENSE_READ 0x9B     // Temperature slave address with R/!W = 1
#define TEMPSENSE_WRITE 0x9A    // Temperature slave address with R/!W = 0
#define NORMAL 0x00
#define STANDBY 0x80
#define RTR 0x00    // Read Temperature (TEMP)
#define RWCR 0x01   // Read/Write Configuration Register (CONFIG) 

#include <xc.h> // include processor files - each processor file is guarded. 

#include "TFT_ILI9341.h"



void I2CInit(void); // I2C initialization routine.
int I2CRead_Temp(void); // Read Temperature sensor.
void I2CWrite_Config(int command); // 

void I2CInit(void) {

    // ***********************************************
    // *** Configuration of the I2C module ***********
    // ***********************************************

    I2C2CONbits.I2CEN = ON; // Turn on I2C module #1.
    I2C2CONbits.IPMIEN = 0; // Must be 0 for master mode.
    I2C2CONbits.A10M = 0; // 7-bit address mode.
    I2C2BRG = 0x4D; // Set baud rate for 100kHZ (Fcy of 8MHz).

}

int I2CRead_Temp(void) {
    I2C2RCV = 0;    // *** Clear the register for testing

    I2C2CONbits.SEN = 1; // Start bit.
    while (I2C2CONbits.SEN) { // Wait for Start bit to clear.
    }

    I2C2TRN = TEMPSENSE_WRITE; // Write to slaved address with WRITE request.

    // while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    // } // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK1 = 1", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK1 = 0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2TRN = RTR; // write command register

    // while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    // } // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK2 = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK2 = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2CONbits.RSEN = 1; // Sending a restart to change direction of bus.
    while (I2C2CONbits.RSEN) { // Wait for Start bit to clear.
    }
    
    I2C2TRN = TEMPSENSE_READ; // Write to slaved address with READ request

    // while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    // } // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    LineWrite_XY_ILI9341_16x25("ACK3 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2CONbits.RCEN = 1; // Receiving DATA from of bus.
    while (I2C2CONbits.RCEN) { // Wait for Start bit to clear.
    }

    I2C2CONbits.PEN = 1; // Generate stop bit.
    while (I2C2CONbits.PEN) { // Wait for Start bit to clear.
    }
    return I2C2RCV; // return the contents of the RCV Register.
}

void I2CWrite_Config(int command) {
    I2C2CONbits.SEN = 1; // Start bit.
    while (I2C2CONbits.SEN) { // Wait for Start bit to clear.
    }

    I2C2TRN = TEMPSENSE_WRITE; // Write to slaved address with WRITE request.

    // while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    // } // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

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

