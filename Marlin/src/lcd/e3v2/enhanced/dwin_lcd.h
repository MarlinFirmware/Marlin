/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 3.6.1
 * Date: 2021/08/29
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/********************************************************************************
 * @file     lcd/e3v2/enhanced/dwin_lcd.h
 * @author   LEO / Creality3D - Enhanced by Miguel A. Risco-Castillo
 * @date     2021/08/09
 * @version  2.2.1
 * @brief    DWIN screen control functions
 ********************************************************************************/

#pragma once

#include <stdint.h>

#define RECEIVED_NO_DATA         0x00
#define RECEIVED_SHAKE_HAND_ACK  0x01

#define FHONE                    0xAA

#define DWIN_SCROLL_UP   2
#define DWIN_SCROLL_DOWN 3

#define DWIN_WIDTH  272
#define DWIN_HEIGHT 480

#define DWIN_DataLength (DWIN_WIDTH / 6 * 2)

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
bool DWIN_Handshake(void);

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
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color,  uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

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
//  widthAdjust: true=self-adjust character width; false=no adjustment
//  bShow: true=display background color; false=don't display background color
//  size: Font size
//  color: Character color
//  bColor: Background color
//  x/y: Upper-left coordinate of the string
//  *string: The string
//  rlimit: For draw less chars than string length use rlimit
void DWIN_Draw_String(bool widthAdjust, bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const char * const string, uint16_t rlimit = 0xFFFF);
inline void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const char * const string, uint16_t rlimit = 0xFFFF) {
  DWIN_Draw_String(0, bShow, size, color, bColor, x, y, string, rlimit);
}

class __FlashStringHelper;

inline void DWIN_Draw_String(bool widthAdjust, bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const __FlashStringHelper *title) {
  DWIN_Draw_String(widthAdjust, bShow, size, color, bColor, x, y, (char *)title);
}
inline void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const __FlashStringHelper *title) {
  DWIN_Draw_String(0, bShow, size, color, bColor, x, y, (char *)title);
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
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value);

// Draw a positive floating point number
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  color: Character color
//  bColor: Background color
//  iNum: Number of whole digits
//  fNum: Number of decimal digits
//  x/y: Upper-left point
//  value: Scaled positive float value
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);
//  value: positive float value
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value);

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Display QR code
//  The size of the QR code is (46*QR_Pixel)*(46*QR_Pixel) dot matrix
//  QR_Pixel: The pixel size occupied by each point of the QR code: 0x01-0x0F (1-16)
//  (Nx, Ny): The coordinates of the upper left corner displayed by the QR code
//  str: multi-bit data
void DWIN_Draw_QR(uint8_t QR_Pixel, uint16_t x, uint16_t y, char *string);

inline void DWIN_Draw_QR(uint8_t QR_Pixel, uint16_t x, uint16_t y, const __FlashStringHelper *title) {
  DWIN_Draw_QR(QR_Pixel, x, y, (char *)title);
}

// Draw JPG and cached in #0 virtual display area
// id: Picture ID
void DWIN_JPG_ShowAndCache(const uint8_t id);

// Draw an Icon
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Show(uint8_t IBD, uint8_t BIR, uint8_t BFI, uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);

// Draw an Icon with transparent background
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
inline void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  DWIN_ICON_Show(0, 0, 1, libID, picID, x, y);
}

// Draw an Icon from SRAM
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  x/y: Upper-left point
//  addr: SRAM address
void DWIN_ICON_Show(uint8_t IBD, uint8_t BIR, uint8_t BFI, uint16_t x, uint16_t y, uint16_t addr);

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
void DWIN_JPG_CacheToN(uint8_t n, uint8_t id);

// Unzip the JPG picture to virtual display area #1
//  id: Picture ID
inline void DWIN_JPG_CacheTo1(uint8_t id) { DWIN_JPG_CacheToN(1, id); }

// Copy area from current virtual display area to current screen
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y);

// Copy area from virtual display area to current screen
//  IBD: background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(uint8_t IBD, uint8_t BIR, uint8_t BFI, uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y);

// Copy area from virtual display area to current screen with transparent background
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
inline void DWIN_Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
  DWIN_Frame_AreaCopy(0, 0, 1, cacheID, xStart, yStart, xEnd, yEnd, x, y);
}

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

// Set LCD Brightness 0x00-0x0F
void DWIN_LCD_Brightness(const uint8_t brightness);

// Write buffer data to the SRAM or Flash
//  mem: 0x5A=32KB SRAM, 0xA5=16KB Flash
//  addr: start address
//  length: Bytes to write
//  data: address of the buffer with data
void DWIN_WriteToMem(uint8_t mem, uint16_t addr, uint16_t length, uint8_t *data);

// Write the contents of the 32KB SRAM data memory into the designated image memory space.
//  picID: Picture memory space location, 0x00-0x0F, each space is 32Kbytes
void DWIN_SRAMToPic(uint8_t picID);
