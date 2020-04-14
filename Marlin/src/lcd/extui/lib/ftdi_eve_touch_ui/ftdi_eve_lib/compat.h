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
 *   location: <http://www.gnu.org/licenses/>.                              *
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

  #define SERIAL_ECHO_START()
  #define SERIAL_ECHOLNPGM(str)       Serial.println(F(str))
  #define SERIAL_ECHOPGM(str)         Serial.print(F(str))
  #define SERIAL_ECHO_MSG(str)        Serial.println(str)
  #define SERIAL_ECHOLNPAIR(str, val) do{ Serial.print(F(str)); Serial.println(val); }while(0)
  #define SERIAL_ECHOPAIR(str, val)   do{ Serial.print(F(str)); Serial.print(val); }while(0)

  #define safe_delay delay

  // Define macros for compatibility

  #define _CAT(a, ...)       a ## __VA_ARGS__
  #define SWITCH_ENABLED_    1
  #define ENABLED(b)         _CAT(SWITCH_ENABLED_, b)
  #define DISABLED(b)        !ENABLED(b)
  #define ANY(A,B)           (ENABLED(A) || ENABLED(B))
  #define EITHER(A,B)        (ENABLED(A) || ENABLED(B))
  #define BOTH(A,B)          (ENABLED(A) && ENABLED(B))
  #define NONE(A,B)          (DISABLED(A) && DISABLED(B))

  // Remove compiler warning on an unused variable
  #ifndef UNUSED
    #if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)
      #define UNUSED(X) (void)X
    #else
      #define UNUSED(x) ((void)(x))
    #endif
  #endif

#endif // !__MARLIN_FIRMWARE__
