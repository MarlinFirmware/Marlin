/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define CONFIG_EXAMPLES_DIR "AnyCubic/Mega Zero 2.0/Anycubic V1"

//
// Status Screen Logo bitmap
//
#define STATUS_LOGO_WIDTH 48
#define STATUS_LOGO_X 8
#define STATUS_LOGO_Y 10
#define STATUS_HEATERS_X 54
#define STATUS_BED_X 74

const unsigned char status_logo_bmp[] PROGMEM = {
  B01110011,B11001111,B11110000,B11111100,B00000000,B00111110,
  B01110011,B11000000,B00110001,B11001110,B00000000,B01110111,
  B01110011,B11000000,B01110001,B10000110,B00000000,B11100011,
  B01110011,B11000000,B11100000,B00000110,B00000000,B11100011,
  B01111011,B11000000,B11000000,B00001110,B00000000,B11000011,
  B01111111,B11000001,B11000000,B00001110,B00000000,B11000011,
  B01111111,B11000001,B10000000,B00011100,B00000000,B11000011,
  B01111111,B11000011,B10000000,B00111000,B00000000,B11000011,
  B01111111,B11000011,B00000000,B01110000,B00000000,B11000011,
  B01101101,B11000110,B00000000,B01100000,B00000000,B11100011,
  B01101101,B11001110,B00000000,B11100000,B00000000,B01100111,
  B01101101,B11001111,B11111001,B11111110,B00011000,B01111110,
  B01101101,B11001111,B11111001,B11111110,B00011000,B00011100
};
