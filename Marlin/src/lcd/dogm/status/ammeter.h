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
// lcd/dogm/status/ammeter.h - Status Screen Laser Ammeter bitmaps
//
#if ENABLED(I2C_AMMETER)

  #define STATUS_AMMETER_WIDTH 22

  const unsigned char status_ammeter_bmp1[] PROGMEM = {
  B00000000,B11110000,B00000000,
  B00000011,B00001100,B00000000,
  B00000100,B00000010,B00000000,
  B00001000,B00000001,B00000000,
  B00010000,B00000000,B10000000,
  B00010000,B00001100,B10000000,
  B00100000,B00010010,B01000000,
  B00100000,B00010010,B01000000,
  B11100110,B10011110,B01110000,
  B00100101,B01010010,B01000000,
  B00100101,B01010010,B01000000,
  B00100101,B01010010,B01000000,
  B00010000,B00000000,B10000000,
  B00010000,B00000000,B10000000,
  B00001000,B00000001,B00000000,
  B00000100,B00000010,B00000000,
  B00000011,B00001100,B00000000,
  B00000000,B11110000,B00000000
  };

  #endif