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

#if TX_BUFFER_SIZE < 1
  #define SERIAL_FLUSHTX_P()
  #define SERIAL_FLUSHTX()
#endif

#if NUM_SERIAL > 1

  extern int8_t serial_port_index;

  //
  // Serial out to all ports
  //
  #define SERIAL_CHAR(x)                    (MYSERIAL0.write(x), MYSERIAL1.write(x))
  #define SERIAL_ECHO(x)                    (MYSERIAL0.print(x), MYSERIAL1.print(x))
  #define SERIAL_ECHO_F(x,y)                (MYSERIAL0.print(x,y), MYSERIAL1.print(x,y))
  #define SERIAL_ECHOLN(x)                  (MYSERIAL0.println(x), MYSERIAL1.println(x))
  #define SERIAL_PRINT(x,b)                 (MYSERIAL0.print(x,b), MYSERIAL1.print(x,b))
  #define SERIAL_PRINTLN(x,b)               (MYSERIAL0.println(x,b), MYSERIAL1.println(x,b))
  #define SERIAL_PRINTF(args...)            (MYSERIAL0.printf(args), MYSERIAL1.printf(args))
  #define SERIAL_FLUSH()                    (MYSERIAL0.flush(), MYSERIAL1.flush())
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX()                (MYSERIAL0.flushTX(), MYSERIAL1.flushTX())
  #endif

  //
  // Serial out with port redirect
  //
  #define _PORT_REDIRECT(n,p)               REMEMBER(n,serial_port_index,p)
  #define _PORT_RESTORE(n)                  RESTORE(n)

  #define SERIAL_CHAR_P(x)                  (serial_port_index ? MYSERIAL1.write(x) : MYSERIAL0.write(x))
  #define SERIAL_ECHO_P(x)                  (serial_port_index ? MYSERIAL1.print(x) : MYSERIAL0.print(x))
  #define SERIAL_ECHO_F_P(x,y)              (serial_port_index ? MYSERIAL1.print(x,y) : MYSERIAL0.print(x,y))
  #define SERIAL_ECHOLN_P(x)                (serial_port_index ? MYSERIAL1.println(x) : MYSERIAL0.println(x))
  #define SERIAL_PRINT_P(x,b)               (serial_port_index ? MYSERIAL1.print(x,b) : MYSERIAL0.print(x,b))
  #define SERIAL_PRINTLN_P(x,b)             (serial_port_index ? MYSERIAL1.println(x,b) : MYSERIAL0.println(x,b))
  #define SERIAL_PRINTF_P(args...)          (serial_port_index ? MYSERIAL1.printf(args) : MYSERIAL0.printf(args))
  #define SERIAL_FLUSH_P()                  (serial_port_index ? MYSERIAL1.flush() : MYSERIAL0.flush())
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX_P()              (serial_port_index ? MYSERIAL1.flushTX() : MYSERIAL0.flushTX())
  #endif

  #define SERIAL_ECHOPGM_P(x)               (serialprintPGM_P(PSTR(x)))
  #define SERIAL_ECHOLNPGM_P(x)             (serialprintPGM_P(PSTR(x "\n")))
  #define SERIAL_ECHOPAIR_P(pre, value)     (serial_echopair_PGM_P(PSTR(pre),(value)))

  #define SERIAL_ECHO_START_P()             serial_echo_start_P()
  #define SERIAL_ERROR_START_P()            serial_error_start_P()
  #define SERIAL_EOL_P()                    SERIAL_CHAR_P('\n')

  #define SERIAL_ECHOPAIR_F_P(pre, value, y)   do{ SERIAL_ECHO_P(pre); SERIAL_ECHO_F_P(value, y); }while(0)
  #define SERIAL_ECHOLNPAIR_F_P(pre, value, y) do{ SERIAL_ECHOPAIR_F_P(pre, value, y); SERIAL_EOL_P(); }while(0)

  void serial_echopair_PGM_P(PGM_P const s_P, const char *v);
  void serial_echopair_PGM_P(PGM_P const s_P, char v);
  void serial_echopair_PGM_P(PGM_P const s_P, int v);
  void serial_echopair_PGM_P(PGM_P const s_P, long v);
  void serial_echopair_PGM_P(PGM_P const s_P, float v);
  void serial_echopair_PGM_P(PGM_P const s_P, double v);
  void serial_echopair_PGM_P(PGM_P const s_P, unsigned int v);
  void serial_echopair_PGM_P(PGM_P const s_P, unsigned long v);
  inline void serial_echopair_PGM_P(PGM_P const s_P, uint8_t v) { serial_echopair_PGM_P(s_P, (int)v); }
  inline void serial_echopair_PGM_P(PGM_P const s_P, bool v)    { serial_echopair_PGM_P(s_P, (int)v); }
  inline void serial_echopair_PGM_P(PGM_P const s_P, void *v)   { serial_echopair_PGM_P(s_P, (unsigned long)v); }

  void serial_spaces_P(uint8_t count);
  #define SERIAL_ECHO_SP_P(C)               serial_spaces_P(C)

  void serialprintPGM_P(PGM_P str);
  void serial_echo_start_P();
  void serial_error_start_P();

