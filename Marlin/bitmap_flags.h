/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef _BITMAP_FLAGS_H_
#define _BITMAP_FLAGS_H_

#include "macros.h"

/**
 * These support functions allow the use of large bit arrays of flags that take very
 * little RAM. Currently they are limited to being 16x16 in size. Changing the declaration
 * to unsigned long will allow us to go to 32x32 if higher resolution meshes are needed
 * in the future.
 */
FORCE_INLINE void bitmap_clear(uint16_t bits[16], const uint8_t x, const uint8_t y) { CBI(bits[y], x); }
FORCE_INLINE void bitmap_set(uint16_t bits[16], const uint8_t x, const uint8_t y) { SBI(bits[y], x); }
FORCE_INLINE bool is_bitmap_set(uint16_t bits[16], const uint8_t x, const uint8_t y) { return TEST(bits[y], x); }

#endif // _BITMAP_FLAGS_H_
