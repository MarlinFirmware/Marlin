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

#include "serial.h"
#include "../inc/MarlinConfig.h"

#if HAS_ETHERNET
  #include "../feature/ethernet.h"
#endif

uint8_t marlin_debug_flags = MARLIN_DEBUG_NONE;

// Commonly-used strings in serial output
PGMSTR(NUL_STR,   "");   PGMSTR(SP_P_STR, " P");  PGMSTR(SP_T_STR, " T");
PGMSTR(X_STR,     "X");  PGMSTR(Y_STR,     "Y");  PGMSTR(Z_STR,     "Z");  PGMSTR(E_STR,     "E");
PGMSTR(X_LBL,     "X:"); PGMSTR(Y_LBL,     "Y:"); PGMSTR(Z_LBL,     "Z:"); PGMSTR(E_LBL,     "E:");
PGMSTR(SP_A_STR, " A");  PGMSTR(SP_B_STR, " B");  PGMSTR(SP_C_STR, " C");
PGMSTR(SP_X_STR, " X");  PGMSTR(SP_Y_STR, " Y");  PGMSTR(SP_Z_STR, " Z");  PGMSTR(SP_E_STR, " E");
PGMSTR(SP_X_LBL, " X:"); PGMSTR(SP_Y_LBL, " Y:"); PGMSTR(SP_Z_LBL, " Z:"); PGMSTR(SP_E_LBL, " E:");

#if HAS_MULTI_SERIAL
  #ifdef SERIAL_CATCHALL
    SerialOutputT multiSerial(MYSERIAL, SERIAL_CATCHALL);
  #else
    #if HAS_ETHERNET
      // Runtime checking of the condition variable
      ConditionalSerial<decltype(MYSERIAL1)> serialOut1(ethernet.have_telnet_client, MYSERIAL1, false); // Takes reference here
    #else
      // Don't pay for runtime checking a true variable, instead use the output directly
      #define serialOut1 MYSERIAL1
    #endif
    SerialOutputT multiSerial(MYSERIAL0, serialOut1);
  #endif
#endif

void serialprintPGM(PGM_P str) {
  while (const char c = pgm_read_byte(str++)) SERIAL_CHAR(c);
}

void serial_echo_start()  { static PGMSTR(echomagic, "echo:"); serialprintPGM(echomagic); }
void serial_error_start() { static PGMSTR(errormagic, "Error:"); serialprintPGM(errormagic); }

void serial_echopair_PGM(PGM_P const s_P, const char *v)   { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, char v)          { serialprintPGM(s_P); SERIAL_CHAR(v); }
void serial_echopair_PGM(PGM_P const s_P, int v)           { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, long v)          { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, float v)         { serialprintPGM(s_P); SERIAL_DECIMAL(v); }
void serial_echopair_PGM(PGM_P const s_P, double v)        { serialprintPGM(s_P); SERIAL_DECIMAL(v); }
void serial_echopair_PGM(PGM_P const s_P, unsigned int v)  { serialprintPGM(s_P); SERIAL_ECHO(v); }
void serial_echopair_PGM(PGM_P const s_P, unsigned long v) { serialprintPGM(s_P); SERIAL_ECHO(v); }

void serial_spaces(uint8_t count) { count *= (PROPORTIONAL_FONT_RATIO); while (count--) SERIAL_CHAR(' '); }

void serial_ternary(const bool onoff, PGM_P const pre, PGM_P const on, PGM_P const off, PGM_P const post/*=nullptr*/) {
  if (pre) serialprintPGM(pre);
  serialprintPGM(onoff ? on : off);
  if (post) serialprintPGM(post);
}
void serialprint_onoff(const bool onoff) { serialprintPGM(onoff ? PSTR(STR_ON) : PSTR(STR_OFF)); }
void serialprintln_onoff(const bool onoff) { serialprint_onoff(onoff); SERIAL_EOL(); }
void serialprint_truefalse(const bool tf) { serialprintPGM(tf ? PSTR("true") : PSTR("false")); }

void print_bin(uint16_t val) {
  for (uint8_t i = 16; i--;) {
    SERIAL_CHAR('0' + TEST(val, i));
    if (!(i & 0x3) && i) SERIAL_CHAR(' ');
  }
}

void print_xyz(const float &x, const float &y, const float &z, PGM_P const prefix/*=nullptr*/, PGM_P const suffix/*=nullptr*/) {
  if (prefix) serialprintPGM(prefix);
  SERIAL_ECHOPAIR_P(SP_X_STR, x, SP_Y_STR, y, SP_Z_STR, z);
  if (suffix) serialprintPGM(suffix); else SERIAL_EOL();
}
