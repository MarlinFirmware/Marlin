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

typedef uint8_t fontid_t;

/**
 * 3-.0：The font size, 0x00-0x09, corresponds to the font size below:
 * 0x00=6*12   0x01=8*16   0x02=10*20  0x03=12*24  0x04=14*28
 * 0x05=16*32  0x06=20*40  0x07=24*48  0x08=28*56  0x09=32*64
 */
#if DISABLED(TJC_DISPLAY)
  #define font6x12  (fontid_t)0x00
  #define font20x40 (fontid_t)0x06
  #define font24x48 (fontid_t)0x07
  #define font28x56 (fontid_t)0x08
  #define font32x64 (fontid_t)0x09
#endif
#define font8x16  (fontid_t)0x01
#define font10x20 (fontid_t)0x02
#define font12x24 (fontid_t)0x03
#define font14x28 (fontid_t)0x04
#define font16x32 (fontid_t)0x05
