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

#if HAS_MEATPACK
  #include "../feature/meatpack.h"
#endif

// Commonly-used strings in serial output
extern const char NUL_STR[], SP_P_STR[], SP_T_STR[],
                  SP_A_STR[], SP_B_STR[], SP_C_STR[],
                  LOGICAL_AXIS_LIST(SP_E_STR[], SP_X_STR[], SP_Y_STR[], SP_Z_STR[], SP_I_STR[], SP_J_STR[], SP_K_STR[], SP_U_STR[], SP_V_STR[], SP_W_STR[]),
                  LOGICAL_AXIS_LIST(SP_E_LBL[], SP_X_LBL[], SP_Y_LBL[], SP_Z_LBL[], SP_I_LBL[], SP_J_LBL[], SP_K_LBL[], SP_U_LBL[], SP_V_LBL[], SP_W_LBL[]),
                  LOGICAL_AXIS_LIST(E_STR[], X_STR[], Y_STR[], Z_STR[], I_STR[], J_STR[], K_STR[], U_STR[], V_STR[], W_STR[]),
                  LOGICAL_AXIS_LIST(E_LBL[], X_LBL[], Y_LBL[], Z_LBL[], I_LBL[], J_LBL[], K_LBL[], U_LBL[], V_LBL[], W_LBL[]);

PGM_P const SP_AXIS_LBL[] PROGMEM = LOGICAL_AXIS_ARRAY(SP_E_LBL, SP_X_LBL, SP_Y_LBL, SP_Z_LBL, SP_I_LBL, SP_J_LBL, SP_K_LBL, SP_U_LBL, SP_V_LBL, SP_W_LBL);
PGM_P const SP_AXIS_STR[] PROGMEM = LOGICAL_AXIS_ARRAY(SP_E_STR, SP_X_STR, SP_Y_STR, SP_Z_STR, SP_I_STR, SP_J_STR, SP_K_STR, SP_U_STR, SP_V_STR, SP_W_STR);

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
// Step 1: Find out what the first serial leaf is
#if HAS_MULTI_SERIAL && defined(SERIAL_CATCHALL)
  #define _SERIAL_LEAF_1 MYSERIAL
#else
  #define _SERIAL_LEAF_1 MYSERIAL1
#endif

// Hook Meatpack if it's enabled on the first leaf
#if ENABLED(MEATPACK_ON_SERIAL_PORT_1)
  typedef MeatpackSerial<decltype(_SERIAL_LEAF_1)> SerialLeafT1;
  extern SerialLeafT1 mpSerial1;
  #define SERIAL_LEAF_1 mpSerial1
#else
  #define SERIAL_LEAF_1 _SERIAL_LEAF_1
#endif

// Step 2: For multiserial wrap all serial ports in a single
//         interface with the ability to output to multiple serial ports.
#if HAS_MULTI_SERIAL
  #define _PORT_REDIRECT(n,p) REMEMBER(n,multiSerial.portMask,p)
  #define _PORT_RESTORE(n)    RESTORE(n)
  #define SERIAL_ASSERT(P)    if (multiSerial.portMask!=(P)) { debugger(); }
  // If we have a catchall, use that directly
  #ifdef SERIAL_CATCHALL
    #define _SERIAL_LEAF_2 SERIAL_CATCHALL
  #elif HAS_ETHERNET
    typedef ConditionalSerial<decltype(MYSERIAL2)> SerialLeafT2;  // We need to create an instance here
    extern SerialLeafT2 msSerial2;
    #define _SERIAL_LEAF_2 msSerial2
  #else
    #define _SERIAL_LEAF_2 MYSERIAL2 // Don't create a useless instance here, directly use the existing instance
  #endif

  // Nothing complicated here
  #define _SERIAL_LEAF_3 MYSERIAL3

  // Hook Meatpack if it's enabled on the second leaf
  #if ENABLED(MEATPACK_ON_SERIAL_PORT_2)
    typedef MeatpackSerial<decltype(_SERIAL_LEAF_2)> SerialLeafT2;
    extern SerialLeafT2 mpSerial2;
    #define SERIAL_LEAF_2 mpSerial2
  #else
    #define SERIAL_LEAF_2 _SERIAL_LEAF_2
  #endif

  // Hook Meatpack if it's enabled on the third leaf
  #if ENABLED(MEATPACK_ON_SERIAL_PORT_3)
    typedef MeatpackSerial<decltype(_SERIAL_LEAF_3)> SerialLeafT3;
    extern SerialLeafT3 mpSerial3;
    #define SERIAL_LEAF_3 mpSerial3
  #else
    #define SERIAL_LEAF_3 _SERIAL_LEAF_3
  #endif

  #define __S_MULTI(N) decltype(SERIAL_LEAF_##N),
  #define _S_MULTI(N) __S_MULTI(N)

  typedef MultiSerial< REPEAT_1(NUM_SERIAL, _S_MULTI) 0> SerialOutputT;

  #undef __S_MULTI
  #undef _S_MULTI

  extern SerialOutputT        multiSerial;
  #define SERIAL_IMPL         multiSerial
