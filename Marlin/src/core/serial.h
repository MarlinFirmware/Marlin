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

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "../inc/MarlinConfig.h"

#if HAS_ABL && ENABLED(DEBUG_LEVELING_FEATURE)
  #include "../libs/vector_3.h"
#endif

/**
 * Define debug bit-masks
 */
enum DebugFlags {
  DEBUG_NONE          = 0,
  DEBUG_ECHO          = _BV(0), ///< Echo commands in order as they are processed
  DEBUG_INFO          = _BV(1), ///< Print messages for code that has debug output
  DEBUG_ERRORS        = _BV(2), ///< Not implemented
  DEBUG_DRYRUN        = _BV(3), ///< Ignore temperature setting and E movement commands
  DEBUG_COMMUNICATION = _BV(4), ///< Not implemented
  DEBUG_LEVELING      = _BV(5), ///< Print detailed output for homing and leveling
  DEBUG_MESH_ADJUST   = _BV(6), ///< UBL bed leveling
  DEBUG_ALL           = 0xFF
};

#if ENABLED(EMERGENCY_PARSER)
  enum e_parser_state {
    state_RESET,
    state_N,
    state_M,
    state_M1,
    state_M10,
    state_M108,
    state_M11,
    state_M112,
    state_M4,
    state_M41,
    state_M410,
    state_IGNORE // to '\n'
  };
#endif

extern uint8_t marlin_debug_flags;
#define DEBUGGING(F) (marlin_debug_flags & (DEBUG_## F))

extern const char echomagic[] PROGMEM;
extern const char errormagic[] PROGMEM;

#if TX_BUFFER_SIZE < 1
  #define SERIAL_FLUSHTX_P(p)
  #define SERIAL_FLUSHTX()
#endif

