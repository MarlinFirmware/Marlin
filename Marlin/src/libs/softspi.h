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

//
// Based on https://github.com/niteris/ArduinoSoftSpi
//

#include "../HAL/shared/Marduino.h" // CORE_TEENSY

#define nop __asm__ volatile ("nop") // NOP for timing

#ifdef __arm__

  #ifdef CORE_TEENSY

    /**
     * Read pin value
     * @param[in] pin Arduino pin number
     * @return value read
     */
    FORCE_INLINE static bool fastDigitalRead(uint8_t pin) {
      return *portInputRegister(pin);
    }

    /**
     * Set pin value
     * @param[in] pin Arduino pin number
     * @param[in] level value to write
     */
    FORCE_INLINE static void fastDigitalWrite(uint8_t pin, bool value) {
      if (value)
        *portSetRegister(pin) = 1;
      else
        *portClearRegister(pin) = 1;
    }

  #else // !CORE_TEENSY

    /**
     * Read pin value
     * @param[in] pin Arduino pin number
     * @return value read
     */
    FORCE_INLINE static bool fastDigitalRead(uint8_t pin) {
      return digitalRead(pin);
    }

    /**
     * Set pin value
     * @param[in] pin Arduino pin number
     * @param[in] level value to write
     */
    FORCE_INLINE static void fastDigitalWrite(uint8_t pin, bool value) {
      digitalWrite(pin, value);
    }

  #endif // !CORE_TEENSY

  inline void fastDigitalToggle(uint8_t pin) { fastDigitalWrite(pin, !fastDigitalRead(pin)); }

  inline void fastPinMode(uint8_t pin, bool mode) { pinMode(pin, mode); }

