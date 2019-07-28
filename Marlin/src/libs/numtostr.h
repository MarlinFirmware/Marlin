/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfigPre.h"

// Convert a full-range unsigned 8bit int to a percentage
char* ui8tostr4pct(const uint8_t i);

// Convert uint8_t to string with 123 format
char* ui8tostr3(const uint8_t i);

// Convert int8_t to string with 123 format
char* i8tostr3(const int8_t x);

// Convert uint16_t to string with 12345 format
char* ui16tostr5(const uint16_t x);

// Convert uint16_t to string with 1234 format
char* ui16tostr4(const uint16_t x);

// Convert uint16_t to string with 123 format
char* ui16tostr3(const uint16_t x);

// Convert int16_t to string with 123 format
char* i16tostr3(const int16_t x);

// Convert unsigned int to lj string with 123 format
char* i16tostr3left(const int16_t xx);

// Convert signed int to rj string with _123, -123, _-12, or __-1 format
char* i16tostr4sign(const int16_t x);

// Convert unsigned float to string with 1.23 format
char* ftostr12ns(const float &x);

// Convert signed float to fixed-length string with 12.34 / -2.34 or 023.45 / -23.45 format
char* ftostr42_52(const float &x);

// Convert signed float to fixed-length string with 023.45 / -23.45 format
char* ftostr52(const float &x);

// Convert float to fixed-length string with +123.4 / -123.4 format
char* ftostr41sign(const float &x);

// Convert signed float to string (6 digit) with -1.234 / _0.000 / +1.234 format
char* ftostr43sign(const float &x, char plus=' ');

// Convert signed float to string (5 digit) with -1.2345 / _0.0000 / +1.2345 format
char* ftostr54sign(const float &x, char plus=' ');

// Convert unsigned float to rj string with 12345 format
char* ftostr5rj(const float &x);

// Convert signed float to string with +1234.5 format
char* ftostr51sign(const float &x);

// Convert signed float to space-padded string with -_23.4_ format
char* ftostr52sp(const float &x);

// Convert signed float to string with +123.45 format
char* ftostr52sign(const float &x);

// Convert unsigned float to string with 1234.5 format omitting trailing zeros
char* ftostr51rj(const float &x);

// Convert float to rj string with 123 or -12 format
FORCE_INLINE char* ftostr3(const float &x) { return i16tostr3(int16_t(x + (x < 0 ? -0.5f : 0.5f))); }

#if ENABLED(LCD_DECIMAL_SMALL_XY)
  // Convert float to rj string with 1234, _123, 12.3, _1.2, -123, _-12, or -1.2 format
  char* ftostr4sign(const float &fx);
#else
  // Convert float to rj string with 1234, _123, -123, __12, _-12, ___1, or __-1 format
  FORCE_INLINE char* ftostr4sign(const float &x) { return i16tostr4sign(int16_t(x + (x < 0 ? -0.5f : 0.5f))); }
#endif
