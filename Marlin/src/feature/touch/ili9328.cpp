/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_CALIBRATION)

#include "ili9328.h"
#include "fsmc.h"

#define LCD_Delay(ms) delay(ms)

void ili9328_Init() {
  /*
  LCD_IO_WriteReg(0x01); LCD_IO_WriteData(0x0000);  // Driver Output Control 1 // Vertical
  LCD_IO_WriteReg(0x01); LCD_IO_WriteData(0x0100);  // Driver Output Control 1 // Horizontal
  LCD_IO_WriteReg(0x60); LCD_IO_WriteData(0x2700);  // Driver Output Control 2 // Vertical
  LCD_IO_WriteReg(0x60); LCD_IO_WriteData(0xA700);  // Driver Output Control 2 // Horizontal
  LCD_IO_WriteReg(0x03); LCD_IO_WriteData(0x1038);  // Horizontal
  LCD_IO_WriteReg(0x60);
  LCD_IO_WriteData(0x2700);
  LCD_IO_WriteReg(0x03);
  LCD_IO_WriteData(0x1038);
  */

  //LCD_IO_WriteReg(0x02);
  //LCD_IO_WriteData(0x0700);
  //LCD_IO_WriteReg(0x02); LCD_IO_WriteData(0x0400);  // LCD Driving Control
  //LCD_IO_WriteReg(0xE5); LCD_IO_WriteData(0x78F0);  // unknown
  //LCD_IO_WriteReg(0x01); LCD_IO_WriteData(0x0000);  // Driver Output Control 1
  LCD_IO_WriteReg(0x01); LCD_IO_WriteData(0x0100);  // Driver Output Control 1 // Landscape
  // 0000 - bottom left (?)
  // 0080 - bottom left (junk)
  // 0100 - top left (junk)

  LCD_IO_WriteReg(0x02); LCD_IO_WriteData(0x0400);  // LCD Driving Control

  //LCD_IO_WriteReg(0x0003); LCD_IO_WriteData(0x1090);  // Entry Mode
  LCD_IO_WriteReg(0x03); LCD_IO_WriteData(0x1038);  // Entry Mode // Landscape

  LCD_IO_WriteReg(0x04); LCD_IO_WriteData(0x0000);  // Resize Control
  LCD_IO_WriteReg(0x08); LCD_IO_WriteData(0x0202);  // Display Control 2
  LCD_IO_WriteReg(0x09); LCD_IO_WriteData(0x0000);  // Display Control 3
  LCD_IO_WriteReg(0x0A); LCD_IO_WriteData(0x0000);  // Display Control 4
  LCD_IO_WriteReg(0x0C); LCD_IO_WriteData(0x0000);  // RGB Display Interface Control 1
  LCD_IO_WriteReg(0x0D); LCD_IO_WriteData(0x0000);  // Frame Maker Position
  LCD_IO_WriteReg(0x0F); LCD_IO_WriteData(0x0000);  // RGB Display Interface Control 2
  LCD_IO_WriteReg(0x10); LCD_IO_WriteData(0x0000);  // Power Control 1
  LCD_IO_WriteReg(0x11); LCD_IO_WriteData(0x0007);  // Power Control 2
  LCD_IO_WriteReg(0x12); LCD_IO_WriteData(0x0000);  // Power Control 3
  LCD_IO_WriteReg(0x13); LCD_IO_WriteData(0x0000);  // Power Control 4
  LCD_IO_WriteReg(0x07); LCD_IO_WriteData(0x0001);  // Display Control 1
  LCD_Delay(200);
  LCD_IO_WriteReg(0x10); LCD_IO_WriteData(0x1690);  // Power Control 1
  LCD_IO_WriteReg(0x11); LCD_IO_WriteData(0x0227);  // Power Control 2
  LCD_Delay(50);
  LCD_IO_WriteReg(0x12); LCD_IO_WriteData(0x008C);  // Power Control 3
  LCD_Delay(50);
  LCD_IO_WriteReg(0x13); LCD_IO_WriteData(0x1500);  // Power Control 4
  LCD_IO_WriteReg(0x29); LCD_IO_WriteData(0x0004);  // Power Control 7
  LCD_IO_WriteReg(0x2B); LCD_IO_WriteData(0x000D);  // Frame Rate and Color Control
  LCD_Delay(50);

  /*
  LCD_IO_WriteReg(0x30); LCD_IO_WriteData(0x0000);
  LCD_IO_WriteReg(0x31); LCD_IO_WriteData(0x0607);
  LCD_IO_WriteReg(0x32); LCD_IO_WriteData(0x0305);
  LCD_IO_WriteReg(0x35); LCD_IO_WriteData(0x0000);
  LCD_IO_WriteReg(0x36); LCD_IO_WriteData(0x1604);
  LCD_IO_WriteReg(0x37); LCD_IO_WriteData(0x0204);
  LCD_IO_WriteReg(0x38); LCD_IO_WriteData(0x0001);
  LCD_IO_WriteReg(0x39); LCD_IO_WriteData(0x0707);
  LCD_IO_WriteReg(0x3C); LCD_IO_WriteData(0x0000);
  LCD_IO_WriteReg(0x3D); LCD_IO_WriteData(0x000F);
  */

  LCD_IO_WriteReg(0x50); LCD_IO_WriteData(0x0000);
  LCD_IO_WriteReg(0x51); LCD_IO_WriteData(0x00EF);
  LCD_IO_WriteReg(0x52); LCD_IO_WriteData(0x0000);
  LCD_IO_WriteReg(0x53); LCD_IO_WriteData(0x013F);
  LCD_IO_WriteReg(0x20); LCD_IO_WriteData(0x0000);
  LCD_IO_WriteReg(0x21); LCD_IO_WriteData(0x0000);

  LCD_IO_WriteReg(0x60); LCD_IO_WriteData(0x2700);  // Driver Output Control 2 // Landscape

  LCD_IO_WriteReg(0x61); LCD_IO_WriteData(0x0001);  // Base Image Display Control
  LCD_IO_WriteReg(0x6A); LCD_IO_WriteData(0x0000);  // Vertical Scroll Control
  LCD_IO_WriteReg(0x80); LCD_IO_WriteData(0x0000);  // Partial Image 1 Display Position
  LCD_IO_WriteReg(0x81); LCD_IO_WriteData(0x0000);  // Partial Image 1 Area (Start Line)
  LCD_IO_WriteReg(0x82); LCD_IO_WriteData(0x0000);  // Partial Image 1 Area (End Line)
  LCD_IO_WriteReg(0x83); LCD_IO_WriteData(0x0000);  // Partial Image 2 Display Position
  LCD_IO_WriteReg(0x84); LCD_IO_WriteData(0x0000);  // Partial Image 2 Area (Start Line)
  LCD_IO_WriteReg(0x85); LCD_IO_WriteData(0x0000);  // Partial Image 2 Area (End Line)

  LCD_IO_WriteReg(0x90); LCD_IO_WriteData(0x0010);  // Panel Interface Control 1
  LCD_IO_WriteReg(0x92); LCD_IO_WriteData(0x0600);  // Panel Interface Control 2
  LCD_IO_WriteReg(0x07); LCD_IO_WriteData(0x0133);  // Display Control 1

  LCD_IO_WriteReg(ILI9328_GRAM);
}

void ili9328_DisplayOn() {
}

void ili9328_WriteRam() {
  LCD_IO_WriteReg(ILI9328_GRAM);
}

void ili9328_SetCursor(uint16_t Xpos, uint16_t Ypos) {
  LCD_IO_WriteReg(ILI9328_YSET);
  LCD_IO_WriteData(Ypos);
  LCD_IO_WriteReg(ILI9328_XSET);
  LCD_IO_WriteData(Xpos);
  LCD_IO_WriteReg(ILI9328_GRAM);
}

void ili9328_SetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax, uint16_t Ymax) {
  LCD_IO_WriteReg(ILI9328_RASET_MIN);
  LCD_IO_WriteData(Ymin);
  LCD_IO_WriteReg(ILI9328_RASET_MAX);
  LCD_IO_WriteData(Ymax);
  LCD_IO_WriteReg(ILI9328_CASET_MIN);
  LCD_IO_WriteData(Xmin);
  LCD_IO_WriteReg(ILI9328_CASET_MAX);
  LCD_IO_WriteData(XMax);

  ili9328_SetCursor(Xmin, Ymin);
}

#endif
