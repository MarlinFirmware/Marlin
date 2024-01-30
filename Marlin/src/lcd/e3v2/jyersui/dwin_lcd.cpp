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

/********************************************************************************
 * @file     lcd/e3v2/jyersui/dwin_lcd.cpp
 * @brief    DWIN screen control functions
 ********************************************************************************/

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD_JYERSUI)

#include "dwin_lcd.h"

/*-------------------------------------- System variable function --------------------------------------*/

void dwinStartup() {}

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Draw the degree (°) symbol
// color: color
//  x/y: Upper-left coordinate of the first pixel
void dwinDrawDegreeSymbol(uint16_t color, uint16_t x, uint16_t y) {
  dwinDrawPoint(color, 1, 1, x + 1, y);
  dwinDrawPoint(color, 1, 1, x + 2, y);
  dwinDrawPoint(color, 1, 1, x, y + 1);
  dwinDrawPoint(color, 1, 1, x + 3, y + 1);
  dwinDrawPoint(color, 1, 1, x, y + 2);
  dwinDrawPoint(color, 1, 1, x + 3, y + 2);
  dwinDrawPoint(color, 1, 1, x + 1, y + 3);
  dwinDrawPoint(color, 1, 1, x + 2, y + 3);
}

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void dwinIconShow(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  dwinIconShow(true, false, false, libID, picID, x, y);
}

#endif // DWIN_CREALITY_LCD_JYERSUI
