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
 * lcd/extui/nextion/nextion_tft_defs.h
 * ****************************************
 * Extensible_UI implementation for Nextion
 * https://github.com/Skorpi08
 * ***************************************/

#include "../../../inc/MarlinConfigPre.h"

//#define NEXDEBUGLEVEL 255
#if NEXDEBUGLEVEL
  // Bit-masks for selective debug:
  enum NexDebugMask : uint8_t {
    N_INFO   = _BV(0),
    N_ACTION = _BV(1),
    N_FILE   = _BV(2),
    N_PANEL  = _BV(3),
    N_MARLIN = _BV(4),
    N_SOME   = _BV(5),
    N_ALL    = _BV(6)
  };
  #define NEXDEBUG(M) (((M) & NEXDEBUGLEVEL) == M)  // Debug flag macro
#else
  #define NEXDEBUG(M) false
#endif

#define MAX_FOLDER_DEPTH                4    // Limit folder depth TFT has a limit for the file path
#define MAX_CMND_LEN                   16 * MAX_FOLDER_DEPTH // Maximum Length for a Panel command
#define MAX_PATH_LEN                   16 * MAX_FOLDER_DEPTH // Maximum number of characters in a SD file path

 // TFT panel commands
#define  msg_welcome                MACHINE_NAME " Ready."

#define SEND_TEMP(x,y,t,z)  (nextion.tftSend(F(x)), nextion.tftSend(F(".txt=\"")), LCD_SERIAL.print(y), nextion.tftSend(F(t)), LCD_SERIAL.print(z), nextion.tftSend(F("\"\xFF\xFF\xFF")))
#define SEND_VAL(x,y)       (nextion.tftSend(F(x)), nextion.tftSend(F(".val=")),   LCD_SERIAL.print(y), nextion.tftSend(F("\xFF\xFF\xFF")))
#define SEND_TXT(x,y)       (nextion.tftSend(F(x)), nextion.tftSend(F(".txt=\"")), nextion.tftSend(F(y)), nextion.tftSend(F("\"\xFF\xFF\xFF")))
#define SEND_TXT_F(x,y)     (nextion.tftSend(F(x)), nextion.tftSend(F(".txt=\"")), nextion.tftSend(y), nextion.tftSend(F("\"\xFF\xFF\xFF")))
#define SEND_VALasTXT(x,y)  (nextion.tftSend(F(x)), nextion.tftSend(F(".txt=\"")), LCD_SERIAL.print(y), nextion.tftSend(F("\"\xFF\xFF\xFF")))
#define SEND_TXT_END(x)     (nextion.tftSend(F(x)), nextion.tftSend(F("\xFF\xFF\xFF")))
#define SEND_PCO2(x,y,z)    (nextion.tftSend(F(x)), LCD_SERIAL.print(y), nextion.tftSend(F(".pco=")), nextion.tftSend(F(z)), nextion.tftSend(F("\xFF\xFF\xFF")))
