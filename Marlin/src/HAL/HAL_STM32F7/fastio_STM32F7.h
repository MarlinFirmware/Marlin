/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2017 Victor Perez
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

/**
 * Fast I/O interfaces for STM32F7
 * These use GPIO functions instead of Direct Port Manipulation, as on AVR.
 */

#define _BV(b) (1 << (b))

#define READ(IO)                digitalRead(IO)
#define WRITE(IO,V)             digitalWrite(IO,V)
#define WRITE_VAR(IO,V)         WRITE(IO,V)

#define _GET_MODE(IO)
#define _SET_MODE(IO,M)         pinMode(IO, M)
#define _SET_OUTPUT(IO)         pinMode(IO, OUTPUT)                               /*!< Output Push Pull Mode & GPIO_NOPULL   */

#define OUT_WRITE(IO,V)         do{ _SET_OUTPUT(IO); WRITE(IO,V); }while(0)

#define SET_INPUT(IO)           _SET_MODE(IO, INPUT)                              /*!< Input Floating Mode                   */
#define SET_INPUT_PULLUP(IO)    _SET_MODE(IO, INPUT_PULLUP)                       /*!< Input with Pull-up activation         */
#define SET_INPUT_PULLDOWN(IO)  _SET_MODE(IO, INPUT_PULLDOWN)                     /*!< Input with Pull-down activation       */
#define SET_OUTPUT(IO)          OUT_WRITE(IO, LOW)

#define TOGGLE(IO)              OUT_WRITE(IO, !READ(IO))

#define GET_INPUT(IO)
#define GET_OUTPUT(IO)
#define GET_TIMER(IO)

#define PWM_PIN(p) true
#define USEABLE_HARDWARE_PWM(p) PWM_PIN(p)

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

#define _STM32_PIN(_PORT,_PIN) ((PORT##_PORT * 16) + _PIN)

#define PA0  _STM32_PIN(A,  0)
#define PA1  _STM32_PIN(A,  1)
#define PA2  _STM32_PIN(A,  2)
#define PA3  _STM32_PIN(A,  3)
#define PA4  _STM32_PIN(A,  4)
#define PA5  _STM32_PIN(A,  5)
#define PA6  _STM32_PIN(A,  6)
#define PA7  _STM32_PIN(A,  7)
#define PA8  _STM32_PIN(A,  8)
#define PA9  _STM32_PIN(A,  9)
#define PA10 _STM32_PIN(A, 10)
#define PA11 _STM32_PIN(A, 11)
#define PA12 _STM32_PIN(A, 12)
#define PA13 _STM32_PIN(A, 13)
#define PA14 _STM32_PIN(A, 14)
#define PA15 _STM32_PIN(A, 15)

#define PB0  _STM32_PIN(B,  0)
#define PB1  _STM32_PIN(B,  1)
#define PB2  _STM32_PIN(B,  2)
#define PB3  _STM32_PIN(B,  3)
#define PB4  _STM32_PIN(B,  4)
#define PB5  _STM32_PIN(B,  5)
#define PB6  _STM32_PIN(B,  6)
#define PB7  _STM32_PIN(B,  7)
#define PB8  _STM32_PIN(B,  8)
#define PB9  _STM32_PIN(B,  9)
#define PB10 _STM32_PIN(B, 10)
#define PB11 _STM32_PIN(B, 11)
#define PB12 _STM32_PIN(B, 12)
#define PB13 _STM32_PIN(B, 13)
#define PB14 _STM32_PIN(B, 14)
#define PB15 _STM32_PIN(B, 15)