#if NUM_SERIAL > 1
  #define SERIAL_CHAR_P(p,x)          (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.write(x) : MYSERIAL1.write(x)) : SERIAL_CHAR(x))
  #define SERIAL_PROTOCOL_P(p,x)      (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.print(x) : MYSERIAL1.print(x)) : SERIAL_PROTOCOL(x))
  #define SERIAL_PROTOCOL_F_P(p,x,y)  (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.print(x,y) : MYSERIAL1.print(x,y)) : SERIAL_PROTOCOL_F(x,y))
  #define SERIAL_PROTOCOLLN_P(p,x)    (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.println(x) : MYSERIAL1.println(x)) : SERIAL_PROTOCOLLN(x))
  #define SERIAL_PRINT_P(p,x,b)       (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.print(x,b) : MYSERIAL1.print(x,b)) : SERIAL_PRINT(x,b))
  #define SERIAL_PRINTLN_P(p,x,b)     (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.println(x,b) : MYSERIAL1.println(x,b)) : SERIAL_PRINTLN(x,b))
  #define SERIAL_PRINTF_P(p,args...)  (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.printf(args) : MYSERIAL1.printf(args)) : SERIAL_PRINTF(args))

  #define SERIAL_CHAR(x)              (MYSERIAL0.write(x), MYSERIAL1.write(x))
  #define SERIAL_PROTOCOL(x)          (MYSERIAL0.print(x), MYSERIAL1.print(x))
  #define SERIAL_PROTOCOL_F(x,y)      (MYSERIAL0.print(x,y), MYSERIAL1.print(x,y))
  #define SERIAL_PROTOCOLLN(x)        (MYSERIAL0.println(x), MYSERIAL1.println(x))
  #define SERIAL_PRINT(x,b)           (MYSERIAL0.print(x,b), MYSERIAL1.print(x,b))
  #define SERIAL_PRINTLN(x,b)         (MYSERIAL0.println(x,b), MYSERIAL1.println(x,b))
  #define SERIAL_PRINTF(args...)      (MYSERIAL0.printf(args), MYSERIAL1.printf(args))

  #define SERIAL_FLUSH_P(p)           (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.flush() : MYSERIAL1.flush()) : SERIAL_FLUSH())
  #define SERIAL_FLUSH()              (MYSERIAL0.flush(), MYSERIAL1.flush())
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX_P(p)       (WITHIN(p, 0, NUM_SERIAL-1) ? (p == 0 ? MYSERIAL0.flushTX() : MYSERIAL1.flushTX()) : SERIAL_FLUSHTX())
    #define SERIAL_FLUSHTX()          (MYSERIAL0.flushTX(), MYSERIAL1.flushTX())
  #endif

  #define SERIAL_EOL_P(p) SERIAL_CHAR_P(p,'\n')

  #define SERIAL_PROTOCOLCHAR_P(p,x)              SERIAL_CHAR_P(p,x)
  #define SERIAL_PROTOCOLPGM_P(p,x)               (serialprintPGM_P(p,PSTR(x)))
  #define SERIAL_PROTOCOLLNPGM_P(p,x)             (serialprintPGM_P(p,PSTR(x "\n")))
  #define SERIAL_PROTOCOLPAIR_P(p, pre, value)    (serial_echopair_PGM_P(p,PSTR(pre),(value)))
  #define SERIAL_PROTOCOLLNPAIR_P(p, pre, value)  do { SERIAL_PROTOCOLPAIR_P(p, pre, value); SERIAL_EOL_P(p); } while(0)

  #define SERIAL_ECHO_START_P(p)             (serialprintPGM_P(p,echomagic))
  #define SERIAL_ECHO_P(p,x)                 SERIAL_PROTOCOL_P(p,x)
  #define SERIAL_ECHOPGM_P(p,x)              SERIAL_PROTOCOLPGM_P(p,x)
  #define SERIAL_ECHOLN_P(p,x)               SERIAL_PROTOCOLLN_P(p,x)
  #define SERIAL_ECHOLNPGM_P(p,x)            SERIAL_PROTOCOLLNPGM_P(p,x)
  #define SERIAL_ECHOPAIR_P(p,pre,value)     SERIAL_PROTOCOLPAIR_P(p, pre, value)
  #define SERIAL_ECHOLNPAIR_P(p,pre, value)  SERIAL_PROTOCOLLNPAIR_P(p, pre, value)
  #define SERIAL_ECHO_F_P(p,x,y)             SERIAL_PROTOCOL_F_P(p,x,y)

  #define SERIAL_ERROR_START_P(p)            (serialprintPGM_P(p,errormagic))
  #define SERIAL_ERROR_P(p,x)                SERIAL_PROTOCOL_P(p,x)
  #define SERIAL_ERRORPGM_P(p,x)             SERIAL_PROTOCOLPGM_P(p,x)
  #define SERIAL_ERRORLN_P(p,x)              SERIAL_PROTOCOLLN_P(p,x)
  #define SERIAL_ERRORLNPGM_P(p,x)           SERIAL_PROTOCOLLNPGM_P(p,x)

  // These macros compensate for float imprecision
  #define SERIAL_PROTOCOLPAIR_F_P(p, pre, value)    SERIAL_PROTOCOLPAIR_P(p, pre, FIXFLOAT(value))
  #define SERIAL_PROTOCOLLNPAIR_F_P(p, pre, value)  SERIAL_PROTOCOLLNPAIR_P(p, pre, FIXFLOAT(value))
  #define SERIAL_ECHOPAIR_F_P(p,pre,value)          SERIAL_ECHOPAIR_P(p, pre, FIXFLOAT(value))
  #define SERIAL_ECHOLNPAIR_F_P(p,pre, value)       SERIAL_ECHOLNPAIR_P(p, pre, FIXFLOAT(value))

  void serial_echopair_PGM_P(const int8_t p, const char* s_P, const char *v);
  void serial_echopair_PGM_P(const int8_t p, const char* s_P, char v);
  void serial_echopair_PGM_P(const int8_t p, const char* s_P, int v);
  void serial_echopair_PGM_P(const int8_t p, const char* s_P, long v);
  void serial_echopair_PGM_P(const int8_t p, const char* s_P, float v);
  void serial_echopair_PGM_P(const int8_t p, const char* s_P, double v);
  void serial_echopair_PGM_P(const int8_t p, const char* s_P, unsigned int v);
  void serial_echopair_PGM_P(const int8_t p, const char* s_P, unsigned long v);
  FORCE_INLINE void serial_echopair_PGM_P(const int8_t p, const char* s_P, uint8_t v) { serial_echopair_PGM_P(p, s_P, (int)v); }
  FORCE_INLINE void serial_echopair_PGM_P(const int8_t p, const char* s_P, bool v) { serial_echopair_PGM_P(p, s_P, (int)v); }
  FORCE_INLINE void serial_echopair_PGM_P(const int8_t p, const char* s_P, void *v) { serial_echopair_PGM_P(p, s_P, (unsigned long)v); }

  void serial_spaces_P(const int8_t p, uint8_t count);
  #define SERIAL_ECHO_SP_P(p,C)     serial_spaces_P(p,C)
  #define SERIAL_ERROR_SP_P(p,C)    serial_spaces_P(p,C)
  #define SERIAL_PROTOCOL_SP_P(p,C) serial_spaces_P(p,C)

  void serialprintPGM_P(const int8_t p, const char* str);
