/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/********************************************************************************
 * @file     lcd/e3v2/marlinui/dwin_lcd.h
 * @brief    DWIN screen control functions
 ********************************************************************************/

#include <stdint.h>

#define RECEIVED_NO_DATA         0x00
#define RECEIVED_SHAKE_HAND_ACK  0x01

#define FHONE                    0xAA

#define DWIN_SCROLL_UP   2
#define DWIN_SCROLL_DOWN 3

#if DISABLED(DWIN_MARLINUI_LANDSCAPE)
  #define DWIN_WIDTH  272
  #define DWIN_HEIGHT 480
#else
  #define DWIN_WIDTH  480
  #define DWIN_HEIGHT 272
#endif

// Picture ID
#define DWIN_Boot_Horiz      0
#define DWIN_Boot_Vert       1
#define DWIN_MarlinUI_Assets 2

/**
 * 3-.0：The font size, 0x00-0x09, corresponds to the font size below:
 * 0x00=6*12   0x01=8*16   0x02=10*20  0x03=12*24  0x04=14*28
 * 0x05=16*32  0x06=20*40  0x07=24*48  0x08=28*56  0x09=32*64
 */
#define font6x12  0x00
#define font8x16  0x01
#define font10x20 0x02
#define font12x24 0x03
#define font14x28 0x04
#define font16x32 0x05
#define font20x40 0x06
#define font24x48 0x07
#define font28x56 0x08
#define font32x64 0x09

#define DWIN_FONT_MENU  font10x20
#define DWIN_FONT_STAT  font14x28
#define DWIN_FONT_ALERT font14x28

// Color
#define Color_White       0xFFFF
#define Color_Yellow      0xFF0F
#define Color_Error_Red   0xB000  // Error!
#define Color_Bg_Red      0xF00F  // Red background color
#define Color_Bg_Window   0x31E8  // Popup background color
#define Color_Bg_Heading  0x3344  // Static Heading
#define Color_Bg_Blue     0x1125  // Dark blue background color
#define Color_Bg_Black    0x0841  // Black background color
#define Color_IconBlue    0x45FA  // Lighter blue that matches icons/accents
#define Popup_Text_Color  0xD6BA  // Popup font background color
#define Line_Color        0x3A6A  // Split line color
#define Rectangle_Color   0xEE2F  // Blue square cursor color
#define Percent_Color     0xFE29  // Percentage color
#define BarFill_Color     0x10E4  // Fill color of progress bar
#define Select_Color      0x33BB  // Selected color

// Character matrix width x height
//#define LCD_WIDTH ((DWIN_WIDTH) / 8)
//#define LCD_HEIGHT ((DWIN_HEIGHT) / 12)

// ICON ID
#define BOOT_ICON           3 // Icon set file 3.ICO
#define ICON                4 // Icon set file 4.ICO

// MarlinUI Boot Icons
#define ICON_MarlinBoot            0
#define ICON_OpenSource            1
#define ICON_GitHubURL             2
#define ICON_MarlinURL             3
#define ICON_Copyright             4

// MarlinUI Icons
#define ICON_LOGO_Marlin           0
#define ICON_HotendOff             1
#define ICON_HotendOn              2
#define ICON_BedOff                3
#define ICON_BedOn                 4
#define ICON_Fan0                  5
#define ICON_Fan1                  6
#define ICON_Fan2                  7
#define ICON_Fan3                  8
#define ICON_Halted                9
#define ICON_Question             10
#define ICON_Alert                11
#define ICON_RotateCW             12
#define ICON_RotateCCW            13
#define ICON_UpArrow              14
#define ICON_DownArrow            15
#define ICON_BedLine              16

#define ICON_AdvSet               ICON_Language
#define ICON_HomeOff              ICON_AdvSet
#define ICON_HomeOffX             ICON_StepX
#define ICON_HomeOffY             ICON_StepY
#define ICON_HomeOffZ             ICON_StepZ
#define ICON_ProbeOff             ICON_AdvSet
#define ICON_ProbeOffX            ICON_StepX
#define ICON_ProbeOffY            ICON_StepY
#define ICON_PIDNozzle            ICON_SetEndTemp
#define ICON_PIDbed               ICON_SetBedTemp

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
bool DWIN_Handshake(void);

