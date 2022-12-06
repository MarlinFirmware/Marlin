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
 * Pin mapping for the 1280 and 2560
 *
 *   Hardware Pin  : 02 03 06 07 01 05 15 16 17 18 23 24 25 26 64 63 13 12 46 45 44 43 78 77 76 75 74 73 72 71 60 59 58 57 56 55 54 53 50 70 52 51 42 41 40 39 38 37 36 35 22 21 20 19 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 | 04 08 09 10 11 14 27 28 29 30 31 32 33 34 47 48 49 61 62 65 66 67 68 69 79 80 81 98 99 100
 *   Port          : E0 E1 E4 E5 G5 E3 H3 H4 H5 H6 B4 B5 B6 B7 J1 J0 H1 H0 D3 D2 D1 D0 A0 A1 A2 A3 A4 A5 A6 A7 C7 C6 C5 C4 C3 C2 C1 C0 D7 G2 G1 G0 L7 L6 L5 L4 L3 L2 L1 L0 B3 B2 B1 B0 F0 F1 F2 F3 F4 F5 F6 F7 K0 K1 K2 K3 K4 K5 K6 K7 | E2 E6 E7 xx xx H2 H7 G3 G4 xx xx xx xx xx D4 D5 D6 xx xx J2 J3 J4 J5 J6 J7 xx xx xx xx xx
 *   Logical Pin   : 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 | 78 79 80 xx xx 84 85 71 70 xx xx xx xx xx 81 82 83 xx xx 72 73 75 76 77 74 xx xx xx xx xx
 */

#include "../fastio.h"

// change for your board
#define DEBUG_LED   DIO21

// UART
#define RXD         DIO0
#define TXD         DIO1

// SPI
#define SCK         DIO52
#define MISO        DIO50
#define MOSI        DIO51
#define SS          DIO53

// TWI (I2C)
#define SCL         DIO21
#define SDA         DIO20

// Timers and PWM
#define OC0A        DIO13
#define OC0B        DIO4
#define OC1A        DIO11
#define OC1B        DIO12
#define OC2A        DIO10
#define OC2B        DIO9
#define OC3A        DIO5
#define OC3B        DIO2
#define OC3C        DIO3
#define OC4A        DIO6
#define OC4B        DIO7
#define OC4C        DIO8
#define OC5A        DIO46
#define OC5B        DIO45
#define OC5C        DIO44

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

#define DIO6_PIN    PINH3
#define DIO6_RPORT  PINH
#define DIO6_WPORT  PORTH
#define DIO6_DDR    DDRH
#define DIO6_PWM    &OCR4AL
#define PinH3       6

#define DIO7_PIN    PINH4
#define DIO7_RPORT  PINH
#define DIO7_WPORT  PORTH
#define DIO7_DDR    DDRH
#define DIO7_PWM    &OCR4BL
#define PinH4       7

#define DIO8_PIN    PINH5
#define DIO8_RPORT  PINH
#define DIO8_WPORT  PORTH
#define DIO8_DDR    DDRH
#define DIO8_PWM    &OCR4CL
#define PinH5       8

#define DIO9_PIN    PINH6
#define DIO9_RPORT  PINH
#define DIO9_WPORT  PORTH
#define DIO9_DDR    DDRH
#define DIO9_PWM    &OCR2B
#define PinH6       9

#define DIO10_PIN   PINB4
#define DIO10_RPORT PINB
#define DIO10_WPORT PORTB
#define DIO10_DDR   DDRB
#define DIO10_PWM   &OCR2A
#define PinB4       10

#define DIO11_PIN   PINB5
#define DIO11_RPORT PINB
#define DIO11_WPORT PORTB
#define DIO11_DDR   DDRB
#define DIO11_PWM   nullptr
#define PinB5       11

#define DIO12_PIN   PINB6
#define DIO12_RPORT PINB
#define DIO12_WPORT PORTB
#define DIO12_DDR   DDRB
#define DIO12_PWM   nullptr
#define PinB6       12

