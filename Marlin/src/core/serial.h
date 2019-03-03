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
#pragma once

#include "../inc/MarlinConfigPre.h"
#include HAL_PATH(../HAL, HAL.h)

/**
 * Define debug bit-masks
 */
enum MarlinDebugFlags : uint8_t {
  MARLIN_DEBUG_NONE          = 0,
  MARLIN_DEBUG_ECHO          = _BV(0), ///< Echo commands in order as they are processed
  MARLIN_DEBUG_INFO          = _BV(1), ///< Print messages for code that has debug output
  MARLIN_DEBUG_ERRORS        = _BV(2), ///< Not implemented
  MARLIN_DEBUG_DRYRUN        = _BV(3), ///< Ignore temperature setting and E movement commands
  MARLIN_DEBUG_COMMUNICATION = _BV(4), ///< Not implemented
  MARLIN_DEBUG_LEVELING      = _BV(5), ///< Print detailed output for homing and leveling
  MARLIN_DEBUG_MESH_ADJUST   = _BV(6), ///< UBL bed leveling
  MARLIN_DEBUG_ALL           = 0xFF
};

extern uint8_t marlin_debug_flags;
#define DEBUGGING(F) (marlin_debug_flags & (MARLIN_DEBUG_## F))

#define SERIAL_BOTH 0x7F
#if NUM_SERIAL > 1
  extern int8_t serial_port_index;
  #define _PORT_REDIRECT(n,p)   REMEMBER(n,serial_port_index,p)
  #define _PORT_RESTORE(n)      RESTORE(n)
  #define SERIAL_OUT(WHAT, ...) do{ \
    if (!serial_port_index || serial_port_index == SERIAL_BOTH) MYSERIAL0.WHAT(__VA_ARGS__); \
    if ( serial_port_index) MYSERIAL1.WHAT(__VA_ARGS__); \
  }while(0)
#else
  #define _PORT_REDIRECT(n,p)   NOOP
  #define _PORT_RESTORE(n)      NOOP
  #define SERIAL_OUT(WHAT, ...) MYSERIAL0.WHAT(__VA_ARGS__)
#endif

#define PORT_REDIRECT(p)        _PORT_REDIRECT(1,p)
#define PORT_RESTORE()          _PORT_RESTORE(1)

#define SERIAL_CHAR(x)          SERIAL_OUT(write, x)
#define SERIAL_ECHO(x)          SERIAL_OUT(print, x)
#define SERIAL_ECHO_F(x,y)      SERIAL_OUT(print, x, y)
#define SERIAL_ECHOLN(x)        SERIAL_OUT(println, x)
#define SERIAL_PRINT(x,b)       SERIAL_OUT(print, x, b)
#define SERIAL_PRINTLN(x,b)     SERIAL_OUT(println, x, b)
#define SERIAL_PRINTF(args...)  SERIAL_OUT(printf, args)
#define SERIAL_FLUSH()          SERIAL_OUT(flush)

#if TX_BUFFER_SIZE > 0
  #define SERIAL_FLUSHTX()      SERIAL_OUT(flushTX)
#else
  #define SERIAL_FLUSHTX()
#endif

#define SERIAL_ECHOPGM(x)                   (serialprintPGM(PSTR(x)))
#define SERIAL_ECHOLNPGM(x)                 (serialprintPGM(PSTR(x "\n")))
#define SERIAL_ECHOPAIR(pre, value)         (serial_echopair_PGM(PSTR(pre), value))
#define SERIAL_ECHOLNPAIR(pre, value)       do{ SERIAL_ECHOPAIR(pre, value); SERIAL_EOL(); }while(0)

#define SERIAL_ECHOPAIR_F(pre, value, y)    do{ SERIAL_ECHO(pre); SERIAL_ECHO_F(value, y); }while(0)
#define SERIAL_ECHOLNPAIR_F(pre, value, y)  do{ SERIAL_ECHOPAIR_F(pre, value, y); SERIAL_EOL(); }while(0)

#define SERIAL_ECHO_START()                 serial_echo_start()
#define SERIAL_ERROR_START()                serial_error_start()
#define SERIAL_EOL()                        SERIAL_CHAR('\n')

#define SERIAL_ECHO_MSG(STR)                do{ SERIAL_ECHO_START(); SERIAL_ECHOLNPGM(STR); }while(0)
#define SERIAL_ERROR_MSG(STR)               do{ SERIAL_ERROR_START(); SERIAL_ECHOLNPGM(STR); }while(0)

#define SERIAL_ECHO_SP(C)                   serial_spaces(C)

//
// Functions for serial printing from PROGMEM. (Saves loads of SRAM.)
//
void serial_echopair_PGM(PGM_P const s_P, const char *v);
void serial_echopair_PGM(PGM_P const s_P, char v);
void serial_echopair_PGM(PGM_P const s_P, int v);
void serial_echopair_PGM(PGM_P const s_P, long v);
void serial_echopair_PGM(PGM_P const s_P, float v);
void serial_echopair_PGM(PGM_P const s_P, double v);
void serial_echopair_PGM(PGM_P const s_P, unsigned int v);
void serial_echopair_PGM(PGM_P const s_P, unsigned long v);
inline void serial_echopair_PGM(PGM_P const s_P, uint8_t v) { serial_echopair_PGM(s_P, (int)v); }
inline void serial_echopair_PGM(PGM_P const s_P, bool v)    { serial_echopair_PGM(s_P, (int)v); }
inline void serial_echopair_PGM(PGM_P const s_P, void *v)   { serial_echopair_PGM(s_P, (unsigned long)v); }

void serialprintPGM(PGM_P str);
void serial_echo_start();
void serial_error_start();
void serialprint_onoff(const bool onoff);
void serialprintln_onoff(const bool onoff);
void serial_spaces(uint8_t count);

void print_bin(const uint16_t val);

#if ENABLED(DEBUG_LEVELING_FEATURE)
  void print_xyz(PGM_P const prefix, PGM_P const suffix, const float x, const float y, const float z);
  void print_xyz(PGM_P const prefix, PGM_P const suffix, const float xyz[]);
  #define DEBUG_POS(SUFFIX,VAR) do { print_xyz(PSTR("  " STRINGIFY(VAR) "="), PSTR(" : " SUFFIX "\n"), VAR); } while(0)
#endif