#else  // !__arm__

  #include <avr/io.h>
  #include <util/atomic.h>

  /**
   * @class pin_map_t
   * @brief struct for mapping digital pins
   */
  struct pin_map_t {
    volatile uint8_t* ddr;   /**< address of DDR for this pin */
    volatile uint8_t* pin;   /**< address of PIN for this pin */
    volatile uint8_t* port;  /**< address of PORT for this pin */
    uint8_t bit;             /**< bit number for this pin */
  };

  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)

    // 168 and 328 Arduinos
    const static pin_map_t pinMap[] = {
      {&DDRD, &PIND, &PORTD, 0},  // D0  0
      {&DDRD, &PIND, &PORTD, 1},  // D1  1
      {&DDRD, &PIND, &PORTD, 2},  // D2  2
      {&DDRD, &PIND, &PORTD, 3},  // D3  3
      {&DDRD, &PIND, &PORTD, 4},  // D4  4
      {&DDRD, &PIND, &PORTD, 5},  // D5  5
      {&DDRD, &PIND, &PORTD, 6},  // D6  6
      {&DDRD, &PIND, &PORTD, 7},  // D7  7
      {&DDRB, &PINB, &PORTB, 0},  // B0  8
      {&DDRB, &PINB, &PORTB, 1},  // B1  9
      {&DDRB, &PINB, &PORTB, 2},  // B2 10
      {&DDRB, &PINB, &PORTB, 3},  // B3 11
      {&DDRB, &PINB, &PORTB, 4},  // B4 12
      {&DDRB, &PINB, &PORTB, 5},  // B5 13
      {&DDRC, &PINC, &PORTC, 0},  // C0 14
      {&DDRC, &PINC, &PORTC, 1},  // C1 15
      {&DDRC, &PINC, &PORTC, 2},  // C2 16
      {&DDRC, &PINC, &PORTC, 3},  // C3 17
      {&DDRC, &PINC, &PORTC, 4},  // C4 18
      {&DDRC, &PINC, &PORTC, 5}   // C5 19
    };

  #elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    // Mega
    static const pin_map_t pinMap[] = {
      {&DDRE, &PINE, &PORTE, 0},  // E0  0
      {&DDRE, &PINE, &PORTE, 1},  // E1  1
      {&DDRE, &PINE, &PORTE, 4},  // E4  2
      {&DDRE, &PINE, &PORTE, 5},  // E5  3
      {&DDRG, &PING, &PORTG, 5},  // G5  4
      {&DDRE, &PINE, &PORTE, 3},  // E3  5
      {&DDRH, &PINH, &PORTH, 3},  // H3  6
      {&DDRH, &PINH, &PORTH, 4},  // H4  7
      {&DDRH, &PINH, &PORTH, 5},  // H5  8
      {&DDRH, &PINH, &PORTH, 6},  // H6  9
      {&DDRB, &PINB, &PORTB, 4},  // B4 10
      {&DDRB, &PINB, &PORTB, 5},  // B5 11
      {&DDRB, &PINB, &PORTB, 6},  // B6 12
      {&DDRB, &PINB, &PORTB, 7},  // B7 13
      {&DDRJ, &PINJ, &PORTJ, 1},  // J1 14
      {&DDRJ, &PINJ, &PORTJ, 0},  // J0 15
      {&DDRH, &PINH, &PORTH, 1},  // H1 16
      {&DDRH, &PINH, &PORTH, 0},  // H0 17
      {&DDRD, &PIND, &PORTD, 3},  // D3 18
      {&DDRD, &PIND, &PORTD, 2},  // D2 19
      {&DDRD, &PIND, &PORTD, 1},  // D1 20
      {&DDRD, &PIND, &PORTD, 0},  // D0 21
      {&DDRA, &PINA, &PORTA, 0},  // A0 22
      {&DDRA, &PINA, &PORTA, 1},  // A1 23
      {&DDRA, &PINA, &PORTA, 2},  // A2 24
      {&DDRA, &PINA, &PORTA, 3},  // A3 25
      {&DDRA, &PINA, &PORTA, 4},  // A4 26
      {&DDRA, &PINA, &PORTA, 5},  // A5 27
      {&DDRA, &PINA, &PORTA, 6},  // A6 28
      {&DDRA, &PINA, &PORTA, 7},  // A7 29
      {&DDRC, &PINC, &PORTC, 7},  // C7 30
      {&DDRC, &PINC, &PORTC, 6},  // C6 31
      {&DDRC, &PINC, &PORTC, 5},  // C5 32
      {&DDRC, &PINC, &PORTC, 4},  // C4 33
      {&DDRC, &PINC, &PORTC, 3},  // C3 34
      {&DDRC, &PINC, &PORTC, 2},  // C2 35
      {&DDRC, &PINC, &PORTC, 1},  // C1 36
      {&DDRC, &PINC, &PORTC, 0},  // C0 37
      {&DDRD, &PIND, &PORTD, 7},  // D7 38
      {&DDRG, &PING, &PORTG, 2},  // G2 39
      {&DDRG, &PING, &PORTG, 1},  // G1 40
      {&DDRG, &PING, &PORTG, 0},  // G0 41
      {&DDRL, &PINL, &PORTL, 7},  // L7 42
      {&DDRL, &PINL, &PORTL, 6},  // L6 43
      {&DDRL, &PINL, &PORTL, 5},  // L5 44
      {&DDRL, &PINL, &PORTL, 4},  // L4 45
      {&DDRL, &PINL, &PORTL, 3},  // L3 46
      {&DDRL, &PINL, &PORTL, 2},  // L2 47
      {&DDRL, &PINL, &PORTL, 1},  // L1 48
      {&DDRL, &PINL, &PORTL, 0},  // L0 49
      {&DDRB, &PINB, &PORTB, 3},  // B3 50
      {&DDRB, &PINB, &PORTB, 2},  // B2 51
      {&DDRB, &PINB, &PORTB, 1},  // B1 52
      {&DDRB, &PINB, &PORTB, 0},  // B0 53
      {&DDRF, &PINF, &PORTF, 0},  // F0 54
      {&DDRF, &PINF, &PORTF, 1},  // F1 55
      {&DDRF, &PINF, &PORTF, 2},  // F2 56
      {&DDRF, &PINF, &PORTF, 3},  // F3 57
      {&DDRF, &PINF, &PORTF, 4},  // F4 58
      {&DDRF, &PINF, &PORTF, 5},  // F5 59
      {&DDRF, &PINF, &PORTF, 6},  // F6 60
      {&DDRF, &PINF, &PORTF, 7},  // F7 61
      {&DDRK, &PINK, &PORTK, 0},  // K0 62
      {&DDRK, &PINK, &PORTK, 1},  // K1 63
      {&DDRK, &PINK, &PORTK, 2},  // K2 64
      {&DDRK, &PINK, &PORTK, 3},  // K3 65
      {&DDRK, &PINK, &PORTK, 4},  // K4 66
      {&DDRK, &PINK, &PORTK, 5},  // K5 67
      {&DDRK, &PINK, &PORTK, 6},  // K6 68
      {&DDRK, &PINK, &PORTK, 7},  // K7 69

      // pins_MIGHTYBOARD_REVE.h
      {&DDRG, &PING, &PORTG, 4},  // G4 70
      {&DDRG, &PING, &PORTG, 3},  // G3 71
      {&DDRJ, &PINJ, &PORTJ, 2},  // J2 72
      {&DDRJ, &PINJ, &PORTJ, 3},  // J3 73
      {&DDRJ, &PINJ, &PORTJ, 7},  // J7 74
      {&DDRJ, &PINJ, &PORTJ, 4},  // J4 75
      {&DDRJ, &PINJ, &PORTJ, 5},  // J5 76
      {&DDRJ, &PINJ, &PORTJ, 6},  // J6 77
      {&DDRE, &PINE, &PORTE, 2},  // E2 78
      {&DDRE, &PINE, &PORTE, 6}   // E6 79
    };

  #elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1284__) \
  || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) \
  || defined(__AVR_ATmega64__) || defined(__AVR_ATmega32__) \
  || defined(__AVR_ATmega324__) || defined(__AVR_ATmega16__)

    #ifdef VARIANT_MIGHTY

      // Mighty Layout
      static const pin_map_t pinMap[] = {
        {&DDRB, &PINB, &PORTB, 0},  // B0  0
        {&DDRB, &PINB, &PORTB, 1},  // B1  1
        {&DDRB, &PINB, &PORTB, 2},  // B2  2
        {&DDRB, &PINB, &PORTB, 3},  // B3  3
        {&DDRB, &PINB, &PORTB, 4},  // B4  4
        {&DDRB, &PINB, &PORTB, 5},  // B5  5
        {&DDRB, &PINB, &PORTB, 6},  // B6  6
        {&DDRB, &PINB, &PORTB, 7},  // B7  7
        {&DDRD, &PIND, &PORTD, 0},  // D0  8
        {&DDRD, &PIND, &PORTD, 1},  // D1  9
        {&DDRD, &PIND, &PORTD, 2},  // D2 10
        {&DDRD, &PIND, &PORTD, 3},  // D3 11
        {&DDRD, &PIND, &PORTD, 4},  // D4 12
        {&DDRD, &PIND, &PORTD, 5},  // D5 13
        {&DDRD, &PIND, &PORTD, 6},  // D6 14
        {&DDRD, &PIND, &PORTD, 7},  // D7 15
        {&DDRC, &PINC, &PORTC, 0},  // C0 16
        {&DDRC, &PINC, &PORTC, 1},  // C1 17
        {&DDRC, &PINC, &PORTC, 2},  // C2 18
        {&DDRC, &PINC, &PORTC, 3},  // C3 19
        {&DDRC, &PINC, &PORTC, 4},  // C4 20
        {&DDRC, &PINC, &PORTC, 5},  // C5 21
        {&DDRC, &PINC, &PORTC, 6},  // C6 22
        {&DDRC, &PINC, &PORTC, 7},  // C7 23
        {&DDRA, &PINA, &PORTA, 0},  // A0 24
        {&DDRA, &PINA, &PORTA, 1},  // A1 25
        {&DDRA, &PINA, &PORTA, 2},  // A2 26
        {&DDRA, &PINA, &PORTA, 3},  // A3 27
        {&DDRA, &PINA, &PORTA, 4},  // A4 28
        {&DDRA, &PINA, &PORTA, 5},  // A5 29
        {&DDRA, &PINA, &PORTA, 6},  // A6 30
        {&DDRA, &PINA, &PORTA, 7}   // A7 31
      };

    #elif defined(VARIANT_BOBUINO)

      // Bobuino Layout
      static const pin_map_t pinMap[] = {
        {&DDRD, &PIND, &PORTD, 0},  // D0  0
        {&DDRD, &PIND, &PORTD, 1},  // D1  1
        {&DDRD, &PIND, &PORTD, 2},  // D2  2
        {&DDRD, &PIND, &PORTD, 3},  // D3  3
        {&DDRB, &PINB, &PORTB, 0},  // B0  4
        {&DDRB, &PINB, &PORTB, 1},  // B1  5
        {&DDRB, &PINB, &PORTB, 2},  // B2  6
        {&DDRB, &PINB, &PORTB, 3},  // B3  7
        {&DDRD, &PIND, &PORTD, 5},  // D5  8
        {&DDRD, &PIND, &PORTD, 6},  // D6  9
        {&DDRB, &PINB, &PORTB, 4},  // B4 10
        {&DDRB, &PINB, &PORTB, 5},  // B5 11
        {&DDRB, &PINB, &PORTB, 6},  // B6 12
        {&DDRB, &PINB, &PORTB, 7},  // B7 13
        {&DDRA, &PINA, &PORTA, 7},  // A7 14
        {&DDRA, &PINA, &PORTA, 6},  // A6 15
        {&DDRA, &PINA, &PORTA, 5},  // A5 16
        {&DDRA, &PINA, &PORTA, 4},  // A4 17
        {&DDRA, &PINA, &PORTA, 3},  // A3 18
        {&DDRA, &PINA, &PORTA, 2},  // A2 19
        {&DDRA, &PINA, &PORTA, 1},  // A1 20
        {&DDRA, &PINA, &PORTA, 0},  // A0 21
        {&DDRC, &PINC, &PORTC, 0},  // C0 22
        {&DDRC, &PINC, &PORTC, 1},  // C1 23
        {&DDRC, &PINC, &PORTC, 2},  // C2 24
        {&DDRC, &PINC, &PORTC, 3},  // C3 25
        {&DDRC, &PINC, &PORTC, 4},  // C4 26
        {&DDRC, &PINC, &PORTC, 5},  // C5 27
        {&DDRC, &PINC, &PORTC, 6},  // C6 28
        {&DDRC, &PINC, &PORTC, 7},  // C7 29
        {&DDRD, &PIND, &PORTD, 4},  // D4 30
        {&DDRD, &PIND, &PORTD, 7}   // D7 31
      };

    #elif defined(VARIANT_STANDARD)

      // Standard Layout
      static const pin_map_t pinMap[] = {
        {&DDRB, &PINB, &PORTB, 0},  // B0  0
        {&DDRB, &PINB, &PORTB, 1},  // B1  1
        {&DDRB, &PINB, &PORTB, 2},  // B2  2
        {&DDRB, &PINB, &PORTB, 3},  // B3  3
        {&DDRB, &PINB, &PORTB, 4},  // B4  4
        {&DDRB, &PINB, &PORTB, 5},  // B5  5
        {&DDRB, &PINB, &PORTB, 6},  // B6  6
        {&DDRB, &PINB, &PORTB, 7},  // B7  7
        {&DDRD, &PIND, &PORTD, 0},  // D0  8
        {&DDRD, &PIND, &PORTD, 1},  // D1  9
        {&DDRD, &PIND, &PORTD, 2},  // D2 10
        {&DDRD, &PIND, &PORTD, 3},  // D3 11
        {&DDRD, &PIND, &PORTD, 4},  // D4 12
        {&DDRD, &PIND, &PORTD, 5},  // D5 13
        {&DDRD, &PIND, &PORTD, 6},  // D6 14
        {&DDRD, &PIND, &PORTD, 7},  // D7 15
        {&DDRC, &PINC, &PORTC, 0},  // C0 16
        {&DDRC, &PINC, &PORTC, 1},  // C1 17
        {&DDRC, &PINC, &PORTC, 2},  // C2 18
        {&DDRC, &PINC, &PORTC, 3},  // C3 19
        {&DDRC, &PINC, &PORTC, 4},  // C4 20
        {&DDRC, &PINC, &PORTC, 5},  // C5 21
        {&DDRC, &PINC, &PORTC, 6},  // C6 22
        {&DDRC, &PINC, &PORTC, 7},  // C7 23
        {&DDRA, &PINA, &PORTA, 7},  // A7 24
        {&DDRA, &PINA, &PORTA, 6},  // A6 25
        {&DDRA, &PINA, &PORTA, 5},  // A5 26
        {&DDRA, &PINA, &PORTA, 4},  // A4 27
        {&DDRA, &PINA, &PORTA, 3},  // A3 28
        {&DDRA, &PINA, &PORTA, 2},  // A2 29
        {&DDRA, &PINA, &PORTA, 1},  // A1 30
        {&DDRA, &PINA, &PORTA, 0}   // A0 31
      };

    #else // !(VARIANT_MIGHTY || VARIANT_BOBUINO || VARIANT_STANDARD)

      #error Undefined variant 1284, 644, 324, 64, 32

    #endif

  #elif defined(__AVR_ATmega32U4__)

    #ifdef CORE_TEENSY

      // Teensy 2.0
      static const pin_map_t pinMap[] = {
        {&DDRB, &PINB, &PORTB, 0},  // B0  0
        {&DDRB, &PINB, &PORTB, 1},  // B1  1
        {&DDRB, &PINB, &PORTB, 2},  // B2  2
        {&DDRB, &PINB, &PORTB, 3},  // B3  3
        {&DDRB, &PINB, &PORTB, 7},  // B7  4
        {&DDRD, &PIND, &PORTD, 0},  // D0  5
        {&DDRD, &PIND, &PORTD, 1},  // D1  6
        {&DDRD, &PIND, &PORTD, 2},  // D2  7
        {&DDRD, &PIND, &PORTD, 3},  // D3  8
        {&DDRC, &PINC, &PORTC, 6},  // C6  9
        {&DDRC, &PINC, &PORTC, 7},  // C7 10
        {&DDRD, &PIND, &PORTD, 6},  // D6 11
        {&DDRD, &PIND, &PORTD, 7},  // D7 12
        {&DDRB, &PINB, &PORTB, 4},  // B4 13
        {&DDRB, &PINB, &PORTB, 5},  // B5 14
        {&DDRB, &PINB, &PORTB, 6},  // B6 15
        {&DDRF, &PINF, &PORTF, 7},  // F7 16
        {&DDRF, &PINF, &PORTF, 6},  // F6 17
        {&DDRF, &PINF, &PORTF, 5},  // F5 18
        {&DDRF, &PINF, &PORTF, 4},  // F4 19
        {&DDRF, &PINF, &PORTF, 1},  // F1 20
        {&DDRF, &PINF, &PORTF, 0},  // F0 21
        {&DDRD, &PIND, &PORTD, 4},  // D4 22
        {&DDRD, &PIND, &PORTD, 5},  // D5 23
        {&DDRE, &PINE, &PORTE, 6}   // E6 24
      };

    #else // !CORE_TEENSY

      // Leonardo
      static const pin_map_t pinMap[] = {
        {&DDRD, &PIND, &PORTD, 2},  // D2  0
        {&DDRD, &PIND, &PORTD, 3},  // D3  1
        {&DDRD, &PIND, &PORTD, 1},  // D1  2
        {&DDRD, &PIND, &PORTD, 0},  // D0  3
        {&DDRD, &PIND, &PORTD, 4},  // D4  4
        {&DDRC, &PINC, &PORTC, 6},  // C6  5
        {&DDRD, &PIND, &PORTD, 7},  // D7  6
        {&DDRE, &PINE, &PORTE, 6},  // E6  7
        {&DDRB, &PINB, &PORTB, 4},  // B4  8
        {&DDRB, &PINB, &PORTB, 5},  // B5  9
        {&DDRB, &PINB, &PORTB, 6},  // B6 10
        {&DDRB, &PINB, &PORTB, 7},  // B7 11
        {&DDRD, &PIND, &PORTD, 6},  // D6 12
        {&DDRC, &PINC, &PORTC, 7},  // C7 13
        {&DDRB, &PINB, &PORTB, 3},  // B3 14
        {&DDRB, &PINB, &PORTB, 1},  // B1 15
        {&DDRB, &PINB, &PORTB, 2},  // B2 16
        {&DDRB, &PINB, &PORTB, 0},  // B0 17
        {&DDRF, &PINF, &PORTF, 7},  // F7 18
        {&DDRF, &PINF, &PORTF, 6},  // F6 19
        {&DDRF, &PINF, &PORTF, 5},  // F5 20
        {&DDRF, &PINF, &PORTF, 4},  // F4 21
        {&DDRF, &PINF, &PORTF, 1},  // F1 22
        {&DDRF, &PINF, &PORTF, 0},  // F0 23
        {&DDRD, &PIND, &PORTD, 4},  // D4 24
        {&DDRD, &PIND, &PORTD, 7},  // D7 25
        {&DDRB, &PINB, &PORTB, 4},  // B4 26
        {&DDRB, &PINB, &PORTB, 5},  // B5 27
        {&DDRB, &PINB, &PORTB, 6},  // B6 28
        {&DDRD, &PIND, &PORTD, 6}   // D6 29
      };

    #endif // !CORE_TEENSY

  #elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)

    // Teensy++ 1.0 & 2.0
    static const pin_map_t pinMap[] = {
      {&DDRD, &PIND, &PORTD, 0},  // D0  0
      {&DDRD, &PIND, &PORTD, 1},  // D1  1
      {&DDRD, &PIND, &PORTD, 2},  // D2  2
      {&DDRD, &PIND, &PORTD, 3},  // D3  3
      {&DDRD, &PIND, &PORTD, 4},  // D4  4
      {&DDRD, &PIND, &PORTD, 5},  // D5  5
      {&DDRD, &PIND, &PORTD, 6},  // D6  6
      {&DDRD, &PIND, &PORTD, 7},  // D7  7
      {&DDRE, &PINE, &PORTE, 0},  // E0  8
      {&DDRE, &PINE, &PORTE, 1},  // E1  9
      {&DDRC, &PINC, &PORTC, 0},  // C0 10
      {&DDRC, &PINC, &PORTC, 1},  // C1 11
      {&DDRC, &PINC, &PORTC, 2},  // C2 12
      {&DDRC, &PINC, &PORTC, 3},  // C3 13
      {&DDRC, &PINC, &PORTC, 4},  // C4 14
      {&DDRC, &PINC, &PORTC, 5},  // C5 15
      {&DDRC, &PINC, &PORTC, 6},  // C6 16
      {&DDRC, &PINC, &PORTC, 7},  // C7 17
      {&DDRE, &PINE, &PORTE, 6},  // E6 18
      {&DDRE, &PINE, &PORTE, 7},  // E7 19
      {&DDRB, &PINB, &PORTB, 0},  // B0 20
      {&DDRB, &PINB, &PORTB, 1},  // B1 21
      {&DDRB, &PINB, &PORTB, 2},  // B2 22
      {&DDRB, &PINB, &PORTB, 3},  // B3 23
      {&DDRB, &PINB, &PORTB, 4},  // B4 24
      {&DDRB, &PINB, &PORTB, 5},  // B5 25
      {&DDRB, &PINB, &PORTB, 6},  // B6 26
      {&DDRB, &PINB, &PORTB, 7},  // B7 27
      {&DDRA, &PINA, &PORTA, 0},  // A0 28
      {&DDRA, &PINA, &PORTA, 1},  // A1 29
      {&DDRA, &PINA, &PORTA, 2},  // A2 30
      {&DDRA, &PINA, &PORTA, 3},  // A3 31
      {&DDRA, &PINA, &PORTA, 4},  // A4 32
      {&DDRA, &PINA, &PORTA, 5},  // A5 33
      {&DDRA, &PINA, &PORTA, 6},  // A6 34
      {&DDRA, &PINA, &PORTA, 7},  // A7 35
      {&DDRE, &PINE, &PORTE, 4},  // E4 36
      {&DDRE, &PINE, &PORTE, 5},  // E5 37
      {&DDRF, &PINF, &PORTF, 0},  // F0 38
      {&DDRF, &PINF, &PORTF, 1},  // F1 39
      {&DDRF, &PINF, &PORTF, 2},  // F2 40
      {&DDRF, &PINF, &PORTF, 3},  // F3 41
      {&DDRF, &PINF, &PORTF, 4},  // F4 42
      {&DDRF, &PINF, &PORTF, 5},  // F5 43
      {&DDRF, &PINF, &PORTF, 6},  // F6 44
      {&DDRF, &PINF, &PORTF, 7}   // F7 45
    };

  #else // CPU type

    #error "Unknown CPU type for Software SPI"

  #endif // CPU type

  /** count of pins */
  static constexpr uint8_t digitalPinCount = sizeof(pinMap) / sizeof(pin_map_t);

  /** generate bad pin number error */
  void badPinNumber()
    __attribute__((error("Pin number is too large or not a constant")));

  /**
   * Check for valid pin number
   * @param[in] pin Number of pin to be checked.
   */
  FORCE_INLINE static void badPinCheck(const uint8_t pin) {
    if (!__builtin_constant_p(pin) || pin >= digitalPinCount) badPinNumber();
  }

  /**
   * Fast write helper
   * @param[in] address I/O register address
   * @param[in] bit bit number to write
   * @param[in] level value for bit
   */
  FORCE_INLINE static void fastBitWriteSafe(volatile uint8_t* address, uint8_t bit, bool level) {
    uint8_t oldSREG;
    if (address > (uint8_t*)0x5F) { oldSREG = SREG; cli(); }
    if (level) SBI(*address, bit); else CBI(*address, bit);
    if (address > (uint8_t*)0x5F) SREG = oldSREG;
  }

  /**
   * Read pin value
   * @param[in] pin Arduino pin number
   * @return value read
   */
  FORCE_INLINE static bool fastDigitalRead(uint8_t pin) {
    badPinCheck(pin);
    return (*pinMap[pin].pin >> pinMap[pin].bit) & 1;
  }

  /**
   * Toggle a pin
   * @param[in] pin Arduino pin number
   *
   * If the pin is in output mode toggle the pin level.
   * If the pin is in input mode toggle the state of the 20K pullup.
   */
  FORCE_INLINE static void fastDigitalToggle(uint8_t pin) {
    badPinCheck(pin);
    if (pinMap[pin].pin > (uint8_t*)0x5F)
      *pinMap[pin].pin = _BV(pinMap[pin].bit);  // Must write bit to high address port
    else
      SBI(*pinMap[pin].pin, pinMap[pin].bit);   // Compiles to sbi and PIN register will not be read
  }

  /**
   * Set pin value
   * @param[in] pin Arduino pin number
   * @param[in] level value to write
   */
  FORCE_INLINE static void fastDigitalWrite(uint8_t pin, bool level) {
    badPinCheck(pin);
    fastBitWriteSafe(pinMap[pin].port, pinMap[pin].bit, level);
  }

  /**
   * Set pin mode
   * @param[in] pin Arduino pin number
   * @param[in] mode if true set output mode else input mode
   *
   * fastPinMode does not enable or disable the 20K pullup for input mode.
   */
  FORCE_INLINE static void fastPinMode(uint8_t pin, bool mode) {
    badPinCheck(pin);
    fastBitWriteSafe(pinMap[pin].ddr, pinMap[pin].bit, mode);
  }

