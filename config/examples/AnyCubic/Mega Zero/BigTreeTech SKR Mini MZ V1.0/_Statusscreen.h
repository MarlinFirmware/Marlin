/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from 128x19 C/C++ data
 */
#pragma once

#define CONFIG_EXAMPLES_DIR "AnyCubic/Mega Zero/BigTreeTech SKR Mini MZ V1.0"

//
// Status Screen Logo bitmap
//
#define STATUS_LOGO_WIDTH 64
#define STATUS_LOGO_X 8
#define STATUS_LOGO_Y 10
#define STATUS_HEATERS_X 80

const unsigned char status_logo_bmp[] PROGMEM = {
  B11101110,B00000000,B00000000,B00000000,B00011111,B00000000,B00000000,B00000000,
  B01101100,B00000000,B00000000,B00000000,B00100010,B00000000,B00000000,B00000000,
  B01101100,B00000000,B00000000,B00000000,B00000010,B00000000,B00000000,B00000000,
  B01101100,B00111100,B00111110,B00111000,B00000100,B00011110,B01110111,B00001100,
  B01101100,B01000010,B01000100,B01000100,B00000100,B00100001,B00011001,B00010010,
  B01010100,B01000010,B01000100,B00001100,B00001000,B00100001,B00010000,B00100001,
  B01010100,B01111110,B00111000,B00110100,B00001000,B00111111,B00010000,B00100001,
  B01010100,B01000000,B01000000,B01000100,B00010000,B00100000,B00010000,B00100001,
  B01010100,B01000010,B00111100,B01001100,B00010001,B00100001,B00010000,B00010010,
  B11010110,B00111100,B01000010,B00110110,B00111111,B00011110,B01111100,B00001100,
  B00000000,B00000000,B01000010,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00111100,B00000000,B00000000,B00000000,B00000000,B00000000
};
