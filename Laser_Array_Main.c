/* 
 * File:        Laser_Array
 * Author:      James_Vlasblom
 * Date:        June 14, 2018. 
 *
 * Comments:
 *  This file controls the Laser Array device consisting of 12 x 100mW 
 * class IIIb, 532nm lasers. The MCU monitors 24 external temperature sensors
 * and 1 local temperature sensor via I2C. A TCA9548A 8-channel I2C switch is
 * incorporated to deal with all the temperature sensors having the same factory
 * preset address. When the laser head temperature exceeds a determined set 
 * point, the laser output LED will flash alerting the user to switch to 
 * another laser.
 * 
 *  There are 12 general I/O pins controlling the laser output LEDS, 2 general
 * I/O pins controlling an external cooling fan and a laser output relay. 
 * 
 * There is one analog I/O measuring the battery voltage and keeping track of 
 * the charging.
 * 
 * Revision History:
 * 
 */


// *****************************************************************************
// *****************************************************************************
// Section: Included Files and DEFINE statements
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stdlib.h>
//#include "p24FJ64GA004.inc"
#include <p24FJ64GA004.h>
#include <xc.h>
//#include <xc.inc>
//#include "ADCInit.h"
#include "Fonts.h"
#include "I2CInit.h"
#include "INTInit.h"
#include "Interrupt_ISR.h"
#include "Misc_Macro.h"
#include "MyFunctions.h"
#include "PortInit.h"
#include "SPIInit.h"
#include "String.h"
#include "TCA9548A_I2CSwitch_Init.h"
#include "Temp_TC74_Init.h"
//#include "TimerInit.h"
#include "TFT_ILI9341.h"


#define ON 1
#define OFF 0

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************

// Sequentially read laser head temperatures and store in array. 1st entry will
// be the local temp sensor, next 12 are heads 1 - 12.

void Display_Laser_Driver_Temp(int *Driverpt);
void Display_Laser_Head_Temp(int *Headpt);
void Display_Sleep(void); // Puts the LCD display to sleep.
void Display_Wake(void); // Wakes display up.
void Ext_Fan(int status);
void Read_Laser_Driver_Temp(int *Driverpt);
void Read_Laser_Head_Temp(int *Headpt);
void Temp_Sensor_Sleep(void); // Puts all the temperature sensors to standby mode.


int Temp_Array_Head [13] = {0}; // Temperature storing laser head temps, ist value is local.
int Temp_Array_Driver [13] = {0}; //Temperature storing laser driver temps, ist value is local.
int *Headpt = Temp_Array_Head; // Pointer to 1st element of array.
int *Driverpt = Temp_Array_Driver;


//******************************************************************************
// MCU Configuration Section
//******************************************************************************

