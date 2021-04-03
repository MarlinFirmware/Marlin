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
#pragma once

#include "../inc/MarlinConfig.h"
#include "serial_hook.h"

// Commonly-used strings in serial output
extern const char NUL_STR[], SP_P_STR[], SP_T_STR[],
                  X_STR[], Y_STR[], Z_STR[], E_STR[],
                  X_LBL[], Y_LBL[], Z_LBL[], E_LBL[],
                  SP_A_STR[], SP_B_STR[], SP_C_STR[],
                  SP_X_STR[], SP_Y_STR[], SP_Z_STR[], SP_E_STR[],
                  SP_X_LBL[], SP_Y_LBL[], SP_Z_LBL[], SP_E_LBL[];

//
// Debugging flags for use by M111
//
enum MarlinDebugFlags : uint8_t {
  MARLIN_DEBUG_NONE          = 0,
  MARLIN_DEBUG_ECHO          = _BV(0), ///< Echo commands in order as they are processed
  MARLIN_DEBUG_INFO          = _BV(1), ///< Print messages for code that has debug output
  MARLIN_DEBUG_ERRORS        = _BV(2), ///< Not implemented
  MARLIN_DEBUG_DRYRUN        = _BV(3), ///< Ignore temperature setting and E movement commands
  MARLIN_DEBUG_COMMUNICATION = _BV(4), ///< Not implemented
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    MARLIN_DEBUG_LEVELING    = _BV(5), ///< Print detailed output for homing and leveling
    MARLIN_DEBUG_MESH_ADJUST = _BV(6), ///< UBL bed leveling
  #else
    MARLIN_DEBUG_LEVELING    = 0,
    MARLIN_DEBUG_MESH_ADJUST = 0,
  #endif
  MARLIN_DEBUG_ALL           = 0xFF
};

extern uint8_t marlin_debug_flags;
#define DEBUGGING(F) (marlin_debug_flags & (MARLIN_DEBUG_## F))

//
// Serial redirection
//
typedef int8_t serial_index_t;
#define SERIAL_ALL 0x7F
#if HAS_MULTI_SERIAL
  #define _PORT_REDIRECT(n,p)   REMEMBER(n,multiSerial.portMask,p)
  #define _PORT_RESTORE(n,p)    RESTORE(n)
  #define SERIAL_ASSERT(P)      if(multiSerial.portMask!=(P)){ debugger(); }
  #ifdef SERIAL_CATCHALL
    typedef MultiSerial<decltype(MYSERIAL), decltype(SERIAL_CATCHALL), 0> SerialOutputT;
  #else
    typedef MultiSerial<decltype(MYSERIAL0), TERN(HAS_ETHERNET, ConditionalSerial<decltype(MYSERIAL1)>, decltype(MYSERIAL1)), 0>      SerialOutputT;
  #endif
  extern SerialOutputT          multiSerial;
  #define SERIAL_IMPL           multiSerial
#else
  #define _PORT_REDIRECT(n,p)   NOOP
  #define _PORT_RESTORE(n)      NOOP
  #define SERIAL_ASSERT(P)      NOOP
  #define SERIAL_IMPL           MYSERIAL0
#endif

#define SERIAL_OUT(WHAT, V...)  (void)SERIAL_IMPL.WHAT(V)

#define PORT_REDIRECT(p)        _PORT_REDIRECT(1,p)
#define PORT_RESTORE()          _PORT_RESTORE(1)
#define SERIAL_PORTMASK(P)      _BV(P)

//
// SERIAL_CHAR - Print one or more individual chars
//
inline void SERIAL_CHAR(char a) { SERIAL_IMPL.write(a); }
template <typename ... Args>
void SERIAL_CHAR(char a, Args ... args) { SERIAL_IMPL.write(a); SERIAL_CHAR(args ...); }

/**
 * SERIAL_ECHO - Print a single string or value.
 *   Any numeric parameter (including char) is printed as a base-10 number.
 *   A string pointer or literal will be output as a string.
 *
 * NOTE: Use SERIAL_CHAR to print char as a single character.
 */
template <typename T>
void SERIAL_ECHO(T x) { SERIAL_IMPL.print(x); }

// Wrapper for ECHO commands to interpret a char
typedef struct SerialChar { char c; SerialChar(char n) : c(n) { } } serial_char_t;
inline void SERIAL_ECHO(serial_char_t x) { SERIAL_IMPL.write(x.c); }
#define AS_CHAR(C) serial_char_t(C)

