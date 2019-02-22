
/* 
 * File:            TFT_ILI9341.h
 * Author:          James Vlasblom
 * Date:            June 14, 2018
 * Comments:     
 * This file contains drivers for the 240RGB x 320 TFT LCD display which using
 * the ILI9341 driver chip.
 * 
 * Revision history: 
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TFT_ILI9341_H
#define	TFT_ILI9341_H


#include <xc.h> // include processor files - each processor file is guarded.  
#include "MyFunctions.h"
#include "Fonts.h"
#include <Math.h>
#include <stdlib.h>

// *** Definitions here

#define BLACK 0
#define COMMAND 0
#define WHITE 1
#define DATA 1
#define Y_OFFSET 10
#define LASER_X_L 15
#define LASER_X_R 300

// #define statements for serial connection to the display.
#define SPI_CS _LATA10            // Chip select for OLED - Serial Mode, initialized as 1.
// Do not use this for Laser Array code.
//#define SPI_RES _LATB14          // Reset for OLED - Serial Mode.
#define SPI_DC _LATB12            // Data / Command for OLED (LATG0 for pictail) - Serial Mode.
// Note: SCLK _LATC0          // Clock for OLED - Serial Mode.
// Note: MOSI _LATC1          // Master out, serial in for OLED - Serial Mode.

#define ILI9341_TFTWIDTH  320
#define ILI9341_TFTHEIGHT 240

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10    // Enter sleep mode
#define ILI9341_SLPOUT  0x11    // Sleep out.
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0D
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1
/*
#define ILI9341_PWCTR6  0xFC

 */


// Line definitions.

#define Line0   0
#define Line1   30
#define Line2   60
#define Line3   90
#define Line4   120
#define Line5   150
#define Line6   180
#define Line7   210

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFB64      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F
#define ILI9341_PHOSPHORGREEN 0x07C7

// *** Function Prototypes here ***


void CharWrite_XY_ILI9341_16x25(int digit, int x_start, int y_start,
        int fore_colour, int back_colour);
// This function puts crosshairs at the x, y coordinate with given 
// half segment length, and colour, and prints the X, Y coordinates as
// returned from X_Screen, and Y_Screen, in the top left corner of the screen.
void Crosshair_TFT_ILI9341(int x, int y, int len, long int colour);

void Display_TFT_ILI9341_Sleep(void); // Puts the LCD display to sleep.
void Display_TFT_ILI9341_Wake(void); // Wakes display up.
// Draws angled line with the specified parameters: midpoint of line, length,
// angle between -90 and +90 deg, and colour.
void DrawAngleLine_TFT_ILI9341(int x1, int y1, int length, float angle, int colour);

/* This function does the same as the above DrawAngleLine_TFT_ILI9341() with
 * the additional parameter of width which is the number of pairs of adjacent
 * lines flanking the original line. It uses DrawAngleLine_TFT_ILI9341()
 * within its function. 
 */
void DrawAngleLineWide_TFT_ILI9341(int x1, int y1, int length, int width, float angle, int colour);

// Draws circle with the specified parameters: centre coordinintes, radius, 
// number of rings starting from outside radius, colour. for solid circle,
// Rings = radius.
void DrawCircle_TFT_ILI9341(int x, int y, int rad, int rings, int colour);

void DrawLaserL(int x, int y, int Colour);
void DrawLaserR(int x, int y, int Colour);

void DrawPixel_ILI9341(int x, int y, int colour);
// This function draws a horizontal line of given colour starting at x, y 
// for given length.
void DrawHLine_TFT_ILI9341(int x, int y, int len, long int colour);
// This function draws a vertical line of given colour starting at x, y 
// for given length.
void DrawVLine_TFT_ILI9341(int x, int y, int len, long int colour);


void FillRec_ILI9341(long int x, long int y, long int w, long int h,
        long int colour);
void FillScreen_ILI9341(int colour);
void Initialize_TFT_ILI9341(void);
//void LineWrite_XY_ILI9341(char *digit, long int x, long int y, long int fore_colour,
//        long int back_colour);
void LineWrite_XY_ILI9341_16x25(char *digit, int x, int line, int fore_colour,
        int back_colour);
void SetAddrWindow_ILI9341(int X_Start, int Y_Start, int X_End, int Y_End);
void WriteCommand_ILI9341(unsigned char Command);
void WriteData_ILI9341(unsigned char Data);
//void WriteGRAM(int Colour);


