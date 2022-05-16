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


#if ENABLED(BTT_TFT)

#ifdef SERIAL_PORT_2
  #warning "SERIAL_PORT_2 must be disabled is on the same port how BTT_TFT* ."
#endif



//#define BTT_TFT_UPLOAD_BAUDRATE  1958400
//#define BTT_SAFE_INSERT

#define BTT_TFT_RX_BUF_SIZE  (1024)
#define BTT_TFT_TX_BUF_SIZE  (64)

//#include "tft_lvgl_configuration.h"
#include "btt_tft_api.h"
#include "../../../src/core/serial_base.h"
#include "../../../src/core/types.h"



#ifdef __STM32F1__
  #include "bttSerial_STM32F1.h"
#else
  #include "bttSerial_STM32.h"
#endif

extern BttSerial BttSerial1;

Serial2

#ifdef LCD_SERIAL_PORT
#define BTT_TFT_SERIAL LCD_SERIAL
#define BTT_BAUDRATE LCD_BAUDRATE  
#else
#define BTT_TFT_SERIAL BttSerial1
#define BTT_BAUDRATE    BTT_TFT_BAUDRATE    
#endif

#if BTT_TFT_SERIAL_Port == 1
#define UART USART1
#define UART_NUMMER 1
#elif BTT_TFT_SERIAL_Port == 2
#define UART USART2
#define UART_NUMMER 2
#elif BTT_TFT_SERIAL_Port == 3
#define UART USART3
#define UART_NUMMER 3
#elif BTT_TFT_SERIAL_Port == 4
#define UART USART4
#define UART_NUMMER 4
#elif BTT_TFT_SERIAL_Port == 5
#define UART USART5
#define UART_NUMMER 5
#elif BTT_TFT_SERIAL_Port == 6
#define UART USART6
#define UART_NUMMER 6
#elif BTT_TFT_SERIAL_Port == 7
#define UART USART7
#define UART_NUMMER 7
#endif





#define BTT_SERIAL_IMPL BTT_TFT_SERIAL
/*
#define BTT_SERIAL_OUT(WHAT, V...)  (void)BTT_SERIAL_IMPL.WHAT(V)

#define PORT_REDIRECT(p)   _PORT_REDIRECT(1,p)
#define PORT_RESTORE()     _PORT_RESTORE(1)
#define BTT_SERIAL_PORTMASK(P) SerialMask::from(P)
*/
//
// BTT_SERIAL_CHAR - Print one or more individual chars
//
inline void BTT_SERIAL_CHAR(char a) { BTT_SERIAL_IMPL.write(a); }
template <typename ... Args>
void BTT_SERIAL_CHAR(char a, Args ... args) { BTT_SERIAL_IMPL.write(a); BTT_SERIAL_CHAR(args ...); }

/**
 * BTT_SERIAL_ECHO - Print a single string or value.
 *   Any numeric parameter (including char) is printed as a base-10 number.
 *   A string pointer or literal will be output as a string.
 *
 * NOTE: Use BTT_SERIAL_CHAR to print char as a single character.
 */
template <typename T>
void BTT_SERIAL_ECHO(T x) { BTT_SERIAL_IMPL.print(x); }

// Wrapper for ECHO commands to interpret a char
typedef struct BttSerialChar { char c; BttSerialChar(char n) : c(n) { } } btt_serial_char_t;
inline void BTT_SERIAL_ECHO(btt_serial_char_t x) { BTT_SERIAL_IMPL.write(x.c); }
#define BTT_AS_CHAR(C) btt_serial_char_t(C)
#define BTT_AS_DIGIT(C) BTT_AS_CHAR('0' + (C))

template <typename T>
void BTT_SERIAL_ECHOLN(T x) { BTT_SERIAL_IMPL.println(x); }

// BTT_SERIAL_PRINT works like BTT_SERIAL_ECHO but also takes the numeric base
template <typename T, typename U>
void BTT_SERIAL_PRINT(T x, U y) { BTT_SERIAL_IMPL.print(x, y); }

template <typename T>
void BTT_SERIAL_PRINTLN(T x, PrintBase y) { BTT_SERIAL_IMPL.println(x, y); }

