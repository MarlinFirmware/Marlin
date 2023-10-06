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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DWIN_MARLINUI_LANDSCAPE)
  #define DWIN_WIDTH  480
  #define DWIN_HEIGHT 272
#else
  #define DWIN_WIDTH  272
  #define DWIN_HEIGHT 480
#endif

#define RECEIVED_NO_DATA         0x00
#define RECEIVED_SHAKE_HAND_ACK  0x01

#define FHONE                    0xAA

#define DWIN_SCROLL_UP   2
#define DWIN_SCROLL_DOWN 3

// Make sure DWIN_SendBuf is large enough to hold the largest string plus draw command and tail.
// Assume the narrowest (6 pixel) font and 2-byte gb2312-encoded characters.
extern uint8_t DWIN_SendBuf[11 + DWIN_WIDTH / 6 * 2];
extern uint8_t DWIN_BufTail[4];
extern uint8_t databuf[26];

inline void DWIN_Byte(size_t &i, const uint16_t bval) {
  DWIN_SendBuf[++i] = bval;
}

inline void DWIN_Word(size_t &i, const uint16_t wval) {
  DWIN_SendBuf[++i] = wval >> 8;
  DWIN_SendBuf[++i] = wval & 0xFF;
}

inline void DWIN_Long(size_t &i, const uint32_t lval) {
  DWIN_SendBuf[++i] = (lval >> 24) & 0xFF;
  DWIN_SendBuf[++i] = (lval >> 16) & 0xFF;
  DWIN_SendBuf[++i] = (lval >>  8) & 0xFF;
  DWIN_SendBuf[++i] = lval & 0xFF;
}

// Send the data in the buffer plus the packet tail
void DWIN_Send(size_t &i);

inline void DWIN_Text(size_t &i, const char * const string, uint16_t rlimit=0xFFFF) {
  if (!string) return;
  const size_t len = _MIN(sizeof(DWIN_SendBuf) - i, _MIN(strlen(string), rlimit));
  if (len == 0) return;
  memcpy(&DWIN_SendBuf[i+1], string, len);
  i += len;
}

inline void DWIN_Text(size_t &i, FSTR_P string, uint16_t rlimit=0xFFFF) {
  if (!string) return;
  const size_t len = _MIN(sizeof(DWIN_SendBuf) - i, _MIN(rlimit, strlen_P(FTOP(string))));
  if (len == 0) return;
  memcpy_P(&DWIN_SendBuf[i+1], string, len);
  i += len;
}

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
bool DWIN_Handshake();

// DWIN startup
void DWIN_Startup();

#if HAS_LCD_BRIGHTNESS
  // Set the backlight brightness
  //  brightness: (0x00-0xFF)
  void DWIN_LCD_Brightness(const uint8_t brightness);
#endif

// Set screen display direction
//  dir: 0=0°, 1=90°, 2=180°, 3=270°
void DWIN_Frame_SetDir(uint8_t dir);

// Update display
void DWIN_UpdateLCD();

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
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

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
//  rlimit: For draw less chars than string length use rlimit
void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const char * const string, uint16_t rlimit=0xFFFF);

inline void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, FSTR_P const ftitle) {
  #ifdef __AVR__
    char ctitle[strlen_P(FTOP(ftitle)) + 1];
    strcpy_P(ctitle, FTOP(ftitle));
    DWIN_Draw_String(bShow, size, color, bColor, x, y, ctitle);
  #else
    DWIN_Draw_String(bShow, size, color, bColor, x, y, FTOP(ftitle));
  #endif
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
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint32_t value);

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
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, int32_t value);

// Draw a floating point number
//  value: positive unscaled float value
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value);

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw JPG and cached in #0 virtual display area
//  id: Picture ID
void DWIN_JPG_ShowAndCache(const uint8_t id);

// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);

// Draw an Icon
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Show(bool IBD, bool BIR, bool BFI, uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);

// Draw an Icon from SRAM
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  x/y: Upper-left point
//  addr: SRAM address
void DWIN_ICON_Show(bool IBD, bool BIR, bool BFI, uint16_t x, uint16_t y, uint16_t addr);

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
void DWIN_JPG_CacheToN(uint8_t n, uint8_t id);

// Unzip the JPG picture to virtual display area #1
//  id: Picture ID
inline void DWIN_JPG_CacheTo1(uint8_t id) { DWIN_JPG_CacheToN(1, id); }

// Animate a series of icons
//  animID: Animation ID  up to 16
//  animate: animation on or off
//  libID: Icon library ID
//  picIDs: Icon starting ID
//  picIDe: Icon ending ID
//  x/y: Upper-left point
//  interval: Display time interval, unit 10mS
void DWIN_ICON_Animation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs, uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval);

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void DWIN_ICON_AnimationControl(uint16_t state);
