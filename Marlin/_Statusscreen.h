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

//#define CONFIG_EXAMPLES_DIR "Anet/A8"

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
/**
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from the file 'DRR200_logo.gif'
 */
#pragma once

#define STATUS_LOGO_WIDTH 40
#define STATUS_LOGO_X      0

const unsigned char status_logo_bmp[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000, // ........................................
  B00000000,B00000000,B01111100,B00000000,B00000000, // .................#####..................
  B00000000,B00000001,B11111111,B00000000,B00000000, // ...............#########................
  B00111100,B00000110,B00101000,B11000000,B00000000, // ..####.......##...#.#...##..............
  B01001000,B00001000,B00101000,B00100000,B00000000, // .#..#.......#.....#.#.....#.............
  B01001000,B00010000,B00101000,B00010000,B00000000, // .#..#......#......#.#......#............
  B00111100,B00100000,B01010000,B00001000,B00000000, // ..####....#......#.#........#...........
  B00000000,B01000000,B01010000,B00000100,B00000000, // .........#.......#.#.........#..........
  B01000000,B01000000,B01010000,B00000100,B00000000, // .#.......#.......#.#.........#..........
  B01111100,B10000000,B10100110,B00000010,B00000000, // .#####..#.......#.#..##.......#.........
  B01000000,B10000000,B10101001,B00000010,B00000000, // .#......#.......#.#.#..#......#.........
  B00000100,B10000000,B10100001,B01001010,B00000000, // .....#..#.......#.#....#.#..#.#.........
  B00000100,B10000001,B01000010,B11010110,B00000000, // .....#..#......#.#....#.##.#.##.........
  B01111100,B10000001,B01000100,B01010110,B00000000, // .#####..#......#.#...#...#.#.##.........
  B00000000,B10000001,B01001111,B01001010,B00000000, // ........#......#.#..####.#..#.#.........
  B01000100,B10000000,B10000000,B00000010,B00000000, // .#...#..#.......#.............#.........
  B01010100,B10000001,B11000000,B00000010,B00000000, // .#.#.#..#......###............#.........
  B01111100,B10000111,B01111100,B00000010,B00000000, // .#####..#....###.#####........#.........
  B00000000,B01011011,B11100011,B10000100,B00000000, // .........#.##.#####...###....#..........
  B00111001,B01100010,B00111000,B01110101,B00000000, // ..###..#.##...#...###....###.#.#........
  B01000101,B10101100,B00000111,B00001111,B00000000, // .#...#.##.#.##.......###....####........
  B01000101,B10110000,B00000000,B11110011,B00000000, // .#...#.##.##............####..##........
  B01111100,B11001000,B00000000,B00101110,B00000000, // .#####..##..#.............#.###.........
  B00000000,B01000110,B00000000,B11000100,B00000000, // .........#...##.........##...#..........
  B00000000,B00000001,B11111111,B00000000,B00000000, // ...............#########................
  B00000000,B00000000,B00000000,B00000000,B00000000  // ........................................
};

#if !STATUS_CHAMBER_WIDTH && HAS_TEMP_CHAMBER && ((HOTENDS <= 4 && !HAS_HEATED_BED) || (HOTENDS <= 3 && HAS_HEATED_BED))
  #define STATUS_CHAMBER_WIDTH 21
  #if STATUS_HEATERS_WIDTH
    #if ENABLED(STATUS_COMBINE_HEATERS)
      #define STATUS_CHAMBER_X (LCD_PIXEL_WIDTH - 2 - (STATUS_CHAMBER_BYTEWIDTH) * 8)
    #elif HAS_FAN0 && HAS_HEATED_BED && HOTENDS <= 2
      #define STATUS_CHAMBER_X (LCD_PIXEL_WIDTH - 2 - (STATUS_HEATERS_BYTEWIDTH - STATUS_CHAMBER_BYTEWIDTH) * 8)
    #elif HAS_FAN0 && !HAS_HEATED_BED
      #define STATUS_CHAMBER_X (LCD_PIXEL_WIDTH - (STATUS_CHAMBER_BYTEWIDTH + STATUS_FAN_BYTEWIDTH) * 8)
    #else
      #define STATUS_CHAMBER_X (LCD_PIXEL_WIDTH - (STATUS_CHAMBER_BYTEWIDTH) * 8)
    #endif
  #endif

 #ifdef STATUS_CHAMBER_ANIM

    const unsigned char status_chamber_bmp[] PROGMEM = {
      B00011111,B11111111,B11111000,
      B00010000,B00111000,B00001000,
      B00010000,B01000100,B00001000,
      B00010000,B10100010,B00001000,
      B00010001,B00010101,B00001000,
      B00010001,B00111001,B00001000,
      B00010001,B01010001,B00001000,
      B00010000,B10001010,B00001000,
      B00010000,B01000100,B00001000,
      B00010000,B00111000,B00001000,
      B00011111,B11111111,B11111000,
      B00011111,B11111111,B11111000
    };
    const unsigned char status_chamber_on_bmp[] PROGMEM = {
      B00011111,B11111111,B11111000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00010000,B00000000,B00001000,
      B00011111,B11111111,B11111000,
      B00011111,B11111111,B11111000
    };


  #else

    const unsigned char status_chamber_bmp[] PROGMEM = {
      B00011111,B11111111,B11111000,
      B00010000,B00000000,B00001000,
      B00010000,B10000100,B00001000,
      B00010000,B01000010,B00001000,
      B00010000,B01000010,B00001000,
      B00010000,B10000100,B00001000,
      B00010001,B00001000,B00001000,
      B00010001,B00001000,B00001000,
      B00010000,B10000100,B00001000,
      B00010000,B00000000,B00001000,
      B00011111,B11111111,B11111000,
      B00011111,B11111111,B11111000
    };

  #endif
#endif

//
// Use default bitmaps
//


#define STATUS_HEATERS_X      37

