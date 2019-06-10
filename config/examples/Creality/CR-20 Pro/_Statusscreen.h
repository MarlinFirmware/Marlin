/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#define STATUS_LOGO_Y            5
#define STATUS_LOGO_WIDTH       40

const unsigned char status_logo_bmp[] PROGMEM = {
  0x01,0xEF,0x87,0x0E,0x00, // .......####.#####....###....###.........
  0x02,0x24,0x48,0x91,0x00, // ......#...#..#...#..#...#..#...#........
  0x04,0x04,0x48,0x91,0x00, // .....#.......#...#..#...#..#...#........
  0x04,0x07,0x80,0x91,0x00, // .....#.......####.......#..#...#........
  0x04,0x05,0x01,0x11,0x00, // .....#.......#.#.......#...#...#........
  0x04,0x04,0x82,0x11,0x00, // .....#.......#..#.....#....#...#........
  0x04,0x04,0x84,0x11,0x00, // .....#.......#..#....#.....#...#........
  0x02,0x24,0x48,0x11,0x00, // ......#...#..#...#..#......#...#........
  0x01,0xCE,0x6F,0x8E,0x00, // .......###..###..##.#####...###.........
  0x00,0x00,0x00,0x00,0x00, // ........................................
  0x00,0xF8,0x00,0x00,0x00, // ........#####...........................
  0x00,0x44,0x00,0x00,0x00, // .........#...#..........................
  0x00,0x44,0x00,0x00,0x00, // .........#...#..........................
  0x00,0x44,0xD8,0xE0,0x00, // .........#...#..##.##...###.............
  0x00,0x78,0x61,0x10,0x00, // .........####....##....#...#............
  0x00,0x40,0x41,0x10,0x00, // .........#.......#.....#...#............
  0x00,0x40,0x41,0x10,0x00, // .........#.......#.....#...#............
  0x00,0x40,0x41,0x10,0x00, // .........#.......#.....#...#............
  0x00,0xE0,0xF0,0xE0, 0x00, // ........###.....####....###.............
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
