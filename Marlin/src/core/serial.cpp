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

#include "serial.h"

uint8_t marlin_debug_flags = MARLIN_DEBUG_NONE;

const char errormagic[] PROGMEM = "Error:";
const char echomagic[]  PROGMEM = "echo:";

#if NUM_SERIAL > 1
  void serialprintPGM_P(const int8_t p, const char * str) {
    while (char ch = pgm_read_byte(str++)) SERIAL_CHAR_P(p, ch);
  }

  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, const char *v)   { serialprintPGM_P(p, s_P); SERIAL_ECHO_P(p, v); }
  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, char v)          { serialprintPGM_P(p, s_P); SERIAL_CHAR_P(p, v); }
  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, int v)           { serialprintPGM_P(p, s_P); SERIAL_ECHO_P(p, v); }
  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, long v)          { serialprintPGM_P(p, s_P); SERIAL_ECHO_P(p, v); }
  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, float v)         { serialprintPGM_P(p, s_P); SERIAL_ECHO_P(p, v); }
  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, double v)        { serialprintPGM_P(p, s_P); SERIAL_ECHO_P(p, v); }
  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, unsigned int v)  { serialprintPGM_P(p, s_P); SERIAL_ECHO_P(p, v); }
  void serial_echopair_PGM_P(const int8_t p, PGM_P s_P, unsigned long v) { serialprintPGM_P(p, s_P); SERIAL_ECHO_P(p, v); }

  void serial_spaces_P(const int8_t p, uint8_t count) { count *= (PROPORTIONAL_FONT_RATIO); while (count--) SERIAL_CHAR_P(p, ' '); }

  void serial_echo_start_P(const int8_t p)  { serialprintPGM_P(p, echomagic); }
  void serial_error_start_P(const int8_t p) { serialprintPGM_P(p, errormagic); }

#endif

void serialprintPGM(PGM_P str) {
  while (char ch = pgm_read_byte(str++)) SERIAL_CHAR(ch);
}

void serial_echopair_PGM(PGM_P s_P, const char *v)   { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P s_P, char v)          { serialprintPGM(s_P); SERIAL_CHAR(v); }
void serial_echopair_PGM(PGM_P s_P, int v)           { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P s_P, long v)          { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P s_P, float v)         { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P s_P, double v)        { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P s_P, unsigned int v)  { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P s_P, unsigned long v) { serialprintPGM(s_P); SERIAL_ECHO(v); }

void serial_spaces(uint8_t count) { count *= (PROPORTIONAL_FONT_RATIO); while (count--) SERIAL_CHAR(' '); }

void serial_echo_start()  { serialprintPGM(echomagic); }
void serial_error_start() { serialprintPGM(errormagic); }

#if ENABLED(DEBUG_LEVELING_FEATURE)

  #include "enum.h"

  void print_xyz(PGM_P prefix, PGM_P suffix, const float x, const float y, const float z) {
    serialprintPGM(prefix);
    SERIAL_CHAR('(');
    SERIAL_ECHO(x);
    SERIAL_ECHOPAIR(", ", y);
    SERIAL_ECHOPAIR(", ", z);
    SERIAL_CHAR(')');
    if (suffix) serialprintPGM(suffix); else SERIAL_EOL();
  }

  void print_xyz(PGM_P prefix, PGM_P suffix, const float xyz[]) {
    print_xyz(prefix, suffix, xyz[X_AXIS], xyz[Y_AXIS], xyz[Z_AXIS]);
  }

#endif
