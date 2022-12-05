/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define STATUS_AMMETER_WIDTH 20

const unsigned char status_ammeter_bmp_mA[] PROGMEM = {
  B00000000,B11111100,B00000000,
  B00000011,B00000011,B00000000,
  B00000100,B00000000,B10000000,
  B00001000,B00000000,B01000000,
  B00010000,B00000110,B00100000,
  B00010000,B00001001,B00100000,
  B00100000,B00001001,B00010000,
  B00100011,B01001111,B00010000,
  B11100010,B10101001,B00011100,
  B00100010,B10101001,B00010000,
  B00100010,B10101001,B00010000,
  B00010000,B00000000,B00100000,
  B00010000,B00000000,B00100000,
  B00001000,B00000000,B01000000,
  B00000100,B00000000,B10000000,
  B00000011,B00000011,B00000000,
  B00000000,B11111100,B00000000
};

const unsigned char status_ammeter_bmp_A[] PROGMEM = {
  B00000000,B11111100,B00000000,
  B00000011,B00000011,B00000000,
  B00000100,B00000000,B10000000,
  B00001000,B00000000,B01000000,
  B00010000,B00000000,B00100000,
  B00010000,B00110000,B00100000,
  B00100000,B01001000,B00010000,
  B00100000,B01001000,B00010000,
  B11100000,B01111000,B00011100,
  B00100000,B01001000,B00010000,
  B00100000,B01001000,B00010000,
  B00010000,B01001000,B00100000,
  B00010000,B00000000,B00100000,
  B00001000,B00000000,B01000000,
  B00000100,B00000000,B10000000,
  B00000011,B00000011,B00000000,
  B00000000,B11111100,B00000000,
};
