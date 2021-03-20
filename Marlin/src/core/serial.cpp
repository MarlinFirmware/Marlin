/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "serial.h"
#include "language.h"
#include "enum.h"

uint8_t marlin_debug_flags = MARLIN_DEBUG_NONE;

static const char errormagic[] PROGMEM = "Error:";
static const char echomagic[]  PROGMEM = "echo:";

#if NUM_SERIAL > 1
  int8_t serial_port_index = SERIAL_PORT;
#endif

void serialprintPGM(PGM_P str) {
  while (const char c = pgm_read_byte(str++)) SERIAL_CHAR(c);
}
void serial_echo_start()  { serialprintPGM(echomagic); }
void serial_error_start() { serialprintPGM(errormagic); }

void serial_echopair_PGM(PGM_P const s_P, const char *v)   { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, char v)          { serialprintPGM(s_P); SERIAL_CHAR(v); }
void serial_echopair_PGM(PGM_P const s_P, int v)           { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, long v)          { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, float v)         { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, double v)        { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, unsigned int v)  { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, unsigned long v) { serialprintPGM(s_P); SERIAL_ECHO(v); }

void serial_spaces(uint8_t count) { count *= (PROPORTIONAL_FONT_RATIO); while (count--) SERIAL_CHAR(' '); }

void serial_ternary(const bool onoff, PGM_P const pre, PGM_P const on, PGM_P const off, PGM_P const post/*=nullptr*/) {
  if (pre) serialprintPGM(pre);
  serialprintPGM(onoff ? on : off);
  if (post) serialprintPGM(post);
}
void serialprint_onoff(const bool onoff) { serialprintPGM(onoff ? PSTR(MSG_ON) : PSTR(MSG_OFF)); }
void serialprintln_onoff(const bool onoff) { serialprint_onoff(onoff); SERIAL_EOL(); }
void serialprint_truefalse(const bool tf) { serialprintPGM(tf ? PSTR("true") : PSTR("false")); }

void print_bin(const uint16_t val) {
  uint16_t mask = 0x8000;
  for (uint8_t i = 16; i--;) {
    if (i && !(i % 4)) SERIAL_CHAR(' ');
    SERIAL_CHAR((val & mask) ? '1' : '0');
    mask >>= 1;
  }
}

void print_xyz(PGM_P const prefix, PGM_P const suffix, const float x, const float y, const float z) {
  serialprintPGM(prefix);
  SERIAL_CHAR('(');
  SERIAL_ECHO(x);
  SERIAL_ECHOPAIR(", ", y, ", ", z);
  SERIAL_CHAR(')');
  if (suffix) serialprintPGM(suffix); else SERIAL_EOL();
}

void print_xyz(PGM_P const prefix, PGM_P const suffix, const float xyz[]) {
  print_xyz(prefix, suffix, xyz[X_AXIS], xyz[Y_AXIS], xyz[Z_AXIS]);
}