#define PC0  _STM32_PIN(C,  0)
#define PC1  _STM32_PIN(C,  1)
#define PC2  _STM32_PIN(C,  2)
#define PC3  _STM32_PIN(C,  3)
#define PC4  _STM32_PIN(C,  4)
#define PC5  _STM32_PIN(C,  5)
#define PC6  _STM32_PIN(C,  6)
#define PC7  _STM32_PIN(C,  7)
#define PC8  _STM32_PIN(C,  8)
#define PC9  _STM32_PIN(C,  9)
#define PC10 _STM32_PIN(C, 10)
#define PC11 _STM32_PIN(C, 11)
#define PC12 _STM32_PIN(C, 12)
#define PC13 _STM32_PIN(C, 13)
#define PC14 _STM32_PIN(C, 14)
#define PC15 _STM32_PIN(C, 15)

#define PD0  _STM32_PIN(D,  0)
#define PD1  _STM32_PIN(D,  1)
#define PD2  _STM32_PIN(D,  2)
#define PD3  _STM32_PIN(D,  3)
#define PD4  _STM32_PIN(D,  4)
#define PD5  _STM32_PIN(D,  5)
#define PD6  _STM32_PIN(D,  6)
#define PD7  _STM32_PIN(D,  7)
#define PD8  _STM32_PIN(D,  8)
#define PD9  _STM32_PIN(D,  9)
#define PD10 _STM32_PIN(D, 10)
#define PD11 _STM32_PIN(D, 11)
#define PD12 _STM32_PIN(D, 12)
#define PD13 _STM32_PIN(D, 13)
#define PD14 _STM32_PIN(D, 14)
#define PD15 _STM32_PIN(D, 15)

#define PE0  _STM32_PIN(E,  0)
#define PE1  _STM32_PIN(E,  1)
#define PE2  _STM32_PIN(E,  2)
#define PE3  _STM32_PIN(E,  3)
#define PE4  _STM32_PIN(E,  4)
#define PE5  _STM32_PIN(E,  5)
#define PE6  _STM32_PIN(E,  6)
#define PE7  _STM32_PIN(E,  7)
#define PE8  _STM32_PIN(E,  8)
#define PE9  _STM32_PIN(E,  9)
#define PE10 _STM32_PIN(E, 10)
#define PE11 _STM32_PIN(E, 11)
#define PE12 _STM32_PIN(E, 12)
#define PE13 _STM32_PIN(E, 13)
#define PE14 _STM32_PIN(E, 14)
#define PE15 _STM32_PIN(E, 15)

#define PF0  _STM32_PIN(F,  0)
#define PF1  _STM32_PIN(F,  1)
#define PF2  _STM32_PIN(F,  2)
#define PF3  _STM32_PIN(F,  3)
#define PF4  _STM32_PIN(F,  4)
#define PF5  _STM32_PIN(F,  5)
#define PF6  _STM32_PIN(F,  6)
#define PF7  _STM32_PIN(F,  7)
#define PF8  _STM32_PIN(F,  8)
#define PF9  _STM32_PIN(F,  9)
#define PF10 _STM32_PIN(F, 10)
#define PF11 _STM32_PIN(F, 11)
#define PF12 _STM32_PIN(F, 12)
#define PF13 _STM32_PIN(F, 13)
#define PF14 _STM32_PIN(F, 14)
#define PF15 _STM32_PIN(F, 15)

#define PG0  _STM32_PIN(G,  0)
#define PG1  _STM32_PIN(G,  1)
#define PG2  _STM32_PIN(G,  2)
#define PG3  _STM32_PIN(G,  3)
#define PG4  _STM32_PIN(G,  4)
#define PG5  _STM32_PIN(G,  5)
#define PG6  _STM32_PIN(G,  6)
#define PG7  _STM32_PIN(G,  7)
#define PG8  _STM32_PIN(G,  8)
#define PG9  _STM32_PIN(G,  9)
#define PG10 _STM32_PIN(G, 10)
#define PG11 _STM32_PIN(G, 11)
#define PG12 _STM32_PIN(G, 12)
#define PG13 _STM32_PIN(G, 13)
#define PG14 _STM32_PIN(G, 14)
#define PG15 _STM32_PIN(G, 15)
