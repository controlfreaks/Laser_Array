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

#define Col_L 0
#define Col_M 100
#define Col_R 150

#define SENSOR_DIS_LOCAL 0x00
#define SENSOR_DIS_1 0x01
#define SENSOR_DIS_2 0x02
#define SENSOR_DIS_3 0x03
#define SENSOR_DIS_4 0x04
#define SENSOR_DIS_5 0x05
#define SENSOR_DIS_6 0x06
#define SENSOR_DIS_7 0x07
#define SENSOR_DIS_8 0x08
#define SENSOR_DIS_9 0x09
#define SENSOR_DIS_10 0x0A
#define SENSOR_DIS_11 0x0B
#define SENSOR_DIS_12 0x0C

void Temp_TC74_Display(int temp, int sensor_pos); // Displays sensor temperature 
int Read_Temp_TC74_Remote(void); // Read remote Temperature sensor.
int Read_Temp_TC74_Local(void); // Read local Temperature sensor.
void Temp_Dis_Frame(void); // Displays the frame work for display.
void Temp_Sensor_Sleep(void); // Puts all the temperature sensors to standby mode.
void Temp_Sensor_Wake(void); // Wakes the temp sensors to be reading to read again.
void Write_Config_TC74_Remote(int command); // Accepts either 'NORMAL' or 'STANDBY'.
void Write_Config_TC74_Local(int command); // Accepts either 'NORMAL' or 'STANDBY'.

