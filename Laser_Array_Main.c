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
#include "ADCInit.h"
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
#define V_Step 3.225806 

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************

// Sequentially read laser head temperatures and store in array. 1st entry will
// be the local temp sensor, next 12 are heads 1 - 12.

void Display_Laser_Driver_Temp(int *Driverpt);
void Display_Laser_Head_Temp(int *Headpt);
void Display_Voltage(double adcvalue);
void Ext_Fan(int status);
void Laser_Activate(int *Driverpt, int *Headpt);
void Read_Laser_Driver_Temp(int *Driverpt);
void Read_Laser_Head_Temp(int *Headpt);
int Read_Voltage(void);
void Start_Screen(void);
void System_Sleep(void); // System Sleep function.
void Version_Screen(void);




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
    int ADCValue = 0;
    // **** Initialize PORTS ****
    PortInit();
    SCREEN = ON; // turn power to screen on.
    DelayMs(1000);

    // *** Initialize ADC ***
    ADCInit();


    // *** Initialize SPI ***
    SPIInit();

    // *** Initialization TFT_ILI9341 *** 
    Initialize_TFT_ILI9341();
    FillScreen_ILI9341(ILI9341_BLACK);

    // *** Initialize I2C ***
    I2CInit();


    Start_Screen();
    while (1) {
    }


    // *** Initialize Temperature Sensor ***
    Temp_Sensor_Wake();

    // *** Initialize FLAGs ***
    SLEEP_FLG = 0;


    Temp_Dis_Frame();
    Ext_Fan(OFF);
    LasRly = OFF;
    ONLED = ON;

    while (1) {
        // Test status of SLeep flag. 0 = goto sleep, 1 = 
        if (SLEEP_FLG == 1) {
            System_Sleep();
        }
        Laser_Activate(Driverpt, Headpt);
        //Read_Laser_Head_Temp(Headpt);
        Read_Laser_Driver_Temp(Driverpt);
        //LineWrite_XY_ILI9341_16x25("Driver  ", 1, Line7, ILI9341_WHITE, ILI9341_BLACK);
        //Display_Laser_Head_Temp(Headpt);
        // DelayMs(1500);
        //LineWrite_XY_ILI9341_16x25("Driver", 1, Line7, ILI9341_WHITE, ILI9341_BLACK);
        Display_Laser_Driver_Temp(Driverpt);

        //  while (1) {
        ADCValue = Read_Voltage();
        Display_Voltage(ADCValue);
        //   }

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

void Display_Voltage(double adcvalue) {

    char Volts1[10];
    char Volts2[10];

    // Print the decimal ADC value.
    sprintf(Volts2, "%4.0f D", adcvalue);
    LineWrite_XY_ILI9341_16x25(Volts2, 1, Line7, ILI9341_WHITE, ILI9341_BLACK);

    adcvalue = ((adcvalue * V_Step) / 187.5); // Convert the ADC value to volts.

    // Convert to string, 5 digits total, 2 decimal places, decimal counts as 1.)
    sprintf(Volts1, "%5.2f V", adcvalue);
    LineWrite_XY_ILI9341_16x25(Volts1, 180, Line7, ILI9341_WHITE, ILI9341_BLACK);
}

void Ext_Fan(int status) {
    ExtFan = status;
    DelayMs(300);
}

void Laser_Activate(int *Driverpt, int *Headpt) {
    ExtFan = ON;
    LasRly = ON;

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

int Read_Voltage(void) {
    // ADC routine samples AN0 16 times and takes the average. (SMPI = F)
    //In ADVInit _SSRC = 7 for auto conversion and _ASAM = 0. Sampling starts when
    // _ASAMP = 1. After 16 sample/conversions the ADC interrupt is set. The flag
    // is manually checked instead of a written routine.

    int ADCValue = 0;
    int count = 0;

    int *ADC16Ptr;

    _ADON = 1; // Start ADC

    //while (1) {
    ADCValue = 0;
    ADC16Ptr = &ADC1BUF0;
    IFS0bits.AD1IF = 0; // Clear interrupt flag

    _ASAM = 1; // Auto sampling start for 31Tad then auto convert

    while (!IFS0bits.AD1IF) {
    } // Waiting for conversion interrupt flag

    _ASAM = 0; // Stop  auto sample/convert

    // Add all 16 buffer locations
    for (count = 0; count <= 15; count++) {
        ADCValue = ADCValue + *ADC16Ptr++;
    }
    // Take the average.
    ADCValue = ADCValue >> 4;
    return ADCValue;
    // }

}

void Start_Screen(void) {
    float a = 0;

    //Crosshair_TFT_ILI9341(100, 100, 75, ILI9341_MAGENTA);


    for (a = -90; a <= 90; a = a + 60) {
        DrawAngleLine_TFT_ILI9341(100, 100, 150, a, ILI9341_PHOSPHORGREEN);
    }
    a = 0;
    for (a = -90; a <= 90; a = a + 30) {
        DrawAngleLine_TFT_ILI9341(100, 100, 120, a, ILI9341_PHOSPHORGREEN);
    }

    DrawHLine_TFT_ILI9341(100, 100, 200, ILI9341_PHOSPHORGREEN);

    for (a = -90; a <= 90; a = a + 30) {
        DrawAngleLine_TFT_ILI9341(100, 100, 25, a, ILI9341_BLACK);
    }
    //LineWrite_XY_ILI9341_16x25("DONE", 0, Line7, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
}

void System_Sleep(void) {
    FillScreen_ILI9341(ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("SYSTEM SHUTTING DOWN", 0, Line3, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25(".", 150, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = ON;
    LasRly = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25("..", 150, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = OFF;
    ExtFan = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25("...", 150, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = ON;
    DelayMs(2000);
    ONLED = OFF;
    Temp_Sensor_Sleep();
    Display_TFT_ILI9341_Sleep();

    Sleep();
    Nop();
}

void Version_Screen(void) {

}

