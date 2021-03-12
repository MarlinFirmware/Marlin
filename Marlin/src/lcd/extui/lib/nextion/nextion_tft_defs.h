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

/* ****************************************
 * lcd/extui/lib/nextion/nextion_tft_defs.h
 * ****************************************
 * Extensible_UI implementation for Nextion
 * https://github.com/Skorpi08
 * ***************************************/

#include "../../../../inc/MarlinConfigPre.h"
//#define NEXDEBUGLEVEL 255

#if NEXDEBUGLEVEL
  // Bit-masks for selective debug:
  enum NexDebugMask : uint8_t {
    N_INFO   =  1,
    N_ACTION =  2,
    N_FILE   =  4,
    N_PANEL  =  8,
    N_MARLIN = 16,
    N_SOME   = 32,
    N_ALL    = 64
  };
  #define NEXDEBUG(mask) ( ((mask) & NEXDEBUGLEVEL) == mask )  // Debug flag macro
#else
  #define NEXDEBUG(mask) false
#endif

#define MAX_FOLDER_DEPTH                4    // Limit folder depth TFT has a limit for the file path
#define MAX_CMND_LEN                   16 * MAX_FOLDER_DEPTH // Maximum Length for a Panel command
#define MAX_PATH_LEN                   16 * MAX_FOLDER_DEPTH // Maximum number of characters in a SD file path

 // TFT panel commands
#define  msg_welcome                MACHINE_NAME " Ready."

#define SEND_TEMP(x,y,t,z)  (nextion.SendtoTFT(PSTR(x)), nextion.SendtoTFT(PSTR(".txt=\"")), LCD_SERIAL.print(y), nextion.SendtoTFT(PSTR(t)), LCD_SERIAL.print(z), nextion.SendtoTFT(PSTR("\"\xFF\xFF\xFF")))
#define SEND_VAL(x,y)       (nextion.SendtoTFT(PSTR(x)), nextion.SendtoTFT(PSTR(".val=")),   LCD_SERIAL.print(y), nextion.SendtoTFT(PSTR("\xFF\xFF\xFF")))
#define SEND_TXT(x,y)       (nextion.SendtoTFT(PSTR(x)), nextion.SendtoTFT(PSTR(".txt=\"")), nextion.SendtoTFT(PSTR(y)),  nextion.SendtoTFT(PSTR("\"\xFF\xFF\xFF")))
#define SEND_TXT_P(x,y)     (nextion.SendtoTFT(PSTR(x)), nextion.SendtoTFT(PSTR(".txt=\"")), nextion.SendtoTFT(y), nextion.SendtoTFT(PSTR("\"\xFF\xFF\xFF")))
#define SEND_VALasTXT(x,y)  (nextion.SendtoTFT(PSTR(x)), nextion.SendtoTFT(PSTR(".txt=\"")), LCD_SERIAL.print(y), nextion.SendtoTFT(PSTR("\"\xFF\xFF\xFF")))
#define SEND_TXT_END(x)     (nextion.SendtoTFT(PSTR(x)), nextion.SendtoTFT(PSTR("\xFF\xFF\xFF")))
#define SEND_PCO2(x,y,z)    (nextion.SendtoTFT(PSTR(x)), LCD_SERIAL.print(y), nextion.SendtoTFT(PSTR(".pco=")), nextion.SendtoTFT(PSTR(z)), nextion.SendtoTFT(PSTR("\xFF\xFF\xFF")))