// SERIAL_ECHO_F prints a floating point value with optional precision
inline void SERIAL_ECHO_F(EnsureDouble x, int digit = 2) { SERIAL_IMPL.print(x, digit); }

template <typename T>
void SERIAL_ECHOLN(T x) { SERIAL_IMPL.println(x); }

// SERIAL_PRINT works like SERIAL_ECHO but allow to specify the encoding base of the number printed
template <typename T, typename U>
void SERIAL_PRINT(T x, U y) { SERIAL_IMPL.print(x, y); }

template <typename T, typename U>
void SERIAL_PRINTLN(T x, U y) { SERIAL_IMPL.println(x, y); }

// Flush the serial port
inline void SERIAL_FLUSH()    { SERIAL_IMPL.flush(); }
inline void SERIAL_FLUSHTX()  { SERIAL_IMPL.flushTX(); }

// Print a single PROGMEM string to serial
void serialprintPGM(PGM_P str);

// SERIAL_ECHOPAIR / SERIAL_ECHOPAIR_P is used to output a key value pair. The key must be a string and the value can be anything
// Print up to 12 pairs of values. Odd elements auto-wrapped in PSTR().
#define __SEP_N(N,V...)   _SEP_##N(V)
#define _SEP_N(N,V...)    __SEP_N(N,V)
#define _SEP_1(PRE)       SERIAL_ECHOPGM(PRE)
#define _SEP_2(PRE,V)     serial_echopair_PGM(PSTR(PRE),V)
#define _SEP_3(a,b,c)     do{ _SEP_2(a,b); SERIAL_ECHOPGM(c); }while(0)
#define _SEP_4(a,b,V...)  do{ _SEP_2(a,b); _SEP_2(V); }while(0)
#define _SEP_5(a,b,V...)  do{ _SEP_2(a,b); _SEP_3(V); }while(0)
#define _SEP_6(a,b,V...)  do{ _SEP_2(a,b); _SEP_4(V); }while(0)
#define _SEP_7(a,b,V...)  do{ _SEP_2(a,b); _SEP_5(V); }while(0)
#define _SEP_8(a,b,V...)  do{ _SEP_2(a,b); _SEP_6(V); }while(0)
#define _SEP_9(a,b,V...)  do{ _SEP_2(a,b); _SEP_7(V); }while(0)
#define _SEP_10(a,b,V...) do{ _SEP_2(a,b); _SEP_8(V); }while(0)
#define _SEP_11(a,b,V...) do{ _SEP_2(a,b); _SEP_9(V); }while(0)
#define _SEP_12(a,b,V...) do{ _SEP_2(a,b); _SEP_10(V); }while(0)
#define _SEP_13(a,b,V...) do{ _SEP_2(a,b); _SEP_11(V); }while(0)
#define _SEP_14(a,b,V...) do{ _SEP_2(a,b); _SEP_12(V); }while(0)
#define _SEP_15(a,b,V...) do{ _SEP_2(a,b); _SEP_13(V); }while(0)
#define _SEP_16(a,b,V...) do{ _SEP_2(a,b); _SEP_14(V); }while(0)
#define _SEP_17(a,b,V...) do{ _SEP_2(a,b); _SEP_15(V); }while(0)
#define _SEP_18(a,b,V...) do{ _SEP_2(a,b); _SEP_16(V); }while(0)
#define _SEP_19(a,b,V...) do{ _SEP_2(a,b); _SEP_17(V); }while(0)
#define _SEP_20(a,b,V...) do{ _SEP_2(a,b); _SEP_18(V); }while(0)
#define _SEP_21(a,b,V...) do{ _SEP_2(a,b); _SEP_19(V); }while(0)
#define _SEP_22(a,b,V...) do{ _SEP_2(a,b); _SEP_20(V); }while(0)
#define _SEP_23(a,b,V...) do{ _SEP_2(a,b); _SEP_21(V); }while(0)
#define _SEP_24(a,b,V...) do{ _SEP_2(a,b); _SEP_22(V); }while(0)

#define SERIAL_ECHOPAIR(V...) _SEP_N(NUM_ARGS(V),V)