#endif // !__arm__

/**
 * Set pin configuration
 * @param[in] pin Arduino pin number
 * @param[in] mode If true set output mode else input mode
 * @param[in] level If mode is output, set level high/low.
 *                  If mode is input, enable or disable the pin's 20K pullup.
 */
FORCE_INLINE static void fastPinConfig(uint8_t pin, bool mode, bool level) {
  fastPinMode(pin, mode);
  fastDigitalWrite(pin, level);
}

/**
 * @class DigitalPin
 * @brief Fast digital port I/O
 */
template<uint8_t PinNumber>
class DigitalPin {
public:

  /** Constructor */
  DigitalPin() {}

  /**
   * Constructor
   * @param[in] pinMode if true set output mode else input mode.
   */
  explicit DigitalPin(bool pinMode) { mode(pinMode); }

  /**
   * Constructor
   * @param[in] mode If true set output mode else input mode
   * @param[in] level If mode is output, set level high/low.
   *                  If mode is input, enable or disable the pin's 20K pullup.
   */
  DigitalPin(bool mode, bool level) { config(mode, level); }

  /**
   * Assignment operator
   * @param[in] value If true set the pin's level high else set the
   *  pin's level low.
   *
   * @return This DigitalPin instance.
   */
  FORCE_INLINE DigitalPin & operator = (bool value) { write(value); return *this; }

