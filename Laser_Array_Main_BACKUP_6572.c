/* 
 * File:        Laser_Array
 * Author:      James_Vlasblom
 * Date:        May 30, 2019. 
<<<<<<< HEAD
 * 
 * Added heater hood software, moved ExtFan = ON to beginning.
=======
>>>>>>> 43b7beb
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
 * Turned external fan on at start.
 * Added heater head software.
 * Modifications for Laser Array serial 001.
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

void ChargeMsg(double *ADCValuept); // Displays appropriate charge message based on voltage.
void Display_Laser_Driver_Temp(int *Driverpt); // Displays laser driver temperatures.
void Display_Laser_Head_Temp(int *Headpt); // Displays laser head temperatures.
void Display_Mode(void); // determines which mode to display, Temp or Symbol.
void Display_Voltage(double *ADCValuept); // Displays voltage
//void Draw_Laser_symbols(int *Temp_Colourpt);
void Ext_Fan(int status);
void Laser_Activate(int *Driverpt, int *Headpt);
void Laser_Colour_Level(int *Driverpt, int *Headpt, int *Colourpt, int *BackColourpt);
void Read_Laser_Driver_Temp(int *Driverpt); // Reads all laser driver sensors.
void Read_Laser_Head_Temp(int *Headpt); // Reads all laser head sensors.
void Read_Voltage(double *ADCValuept); // Reads voltage from ADC, converts it.
void Show_Temps(void); // Displays raw temperature data.
void Show_Symbols(void); // Shows each laser as a colour temperature.
void Start_Screen(void);
void System_Sleep(void); // System Sleep function.
void Version_Screen(void);



double ADCValue = 0;
int Temp_Array_Head [13] = {0}; // Temperature storing laser head temps, 1st value is local.
int Temp_Array_Driver [13] = {0}; //Temperature storing laser driver temps, 1st value is local.
int Colour_Array[13] = {0}; // Array holding laser symbol colours.
int Back_Colour_Array[13] = {0}; //Background symbol colour.
int Temp_Colour_Array [13] = {0}; //Temporary colour array.
int *Colourpt = Colour_Array;
int *BackColourpt = Back_Colour_Array;
int *Temp_Colourpt = Temp_Colour_Array;
int *Headpt = Temp_Array_Head; // Pointer to 1st element of array.
int *Driverpt = Temp_Array_Driver;
double *ADCValuept = &ADCValue;



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
    // **** Initialize PORTS ****
    PortInit();

    //Temp_Dis_Frame();
    Ext_Fan(ON);
    LasRly = OFF;
    ONLED = ON;

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


    // *** Initialize Temperature Sensor ***
    Temp_Sensor_Wake();


    // *** Initialize FLAGs ***
    SLEEP_FLG = 0;
    LASER_OK_FLG = 0;
    LASER_PROMPT_FLG = 0;
    DISP_MODE_FLG = 0;


    Read_Voltage(&ADCValue);
    Display_Voltage(&ADCValue);
    ChargeMsg(ADCValuept);


    Display_Mode(); // Determine which mode to enter into.

    Laser_Activate(Driverpt, Headpt);

    while (1) {
        // Test status of SLeep flag. 0 = goto sleep, 1 = 
        if (SLEEP_FLG == 1) {
            System_Sleep();
        }

        Read_Voltage(&ADCValue); // Read voltage from ADC.
        Display_Voltage(&ADCValue); // Display the voltage derived from ADC.
        ChargeMsg(ADCValuept); // Display appropriate message based on voltage level.



        if (DISP_MODE_FLG) { // Show data according to entered mode.
            Show_Symbols();
        } else {
            Show_Temps();
        }

    }

    return (EXIT_SUCCESS);
}// End of main program loop. 

void ChargeMsg(double *ADCValuept) {
    if (*ADCValuept > 14.1) { // Charging condition.
        LineWrite_XY_ILI9341_16x25("<Charging>", 1, Line7, ILI9341_CYAN, ILI9341_BLACK);
    } else if ((*ADCValuept >= 11.61) && (*ADCValuept <= 14.09)) { // Normal Condition.
        LineWrite_XY_ILI9341_16x25("          ", 1, Line7, ILI9341_BLACK, ILI9341_BLACK);
    } else if ((*ADCValuept >= 11.0) && (*ADCValuept <= 11.6)) { // Low Batt.
        LineWrite_XY_ILI9341_16x25("<Low Batt>", 1, Line7, ILI9341_YELLOW, ILI9341_BLACK);
    } else if ((*ADCValuept >= 10.0) && (*ADCValuept <= 10.99)) { // Charge Batt.

        LineWrite_XY_ILI9341_16x25("<Chg Batt>", 1, Line7, ILI9341_RED, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("<Chg Batt>", 1, Line7, ILI9341_BLACK, ILI9341_RED);
        LineWrite_XY_ILI9341_16x25("<Chg Batt>", 1, Line7, ILI9341_RED, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("<Chg Batt>", 1, Line7, ILI9341_BLACK, ILI9341_RED);
        LineWrite_XY_ILI9341_16x25("<Chg Batt>", 1, Line7, ILI9341_RED, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("<Chg Batt>", 1, Line7, ILI9341_BLACK, ILI9341_RED);

    } else if (*ADCValuept < 9.99) { // Shutting now.

        FillScreen_ILI9341(ILI9341_BLACK);
        Read_Voltage(&ADCValue);
        Display_Voltage(&ADCValue);
        LineWrite_XY_ILI9341_16x25("SHUTTING DOWN DUE TO ", 1, Line2, ILI9341_GREEN, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("LOW BATTERY. PLEASE", 1, Line3, ILI9341_GREEN, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("PLUG INTO AC POWER", 1, Line4, ILI9341_GREEN, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("TO CONTINUE.", 1, Line5, ILI9341_GREEN, ILI9341_BLACK);
        DelayMs(4000);
        System_Sleep();

    }

}

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

void Display_Mode(void) {
    if (_RB7 == 0) {
        LineWrite_XY_ILI9341_16x25("Temperature Display", 35, Line2, ILI9341_WHITE, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("Mode", 130, Line3, ILI9341_WHITE, ILI9341_BLACK);
        DISP_MODE_FLG = 0; // Show Temperature.
        DelayMs(2000);
        LineWrite_XY_ILI9341_16x25("Temperature Display", 35, Line2, ILI9341_BLACK, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("Mode", 130, Line3, ILI9341_BLACK, ILI9341_BLACK);
        Version_Screen();

    } else if (_RB7 == 1) {
        DISP_MODE_FLG = 1; // Show symbols.
    }
    SLEEP_FLG = 0;
}

void Display_Voltage(double *ADCValuept) {

    char Volts1[10];
    char Volts2[10];
    ADCValue = *ADCValuept;

    // Print the decimal ADC value.
    // sprintf(Volts2, "%4.0f D", ADCValue);
    // LineWrite_XY_ILI9341_16x25(Volts2, 1, Line7, ILI9341_WHITE, ILI9341_BLACK);

    ADCValue = ((ADCValue * V_Step) / 187.5); // Convert the ADC value to volts.

    // Convert to string, 5 digits total, 2 decimal places, decimal counts as 1.)
    sprintf(Volts1, "%5.2f V", ADCValue);
    LineWrite_XY_ILI9341_16x25(Volts1, 195, Line7, ILI9341_WHITE, ILI9341_BLACK);
}

//void Draw_Laser_symbols(int *Temp_Colourpt) {
//
//}

void Ext_Fan(int status) {
    ExtFan = status;
    DelayMs(300);
}

void Laser_Activate(int *Driverpt, int *Headpt) {
    /* This function turns on the fan and the laser relay after prompting the 
     * user to signal it is ok by pressing the ON/OFF button. This tries to 
     * ensure that the lasers are not turned on unexpectedly. */

    LASER_PROMPT_FLG = 1; // Prompt user
    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_RED, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("ENSURE LASERS ARE", 20, Line1, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("IN SAFE POSITION", 20, Line2, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_RED, ILI9341_BLACK);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_BLACK, ILI9341_RED);
    DelayMs(50);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_BLACK, ILI9341_RED);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_RED, ILI9341_BLACK);
    DelayMs(10);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_RED, ILI9341_BLACK);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_BLACK, ILI9341_RED);
    DelayMs(10);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_BLACK, ILI9341_RED);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_RED, ILI9341_BLACK);
    DelayMs(10);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_RED, ILI9341_BLACK);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_BLACK, ILI9341_RED);
    DelayMs(10);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_BLACK, ILI9341_RED);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_RED, ILI9341_BLACK);
    DelayMs(10);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_RED, ILI9341_BLACK);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_BLACK, ILI9341_RED);
    DelayMs(10);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_BLACK, ILI9341_RED);

    LineWrite_XY_ILI9341_16x25("CAUTION", 90, Line0, ILI9341_RED, ILI9341_BLACK);
    DelayMs(10);
    LineWrite_XY_ILI9341_16x25("ATTENTION", 75, Line3, ILI9341_RED, ILI9341_BLACK);

    LineWrite_XY_ILI9341_16x25("PRESS ON BUTTON TO", 15, Line5, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25(" ACTIVATE LASERS.", 20, Line6, ILI9341_WHITE, ILI9341_BLACK);

    while (!LASER_OK_FLG) {
        ONLED = OFF;
        DelayMs(100);
        ONLED = ON;
        DelayMs(100);
        Read_Voltage(&ADCValue);
        Display_Voltage(&ADCValue);
        ChargeMsg(ADCValuept);
    }
    LASER_PROMPT_FLG = 0;
    LASER_OK_FLG = 0;
    ONLED = ON;
    ExtFan = ON;
    LasRly = ON;
    DelayMs(500); // Needed to let relays settle

    FillScreen_ILI9341(ILI9341_BLACK);

}

