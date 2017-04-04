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
#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(M100_FREE_MEMORY_WATCHER)

#include "hex_print_routines.h"

static char _hex[5] = { 0 };

char* hex_byte(const uint8_t b) {
  _hex[0] = hex_nybble(b >> 4);
  _hex[1] = hex_nybble(b);
  _hex[2] = '\0';
  return _hex;
}

char* hex_word(const uint16_t w) {
  _hex[0] = hex_nybble(w >> 12);
  _hex[1] = hex_nybble(w >> 8);
  _hex[2] = hex_nybble(w >> 4);
  _hex[3] = hex_nybble(w);
  return _hex;
}

void print_hex_nybble(const uint8_t n) { SERIAL_CHAR(hex_nybble(n));  }
void print_hex_byte(const uint8_t b)   { SERIAL_ECHO(hex_byte(b)); }
void print_hex_word(const uint16_t w)  { SERIAL_ECHO(hex_word(w)); }

#endif // AUTO_BED_LEVELING_UBL || M100_FREE_MEMORY_WATCHER