// Print up to 12 pairs of values. Odd elements must be PSTR pointers.
#define __SEP_N_P(N,V...)   _SEP_##N##_P(V)
#define _SEP_N_P(N,V...)    __SEP_N_P(N,V)
#define _SEP_1_P(PRE)       serialprintPGM(PRE)
#define _SEP_2_P(PRE,V)     serial_echopair_PGM(PRE,V)
#define _SEP_3_P(a,b,c)     do{ _SEP_2_P(a,b); serialprintPGM(c); }while(0)
#define _SEP_4_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_2_P(V); }while(0)
#define _SEP_5_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_3_P(V); }while(0)
#define _SEP_6_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_4_P(V); }while(0)
#define _SEP_7_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_5_P(V); }while(0)
#define _SEP_8_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_6_P(V); }while(0)
#define _SEP_9_P(a,b,V...)  do{ _SEP_2_P(a,b); _SEP_7_P(V); }while(0)
#define _SEP_10_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_8_P(V); }while(0)
#define _SEP_11_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_9_P(V); }while(0)
#define _SEP_12_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_10_P(V); }while(0)
#define _SEP_13_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_11_P(V); }while(0)
#define _SEP_14_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_12_P(V); }while(0)
#define _SEP_15_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_13_P(V); }while(0)
#define _SEP_16_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_14_P(V); }while(0)
#define _SEP_17_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_15_P(V); }while(0)
#define _SEP_18_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_16_P(V); }while(0)
#define _SEP_19_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_17_P(V); }while(0)
#define _SEP_20_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_18_P(V); }while(0)
#define _SEP_21_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_19_P(V); }while(0)
#define _SEP_22_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_20_P(V); }while(0)
#define _SEP_23_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_21_P(V); }while(0)
#define _SEP_24_P(a,b,V...) do{ _SEP_2_P(a,b); _SEP_22_P(V); }while(0)

// SERIAL_ECHOPAIR_P is used to output a key value pair. Unlike SERIAL_ECHOPAIR, the key must be a PGM string already and the value can be anything
#define SERIAL_ECHOPAIR_P(V...) _SEP_N_P(NUM_ARGS(V),V)

// Print up to 12 pairs of values followed by newline
#define __SELP_N(N,V...)   _SELP_##N(V)
#define _SELP_N(N,V...)    __SELP_N(N,V)
#define _SELP_1(PRE)       SERIAL_ECHOLNPGM(PRE)
#define _SELP_2(PRE,V)     do{ serial_echopair_PGM(PSTR(PRE),V); SERIAL_EOL(); }while(0)
#define _SELP_3(a,b,c)     do{ _SEP_2(a,b); SERIAL_ECHOLNPGM(c); }while(0)
#define _SELP_4(a,b,V...)  do{ _SEP_2(a,b); _SELP_2(V); }while(0)
#define _SELP_5(a,b,V...)  do{ _SEP_2(a,b); _SELP_3(V); }while(0)
#define _SELP_6(a,b,V...)  do{ _SEP_2(a,b); _SELP_4(V); }while(0)
#define _SELP_7(a,b,V...)  do{ _SEP_2(a,b); _SELP_5(V); }while(0)
#define _SELP_8(a,b,V...)  do{ _SEP_2(a,b); _SELP_6(V); }while(0)
#define _SELP_9(a,b,V...)  do{ _SEP_2(a,b); _SELP_7(V); }while(0)
#define _SELP_10(a,b,V...) do{ _SEP_2(a,b); _SELP_8(V); }while(0)
#define _SELP_11(a,b,V...) do{ _SEP_2(a,b); _SELP_9(V); }while(0)
#define _SELP_12(a,b,V...) do{ _SEP_2(a,b); _SELP_10(V); }while(0)
#define _SELP_13(a,b,V...) do{ _SEP_2(a,b); _SELP_11(V); }while(0)
#define _SELP_14(a,b,V...) do{ _SEP_2(a,b); _SELP_12(V); }while(0)
#define _SELP_15(a,b,V...) do{ _SEP_2(a,b); _SELP_13(V); }while(0)
#define _SELP_16(a,b,V...) do{ _SEP_2(a,b); _SELP_14(V); }while(0)
#define _SELP_17(a,b,V...) do{ _SEP_2(a,b); _SELP_15(V); }while(0)
#define _SELP_18(a,b,V...) do{ _SEP_2(a,b); _SELP_16(V); }while(0)
#define _SELP_19(a,b,V...) do{ _SEP_2(a,b); _SELP_17(V); }while(0)
#define _SELP_20(a,b,V...) do{ _SEP_2(a,b); _SELP_18(V); }while(0)
#define _SELP_21(a,b,V...) do{ _SEP_2(a,b); _SELP_19(V); }while(0)
#define _SELP_22(a,b,V...) do{ _SEP_2(a,b); _SELP_20(V); }while(0)
#define _SELP_23(a,b,V...) do{ _SEP_2(a,b); _SELP_21(V); }while(0)
#define _SELP_24(a,b,V...) do{ _SEP_2(a,b); _SELP_22(V); }while(0)
#define _SELP_25(a,b,V...) do{ _SEP_2(a,b); _SELP_23(V); }while(0)
#define _SELP_26(a,b,V...) do{ _SEP_2(a,b); _SELP_24(V); }while(0)
#define _SELP_27(a,b,V...) do{ _SEP_2(a,b); _SELP_25(V); }while(0)
#define _SELP_28(a,b,V...) do{ _SEP_2(a,b); _SELP_26(V); }while(0)
#define _SELP_29(a,b,V...) do{ _SEP_2(a,b); _SELP_27(V); }while(0)
#define _SELP_30(a,b,V...) do{ _SEP_2(a,b); _SELP_28(V); }while(0) // Eat two args, pass the rest up