// ****  Insert code here. *****

void Initialize_TFT_ILI9341(void) {
    //SPI_RES = 0; // Reset the TFT.
    DelayMs(120);
    //SPI_RES = 1; // Take TFT out of reset.

    WriteCommand_ILI9341(ILI9341_SWRESET);
    DelayMs(50); // Necessary delay for FTF to settle.


    WriteCommand_ILI9341(0xEF);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0x80);
    WriteData_ILI9341(0x02);

    WriteCommand_ILI9341(0xCF);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0XC1);
    WriteData_ILI9341(0X30);

    WriteCommand_ILI9341(0xED);
    WriteData_ILI9341(0x64);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0X12);
    WriteData_ILI9341(0X81);

    WriteCommand_ILI9341(0xE8);
    WriteData_ILI9341(0x85);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x78);

    WriteCommand_ILI9341(0xCB);
    WriteData_ILI9341(0x39);
    WriteData_ILI9341(0x2C);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x34);
    WriteData_ILI9341(0x02);

    WriteCommand_ILI9341(0xF7);
    WriteData_ILI9341(0x20);

    WriteCommand_ILI9341(0xEA);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x00);

    WriteCommand_ILI9341(ILI9341_PWCTR1); //Power control 
    WriteData_ILI9341(0x23); //VRH[5:0] 

    WriteCommand_ILI9341(ILI9341_PWCTR2); //Power control 
    WriteData_ILI9341(0x10); //SAP[2:0];BT[3:0] 

    WriteCommand_ILI9341(ILI9341_VMCTR1); //VCM control 
    WriteData_ILI9341(0x3e); //Â¶Ã?Â±Ã?Â¶Ã?ÂµÃ·Â½Ã?
    WriteData_ILI9341(0x28);

    WriteCommand_ILI9341(ILI9341_VMCTR2); //VCM control2 
    WriteData_ILI9341(0x86); //--

    WriteCommand_ILI9341(ILI9341_MADCTL); // Memory Access Control 
    WriteData_ILI9341(0x28);

    WriteCommand_ILI9341(ILI9341_PIXFMT); // DPI and DBI set to 16 bits/pixel
    WriteData_ILI9341(0x55); // 16 bits/pixel (65k colour, RGB 5-6-6 bit input)
    //WriteData_ILI9341(0x66); // 18 bits/pixel (262k colour, RGB 6-6-6 bit input)

    WriteCommand_ILI9341(ILI9341_FRMCTR1);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x18);

    WriteCommand_ILI9341(ILI9341_DFUNCTR); // Display Function Control 
    WriteData_ILI9341(0x08);
    WriteData_ILI9341(0x82);
    WriteData_ILI9341(0x27);

    WriteCommand_ILI9341(0xF2); // 3Gamma Function Disable 
    WriteData_ILI9341(0x00);

    WriteCommand_ILI9341(ILI9341_GAMMASET); //Gamma curve selected 
    WriteData_ILI9341(0x01);

    WriteCommand_ILI9341(ILI9341_GMCTRP1); //Set Gamma 
    WriteData_ILI9341(0x0F);
    WriteData_ILI9341(0x31);
    WriteData_ILI9341(0x2B);
    WriteData_ILI9341(0x0C);
    WriteData_ILI9341(0x0E);
    WriteData_ILI9341(0x08);
    WriteData_ILI9341(0x4E);
    WriteData_ILI9341(0xF1);
    WriteData_ILI9341(0x37);
    WriteData_ILI9341(0x07);
    WriteData_ILI9341(0x10);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0x0E);
    WriteData_ILI9341(0x09);
    WriteData_ILI9341(0x00);

    WriteCommand_ILI9341(ILI9341_GMCTRN1); //Set Gamma 
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x0E);
    WriteData_ILI9341(0x14);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0x11);
    WriteData_ILI9341(0x07);
    WriteData_ILI9341(0x31);
    WriteData_ILI9341(0xC1);
    WriteData_ILI9341(0x48);
    WriteData_ILI9341(0x08);
    WriteData_ILI9341(0x0F);
    WriteData_ILI9341(0x0C);
    WriteData_ILI9341(0x31);
    WriteData_ILI9341(0x36);
    WriteData_ILI9341(0x0F);

    WriteCommand_ILI9341(ILI9341_SLPOUT); //Exit Sleep 
    DelayMs(120);
    WriteCommand_ILI9341(ILI9341_DISPON); //Display on 
    DelayMs(120);
}