/*Note, adjust algorithm for if <= 20 turn heaters on, if >= 25 turn heaters off.*/
void Laser_Colour_Level(int *Driverpt, int *Headpt, int *Colourpt, int *BackColourpt) {
    int element = 0;

    while (element <= 12) {
        /*  This section sets a flag, (HEATER_FLG) if temp drops below 20C for any head
         *  and resets the flag if any head goes above 32C. After testing all heads, the 
         * heater is turned on or off depending on the status of the flag.
         */

        if (element != 0) { // Filter out local temperature sensor.
            if (*Headpt <= 20) {
                HEATER_FLG = ON;
                //HeatRly = ON;
                //LineWrite_XY_ILI9341_16x25("HEATER", 0, Line7, ILI9341_RED, ILI9341_BLACK);
            } else if (*Headpt >= 32) {
                HEATER_FLG = OFF;
                //HeatRly = OFF;
                //LineWrite_XY_ILI9341_16x25("HEATER", 0, Line7, ILI9341_BLACK, ILI9341_BLACK);
            }
        }

        if ((*Driverpt >= 45) || (*Headpt >= 40)) {
            *Colourpt = ILI9341_BLACK; // Make colour RED.
            *BackColourpt = ILI9341_RED;
        }// else if (((*Driverpt >= 41) && (*Driverpt <= 47)) || ((*Headpt >= 38) && (*Headpt <= 40))) {
            //    *Colourpt = ILI9341_RED; // Make colour RED.
            //    *BackColourpt = ILI9341_BLACK;
            //}
        else if (((*Driverpt >= 43) && (*Driverpt <= 44)) || ((*Headpt >= 35) && (*Headpt <= 42))) {
            *Colourpt = ILI9341_ORANGE; // Make colour ORANGE.
            *BackColourpt = ILI9341_BLACK;
        } else if (((*Driverpt >= 38) && (*Driverpt <= 42)) || ((*Headpt >= 31) && (*Headpt <= 34))) {
            *Colourpt = ILI9341_YELLOW; // Make colour YELLOW.
            *BackColourpt = ILI9341_BLACK;
        } else if (((*Driverpt >= 30) && (*Driverpt <= 37)) || ((*Headpt >= 26) && (*Headpt <= 30))) {
            *Colourpt = ILI9341_GREEN; // Make colour GREEN.
            *BackColourpt = ILI9341_BLACK;
        } else if ((*Headpt >= 0) && (*Headpt <= 25)) {
            *Colourpt = ILI9341_WHITE; // Make colour WHITE.
            *BackColourpt = ILI9341_BLACK;
        }
        // *Temp_Colourpt = *Colourpt; // Copy array to temp.
        element++;
        Driverpt++;
        Headpt++;
        Colourpt++;
        BackColourpt++;

        if (HEATER_FLG) {
            HeatRly = ON;
            LineWrite_XY_ILI9341_16x25("HEATER", 0, Line7, ILI9341_RED, ILI9341_BLACK);
        } else {
            HeatRly = OFF;
            LineWrite_XY_ILI9341_16x25("HEATER", 0, Line7, ILI9341_BLACK, ILI9341_BLACK);
        }
    }

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
    // ***Note in Laser Array box serial number LA00191018-001, Head temperature
    // senor #2 and #4 are switched because of wiring constraints therefore 
    // their values need to be switched at the end of this function.

    int temp = 0;
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

    // Switch values 2 and 4 as discussed at top of function.
    temp = Temp_Array_Head[2]; //Store element 2 temporarily;
    Temp_Array_Head[2] = Temp_Array_Head[4]; // Element 2 is 4;
    Temp_Array_Head[4] = temp; // Element 4 is 3;


}

