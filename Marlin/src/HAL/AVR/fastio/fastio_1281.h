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
 * Pin mapping for the 1281 and 2561
 *
 *   Logical Pin: 38 39 40 41 42 43 44 45 16 10 11 12 06 07 08 09 30 31 32 33 34 35 36 37 17 18 19 20 21 22 23 24 00 01 13 05 02 03 14 15 46 47 48 49 50 51 52 53 25 26 27 28 29 04
 *   Port:        A0 A1 A2 A3 A4 A5 A6 A7 B0 B1 B2 B3 B4 B5 B6 B7 C0 C1 C2 C3 C4 C5 C6 C7 D0 D1 D2 D3 D4 D5 D6 D7 E0 E1 E2 E3 E4 E5 E6 E7 F0 F1 F2 F3 F4 F5 F6 F7 G0 G1 G2 G3 G4 G5
 */

#include "../fastio.h"

// change for your board
#define DEBUG_LED   DIO46

// UART
#define RXD         DIO0
#define TXD         DIO1

// SPI
#define SCK         DIO10
#define MISO        DIO12
#define MOSI        DIO11
#define SS          DIO16

// TWI (I2C)
#define SCL         DIO17
#define SDA         DIO18

// Timers and PWM
#define OC0A        DIO9
#define OC0B        DIO4
#define OC1A        DIO7
#define OC1B        DIO8
#define OC2A        DIO6
#define OC3A        DIO5
#define OC3B        DIO2
#define OC3C        DIO3

// Digital I/O

#define DIO0_PIN    PINE0
#define DIO0_RPORT  PINE
#define DIO0_WPORT  PORTE
#define DIO0_DDR    DDRE
#define DIO0_PWM    nullptr
#define PinE0       0

#define DIO1_PIN    PINE1
#define DIO1_RPORT  PINE
#define DIO1_WPORT  PORTE
#define DIO1_DDR    DDRE
#define DIO1_PWM    nullptr
#define PinE1       1

#define DIO2_PIN    PINE4
#define DIO2_RPORT  PINE
#define DIO2_WPORT  PORTE
#define DIO2_DDR    DDRE
#define DIO2_PWM    &OCR3BL
#define PinE4       2

#define DIO3_PIN    PINE5
#define DIO3_RPORT  PINE
#define DIO3_WPORT  PORTE
#define DIO3_DDR    DDRE
#define DIO3_PWM    &OCR3CL
#define PinE5       3

#define DIO4_PIN    PING5
#define DIO4_RPORT  PING
#define DIO4_WPORT  PORTG
#define DIO4_DDR    DDRG
#define DIO4_PWM    &OCR0B
#define PinG5       4

#define DIO5_PIN    PINE3
#define DIO5_RPORT  PINE
#define DIO5_WPORT  PORTE
#define DIO5_DDR    DDRE
#define DIO5_PWM    &OCR3AL
#define PinE3       5

#define DIO6_PIN    PINB4
#define DIO6_RPORT  PINB
#define DIO6_WPORT  PORTB
#define DIO6_DDR    DDRB
#define DIO6_PWM    &OCR2AL
#define PinB4       6

#define DIO7_PIN    PINB5
#define DIO7_RPORT  PINB
#define DIO7_WPORT  PORTB
#define DIO7_DDR    DDRB
#define DIO7_PWM    &OCR1AL
#define PinB5       7

#define DIO8_PIN    PINB6
#define DIO8_RPORT  PINB
#define DIO8_WPORT  PORTB
#define DIO8_DDR    DDRB
#define DIO8_PWM    &OCR1BL
#define PinB6       8

#define DIO9_PIN    PINB7
#define DIO9_RPORT  PINB
#define DIO9_WPORT  PORTB
#define DIO9_DDR    DDRB
#define DIO9_PWM    &OCR0AL
#define PinB7       9

#define DIO10_PIN   PINB1
#define DIO10_RPORT PINB
#define DIO10_WPORT PORTB
#define DIO10_DDR   DDRB
#define DIO10_PWM   nullptr
#define PinB1       10

#define DIO11_PIN   PINB2
#define DIO11_RPORT PINB
#define DIO11_WPORT PORTB
#define DIO11_DDR   DDRB
#define DIO11_PWM   nullptr
#define PinB2       11

#define DIO12_PIN   PINB3
#define DIO12_RPORT PINB
#define DIO12_WPORT PORTB
#define DIO12_DDR   DDRB
#define DIO12_PWM   nullptr
#define PinB3       12