#define DIO13_PIN   PINB7
#define DIO13_RPORT PINB
#define DIO13_WPORT PORTB
#define DIO13_DDR   DDRB
#define DIO13_PWM   &OCR0A
#define PinB7       13

#define DIO14_PIN   PINJ1
#define DIO14_RPORT PINJ
#define DIO14_WPORT PORTJ
#define DIO14_DDR   DDRJ
#define DIO14_PWM   nullptr
#define PinJ1       14

#define DIO15_PIN   PINJ0
#define DIO15_RPORT PINJ
#define DIO15_WPORT PORTJ
#define DIO15_DDR   DDRJ
#define DIO15_PWM   nullptr
#define PinJ0       15

#define DIO16_PIN   PINH1
#define DIO16_RPORT PINH
#define DIO16_WPORT PORTH
#define DIO16_DDR   DDRH
#define DIO16_PWM   nullptr
#define PinH1       16

#define DIO17_PIN   PINH0
#define DIO17_RPORT PINH
#define DIO17_WPORT PORTH
#define DIO17_DDR   DDRH
#define DIO17_PWM   nullptr
#define PinH0       17

#define DIO18_PIN   PIND3
#define DIO18_RPORT PIND
#define DIO18_WPORT PORTD
#define DIO18_DDR   DDRD
#define DIO18_PWM   nullptr
#define PinD3       18

#define DIO19_PIN   PIND2
#define DIO19_RPORT PIND
#define DIO19_WPORT PORTD
#define DIO19_DDR   DDRD
#define DIO19_PWM   nullptr
#define PinD2       19

#define DIO20_PIN   PIND1
#define DIO20_RPORT PIND
#define DIO20_WPORT PORTD
#define DIO20_DDR   DDRD
#define DIO20_PWM   nullptr
#define PinD1       20

#define DIO21_PIN   PIND0
#define DIO21_RPORT PIND
#define DIO21_WPORT PORTD
#define DIO21_DDR   DDRD
#define DIO21_PWM   nullptr
#define PinD0       21

#define DIO22_PIN   PINA0
#define DIO22_RPORT PINA
#define DIO22_WPORT PORTA
#define DIO22_DDR   DDRA
#define DIO22_PWM   nullptr
#define PinA0       22

#define DIO23_PIN   PINA1
#define DIO23_RPORT PINA
#define DIO23_WPORT PORTA
#define DIO23_DDR   DDRA
#define DIO23_PWM   nullptr
#define PinA1       23

#define DIO24_PIN   PINA2
#define DIO24_RPORT PINA
#define DIO24_WPORT PORTA
#define DIO24_DDR   DDRA
#define DIO24_PWM   nullptr
#define PinA2       24

#define DIO25_PIN   PINA3
#define DIO25_RPORT PINA
#define DIO25_WPORT PORTA
#define DIO25_DDR   DDRA
#define DIO25_PWM   nullptr
#define PinA3       25

#define DIO26_PIN   PINA4
#define DIO26_RPORT PINA
#define DIO26_WPORT PORTA
#define DIO26_DDR   DDRA
#define DIO26_PWM   nullptr
#define PinA4       26

#define DIO27_PIN   PINA5
#define DIO27_RPORT PINA
#define DIO27_WPORT PORTA
#define DIO27_DDR   DDRA
#define DIO27_PWM   nullptr
#define PinA5       27

#define DIO28_PIN   PINA6
#define DIO28_RPORT PINA
#define DIO28_WPORT PORTA
#define DIO28_DDR   DDRA
#define DIO28_PWM   nullptr
#define PinA6       28

#define DIO29_PIN   PINA7
#define DIO29_RPORT PINA
#define DIO29_WPORT PORTA
#define DIO29_DDR   DDRA
#define DIO29_PWM   nullptr
#define PinA7       29

#define DIO30_PIN   PINC7
#define DIO30_RPORT PINC
#define DIO30_WPORT PORTC
#define DIO30_DDR   DDRC
#define DIO30_PWM   nullptr
#define PinC7       30

#define DIO31_PIN   PINC6
#define DIO31_RPORT PINC
#define DIO31_WPORT PORTC
#define DIO31_DDR   DDRC
#define DIO31_PWM   nullptr
#define PinC6       31

