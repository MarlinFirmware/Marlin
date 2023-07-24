/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define TFT_EXCHANGE_XY _BV32(1)
#define TFT_INVERT_X    _BV32(2)
#define TFT_INVERT_Y    _BV32(3)

#define TFT_NO_ROTATION           (0x00)
#define TFT_ROTATE_90             (TFT_EXCHANGE_XY | TFT_INVERT_X)
#define TFT_ROTATE_180            (TFT_INVERT_X    | TFT_INVERT_Y)
#define TFT_ROTATE_270            (TFT_EXCHANGE_XY | TFT_INVERT_Y)

#define TFT_MIRROR_X              (TFT_INVERT_Y)
#define TFT_MIRROR_Y              (TFT_INVERT_X)

#define TFT_ROTATE_90_MIRROR_X    (TFT_ROTATE_90 ^ TFT_MIRROR_X)
#define TFT_ROTATE_90_MIRROR_Y    (TFT_ROTATE_90 ^ TFT_MIRROR_Y)

#define TFT_ROTATE_180_MIRROR_X   (TFT_ROTATE_180 ^ TFT_MIRROR_X)
#define TFT_ROTATE_180_MIRROR_Y   (TFT_ROTATE_180 ^ TFT_MIRROR_Y)

#define TFT_ROTATE_270_MIRROR_X   (TFT_ROTATE_270 ^ TFT_MIRROR_X)
#define TFT_ROTATE_270_MIRROR_Y   (TFT_ROTATE_270 ^ TFT_MIRROR_Y)

// TFT_ROTATION is user configurable
#ifndef TFT_ROTATION
  #define TFT_ROTATION TFT_NO_ROTATION
#endif

// TFT_ORIENTATION is the "sum" of TFT_DEFAULT_ORIENTATION plus user TFT_ROTATION
#define TFT_ORIENTATION ((TFT_DEFAULT_ORIENTATION) ^ (TFT_ROTATION))

// Set TFT_COLOR_UI_PORTRAIT flag, if needed
#if ((TFT_ORIENTATION) & TFT_EXCHANGE_XY) == 0
  #define TFT_COLOR_UI_PORTRAIT
#endif

#define TFT_COLOR_RGB   _BV32(3)
#define TFT_COLOR_BGR   _BV32(4)

#define TOUCH_ORIENTATION_NONE  0
#define TOUCH_LANDSCAPE         1
#define TOUCH_PORTRAIT          2
