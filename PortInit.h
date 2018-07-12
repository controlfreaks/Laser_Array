
/* 
 * File:        PortInit.h
 * Author:      James Vlasblom
 * Date:        June 14, 2018
 * 
 * Comments:    This file contains the function 'PortInit()', 
 *              which contains all the code for all the PORT initialization
 *              including input / output, open drain, and initial value. PortB
 *              also has Analog / Digital designation.
 * 
 *              This file also contains the necessary RPx pin re-mapping for
 *              the required modules.
 * 
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PORTINIT_50_Shot_H
#define	PORTINIT_50_Shot_H

#include <xc.h> // include processor files - each processor file is guarded.  



void PortInit(void);

void PortInit(void) {

    // ****************************
    // *** Redefine PORT MACROS ***
    // ****************************

    // *** OUTPUTS ***
#define Laser_LED1 _LATA9
#define Laser_LED2 _LATA8
#define Laser_LED3 _LATC2
#define Laser_LED5 _LATC4

    //************************
    //************************
    //*         ADC          *
    //************************
    //************************

    // *** Setting Inputs as Analog or Digital (PORTB only) ***

    //AD1PCFG = 0xFF;     // set entire PORT digital

    _PCFG0 = 0; // 0 = analog, 1 = digital
    _PCFG1 = 1; // 0 = analog, 1 = digital
    _PCFG2 = 1; // 0 = analog, 1 = digital
    _PCFG3 = 1; // 0 = analog, 1 = digital
    _PCFG4 = 1; // 0 = analog, 1 = digital
    _PCFG5 = 1; // 0 = analog, 1 = digital
    _PCFG6 = 1; // 0 = analog, 1 = digital
    _PCFG7 = 1; // 0 = analog, 1 = digital
    _PCFG8 = 1; // 0 = analog, 1 = digital
    _PCFG9 = 1; // 0 = analog, 1 = digital
    _PCFG10 = 1; // 0 = analog, 1 = digital
    _PCFG11 = 1; // 0 = analog, 1 = digital
    _PCFG12 = 1; // 0 = analog, 1 = digital
    // no _PCFG13.
    // no _PCFG13.
    _PCFG15 = 1; // 0 = analog, 1 = digital


    //*** INPUTS ***
    //#define ENCDR_SW PORTAbits.RA14


    //*** All PORTS are inputs at POR ***

    //************************
    //************************
    //* PortA initialization *
    //************************
    //************************

    //*** INPUT / OUTPUT Configuration***

    //TRISA = 0x00;      // Set entire PORT

    _TRISA0 = 1; // 0 = output, 1 = input   // AN0
    _TRISA1 = 0; // 0 = output, 1 = input   // Power LED
    _TRISA2 = 0; // 0 = output, 1 = input
    _TRISA3 = 0; // 0 = output, 1 = input 
    _TRISA4 = 0; // 0 = output, 1 = input
    // NO _TRISA5
    // NO _TRISA6
    _TRISA7 = 0; // 0 = output, 1 = input
    _TRISA8 = 0; // 0 = output, 1 = input    
    _TRISA9 = 0; // 0 = output, 1 = input
    _TRISA10 = 0; // 0 = output, 1 = input  // SPI-CS output.


    // *** Open Drain Configuration ***

    //ODCA = 0x00;         // Set entire PORT

    _ODA0 = 0; // 0 = normal, 1 = open drain
    _ODA1 = 0; // 0 = normal, 1 = open drain
    _ODA2 = 0; // 0 = normal, 1 = open drain        
    _ODA3 = 0; // 0 = normal, 1 = open drain        
    _ODA4 = 0; // 0 = normal, 1 = open drain 
    //no ODA5       
    //no ODA6 
    _ODA7 = 0; // 0 = normal, 1 = open drain 
    _ODA8 = 0; // 0 = normal, 1 = open drain 
    _ODA9 = 0; // 0 = normal, 1 = open drain
    _ODA10 = 0; // 0 = normal, 1 = open drain


    // *** Initial Port settings, entire of individual ***

    //LATTA = 0x8000;   // initial port settings

    _LATA0 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA2 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA3 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA4 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _LATA5
    // no _LATA6
    _LATA7 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA8 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA9 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA10 = 0; // 0 = low(0v), 1 = high(+3.3V)


    //***********************
    //***********************
    // PORTB initialization *
    //***********************
    //***********************

    //*** INPUT / OUTPUT Configuration***

    //TRISB = 0x00;      // Set entire PORT

    // Note: Must be set for analog inputs for A/D.

    _TRISB0 = 1; // 0 = output, 1 = input   // PCEC1
    _TRISB1 = 1; // 0 = output, 1 = input   // PCEG1
    _TRISB2 = 1; // 0 = output, 1 = input   // ENCOD-2
    _TRISB3 = 1; // 0 = output, 1 = input   // ENCOD-A
    _TRISB4 = 1; // 0 = output, 1 = input
    _TRISB5 = 1; // 0 = output, 1 = input
    _TRISB6 = 0; // 0 = output, 1 = input
    _TRISB7 = 0; // 0 = output, 1 = input
    _TRISB8 = 1; // 0 = output, 1 = input   // I2C-SCL1
    _TRISA9 = 1; // 0 = output, 1 = input   // I2C-SDA1
    _TRISA10 = 0; // 0 = output, 1 = input
    _TRISB11 = 0; // 0 = output, 1 = input
    _TRISB12 = 0; // 0 = output, 1 = input  // SPI-DC
    _TRISB13 = 0; // 0 = output, 1 = input
    _TRISB14 = 0; // 0 = output, 1 = input  // SPI-RST
    _TRISB15 = 0; // 0 = output, 1 = input  // Re-mapped to INT1

    // *** Open Drain Configuration ***

    //ODCB = 0x00;         // Set entire PORT

    _ODB0 = 0; // 0 = normal, 1 = open drain
    _ODB1 = 0; // 0 = normal, 1 = open drain
    _ODB2 = 0; // 0 = normal, 1 = open drain        
    _ODB3 = 0; // 0 = normal, 1 = open drain        
    _ODB4 = 0; // 0 = normal, 1 = open drain 
    _ODB5 = 0; // 0 = normal, 1 = open drain         
    _ODB6 = 0; // 0 = normal, 1 = open drain
    _ODB7 = 0; // 0 = normal, 1 = open drain  
    _ODB8 = 0; // 0 = normal, 1 = open drain 
    _ODB9 = 0; // 0 = normal, 1 = open drain 
    _ODB10 = 0; // 0 = normal, 1 = open drain 
    _ODB11 = 0; // 0 = normal, 1 = open drain 
    _ODB12 = 0; // 0 = normal, 1 = open drain 
    _ODB13 = 0; // 0 = normal, 1 = open drain 
    _ODB14 = 0; // 0 = normal, 1 = open drain 
    _ODB15 = 0; // 0 = normal, 1 = open drain 

    // *** Initial Port settings, entire of individual ***

    //LATB = 0x00; // Set entire PORT

    //_LATB0 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB1 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    _LATB2 = 0;        // 0 = low(0v), 1 = high(+3.3V)  //  temp SDA2
    _LATB3 = 0;        // 0 = low(0v), 1 = high(+3.3V) // temp SCL2
    //_LATB4 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB5 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB6 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB7 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB8 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB9 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB10 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB11 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB12 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB13 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB14 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    _LATB15 = 0;       // 0 = low(0v), 1 = high(+3.3V)

    //************************
    //************************
    //* PortC initialization *
    //************************
    //************************


    //*** INPUT / OUTPUT Configuration***

    //TRISC = 0x00;         // Set entire PORT

    _TRISC0 = 0; // 0 = output, 1 = input  // re-mapped for RP16 (SPI-SDO1) 
    _TRISC1 = 0; // 0 = output, 1 = input   // re-mapped for RP17 (SPI-SCLKO1)
    _TRISC2 = 0; // 0 = output, 1 = input
    _TRISC3 = 0; // 0 = output, 1 = input 
    _TRISC4 = 0; // 0 = output, 1 = input
    _TRISC5 = 0; // 0 = output, 1 = input   // EXT_FAN
    _TRISC6 = 0; // 0 = output, 1 = input
    _TRISC7 = 0; // 0 = output, 1 = input 
    _TRISC8 = 0; // 0 = output, 1 = input   // LAS_RLY
    _TRISC9 = 1; // 0 = output, 1 = input   // POW-SW
    // no _TRISC10
    // no _TRISC11
    // no _TRISC12
    // no _TRISC13
    // no _TRISC14
    // no _TRISC15

    // *** Open Drain Configuration ***

    //ODCC = 0x00;         // Set entire PORT

    _ODC0 = 0; // 0 = normal, 1 = open drain
    _ODC1 = 0; // 0 = normal, 1 = open drain
    _ODC2 = 0; // 0 = normal, 1 = open drain        
    _ODC3 = 0; // 0 = normal, 1 = open drain        
    _ODC4 = 0; // 0 = normal, 1 = open drain 
    _ODC5 = 0; // 0 = normal, 1 = open drain
    _ODC6 = 0; // 0 = normal, 1 = open drain
    _ODC7 = 0; // 0 = normal, 1 = open drain        
    _ODC8 = 0; // 0 = normal, 1 = open drain        
    _ODC9 = 0; // 0 = normal, 1 = open drain 
    // no _ODC10
    // no _ODC11
    // no _ODC12
    // no _ODC13
    // no _ODC14
    // no _ODC15

    // *** Initial Port settings, entire of individual ***

    //LATTC = 0x6a;   // initial port settings

    _LATC0 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC2 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC3 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC4 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC5 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC6 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC7 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC8 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC9 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _RC10
    // no _RC11
    // no _RC12
    // no _RC13
    // no _RC14
    // no _RC15

    //************************
    //************************
    //* PortC re-mapping     *
    //************************
    //************************
    
    // RC0 and RC1 are re-mapped for SCI_SDO1 and SCLKo1 respectively. This is
    // done writing to the RPOP8 register.
    
    RPOR8 = 0x0B0A; // re-map SPI1 module to RP16, RP17 (RC0, RC1).

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