#define DIO32_PIN   PINC5
#define DIO32_RPORT PINC
#define DIO32_WPORT PORTC
#define DIO32_DDR   DDRC
#define DIO32_PWM   nullptr
#define PinC5       32

#define DIO33_PIN   PINC4
#define DIO33_RPORT PINC
#define DIO33_WPORT PORTC
#define DIO33_DDR   DDRC
#define DIO33_PWM   nullptr
#define PinC4       33

#define DIO34_PIN   PINC3
#define DIO34_RPORT PINC
#define DIO34_WPORT PORTC
#define DIO34_DDR   DDRC
#define DIO34_PWM   nullptr
#define PinC3       34

#define DIO35_PIN   PINC2
#define DIO35_RPORT PINC
#define DIO35_WPORT PORTC
#define DIO35_DDR   DDRC
#define DIO35_PWM   nullptr
#define PinC2       35

#define DIO36_PIN   PINC1
#define DIO36_RPORT PINC
#define DIO36_WPORT PORTC
#define DIO36_DDR   DDRC
#define DIO36_PWM   nullptr
#define PinC1       36

#define DIO37_PIN   PINC0
#define DIO37_RPORT PINC
#define DIO37_WPORT PORTC
#define DIO37_DDR   DDRC
#define DIO37_PWM   nullptr
#define PinC0       37

#define DIO38_PIN   PIND7
#define DIO38_RPORT PIND
#define DIO38_WPORT PORTD
#define DIO38_DDR   DDRD
#define DIO38_PWM   nullptr
#define PinD7       38

#define DIO39_PIN   PING2
#define DIO39_RPORT PING
#define DIO39_WPORT PORTG
#define DIO39_DDR   DDRG
#define DIO39_PWM   nullptr
#define PinG2       39

#define DIO40_PIN   PING1
#define DIO40_RPORT PING
#define DIO40_WPORT PORTG
#define DIO40_DDR   DDRG
#define DIO40_PWM   nullptr
#define PinG1       40

#define DIO41_PIN   PING0
#define DIO41_RPORT PING
#define DIO41_WPORT PORTG
#define DIO41_DDR   DDRG
#define DIO41_PWM   nullptr
#define PinG0       41

#define DIO42_PIN   PINL7
#define DIO42_RPORT PINL
#define DIO42_WPORT PORTL
#define DIO42_DDR   DDRL
#define DIO42_PWM   nullptr
#define PinL7       42

#define DIO43_PIN   PINL6
#define DIO43_RPORT PINL
#define DIO43_WPORT PORTL
#define DIO43_DDR   DDRL
#define DIO43_PWM   nullptr
#define PinL6       43

#define DIO44_PIN   PINL5
#define DIO44_RPORT PINL
#define DIO44_WPORT PORTL
#define DIO44_DDR   DDRL
#define DIO44_PWM   &OCR5CL
#define PinL5       44

#define DIO45_PIN   PINL4
#define DIO45_RPORT PINL
#define DIO45_WPORT PORTL
#define DIO45_DDR   DDRL
#define DIO45_PWM   &OCR5BL
#define PinL4       45

#define DIO46_PIN   PINL3
#define DIO46_RPORT PINL
#define DIO46_WPORT PORTL
#define DIO46_DDR   DDRL
#define DIO46_PWM   &OCR5AL
#define PinL3       46

#define DIO47_PIN   PINL2
#define DIO47_RPORT PINL
#define DIO47_WPORT PORTL
#define DIO47_DDR   DDRL
#define DIO47_PWM   nullptr
#define PinL2       47

#define DIO48_PIN   PINL1
#define DIO48_RPORT PINL
#define DIO48_WPORT PORTL
#define DIO48_DDR   DDRL
#define DIO48_PWM   nullptr
#define PinL1       48

#define DIO49_PIN   PINL0
#define DIO49_RPORT PINL
#define DIO49_WPORT PORTL
#define DIO49_DDR   DDRL
#define DIO49_PWM   nullptr
#define PinL0       49