  /**
   * Parentheses operator
   * @return Pin's level
   */
  FORCE_INLINE operator bool () const { return read(); }

  /**
   * Set pin configuration
   * @param[in] mode If true set output mode else input mode
   * @param[in] level If mode is output, set level high/low.
   *                  If mode is input, enable or disable the pin's 20K pullup.
   */
  FORCE_INLINE void config(bool mode, bool level) { fastPinConfig(PinNumber, mode, level); }

  /**
   * Set pin level high if output mode or enable 20K pullup if input mode.
   */
  FORCE_INLINE void high() { write(true); }

  /**
   * Set pin level low if output mode or disable 20K pullup if input mode.
   */
  FORCE_INLINE void low() { write(false); }

  /**
   * Set pin mode
   * @param[in] pinMode if true set output mode else input mode.
   *
   * mode() does not enable or disable the 20K pullup for input mode.
   */
  FORCE_INLINE void mode(bool pinMode) { fastPinMode(PinNumber, pinMode); }

  /** @return Pin's level */
  FORCE_INLINE bool read() const { return fastDigitalRead(PinNumber); }

  /**
   * Toggle a pin
   * If the pin is in output mode toggle the pin's level.
   * If the pin is in input mode toggle the state of the 20K pullup.
   */
  FORCE_INLINE void toggle() { fastDigitalToggle(PinNumber); }