void Crosshair_TFT_ILI9341(int x, int y, int len, long int colour) {

    DrawHLine_TFT_ILI9341(x, y, len, colour); // Right horizontal segment.
    DrawVLine_TFT_ILI9341(x, y, len, colour); // Bottom vertical  segment.
    DrawHLine_TFT_ILI9341((x - len), y, len, colour); // Left horizontal segment.
    DrawVLine_TFT_ILI9341(x, (y - len), len, colour); // Top vertical segment.
}

void DrawVLine_TFT_ILI9341(int x, int y, int len, long int colour) {

    int colour_hi;
    int colour_low;

    // Set the memory window;
    SetAddrWindow_ILI9341(x, y, x, (y + len));

    colour_hi = colour >> 8;
    colour_low = colour;

    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop();

    SPI_DC = DATA; // Write Command, leave low
    SPI_CS = 0; // Activate ~CS   

    for (y = 0; y <= len; y++) {
        SPI2BUF = colour_hi;
        Nop(), Nop(), Nop(), Nop();
        Nop(), Nop(), Nop(), Nop();
        SPI2BUF = colour_low;
    }
    SPI_CS = 1; // deactivate ~CS. - end RAM read.
    IEC0bits.INT0IE = 1; // enable INT0 ISR
    Nop();
}

void DrawHLine_TFT_ILI9341(int x, int y, int len, long int colour) {

    int colour_hi;
    int colour_low;
    // Set the memory window;
    SetAddrWindow_ILI9341(x, y, (x + len), y);

    colour_hi = colour >> 8;
    colour_low = colour;

    SPI_DC = DATA; // Write Command, leave low
    SPI_CS = 0; // Activate ~CS   

    IEC0bits.INT0IE = 0; // disable INT0 ISR

    for (x = 0; x <= len; x++) {
        SPI2BUF = colour_hi;
        Nop(), Nop(), Nop(), Nop();
        Nop(), Nop(), Nop(), Nop();
        SPI2BUF = colour_low;
    }
    SPI_CS = 1; // deactivate ~CS. - end RAM read.
    IEC0bits.INT0IE = 1; // enable INT0 ISR
}

void Display_TFT_ILI9341_Sleep(void) {
    // These commands seem to save 100-200uA
    WriteCommand_ILI9341(ILI9341_DISPOFF);
    DelayMs(10);
    WriteCommand_ILI9341(ILI9341_SLPIN);
    DelayMs(10);
    WriteCommand_ILI9341(ILI9341_SLPOUT); //VCM control 
    WriteCommand_ILI9341(ILI9341_DISPON);

    // Change SPI pins, either high or OD, saves about 1 mA.
    _ODC0 = 1;
    _ODC1 = 1;
    _ODA10 = 1;
    _ODB12 = 1;
    _LATA8 = 1;

    SCREEN = OFF;
    DelayMs(500); // Time for relay to settle.
}

void Display_TFT_ILI9341_Wake(void) {
    SCREEN = ON;
    DelayMs(500); //  Necessary to let relay settle.
    _ODC0 = 0;
    _ODC1 = 0;
    _ODA10 = 0;
    _ODB12 = 0;
    _LATA8 = 0;
    Initialize_TFT_ILI9341();
    DelayMs(500);
}

