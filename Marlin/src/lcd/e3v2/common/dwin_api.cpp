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

#include "../../../inc/MarlinConfig.h"

#include <string.h> // for memset

// Make sure dwinSendBuf is large enough to hold the largest string plus draw command and tail.
// Assume the narrowest (6 pixel) font and 2-byte gb2312-encoded characters.
uint8_t dwinSendBuf[11 + DWIN_WIDTH / 6 * 2] = { 0xAA };
uint8_t dwinBufTail[4] = { 0xCC, 0x33, 0xC3, 0x3C };
uint8_t databuf[26] = { 0 };
bool need_lcd_update = true;

// Send the data in the buffer plus the packet tail
void dwinSend(size_t &i) {
  ++i;
  for (uint8_t n = 0; n < i; ++n) { LCD_SERIAL.write(dwinSendBuf[n]); delayMicroseconds(1); }
  for (uint8_t n = 0; n < 4; ++n) { LCD_SERIAL.write(dwinBufTail[n]); delayMicroseconds(1); }
  need_lcd_update = true;
}

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
bool dwinHandshake() {
  int recnum = 0;
  #ifndef LCD_BAUDRATE
    #define LCD_BAUDRATE 115200
  #endif
  LCD_SERIAL.begin(LCD_BAUDRATE);
  const millis_t serial_connect_timeout = millis() + 1000UL;
  while (!LCD_SERIAL.connected() && PENDING(millis(), serial_connect_timeout)) { /*nada*/ }

  size_t i = 0;
  dwinByte(i, 0x00);
  dwinSend(i);
  delay(10);

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
  void dwinLCDBrightness(const uint8_t brightness) {
    size_t i = 0;
    dwinByte(i, 0x30);
    dwinByte(i, brightness);
    dwinSend(i);
  }
#endif

// Set screen display direction
//  dir: 0=0°, 1=90°, 2=180°, 3=270°
void dwinFrameSetDir(uint8_t dir) {
  size_t i = 0;
  dwinByte(i, 0x34);
  dwinByte(i, 0x5A);
  dwinByte(i, 0xA5);
  dwinByte(i, dir);
  dwinSend(i);
}

// Update display
void dwinUpdateLCD() {
  if (need_lcd_update) {
    size_t i = 0;
    dwinByte(i, 0x3D);
    dwinSend(i);
    need_lcd_update = false;
  }
}

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Clear screen
//  color: Clear screen color
void dwinFrameClear(const uint16_t color) {
  size_t i = 0;
  dwinByte(i, 0x01);
  dwinWord(i, color);
  dwinSend(i);
}

#if DISABLED(TJC_DISPLAY)
  // Draw a point
  //  color: point color
  //  width: point width   0x01-0x0F
  //  height: point height 0x01-0x0F
  //  x,y: upper left point
  void dwinDrawPoint(uint16_t color, uint8_t width, uint8_t height, uint16_t x, uint16_t y) {
    size_t i = 0;
    dwinByte(i, 0x02);
    dwinWord(i, color);
    dwinByte(i, width);
    dwinByte(i, height);
    dwinWord(i, x);
    dwinWord(i, y);
    dwinSend(i);
  }
#endif

// Draw a line
//  color: Line segment color
//  xStart/yStart: Start point
//  xEnd/yEnd: End point
void dwinDrawLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  dwinByte(i, 0x03);
  dwinWord(i, color);
  dwinWord(i, xStart);
  dwinWord(i, yStart);
  dwinWord(i, xEnd);
  dwinWord(i, yEnd);
  dwinSend(i);
}

// Draw a rectangle
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xEnd/yEnd: lower right point
void dwinDrawRectangle(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  dwinByte(i, 0x05);
  dwinByte(i, mode);
  dwinWord(i, color);
  dwinWord(i, xStart);
  dwinWord(i, yStart);
  dwinWord(i, xEnd);
  dwinWord(i, yEnd);
  dwinSend(i);
}

