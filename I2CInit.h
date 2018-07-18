
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
#ifndef I2CInit_H
#define	I2CInit_H

#define ON 1
#define OFF 0

#include <xc.h> // include processor files - each processor file is guarded. 


void I2CInit(void); // I2C initialization routine.

void I2CInit(void) {

    // ***********************************************
    // *** Configuration of the I2C modules ***********
    // ***********************************************

    I2C1CONbits.I2CEN = ON; // Turn on I2C module #1.
    I2C1CONbits.IPMIEN = 0; // Must be 0 for master mode.
    I2C1CONbits.A10M = 0; // 7-bit address mode.
    I2C1BRG = 0x4D; // Set baud rate for 100kHZ (Fcy of 8MHz).

    I2C2CONbits.I2CEN = ON; // Turn on I2C module #2.
    I2C2CONbits.IPMIEN = 0; // Must be 0 for master mode.
    I2C2CONbits.A10M = 0; // 7-bit address mode.
    I2C2BRG = 0x4D; // Set baud rate for 100kHZ (Fcy of 8MHz).

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

