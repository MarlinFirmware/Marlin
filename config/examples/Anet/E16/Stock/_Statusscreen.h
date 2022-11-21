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
#pragma once

#define CONFIG_EXAMPLES_DIR "Anet/E16/Stock"

/**
 * Custom Status Screen bitmap
 *
 * Place this file in the root with your configuration files
 * and enable CUSTOM_STATUS_SCREEN_IMAGE in Configuration.h.
 */

//
// Status Screen Logo bitmap
//
#define STATUS_LOGO_Y      3
#define STATUS_LOGO_WIDTH 24

/**
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from the file '3dpc_tech_statusscreen.bmp'
 */
const unsigned char status_logo_bmp[] PROGMEM = {
  B11111111,B11111111,B11111111,
  B11111111,B00000000,B11111111,
  B11111100,B00000000,B00111111,
  B11110000,B00000000,B00001111,
  B11100000,B00000000,B00000111,
  B11100000,B00000000,B00000111,
  B11000000,B00000000,B00000011,
  B10011101,B11001110,B00110001,
  B10010101,B10101111,B01101001,
  B10010101,B10101101,B01101001,
  B10000101,B10101101,B01100001,
  B10001101,B10101111,B01100001,
  B10010101,B10101100,B01100001,
  B10010101,B10101100,B01101001,
  B10010101,B10101100,B01101001,
  B10011101,B11001100,B00110001,
  B10000000,B00000000,B00000001,
  B11000000,B00000001,B11111011,
  B11100000,B00000000,B00000111,
  B11100000,B00000000,B00000111,
  B11110000,B00000000,B00001111,
  B11111100,B00000000,B00111111,
  B11111110,B01101100,B01111111,
  B11111111,B11111111,B11111111
};

//
// Use default bitmaps
//
#define STATUS_HOTEND_ANIM
#define STATUS_BED_ANIM
#if HOTENDS < 2
  #define STATUS_LOGO_X          8
  #define STATUS_HEATERS_X      40
  #define STATUS_BED_X          72
#else
  #define STATUS_LOGO_X          0
  #define STATUS_HEATERS_X      32
  #define STATUS_BED_X          80
#endif
