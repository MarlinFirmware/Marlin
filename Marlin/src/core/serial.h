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
  #define SERIAL_FLUSHTX_P(p)
  #define SERIAL_FLUSHTX()
#endif

#if NUM_SERIAL > 1

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
  #define SERIAL_CHAR_P(p,x)                (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.write(x) : MYSERIAL1.write(x)) : SERIAL_CHAR(x))
  #define SERIAL_ECHO_P(p,x)                (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.print(x) : MYSERIAL1.print(x)) : SERIAL_ECHO(x))
  #define SERIAL_ECHO_F_P(p,x,y)            (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.print(x,y) : MYSERIAL1.print(x,y)) : SERIAL_ECHO_F(x,y))
  #define SERIAL_ECHOLN_P(p,x)              (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.println(x) : MYSERIAL1.println(x)) : SERIAL_ECHOLN(x))
  #define SERIAL_PRINT_P(p,x,b)             (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.print(x,b) : MYSERIAL1.print(x,b)) : SERIAL_PRINT(x,b))
  #define SERIAL_PRINTLN_P(p,x,b)           (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.println(x,b) : MYSERIAL1.println(x,b)) : SERIAL_PRINTLN(x,b))
  #define SERIAL_PRINTF_P(p,args...)        (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.printf(args) : MYSERIAL1.printf(args)) : SERIAL_PRINTF(args))
  #define SERIAL_FLUSH_P(p)                 (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.flush() : MYSERIAL1.flush()) : SERIAL_FLUSH())
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX_P(p)             (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.flushTX() : MYSERIAL1.flushTX()) : SERIAL_FLUSHTX())
  #endif

  #define SERIAL_ECHOPGM_P(p,x)             (serialprintPGM_P(p,PSTR(x)))
  #define SERIAL_ECHOLNPGM_P(p,x)           (serialprintPGM_P(p,PSTR(x "\n")))
  #define SERIAL_ECHOPAIR_P(p, pre, value)  (serial_echopair_PGM_P(p,PSTR(pre),(value)))

  #define SERIAL_ECHO_START_P(p)            serial_echo_start_P(p)
  #define SERIAL_ERROR_START_P(p)           serial_error_start_P(p)
  #define SERIAL_EOL_P(p)                   SERIAL_CHAR_P(p,'\n')

  #define SERIAL_ECHOPAIR_F_P(p, pre, value, y)   do{ SERIAL_ECHO_P(p, pre); SERIAL_ECHO_F_P(p, value, y); }while(0)
  #define SERIAL_ECHOLNPAIR_F_P(p, pre, value, y) do{ SERIAL_ECHOPAIR_F_P(p, pre, value, y); SERIAL_EOL_P(p); }while(0)

  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, const char *v);
  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, char v);
  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, int v);
  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, long v);
  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, float v);
  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, double v);
  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, unsigned int v);
  void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, unsigned long v);
  inline void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, uint8_t v) { serial_echopair_PGM_P(p, s_P, (int)v); }
  inline void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, bool v)    { serial_echopair_PGM_P(p, s_P, (int)v); }
  inline void serial_echopair_PGM_P(const int8_t p, PGM_P const s_P, void *v)   { serial_echopair_PGM_P(p, s_P, (unsigned long)v); }

  void serial_spaces_P(const int8_t p, uint8_t count);
  #define SERIAL_ECHO_SP_P(p,C)             serial_spaces_P(p,C)

  void serialprintPGM_P(const int8_t p, PGM_P str);
  void serial_echo_start_P(const int8_t p);
  void serial_error_start_P(const int8_t p);

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
  #define SERIAL_CHAR_P(p,x)                SERIAL_CHAR(x)
  #define SERIAL_ECHO_P(p,x)                SERIAL_ECHO(x)
  #define SERIAL_ECHO_F_P(p,x,y)            SERIAL_ECHO_F(x,y)
  #define SERIAL_ECHOLN_P(p,x)              SERIAL_ECHOLN(x)
  #define SERIAL_PRINT_P(p,x,b)             SERIAL_PRINT(x,b)
  #define SERIAL_PRINTLN_P(p,x,b)           SERIAL_PRINTLN(x,b)
  #define SERIAL_PRINTF_P(p,args...)        SERIAL_PRINTF(args)
  #define SERIAL_FLUSH_P(p)                 SERIAL_FLUSH()
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX_P(p)             SERIAL_FLUSHTX()
  #endif

  #define SERIAL_ECHOPGM_P(p,x)             SERIAL_ECHOPGM(x)
  #define SERIAL_ECHOLNPGM_P(p,x)           SERIAL_ECHOLNPGM(x)
  #define SERIAL_ECHOPAIR_P(p, pre, value)  SERIAL_ECHOPAIR(pre, value)

  #define SERIAL_ECHO_P(p,x)                SERIAL_ECHO(x)
  #define SERIAL_ECHOLN_P(p,x)              SERIAL_ECHOLN(x)

  #define SERIAL_ECHO_START_P(p)            SERIAL_ECHO_START()
  #define SERIAL_ERROR_START_P(p)           SERIAL_ERROR_START()
  #define SERIAL_EOL_P(p)                   SERIAL_EOL()

  #define SERIAL_ECHOPAIR_F_P(p, pre, value, y)   SERIAL_ECHOPAIR_F(pre, value, y)
  #define SERIAL_ECHOLNPAIR_F_P(p, pre, value, y) SERIAL_ECHOLNPAIR_F(pre, value, y)

  #define serial_echopair_PGM_P(p,s_P,v)    serial_echopair_PGM(s_P, v)

  #define serial_spaces_P(p,c)              serial_spaces(c)
  #define SERIAL_ECHO_SP_P(p,C)             SERIAL_ECHO_SP(C)

  #define serialprintPGM_P(p,s)             serialprintPGM(s)

#endif // NUM_SERIAL < 2

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
#define SERIAL_ECHO_MSG_P(p, STR)           do{ SERIAL_ECHO_START_P(p); SERIAL_ECHOLNPGM_P(p, STR); }while(0)
#define SERIAL_ERROR_MSG(STR)               do{ SERIAL_ERROR_START(); SERIAL_ECHOLNPGM(STR); }while(0)
#define SERIAL_ERROR_MSG_P(p, STR)          do{ SERIAL_ERROR_START_P(p); SERIAL_ECHOLNPGM_P(p, STR); }while(0)

#define SERIAL_ECHOLNPAIR_P(p, pre, value)  do{ SERIAL_ECHOPAIR_P(p, pre, value); SERIAL_EOL_P(p); }while(0)

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