void Read_Voltage(double *ADCValuept) {
    // ADC routine samples AN0 16 times and takes the average. (SMPI = F)
    //In ADVInit _SSRC = 7 for auto conversion and _ASAM = 0. Sampling starts when
    // _ASAMP = 1. After 16 sample/conversions the ADC interrupt is set. The flag
    // is manually checked instead of a written routine.

    //int Value = *ADCValuept;
    int count = 0;

    int *ADC16Ptr;

    _ADON = 1; // Start ADC

    //while (1) {
    *ADCValuept = 0;
    ADC16Ptr = &ADC1BUF0;
    IFS0bits.AD1IF = 0; // Clear interrupt flag

    _ASAM = 1; // Auto sampling start for 31Tad then auto convert

    while (!IFS0bits.AD1IF) {
    } // Waiting for conversion interrupt flag

    _ASAM = 0; // Stop  auto sample/convert

    // Add all 16 buffer locations
    for (count = 0; count <= 15; count++) {
        *ADCValuept = *ADCValuept + *ADC16Ptr++;
    }
    // Take the average.
    *ADCValuept = *ADCValuept / 16;
    //return *ADCValue;
    // }

}

void Show_Symbols(void) {
    int element = 0;

    Read_Laser_Head_Temp(Headpt); // Read laser head temps into array. *** REMOVE ***
    Read_Laser_Driver_Temp(Driverpt); // Read laser driver temps into array.  *** REMOVE ***
    Laser_Colour_Level(Driverpt, Headpt, Colourpt, BackColourpt); // Colour levels into array.

    if (Colour_Array[2] != Temp_Colour_Array[2]) {// Only print symbol it changed.       
        DrawLaserL(LASER_X_L, Line0, Colour_Array[2]);
        LineWrite_XY_ILI9341_16x25("Laser 2", (LASER_X_L + 20), (Line0 + Y_OFFSET), Colour_Array[2], Back_Colour_Array[2]);
        Temp_Colour_Array[2] = Colour_Array[2];
    }
    if (Colour_Array[1] != Temp_Colour_Array[1]) {
        DrawLaserR(LASER_X_R, Line0, Colour_Array[1]);
        LineWrite_XY_ILI9341_16x25("Laser 1", (LASER_X_R - 120), (Line0 + Y_OFFSET), Colour_Array[1], Back_Colour_Array[1]);
        Temp_Colour_Array[1] = Colour_Array[1];
    }
    if (Colour_Array[4] != Temp_Colour_Array[4]) {
        DrawLaserL(LASER_X_L, Line1, Colour_Array[4]);
        LineWrite_XY_ILI9341_16x25("Laser 4", (LASER_X_L + 20), (Line1 + Y_OFFSET), Colour_Array[4], Back_Colour_Array[4]);
        Temp_Colour_Array[4] = Colour_Array[4];
    }
    if (Colour_Array[3] != Temp_Colour_Array[3]) {
        DrawLaserR(LASER_X_R, Line1, Colour_Array[3]);
        LineWrite_XY_ILI9341_16x25("Laser 3", (LASER_X_R - 120), (Line1 + Y_OFFSET), Colour_Array[3], Back_Colour_Array[3]);
        Temp_Colour_Array[3] = Colour_Array[3];
    }

    if (Colour_Array[6] != Temp_Colour_Array[6]) {
        DrawLaserL(LASER_X_L, Line2, Colour_Array[6]);
        LineWrite_XY_ILI9341_16x25("Laser 6", (LASER_X_L + 20), (Line2 + Y_OFFSET), Colour_Array[6], Back_Colour_Array[6]);
        Temp_Colour_Array[6] = Colour_Array[6];
    }

    if (Colour_Array[5] != Temp_Colour_Array[5]) {
        DrawLaserR(LASER_X_R, Line2, Colour_Array[5]);
        LineWrite_XY_ILI9341_16x25("Laser 5", (LASER_X_R - 120), (Line2 + Y_OFFSET), Colour_Array[5], Back_Colour_Array[5]);
        Temp_Colour_Array[5] = Colour_Array[5];
    }


    if (Colour_Array[8] != Temp_Colour_Array[8]) {
        DrawLaserL(LASER_X_L, Line3, Colour_Array[8]);
        LineWrite_XY_ILI9341_16x25("Laser 8", (LASER_X_L + 20), (Line3 + Y_OFFSET), Colour_Array[8], Back_Colour_Array[8]);
        Temp_Colour_Array[8] = Colour_Array[8];
    }

    if (Colour_Array[7] != Temp_Colour_Array[7]) {
        DrawLaserR(LASER_X_R, Line3, Colour_Array[7]);
        LineWrite_XY_ILI9341_16x25("Laser 7", (LASER_X_R - 120), (Line3 + Y_OFFSET), Colour_Array[7], Back_Colour_Array[7]);
        Temp_Colour_Array[7] = Colour_Array[7];
    }

    if (Colour_Array[10] != Temp_Colour_Array[10]) {
        DrawLaserL(LASER_X_L, Line4, Colour_Array[10]);
        LineWrite_XY_ILI9341_16x25("Laser 10", (LASER_X_L + 20), (Line4 + Y_OFFSET), Colour_Array[10], Back_Colour_Array[10]);
        Temp_Colour_Array[10] = Colour_Array[10];
    }

    if (Colour_Array[9] != Temp_Colour_Array[9]) {
        DrawLaserR(LASER_X_R, Line4, Colour_Array[9]);
        LineWrite_XY_ILI9341_16x25("Laser 9", (LASER_X_R - 120), (Line4 + Y_OFFSET), Colour_Array[9], Back_Colour_Array[9]);
        Temp_Colour_Array[9] = Colour_Array[9];
    }

    if (Colour_Array[12] != Temp_Colour_Array[12]) {
        DrawLaserL(LASER_X_L, Line5, Colour_Array[12]);
        LineWrite_XY_ILI9341_16x25("Laser 12", (LASER_X_L + 20), (Line5 + Y_OFFSET), Colour_Array[12], Back_Colour_Array[12]);
        Temp_Colour_Array[12] = Colour_Array[12];
    }

    if (Colour_Array[11] != Temp_Colour_Array[11]) {
        DrawLaserR(LASER_X_R, Line5, Colour_Array[11]);
        LineWrite_XY_ILI9341_16x25("Laser 11", (LASER_X_R - 130), (Line5 + Y_OFFSET), Colour_Array[11], Back_Colour_Array[11]);
        Temp_Colour_Array[11] = Colour_Array[11];
    }
}

