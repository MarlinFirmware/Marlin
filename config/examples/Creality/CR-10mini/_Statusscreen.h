/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Custom Status Screen bitmap
 *
 * Place this file in the root with your configuration files
 * and enable CUSTOM_STATUS_SCREEN_IMAGE in Configuration.h.
 *
 * Use the Marlin Bitmap Converter to make your own:
 * http://marlinfw.org/tools/u8glib/converter.html
 */

//
// Status Screen Logo bitmap
//
#define STATUS_LOGO_Y            5
#define STATUS_LOGO_WIDTH       36

const unsigned char status_logo_bmp[] PROGMEM = {
  B00000111,B11001111,B10000000,B00110001,B11100000,
  B00001111,B11001111,B11000000,B01110011,B11110000,
  B00001100,B00001100,B01000000,B01110011,B00110000,
  B00001100,B00001100,B11000000,B00110011,B00110000,
  B00001100,B00001111,B11001111,B00110011,B00110000,
  B00001100,B00001101,B10001111,B00110011,B00110000,
  B00001100,B00001100,B11000000,B00110011,B00110000,
  B00001111,B11001100,B11000000,B00110011,B11110000,
  B00000111,B11001100,B11000000,B00110001,B11100000,
  B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000001,B10000011,B00110000,B00000011,B00000000,
  B00000001,B10000011,B00110010,B00000011,B00000000,
  B00000001,B11000111,B00000010,B11100000,B00000000,
  B00000001,B11000111,B00110011,B11110011,B00000000,
  B00000001,B11101111,B00110011,B00110011,B00000000,
  B00000001,B10111011,B00110011,B00110011,B00000000,
  B00000001,B10010011,B00110011,B00110011,B00000000,
  B00000001,B10010011,B00110011,B00110011,B00000000,
  B00000001,B10000011,B00110011,B00110011,B00000000
};

//
// Use default bitmaps
//
#define STATUS_HOTEND_ANIM
#define STATUS_BED_ANIM
#define STATUS_HEATERS_XSPACE   20
#if HOTENDS < 2
  #define STATUS_HEATERS_X      48
  #define STATUS_BED_X          72
#else
  #define STATUS_HEATERS_X      40
  #define STATUS_BED_X          80
#endif
