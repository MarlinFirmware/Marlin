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

#ifndef HEX_PRINT_ROUTINES_H
#define HEX_PRINT_ROUTINES_H

#include <stdint.h>

//
// Utility functions to create and print hex strings as nybble, byte, and word.
//

FORCE_INLINE char hex_nybble(const uint8_t n) {
  return (n & 0xF) + ((n & 0xF) < 10 ? '0' : 'A' - 10);
}
char* hex_byte(const uint8_t b);
char* hex_word(const uint16_t w);
char* hex_address(const void * const w);

void print_hex_nybble(const uint8_t n);
void print_hex_byte(const uint8_t b);
void print_hex_word(const uint16_t w);
void print_hex_address(const void * const w);

#ifdef CPU_32_BIT
  typedef uint32_t ptr_int_t;
#else
  typedef uint16_t ptr_int_t;
#endif

#endif // HEX_PRINT_ROUTINES_H