void Temp_Dis_Frame(void) {
    LineWrite_XY_ILI9341_16x25("LOCAL   C", 85, Line0, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L1   C", 170, Line1, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L2   C", 1, Line1, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L3   C", 170, Line2, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L4   C", 1, Line2, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L5   C", 170, Line3, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L6   C", 1, Line3, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L7   C", 170, Line4, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L8   C", 1, Line4, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L9   C", 170, Line5, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L10   C", 1, Line5, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L11   C", 170, Line6, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("L12   C", 1, Line6, ILI9341_WHITE, ILI9341_BLACK);
}

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
        case SENSOR_DIS_LOCAL:
            sprintf(Temp_buf, "Local %02d C", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line0, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_1:
            sprintf(Temp_buf, " 1 %02d C", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line1, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_2:
            sprintf(Temp_buf, " 2 %02d C", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line1, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_3:
            sprintf(Temp_buf, " 3 %02d C", temp); // Convert Temp value to string.          
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line2, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_4:
            sprintf(Temp_buf, " 4 %02d C", temp); // Convert Temp value to string.           
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line2, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_5:
            sprintf(Temp_buf, " 5 %02d C", temp); // Convert Temp value to string.           
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line3, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_6:
            sprintf(Temp_buf, " 6 %02d C", temp); // Convert Temp value to string.            
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line3, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_7:
            sprintf(Temp_buf, " 7 %02d C", temp); // Convert Temp value to string.          
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line4, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_8:
            sprintf(Temp_buf, " 8 %02d C", temp); // Convert Temp value to string.          
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line4, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_9:
            sprintf(Temp_buf, " 9 %02d C", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line5, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_10:
            sprintf(Temp_buf, "10 %02d C", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, (Col_L), Line5, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_11:
            sprintf(Temp_buf, "11 %02d C", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, (Col_R), Line6, ILI9341_WHITE, ILI9341_BLACK);
            break;

        case SENSOR_DIS_12:
            sprintf(Temp_buf, "12 %02d C", temp); // Convert Temp value to string.
            LineWrite_XY_ILI9341_16x25(Temp_buf, (Col_L), Line6, ILI9341_WHITE, ILI9341_BLACK);
            break;

        default:
            LineWrite_XY_ILI9341_16x25("No sensor found.", 1, Line0, ILI9341_WHITE, ILI9341_BLACK);

    }
}

int Read_Temp_TC74_Local(void) {
    // I2C1RCV = 0;    // *** Clear the register for testing

    I2C1CONbits.SEN = 1; // Start bit.
    while (I2C1CONbits.SEN) { // Wait for Start bit to clear.
    }

    I2C1TRN = TEMPSENSE_WRITE; // Write to slaved address with WRITE request.

    while (I2C1STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C1STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("TACK1 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("TACK1 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    // DelayMs(3000);

    I2C1TRN = RTR; // write command register

    while (I2C1STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C1STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("TACK2 = 1", 0, Line7, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("TACK2 = 0", 0, Line7, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    I2C1CONbits.RSEN = 1; // Sending a restart to change direction of bus.
    while (I2C1CONbits.RSEN) { // Wait for Start bit to clear.
    }


    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    I2C1TRN = TEMPSENSE_READ; // Write to slaved address with READ request

    while (I2C1STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C1STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    //LineWrite_XY_ILI9341_16x25("ACK3 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    I2C1CONbits.RCEN = 1; // Receiving DATA from of bus.
    while (I2C1CONbits.RCEN) { // Wait for Start bit to clear.
    }

    I2C1CONbits.PEN = 1; // Generate stop bit.
    while (I2C1CONbits.PEN) { // Wait for Start bit to clear.
    }
    return I2C1RCV; // return the contents of the RCV Register.
}

int Read_Temp_TC74_Remote(void) {
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

void Temp_Sensor_Sleep(void) {

    // Close remote sensor (no switch needed)
    Write_Config_TC74_Local(STANDBY);


    // Writing to I2C switch 0, Laser Heads 1 - 4
    TCA9548A_I2CSwitch_Open(LH_1, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_2, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_3, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_4, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(STANDBY);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_0);


    // Writing to I2C switch 1, Laser Heads 5 - 8
    TCA9548A_I2CSwitch_Open(LH_5, TCA9548A_I2CSwitch_1);
    Write_Config_TC74_Remote(STANDBY);

    //TCA9548A_I2CSwitch_Open(LH_6, TCA9548A_I2CSwitch_1);
   // Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_7, TCA9548A_I2CSwitch_1);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_8, TCA9548A_I2CSwitch_1);
    Write_Config_TC74_Remote(STANDBY);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_1);


    // Writing to I2C switch 2, Laser Heads 9 - 12
    TCA9548A_I2CSwitch_Open(LH_9, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_10, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_11, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LH_12, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(STANDBY);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_2);


    // Writing to I2C switch 3, Laser Heads 1 - 4
    TCA9548A_I2CSwitch_Open(LD_1, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_2, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_3, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_4, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(STANDBY);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_3);


    // Writing to I2C switch 4, Laser Heads 5 - 8
    TCA9548A_I2CSwitch_Open(LD_5, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_6, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_7, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_8, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(STANDBY);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_4);


    // Writing to I2C switch 5, Laser Heads 9 - 12
    TCA9548A_I2CSwitch_Open(LD_9, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_10, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_11, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(STANDBY);

    TCA9548A_I2CSwitch_Open(LD_12, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(STANDBY);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_5);

    // TCA9548A_I2CSwitch_Reset = 0;

}

void Temp_Sensor_Wake(void) {

    // Close remote sensor (no switch needed)
    Write_Config_TC74_Local(NORMAL);


    // Writing to I2C switch 0, Laser Heads 1 - 4
    TCA9548A_I2CSwitch_Open(LH_1, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_2, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_3, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_4, TCA9548A_I2CSwitch_0);
    Write_Config_TC74_Remote(NORMAL);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_0);


    // Writing to I2C switch 1, Laser Heads 5 - 8
    TCA9548A_I2CSwitch_Open(LH_5, TCA9548A_I2CSwitch_1);
    Write_Config_TC74_Remote(NORMAL);

    //This one not working right now.
    //TCA9548A_I2CSwitch_Open(LH_6, TCA9548A_I2CSwitch_1);
    //Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_7, TCA9548A_I2CSwitch_1);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_8, TCA9548A_I2CSwitch_1);
    Write_Config_TC74_Remote(NORMAL);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_1);


    // Writing to I2C switch 2, Laser Heads 9 - 12
    TCA9548A_I2CSwitch_Open(LH_9, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_10, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_11, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LH_12, TCA9548A_I2CSwitch_2);
    Write_Config_TC74_Remote(NORMAL);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_2);


    // Writing to I2C switch 3, Laser Drivers 1 - 4
    TCA9548A_I2CSwitch_Open(LD_1, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_2, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_3, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_4, TCA9548A_I2CSwitch_3);
    Write_Config_TC74_Remote(NORMAL);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_3);


    // Writing to I2C switch 4, Laser Drivers 5 - 8
    TCA9548A_I2CSwitch_Open(LD_5, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_6, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_7, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_8, TCA9548A_I2CSwitch_4);
    Write_Config_TC74_Remote(NORMAL);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_4);


    // Writing to I2C switch 5, Laser Drivers 9 - 12
    TCA9548A_I2CSwitch_Open(LD_9, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_10, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_11, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(NORMAL);

    TCA9548A_I2CSwitch_Open(LD_12, TCA9548A_I2CSwitch_5);
    Write_Config_TC74_Remote(NORMAL);

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_5);

    // TCA9548A_I2CSwitch_Reset = 0;

}

void Write_Config_TC74_Local(int command) {
    I2C1CONbits.SEN = 1; // Start bit.
    while (I2C1CONbits.SEN) { // Wait for Start bit to clear.
    }

    I2C1TRN = TEMPSENSE_WRITE; // Write to slaved address with WRITE request.

    while (I2C1STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C1STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        //LineWrite_XY_ILI9341_16x25("ACK1 = 1", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }

    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    // LineWrite_XY_ILI9341_16x25("ACK1 = 0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C1TRN = RWCR; // write command register

    while (I2C1STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C1STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        // LineWrite_XY_ILI9341_16x25("ACK2 = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }
    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    // LineWrite_XY_ILI9341_16x25("ACK2 = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C1TRN = command; // One of two DATA commands, NORMAL or STANDBY

    while (I2C1STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C1STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        // LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }
    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    // LineWrite_XY_ILI9341_16x25("ACK3 = 0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C1CONbits.PEN = 1; // Generate stop bit.
    while (I2C1CONbits.PEN) { // Wait for Start bit to clear.
    }
}

void Write_Config_TC74_Remote(int command) {
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

    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    // LineWrite_XY_ILI9341_16x25("ACK1 = 0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2TRN = RWCR; // write command register

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        // LineWrite_XY_ILI9341_16x25("ACK2 = 1", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }
    DelayUs(5); // Necessary delay for ACKSTAT to settle.
    // LineWrite_XY_ILI9341_16x25("ACK2 = 0", 0, Line5, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***

    I2C2TRN = command; // One of two DATA commands, NORMAL or STANDBY

    while (I2C2STATbits.TBF) { // Waiting for transmit to finish
    } // before testing the ACKSTAT bit.

    DelayUs(5); // Necessary delay for ACKSTAT to settle.

    while (I2C2STATbits.ACKSTAT) { // Waiting for !ACK from slave.
        // LineWrite_XY_ILI9341_16x25("ACK3 = 1", 0, Line6, ILI9341_WHITE, ILI9341_BLACK); // ***test line ***
    }
    DelayUs(5); // Necessary delay for ACKSTAT to settle.
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