// Flush the btt_serial port
inline void BTT_SERIAL_FLUSH()    { BTT_SERIAL_IMPL.flush(); }
//inline void BTT_SERIAL_FLUSHTX()  { BTT_SERIAL_IMPL.flushTX(); }

// Serial echo and error prefixes
#define BTT_SERIAL_ECHO_START()           btt_serial_echo_start()
#define BTT_SERIAL_ERROR_START()          btt_serial_error_start()

// Serial end-of-line
#define BTT_SERIAL_EOL()                  BTT_SERIAL_CHAR('\n')

// Print a single PROGMEM, PGM_P, or PSTR() string.
void btt_serial_print_P(PGM_P str);
inline void btt_serial_println_P(PGM_P str) { btt_serial_print_P(str); BTT_SERIAL_EOL(); }

// Print a single FSTR_P, F(), or FPSTR() string.
inline void btt_serial_print(FSTR_P const fstr) { btt_serial_print_P(FTOP(fstr)); }
inline void btt_serial_println(FSTR_P const fstr) { btt_serial_println_P(FTOP(fstr)); }

//
// BTT_SERIAL_ECHOPGM... macros are used to output string-value pairs.
//

// Print up to 20 pairs of values. Odd elements must be literal strings.
#define _BTT__SEP_N(N,V...)           BTT__SEP_##N(V)
#define BTT__SEP_N(N,V...)            _BTT__SEP_N(N,V)
#define BTT__SEP_N_REF()              BTT__SEP_N
#define BTT__SEP_1(s)                 btt_serial_print(F(s));
#define BTT__SEP_2(s,v)               btt_serial_echopair(F(s),v);
#define BTT__SEP_3(s,v,V...)          BTT__SEP_2(s,v); DEFER2(BTT__SEP_N_REF)()(TWO_ARGS(V),V);
#define BTT_SERIAL_ECHOPGM(V...)      do{ EVAL(BTT__SEP_N(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values followed by newline. Odd elements must be literal strings.
#define _BTT__SELP_N(N,V...)          BTT__SELP_##N(V)
#define BTT__SELP_N(N,V...)           _BTT__SELP_N(N,V)
#define BTT__SELP_N_REF()             BTT__SELP_N
#define BTT__SELP_1(s)                btt_serial_print(F(s "\n"));
#define BTT__SELP_2(s,v)              btt_serial_echolnpair(F(s),v);
#define BTT__SELP_3(s,v,V...)         BTT__SEP_2(s,v); DEFER2(BTT__SELP_N_REF)()(TWO_ARGS(V),V);
#define BTT_SERIAL_ECHOLNPGM(V...)    do{ EVAL(BTT__SELP_N(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values. Odd elements must be PSTR pointers.
#define _BTT__SEP_N_P(N,V...)         BTT__SEP_##N##_P(V)
#define BTT__SEP_N_P(N,V...)          _BTT__SEP_N_P(N,V)
#define BTT__SEP_N_P_REF()            BTT__SEP_N_P
#define BTT__SEP_1_P(p)               btt_serial_print_P(p);
#define BTT__SEP_2_P(p,v)             btt_serial_echopair_P(p,v);
#define BTT__SEP_3_P(p,v,V...)        BTT__SEP_2_P(p,v); DEFER2(BTT__SEP_N_P_REF)()(TWO_ARGS(V),V);
#define BTT_SERIAL_ECHOPGM_P(V...)    do{ EVAL(BTT__SEP_N_P(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values followed by newline. Odd elements must be PSTR pointers.
#define _BTT__SELP_N_P(N,V...)        BTT__SELP_##N##_P(V)
#define BTT__SELP_N_P(N,V...)         _BTT__SELP_N_P(N,V)
#define BTT__SELP_N_P_REF()           BTT__SELP_N_P
#define BTT__SELP_1_P(p)              btt_serial_println_P(p)
#define BTT__SELP_2_P(p,v)            btt_serial_echolnpair_P(p,v)
#define BTT__SELP_3_P(p,v,V...)       { BTT__SEP_2_P(p,v); DEFER2(BTT__SELP_N_P_REF)()(TWO_ARGS(V),V); }
#define BTT_SERIAL_ECHOLNPGM_P(V...)  do{ EVAL(BTT__SELP_N_P(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values. Odd elements must be FSTR_P, F(), or FPSTR().
#define _BTT__SEP_N_F(N,V...)         BTT__SEP_##N##_F(V)
#define BTT__SEP_N_F(N,V...)          _BTT__SEP_N_F(N,V)
#define BTT__SEP_N_F_REF()            BTT__SEP_N_F
#define BTT__SEP_1_F(p)               btt_serial_print(p);
#define BTT__SEP_2_F(p,v)             btt_serial_echopair(p,v);
#define BTT__SEP_3_F(p,v,V...)        BTT__SEP_2_F(p,v); DEFER2(BTT__SEP_N_F_REF)()(TWO_ARGS(V),V);
#define BTT_SERIAL_ECHOF(V...)        do{ EVAL(BTT__SEP_N_F(TWO_ARGS(V),V)); }while(0)