void Show_Temps(void) {

    char Temp_buf [12] = "";
    int element = 0;
    int show_driver = 0;

    Read_Laser_Head_Temp(Headpt);
    Read_Laser_Driver_Temp(Driverpt);
    Laser_Colour_Level(Driverpt, Headpt, Colourpt, BackColourpt); // Colour levels into array.


    sprintf(Temp_buf, "Local %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_M, Line0, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "1 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line1, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "2 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line1, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "3 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line2, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "4 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line2, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "5 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line3, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "6 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line3, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "7 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line4, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "8 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line4, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "9 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line5, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "10 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line5, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "11 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line6, Colour_Array[element], Back_Colour_Array[element]);
    element++;

    sprintf(Temp_buf, "12 %02d C", Temp_Array_Head[element]); // Convert Temp value to string.
    LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line6, Colour_Array[element], Back_Colour_Array[element]);

    LineWrite_XY_ILI9341_16x25("Head  ", 1, Line0, ILI9341_WHITE, ILI9341_BLACK);

    show_driver = ON; // Show driver temps or not, for testing

    if (show_driver) {
        DelayMs(1000);
        element = 0;

        sprintf(Temp_buf, "Local %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_M, Line0, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "1 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line1, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "2 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line1, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "3 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line2, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "4 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line2, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "5 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line3, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "6 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line3, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "7 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line4, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "8 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line4, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "9 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line5, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "10 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line5, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "11 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_R, Line6, Colour_Array[element], Back_Colour_Array[element]);
        element++;

        sprintf(Temp_buf, "12 %02d C", Temp_Array_Driver[element]); // Convert Temp value to string.
        LineWrite_XY_ILI9341_16x25(Temp_buf, Col_L, Line6, Colour_Array[element], Back_Colour_Array[element]);

        LineWrite_XY_ILI9341_16x25("Driver", 1, Line0, ILI9341_WHITE, ILI9341_BLACK);
        DelayMs(1000);
    }
}

