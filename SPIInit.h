
/* 
 * File:                SPIInit.h
 * Author:              James Vlasblom
 * Date:                June 14, 2018.
 * 
 * Comments:            This file contains the function SPIInit(void)
 *                      which initializes the SPI module for the TFT
 * 
 * Revision history:    
 * 

  
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPIINIT_50_Shot_H
#define	SPIINIT_50_Shot_H

#include <xc.h> 


void SPIInit(void);

void SPIInit(void) {


    // ***********************************************
    // *** Configuration of the SPI module ***********
    // ***********************************************

    // Note: Because SPI1 and SPI2 share some of the same registers only SPI1
    // is fully defined.

    // *** SPI1 ***

    // SPI1STAT - Status register

    _SPIEN = 1;         // 0 = disables module, 1 = enables module and 
                        // configures SCKx, SDOx, SDIx, and ~SSD as serial 
                        // port pins.
    _SPISIDL = 0;       // 0 = continues module operation in idle mode, 1 = 
                        // discontinues module in idle mode.
    _SPIBEC = 0;        // <2:0> SPIx buffer element count bits.
                        // Master mode: Number of SPI transfers pending.
                        // Slave mode: Number of SPI transfers unread.
    
 // *** OK down to business for SPI testing in SPI2
    
 // ***  Status Register    ***
    


IFS2bits.SPI2IF = 0;        // Clear Interrupt flag
  //IFS0bits.SPI1IF = 0;        // Clear Interrupt flag
IEC2bits.SPI2IE = 0;        // Disable SPI2 interrupts
  //IEC0bits.SPI1IE = 0;

// *** Control Register ****

SPI2CON1bits.DISSCK = 0;    // 0 = Internal SPI clock enabled, 1 = Internal
                            // SPI clock disabled, the pins functions as an I/O.
SPI2CON1bits.DISSDO = 0;    // 0 = SDO2 pin is controlled by the module. 
                            // 1 = SDO2 pin functions as an I/O.
SPI2CON1bits.MODE16 = 0;    // 0 = Communication word 8-bits wide.
                            // 1 = Communication word 16-bits wide.
SPI2CON1bits.MSTEN = 1;     // 0 = slave mode, 1 = master mode.

                        
SPI2CON1bits.SMP = 0;       // 0 = Input data sampled in middle of data stream.
                            // 1 = Input data sampled in end of data stream.
                            // Note: SMP must be cleared when in slave mode.
SPI2CON1bits.CKE = 1;       // 0 = serial output changes on active to idle clock
                            // 1 = serial output changes on idle to active clock

SPI2CON1bits.CKP = 0;       // 0 = idle clock is low, active if high.
                            // 1 = idle clock is high, active is low.
SPI2CON1bits.SSEN = 0;      // 1 = ~SS2 pin used for slave mode.
                            // 0 = ~SS2 pin not used, pin controlled by port fcn
SPI2CON1bits.SPRE = 7;      // <2:0> Secondary prescale, 111 = 1:1, 110 = 1:2.
Nop();
Nop();

SPI2CON1bits.PPRE = 3;      // <1:0> Primary prescale, 11 = 1:1, 10 = 4:1, 
                            // 01 = 16:1, 00 = 64:1.
SPI2STATbits.SPIEN = 1;     // 0 = disables module, 1 = enables module and 
                            // configures SCKx, SDOx, SDIx, and ~SSD as serial 
                            // port pins. *** Set this bit last.

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

