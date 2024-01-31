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
#include "../../../inc/MarlinConfigPre.h"

#if HAS_DWIN_E3V2 || IS_DWIN_MARLINUI

#include "dwin_api.h"
#include "dwin_set.h"
#include "dwin_font.h"

#include "../../../inc/MarlinConfig.h"

#include <string.h> // for memset

uint8_t DWIN_SendBuf[11 + DWIN_WIDTH / 6 * 2] = { 0xAA };
uint8_t DWIN_BufTail[4] = { 0xCC, 0x33, 0xC3, 0x3C };
uint8_t databuf[26] = { 0 };

// Send the data in the buffer plus the packet tail
void DWIN_Send(size_t &i) {
  ++i;
  for (uint8_t n = 0; n < i; ++n) { LCD_SERIAL.write(DWIN_SendBuf[n]); delayMicroseconds(1); }
  for (uint8_t n = 0; n < 4; ++n) { LCD_SERIAL.write(DWIN_BufTail[n]); delayMicroseconds(1); }
}

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
bool DWIN_Handshake() {
  static int recnum = 0;
  #ifndef LCD_BAUDRATE
    #define LCD_BAUDRATE 115200
  #endif
  LCD_SERIAL.begin(LCD_BAUDRATE);
  const millis_t serial_connect_timeout = millis() + 1000UL;
  while (!LCD_SERIAL.connected() && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }

  size_t i = 0;
  DWIN_Byte(i, 0x00);
  DWIN_Send(i);

  while (LCD_SERIAL.available() > 0 && recnum < (signed)sizeof(databuf)) {
    databuf[recnum] = LCD_SERIAL.read();
    // ignore the invalid data
    if (databuf[0] != FHONE) { // prevent the program from running.
      if (recnum > 0) {
        recnum = 0;
        ZERO(databuf);
      }
      continue;
    }
    delay(10);
    recnum++;
  }

  return ( recnum >= 3
        && databuf[0] == FHONE
        && databuf[1] == '\0'
        && databuf[2] == 'O'
        && databuf[3] == 'K' );
}

#if HAS_LCD_BRIGHTNESS
  // Set LCD backlight (from DWIN Enhanced)
  //  brightness: 0x00-0xFF
  void DWIN_LCD_Brightness(const uint8_t brightness) {
    size_t i = 0;
    DWIN_Byte(i, 0x30);
    DWIN_Byte(i, brightness);
    DWIN_Send(i);
  }
#endif

// Get font character width
uint8_t fontWidth(uint8_t cfont) {
  switch (cfont) {
    case font6x12 : return 6;
    case font8x16 : return 8;
    case font10x20: return 10;
    case font12x24: return 12;
    case font14x28: return 14;
    case font16x32: return 16;
    case font20x40: return 20;
    case font24x48: return 24;
    case font28x56: return 28;
    case font32x64: return 32;
    default: return 0;
  }
}

// Get font character height
uint8_t fontHeight(uint8_t cfont) {
  switch (cfont) {
    case font6x12 : return 12;
    case font8x16 : return 16;
    case font10x20: return 20;
    case font12x24: return 24;
    case font14x28: return 28;
    case font16x32: return 32;
    case font20x40: return 40;
    case font24x48: return 48;
    case font28x56: return 56;
    case font32x64: return 64;
    default: return 0;
  }
}

// Set screen display direction
//  dir: 0=0°, 1=90°, 2=180°, 3=270°
void DWIN_Frame_SetDir(uint8_t dir) {
  size_t i = 0;
  DWIN_Byte(i, 0x34);
  DWIN_Byte(i, 0x5A);
  DWIN_Byte(i, 0xA5);
  DWIN_Byte(i, dir);
  DWIN_Send(i);
}

// Update display
void DWIN_UpdateLCD() {
  size_t i = 0;
  DWIN_Byte(i, 0x3D);
  DWIN_Send(i);
}

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Clear screen
//  color: Clear screen color
void DWIN_Frame_Clear(const uint16_t color) {
  size_t i = 0;
  DWIN_Byte(i, 0x01);
  DWIN_Word(i, color);
  DWIN_Send(i);
}