#define DIO50_PIN   PINB3
#define DIO50_RPORT PINB
#define DIO50_WPORT PORTB
#define DIO50_DDR   DDRB
#define DIO50_PWM   nullptr
#define PinB3       50

#define DIO51_PIN   PINB2
#define DIO51_RPORT PINB
#define DIO51_WPORT PORTB
#define DIO51_DDR   DDRB
#define DIO51_PWM   nullptr
#define PinB2       51

#define DIO52_PIN   PINB1
#define DIO52_RPORT PINB
#define DIO52_WPORT PORTB
#define DIO52_DDR   DDRB
#define DIO52_PWM   nullptr
#define PinB1       52

#define DIO53_PIN   PINB0
#define DIO53_RPORT PINB
#define DIO53_WPORT PORTB
#define DIO53_DDR   DDRB
#define DIO53_PWM   nullptr
#define PinB0       53

#define DIO54_PIN   PINF0
#define DIO54_RPORT PINF
#define DIO54_WPORT PORTF
#define DIO54_DDR   DDRF
#define DIO54_PWM   nullptr
#define PinF0       54

#define DIO55_PIN   PINF1
#define DIO55_RPORT PINF
#define DIO55_WPORT PORTF
#define DIO55_DDR   DDRF
#define DIO55_PWM   nullptr
#define PinF1       55

#define DIO56_PIN   PINF2
#define DIO56_RPORT PINF
#define DIO56_WPORT PORTF
#define DIO56_DDR   DDRF
#define DIO56_PWM   nullptr
#define PinF2       56

#define DIO57_PIN   PINF3
#define DIO57_RPORT PINF
#define DIO57_WPORT PORTF
#define DIO57_DDR   DDRF
#define DIO57_PWM   nullptr
#define PinF3       57

#define DIO58_PIN   PINF4
#define DIO58_RPORT PINF
#define DIO58_WPORT PORTF
#define DIO58_DDR   DDRF
#define DIO58_PWM   nullptr
#define PinF4       58

#define DIO59_PIN   PINF5
#define DIO59_RPORT PINF
#define DIO59_WPORT PORTF
#define DIO59_DDR   DDRF
#define DIO59_PWM   nullptr
#define PinF5       59

#define DIO60_PIN   PINF6
#define DIO60_RPORT PINF
#define DIO60_WPORT PORTF
#define DIO60_DDR   DDRF
#define DIO60_PWM   nullptr
#define PinF6       60

#define DIO61_PIN   PINF7
#define DIO61_RPORT PINF
#define DIO61_WPORT PORTF
#define DIO61_DDR   DDRF
#define DIO61_PWM   nullptr
#define PinF7       61

#define DIO62_PIN   PINK0
#define DIO62_RPORT PINK
#define DIO62_WPORT PORTK
#define DIO62_DDR   DDRK
#define DIO62_PWM   nullptr
#define PinK0       62

#define DIO63_PIN   PINK1
#define DIO63_RPORT PINK
#define DIO63_WPORT PORTK
#define DIO63_DDR   DDRK
#define DIO63_PWM   nullptr
#define PinK1       63

#define DIO64_PIN   PINK2
#define DIO64_RPORT PINK
#define DIO64_WPORT PORTK
#define DIO64_DDR   DDRK
#define DIO64_PWM   nullptr
#define PinK2       64

#define DIO65_PIN   PINK3
#define DIO65_RPORT PINK
#define DIO65_WPORT PORTK
#define DIO65_DDR   DDRK
#define DIO65_PWM   nullptr
#define PinK3       65

#define DIO66_PIN   PINK4
#define DIO66_RPORT PINK
#define DIO66_WPORT PORTK
#define DIO66_DDR   DDRK
#define DIO66_PWM   nullptr
#define PinK4       66

#define DIO67_PIN   PINK5
#define DIO67_RPORT PINK
#define DIO67_WPORT PORTK
#define DIO67_DDR   DDRK
#define DIO67_PWM   nullptr
#define PinK5       67

