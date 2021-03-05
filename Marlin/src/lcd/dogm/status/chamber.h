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
// lcd/dogm/status/chamber.h - Status Screen Chamber bitmaps
//

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
  const unsigned char status_chamber_on_bmp[] PROGMEM = {
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