// Draw a point
//  color: point color
//  width: point width   0x01-0x0F
//  height: point height 0x01-0x0F
//  x,y: upper left point
void DWIN_Draw_Point(uint16_t color, uint8_t width, uint8_t height, uint16_t x, uint16_t y) {
  size_t i = 0;
  DWIN_Byte(i, 0x02);
  DWIN_Word(i, color);
  DWIN_Byte(i, width);
  DWIN_Byte(i, height);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

// Draw a line
//  color: Line segment color
//  xStart/yStart: Start point
//  xEnd/yEnd: End point
void DWIN_Draw_Line(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x03);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

// Draw a rectangle
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xEnd/yEnd: lower right point
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x05);
  DWIN_Byte(i, mode);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

// Move a screen area
//  mode: 0, circle shift; 1, translation
//  dir: 0=left, 1=right, 2=up, 3=down
//  dis: Distance
//  color: Fill color
//  xStart/yStart: upper left point
//  xEnd/yEnd: bottom right point
void DWIN_Frame_AreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x09);
  DWIN_Byte(i, (mode << 7) | dir);
  DWIN_Word(i, dis);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

/*---------------------------------------- Text related functions ----------------------------------------*/

// Draw a string
//  widthAdjust: true=self-adjust character width; false=no adjustment
//  bShow: true=display background color; false=don't display background color
//  size: Font size
//  color: Character color
//  bColor: Background color
//  x/y: Upper-left coordinate of the string
//  *string: The string
//  rlimit: To limit the drawn string length
void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const char * const string, uint16_t rlimit/*=0xFFFF*/) {
  #if NONE(DWIN_LCD_PROUI, DWIN_CREALITY_LCD_JYERSUI, IS_DWIN_MARLINUI)
    DWIN_Draw_Rectangle(1, bColor, x, y, x + (fontWidth(size) * strlen_P(string)), y + fontHeight(size));
  #endif
  constexpr uint8_t widthAdjust = 0;
  size_t i = 0;
  DWIN_Byte(i, 0x11);
  // Bit 7: widthAdjust
  // Bit 6: bShow
  // Bit 5-4: Unused (0)
  // Bit 3-0: size
  DWIN_Byte(i, (widthAdjust * 0x80) | (bShow * 0x40) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Text(i, string, rlimit);
  DWIN_Send(i);
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
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint32_t value) {
  size_t i = 0;
  #if DISABLED(DWIN_CREALITY_LCD_JYERSUI)
    DWIN_Draw_Rectangle(1, bColor, x, y, x + fontWidth(size) * iNum + 1, y + fontHeight(size));
  #endif
  DWIN_Byte(i, 0x14);
  // Bit 7: bshow
  // Bit 6: 1 = signed; 0 = unsigned number;
  // Bit 5: zeroFill
  // Bit 4: zeroMode
  // Bit 3-0: size
  DWIN_Byte(i, (bShow * 0x80) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, 0); // fNum
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  #if 0
    for (char count = 0; count < 8; count++) {
      DWIN_Byte(i, value);
      value >>= 8;
      if (!(value & 0xFF)) break;
    }
  #else
    // Write a big-endian 64 bit integer
    const size_t p = i + 1;
    for (char count = 8; count--;) { // 7..0
      ++i;
      DWIN_SendBuf[p + count] = value;
      value >>= 8;
    }
  #endif

  DWIN_Send(i);
}

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
                          uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, int32_t value) {
  //uint8_t *fvalue = (uint8_t*)&value;
  size_t i = 0;
  #if DISABLED(DWIN_CREALITY_LCD_JYERSUI)
    DWIN_Draw_Rectangle(1, bColor, x, y, x + fontWidth(size) * (iNum+fNum+1), y + fontHeight(size));
  #endif
  DWIN_Byte(i, 0x14);
  DWIN_Byte(i, (bShow * 0x80) | (zeroFill * 0x20) | (zeroMode * 0x10) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, fNum);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Long(i, value);
  /*
  DWIN_Byte(i, fvalue[3]);
  DWIN_Byte(i, fvalue[2]);
  DWIN_Byte(i, fvalue[1]);
  DWIN_Byte(i, fvalue[0]);
  */
  DWIN_Send(i);
}

