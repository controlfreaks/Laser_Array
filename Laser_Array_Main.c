/* 
 * File:        Laser_Array
 * Author:      James_Vlasblom
 * Date:        June 14, 2018. 
 *
 * Comments:
 *  This file controls the Laser Array device consisting of 12 x 100mW 
 * class IIIb, 532nm lasers. The MCU monitors 12 external temperature sensors
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
#include <p24FJ64GA004.h>
#include <xc.h>
//#include "ADCInit.h"
#include "Fonts.h"
#include "I2CInit.h"
//#include "INTInit.h"
#include "Misc_Macro.h"
#include "MyFunctions.h"
#include "PortInit.h"
#include "SPIInit.h"
#include "String.h"
//#include "TimerInit.h"
#include "TFT_ILI9341.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************




//******************************************************************************
// MCU Configuration Section
//******************************************************************************



// PIC24FJ64GA004 Configuration Bit Settings

// 'C' source line config statements


// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
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

    // *** Initialize SPI ***
    SPIInit();

    // *** Initialization TFT_ILI9341 *** 
    Initialize_TFT_ILI9341();

    // *** Initialize SPI ***
    I2CInit();

    FillScreen_ILI9341(ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("TEMP", 0, Line1, ILI9341_WHITE, ILI9341_BLACK);
    
    I2CWrite_Config(NORMAL); // Set Temperature to normal mode.
    
    while (1) {
        

        //DelayMs(1);        

    }
    return (EXIT_SUCCESS);
} // End of main program loop.
