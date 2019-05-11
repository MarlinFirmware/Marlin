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
#define STATUS_LOGO_Y      3
#define STATUS_LOGO_WIDTH 24
/**
 * Made with Marlin Bitmap Converter
 * http://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from the file '3dpc_tech_statusscreen.bmp'
 */
const unsigned char status_logo_bmp[] PROGMEM = {
  0xFF,0xFF,0xFF, // ########################
  0xFF,0x00,0xFF, // ########........########
  0xFC,0x00,0x3F, // ######............######
  0xF0,0x00,0x0F, // ####................####
  0xE0,0x00,0x07, // ###..................###
  0xE0,0x00,0x07, // ###..................###
  0xC0,0x00,0x03, // ##....................##
  0x9D,0xCE,0x31, // #..###.###..###...##...#
  0x95,0xAF,0x69, // #..#.#.##.#.####.##.#..#
  0x95,0xAD,0x69, // #..#.#.##.#.##.#.##.#..#
  0x85,0xAD,0x61, // #....#.##.#.##.#.##....#
  0x8D,0xAF,0x61, // #...##.##.#.####.##....#
  0x95,0xAC,0x61, // #..#.#.##.#.##...##....#
  0x95,0xAC,0x69, // #..#.#.##.#.##...##.#..#
  0x95,0xAC,0x69, // #..#.#.##.#.##...##.#..#
  0x9D,0xCC,0x31, // #..###.###..##....##...#
  0x80,0x00,0x01, // #......................#
  0xC0,0x01,0xFB, // ##.............######.##
  0xE0,0x00,0x07, // ###..................###
  0xE0,0x00,0x07, // ###..................###
  0xF0,0x00,0x0F, // ####................####
  0xFC,0x00,0x3F, // ######............######
  0xFE,0x6C,0x7F, // #######..##.##...#######
  0xFF,0xFF,0xFF  // ########################
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