// Print up to 20 pairs of values followed by newline. Odd elements must be FSTR_P, F(), or FPSTR().
#define _BTT__SELP_N_F(N,V...)        BTT__SELP_##N##_F(V)
#define BTT__SELP_N_F(N,V...)         _BTT__SELP_N_F(N,V)
#define BTT__SELP_N_F_REF()           BTT__SELP_N_F
#define BTT__SELP_1_F(p)              btt_serial_println(p)
#define BTT__SELP_2_F(p,v)            btt_serial_echolnpair(p,v)
#define BTT__SELP_3_F(p,v,V...)       { BTT__SEP_2_F(p,v); DEFER2(BTT__SELP_N_F_REF)()(TWO_ARGS(V),V); }
#define BTT_SERIAL_ECHOLNF(V...)      do{ EVAL(BTT__SELP_N_F(TWO_ARGS(V),V)); }while(0)

#ifdef AllowDifferentTypeInList

  inline void BTT_SERIAL_ECHOLIST_IMPL() {}
  template <typename T>
  void BTT_SERIAL_ECHOLIST_IMPL(T && t) { BTT_SERIAL_IMPL.print(t); }

  template <typename T, typename ... Args>
  void BTT_SERIAL_ECHOLIST_IMPL(T && t, Args && ... args) {
    BTT_SERIAL_IMPL.print(t);
    btt_serial_print(F(", "));
    BTT_SERIAL_ECHOLIST_IMPL(args...);
  }

  template <typename ... Args>
  void BTT_SERIAL_ECHOLIST(FSTR_P const str, Args && ... args) {
    BTT_SERIAL_IMPL.print(FTOP(str));
    BTT_SERIAL_ECHOLIST_IMPL(args...);
  }

#else // Optimization if the listed type are all the same (seems to be the case in the codebase so use that instead)

  template <typename ... Args>
  void BTT_SERIAL_ECHOLIST(FSTR_P const fstr, Args && ... args) {
    btt_serial_print(fstr);
    typename Private::first_type_of<Args...>::type values[] = { args... };
    constexpr size_t argsSize = sizeof...(args);
    for (size_t i = 0; i < argsSize; i++) {
      if (i) btt_serial_print(F(", "));
      BTT_SERIAL_IMPL.print(values[i]);
    }
  }

#endif

// BTT_SERIAL_ECHO_F prints a floating point value with optional precision
inline void BTT_SERIAL_ECHO_F(EnsureDouble x, int digit=2) { BTT_SERIAL_IMPL.print(x, digit); }

#define BTT_SERIAL_ECHOPAIR_F_P(P,V...)   do{ btt_serial_print_P(P); BTT_SERIAL_ECHO_F(V); }while(0)
#define BTT_SERIAL_ECHOLNPAIR_F_P(P,V...) do{ BTT_SERIAL_ECHOPAIR_F_P(P,V); BTT_SERIAL_EOL(); }while(0)

#define BTT_SERIAL_ECHOPAIR_F_F(S,V...)   do{ btt_serial_print(S); BTT_SERIAL_ECHO_F(V); }while(0)
#define BTT_SERIAL_ECHOLNPAIR_F_F(S,V...) do{ BTT_SERIAL_ECHOPAIR_F_F(S,V); BTT_SERIAL_EOL(); }while(0)