// Move a screen area
//  mode: 0, circle shift; 1, translation
//  dir: 0=left, 1=right, 2=up, 3=down
//  dis: Distance
//  color: Fill color
//  xStart/yStart: upper left point
//  xEnd/yEnd: bottom right point
void dwinFrameAreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  dwinByte(i, 0x09);
  dwinByte(i, (mode << 7) | dir);
  dwinWord(i, dis);
  dwinWord(i, color);
  dwinWord(i, xStart);
  dwinWord(i, yStart);
  dwinWord(i, xEnd);
  dwinWord(i, yEnd);
  dwinSend(i);
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
void dwinDrawString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const char * const string, uint16_t rlimit/*=0xFFFF*/) {

  constexpr uint8_t widthAdjust = 0;
  size_t i = 0;
  dwinByte(i, 0x11);
  // Bit 7: widthAdjust
  // Bit 6: bShow
  // Bit 5-4: Unused (0)
  // Bit 3-0: size
  dwinByte(i, (widthAdjust * 0x80) | (bShow * 0x40) | size);
  dwinWord(i, color);
  dwinWord(i, bColor);
  dwinWord(i, x);
  dwinWord(i, y);
  dwinText(i, string, rlimit);
  dwinSend(i);
}

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw JPG and cached in #0 virtual display area
//  id: Picture ID
void dwinJPGShowAndCache(const uint8_t id) {
  size_t i = 0;
  dwinWord(i, 0x2200);
  dwinByte(i, id);
  dwinSend(i);     // AA 23 00 00 00 00 08 00 01 02 03 CC 33 C3 3C
}

// Draw an Icon
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void dwinIconShow(bool IBD, bool BIR, bool BFI, uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;
  dwinByte(i, 0x23);
  dwinWord(i, x);
  dwinWord(i, y);
  dwinByte(i, (IBD << 7) | (BIR << 6) | (BFI << 5) | libID);
  dwinByte(i, picID);
  dwinSend(i);
}

// Draw an Icon from SRAM
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  x/y: Upper-left point
//  addr: SRAM address
void dwinIconShow(bool IBD, bool BIR, bool BFI, uint16_t x, uint16_t y, uint16_t addr) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1);
  size_t i = 0;
  dwinByte(i, 0x24);
  dwinWord(i, x);
  dwinWord(i, y);
  dwinByte(i, (IBD << 7) | (BIR << 6) | (BFI << 5) | 0x00);
  dwinWord(i, addr);
  dwinSend(i);
}

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
void dwinJPGCacheToN(uint8_t n, uint8_t id) {
  size_t i = 0;
  dwinByte(i, 0x25);
  dwinByte(i, n);
  dwinByte(i, id);
  dwinSend(i);
}

// Animate a series of icons
//  animID: Animation ID; 0x00-0x0F
//  animate: true on; false off;
//  libID: Icon library ID
//  picIDs: Icon starting ID
//  picIDe: Icon ending ID
//  x/y: Upper-left point
//  interval: Display time interval, unit 10mS
void dwinIconAnimation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs, uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;
  dwinByte(i, 0x28);
  dwinWord(i, x);
  dwinWord(i, y);
  // Bit 7: animation on or off
  // Bit 6: start from begin or end
  // Bit 5-4: unused (0)
  // Bit 3-0: animID
  dwinByte(i, (animate * 0x80) | 0x40 | animID);
  dwinByte(i, libID);
  dwinByte(i, picIDs);
  dwinByte(i, picIDe);
  dwinByte(i, interval);
  dwinSend(i);
}

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void dwinIconAnimationControl(uint16_t state) {
  size_t i = 0;
  dwinByte(i, 0x29);
  dwinWord(i, state);
  dwinSend(i);
}

#endif // HAS_DWIN_E3V2 || IS_DWIN_MARLINUI
