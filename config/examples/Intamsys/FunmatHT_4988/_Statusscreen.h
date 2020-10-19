/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#define CONFIG_EXAMPLES_DIR "Intamsys/FunmatHT_4988"

/**
 * Custom Status Screen bitmap
 *
 * Place this file in the root with your configuration files
 * and enable CUSTOM_STATUS_SCREEN_IMAGE in Configuration.h.
 *
 * Use the Marlin Bitmap Converter to make your own:
 * https://marlinfw.org/tools/u8glib/converter.html
 */

//
// Status Screen Logo bitmap
//
#define STATUS_LOGO_Y            1
#define STATUS_LOGO_WIDTH       32

const unsigned char status_logo_bmp[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00010000,B00000000,
  B00000000,B00000000,B00000110,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000001,B00000000,B00000000,B00000000,
  B00001000,B00000000,B00000001,B00000000,
  B00000001,B00000000,B01000001,B00000000,
  B00000001,B00000000,B00000010,B00000000,
  B00000000,B11100000,B00000000,B00000000,
  B10000000,B00001000,B00010000,B10000000,
  B00000100,B00000000,B00000000,B01000000,
  B00001000,B00000000,B00001000,B00000000,
  B00010000,B00000100,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00110000,
  B00000000,B00000000,B00000000,B00100000,
  B00000000,B00000001,B11101000,B00100000,
  B00000000,B00000000,B00000000,B01100000,
  B00000000,B00000000,B00000010,B00000000,
  B00000000,B00001000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B00100000,
  B00000010,B11101000,B00010000,B01100000,
  B00000000,B00000000,B00000000,B01000000,
  B00000000,B00000011,B11000000,B01100000,
  B01100100,B00000000,B00010000,B00000000,
  B00000100,B00000011,B11000000,B00000000,
  B00000010,B00000000,B00100000,B00000000,
  B00000000,B00101000,B00000000,B00000000,
  B00000000,B00001000,B00100000,B00000000,
  B00000000,B00000000,B01000000,B00000000,
  B00000000,B00000000,B00000000,B00000000
};
