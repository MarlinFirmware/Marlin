/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#include "../config.h"

#ifdef __MARLIN_FIRMWARE__

  // Marlin will define the I/O functions for us
  #if ENABLED(TOUCH_UI_FTDI_EVE)
    #define FTDI_BASIC
    #define FTDI_EXTENDED
  #endif

#else // !__MARLIN_FIRMWARE__

  #include <Arduino.h>

  #ifndef CLCD_USE_SOFT_SPI
    #include <SPI.h>
  #endif

  namespace fast_io {

    template<typename port_t,uint8_t bits>
    struct port_pin {
      typedef port_t port;
      static inline void set_high()         {port::port() = (port::port() |   bits);}
      static inline void set_low()          {port::port() = (port::port() & (~bits));}
      static inline void set_input()        {port::ddr()  = (port::ddr()  & (~bits));}
      static inline void set_input_pullup() {set_input(); set_high();}
      static inline void set_output()       {port::ddr()  = (port::ddr()  |   bits);}
      static inline uint8_t read()          {return port::pin() & bits;}
      static inline void write(bool v)      {if (v) set_high(); else set_low();}
    };

    #define MAKE_AVR_PORT_PINS(ID) \
      struct port_##ID { \
        static volatile uint8_t &pin()  {return PIN##ID;}; \
        static volatile uint8_t &port() {return PORT##ID;}; \
        static volatile uint8_t &ddr()  {return DDR##ID;}; \
      }; \
      typedef port_pin<port_##ID, 0b00000001> AVR_##ID##0; \
      typedef port_pin<port_##ID, 0b00000010> AVR_##ID##1; \
      typedef port_pin<port_##ID, 0b00000100> AVR_##ID##2; \
      typedef port_pin<port_##ID, 0b00001000> AVR_##ID##3; \
      typedef port_pin<port_##ID, 0b00010000> AVR_##ID##4; \
      typedef port_pin<port_##ID, 0b00100000> AVR_##ID##5; \
      typedef port_pin<port_##ID, 0b01000000> AVR_##ID##6; \
      typedef port_pin<port_##ID, 0b10000000> AVR_##ID##7;

    #ifdef PORTA
      MAKE_AVR_PORT_PINS(A);
    #endif
    #ifdef PORTB
      MAKE_AVR_PORT_PINS(B);
    #endif
    #ifdef PORTC
      MAKE_AVR_PORT_PINS(C);
    #endif
    #ifdef PORTD
      MAKE_AVR_PORT_PINS(D);
    #endif
    #ifdef PORTE
      MAKE_AVR_PORT_PINS(E);
    #endif
    #ifdef PORTF
      MAKE_AVR_PORT_PINS(F);
    #endif
    #ifdef PORTG
      MAKE_AVR_PORT_PINS(G);
    #endif
    #ifdef PORTH
      MAKE_AVR_PORT_PINS(H);
    #endif
    #ifdef PORTJ
      MAKE_AVR_PORT_PINS(J);
    #endif
    #ifdef PORTK
      MAKE_AVR_PORT_PINS(K);
    #endif
    #ifdef PORTL
      MAKE_AVR_PORT_PINS(L);
    #endif
    #ifdef PORTQ
      MAKE_AVR_PORT_PINS(Q);
    #endif
    #ifdef PORTR
      MAKE_AVR_PORT_PINS(R);
    #endif

    #undef MAKE_AVR_PORT_PINS

    template<uint8_t p>
    struct arduino_digital_pin {
      static constexpr uint8_t pin = p;
      static inline void set_high()          {digitalWrite(p, HIGH);}
      static inline void set_low()           {digitalWrite(p, LOW);}
      static inline void set_input()         {pinMode(p, INPUT);}
      static inline void set_input_pullup()  {pinMode(p, INPUT_PULLUP);}
      static inline void set_output()        {pinMode(p, OUTPUT);}
      static inline uint8_t read()           {return digitalRead(p);}
      static inline void write(bool v)       {digitalWrite(p, v ? HIGH : LOW);}
    };

    #define MAKE_ARDUINO_PINS(ID) typedef arduino_digital_pin<ID> ARDUINO_DIGITAL_##ID;
    MAKE_ARDUINO_PINS( 0);
    MAKE_ARDUINO_PINS( 1);
    MAKE_ARDUINO_PINS( 2);
    MAKE_ARDUINO_PINS( 3);
    MAKE_ARDUINO_PINS( 4);
    MAKE_ARDUINO_PINS( 5);
    MAKE_ARDUINO_PINS( 6);
    MAKE_ARDUINO_PINS( 7);
    MAKE_ARDUINO_PINS( 8);
    MAKE_ARDUINO_PINS( 9);
    MAKE_ARDUINO_PINS(10);
    MAKE_ARDUINO_PINS(11);
    MAKE_ARDUINO_PINS(12);
    MAKE_ARDUINO_PINS(13);
    MAKE_ARDUINO_PINS(14);
    MAKE_ARDUINO_PINS(15);
    MAKE_ARDUINO_PINS(16);
    MAKE_ARDUINO_PINS(17);
    MAKE_ARDUINO_PINS(18);
    MAKE_ARDUINO_PINS(19);
    MAKE_ARDUINO_PINS(10);
    MAKE_ARDUINO_PINS(21);
    MAKE_ARDUINO_PINS(22);
    MAKE_ARDUINO_PINS(23);
    MAKE_ARDUINO_PINS(24);
    MAKE_ARDUINO_PINS(25);
    MAKE_ARDUINO_PINS(26);
    MAKE_ARDUINO_PINS(27);
    MAKE_ARDUINO_PINS(28);
    MAKE_ARDUINO_PINS(29);
    MAKE_ARDUINO_PINS(30);
    MAKE_ARDUINO_PINS(31);
    MAKE_ARDUINO_PINS(32);
    MAKE_ARDUINO_PINS(33);
    MAKE_ARDUINO_PINS(34);
    MAKE_ARDUINO_PINS(35);
    MAKE_ARDUINO_PINS(36);
    MAKE_ARDUINO_PINS(37);
    MAKE_ARDUINO_PINS(38);
    MAKE_ARDUINO_PINS(39);
    MAKE_ARDUINO_PINS(40);
    MAKE_ARDUINO_PINS(41);
    MAKE_ARDUINO_PINS(42);
    MAKE_ARDUINO_PINS(43);
    MAKE_ARDUINO_PINS(44);
    MAKE_ARDUINO_PINS(45);
    MAKE_ARDUINO_PINS(46);
    MAKE_ARDUINO_PINS(47);
    MAKE_ARDUINO_PINS(48);
    MAKE_ARDUINO_PINS(49);
    MAKE_ARDUINO_PINS(50);
    MAKE_ARDUINO_PINS(51);
    MAKE_ARDUINO_PINS(52);
    MAKE_ARDUINO_PINS(53);
    #undef MAKE_ARDUINO_PINS
  } // namespace fast_io

  #define SET_INPUT(pin)              fast_io::pin::set_input()
  #define SET_INPUT_PULLUP(pin)       do{ fast_io::pin::set_input(); fast_io::pin::set_high(); }while(0)
  #define SET_INPUT_PULLDOWN          SET_INPUT
  #define SET_OUTPUT(pin)             fast_io::pin::set_output()
  #define READ(pin)                   fast_io::pin::read()
  #define WRITE(pin, value)           fast_io::pin::write(value)

  #ifndef pgm_read_word_far
  #define pgm_read_word_far pgm_read_word
  #endif

  #ifndef pgm_read_dword_far
  #define pgm_read_dword_far pgm_read_dword
  #endif

  #ifndef pgm_read_ptr_far
  #define pgm_read_ptr_far pgm_read_ptr
  #endif

  // Use NUM_ARGS(__VA_ARGS__) to get the number of variadic arguments
  #define _NUM_ARGS(_,Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A,OUT,...) OUT
  #define NUM_ARGS(V...) _NUM_ARGS(0,V,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

  // SERIAL_ECHOPGM / SERIAL_ECHOPGM_P is used to output a key value pair. The key must be a string and the value can be anything
  // Print up to 12 pairs of values. Odd elements auto-wrapped in PSTR().
  #define __SEP_N(N,V...)   _SEP_##N(V)
  #define _SEP_N(N,V...)    __SEP_N(N,V)
  #define _SEP_1(PRE)       SERIAL_ECHOPGM(PRE)
  #define _SEP_2(PRE,V)     do{ Serial.print(F(PRE)); Serial.print(V); }while(0)
  #define _SEP_3(a,b,c)     do{ _SEP_2(a,b); SERIAL_ECHOPGM(c); }while(0)
  #define _SEP_4(a,b,V...)  do{ _SEP_2(a,b); _SEP_2(V); }while(0)

  // Print up to 1 pairs of values followed by newline
  #define __SELP_N(N,V...)            _SELP_##N(V)
  #define _SELP_N(N,V...)             __SELP_N(N,V)
  #define _SELP_1(PRE)                SERIAL_ECHOLNPGM(PRE)
  #define _SELP_2(PRE,V)              do{ Serial.print(F(PRE)); Serial.println(V); }while(0)
  #define _SELP_3(a,b,c)              do{ _SEP_2(a,b); SERIAL_ECHOLNPGM(c); }while(0)
  #define _SELP_4(a,b,V...)           do{ _SEP_2(a,b); _SELP_2(V); }while(0)
  #define SERIAL_ECHO_START()
  #define SERIAL_ECHOLNPGM(str)       Serial.println(F(str))
  #define SERIAL_ECHOPGM(str)         Serial.print(F(str))
  #define SERIAL_ECHO_MSG(V...)       SERIAL_ECHOLNPGM(V)
  #define SERIAL_ECHOLNPGM(V...)     _SELP_N(NUM_ARGS(V),V)
  #define SERIAL_ECHOPGM(str, val)   do{ Serial.print(F(str)); Serial.print(val); }while(0)

  #define safe_delay delay

  // Define macros for compatibility

  // Use NUM_ARGS(__VA_ARGS__) to get the number of variadic arguments
  #define _NUM_ARGS(_,Z,Y,X,W,V,U,T,S,R,Q,P,O,N,M,L,K,J,I,H,G,F,E,D,C,B,A,OUT,...) OUT
  #define NUM_ARGS(V...) _NUM_ARGS(0,V,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

  #define _CAT(a,V...) a##V
  #define CAT(a,V...) _CAT(a,V)

  #define FIRST(a,...)     a
  #define SECOND(a,b,...)  b
  #define THIRD(a,b,c,...) c

  #define IS_PROBE(V...) SECOND(V, 0)     // Get the second item passed, or 0
  #define PROBE() ~, 1                    // Second item will be 1 if this is passed
  #define _NOT_0 PROBE()
  #define NOT(x) IS_PROBE(_CAT(_NOT_, x)) // NOT('0') gets '1'. Anything else gets '0'.
  #define _BOOL(x) NOT(NOT(x))            // NOT('0') gets '0'. Anything else gets '1'.

  #define _DO_1(W,C,A)       (_##W##_1(A))
  #define _DO_2(W,C,A,B)     (_##W##_1(A) C _##W##_1(B))
  #define _DO_3(W,C,A,V...)  (_##W##_1(A) C _DO_2(W,C,V))
  #define _DO_4(W,C,A,V...)  (_##W##_1(A) C _DO_3(W,C,V))
  #define _DO_5(W,C,A,V...)  (_##W##_1(A) C _DO_4(W,C,V))
  #define _DO_6(W,C,A,V...)  (_##W##_1(A) C _DO_5(W,C,V))
  #define _DO_7(W,C,A,V...)  (_##W##_1(A) C _DO_6(W,C,V))
  #define _DO_8(W,C,A,V...)  (_##W##_1(A) C _DO_7(W,C,V))
  #define _DO_9(W,C,A,V...)  (_##W##_1(A) C _DO_8(W,C,V))
  #define _DO_10(W,C,A,V...) (_##W##_1(A) C _DO_9(W,C,V))
  #define _DO_11(W,C,A,V...) (_##W##_1(A) C _DO_10(W,C,V))
  #define _DO_12(W,C,A,V...) (_##W##_1(A) C _DO_11(W,C,V))
  #define _DO_13(W,C,A,V...) (_##W##_1(A) C _DO_12(W,C,V))
  #define _DO_14(W,C,A,V...) (_##W##_1(A) C _DO_13(W,C,V))
  #define _DO_15(W,C,A,V...) (_##W##_1(A) C _DO_14(W,C,V))
  #define _DO_16(W,C,A,V...) (_##W##_1(A) C _DO_15(W,C,V))
  #define _DO_17(W,C,A,V...) (_##W##_1(A) C _DO_16(W,C,V))
  #define _DO_18(W,C,A,V...) (_##W##_1(A) C _DO_17(W,C,V))
  #define _DO_19(W,C,A,V...) (_##W##_1(A) C _DO_18(W,C,V))
  #define _DO_20(W,C,A,V...) (_##W##_1(A) C _DO_19(W,C,V))
  #define _DO_21(W,C,A,V...) (_##W##_1(A) C _DO_20(W,C,V))
  #define _DO_22(W,C,A,V...) (_##W##_1(A) C _DO_21(W,C,V))
  #define _DO_23(W,C,A,V...) (_##W##_1(A) C _DO_22(W,C,V))
  #define _DO_24(W,C,A,V...) (_##W##_1(A) C _DO_23(W,C,V))
  #define _DO_25(W,C,A,V...) (_##W##_1(A) C _DO_24(W,C,V))
  #define _DO_26(W,C,A,V...) (_##W##_1(A) C _DO_25(W,C,V))
  #define _DO_27(W,C,A,V...) (_##W##_1(A) C _DO_26(W,C,V))
  #define _DO_28(W,C,A,V...) (_##W##_1(A) C _DO_27(W,C,V))
  #define _DO_29(W,C,A,V...) (_##W##_1(A) C _DO_28(W,C,V))
  #define _DO_30(W,C,A,V...) (_##W##_1(A) C _DO_29(W,C,V))
  #define _DO_31(W,C,A,V...) (_##W##_1(A) C _DO_30(W,C,V))
  #define _DO_32(W,C,A,V...) (_##W##_1(A) C _DO_31(W,C,V))
  #define _DO_33(W,C,A,V...) (_##W##_1(A) C _DO_32(W,C,V))
  #define _DO_34(W,C,A,V...) (_##W##_1(A) C _DO_33(W,C,V))
  #define _DO_35(W,C,A,V...) (_##W##_1(A) C _DO_34(W,C,V))
  #define _DO_36(W,C,A,V...) (_##W##_1(A) C _DO_35(W,C,V))
  #define _DO_37(W,C,A,V...) (_##W##_1(A) C _DO_36(W,C,V))
  #define _DO_38(W,C,A,V...) (_##W##_1(A) C _DO_37(W,C,V))
  #define _DO_39(W,C,A,V...) (_##W##_1(A) C _DO_38(W,C,V))
  #define _DO_40(W,C,A,V...) (_##W##_1(A) C _DO_39(W,C,V))
  #define __DO_N(W,C,N,V...) _DO_##N(W,C,V)
  #define _DO_N(W,C,N,V...)  __DO_N(W,C,N,V)
  #define DO(W,C,V...)       _DO_N(W,C,NUM_ARGS(V),V)

  #define _ISENA_     ~,1
  #define _ISENA_1    ~,1
  #define _ISENA_0x1  ~,1
  #define _ISENA_true ~,1
  #define _ISENA(V...)        IS_PROBE(V)
  #define _ENA_1(O)           _ISENA(CAT(_IS,CAT(ENA_, O)))
  #define _DIS_1(O)           NOT(_ENA_1(O))
  #define ENABLED(V...)       DO(ENA,&&,V)
  #define DISABLED(V...)      DO(DIS,&&,V)

  #define TERN(O,A,B)         _TERN(_ENA_1(O),B,A)    // OPTION converted to '0' or '1'
  #define TERN0(O,A)          _TERN(_ENA_1(O),0,A)    // OPTION converted to A or '0'
  #define TERN1(O,A)          _TERN(_ENA_1(O),1,A)    // OPTION converted to A or '1'
  #define TERN_(O,A)          _TERN(_ENA_1(O),,A)     // OPTION converted to A or '<nul>'
  #define _TERN(E,V...)       __TERN(_CAT(T_,E),V)    // Prepend 'T_' to get 'T_0' or 'T_1'
  #define __TERN(T,V...)      ___TERN(_CAT(_NO,T),V)  // Prepend '_NO' to get '_NOT_0' or '_NOT_1'
  #define ___TERN(P,V...)     THIRD(P,V)              // If first argument has a comma, A. Else B.

  #define IF_ENABLED          TERN_
  #define IF_DISABLED(O,A)    _TERN(_ENA_1(O),,A)

  #define ANY(V...)          !DISABLED(V)
  #define NONE(V...)          DISABLED(V)
  #define ALL(V...)           ENABLED(V)
  #define BOTH(V1,V2)         ALL(V1,V2)
  #define EITHER(V1,V2)       ANY(V1,V2)

  // Remove compiler warning on an unused variable
  #ifndef UNUSED
    #ifdef HAL_STM32
      #define UNUSED(X) (void)X
    #else
      #define UNUSED(x) ((void)(x))
    #endif
  #endif

#endif // !__MARLIN_FIRMWARE__

#ifndef SD_SPI_SPEED
  #define SD_SPI_SPEED SPI_FULL_SPEED
#endif
