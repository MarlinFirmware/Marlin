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
  0b00000000,0b11111100,0b00000000,
  0b00000011,0b00000011,0b00000000,
  0b00000100,0b00000000,0b10000000,
  0b00001000,0b00000000,0b01000000,
  0b00010000,0b00000110,0b00100000,
  0b00010000,0b00001001,0b00100000,
  0b00100000,0b00001001,0b00010000,
  0b00100011,0b01001111,0b00010000,
  0b11100010,0b10101001,0b00011100,
  0b00100010,0b10101001,0b00010000,
  0b00100010,0b10101001,0b00010000,
  0b00010000,0b00000000,0b00100000,
  0b00010000,0b00000000,0b00100000,
  0b00001000,0b00000000,0b01000000,
  0b00000100,0b00000000,0b10000000,
  0b00000011,0b00000011,0b00000000,
  0b00000000,0b11111100,0b00000000
};

const unsigned char status_ammeter_bmp_A[] PROGMEM = {
  0b00000000,0b11111100,0b00000000,
  0b00000011,0b00000011,0b00000000,
  0b00000100,0b00000000,0b10000000,
  0b00001000,0b00000000,0b01000000,
  0b00010000,0b00000000,0b00100000,
  0b00010000,0b00110000,0b00100000,
  0b00100000,0b01001000,0b00010000,
  0b00100000,0b01001000,0b00010000,
  0b11100000,0b01111000,0b00011100,
  0b00100000,0b01001000,0b00010000,
  0b00100000,0b01001000,0b00010000,
  0b00010000,0b01001000,0b00100000,
  0b00010000,0b00000000,0b00100000,
  0b00001000,0b00000000,0b01000000,
  0b00000100,0b00000000,0b10000000,
  0b00000011,0b00000011,0b00000000,
  0b00000000,0b11111100,0b00000000,
};
