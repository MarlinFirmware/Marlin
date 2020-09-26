/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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

/**
 * Fast I/O interfaces for STM32F4/7
 * These use GPIO functions instead of Direct Port Manipulation, as on AVR.
 */

#ifndef PWM
  #define PWM OUTPUT
#endif

#define READ(IO)                digitalRead(IO)
#define WRITE(IO,V)             digitalWrite(IO,V)

#define _GET_MODE(IO)
#define _SET_MODE(IO,M)         pinMode(IO, M)
#define _SET_OUTPUT(IO)         pinMode(IO, OUTPUT)                               /*!< Output Push Pull Mode & GPIO_NOPULL   */

#define OUT_WRITE(IO,V)         do{ _SET_OUTPUT(IO); WRITE(IO,V); }while(0)

#define SET_INPUT(IO)           _SET_MODE(IO, INPUT)                              /*!< Input Floating Mode                   */
#define SET_INPUT_PULLUP(IO)    _SET_MODE(IO, INPUT_PULLUP)                       /*!< Input with Pull-up activation         */
#define SET_INPUT_PULLDOWN(IO)  _SET_MODE(IO, INPUT_PULLDOWN)                     /*!< Input with Pull-down activation       */
#define SET_OUTPUT(IO)          OUT_WRITE(IO, LOW)
#define SET_PWM(IO)             _SET_MODE(IO, PWM)

#define TOGGLE(IO)              OUT_WRITE(IO, !READ(IO))

#define IS_INPUT(IO)
#define IS_OUTPUT(IO)

#define PWM_PIN(P)              true

// digitalRead/Write wrappers
#define extDigitalRead(IO)    digitalRead(IO)
#define extDigitalWrite(IO,V) digitalWrite(IO,V)

//
// Pins Definitions
//
#define PORTA 0
#define PORTB 1
#define PORTC 2
#define PORTD 3
#define PORTE 4
#define PORTF 5
#define PORTG 6

#define _STM32_PIN(P,PN) ((PORT##P * 16) + PN)

#undef PA0
#define PA0  _STM32_PIN(A,  0)
#undef PA1
#define PA1  _STM32_PIN(A,  1)
#undef PA2
#define PA2  _STM32_PIN(A,  2)
#undef PA3
#define PA3  _STM32_PIN(A,  3)
#undef PA4
#define PA4  _STM32_PIN(A,  4)
#undef PA5
#define PA5  _STM32_PIN(A,  5)
#undef PA6
#define PA6  _STM32_PIN(A,  6)
#undef PA7
#define PA7  _STM32_PIN(A,  7)
#undef PA8
#define PA8  _STM32_PIN(A,  8)
#undef PA9
#define PA9  _STM32_PIN(A,  9)
#undef PA10
#define PA10 _STM32_PIN(A, 10)
#undef PA11
#define PA11 _STM32_PIN(A, 11)
#undef PA12
#define PA12 _STM32_PIN(A, 12)
#undef PA13
#define PA13 _STM32_PIN(A, 13)
#undef PA14
#define PA14 _STM32_PIN(A, 14)
#undef PA15
#define PA15 _STM32_PIN(A, 15)

#undef PB0
#define PB0  _STM32_PIN(B,  0)
#undef PB1
#define PB1  _STM32_PIN(B,  1)
#undef PB2
#define PB2  _STM32_PIN(B,  2)
#undef PB3
#define PB3  _STM32_PIN(B,  3)
#undef PB4
#define PB4  _STM32_PIN(B,  4)
#undef PB5
#define PB5  _STM32_PIN(B,  5)
#undef PB6
#define PB6  _STM32_PIN(B,  6)
#undef PB7
#define PB7  _STM32_PIN(B,  7)
#undef PB8
#define PB8  _STM32_PIN(B,  8)
#undef PB9
#define PB9  _STM32_PIN(B,  9)
#undef PB10
#define PB10 _STM32_PIN(B, 10)
#undef PB11
#define PB11 _STM32_PIN(B, 11)
#undef PB12
#define PB12 _STM32_PIN(B, 12)
#undef PB13
#define PB13 _STM32_PIN(B, 13)
#undef PB14
#define PB14 _STM32_PIN(B, 14)
#undef PB15
#define PB15 _STM32_PIN(B, 15)

#undef PC0
#define PC0  _STM32_PIN(C,  0)
#undef PC1
#define PC1  _STM32_PIN(C,  1)
#undef PC2
#define PC2  _STM32_PIN(C,  2)
#undef PC3
#define PC3  _STM32_PIN(C,  3)
#undef PC4
#define PC4  _STM32_PIN(C,  4)
#undef PC5
#define PC5  _STM32_PIN(C,  5)
#undef PC6
#define PC6  _STM32_PIN(C,  6)
#undef PC7
#define PC7  _STM32_PIN(C,  7)
#undef PC8
#define PC8  _STM32_PIN(C,  8)
#undef PC9
#define PC9  _STM32_PIN(C,  9)
#undef PC10
#define PC10 _STM32_PIN(C, 10)
#undef PC11
#define PC11 _STM32_PIN(C, 11)
#undef PC12
#define PC12 _STM32_PIN(C, 12)
#undef PC13
#define PC13 _STM32_PIN(C, 13)
#undef PC14
#define PC14 _STM32_PIN(C, 14)
#undef PC15
#define PC15 _STM32_PIN(C, 15)