#define BTT_SERIAL_ECHOPAIR_F(S,V...)     BTT_SERIAL_ECHOPAIR_F_F(F(S),V)
#define BTT_SERIAL_ECHOLNPAIR_F(V...)     do{ BTT_SERIAL_ECHOPAIR_F(V); BTT_SERIAL_EOL(); }while(0)

#define BTT_SERIAL_ECHO_MSG(V...)         do{ BTT_SERIAL_ECHO_START();  BTT_SERIAL_ECHOLNPGM(V); }while(0)
#define BTT_SERIAL_ERROR_MSG(V...)        do{ BTT_SERIAL_ERROR_START(); BTT_SERIAL_ECHOLNPGM(V); }while(0)

#define BTT_SERIAL_ECHO_SP(C)             btt_serial_spaces(C)

#define BTT_SERIAL_ECHO_TERNARY(TF, PRE, ON, OFF, POST) btt_serial_ternary(TF, F(PRE), F(ON), F(OFF), F(POST))

#if BTT_SERIAL_FLOAT_PRECISION
  #define BTT_SERIAL_DECIMAL(V) BTT_SERIAL_PRINT(V, BTT_SERIAL_FLOAT_PRECISION)
#else
  #define BTT_SERIAL_DECIMAL(V) BTT_SERIAL_ECHO(V)
#endif

//
// Functions for btt_serial printing from PROGMEM. (Saves loads of SRAM.)
//
inline void btt_serial_echopair_P(PGM_P const pstr, btt_serial_char_t v) { btt_serial_print_P(pstr); BTT_SERIAL_CHAR(v.c); }
inline void btt_serial_echopair_P(PGM_P const pstr, float v)         { btt_serial_print_P(pstr); BTT_SERIAL_DECIMAL(v); }
inline void btt_serial_echopair_P(PGM_P const pstr, double v)        { btt_serial_print_P(pstr); BTT_SERIAL_DECIMAL(v); }
//inline void btt_serial_echopair_P(PGM_P const pstr, const char *v)   { btt_serial_print_P(pstr); BTT_SERIAL_ECHO(v); }
inline void btt_serial_echopair_P(PGM_P const pstr, FSTR_P v)        { btt_serial_print_P(pstr); BTT_SERIAL_ECHOF(v); }

// Default implementation for types without a specialization. Handles integers.
template <typename T>
inline void btt_serial_echopair_P(PGM_P const pstr, T v) { btt_serial_print_P(pstr); BTT_SERIAL_ECHO(v); }

// Add a newline.
template <typename T>
inline void btt_serial_echolnpair_P(PGM_P const pstr, T v) { btt_serial_echopair_P(pstr, v); BTT_SERIAL_EOL(); }

// Catch-all for __FlashStringHelper *
template <typename T>
inline void btt_serial_echopair(FSTR_P const fstr, T v) { btt_serial_echopair_P(FTOP(fstr), v); }

// Add a newline to the btt_serial output
template <typename T>
inline void btt_serial_echolnpair(FSTR_P const fstr, T v) { btt_serial_echolnpair_P(FTOP(fstr), v); }

void btt_serial_echo_start();
void btt_serial_error_start();
void btt_serial_ternary(const bool onoff, FSTR_P const pre, FSTR_P const on, FSTR_P const off, FSTR_P const post=nullptr);
void btt_serialprint_onoff(const bool onoff);
void btt_serialprintln_onoff(const bool onoff);
void btt_serialprint_truefalse(const bool tf);
void btt_serial_spaces(uint8_t count);

void btt_print_bin(const uint16_t val);
void btt_print_pos(LINEAR_AXIS_ARGS(const_float_t), FSTR_P const prefix=nullptr, FSTR_P const suffix=nullptr);

inline void btt_print_pos(const xyz_pos_t &xyz, FSTR_P const prefix=nullptr, FSTR_P const suffix=nullptr) {
  btt_print_pos(LINEAR_AXIS_ELEM(xyz), prefix, suffix);
}

#define BTT_SERIAL_POS(SUFFIX,VAR) do { btt_print_pos(VAR, F("  " STRINGIFY(VAR) "="), F(" : " SUFFIX "\n")); }while(0)
#define BTT_SERIAL_XYZ(PREFIX,V...) do { btt_print_pos(V, F(PREFIX)); }while(0)


#endif