// PIC24FJ64GA004 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config SOSCSEL = SOSC           // Sec Oscillator Select (Default Secondary Oscillator (SOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(int argc, char** argv) {
    int temp = 0;

    // **** Initialize PORTS ****
    PortInit();
    SCREEN = ON; // turn power to screen on.
    DelayMs(1000);

    // *** Initialize SPI ***
    SPIInit();

    // *** Initialization TFT_ILI9341 *** 
    Initialize_TFT_ILI9341();
    FillScreen_ILI9341(ILI9341_MAGENTA);

    // *** Initialize I2C ***
    I2CInit();

    // *** Initialize I2C Switch ***
    // No initiation necessary.
    TCA9548A_I2CSwitch_Reset = 1; // Take TCA9548A out of reset.
    //TCA9548A_I2CSwitch_A0 = 0;           
    //TCA9548A_I2CSwitch_Open(SENSOR_1, TCA9548A_I2CSwitch_0);

    //Write_Config_TC74_Local(NORMAL); // Set Temperature to normal mode.
    //TCA9548A_I2CSwitch_Open(SENSOR_1, TCA9548A_I2CSwitch_0);
    // Write_Config_TC74_Remote(NORMAL); // Set Temperature to normal mode.

    //TCA9548A_I2CSwitch_Open(SENSOR_CLEAR, TCA9548A_I2CSwitch_1);

    //ExtFan = ON;
    //LasRly = ON;
    //LineWrite_XY_ILI9341_16x25("FAN ON ", 1, Line6, ILI9341_GREEN, ILI9341_BLACK);
    // LineWrite_XY_ILI9341_16x25("LASER ON ", 1, Line5, ILI9341_GREEN, ILI9341_BLACK);
    // DelayMs(5000);
    // ExtFan = OFF;
    // LasRly = OFF;
    // LineWrite_XY_ILI9341_16x25("FAN OFF ", 1, Line6, ILI9341_RED, ILI9341_BLACK);
    // LineWrite_XY_ILI9341_16x25("LASER OFF ", 1, Line5, ILI9341_RED, ILI9341_BLACK);


    // DelayMs(10);


    Temp_Dis_Frame();
    Ext_Fan(OFF);
    LasRly = OFF;


    Temp_Sensor_Sleep();

    // Write_Config_TC74_Local(STANDBY);
    // _TRISB8 = 0;
    // _TRISB9 = 0;
    //_ODB8 = 1; // 0 = normal, 1 = open drain 
    //_ODB9 = 1; // 0 = normal, 1 = open drain 
    //_LATB9 = 0; // Force pin high
    // _LATB8 = 0;


    while (1) {

        //  temp = Read_Temp_TC74_Local();
        //  Temp_TC74_Display(temp, SENSOR_DIS_LOCAL);

        //DelayMs(500);
        //Read_Laser_Head_Temp(Headpt);
        Read_Laser_Driver_Temp(Driverpt);
        //LineWrite_XY_ILI9341_16x25("Head  ", 1, Line7, ILI9341_WHITE, ILI9341_BLACK);
        //Display_Laser_Head_Temp(Headpt);
       // DelayMs(1500);
        LineWrite_XY_ILI9341_16x25("Driver", 1, Line7, ILI9341_WHITE, ILI9341_BLACK);
        Display_Laser_Driver_Temp(Driverpt);
        DelayMs(1500);
        Display_Sleep();
        //DelayMs(100);
        Sleep();
        
        // FillScreen_ILI9341(ILI9341_MAGENTA);
         DelayMs(500);
       // while (1) {

       // }





        //Nop(), Nop();   // Time for display to settle.



    }
    return (EXIT_SUCCESS);
} // End of main program loop.

void Display_Laser_Driver_Temp(int *Driverpt) {
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_LOCAL);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_1);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_2);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_3);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_4);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_5);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_6);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_7);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_8);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_9);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_10);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_11);
    Temp_TC74_Display(*Driverpt++, SENSOR_DIS_12);

    Driverpt = Temp_Array_Driver; // Reset pointer when finished.
}

void Display_Laser_Head_Temp(int *Headpt) {
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_LOCAL);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_1);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_2);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_3);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_4);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_5);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_6);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_7);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_8);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_9);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_10);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_11);
    Temp_TC74_Display(*Headpt++, SENSOR_DIS_12);

    Headpt = Temp_Array_Head; // Reset pointer when finished.
}

void Display_Sleep(void) {
   // WriteCommand_ILI9341(ILI9341_DISPOFF);
  //  DelayMs(10);
  //  WriteCommand_ILI9341(ILI9341_SLPIN);
  //  DelayMs(10);
   // WriteCommand_ILI9341(ILI9341_SLPOUT); //VCM control 
   // WriteCommand_ILI9341(ILI9341_DISPON);

    // Change SPI pins, either high or OD 
    _ODC0 = 1;
    _ODC1 = 1;
    _ODA10 = 1;
    _ODB12 = 1;
    _LATA8 = 1;

    SCREEN = OFF;
    DelayMs(3000);
}

void Display_Wake(void) {
    // WriteCommand_ILI9341(ILI9341_DISPOFF);
    //DelayMs(10);
    //WriteCommand_ILI9341(ILI9341_SLPIN);
    // DelayMs(10);
    //WriteCommand_ILI9341(ILI9341_SLPOUT); //VCM control 
    // WriteCommand_ILI9341(ILI9341_DISPON);

    // Change SPI pins, either high or OD 
   // _ODC0 = 0;
   // _ODC1 = 0;
   // _ODA10 = 0;
   // _ODB12 = 0;
    //_LATA8 = 0;

    SCREEN = ON;
}

void Ext_Fan(int status) {
    ExtFan = status;
    DelayMs(300);
}