#undef PD0
#define PD0  _STM32_PIN(D,  0)
#undef PD1
#define PD1  _STM32_PIN(D,  1)
#undef PD2
#define PD2  _STM32_PIN(D,  2)
#undef PD3
#define PD3  _STM32_PIN(D,  3)
#undef PD4
#define PD4  _STM32_PIN(D,  4)
#undef PD5
#define PD5  _STM32_PIN(D,  5)
#undef PD6
#define PD6  _STM32_PIN(D,  6)
#undef PD7
#define PD7  _STM32_PIN(D,  7)
#undef PD8
#define PD8  _STM32_PIN(D,  8)
#undef PD9
#define PD9  _STM32_PIN(D,  9)
#undef PD10
#define PD10 _STM32_PIN(D, 10)
#undef PD11
#define PD11 _STM32_PIN(D, 11)
#undef PD12
#define PD12 _STM32_PIN(D, 12)
#undef PD13
#define PD13 _STM32_PIN(D, 13)
#undef PD14
#define PD14 _STM32_PIN(D, 14)
#undef PD15
#define PD15 _STM32_PIN(D, 15)

#undef PE0
#define PE0  _STM32_PIN(E,  0)
#undef PE1
#define PE1  _STM32_PIN(E,  1)
#undef PE2
#define PE2  _STM32_PIN(E,  2)
#undef PE3
#define PE3  _STM32_PIN(E,  3)
#undef PE4
#define PE4  _STM32_PIN(E,  4)
#undef PE5
#define PE5  _STM32_PIN(E,  5)
#undef PE6
#define PE6  _STM32_PIN(E,  6)
#undef PE7
#define PE7  _STM32_PIN(E,  7)
#undef PE8
#define PE8  _STM32_PIN(E,  8)
#undef PE9
#define PE9  _STM32_PIN(E,  9)
#undef PE10
#define PE10 _STM32_PIN(E, 10)
#undef PE11
#define PE11 _STM32_PIN(E, 11)
#undef PE12
#define PE12 _STM32_PIN(E, 12)
#undef PE13
#define PE13 _STM32_PIN(E, 13)
#undef PE14
#define PE14 _STM32_PIN(E, 14)
#undef PE15
#define PE15 _STM32_PIN(E, 15)

#ifdef STM32F7

  #undef PORTF
  #define PORTF 5
  #undef PF0
  #define PF0  _STM32_PIN(F,  0)
  #undef PF1
  #define PF1  _STM32_PIN(F,  1)
  #undef PF2
  #define PF2  _STM32_PIN(F,  2)
  #undef PF3
  #define PF3  _STM32_PIN(F,  3)
  #undef PF4
  #define PF4  _STM32_PIN(F,  4)
  #undef PF5
  #define PF5  _STM32_PIN(F,  5)
  #undef PF6
  #define PF6  _STM32_PIN(F,  6)
  #undef PF7
  #define PF7  _STM32_PIN(F,  7)
  #undef PF8
  #define PF8  _STM32_PIN(F,  8)
  #undef PF9
  #define PF9  _STM32_PIN(F,  9)
  #undef PF10
  #define PF10 _STM32_PIN(F, 10)
  #undef PF11
  #define PF11 _STM32_PIN(F, 11)
  #undef PF12
  #define PF12 _STM32_PIN(F, 12)
  #undef PF13
  #define PF13 _STM32_PIN(F, 13)
  #undef PF14
  #define PF14 _STM32_PIN(F, 14)
  #undef PF15
  #define PF15 _STM32_PIN(F, 15)

  #undef PORTG
  #define PORTG 6
  #undef PG0
  #define PG0  _STM32_PIN(G,  0)
  #undef PG1
  #define PG1  _STM32_PIN(G,  1)
  #undef PG2
  #define PG2  _STM32_PIN(G,  2)
  #undef PG3
  #define PG3  _STM32_PIN(G,  3)
  #undef PG4
  #define PG4  _STM32_PIN(G,  4)
  #undef PG5
  #define PG5  _STM32_PIN(G,  5)
  #undef PG6
  #define PG6  _STM32_PIN(G,  6)
  #undef PG7
  #define PG7  _STM32_PIN(G,  7)
  #undef PG8
  #define PG8  _STM32_PIN(G,  8)
  #undef PG9
  #define PG9  _STM32_PIN(G,  9)
  #undef PG10
  #define PG10 _STM32_PIN(G, 10)
  #undef PG11
  #define PG11 _STM32_PIN(G, 11)
  #undef PG12
  #define PG12 _STM32_PIN(G, 12)
  #undef PG13
  #define PG13 _STM32_PIN(G, 13)
  #undef PG14
  #define PG14 _STM32_PIN(G, 14)
  #undef PG15
  #define PG15 _STM32_PIN(G, 15)

#endif // STM32GENERIC && STM32F7