void Start_Screen(void) {
    int b = 0;
    float a = 0;

    //Crosshair_TFT_ILI9341(100, 100, 100, ILI9341_MAGENTA);

    for (a = -90; a <= 90; a = a + 60) {
        DrawAngleLineWide_TFT_ILI9341(100, 100, 90, 3, a, ILI9341_PHOSPHORGREEN);
    }
    for (a = -90; a <= 90; a = a + 30) {
        DrawAngleLineWide_TFT_ILI9341(100, 100, 70, 3, a, ILI9341_GREEN);
    }
    DrawAngleLineWide_TFT_ILI9341(180, 100, 260, 3, 0, ILI9341_PHOSPHORGREEN);

    for (b = 100; b >= 1; b = b - 15) {
        for (a = 75; a >= -90; a = a - 30) {
            DrawAngleLineWide_TFT_ILI9341(100, 100, 100, 1, a, ILI9341_WHITE);
            DelayMs(b);
            DrawAngleLineWide_TFT_ILI9341(100, 100, 100, 1, a, ILI9341_BLACK);
        }
    }
    LineWrite_XY_ILI9341_16x25("LASER ARRAY", 145, (Line2 + 10), ILI9341_YELLOW, ILI9341_BLACK);
    DelayMs(2000);
    FillScreen_ILI9341(ILI9341_BLACK);
}

void System_Sleep(void) {
    FillScreen_ILI9341(ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("SYSTEM SHUTTING DOWN", 0, Line3, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25(".", 130, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = ON;
    LasRly = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25("..", 130, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25("...", 130, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = ON;
    LasRly = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25("....", 130, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = OFF;
    ExtFan = OFF;
    HeatRly = OFF;
    DelayMs(300);
    LineWrite_XY_ILI9341_16x25("......", 130, Line4, ILI9341_PHOSPHORGREEN, ILI9341_BLACK);
    ONLED = ON;
    DelayMs(2000);
    ONLED = OFF;
    Temp_Sensor_Sleep();
    Display_TFT_ILI9341_Sleep();

    Sleep();
    Nop();
}

void Version_Screen(void) {
    LineWrite_XY_ILI9341_16x25("Control Freaks", 48, Line0, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Version", 104, Line1, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Serial", 110, Line2, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("LA00191018 002", 0, Line3, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Software", 96, Line4, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Laser Array", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("TAG B001 13", 0, Line6, ILI9341_WHITE, ILI9341_BLACK);


    DelayMs(5000);
    FillScreen_ILI9341(ILI9341_BLACK);
}