#define DIO13_PIN   PINE2
#define DIO13_RPORT PINE
#define DIO13_WPORT PORTE
#define DIO13_DDR   DDRE
#define DIO13_PWM   nullptr
#define PinE2       13

#define DIO14_PIN   PINE6
#define DIO14_RPORT PINE
#define DIO14_WPORT PORTE
#define DIO14_DDR   DDRE
#define DIO14_PWM   nullptr
#define PinE6       14

#define DIO15_PIN   PINE7
#define DIO15_RPORT PINE
#define DIO15_WPORT PORTE
#define DIO15_DDR   DDRE
#define DIO15_PWM   nullptr
#define PinE7       15

#define DIO16_PIN   PINB0
#define DIO16_RPORT PINB
#define DIO16_WPORT PORTB
#define DIO16_DDR   DDRB
#define DIO16_PWM   nullptr
#define PinB0       16

#define DIO17_PIN   PIND0
#define DIO17_RPORT PIND
#define DIO17_WPORT PORTD
#define DIO17_DDR   DDRD
#define DIO17_PWM   nullptr
#define PinD0       17

#define DIO18_PIN   PIND1
#define DIO18_RPORT PIND
#define DIO18_WPORT PORTD
#define DIO18_DDR   DDRD
#define DIO18_PWM   nullptr
#define PinD1       18

#define DIO19_PIN   PIND2
#define DIO19_RPORT PIND
#define DIO19_WPORT PORTD
#define DIO19_DDR   DDRD
#define DIO19_PWM   nullptr
#define PinD2       19

#define DIO20_PIN   PIND3
#define DIO20_RPORT PIND
#define DIO20_WPORT PORTD
#define DIO20_DDR   DDRD
#define DIO20_PWM   nullptr
#define PinD3       20

#define DIO21_PIN   PIND4
#define DIO21_RPORT PIND
#define DIO21_WPORT PORTD
#define DIO21_DDR   DDRD
#define DIO21_PWM   nullptr
#define PinD4       21

#define DIO22_PIN   PIND5
#define DIO22_RPORT PIND
#define DIO22_WPORT PORTD
#define DIO22_DDR   DDRD
#define DIO22_PWM   nullptr
#define PinD5       22

#define DIO23_PIN   PIND6
#define DIO23_RPORT PIND
#define DIO23_WPORT PORTD
#define DIO23_DDR   DDRD
#define DIO23_PWM   nullptr
#define PinD6       23

#define DIO24_PIN   PIND7
#define DIO24_RPORT PIND
#define DIO24_WPORT PORTD
#define DIO24_DDR   DDRD
#define DIO24_PWM   nullptr
#define PinD7       24

#define DIO25_PIN   PING0
#define DIO25_RPORT PING
#define DIO25_WPORT PORTG
#define DIO25_DDR   DDRG
#define DIO25_PWM   nullptr
#define PinG0       25

#define DIO26_PIN   PING1
#define DIO26_RPORT PING
#define DIO26_WPORT PORTG
#define DIO26_DDR   DDRG
#define DIO26_PWM   nullptr
#define PinG1       26

#define DIO27_PIN   PING2
#define DIO27_RPORT PING
#define DIO27_WPORT PORTG
#define DIO27_DDR   DDRG
#define DIO27_PWM   nullptr
#define PinG2       27

#define DIO28_PIN   PING3
#define DIO28_RPORT PING
#define DIO28_WPORT PORTG
#define DIO28_DDR   DDRG
#define DIO28_PWM   nullptr
#define PinG3       28

#define DIO29_PIN   PING4
#define DIO29_RPORT PING
#define DIO29_WPORT PORTG
#define DIO29_DDR   DDRG
#define DIO29_PWM   nullptr
#define PinG4       29

#define DIO30_PIN   PINC0
#define DIO30_RPORT PINC
#define DIO30_WPORT PORTC
#define DIO30_DDR   DDRC
#define DIO30_PWM   nullptr
#define PinC0       30

#define DIO31_PIN   PINC1
#define DIO31_RPORT PINC
#define DIO31_WPORT PORTC
#define DIO31_DDR   DDRC
#define DIO31_PWM   nullptr
#define PinC1       31

#define DIO32_PIN   PINC2
#define DIO32_RPORT PINC
#define DIO32_WPORT PORTC
#define DIO32_DDR   DDRC
#define DIO32_PWM   nullptr
#define PinC2       32

#define DIO33_PIN   PINC3
#define DIO33_RPORT PINC
#define DIO33_WPORT PORTC
#define DIO33_DDR   DDRC
#define DIO33_PWM   nullptr
#define PinC3       33