  /**
   * Write the pin's level.
   * @param[in] value If true set the pin's level high else set the
   *  pin's level low.
   */
  FORCE_INLINE void write(bool value) { fastDigitalWrite(PinNumber, value); }
};

const bool MISO_MODE  = false,  // Pin Mode for MISO is input.
           MISO_LEVEL = false,  // Pullups disabled for MISO are disabled.
           MOSI_MODE  = true,   // Pin Mode for MOSI is output.
           SCK_MODE   = true;   // Pin Mode for SCK is output.

/**
 * @class SoftSPI
 * @brief Fast software SPI.
 */
template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin, uint8_t Mode = 0>
class SoftSPI {
 public:

  /** Initialize SoftSPI pins. */
  void begin() {
    fastPinConfig(MisoPin, MISO_MODE, MISO_LEVEL);
    fastPinConfig(MosiPin, MOSI_MODE, !MODE_CPHA(Mode));
    fastPinConfig(SckPin, SCK_MODE, MODE_CPOL(Mode));
  }

  /**
   * Soft SPI receive byte.
   * @return Data byte received.
   */
  FORCE_INLINE uint8_t receive() {
    uint8_t data = 0;
    receiveBit(7, &data);
    receiveBit(6, &data);
    receiveBit(5, &data);
    receiveBit(4, &data);
    receiveBit(3, &data);
    receiveBit(2, &data);
    receiveBit(1, &data);
    receiveBit(0, &data);
    return data;
  }