// Draw a floating point number
//  value: positive unscaled float value
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
  const int32_t val = round(value * POW(10, fNum));
  DWIN_Draw_FloatValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, val);
}

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw JPG and cached in #0 virtual display area
//  id: Picture ID
void DWIN_JPG_ShowAndCache(const uint8_t id) {
  size_t i = 0;
  DWIN_Word(i, 0x2200);
  DWIN_Byte(i, id);
  DWIN_Send(i);     // AA 23 00 00 00 00 08 00 01 02 03 CC 33 C3 3C
}

// Draw an Icon
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Show(bool IBD, bool BIR, bool BFI, uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;
  DWIN_Byte(i, 0x23);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, (IBD << 7) | (BIR << 6) | (BFI << 5) | libID);
  DWIN_Byte(i, picID);
  DWIN_Send(i);
}

// Draw an Icon from SRAM
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  x/y: Upper-left point
//  addr: SRAM address
void DWIN_ICON_Show(bool IBD, bool BIR, bool BFI, uint16_t x, uint16_t y, uint16_t addr) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;
  DWIN_Byte(i, 0x24);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, (IBD << 7) | (BIR << 6) | (BFI << 5) | 0x00);
  DWIN_Word(i, addr);
  DWIN_Send(i);
}

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
void DWIN_JPG_CacheToN(uint8_t n, uint8_t id) {
  size_t i = 0;
  DWIN_Byte(i, 0x25);
  DWIN_Byte(i, n);
  DWIN_Byte(i, id);
  DWIN_Send(i);
}

// Animate a series of icons
//  animID: Animation ID; 0x00-0x0F
//  animate: true on; false off;
//  libID: Icon library ID
//  picIDs: Icon starting ID
//  picIDe: Icon ending ID
//  x/y: Upper-left point
//  interval: Display time interval, unit 10mS
void DWIN_ICON_Animation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs, uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;
  DWIN_Byte(i, 0x28);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  // Bit 7: animation on or off
  // Bit 6: start from begin or end
  // Bit 5-4: unused (0)
  // Bit 3-0: animID
  DWIN_Byte(i, (animate * 0x80) | 0x40 | animID);
  DWIN_Byte(i, libID);
  DWIN_Byte(i, picIDs);
  DWIN_Byte(i, picIDe);
  DWIN_Byte(i, interval);
  DWIN_Send(i);
}

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void DWIN_ICON_AnimationControl(uint16_t state) {
  size_t i = 0;
  DWIN_Byte(i, 0x29);
  DWIN_Word(i, state);
  DWIN_Send(i);
}

/*---------------------------------------- Memory functions ----------------------------------------*/
// The LCD has an additional 32KB SRAM and 16KB Flash
// Data can be written to the SRAM and saved to one of the jpeg page files

// Write Data Memory
//  command 0x31
//  Type: Write memory selection; 0x5A=SRAM; 0xA5=Flash
//  Address: Write data memory address; 0x000-0x7FFF for SRAM; 0x000-0x3FFF for Flash
//  Data: data
//
//  Flash writing returns 0xA5 0x4F 0x4B

// Read Data Memory
//  command 0x32
//  Type: Read memory selection; 0x5A=SRAM; 0xA5=Flash
//  Address: Read data memory address; 0x000-0x7FFF for SRAM; 0x000-0x3FFF for Flash
//  Length: leangth of data to read; 0x01-0xF0
//
//  Response:
//    Type, Address, Length, Data

// Write Picture Memory
//  Write the contents of the 32KB SRAM data memory into the designated image memory space
//  Issued: 0x5A, 0xA5, PIC_ID
//  Response: 0xA5 0x4F 0x4B
//
//  command 0x33
//  0x5A, 0xA5
//  PicId: Picture Memory location, 0x00-0x0F
//
//  Flash writing returns 0xA5 0x4F 0x4B

#endif // HAS_DWIN_E3V2 || IS_DWIN_MARLINUI
