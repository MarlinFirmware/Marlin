/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#if NEED_HEX_PRINT

#include "hex_print.h"
#include "../core/serial.h"

#ifdef CPU_32_BIT
  constexpr int byte_start = 4;
  static char _hex[] = "0x00000000";
#else
  constexpr int byte_start = 0;
  static char _hex[] = "0x0000";
#endif

char* hex_byte(const uint8_t b) {
  _hex[byte_start + 4] = hex_nybble(b >> 4);
  _hex[byte_start + 5] = hex_nybble(b);
  return &_hex[byte_start + 4];
}

inline void __hex_word(const uint16_t w) {
  _hex[byte_start + 2] = hex_nybble(w >> 12);
  _hex[byte_start + 3] = hex_nybble(w >> 8);
  _hex[byte_start + 4] = hex_nybble(w >> 4);
  _hex[byte_start + 5] = hex_nybble(w);
}

char* _hex_word(const uint16_t w) {
  __hex_word(w);
  return &_hex[byte_start + 2];
}

char* _hex_long(const uintptr_t l) {
  _hex[2] = hex_nybble(l >> 28);
  _hex[3] = hex_nybble(l >> 24);
  _hex[4] = hex_nybble(l >> 20);
  _hex[5] = hex_nybble(l >> 16);
  __hex_word((uint16_t)(l & 0xFFFF));
  return &_hex[2];
}

char* hex_address(const void * const w) {
  #ifdef CPU_32_BIT
    (void)hex_long((uintptr_t)w);
  #else
    (void)hex_word((uintptr_t)w);
  #endif
  return _hex;
}

void print_hex_nybble(const uint8_t n)       { SERIAL_CHAR(hex_nybble(n));  }
void print_hex_byte(const uint8_t b)         { SERIAL_ECHO(hex_byte(b));    }
void print_hex_word(const uint16_t w)        { SERIAL_ECHO(hex_word(w));    }
void print_hex_address(const void * const w) { SERIAL_ECHO(hex_address(w)); }

void print_hex_long(const uint32_t w, const char delimiter/*='\0'*/) {
  SERIAL_ECHOPGM("0x");
  for (int B = 24; B >= 8; B -= 8) {
    print_hex_byte(w >> B);
    if (delimiter) SERIAL_CHAR(delimiter);
  }
  print_hex_byte(w);
}

#endif // NEED_HEX_PRINT
