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

uint8_t marlin_debug_flags = DEBUG_NONE;

const char errormagic[] PROGMEM = "Error:";
const char echomagic[] PROGMEM = "echo:";


void serialprintPGM(const char * str) {
  #ifdef TARGET_LPC1768
    MYSERIAL.print(str);
  #else
    while (char ch = pgm_read_byte(str++)) MYSERIAL.write(ch);
  #endif
}

void serial_echopair_P(const char* s_P, const char *v)   { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, char v)          { serialprintPGM(s_P); SERIAL_CHAR(v); }
void serial_echopair_P(const char* s_P, int v)           { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, long v)          { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, float v)         { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, double v)        { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_P(const char* s_P, unsigned long v) { serialprintPGM(s_P); SERIAL_ECHO(v); }

void serial_spaces(uint8_t count) { count *= (PROPORTIONAL_FONT_RATIO); while (count--) MYSERIAL.write(' '); }

#if ENABLED(DEBUG_LEVELING_FEATURE)

  void print_xyz(const char* prefix, const char* suffix, const float x, const float y, const float z) {
    serialprintPGM(prefix);
    SERIAL_CHAR('(');
    SERIAL_ECHO(x);
    SERIAL_ECHOPAIR(", ", y);
    SERIAL_ECHOPAIR(", ", z);
    SERIAL_CHAR(')');
    if (suffix) serialprintPGM(suffix); else SERIAL_EOL();
  }

  void print_xyz(const char* prefix, const char* suffix, const float xyz[]) {
    print_xyz(prefix, suffix, xyz[X_AXIS], xyz[Y_AXIS], xyz[Z_AXIS]);
  }

  #if HAS_ABL
    void print_xyz(const char* prefix, const char* suffix, const vector_3 &xyz) {
      print_xyz(prefix, suffix, xyz.x, xyz.y, xyz.z);
    }
  #endif

#endif
