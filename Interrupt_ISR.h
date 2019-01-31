

/* 
 * File: Interrupt_IRS.h
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTERRUPT_ISR_H
#define	INTERRUPT_ISR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "INTInit.h"
#include "PortInit.h"






// *** Local define statements here ***




// **********************
// *** ISR Prototypes ***
// **********************

//void __attribute__((interrupt)) _ADC1Interrupt(void);
//void __attribute__((interrupt)) _CNInterrupt(void);
void __attribute__((interrupt)) _INT0Interrupt(void);
//void __attribute__((interrupt)) _INT1Interrupt(void);
//void __attribute__((interrupt)) _INT2Interrupt(void);
//void __attribute__((interrupt)) _T1Interrupt(void);
//void __attribute__((interrupt)) _T4Interrupt(void);


//void Encoder_Switch(void);
//void Reset(void);
//void Rotory_Encoder(void);
//void Version(void);

//*** Extern statements go here ***



//int DoUp = 0; // Used for skipping Line Writes when going back up the menu chain.
//int DoDown = 1; // Used for skipping LineWrites, going down menu chain
//int DoBack = 0; // Used for skipping LineWrites, going back from sub menu.
//int DoForward = 0; // Used for skipping LineWrites, going into sub menu.
//int column = 222;




// ***********************
// *** ISR Definitions ***
// ***********************

// *** A/D Conversion Done Interrupt ***

//void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void) {

// ADC calculations are done inside the Disp_Batt_Voltage() function
// in the Main() of the source file.

// IFS0bits.AD1IF = 0; // reset A/D Conversion Done interrupt flag
//}

// *** Input Change Notification Interrupt ***
// Captures FIRE, ARM and S/M interrupts.

//void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
//}


// *** External INT0 Interrupt - RESET INTERRUPT ***

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {

    ONLED = ~ONLED;
    SCREEN = ON;
    _ODC0 = 0;
    _ODC1 = 0;
   // _ODA10 = 0;
    _ODB12 = 0;
    // _LATA8 = 0;
    //   SPIInit();
    //DelayMs(500);
    Initialize_TFT_ILI9341();
    DelayMs(500);
    //FillScreen_ILI9341(ILI9341_MAGENTA);

    IFS0bits.INT0IF = 0; // reset INT0 interrupt flag
}
// *** External INT1 Interrupt - ON-SWITCH INTERRUPT ***

//void __attribute__((interrupt, auto_psv)) _INT1Interrupt(void) {
//}


// *** External INT2 Interrupt ***

//void __attribute__((interrupt, auto_psv)) _INT2Interrupt(void) {
//}




// *** Timer1 TI1 Interrupt ***
// This timer controls the timing of the MULTI LED and the MULTI function.
/*
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {

void __attribute__((interrupt, auto_psv)) _T4Interrupt(void) {
}
 */
// ***************************************************************************
// *************                   Non IRS FUNCTIONS             *************
// ***************************************************************************