#else
  #define _PORT_REDIRECT(n,p) NOOP
  #define _PORT_RESTORE(n)    NOOP
  #define SERIAL_ASSERT(P)    NOOP
  #define SERIAL_IMPL         SERIAL_LEAF_1
#endif

#define SERIAL_OUT(WHAT, V...)  (void)SERIAL_IMPL.WHAT(V)

#define PORT_REDIRECT(p)   _PORT_REDIRECT(1,p)
#define PORT_RESTORE()     _PORT_RESTORE(1)
#define SERIAL_PORTMASK(P) SerialMask::from(P)

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
#define AS_DIGIT(C) AS_CHAR('0' + (C))

template <typename T>
void SERIAL_ECHOLN(T x) { SERIAL_IMPL.println(x); }

// SERIAL_PRINT works like SERIAL_ECHO but also takes the numeric base
template <typename T, typename U>
void SERIAL_PRINT(T x, U y) { SERIAL_IMPL.print(x, y); }

template <typename T>
void SERIAL_PRINTLN(T x, PrintBase y) { SERIAL_IMPL.println(x, y); }

// Flush the serial port
inline void SERIAL_FLUSH()    { SERIAL_IMPL.flush(); }
inline void SERIAL_FLUSHTX()  { SERIAL_IMPL.flushTX(); }

// Serial echo and error prefixes
#define SERIAL_ECHO_START()           serial_echo_start()
#define SERIAL_ERROR_START()          serial_error_start()

// Serial end-of-line
#define SERIAL_EOL()                  SERIAL_CHAR('\n')

// Print a single PROGMEM, PGM_P, or PSTR() string.
void serial_print_P(PGM_P str);
inline void serial_println_P(PGM_P str) { serial_print_P(str); SERIAL_EOL(); }

// Print a single FSTR_P, F(), or FPSTR() string.
inline void serial_print(FSTR_P const fstr) { serial_print_P(FTOP(fstr)); }
inline void serial_println(FSTR_P const fstr) { serial_println_P(FTOP(fstr)); }

//
// SERIAL_ECHOPGM... macros are used to output string-value pairs.
//

