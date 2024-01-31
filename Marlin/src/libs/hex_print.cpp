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

static char _hex[] = "00000000";

inline void __hex_byte(const uint8_t b, uint8_t o) {
  _hex[o + 0] = hex_nybble(b >> 4);
  _hex[o + 1] = hex_nybble(b);
}
inline void __hex_word(const uint16_t w, const uint8_t o=4) {
  __hex_byte(w >> 8, o + 0);
  __hex_byte(w >> 0, o + 2);
}
inline void __hex_long(const uint16_t w) {
  __hex_byte(w >> 8, 0);
  __hex_byte(w >> 0, 4);
}

char* hex_byte(const uint8_t b) {
  __hex_byte(b, 6);
  return &_hex[6];
}
char* _hex_word(const uint16_t w) {
  __hex_word(w, 4);
  return &_hex[4];
}
char* _hex_long(const uint32_t l) {
  __hex_long(l);
  return _hex;
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

void print_hex_long(const uint32_t w, const char delimiter/*='\0'*/, const bool prefix/*=false*/) {
  if (prefix) SERIAL_ECHOPGM("0x");
  for (int B = 24; B >= 8; B -= 8) {
    print_hex_byte(w >> B);
    if (delimiter) SERIAL_CHAR(delimiter);
  }
  print_hex_byte(w);
}

#endif // NEED_HEX_PRINT
