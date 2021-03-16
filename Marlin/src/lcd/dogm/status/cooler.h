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

//
// lcd/dogm/status/cooler.h - Status Screen Laser Cooler bitmaps
//

#define STATUS_COOLER_WIDTH 16

#ifdef STATUS_COOLER_ANIM

  const unsigned char status_cooler_on_bmp[] PROGMEM = {
    B00010000,B00001000,
    B00010010,B01001001,
    B01010100,B00101010,
    B00111000,B00011100,
    B11111110,B11111111,
    B00111000,B00011100,
    B01010100,B00101010,
    B10010000,B10001001,
    B00010000,B10000000,
    B00000100,B10010000,
    B00000010,B10100000,
    B00000001,B11000000,
    B00011111,B11111100,
    B00000001,B11000000,
    B00000010,B10100000,
    B00000100,B10010000
  };

#endif

const unsigned char status_cooler_bmp[] PROGMEM = {
  B00010000,B00001000,
  B00010010,B01001001,
  B01010100,B00101010,
  B00101000,B00010100,
  B11000111,B01100011,
  B00101000,B00010100,
  B01010100,B00101010,
  B10010000,B10001001,
  B00010000,B10000000,
  B00000100,B10010000,
  B00000010,B10100000,
  B00000001,B01000000,
  B00011110,B00111100,
  B00000001,B01000000,
  B00000010,B10100000,
  B00000100,B10010000
};