#define DIO34_PIN   PINC4
#define DIO34_RPORT PINC
#define DIO34_WPORT PORTC
#define DIO34_DDR   DDRC
#define DIO34_PWM   nullptr
#define PinC4       34

#define DIO35_PIN   PINC5
#define DIO35_RPORT PINC
#define DIO35_WPORT PORTC
#define DIO35_DDR   DDRC
#define DIO35_PWM   nullptr
#define PinC5       35

#define DIO36_PIN   PINC6
#define DIO36_RPORT PINC
#define DIO36_WPORT PORTC
#define DIO36_DDR   DDRC
#define DIO36_PWM   nullptr
#define PinC6       36

#define DIO37_PIN   PINC7
#define DIO37_RPORT PINC
#define DIO37_WPORT PORTC
#define DIO37_DDR   DDRC
#define DIO37_PWM   nullptr
#define PinC7       37

#define DIO38_PIN   PINA0
#define DIO38_RPORT PINA
#define DIO38_WPORT PORTA
#define DIO38_DDR   DDRA
#define DIO38_PWM   nullptr
#define PinA0       38

#define DIO39_PIN   PINA1
#define DIO39_RPORT PINA
#define DIO39_WPORT PORTA
#define DIO39_DDR   DDRA
#define DIO39_PWM   nullptr
#define PinA1       39

#define DIO40_PIN   PINA2
#define DIO40_RPORT PINA
#define DIO40_WPORT PORTA
#define DIO40_DDR   DDRA
#define DIO40_PWM   nullptr
#define PinA2       40

#define DIO41_PIN   PINA3
#define DIO41_RPORT PINA
#define DIO41_WPORT PORTA
#define DIO41_DDR   DDRA
#define DIO41_PWM   nullptr
#define PinA3       41

#define DIO42_PIN   PINA4
#define DIO42_RPORT PINA
#define DIO42_WPORT PORTA
#define DIO42_DDR   DDRA
#define DIO42_PWM   nullptr
#define PinA4       42

#define DIO43_PIN   PINA5
#define DIO43_RPORT PINA
#define DIO43_WPORT PORTA
#define DIO43_DDR   DDRA
#define DIO43_PWM   nullptr
#define PinA5       43

#define DIO44_PIN   PINA6
#define DIO44_RPORT PINA
#define DIO44_WPORT PORTA
#define DIO44_DDR   DDRA
#define DIO44_PWM   nullptr
#define PinA6       44

#define DIO45_PIN   PINA7
#define DIO45_RPORT PINA
#define DIO45_WPORT PORTA
#define DIO45_DDR   DDRA
#define DIO45_PWM   nullptr
#define PinA7       45

#define DIO46_PIN   PINF0
#define DIO46_RPORT PINF
#define DIO46_WPORT PORTF
#define DIO46_DDR   DDRF
#define DIO46_PWM   nullptr
#define PinF0       46

#define DIO47_PIN   PINF1
#define DIO47_RPORT PINF
#define DIO47_WPORT PORTF
#define DIO47_DDR   DDRF
#define DIO47_PWM   nullptr
#define PinF1       47

#define DIO48_PIN   PINF2
#define DIO48_RPORT PINF
#define DIO48_WPORT PORTF
#define DIO48_DDR   DDRF
#define DIO48_PWM   nullptr
#define PinF2       48

#define DIO49_PIN   PINF3
#define DIO49_RPORT PINF
#define DIO49_WPORT PORTF
#define DIO49_DDR   DDRF
#define DIO49_PWM   nullptr
#define PinF3       49

#define DIO50_PIN   PINF4
#define DIO50_RPORT PINF
#define DIO50_WPORT PORTF
#define DIO50_DDR   DDRF
#define DIO50_PWM   nullptr
#define PinF4       50

#define DIO51_PIN   PINF5
#define DIO51_RPORT PINF
#define DIO51_WPORT PORTF
#define DIO51_DDR   DDRF
#define DIO51_PWM   nullptr
#define PinF5       51

#define DIO52_PIN   PINF6
#define DIO52_RPORT PINF
#define DIO52_WPORT PORTF
#define DIO52_DDR   DDRF
#define DIO52_PWM   nullptr
#define PinF6       52

#define DIO53_PIN   PINF7
#define DIO53_RPORT PINF
#define DIO53_WPORT PORTF
#define DIO53_DDR   DDRF
#define DIO53_PWM   nullptr
#define PinF7       53

#define DIO_NUM 54