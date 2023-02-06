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

/**
 * Pin mapping for the 644, 644p, 644pa, and 1284p
 *
 *   Logical Pin: 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
 *   Port:        B0 B1 B2 B3 B4 B5 B6 B7 D0 D1 D2 D3 D4 D5 D6 D7 C0 C1 C2 C3 C4 C5 C6 C7 A7 A6 A5 A4 A3 A2 A1 A0
 * 
 * Arduino Pin Layout video: https://www.youtube.com/watch?v=rIqeVCX09FA
 * AVR alternate pin function overview video: https://www.youtube.com/watch?v=Gz9QU3FvdXs
 */

/**                        ATMega644
 *
 *                        +---\/---+
 *            (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
 *            (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
 *       INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
 *        PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
 *        PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
 *       MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
 *       MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
 *        SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
 *                  RST  9|        |32  AREF
 *                  VCC 10|        |31  GND
 *                  GND 11|        |30  AVCC
 *                XTAL2 12|        |29  PC7 (D 23)
 *                XTAL1 13|        |28  PC6 (D 22)
 *       RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
 *       TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
 *  INT0 RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
 *  INT1 TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
 *       PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
 *       PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
 *       PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
 *                        +--------+
 */

#include "../fastio.h"

// Digital I/O

#define DIO0_PIN    PINB0
#define DIO0_RPORT  PINB
#define DIO0_WPORT  PORTB
#define DIO0_DDR    DDRB
#define DIO0_PWM    nullptr
#define PinB0       0

#define DIO1_PIN    PINB1
#define DIO1_RPORT  PINB
#define DIO1_WPORT  PORTB
#define DIO1_DDR    DDRB
#define DIO1_PWM    nullptr
#define PinB1       1

#define DIO2_PIN    PINB2
#define DIO2_RPORT  PINB
#define DIO2_WPORT  PORTB
#define DIO2_DDR    DDRB
#define DIO2_PWM    nullptr
#define PinB2       2

#define DIO3_PIN    PINB3
#define DIO3_RPORT  PINB
#define DIO3_WPORT  PORTB
#define DIO3_DDR    DDRB
#define DIO3_PWM    &OCR0A
#define PinB3       3

#define DIO4_PIN    PINB4
#define DIO4_RPORT  PINB
#define DIO4_WPORT  PORTB
#define DIO4_DDR    DDRB
#define DIO4_PWM    &OCR0B
#define PinB4       4

#define DIO5_PIN    PINB5
#define DIO5_RPORT  PINB
#define DIO5_WPORT  PORTB
#define DIO5_DDR    DDRB
#define DIO5_PWM    nullptr
#define PinB5       5

#define DIO6_PIN    PINB6
#define DIO6_RPORT  PINB
#define DIO6_WPORT  PORTB
#define DIO6_DDR    DDRB
#define DIO6_PWM    nullptr
#define PinB6       6

#define DIO7_PIN    PINB7
#define DIO7_RPORT  PINB
#define DIO7_WPORT  PORTB
#define DIO7_DDR    DDRB
#define DIO7_PWM    nullptr
#define PinB7       7

#define DIO8_PIN    PIND0
#define DIO8_RPORT  PIND
#define DIO8_WPORT  PORTD
#define DIO8_DDR    DDRD
#define DIO8_PWM    nullptr
#define PinD0       8

#define DIO9_PIN    PIND1
#define DIO9_RPORT  PIND
#define DIO9_WPORT  PORTD
#define DIO9_DDR    DDRD
#define DIO9_PWM    nullptr
#define PinD1       9

#define DIO10_PIN   PIND2
#define DIO10_RPORT PIND
#define DIO10_WPORT PORTD
#define DIO10_DDR   DDRD
#define DIO10_PWM   nullptr
#define PinD2       10

#define DIO11_PIN   PIND3
#define DIO11_RPORT PIND
#define DIO11_WPORT PORTD
#define DIO11_DDR   DDRD
#define DIO11_PWM   nullptr
#define PinD3       11

#define DIO12_PIN   PIND4
#define DIO12_RPORT PIND
#define DIO12_WPORT PORTD
#define DIO12_DDR   DDRD
#define DIO12_PWM   &OCR1B
#define PinD4       12

#define DIO13_PIN   PIND5
#define DIO13_RPORT PIND
#define DIO13_WPORT PORTD
#define DIO13_DDR   DDRD
#define DIO13_PWM   &OCR1A
#define PinD5       13

#define DIO14_PIN   PIND6
#define DIO14_RPORT PIND
#define DIO14_WPORT PORTD
#define DIO14_DDR   DDRD
#define DIO14_PWM   &OCR2B
#define PinD6       14

#define DIO15_PIN   PIND7
#define DIO15_RPORT PIND
#define DIO15_WPORT PORTD
#define DIO15_DDR   DDRD
#define DIO15_PWM   &OCR2A
#define PinD7       15