/*
void Encoder_Switch(void) {
    // This disables the encoder button while the encoder rotation is running.
    // Change from M10 to M11.
    if (MenuNo == 10) {
        MenuNo = 11;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoDown = 0;
        DoForward = 1;
        DoUp = 0;
        Menu();
    }// Change from M11 back to M10.
        // This puts klunker into SINGLE mode.
    else if (MenuNo == 11) {
        MenuNo = 10;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
        Single();
        Menu();
    }// Change from M12 back to M10.
        // This puts klunker into MULTI mode.
    else if (MenuNo == 12) {
        MenuNo = 10;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoUp = 0;
        DoBack = 1;
        Multi();
        Menu();
    }// Change from M50 to M51

    else if (MenuNo == 50) {
        MenuNo = 51;
        DoDown = 0;
        DoForward = 1;
        DoUp = 0;
        Menu();
    }// Change from M51 back to M50
    else if (MenuNo == 51) {
        MenuNo = 50;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
        MEMORY_MODE_FLG = OFF; // Set MEM flag.
        E_Write(EMemory_Mode, OFF); // Save Memory Condition.
        Menu();
    }// Change from M52 back to M50
    else if (MenuNo == 52) {
        MenuNo = 50;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
        MEMORY_MODE_FLG = ON; // Set MEM flag.
        E_Write(EMemory_Mode, ON); // Save Memory Condition.
        Current_Condition();
        Memory_Recovery();
        //Reset();
        //SM_FLG = 1;
        //Single();
        Memory_Recovery();
        Menu();
    }// Change from M21 back to M20.
        // This action also turns GALV function off.
    else if (MenuNo == 21) {
        MenuNo = 20;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
        GALV_FLG = 0; // Set GALV flag.
        GALV_OUT = 0; // Turn GALV function and LED off.
        Menu();
    }// Change from M22 back to M20.
        // This action also turns GALV function on.
    else if (MenuNo == 22) {

        // Test for ARMA_FLG here.
        if (ARM_FLG == 1) {
            ARM_GLV_FLG = 1;
            MenuNo = 70; // Menu 70 is the ARM/GALV conflict message.
            //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
            Menu();
        }
        Reset(); // reset before entering GALV mode.
        MenuNo = 22; // ** For full function uncomment Menu() and Change
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        // MenuNo = 21 instead of MenuNo = 22.
        LineWrite_XY_ILI9341_16x25("ON ", 0, Line3, ILI9341_BLACK, ILI9341_BLUE);
        Box_ILI9341(0, Line3, 55, 30, ILI9341_WHITE); // BOX <ON>.
        GALV_FLG = 1; // Set GALV flag.
        GALV_OUT = 1; // Turn GALV function and LED on.
    }// Change from M20 to M21.
    else if (MenuNo == 20) {
        MenuNo = 21;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoDown = 0;
        DoUp = 0;
        DoForward = 1;
        Menu();
    }// Change from M30 to Running Display
    else if (MenuNo == 30) {
        MenuNo = 40;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        VER_FLG = 0;
        MENU_FLG = 0; // leaving MENU mode (OK to fire).
        DoForward = 1; // Do not skip ClearOLED().
        IEC0bits.AD1IE = 1; // enable A/D Conversion Done interrupt
        Menu();
    }// Change back to main menu from RUNNING menu.
    else if (MenuNo == 40) {
        MENU_FLG = 1; // entering MENU mode (Not OK to fire).
        MenuNo = 10; // Enter menu in M10 screen.
        DoBack = 1;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        // Do not skip ClearOLED().
        //IEC0bits.AD1IE = 0; // disable A/D Conversion Done interrupt
        Menu();
    }
}
 */




//void Reset(void) {