// Print up to 20 pairs of values. Odd elements must be literal strings.
#define __SEP_N(N,V...)           _SEP_##N(V)
#define _SEP_N(N,V...)            __SEP_N(N,V)
#define _SEP_N_REF()              _SEP_N
#define _SEP_1(s)                 serial_print(F(s));
#define _SEP_2(s,v)               serial_echopair(F(s),v);
#define _SEP_3(s,v,V...)          _SEP_2(s,v); DEFER2(_SEP_N_REF)()(TWO_ARGS(V),V);
#define SERIAL_ECHOPGM(V...)      do{ EVAL(_SEP_N(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values followed by newline. Odd elements must be literal strings.
#define __SELP_N(N,V...)          _SELP_##N(V)
#define _SELP_N(N,V...)           __SELP_N(N,V)
#define _SELP_N_REF()             _SELP_N
#define _SELP_1(s)                serial_print(F(s "\n"));
#define _SELP_2(s,v)              serial_echolnpair(F(s),v);
#define _SELP_3(s,v,V...)         _SEP_2(s,v); DEFER2(_SELP_N_REF)()(TWO_ARGS(V),V);
#define SERIAL_ECHOLNPGM(V...)    do{ EVAL(_SELP_N(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values. Odd elements must be PSTR pointers.
#define __SEP_N_P(N,V...)         _SEP_##N##_P(V)
#define _SEP_N_P(N,V...)          __SEP_N_P(N,V)
#define _SEP_N_P_REF()            _SEP_N_P
#define _SEP_1_P(p)               serial_print_P(p);
#define _SEP_2_P(p,v)             serial_echopair_P(p,v);
#define _SEP_3_P(p,v,V...)        _SEP_2_P(p,v); DEFER2(_SEP_N_P_REF)()(TWO_ARGS(V),V);
#define SERIAL_ECHOPGM_P(V...)    do{ EVAL(_SEP_N_P(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values followed by newline. Odd elements must be PSTR pointers.
#define __SELP_N_P(N,V...)        _SELP_##N##_P(V)
#define _SELP_N_P(N,V...)         __SELP_N_P(N,V)
#define _SELP_N_P_REF()           _SELP_N_P
#define _SELP_1_P(p)              serial_println_P(p)
#define _SELP_2_P(p,v)            serial_echolnpair_P(p,v)
#define _SELP_3_P(p,v,V...)       { _SEP_2_P(p,v); DEFER2(_SELP_N_P_REF)()(TWO_ARGS(V),V); }
#define SERIAL_ECHOLNPGM_P(V...)  do{ EVAL(_SELP_N_P(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values. Odd elements must be FSTR_P, F(), or FPSTR().
#define __SEP_N_F(N,V...)         _SEP_##N##_F(V)
#define _SEP_N_F(N,V...)          __SEP_N_F(N,V)
#define _SEP_N_F_REF()            _SEP_N_F
#define _SEP_1_F(p)               serial_print(p);
#define _SEP_2_F(p,v)             serial_echopair(p,v);
#define _SEP_3_F(p,v,V...)        _SEP_2_F(p,v); DEFER2(_SEP_N_F_REF)()(TWO_ARGS(V),V);
#define SERIAL_ECHOF(V...)        do{ EVAL(_SEP_N_F(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values followed by newline. Odd elements must be FSTR_P, F(), or FPSTR().
#define __SELP_N_F(N,V...)        _SELP_##N##_F(V)
#define _SELP_N_F(N,V...)         __SELP_N_F(N,V)
#define _SELP_N_F_REF()           _SELP_N_F
#define _SELP_1_F(p)              serial_println(p)
#define _SELP_2_F(p,v)            serial_echolnpair(p,v)
#define _SELP_3_F(p,v,V...)       { _SEP_2_F(p,v); DEFER2(_SELP_N_F_REF)()(TWO_ARGS(V),V); }
#define SERIAL_ECHOLNF(V...)      do{ EVAL(_SELP_N_F(TWO_ARGS(V),V)); }while(0)

#ifdef AllowDifferentTypeInList

  inline void SERIAL_ECHOLIST_IMPL() {}
  template <typename T>
  void SERIAL_ECHOLIST_IMPL(T && t) { SERIAL_IMPL.print(t); }

  template <typename T, typename ... Args>
  void SERIAL_ECHOLIST_IMPL(T && t, Args && ... args) {
    SERIAL_IMPL.print(t);
    serial_print(F(", "));
    SERIAL_ECHOLIST_IMPL(args...);
  }

  template <typename ... Args>
  void SERIAL_ECHOLIST(FSTR_P const str, Args && ... args) {
    SERIAL_IMPL.print(FTOP(str));
    SERIAL_ECHOLIST_IMPL(args...);
  }

#else // Optimization if the listed type are all the same (seems to be the case in the codebase so use that instead)

  template <typename ... Args>
  void SERIAL_ECHOLIST(FSTR_P const fstr, Args && ... args) {
    serial_print(fstr);
    typename Private::first_type_of<Args...>::type values[] = { args... };
    constexpr size_t argsSize = sizeof...(args);
    for (size_t i = 0; i < argsSize; i++) {
      if (i) serial_print(F(", "));
      SERIAL_IMPL.print(values[i]);
    }
  }

#endif

// SERIAL_ECHO_F prints a floating point value with optional precision
inline void SERIAL_ECHO_F(EnsureDouble x, int digit=2) { SERIAL_IMPL.print(x, digit); }

#define SERIAL_ECHOPAIR_F_P(P,V...)   do{ serial_print_P(P); SERIAL_ECHO_F(V); }while(0)
#define SERIAL_ECHOLNPAIR_F_P(P,V...) do{ SERIAL_ECHOPAIR_F_P(P,V); SERIAL_EOL(); }while(0)