#else // NUM_SERIAL <= 1

  //
  // Serial out to all ports
  //
  #define SERIAL_CHAR(x)                    MYSERIAL0.write(x)
  #define SERIAL_ECHO(x)                    MYSERIAL0.print(x)
  #define SERIAL_ECHO_F(x,y)                MYSERIAL0.print(x,y)
  #define SERIAL_ECHOLN(x)                  MYSERIAL0.println(x)
  #define SERIAL_PRINT(x,b)                 MYSERIAL0.print(x,b)
  #define SERIAL_PRINTLN(x,b)               MYSERIAL0.println(x,b)
  #define SERIAL_PRINTF(args...)            MYSERIAL0.printf(args)
  #define SERIAL_FLUSH()                    MYSERIAL0.flush()
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX()                MYSERIAL0.flushTX()
  #endif

  //
  // Serial out with port redirect
  //
  #define _PORT_REDIRECT(n,p)               NOOP
  #define _PORT_RESTORE(n)                  NOOP

  #define SERIAL_CHAR_P(x)                  SERIAL_CHAR(x)
  #define SERIAL_ECHO_P(x)                  SERIAL_ECHO(x)
  #define SERIAL_ECHO_F_P(x,y)              SERIAL_ECHO_F(x,y)
  #define SERIAL_ECHOLN_P(x)                SERIAL_ECHOLN(x)
  #define SERIAL_PRINT_P(x,b)               SERIAL_PRINT(x,b)
  #define SERIAL_PRINTLN_P(x,b)             SERIAL_PRINTLN(x,b)
  #define SERIAL_PRINTF_P(args...)          SERIAL_PRINTF(args)
  #define SERIAL_FLUSH_P()                  SERIAL_FLUSH()
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX_P()              SERIAL_FLUSHTX()
  #endif

  #define SERIAL_ECHOPGM_P(x)               SERIAL_ECHOPGM(x)
  #define SERIAL_ECHOLNPGM_P(x)             SERIAL_ECHOLNPGM(x)
  #define SERIAL_ECHOPAIR_P(pre, value)     SERIAL_ECHOPAIR(pre, value)

  #define SERIAL_ECHO_P(x)                  SERIAL_ECHO(x)
  #define SERIAL_ECHOLN_P(x)                SERIAL_ECHOLN(x)

  #define SERIAL_ECHO_START_P()             SERIAL_ECHO_START()
  #define SERIAL_ERROR_START_P()            SERIAL_ERROR_START()
  #define SERIAL_EOL_P()                    SERIAL_EOL()

  #define SERIAL_ECHOPAIR_F_P(pre, value, y)   SERIAL_ECHOPAIR_F(pre, value, y)
  #define SERIAL_ECHOLNPAIR_F_P(pre, value, y) SERIAL_ECHOLNPAIR_F(pre, value, y)

  #define serial_echopair_PGM_P(s_P,v)      serial_echopair_PGM(s_P, v)

  #define serial_spaces_P(c)                serial_spaces(c)
  #define SERIAL_ECHO_SP_P(C)               SERIAL_ECHO_SP(C)

  #define serialprintPGM_P(s)               serialprintPGM(s)

#endif // NUM_SERIAL < 2

#define PORT_REDIRECT(p)                    _PORT_REDIRECT(1,p)
#define PORT_RESTORE()                      _PORT_RESTORE(1)

#define SERIAL_ECHOPGM(x)                   (serialprintPGM(PSTR(x)))
#define SERIAL_ECHOLNPGM(x)                 (serialprintPGM(PSTR(x "\n")))
#define SERIAL_ECHOPAIR(pre, value)         (serial_echopair_PGM(PSTR(pre), value))
#define SERIAL_ECHOLNPAIR(pre, value)       do { SERIAL_ECHOPAIR(pre, value); SERIAL_EOL(); } while(0)

#define SERIAL_ECHOPAIR_F(pre, value, y)    do{ SERIAL_ECHO(pre); SERIAL_ECHO_F(value, y); }while(0)
#define SERIAL_ECHOLNPAIR_F(pre, value, y)  do{ SERIAL_ECHOPAIR_F(pre, value, y); SERIAL_EOL(); }while(0)

#define SERIAL_ECHO_START()                 serial_echo_start()
#define SERIAL_ERROR_START()                serial_error_start()
#define SERIAL_EOL()                        SERIAL_CHAR('\n')

#define SERIAL_ECHO_MSG(STR)                do{ SERIAL_ECHO_START(); SERIAL_ECHOLNPGM(STR); }while(0)
#define SERIAL_ECHO_MSG_P(STR)              do{ SERIAL_ECHO_START_P(); SERIAL_ECHOLNPGM_P(STR); }while(0)
#define SERIAL_ERROR_MSG(STR)               do{ SERIAL_ERROR_START(); SERIAL_ECHOLNPGM(STR); }while(0)
#define SERIAL_ERROR_MSG_P(STR)             do{ SERIAL_ERROR_START_P(); SERIAL_ECHOLNPGM_P(STR); }while(0)

#define SERIAL_ECHOLNPAIR_P(pre, value)     do{ SERIAL_ECHOPAIR_P(pre, value); SERIAL_EOL_P(); }while(0)

void serial_spaces(uint8_t count);
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

#if ENABLED(DEBUG_LEVELING_FEATURE)
  void print_xyz(PGM_P const prefix, PGM_P const suffix, const float x, const float y, const float z);
  void print_xyz(PGM_P const prefix, PGM_P const suffix, const float xyz[]);
  #define DEBUG_POS(SUFFIX,VAR) do { print_xyz(PSTR("  " STRINGIFY(VAR) "="), PSTR(" : " SUFFIX "\n"), VAR); } while(0)
#endif