#define SERIAL_ECHOLNPAIR(V...) _SELP_N(NUM_ARGS(V),V)

// Print up to 12 pairs of values followed by newline
#define __SELP_N_P(N,V...)   _SELP_##N##_P(V)
#define _SELP_N_P(N,V...)    __SELP_N_P(N,V)
#define _SELP_1_P(PRE)       serialprintPGM(PRE)
#define _SELP_2_P(PRE,V)     do{ serial_echopair_PGM(PRE,V); SERIAL_EOL(); }while(0)
#define _SELP_3_P(a,b,c)     do{ _SEP_2_P(a,b); serialprintPGM(c); }while(0)
#define _SELP_4_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_2_P(V); }while(0)
#define _SELP_5_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_3_P(V); }while(0)
#define _SELP_6_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_4_P(V); }while(0)
#define _SELP_7_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_5_P(V); }while(0)
#define _SELP_8_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_6_P(V); }while(0)
#define _SELP_9_P(a,b,V...)  do{ _SEP_2_P(a,b); _SELP_7_P(V); }while(0)
#define _SELP_10_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_8_P(V); }while(0)
#define _SELP_11_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_9_P(V); }while(0)
#define _SELP_12_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_10_P(V); }while(0)
#define _SELP_13_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_11_P(V); }while(0)
#define _SELP_14_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_12_P(V); }while(0)
#define _SELP_15_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_13_P(V); }while(0)
#define _SELP_16_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_14_P(V); }while(0)
#define _SELP_17_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_15_P(V); }while(0)
#define _SELP_18_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_16_P(V); }while(0)
#define _SELP_19_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_17_P(V); }while(0)
#define _SELP_20_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_18_P(V); }while(0)
#define _SELP_21_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_19_P(V); }while(0)
#define _SELP_22_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_20_P(V); }while(0)
#define _SELP_23_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_21_P(V); }while(0)
#define _SELP_24_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_22_P(V); }while(0)
#define _SELP_25_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_23_P(V); }while(0)
#define _SELP_26_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_24_P(V); }while(0)
#define _SELP_27_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_25_P(V); }while(0)
#define _SELP_28_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_26_P(V); }while(0)
#define _SELP_29_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_27_P(V); }while(0)
#define _SELP_30_P(a,b,V...) do{ _SEP_2_P(a,b); _SELP_28_P(V); }while(0) // Eat two args, pass the rest up

#define SERIAL_ECHOLNPAIR_P(V...) _SELP_N_P(NUM_ARGS(V),V)

#ifdef AllowDifferentTypeInList

  inline void SERIAL_ECHOLIST_IMPL() {}
  template <typename T>
  void SERIAL_ECHOLIST_IMPL(T && t) { SERIAL_IMPL.print(t); }

  template <typename T, typename ... Args>
  void SERIAL_ECHOLIST_IMPL(T && t, Args && ... args) {
    SERIAL_IMPL.print(t);
    serialprintPGM(PSTR(", "));
    SERIAL_ECHOLIST_IMPL(args...);
  }

  template <typename ... Args>
  void SERIAL_ECHOLIST(PGM_P const str, Args && ... args) {
    SERIAL_IMPL.print(str);
    SERIAL_ECHOLIST_IMPL(args...);
  }