#define DIO68_PIN   PINK6
#define DIO68_RPORT PINK
#define DIO68_WPORT PORTK
#define DIO68_DDR   DDRK
#define DIO68_PWM   nullptr
#define PinK6       68

#define DIO69_PIN   PINK7
#define DIO69_RPORT PINK
#define DIO69_WPORT PORTK
#define DIO69_DDR   DDRK
#define DIO69_PWM   nullptr
#define PinK7       69

#define DIO70_PIN   PING4
#define DIO70_RPORT PING
#define DIO70_WPORT PORTG
#define DIO70_DDR   DDRG
#define DIO70_PWM   nullptr
#define PinG4       70

#define DIO71_PIN   PING3
#define DIO71_RPORT PING
#define DIO71_WPORT PORTG
#define DIO71_DDR   DDRG
#define DIO71_PWM   nullptr
#define PinG3       71

#define DIO72_PIN   PINJ2
#define DIO72_RPORT PINJ
#define DIO72_WPORT PORTJ
#define DIO72_DDR   DDRJ
#define DIO72_PWM   nullptr
#define PinJ2       72

#define DIO73_PIN   PINJ3
#define DIO73_RPORT PINJ
#define DIO73_WPORT PORTJ
#define DIO73_DDR   DDRJ
#define DIO73_PWM   nullptr
#define PinJ3       73

#define DIO74_PIN   PINJ7
#define DIO74_RPORT PINJ
#define DIO74_WPORT PORTJ
#define DIO74_DDR   DDRJ
#define DIO74_PWM   nullptr
#define PinJ7       74

#define DIO75_PIN   PINJ4
#define DIO75_RPORT PINJ
#define DIO75_WPORT PORTJ
#define DIO75_DDR   DDRJ
#define DIO75_PWM   nullptr
#define PinJ4       75

#define DIO76_PIN   PINJ5
#define DIO76_RPORT PINJ
#define DIO76_WPORT PORTJ
#define DIO76_DDR   DDRJ
#define DIO76_PWM   nullptr
#define PinJ5       76

#define DIO77_PIN   PINJ6
#define DIO77_RPORT PINJ
#define DIO77_WPORT PORTJ
#define DIO77_DDR   DDRJ
#define DIO77_PWM   nullptr
#define PinJ6       77

#define DIO78_PIN   PINE2
#define DIO78_RPORT PINE
#define DIO78_WPORT PORTE
#define DIO78_DDR   DDRE
#define DIO78_PWM   nullptr
#define PinE2       78

#define DIO79_PIN   PINE6
#define DIO79_RPORT PINE
#define DIO79_WPORT PORTE
#define DIO79_DDR   DDRE
#define DIO79_PWM   nullptr
#define PinE6       79

#define DIO80_PIN   PINE7
#define DIO80_RPORT PINE
#define DIO80_WPORT PORTE
#define DIO80_DDR   DDRE
#define DIO80_PWM   nullptr
#define PinE7       80

#define DIO81_PIN   PIND4
#define DIO81_RPORT PIND
#define DIO81_WPORT PORTD
#define DIO81_DDR   DDRD
#define DIO81_PWM   nullptr
#define PinD4       81

#define DIO82_PIN   PIND5
#define DIO82_RPORT PIND
#define DIO82_WPORT PORTD
#define DIO82_DDR   DDRD
#define DIO82_PWM   nullptr
#define PinD5       82

#define DIO83_PIN   PIND6
#define DIO83_RPORT PIND
#define DIO83_WPORT PORTD
#define DIO83_DDR   DDRD
#define DIO83_PWM   nullptr
#define PinD6       83

#define DIO84_PIN   PINH2
#define DIO84_RPORT PINH
#define DIO84_WPORT PORTH
#define DIO84_DDR   DDRH
#define DIO84_PWM   nullptr
#define PinH2       84

#define DIO85_PIN   PINH7
#define DIO85_RPORT PINH
#define DIO85_WPORT PORTH
#define DIO85_DDR   DDRH
#define DIO85_PWM   nullptr
#define PinH7       85

#define DIO_NUM 86