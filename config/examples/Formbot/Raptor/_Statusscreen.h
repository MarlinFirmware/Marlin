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
#define STATUS_LOGO_Y            3
#define STATUS_LOGO_WIDTH       24

const unsigned char status_logo_bmp[] PROGMEM = {
  B11111111,B11111111,B11111111,
  B10000000,B00000000,B00000001,
  B10001110,B00000000,B11100001,
  B10011111,B00000001,B11110001,
  B10010011,B10000001,B00111001,
  B10011111,B10000001,B11111001,
  B10011111,B10000001,B11111001,
  B10011111,B10111001,B11111001,
  B10001111,B00101000,B11110001,
  B10000000,B00111000,B00000001,
  B10000000,B00000000,B00000001,
  B10011111,B11111111,B11111001,
  B10010001,B01110100,B10011001,
  B10011011,B00000110,B10101001,
  B10011011,B01010100,B10101001,
  B10011011,B01010110,B10101001,
  B10011011,B01010100,B10011001,
  B10011111,B11111111,B11111001,
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