#else // Optimization if the listed type are all the same (seems to be the case in the codebase so use that instead)

  template <typename ... Args>
  void SERIAL_ECHOLIST(PGM_P const str, Args && ... args) {
    serialprintPGM(str);
    typename Private::first_type_of<Args...>::type values[] = { args... };
    constexpr size_t argsSize = sizeof...(args);
    for (size_t i = 0; i < argsSize; i++) {
      if (i) serialprintPGM(PSTR(", "));
      SERIAL_IMPL.print(values[i]);
    }
  }

#endif

#define SERIAL_ECHOPGM_P(P)         (serialprintPGM(P))
#define SERIAL_ECHOLNPGM_P(P)       (serialprintPGM(P "\n"))

#define SERIAL_ECHOPGM(S)           (serialprintPGM(PSTR(S)))
#define SERIAL_ECHOLNPGM(S)         (serialprintPGM(PSTR(S "\n")))

#define SERIAL_ECHOPAIR_F_P(P,V...) do{ serialprintPGM(P); SERIAL_ECHO_F(V); }while(0)
#define SERIAL_ECHOLNPAIR_F_P(V...) do{ SERIAL_ECHOPAIR_F_P(V); SERIAL_EOL(); }while(0)

#define SERIAL_ECHOPAIR_F(S,V...)   SERIAL_ECHOPAIR_F_P(PSTR(S),V)
#define SERIAL_ECHOLNPAIR_F(V...)   do{ SERIAL_ECHOPAIR_F(V); SERIAL_EOL(); }while(0)

#define SERIAL_ECHO_START()         serial_echo_start()
#define SERIAL_ERROR_START()        serial_error_start()
#define SERIAL_EOL()                SERIAL_CHAR('\n')

#define SERIAL_ECHO_MSG(V...)       do{ SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR(V); }while(0)
#define SERIAL_ERROR_MSG(V...)      do{ SERIAL_ERROR_START(); SERIAL_ECHOLNPAIR(V); }while(0)

#define SERIAL_ECHO_SP(C)           serial_spaces(C)

#define SERIAL_ECHO_TERNARY(TF, PRE, ON, OFF, POST) serial_ternary(TF, PSTR(PRE), PSTR(ON), PSTR(OFF), PSTR(POST))

#if SERIAL_FLOAT_PRECISION
  #define SERIAL_DECIMAL(V) SERIAL_PRINT(V, SERIAL_FLOAT_PRECISION)
#else
  #define SERIAL_DECIMAL(V) SERIAL_ECHO(V)
#endif

//
// Functions for serial printing from PROGMEM. (Saves loads of SRAM.)
//
void serial_echopair_PGM(PGM_P const s_P, serial_char_t v);
void serial_echopair_PGM(PGM_P const s_P, const char *v);
void serial_echopair_PGM(PGM_P const s_P, char v);
void serial_echopair_PGM(PGM_P const s_P, int v);
void serial_echopair_PGM(PGM_P const s_P, long v);
void serial_echopair_PGM(PGM_P const s_P, float v);
void serial_echopair_PGM(PGM_P const s_P, double v);
void serial_echopair_PGM(PGM_P const s_P, unsigned char v);
void serial_echopair_PGM(PGM_P const s_P, unsigned int v);
void serial_echopair_PGM(PGM_P const s_P, unsigned long v);
inline void serial_echopair_PGM(PGM_P const s_P, bool v)    { serial_echopair_PGM(s_P, (int)v); }
inline void serial_echopair_PGM(PGM_P const s_P, void *v)   { serial_echopair_PGM(s_P, (uintptr_t)v); }

void serial_echo_start();
void serial_error_start();
void serial_ternary(const bool onoff, PGM_P const pre, PGM_P const on, PGM_P const off, PGM_P const post=nullptr);
void serialprint_onoff(const bool onoff);
void serialprintln_onoff(const bool onoff);
void serialprint_truefalse(const bool tf);
void serial_spaces(uint8_t count);

void print_bin(const uint16_t val);
void print_xyz(const float &x, const float &y, const float &z, PGM_P const prefix=nullptr, PGM_P const suffix=nullptr);

inline void print_xyz(const xyz_pos_t &xyz, PGM_P const prefix=nullptr, PGM_P const suffix=nullptr) {
  print_xyz(xyz.x, xyz.y, xyz.z, prefix, suffix);
}

#define SERIAL_POS(SUFFIX,VAR) do { print_xyz(VAR, PSTR("  " STRINGIFY(VAR) "="), PSTR(" : " SUFFIX "\n")); }while(0)
#define SERIAL_XYZ(PREFIX,V...) do { print_xyz(V, PSTR(PREFIX), nullptr); }while(0)