  /**
   * Soft SPI send byte.
   * @param[in] data Data byte to send.
   */
  FORCE_INLINE void send(uint8_t data) {
    sendBit(7, data);
    sendBit(6, data);
    sendBit(5, data);
    sendBit(4, data);
    sendBit(3, data);
    sendBit(2, data);
    sendBit(1, data);
    sendBit(0, data);
  }

  /**
   * Soft SPI transfer byte.
   * @param[in] txData Data byte to send.
   * @return Data byte received.
   */
  FORCE_INLINE uint8_t transfer(uint8_t txData) {
    uint8_t rxData = 0;
    transferBit(7, &rxData, txData);
    transferBit(6, &rxData, txData);
    transferBit(5, &rxData, txData);
    transferBit(4, &rxData, txData);
    transferBit(3, &rxData, txData);
    transferBit(2, &rxData, txData);
    transferBit(1, &rxData, txData);
    transferBit(0, &rxData, txData);
    return rxData;
  }

 private:

  FORCE_INLINE bool MODE_CPHA(uint8_t mode) { return bool(mode & 1); }
  FORCE_INLINE bool MODE_CPOL(uint8_t mode) { return bool(mode & 2); }
  FORCE_INLINE void receiveBit(uint8_t bit, uint8_t *data) {
    if (MODE_CPHA(Mode)) fastDigitalWrite(SckPin, !MODE_CPOL(Mode));
    nop;
    nop;
    fastDigitalWrite(SckPin,
      MODE_CPHA(Mode) ? MODE_CPOL(Mode) : !MODE_CPOL(Mode));
    if (fastDigitalRead(MisoPin)) SBI(*data, bit);
    if (!MODE_CPHA(Mode)) fastDigitalWrite(SckPin, MODE_CPOL(Mode));
  }

  FORCE_INLINE void sendBit(uint8_t bit, uint8_t data) {
    if (MODE_CPHA(Mode)) fastDigitalWrite(SckPin, !MODE_CPOL(Mode));
    fastDigitalWrite(MosiPin, data & _BV(bit));
    fastDigitalWrite(SckPin, MODE_CPHA(Mode) ? MODE_CPOL(Mode) : !MODE_CPOL(Mode));
    nop;
    nop;
    if (!MODE_CPHA(Mode)) fastDigitalWrite(SckPin, MODE_CPOL(Mode));
  }

  FORCE_INLINE void transferBit(uint8_t bit, uint8_t *rxData, uint8_t txData) {
    if (MODE_CPHA(Mode)) fastDigitalWrite(SckPin, !MODE_CPOL(Mode));
    fastDigitalWrite(MosiPin, txData & _BV(bit));
    fastDigitalWrite(SckPin,
      MODE_CPHA(Mode) ? MODE_CPOL(Mode) : !MODE_CPOL(Mode));
    if (fastDigitalRead(MisoPin)) SBI(*rxData, bit);
    if (!MODE_CPHA(Mode)) fastDigitalWrite(SckPin, MODE_CPOL(Mode));
  }

};