void DrawAngleLine_TFT_ILI9341(int x1, int y1, int length, float angle, int colour) {
    /* Note: this TFT screen has its origin at the upper left hand corner, not
     * the lower left therefore this function does some trickery to get this
     * while sill using Quad I.
     * The line equation uses y = mx+b, and the Law of Sins to determine 
     * (x2, y2). 
     * For some unknown reason(at time of writing this), angles between -45
     * through +45 graph nicely but angles between -90 & -45, 45 & 90 graph
     * (nearest the Y-axis) graph with many missing points. The work around to
     * this was to graph wrt x instead of y at these angles, hence the 
     * conditional statements concerning the angles. An additional angle check
     * needed to be done because of the inverted Y-axis.
     */
    float xangle, yangle = 0; // x dimension of triangle.
    float x, y, y2, ny2, x2, nx2, m, nm, b, nb, radangle;

    // Calculate constants for the line.
    length = length / 2; // Because starting in centre of line.
    radangle = Deg2Rad(angle); // convert degrees to radians

    if ((angle >= -45) && (angle <= 45)) {
        // Calculate constants for this angle range
        xangle = radangle - (3 * PI / 2);
        m = (tan(radangle) * -1); // Make slope -ve to counter y axis backwards.
        b = y1 - (x1 * m); //  This makes this -ve b.
        x2 = x1 + (length * sin(xangle));
        nx2 = x1 - (length * sin(xangle));

        // -45 to +45 deg.    
        // Quad I & IV
        for (x = x1; x <= x2; x = x + 1) {
            y2 = (m * x) + b;
            DrawPixel_ILI9341(x, y2, colour);
        }

        // Quad I & IV
        for (x = x1; x >= nx2; x = x - 1) {
            y2 = (m * x) + b;
            DrawPixel_ILI9341(x, y2, colour);
        }
    }

    if (((angle >= -90) && (angle <= -46)) || ((angle >= 46) && (angle <= 90))) {

        yangle = radangle;
        m = (tan(radangle) * -1); // Make slope -ve to counter y axis backwards.
        b = y1 - (x1 * m); //  This makes this -ve b.
        nb = y1 - (x1 * m);
        y2 = y1 + (length * sin(yangle));
        ny2 = y1 - (length * sin(yangle));

        // -90 to -45 deg
        // Quads I & IV 
        for (y = y1; y <= ny2; y = y + 1) {
            x2 = (y - b) / m;
            DrawPixel_ILI9341(x2, y, colour);
        }

        // Quads II & III;
        for (y = y1; y >= y2; y = y - 1) {
            x2 = (y - b) / m;
            DrawPixel_ILI9341(x2, y, colour);
        }

        //46 to 90 deg
        // Quads II & III
        for (y = y1; y <= y2; y = y + 1) {
            x2 = (y - b) / m;
            DrawPixel_ILI9341(x2, y, colour);
        }

        // Quads I & IV;
        for (y = y1; y >= ny2; y = y - 1) {
            x2 = (y - b) / m;
            DrawPixel_ILI9341(x2, y, colour);
        }
    }
}

void DrawAngleLineWide_TFT_ILI9341(int x1, int y1, int length, int width, float angle, int colour) {
    int linecount = (width - 1);
    int newlen = length; // The decreasing length.
    int lenadj = 2; // Amount to shorten lengths each layer.

    float x1a, y1a = 0;
    DrawAngleLine_TFT_ILI9341(x1, y1, length, angle, colour); //Original line

    // Adjusting the x1, y1 coordinates according to the angle.
    x1a = (angle / 90);
    y1a = (90 - angle) / 90;

    while (linecount > 0) {
        newlen = newlen - lenadj; // To give the thicker line a rounded effect. 
        DrawAngleLine_TFT_ILI9341((x1 + x1a), (y1 + y1a), newlen, angle, colour);
        DrawAngleLine_TFT_ILI9341((x1 - x1a), (y1 - y1a), newlen, angle, colour);
        linecount--;
        x1a = x1a + x1a;
        y1a = y1a + y1a;
    }
}

void DrawCircle_TFT_ILI9341(int h, int k, int rad, int rings, int colour) {
    float x, y = 0;
    int count = rings;


    while (count) {

        for (x = h - (rad / sqrt(2)); x <= h + (rad / sqrt(2)); x = x + 0.5) {
            y = -(sqrt((rad * rad) - ((x - h) * (x - h)))) + k;
            DrawPixel_ILI9341(x, y, colour);
            y = (sqrt((rad * rad) - ((x - h) * (x - h)))) + k;
            DrawPixel_ILI9341(x, y, colour);
            //DelayMs(50);
        }


        for (y = k - (rad / sqrt(2)); y <= k + (rad / sqrt(2)); y = y + 1) {
            x = -(sqrt((rad * rad) - ((y - k) * (y - k)))) + h;
            DrawPixel_ILI9341(x, y, colour);
            x = (sqrt((rad * rad) - ((y - k) * (y - k)))) + h;
            DrawPixel_ILI9341(x, y, colour);
            //DelayMs(50);
        }

        count--;
        rad--;

    }
}

