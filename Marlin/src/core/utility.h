/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "../inc/MarlinConfigPre.h"

constexpr char axis_codes[XYZE] = { 'X', 'Y', 'Z', 'E' };

void safe_delay(millis_t ms);

#if ENABLED(EEPROM_SETTINGS)
  void crc16(uint16_t *crc, const void * const data, uint16_t cnt);
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(G26_MESH_VALIDATION)
  /**
   * These support functions allow the use of large bit arrays of flags that take very
   * little RAM. Currently they are limited to being 16x16 in size. Changing the declaration
   * to unsigned long will allow us to go to 32x32 if higher resolution Mesh's are needed
   * in the future.
   */
  FORCE_INLINE void bitmap_clear(uint16_t bits[16], const uint8_t x, const uint8_t y)  { CBI(bits[y], x); }
  FORCE_INLINE void bitmap_set(uint16_t bits[16], const uint8_t x, const uint8_t y)    { SBI(bits[y], x); }
  FORCE_INLINE bool is_bitmap_set(uint16_t bits[16], const uint8_t x, const uint8_t y) { return TEST(bits[y], x); }
#endif

#if ENABLED(ULTRA_LCD) || ENABLED(DEBUG_LEVELING_FEATURE)

  // Convert uint8_t to string with 123 format
  char* i8tostr3(const uint8_t x);

  // Convert signed int to rj string with 123 or -12 format
  char* itostr3(const int x);

  // Convert unsigned int to lj string with 123 format
  char* itostr3left(const int xx);

  // Convert signed int to rj string with _123, -123, _-12, or __-1 format
  char *itostr4sign(const int x);

  // Convert unsigned float to string with 1.23 format
  char* ftostr12ns(const float &x);

  // Convert signed float to fixed-length string with 023.45 / -23.45 format
  char* ftostr32(const float &x);

  // Convert float to fixed-length string with +123.4 / -123.4 format
  char* ftostr41sign(const float &x);

  // Convert signed float to string (6 digit) with -1.234 / _0.000 / +1.234 format
  char* ftostr43sign(const float &x, char plus=' ');

  // Convert unsigned float to rj string with 12345 format
  char* ftostr5rj(const float &x);

  // Convert signed float to string with +1234.5 format
  char* ftostr51sign(const float &x);

  // Convert signed float to space-padded string with -_23.4_ format
  char* ftostr52sp(const float &x);

  // Convert signed float to string with +123.45 format
  char* ftostr52sign(const float &x);

  // Convert unsigned float to string with 1234.56 format omitting trailing zeros
  char* ftostr62rj(const float &x);

  // Convert float to rj string with 123 or -12 format
  FORCE_INLINE char *ftostr3(const float &x) { return itostr3((int)x); }

  #if ENABLED(LCD_DECIMAL_SMALL_XY)
    // Convert float to rj string with 1234, _123, 12.3, _1.2, -123, _-12, or -1.2 format
    char *ftostr4sign(const float &fx);
  #else
    // Convert float to rj string with 1234, _123, -123, __12, _-12, ___1, or __-1 format
    FORCE_INLINE char *ftostr4sign(const float &x) { return itostr4sign((int)x); }
  #endif

#endif // ULTRA_LCD

#if ENABLED(DEBUG_LEVELING_FEATURE)
  void log_machine_info();
#endif

#endif // __UTILITY_H__
