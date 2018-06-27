/* 
 * File:                Temp_TC74_Init.h
 * Author:              James Vlasblom
 * Date:                June 26, 2018.
 * 
 * Comments:            This file contains the functions for reading/ writing
 *                      and displaying temperature from the TC74 temperature
 *                      sensor.
 * 
 * Revision history:    
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  

#ifndef Temp_TC74_Init_H
#define	Temp_TC74_Init_H



#include <stdio.h>
#include <stdlib.h>

#define TEMPSENSE_READ 0x9B     // Temperature slave address with R/!W = 1
#define TEMPSENSE_WRITE 0x9A    // Temperature slave address with R/!W = 0
#define NORMAL 0x00
#define STANDBY 0x80
#define RTR 0x00    // Read Temperature (TEMP)
#define RWCR 0x01   // Read/Write Configuration Register (CONFIG) 

#define SENSOR_1 0x01
#define SENSOR_2 0x02
#define SENSOR_3 0x04
#define SENSOR_4 0x08
#define SENSOR_5 0x10
#define SENSOR_6 0x20
#define SENSOR_7 0x40
#define SENSOR_8 0x80
#define SENSOR_9 0x01
#define SENSOR_10 0x02
#define SENSOR_11 0x04
#define SENSOR_12 0x08
#define SENSOR_13 0x10


void Temp_TC74_Display(int temp, int sensor_pos); // Displays sensor temperature 
int Read_Temp_TC74(void); // Read Temperature sensor.
void Write_Config_TC74(int command); // Accepts either 'NORMAL' or 'STANDBY'.

// This function displays the temperature of given sensor, in one of 12 
// positions.

void Temp_TC74_Display(int temp, int sensor_no) {

    // Buffer for Int to ASCII conversion, ADC channel 1.
    char Temp_buf [25] = "";

    // **** Convert Hex to Dec ASCII
    // Always a 5 digit string because of how the buffer is defined.
    // Format: char *  itoa ( int value, char * str, int base );

    //sprintf(Temp_buf, "TEMP 30mW: %d C ", temp); // Convert Temp value to string.

    switch (sensor_no) {

        case SENSOR_1:
            sprintf(Temp_buf, "TEMP  30mW: %d C ", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, 1, Line1, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_2:
            sprintf(Temp_buf, "TEMP  50mW: %d C ", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, 1, Line2, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_3:
            sprintf(Temp_buf, "TEMP 100mW: %d C ", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, 1, Line3, ILI9341_WHITE, ILI9341_BLACK);
            break;

        default:
            LineWrite_XY_ILI9341_16x25("No sensor found.", 1, Line0, ILI9341_WHITE, ILI9341_BLACK);

    }




}

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
        //LineWrite_XY_ILI9341_16x25("TACK1 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("TACK1 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    // DelayMs(3000);

    I2C2TRN = RTR; // write command register

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("TACK2 = 1", 0, Line7, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("TACK2 = 0", 0, Line7, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    I2C2CONbits.RSEN = 1; // Sending a restart to change direction of bus.
    while (I2C2CONbits.RSEN) { // Wait for Start bit to clear.
    }


    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    I2C2TRN = TEMPSENSE_READ; // Write to slaved address with READ request

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("ACK3 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

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
        //LineWrite_XY_ILI9341_16x25("ACK1 = 1", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    // LineWrite_XY_ILI9341_16x25("ACK1 = 0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2TRN = RWCR; // write command register

    // while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    // } // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        // LineWrite_XY_ILI9341_16x25("ACK2 = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    // LineWrite_XY_ILI9341_16x25("ACK2 = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2TRN = command; // One of two DATA commands, NORMAL or STANDBY

    //while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    //} // before testing the ACKSTAT bit.

    //DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        // LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    // LineWrite_XY_ILI9341_16x25("ACK3 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

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