#else
  #define SERIAL_CHAR_P(p,x)          SERIAL_CHAR(x)
  #define SERIAL_PROTOCOL_P(p,x)      SERIAL_PROTOCOL(x)
  #define SERIAL_PROTOCOL_F_P(p,x,y)  SERIAL_PROTOCOL_F(x,y)
  #define SERIAL_PROTOCOLLN_P(p,x)    SERIAL_PROTOCOLLN(x)
  #define SERIAL_PRINT_P(p,x,b)       SERIAL_PRINT(x,b)
  #define SERIAL_PRINTLN_P(p,x,b)     SERIAL_PRINTLN(x,b)
  #define SERIAL_PRINTF_P(p,args...)  SERIAL_PRINTF(args)

  #define SERIAL_CHAR(x)              MYSERIAL0.write(x)
  #define SERIAL_PROTOCOL(x)          MYSERIAL0.print(x)
  #define SERIAL_PROTOCOL_F(x,y)      MYSERIAL0.print(x,y)
  #define SERIAL_PROTOCOLLN(x)        MYSERIAL0.println(x)
  #define SERIAL_PRINT(x,b)           MYSERIAL0.print(x,b)
  #define SERIAL_PRINTLN(x,b)         MYSERIAL0.println(x,b)
  #define SERIAL_PRINTF(args...)      MYSERIAL0.printf(args)

  #define SERIAL_FLUSH_P(p)           SERIAL_FLUSH()
  #define SERIAL_FLUSH()              MYSERIAL0.flush()
  #if TX_BUFFER_SIZE > 0
    #define SERIAL_FLUSHTX_P(p)       SERIAL_FLUSHTX()
    #define SERIAL_FLUSHTX()          MYSERIAL0.flushTX()
  #endif

  #define SERIAL_EOL_P(p) SERIAL_EOL()

  #define SERIAL_PROTOCOLCHAR_P(p,x)              SERIAL_PROTOCOLCHAR(x)
  #define SERIAL_PROTOCOLPGM_P(p,x)               SERIAL_PROTOCOLPGM(x)
  #define SERIAL_PROTOCOLLNPGM_P(p,x)             SERIAL_PROTOCOLLNPGM(x)
  #define SERIAL_PROTOCOLPAIR_P(p, pre, value)    SERIAL_PROTOCOLPAIR(pre, value)
  #define SERIAL_PROTOCOLLNPAIR_P(p, pre, value)  SERIAL_PROTOCOLLNPAIR(pre, value)

  #define SERIAL_ECHO_START_P(p)             SERIAL_ECHO_START()
  #define SERIAL_ECHO_P(p,x)                 SERIAL_ECHO(x)
  #define SERIAL_ECHOPGM_P(p,x)              SERIAL_ECHOPGM(x)
  #define SERIAL_ECHOLN_P(p,x)               SERIAL_ECHOLN(x)
  #define SERIAL_ECHOLNPGM_P(p,x)            SERIAL_ECHOLNPGM(x)
  #define SERIAL_ECHOPAIR_P(p,pre,value)     SERIAL_ECHOPAIR(pre, value)
  #define SERIAL_ECHOLNPAIR_P(p,pre, value)  SERIAL_ECHOLNPAIR(pre, value)
  #define SERIAL_ECHO_F_P(p,x,y)             SERIAL_ECHO_F(x,y)

  #define SERIAL_ERROR_START_P(p)            SERIAL_ERROR_START()
  #define SERIAL_ERROR_P(p,x)                SERIAL_ERROR(x)
  #define SERIAL_ERRORPGM_P(p,x)             SERIAL_ERRORPGM(x)
  #define SERIAL_ERRORLN_P(p,x)              SERIAL_ERRORLN(x)
  #define SERIAL_ERRORLNPGM_P(p,x)           SERIAL_ERRORLNPGM(x)

  // These macros compensate for float imprecision
  #define SERIAL_PROTOCOLPAIR_F_P(p, pre, value)    SERIAL_PROTOCOLPAIR_F(pre, value)
  #define SERIAL_PROTOCOLLNPAIR_F_P(p, pre, value)  SERIAL_PROTOCOLLNPAIR_F(pre, value)
  #define SERIAL_ECHOPAIR_F_P(p,pre,value)          SERIAL_ECHOPAIR_F(pre, value)
  #define SERIAL_ECHOLNPAIR_F_P(p,pre, value)       SERIAL_ECHOLNPAIR_F(pre, value)

  #define serial_echopair_PGM_P(p,s_P,v)            serial_echopair_PGM(s_P, v)

  #define serial_spaces_P(p,c)      serial_spaces(c)
  #define SERIAL_ECHO_SP_P(p,C)     SERIAL_ECHO_SP(C)
  #define SERIAL_ERROR_SP_P(p,C)    SERIAL_ERROR_SP(C)
  #define SERIAL_PROTOCOL_SP_P(p,C) SERIAL_PROTOCOL_SP(C)

  #define serialprintPGM_P(p,s)     serialprintPGM(s)