#define DIO16_PIN   PINC0
#define DIO16_RPORT PINC
#define DIO16_WPORT PORTC
#define DIO16_DDR   DDRC
#define DIO16_PWM   nullptr
#define PinC0       16

#define DIO17_PIN   PINC1
#define DIO17_RPORT PINC
#define DIO17_WPORT PORTC
#define DIO17_DDR   DDRC
#define DIO17_PWM   nullptr
#define PinC1       17

#define DIO18_PIN   PINC2
#define DIO18_RPORT PINC
#define DIO18_WPORT PORTC
#define DIO18_DDR   DDRC
#define DIO18_PWM   nullptr
#define PinC2       18

#define DIO19_PIN   PINC3
#define DIO19_RPORT PINC
#define DIO19_WPORT PORTC
#define DIO19_DDR   DDRC
#define DIO19_PWM   nullptr
#define PinC3       19

#define DIO20_PIN   PINC4
#define DIO20_RPORT PINC
#define DIO20_WPORT PORTC
#define DIO20_DDR   DDRC
#define DIO20_PWM   nullptr
#define PinC4       20

#define DIO21_PIN   PINC5
#define DIO21_RPORT PINC
#define DIO21_WPORT PORTC
#define DIO21_DDR   DDRC
#define DIO21_PWM   nullptr
#define PinC5       21

#define DIO22_PIN   PINC6
#define DIO22_RPORT PINC
#define DIO22_WPORT PORTC
#define DIO22_DDR   DDRC
#define DIO22_PWM   nullptr
#define PinC6       22

#define DIO23_PIN   PINC7
#define DIO23_RPORT PINC
#define DIO23_WPORT PORTC
#define DIO23_DDR   DDRC
#define DIO23_PWM   nullptr
#define PinC7       23

#define DIO24_PIN   PINA7
#define DIO24_RPORT PINA
#define DIO24_WPORT PORTA
#define DIO24_DDR   DDRA
#define DIO24_PWM   nullptr
#define PinA7       24

#define DIO25_PIN   PINA6
#define DIO25_RPORT PINA
#define DIO25_WPORT PORTA
#define DIO25_DDR   DDRA
#define DIO25_PWM   nullptr
#define PinA6       25

#define DIO26_PIN   PINA5
#define DIO26_RPORT PINA
#define DIO26_WPORT PORTA
#define DIO26_DDR   DDRA
#define DIO26_PWM   nullptr
#define PinA5       26

#define DIO27_PIN   PINA4
#define DIO27_RPORT PINA
#define DIO27_WPORT PORTA
#define DIO27_DDR   DDRA
#define DIO27_PWM   nullptr
#define PinA4       27

#define DIO28_PIN   PINA3
#define DIO28_RPORT PINA
#define DIO28_WPORT PORTA
#define DIO28_DDR   DDRA
#define DIO28_PWM   nullptr
#define PinA3       28

#define DIO29_PIN   PINA2
#define DIO29_RPORT PINA
#define DIO29_WPORT PORTA
#define DIO29_DDR   DDRA
#define DIO29_PWM   nullptr
#define PinA2       29

#define DIO30_PIN   PINA1
#define DIO30_RPORT PINA
#define DIO30_WPORT PORTA
#define DIO30_DDR   DDRA
#define DIO30_PWM   nullptr
#define PinA1       30

#define DIO31_PIN   PINA0
#define DIO31_RPORT PINA
#define DIO31_WPORT PORTA
#define DIO31_DDR   DDRA
#define DIO31_PWM   nullptr
#define PinA0       31

#define DIO_NUM 32

// NOTE: may be used by external libraries (LiquidCrystal)
#ifndef DEBUG_LED
  #define DEBUG_LED   PinB0
#endif

// UART
#ifndef RXD
  #define RXD         PinD0
#endif
#ifndef TXD
  #define TXD         PinD1
#endif
#ifndef RXD0
  #define RXD0        PinD0
#endif
#ifndef TXD0
  #define TXD0        PinD1
#endif

#ifndef RXD1
  #define RXD1        PinD2
#endif
#ifndef TXD1
  #define TXD1        PinD3
#endif

// SPI
#ifndef SCK
  #define SCK         PinB7
#endif
#ifndef MISO
  #define MISO        PinB6
#endif
#ifndef MOSI
  #define MOSI        PinB5
#endif
#ifndef SS
  #define SS          PinB4
#endif

// TWI (I2C)
#define SCL         PinC0
#define SDA         PinC1

// Timers and PWM
#define OC0A        PinB3
#define OC0B        PinB4
#define OC1A        PinD5
#define OC1B        PinD4
#define OC2A        PinD7
#define OC2B        PinD6