// Common DWIN startup
void DWIN_Startup(void);

// Set the backlight luminance
//  luminance: (0x00-0xFF)
void DWIN_Backlight_SetLuminance(const uint8_t luminance);

// Set screen display direction
//  dir: 0=0°, 1=90°, 2=180°, 3=270°
void DWIN_Frame_SetDir(uint8_t dir);

// Update display
void DWIN_UpdateLCD(void);

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Clear screen
//  color: Clear screen color
void DWIN_Frame_Clear(const uint16_t color);

// Draw a point
//  color: point color
//  width: point width   0x01-0x0F
//  height: point height 0x01-0x0F
//  x,y: upper left point
void DWIN_Draw_Point(uint16_t color, uint8_t width, uint8_t height, uint16_t x, uint16_t y);

// Draw a line
//  color: Line segment color
//  xStart/yStart: Start point
//  xEnd/yEnd: End point
void DWIN_Draw_Line(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

// Draw a Horizontal line
//  color: Line segment color
//  xStart/yStart: Start point
//  xLength: Line Length
inline void DWIN_Draw_HLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xLength) {
  DWIN_Draw_Line(color, xStart, yStart, xStart + xLength - 1, yStart);
}

// Draw a Vertical line
//  color: Line segment color
//  xStart/yStart: Start point
//  yLength: Line Length
inline void DWIN_Draw_VLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t yLength) {
  DWIN_Draw_Line(color, xStart, yStart, xStart, yStart + yLength - 1);
}

// Draw a rectangle
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xEnd/yEnd: lower right point
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color,
                         uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

// Draw a box
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xSize/ySize: box size
inline void DWIN_Draw_Box(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xSize, uint16_t ySize) {
  DWIN_Draw_Rectangle(mode, color, xStart, yStart, xStart + xSize - 1, yStart + ySize - 1);
}

// Move a screen area
//  mode: 0, circle shift; 1, translation
//  dir: 0=left, 1=right, 2=up, 3=down
//  dis: Distance
//  color: Fill color
//  xStart/yStart: upper left point
//  xEnd/yEnd: bottom right point
void DWIN_Frame_AreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

/*---------------------------------------- Text related functions ----------------------------------------*/

// Draw a string
//  bShow: true=display background color; false=don't display background color
//  size: Font size
//  color: Character color
//  bColor: Background color
//  x/y: Upper-left coordinate of the string
//  *string: The string
void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, char *string);

class __FlashStringHelper;

inline void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const __FlashStringHelper *title) {
  DWIN_Draw_String(bShow, size, color, bColor, x, y, (char *)title);
}

// Draw a positive integer
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of digits
//  x/y: Upper-left coordinate
//  value: Integer value
void DWIN_Draw_IntValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint16_t value);

// Draw a floating point number
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of whole digits
//  fNum: Number of decimal digits
//  x/y: Upper-left point
//  value: Float value
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw JPG and cached in #0 virtual display area
// id: Picture ID
void DWIN_JPG_ShowAndCache(const uint8_t id);

// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
void DWIN_JPG_CacheToN(uint8_t n, uint8_t id);

// Unzip the JPG picture to virtual display area #1
//  id: Picture ID
inline void DWIN_JPG_CacheTo1(uint8_t id) { DWIN_JPG_CacheToN(1, id); }

// Copy area from virtual display area to current screen
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y);

// Animate a series of icons
//  animID: Animation ID  up to 16
//  animate: animation on or off
//  libID: Icon library ID
//  picIDs: Icon starting ID
//  picIDe: Icon ending ID
//  x/y: Upper-left point
//  interval: Display time interval, unit 10mS
void DWIN_ICON_Animation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs,
                         uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval);

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void DWIN_ICON_AnimationControl(uint16_t state);