#endif

#define SERIAL_EOL() SERIAL_CHAR('\n')

#define SERIAL_PROTOCOLCHAR(x)              SERIAL_CHAR(x)
#define SERIAL_PROTOCOLPGM(x)               (serialprintPGM(PSTR(x)))
#define SERIAL_PROTOCOLLNPGM(x)             (serialprintPGM(PSTR(x "\n")))
#define SERIAL_PROTOCOLPAIR(pre, value)     (serial_echopair_PGM(PSTR(pre), value))
#define SERIAL_PROTOCOLLNPAIR(pre, value)   do { SERIAL_PROTOCOLPAIR(pre, value); SERIAL_EOL(); } while(0)

#define SERIAL_ECHO_START()            (serialprintPGM(echomagic))
#define SERIAL_ECHO(x)                 SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x)              SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x)               SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x)            SERIAL_PROTOCOLLNPGM(x)
#define SERIAL_ECHOPAIR(pre,value)     SERIAL_PROTOCOLPAIR(pre, value)
#define SERIAL_ECHOLNPAIR(pre, value)  SERIAL_PROTOCOLLNPAIR(pre, value)
#define SERIAL_ECHO_F(x,y)             SERIAL_PROTOCOL_F(x, y)

#define SERIAL_ERROR_START()           (serialprintPGM(errormagic))
#define SERIAL_ERROR(x)                SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x)             SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x)              SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x)           SERIAL_PROTOCOLLNPGM(x)

// These macros compensate for float imprecision
#define SERIAL_PROTOCOLPAIR_F(pre, value)   SERIAL_PROTOCOLPAIR(pre, FIXFLOAT(value))
#define SERIAL_PROTOCOLLNPAIR_F(pre, value) SERIAL_PROTOCOLLNPAIR(pre, FIXFLOAT(value))
#define SERIAL_ECHOPAIR_F(pre,value)        SERIAL_ECHOPAIR(pre, FIXFLOAT(value))
#define SERIAL_ECHOLNPAIR_F(pre, value)     SERIAL_ECHOLNPAIR(pre, FIXFLOAT(value))

void serial_echopair_PGM(const char* s_P, const char *v);
void serial_echopair_PGM(const char* s_P, char v);
void serial_echopair_PGM(const char* s_P, int v);
void serial_echopair_PGM(const char* s_P, long v);
void serial_echopair_PGM(const char* s_P, float v);
void serial_echopair_PGM(const char* s_P, double v);
void serial_echopair_PGM(const char* s_P, unsigned int v);
void serial_echopair_PGM(const char* s_P, unsigned long v);
FORCE_INLINE void serial_echopair_PGM(const char* s_P, uint8_t v) { serial_echopair_PGM(s_P, (int)v); }
FORCE_INLINE void serial_echopair_PGM(const char* s_P, bool v) { serial_echopair_PGM(s_P, (int)v); }
FORCE_INLINE void serial_echopair_PGM(const char* s_P, void *v) { serial_echopair_PGM(s_P, (unsigned long)v); }

void serial_spaces(uint8_t count);
#define SERIAL_ECHO_SP(C)     serial_spaces(C)
#define SERIAL_ERROR_SP(C)    serial_spaces(C)
#define SERIAL_PROTOCOL_SP(C) serial_spaces(C)

//
// Functions for serial printing from PROGMEM. (Saves loads of SRAM.)
//
void serialprintPGM(const char* str);

#if ENABLED(DEBUG_LEVELING_FEATURE)
  void print_xyz(const char* prefix, const char* suffix, const float x, const float y, const float z);
  void print_xyz(const char* prefix, const char* suffix, const float xyz[]);
  #if HAS_ABL
    void print_xyz(const char* prefix, const char* suffix, const vector_3 &xyz);
  #endif
  #define DEBUG_POS(SUFFIX,VAR) do { print_xyz(PSTR("  " STRINGIFY(VAR) "="), PSTR(" : " SUFFIX "\n"), VAR); } while(0)
#endif

#endif // __SERIAL_H__
