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

/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.8.1
 * Date: 2021/11/09
 *
 * Based on the original code provided by Creality under GPL
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "../../../inc/MarlinConfig.h"

#include "dwin_lcd.h"

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Display QR code
//  The size of the QR code is (46*QR_Pixel)*(46*QR_Pixel) dot matrix
//  QR_Pixel: The pixel size occupied by each point of the QR code: 0x01-0x0F (1-16)
//  (Nx, Ny): The coordinates of the upper left corner displayed by the QR code
//  str: multi-bit data
void DWIN_Draw_QR(uint8_t QR_Pixel, uint16_t x, uint16_t y, char *string) {
  size_t i = 0;
  DWIN_Byte(i, 0x21);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, QR_Pixel);
  DWIN_Text(i, string);
  DWIN_Send(i);
}

// Draw an Icon with transparent background
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  DWIN_ICON_Show(false, false, true, libID, picID, x, y);
}

// Copy area from current virtual display area to current screen
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
  size_t i = 0;
  DWIN_Byte(i, 0x26);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

// Copy area from virtual display area to current screen
//  IBD: background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(bool IBD, bool BIR, bool BFI, uint8_t cacheID, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
  size_t i = 0;
  DWIN_Byte(i, 0x27);
  DWIN_Byte(i, (IBD & 1) << 7 | (BIR & 1) << 6 | (BFI & 1) << 5 | cacheID);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

// Copy area from virtual display area to current screen with transparent background
//  cacheID: virtual area number
//  xStart/yStart: Upper-left of virtual area
//  xEnd/yEnd: Lower-right of virtual area
//  x/y: Screen paste point
void DWIN_Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
  DWIN_Frame_AreaCopy(false, false, true, cacheID, xStart, yStart, xEnd, yEnd, x, y);
}

// Write buffer data to the SRAM or Flash
//  mem: 0x5A=32KB SRAM, 0xA5=16KB Flash
//  addr: start address
//  length: Bytes to write
//  data: address of the buffer with data
void DWIN_WriteToMem(uint8_t mem, uint16_t addr, uint16_t length, uint8_t *data) {
  const uint8_t max_size = 128;
  uint16_t pending = length;
  uint16_t to_send;
  uint16_t indx;
  uint8_t block = 0;

  while (pending > 0) {
    indx = block * max_size;
    to_send = _MIN(pending, max_size);
    size_t i = 0;
    DWIN_Byte(i, 0x31);
    DWIN_Byte(i, mem);
    DWIN_Word(i, addr + indx); // start address of the data block
    ++i;
    LOOP_L_N(j, i) { LCD_SERIAL.write(DWIN_SendBuf[j]); delayMicroseconds(1); }  // Buf header
    for (uint16_t j = indx; j <= indx + to_send - 1; j++) LCD_SERIAL.write(*(data + j)); delayMicroseconds(1);  // write block of data
    LOOP_L_N(j, 4) { LCD_SERIAL.write(DWIN_BufTail[j]); delayMicroseconds(1); }
    block++;
    pending -= to_send;
  }
}

// Write the contents of the 32KB SRAM data memory into the designated image memory space.
//  picID: Picture memory space location, 0x00-0x0F, each space is 32Kbytes
void DWIN_SRAMToPic(uint8_t picID) {
  size_t i = 0;
  DWIN_Byte(i, 0x33);
  DWIN_Byte(i, 0x5A);
  DWIN_Byte(i, 0xA5);
  DWIN_Byte(i, picID);
  DWIN_Send(i);
}

//--------------------------Test area -------------------------

//void DWIN_ReadSRAM(uint16_t addr, uint8_t length, const char * const data) {
//  size_t i = 0;
//  DWIN_Byte(i, 0x32);
//  DWIN_Byte(i, 0x5A);  // 0x5A Read from SRAM - 0xA5 Read from Flash
//  DWIN_Word(i, addr);  // 0x0000 to 0x7FFF
//  const size_t len = _MIN(0xF0, length);
//  DWIN_Byte(i, len);
//  DWIN_Send(i);
//}

#endif // DWIN_LCD_PROUI
