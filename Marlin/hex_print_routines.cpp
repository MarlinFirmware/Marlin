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


#include "Marlin.h"
#if ENABLED(UNIFIED_BED_LEVELING_FEATURE) || ENABLED(M100_FREE_MEMORY_WATCHER)


//
// 3 support routines to print hex numbers.  We can print a nibble, byte and word
//

void prt_hex_nibble(unsigned int n) {
  if (n <= 9)
    SERIAL_ECHO(n);
  else
    SERIAL_ECHO((char)('A' + n - 10));
  delay(3);
}

void prt_hex_byte(unsigned int b) {
  prt_hex_nibble((b & 0xf0) >> 4);
  prt_hex_nibble(b & 0x0f);
}

void prt_hex_word(unsigned int w) {
  prt_hex_byte((w & 0xff00) >> 8);
  prt_hex_byte(w & 0x0ff);
}

#endif

