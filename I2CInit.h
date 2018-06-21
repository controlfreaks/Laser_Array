
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
#define RWCR 0x0A   // Read/Write Configuration Register (CONFIG) 

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
    int temp = 0; // temperature variable returned.


    return temp;
}

void I2CWrite_Config(int command) {
    I2C2CONbits.SEN = 1; // Start bit.
    
    while (I2C2CONbits.SEN) {
    } // wait for Start bit to clear.
    
    I2C2TRN = TEMPSENSE_WRITE; // Write to slaved address.
    
    while (I2C2STATbits.TBF) { // Waiting for transmit to finish.
     //   Nop(), Nop(), Nop();
      //  LineWrite_XY_ILI9341_16x25("TFB = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
    }
    //DelayMs(10);
   // Nop(), Nop(), Nop();
    //I2C2TRN = RWCR; // write command register

    //while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
    // Nop(), Nop(), Nop();
   // LineWrite_XY_ILI9341_16x25("TFB = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
    // }
    //Nop(), Nop(), Nop();
    //LineWrite_XY_ILI9341_16x25("C", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
    //I2C2TRN = command; // One of two commands, NORMAL or STANDBY

    //while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
    //}
    //DelayUs(10);
    //LineWrite_XY_ILI9341_16x25("Start cleared", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
    //I2C2CONbits.PEN = 1; // Generate stop bit.
    //  while (I2C2STATbits.P) {
    // } // wait for Start bit to clear.
    // LineWrite_XY_ILI9341_16x25("Stop cleared  ", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
    // I2C2CONbits.I2CEN = OFF; // Turn on I2C module #1.
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

