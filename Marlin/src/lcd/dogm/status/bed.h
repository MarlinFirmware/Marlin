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
// lcd/dogm/status/bed.h - Status Screen Bed bitmaps
//

#if ENABLED(STATUS_ALT_BED_BITMAP)

  #define STATUS_BED_ANIM
  #define STATUS_BED_WIDTH  24

  #define STATUS_BED_TEXT_X (STATUS_BED_X + 11)

  const unsigned char status_bed_bmp[] PROGMEM = {
    0b11111111,0b11111111,0b11000000,
    0b01000000,0b00000000,0b00100000,
    0b00100000,0b00000000,0b00010000,
    0b00010000,0b00000000,0b00001000,
    0b00001000,0b00000000,0b00000100,
    0b00000100,0b00000000,0b00000010,
    0b00000011,0b11111111,0b11111111
  };

  #if HAS_LEVELING
    const unsigned char status_bed_leveled_bmp[] PROGMEM = {
      0b11111111,0b11111111,0b11001110,
      0b01000000,0b00100000,0b00100100,
      0b00100000,0b00010000,0b00010000,
      0b00011111,0b11111111,0b11111000,
      0b00001000,0b00000100,0b00000100,
      0b00100100,0b00000010,0b00000010,
      0b01110011,0b11111111,0b11111111
    };
  #endif

  const unsigned char status_bed_on_bmp[] PROGMEM = {
    0b00000010,0b00100010,0b00000000,
    0b00000100,0b01000100,0b00000000,
    0b00000100,0b01000100,0b00000000,
    0b00000010,0b00100010,0b00000000,
    0b00000001,0b00010001,0b00000000,
    0b11111111,0b11111111,0b11000000,
    0b01000000,0b10001000,0b10100000,
    0b00100001,0b00010001,0b00010000,
    0b00010010,0b00100010,0b00001000,
    0b00001000,0b00000000,0b00000100,
    0b00000100,0b00000000,0b00000010,
    0b00000011,0b11111111,0b11111111
  };

  #if HAS_LEVELING
    const unsigned char status_bed_leveled_on_bmp[] PROGMEM = {
      0b00000010,0b00100010,0b00000000,
      0b00000100,0b01000100,0b00000000,
      0b00000100,0b01000100,0b00000000,
      0b00000010,0b00100010,0b00000000,
      0b00000001,0b00010001,0b00000000,
      0b11111111,0b11111111,0b11001110,
      0b01000000,0b10101000,0b10100100,
      0b00100001,0b00010001,0b00010000,
      0b00011111,0b11111111,0b11111000,
      0b00001000,0b00000100,0b00000100,
      0b00100100,0b00000010,0b00000010,
      0b01110011,0b11111111,0b11111111
    };
  #endif

#else

  #define STATUS_BED_WIDTH  21

  #ifdef STATUS_BED_ANIM

    const unsigned char status_bed_bmp[] PROGMEM = {
      0b00011111,0b11111111,0b11111000,
      0b00011111,0b11111111,0b11111000
    };

    const unsigned char status_bed_on_bmp[] PROGMEM = {
      0b00000100,0b00010000,0b01000000,
      0b00000010,0b00001000,0b00100000,
      0b00000010,0b00001000,0b00100000,
      0b00000100,0b00010000,0b01000000,
      0b00001000,0b00100000,0b10000000,
      0b00010000,0b01000001,0b00000000,
      0b00010000,0b01000001,0b00000000,
      0b00001000,0b00100000,0b10000000,
      0b00000100,0b00010000,0b01000000,
      0b00000000,0b00000000,0b00000000,
      0b00011111,0b11111111,0b11111000,
      0b00011111,0b11111111,0b11111000
    };

  #else

    const unsigned char status_bed_bmp[] PROGMEM = {
      0b00000100,0b00010000,0b01000000,
      0b00000010,0b00001000,0b00100000,
      0b00000010,0b00001000,0b00100000,
      0b00000100,0b00010000,0b01000000,
      0b00001000,0b00100000,0b10000000,
      0b00010000,0b01000001,0b00000000,
      0b00010000,0b01000001,0b00000000,
      0b00001000,0b00100000,0b10000000,
      0b00000100,0b00010000,0b01000000,
      0b00000000,0b00000000,0b00000000,
      0b00011111,0b11111111,0b11111000,
      0b00011111,0b11111111,0b11111000
    };

  #endif

#endif

#ifndef STATUS_BED_X
  #define STATUS_BED_X (LCD_PIXEL_WIDTH - (STATUS_BED_BYTEWIDTH + STATUS_CHAMBER_BYTEWIDTH + STATUS_FAN_BYTEWIDTH) * 8 - TERN0(STATUS_HEAT_PERCENT, 4))
#endif