#define SERIAL_ECHOPAIR_F_F(S,V...)   do{ serial_print(S); SERIAL_ECHO_F(V); }while(0)
#define SERIAL_ECHOLNPAIR_F_F(S,V...) do{ SERIAL_ECHOPAIR_F_F(S,V); SERIAL_EOL(); }while(0)

#define SERIAL_ECHOPAIR_F(S,V...)     SERIAL_ECHOPAIR_F_F(F(S),V)
#define SERIAL_ECHOLNPAIR_F(V...)     do{ SERIAL_ECHOPAIR_F(V); SERIAL_EOL(); }while(0)

#define SERIAL_ECHO_MSG(V...)         do{ SERIAL_ECHO_START();  SERIAL_ECHOLNPGM(V); }while(0)
#define SERIAL_ERROR_MSG(V...)        do{ SERIAL_ERROR_START(); SERIAL_ECHOLNPGM(V); }while(0)

#define SERIAL_ECHO_SP(C)             serial_spaces(C)

#define SERIAL_ECHO_TERNARY(TF, PRE, ON, OFF, POST) serial_ternary(TF, F(PRE), F(ON), F(OFF), F(POST))

#if SERIAL_FLOAT_PRECISION
  #define SERIAL_DECIMAL(V) SERIAL_PRINT(V, SERIAL_FLOAT_PRECISION)
#else
  #define SERIAL_DECIMAL(V) SERIAL_ECHO(V)
#endif

//
// Functions for serial printing from PROGMEM. (Saves loads of SRAM.)
//
inline void serial_echopair_P(PGM_P const pstr, serial_char_t v) { serial_print_P(pstr); SERIAL_CHAR(v.c); }
inline void serial_echopair_P(PGM_P const pstr, float v)         { serial_print_P(pstr); SERIAL_DECIMAL(v); }
inline void serial_echopair_P(PGM_P const pstr, double v)        { serial_print_P(pstr); SERIAL_DECIMAL(v); }
//inline void serial_echopair_P(PGM_P const pstr, const char *v)   { serial_print_P(pstr); SERIAL_ECHO(v); }
inline void serial_echopair_P(PGM_P const pstr, FSTR_P v)        { serial_print_P(pstr); SERIAL_ECHOF(v); }

// Default implementation for types without a specialization. Handles integers.
template <typename T>
inline void serial_echopair_P(PGM_P const pstr, T v) { serial_print_P(pstr); SERIAL_ECHO(v); }

// Add a newline.
template <typename T>
inline void serial_echolnpair_P(PGM_P const pstr, T v) { serial_echopair_P(pstr, v); SERIAL_EOL(); }

// Catch-all for __FlashStringHelper *
template <typename T>
inline void serial_echopair(FSTR_P const fstr, T v) { serial_echopair_P(FTOP(fstr), v); }

// Add a newline to the serial output
template <typename T>
inline void serial_echolnpair(FSTR_P const fstr, T v) { serial_echolnpair_P(FTOP(fstr), v); }

void serial_echo_start();
void serial_error_start();
void serial_ternary(const bool onoff, FSTR_P const pre, FSTR_P const on, FSTR_P const off, FSTR_P const post=nullptr);
void serialprint_onoff(const bool onoff);
void serialprintln_onoff(const bool onoff);
void serialprint_truefalse(const bool tf);
void serial_spaces(uint8_t count);
void serial_offset(const_float_t v, const uint8_t sp=0); // For v==0 draw space (sp==1) or plus (sp==2)

void print_bin(const uint16_t val);
void print_pos(NUM_AXIS_ARGS(const_float_t), FSTR_P const prefix=nullptr, FSTR_P const suffix=nullptr);

inline void print_pos(const xyz_pos_t &xyz, FSTR_P const prefix=nullptr, FSTR_P const suffix=nullptr) {
  print_pos(NUM_AXIS_ELEM(xyz), prefix, suffix);
}

#define SERIAL_POS(SUFFIX,VAR) do { print_pos(VAR, F("  " STRINGIFY(VAR) "="), F(" : " SUFFIX "\n")); }while(0)
#define SERIAL_XYZ(PREFIX,V...) do { print_pos(V, F(PREFIX)); }while(0)
