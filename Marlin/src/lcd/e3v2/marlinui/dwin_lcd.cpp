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
 * @file     lcd/e3v2/marlinui/dwin_lcd.cpp
 * @brief    DWIN screen control functions
 ********************************************************************************/

#include "../../../inc/MarlinConfigPre.h"

#if IS_DWIN_MARLINUI

#include "../../../inc/MarlinConfig.h"

#include "dwin_lcd.h"
#include <string.h> // for memset

//#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

/*-------------------------------------- System variable function --------------------------------------*/

void dwinStartup() {
  DEBUG_ECHOPGM("\r\nDWIN handshake ");
  delay(750);   // Delay here or init later in the boot process
  const bool success = dwinHandshake();
  if (success) DEBUG_ECHOLNPGM("ok."); else DEBUG_ECHOLNPGM("error.");
  dwinFrameSetDir(TERN(DWIN_MARLINUI_LANDSCAPE, 0, 1));
  dwinFrameClear(COLOR_BG_BLACK); // MarlinUI handles the bootscreen so just clear here
  dwinJPGShowAndCache(3);
  dwinUpdateLCD();
}

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void dwinIconShow(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  dwinIconShow(true, false, false, libID, picID, x, y);
}

#endif // IS_DWIN_MARLINUI