void DrawLaserL(int x, int y, int Colour) {
    float a = 0;
    for (a = -90; a <= 90; a = a + 30) {
        DrawAngleLine_TFT_ILI9341(x, (y + 22), 30, a, Colour);
    }
    for (a = -75; a <= 75; a = a + 30) {
        DrawAngleLine_TFT_ILI9341(x, (y + 22), 25, a, Colour);
    }
    DrawAngleLineWide_TFT_ILI9341((LASER_X_L + 2), (y + 22), 35, 2, -45, Colour);
    //DrawHLine_TFT_ILI9341((LASER_X_L + 15), (y + 34), 115, Colour);
    DrawAngleLineWide_TFT_ILI9341((LASER_X_L + 72), (y + 34), 120, 2, 0, Colour);
}

void DrawLaserR(int x, int y, int Colour) {
    float a = 0;
    for (a = -90; a <= 90; a = a + 30) {
        DrawAngleLine_TFT_ILI9341(x, (y + 22), 30, a, Colour);
    }
    for (a = -75; a <= 75; a = a + 30) {
        DrawAngleLine_TFT_ILI9341(x, (y + 22), 25, a, Colour);
    }
    DrawAngleLineWide_TFT_ILI9341((LASER_X_R), (y + 22), 35, 2, 45, Colour);
    //DrawHLine_TFT_ILI9341((LASER_X_L + 15), (y + 34), 115, Colour);
    DrawAngleLineWide_TFT_ILI9341((LASER_X_R - 71), (y + 34), 120, 2, 0, Colour);
}


void DrawPixel_ILI9341(int x, int y, int colour) {

    int colour_hi;
    int colour_low;

    if ((x < 0) || (x >= ILI9341_TFTWIDTH) || (y < 0) || (y >= ILI9341_TFTHEIGHT)) return;
    SetAddrWindow_ILI9341(x, y, x, y);

    colour_hi = colour >> 8;
    colour_low = colour;

    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop(); // Added for interrupt disable timing.

    SPI_DC = DATA; // Write Command, leave low
    SPI_CS = 0; // Activate ~CS   
    SPI2BUF = colour_hi;
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    SPI2BUF = colour_low;
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    SPI_CS = 1; // deactivate ~CS.

    IEC0bits.INT0IE = 1; // enable INT0 ISR
}

void FillRec_ILI9341(long int x, long int y, long int w, long int h, long int colour) {

    int colour_hi;
    int colour_low;

    //rudimentary clipping(drawChar w / big text requires this)
    if ((x >= ILI9341_TFTWIDTH) || (y >= ILI9341_TFTHEIGHT)) return;
    if ((x + w - 1) >= ILI9341_TFTWIDTH) w = ILI9341_TFTWIDTH - x;
    if ((y + h - 1) >= ILI9341_TFTHEIGHT) h = ILI9341_TFTHEIGHT - y;

    SetAddrWindow_ILI9341(x, y, x + w - 1, y + h - 1);

    colour_hi = colour >> 8;
    colour_low = colour;

    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop(); // Added for interrupt disable timing.

    SPI_DC = DATA; // Write Command, leave low
    SPI_CS = 0; // Activate ~CS   

    for (y = h; y > 0; y--) {
        for (x = w; x > 0; x--) {
            SPI2BUF = colour_hi;
            Nop(), Nop(), Nop();
            SPI2BUF = colour_low;
        }
    }
    SPI_CS = 1; // deactivate ~CS.

    IEC0bits.INT0IE = 1; // enable INT0 ISR
}

void FillScreen_ILI9341(int colour) {
    FillRec_ILI9341(0, 0, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, colour);
}

void SetAddrWindow_ILI9341(int X_Start, int Y_Start, int X_End, int Y_End) {
    WriteCommand_ILI9341(ILI9341_CASET); // Column addr set (Horizontal, 480 Max.)
    WriteData_ILI9341(X_Start >> 8);
    WriteData_ILI9341(X_Start & 0xFF); // XSTART 
    WriteData_ILI9341(X_End >> 8);
    WriteData_ILI9341(X_End & 0xFF); // XEND

    WriteCommand_ILI9341(ILI9341_PASET); // Page addr set (vertical, 320 Max))
    WriteData_ILI9341(Y_Start >> 8);
    WriteData_ILI9341(Y_Start); // YSTART
    WriteData_ILI9341(Y_End >> 8);
    WriteData_ILI9341(Y_End); // YEND

    WriteCommand_ILI9341(ILI9341_RAMWR); // write to RAM
}