/*
void Rotory_Encoder(void) {
    int Encoder_Dir = 0;
    long time = 15; // Delay time for encode ton settle.

    Encoder_Dir = _RF0; //'encoder pin B' determines CW or CCW, 0 = CCW, 1 = CW.
    DelayMs(time); // Delay for encoder B to settle

    // This disables the rotation while the button routine is running.

    // *** This series of conditional statements assume Running Display Screen
    // in MULTI mode.
    if ((MenuNo == 40) && (SM_FLG == 0)) {// Done with menu, goto running display.
        if (Encoder_Dir == CW) { // *** INCREASING DELAY (CCW)***
            Array_Count++; // Increase the count of both arrays.
            E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.

            if (Array_Count > ARRAY_MAX) { // Check if hit upper limit of array.
                Array_Count = ARRAY_MAX; // Oops, went above, hold at upper limit.
                E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.
            }
            // if count > 90 go into 3 digit mode, no right justify, 
            // three digit flag = 3.
            if (Array_DelayHex[Array_Count] > 0x57E) { // f count > 90. *** CHANGED FEB15/18 ***
                THREE_DIGIT_FLG = 1;
                E_Write(EDelay_3D_FLG, THREE_DIGIT_FLG); // Save place to EEPROM.
                place = 85;
                E_Write(EDelay_Place, place); // Save place to EEPROM.
            }
            LineWrite_XY_ILI9341_16x25(Array_DelayMs[Array_Count], place, Line2, ILI9341_WHITE, ILI9341_BLACK);

        } else if (Encoder_Dir == CCW) { // *** DECREASING DELAY (CW)***
            // Checking if count < 100 AND coming from 3 digits.
            if ((Array_DelayHex[Array_Count] <= 0x61A) && (THREE_DIGIT_FLG == 1)) {
                //  Effectively clears the digit no longer being printed. When 
                // working place one character at the proper location.
                place = 101; // Right justification
                E_Write(EDelay_Place, place); // Save place to EEPROM.
                // Places a blank at the MSB.
                LineWrite_XY_ILI9341_16x25(" ", 85, Line2, ILI9341_BLACK, ILI9341_BLACK);
                THREE_DIGIT_FLG = 0; // signals 2 digits
                E_Write(EDelay_3D_FLG, THREE_DIGIT_FLG); // Save THREE_DIGIT_FLG to EEPROM.
            }

            Array_Count--;
            E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.
            if (TMR1 >= Array_DelayHex[Array_Count]) // Check if TMR is above PRx, if
                TMR1 = Array_DelayHex[Array_Count - 2]; // so, stick TMR below PRx.; // so, stick TMR below PRx.

            if (Array_Count < 1) { // Check for lower array limit.
                Array_Count = 1; // Oops, went below, hold at lower limit.
                E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.
            }
            // Only clear the delay once when going from 100 - 90. Stops blanking.
            // When THREE_DIGIT_FLG = 1, count is 3 digits.

            LineWrite_XY_ILI9341_16x25(Array_DelayMs[Array_Count], place, Line2, ILI9341_WHITE, ILI9341_BLACK);
        }
        PR1 = Array_DelayHex[Array_Count]; // Load delay value into Timer Period Register.
    }


    // NOTE: encoder_B = 0: CW, encoder_B = 1: CCW
    // *** This series of conditional statements assume Menu Screen ***
    // Start with CCW then move to CW conditions.

    if (Encoder_Dir == CCW) {
        if (MenuNo == 11) {
            MenuNo = 12;
            Menu();
        }// Menu 10 to 20 (CCW)

        else if (MenuNo == 10) {
            MenuNo = 50; // Changed from MenuNo = 20.
            DoDown = 1;
            DoUp = 0;
            DoBack = 0;
            Menu();
            // Skip Next ClearOLED().

        }// Menu 51 to 52 (CCW)
        else if (MenuNo == 50) {
            MenuNo = 20; // Changed from MenuNo = 20.
            DoDown = 1;
            DoUp = 0;
            DoBack = 0;
            Menu();
            // Skip Next ClearOLED().
        }// Menu 51 to 22 (CCW)
        else if (MenuNo == 51) {
            MenuNo = 52;
            Menu();
        }// Menu 21 to 22 (CCW)
        else if (MenuNo == 21) {
            MenuNo = 22;
            Menu();
        }// Menu 20 to 30 (CCW)


        else if (MenuNo == 20) {
            MenuNo = 30;
            // Skip Next ClearOLED().
            Menu();
        }

        // Now determine actions for CW actions
    } else if (Encoder_Dir == CW) {
        // Menu 30 to 20 (CW)
        if (MenuNo == 30) {
            MenuNo = 20;
            DoUp = 1;
            DoDown = 0;
            DoBack = 0; // Skip Next ClearOLED().
            Menu();
        }// Menu 20 to 50 (CW)
        else if (MenuNo == 20) {
            MenuNo = 50;
            DoUp = 1;
            DoDown = 0;
            DoBack = 0;
            Menu();

        }// Menu 50 to 10 (CW)
        else if (MenuNo == 50) {
            MenuNo = 10;
            DoUp = 1;
            DoDown = 0;
            DoBack = 0;
            Menu();

        } else if (MenuNo == 12) {
            MenuNo = 11;
            DoForward = 0;
            DoDown = 0;
            DoUp = 1;
            Menu();
        }// Menu 52 to 51 (CW)
        else if (MenuNo == 52) {
            MenuNo = 51;
            DoForward = 0;
            DoDown = 0;
            DoUp = 1;
            Menu();
        }// Menu 22 to 21 (CCW)
        else if (MenuNo == 22) {
            // The disables the rotary encode during ARM/GLV conflict.
            if (ARM_GLV_FLG == 0) {
                MenuNo = 21;
                DoForward = 0;
                DoDown = 0;
                DoUp = 1;
                Menu();
            }
        }
    }
}

 */



// Fill this out for each klunker programmed.
/*
void Version(void) {

    FillScreen_ILI9341(ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Serial:", 0, Line0, ILI9341_GREENYELLOW, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("DD0301017-010", 0, Line1, ILI9341_WHITE, ILI9341_BLACK);//Personal Dutch Dozen
    LineWrite_XY_ILI9341_16x25("Software Version: ", 0, Line2, ILI9341_GREENYELLOW, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Dutch-Dozen-V5.0", 0, Line3, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("TAG:002", 0, Line4, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Hardware Version: ", 0, Line5, ILI9341_GREENYELLOW, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("dutch-dozen-cpu-V7.0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK);

    while ((_RF6 == 1) && (_RB4)) {
    } // Wait in Version screen until <RESET> is pressed. 
}
 */



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

