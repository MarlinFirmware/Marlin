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

#include "serial.h"
#include "language.h"

uint8_t marlin_debug_flags = MARLIN_DEBUG_NONE;

static const char errormagic[] PROGMEM = "Error:";
static const char echomagic[]  PROGMEM = "echo:";

#if NUM_SERIAL > 1
  int8_t serial_port_index = 0;
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

void print_bin(uint16_t val) {
  for (uint8_t i = 16; i--;) {
    SERIAL_CHAR('0' + TEST(val, i));
    if (!(i & 0x3) && i) SERIAL_CHAR(' ');
  }
}

extern const char SP_X_STR[], SP_Y_STR[], SP_Z_STR[];

void print_xyz(const float &x, const float &y, const float &z, PGM_P const prefix/*=nullptr*/, PGM_P const suffix/*=nullptr*/) {
  serialprintPGM(prefix);
  SERIAL_ECHOPAIR_P(SP_X_STR, x, SP_Y_STR, y, SP_Z_STR, z);
  if (suffix) serialprintPGM(suffix); else SERIAL_EOL();
}