void Read_Laser_Driver_Temp(int *Driverpt) {
    //Advance the Laser_Driver_Array pointer immediately after assigning it a 
    // value.

    // Assign local temp sensor to first array location.
    *Driverpt++ = Read_Temp_TC74_Local();

    // Reading from I2C switch 3, Laser Drivers 1 - 4
    TCA9548A_I2CSwitch_Open(LD_1, TCA9548A_I2CSwitch_3);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_2, TCA9548A_I2CSwitch_3);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_3, TCA9548A_I2CSwitch_3);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_4, TCA9548A_I2CSwitch_3);
    *Driverpt++ = Read_Temp_TC74_Remote();

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_3);



    // Reading from I2C switch 4, Laser Drivers 5 - 8
    TCA9548A_I2CSwitch_Open(LD_5, TCA9548A_I2CSwitch_4);
    *Driverpt++ = Read_Temp_TC74_Remote(); // *****This temp sensor not working.

    TCA9548A_I2CSwitch_Open(LD_6, TCA9548A_I2CSwitch_4);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_7, TCA9548A_I2CSwitch_4);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_8, TCA9548A_I2CSwitch_4);
    *Driverpt++ = Read_Temp_TC74_Remote();

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_4);


    // Reading from I2C switch 5, Laser Drivers 9 - 12
    TCA9548A_I2CSwitch_Open(LD_9, TCA9548A_I2CSwitch_5);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_10, TCA9548A_I2CSwitch_5);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_11, TCA9548A_I2CSwitch_5);
    *Driverpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LD_12, TCA9548A_I2CSwitch_5);
    *Driverpt++ = Read_Temp_TC74_Remote();

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_5);


    Driverpt = Temp_Array_Driver; // Reset pointer when finished.

}

void Read_Laser_Head_Temp(int *Headpt) {
    //Advance the Laser_Head_Array pointer immediately after assigning it a 
    // value.

    // Assign local temp sensor to first array location.
    *Headpt++ = Read_Temp_TC74_Local();

    // Reading from I2C switch 0, Laser Heads 1 - 4
    TCA9548A_I2CSwitch_Open(LH_1, TCA9548A_I2CSwitch_0);
    *Headpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LH_2, TCA9548A_I2CSwitch_0);
    *Headpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LH_3, TCA9548A_I2CSwitch_0);
    *Headpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LH_4, TCA9548A_I2CSwitch_0);
    *Headpt++ = Read_Temp_TC74_Remote();

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_0);


    // Reading from I2C switch 1, Laser Heads 5 - 8
    TCA9548A_I2CSwitch_Open(LH_5, TCA9548A_I2CSwitch_1);
    *Headpt++ = Read_Temp_TC74_Remote(); // *****This temp sensor not working.

    TCA9548A_I2CSwitch_Open(LH_6, TCA9548A_I2CSwitch_1);
    //*Headpt++ = Read_Temp_TC74_Remote();
    *Headpt++;

    TCA9548A_I2CSwitch_Open(LH_7, TCA9548A_I2CSwitch_1);
    *Headpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LH_8, TCA9548A_I2CSwitch_1);
    *Headpt++ = Read_Temp_TC74_Remote();

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_1);


    // Reading from I2C switch 2, Laser Heads 9 - 12
    TCA9548A_I2CSwitch_Open(LH_9, TCA9548A_I2CSwitch_2);
    *Headpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LH_10, TCA9548A_I2CSwitch_2);
    *Headpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LH_11, TCA9548A_I2CSwitch_2);
    *Headpt++ = Read_Temp_TC74_Remote();

    TCA9548A_I2CSwitch_Open(LH_12, TCA9548A_I2CSwitch_2);
    *Headpt++ = Read_Temp_TC74_Remote();

    // Close current switch before opening another switch.
    TCA9548A_I2CSwitch_Open(NULL_SENSOR, TCA9548A_I2CSwitch_2);

    Headpt = Temp_Array_Head; // Reset pointer when finished.
}

void Temp_Sensor_Sleep(void) {

    //I2C1CONbits.I2CEN = OFF; // Turn off I2c module #1.
    // I2C2CONbits.I2CEN = OFF; // Turn off I2c module #2.
    // _TRISB8 = 0;
    //_TRISB9 = 0;
    // _ODB8 = 1; // 0 = normal, 1 = open drain 
    // _ODB9 = 1; // 0 = normal, 1 = open drain 
    // _LATB9 = 0; // Force pin high
    // _LATB8 = 0;
    // _LATB2 = 0;
    //  _LATB3 = 0;
    // TCA9548A_I2CSwitch_Reset = 0;


    // Close remote sensor (no switch needed)

    Write_Config_TC74_Local(STANDBY);
    /*
  
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

      TCA9548A_I2CSwitch_Open(LH_6, TCA9548A_I2CSwitch_1);
      Write_Config_TC74_Remote(STANDBY);

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
     */

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