/*
void CharWrite_XY_ILI9341(int digit, long int x_start, long int y_start,
        long int fore_colour, long int back_colour) {
    int col;
    int row;
    int test = 0;
    int Mask = 0x01;
    int temp = 0;

    for (col = 0; col <= 4; col++) {
        temp = Font[(digit * 5) + col];
        for (row = 0; row <= 8; row++) // Loop because characters are 5 wide
        {
            test = (temp & Mask);
            if (test == 1) {
                DrawPixel_ILI9341((col + x_start), (row + y_start), fore_colour);
            } else {
                DrawPixel_ILI9341((col + x_start), (row + y_start), back_colour);
            }
            temp = temp >> 1;
        }
        // This little bit adds a background coloured column at the end of a 
        // character. (AKA blank space)
        for (row = 0; row <= 8; row++) {// Loop because characters are 5 wide

            DrawPixel_ILI9341((col + x_start + 1), (row + y_start), back_colour);
        }
    }
}
 */
void CharWrite_XY_ILI9341_16x25(int digit, int x_start, int y_start,
        int fore_colour, int back_colour) {
    int col = 0;
    int row = 0;
    int test = 0;
    int Mask = 0x80;
    int temp = 0;
    for (row = 0; row <= 21; row++) {
        temp = Font_16x25[((digit - 32)* 50) + (2 * row)];
        for (col = 0; col <= 7; col++) {
            test = (temp & Mask);
            if (test == 0x80) {
                DrawPixel_ILI9341((col + x_start), (row + y_start), fore_colour);
            } else {
                DrawPixel_ILI9341((col + x_start), (row + y_start), back_colour);
            }
            temp = temp << 1;
        }

        temp = Font_16x25[((digit - 32) * 50) + ((2 * row) + 1)];
        for (col = 8; col <= 15; col++) {
            test = (temp & Mask);
            if (test == 0x80) {
                DrawPixel_ILI9341((col + x_start), (row + y_start), fore_colour);
            } else {
                DrawPixel_ILI9341((col + x_start), (row + y_start), back_colour);
            }
            temp = temp << 1;
        }
    }
}

/*
void LineWrite_XY_ILI9341(char *digit, long int x, long int y, long int fore_colour,
        long int back_colour) {

    long int xtemp = x;

    SetAddrWindow_ILI9341(x, y, 5, 8);

    for (; *digit != NULL;) {
        CharWrite_XY_ILI9341(*digit, xtemp, y, fore_colour, back_colour);
        digit++;
        xtemp = xtemp + 6; // Make 6 wide to account for the blank
        // at the end of the character.
        SetAddrWindow_ILI9341(xtemp, y, 5, 8);
    }

}
 */
void LineWrite_XY_ILI9341_16x25(char *digit, int x, int line, int fore_colour,
        int back_colour) {
    int xtemp = x;

    SetAddrWindow_ILI9341(x, line, 16, 22);

    for (; *digit != NULL;) {
        CharWrite_XY_ILI9341_16x25(*digit, xtemp, line, fore_colour, back_colour);

        // This next if statement checks if the digit is lower case, if so it
        // advances only 13 instead of 16, compressing the letters closer.
        // Looks better.

        if ((*digit >= 97) && (*digit <= 122)) {
            xtemp = xtemp + 13;
        } else {
            xtemp = xtemp + 16; // Make 6 wide to account for the blank
        }
        // at the end of the character.
        digit++;
        SetAddrWindow_ILI9341(xtemp, line, 16, 22);
    }
}

void WriteCommand_ILI9341(unsigned char Command) {

    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop(); // Added for interrupt disable timing.

    SPI_DC = COMMAND; // Write Command, leave low
    //Original delay of 12 NOPs approx 1.5uS.
    SPI_CS = 0; // Activate ~CS
    //Original delay of 15 NOPs approx 1.9uS.
    SPI2BUF = Command;
    //Original delay of 23 NOPs approx 2.8uS.
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    Nop(), Nop();
    SPI_CS = 1; // deactivate ~CS.

    IEC0bits.INT0IE = 1; // enable INT0 ISR
}

void WriteData_ILI9341(unsigned char Data) {

    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop(); // Added for interrupt disable timing.

    SPI_DC = DATA; // Write Command, leave low
    //Original delay of 12 NOPs approx 1.5uS.
    SPI_CS = 0; // Activate ~CS
    //Original delay of 15 NOPs approx 1.9uS.
    SPI2BUF = Data;

    //Original delay of 23 NOPs approx 2.8uS.
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    Nop(), Nop();
    SPI_CS = 1; // deactivate ~CS.

    IEC0bits.INT0IE = 1; // enable INT0 ISR
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

