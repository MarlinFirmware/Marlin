/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * Pin mapping for the 1280 and 2560
 *
 *   Hardware Pin  : 02 03 06 07 01 05 15 16 17 18 23 24 25 26 64 63 13 12 46 45 44 43 78 77 76 75 74 73 72 71 60 59 58 57 56 55 54 53 50 70 52 51 42 41 40 39 38 37 36 35 22 21 20 19 97 96 95 94 93 92 91 90 89 88 87 86 85 84 83 82 | 04 08 09 10 11 14 27 28 29 30 31 32 33 34 47 48 49 61 62 65 66 67 68 69 79 80 81 98 99 100
 *   Port          : E0 E1 E4 E5 G5 E3 H3 H4 H5 H6 B4 B5 B6 B7 J1 J0 H1 H0 D3 D2 D1 D0 A0 A1 A2 A3 A4 A5 A6 A7 C7 C6 C5 C4 C3 C2 C1 C0 D7 G2 G1 G0 L7 L6 L5 L4 L3 L2 L1 L0 B3 B2 B1 B0 F0 F1 F2 F3 F4 F5 F6 F7 K0 K1 K2 K3 K4 K5 K6 K7 | E2 E6 E7 xx xx H2 H7 G3 G4 xx xx xx xx xx D4 D5 D6 xx xx J2 J3 J4 J5 J6 J7 xx xx xx xx xx
 *   Logical Pin   : 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 | 78 79 80 xx xx 84 85 71 70 xx xx xx xx xx 81 82 83 xx xx 72 72 75 76 77 74 xx xx xx xx xx
 */

#include "fastio_AVR.h"

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

#define DIO1_PIN    PINE1
#define DIO1_RPORT  PINE
#define DIO1_WPORT  PORTE
#define DIO1_DDR    DDRE
#define DIO1_PWM    nullptr

#define DIO2_PIN    PINE4
#define DIO2_RPORT  PINE
#define DIO2_WPORT  PORTE
#define DIO2_DDR    DDRE
#define DIO2_PWM    &OCR3BL

#define DIO3_PIN    PINE5
#define DIO3_RPORT  PINE
#define DIO3_WPORT  PORTE
#define DIO3_DDR    DDRE
#define DIO3_PWM    &OCR3CL

#define DIO4_PIN    PING5
#define DIO4_RPORT  PING
#define DIO4_WPORT  PORTG
#define DIO4_DDR    DDRG
#define DIO4_PWM    &OCR0B

#define DIO5_PIN    PINE3
#define DIO5_RPORT  PINE
#define DIO5_WPORT  PORTE
#define DIO5_DDR    DDRE
#define DIO5_PWM    &OCR3AL

#define DIO6_PIN    PINH3
#define DIO6_RPORT  PINH
#define DIO6_WPORT  PORTH
#define DIO6_DDR    DDRH
#define DIO6_PWM    &OCR4AL

#define DIO7_PIN    PINH4
#define DIO7_RPORT  PINH
#define DIO7_WPORT  PORTH
#define DIO7_DDR    DDRH
#define DIO7_PWM    &OCR4BL

#define DIO8_PIN    PINH5
#define DIO8_RPORT  PINH
#define DIO8_WPORT  PORTH
#define DIO8_DDR    DDRH
#define DIO8_PWM    &OCR4CL

#define DIO9_PIN    PINH6
#define DIO9_RPORT  PINH
#define DIO9_WPORT  PORTH
#define DIO9_DDR    DDRH
#define DIO9_PWM    &OCR2B

#define DIO10_PIN   PINB4
#define DIO10_RPORT PINB
#define DIO10_WPORT PORTB
#define DIO10_DDR   DDRB
#define DIO10_PWM   &OCR2A

#define DIO11_PIN   PINB5
#define DIO11_RPORT PINB
#define DIO11_WPORT PORTB
#define DIO11_DDR   DDRB
#define DIO11_PWM   nullptr

#define DIO12_PIN   PINB6
#define DIO12_RPORT PINB
#define DIO12_WPORT PORTB
#define DIO12_DDR   DDRB
#define DIO12_PWM   nullptr

#define DIO13_PIN   PINB7
#define DIO13_RPORT PINB
#define DIO13_WPORT PORTB
#define DIO13_DDR   DDRB
#define DIO13_PWM   &OCR0A

#define DIO14_PIN   PINJ1
#define DIO14_RPORT PINJ
#define DIO14_WPORT PORTJ
#define DIO14_DDR   DDRJ
#define DIO14_PWM   nullptr

#define DIO15_PIN   PINJ0
#define DIO15_RPORT PINJ
#define DIO15_WPORT PORTJ
#define DIO15_DDR   DDRJ
#define DIO15_PWM   nullptr

#define DIO16_PIN   PINH1
#define DIO16_RPORT PINH
#define DIO16_WPORT PORTH
#define DIO16_DDR   DDRH
#define DIO16_PWM   nullptr

#define DIO17_PIN   PINH0
#define DIO17_RPORT PINH
#define DIO17_WPORT PORTH
#define DIO17_DDR   DDRH
#define DIO17_PWM   nullptr

#define DIO18_PIN   PIND3
#define DIO18_RPORT PIND
#define DIO18_WPORT PORTD
#define DIO18_DDR   DDRD
#define DIO18_PWM   nullptr

#define DIO19_PIN   PIND2
#define DIO19_RPORT PIND
#define DIO19_WPORT PORTD
#define DIO19_DDR   DDRD
#define DIO19_PWM   nullptr

#define DIO20_PIN   PIND1
#define DIO20_RPORT PIND
#define DIO20_WPORT PORTD
#define DIO20_DDR   DDRD
#define DIO20_PWM   nullptr

#define DIO21_PIN   PIND0
#define DIO21_RPORT PIND
#define DIO21_WPORT PORTD
#define DIO21_DDR   DDRD
#define DIO21_PWM   nullptr

#define DIO22_PIN   PINA0
#define DIO22_RPORT PINA
#define DIO22_WPORT PORTA
#define DIO22_DDR   DDRA
#define DIO22_PWM   nullptr

#define DIO23_PIN   PINA1
#define DIO23_RPORT PINA
#define DIO23_WPORT PORTA
#define DIO23_DDR   DDRA
#define DIO23_PWM   nullptr

#define DIO24_PIN   PINA2
#define DIO24_RPORT PINA
#define DIO24_WPORT PORTA
#define DIO24_DDR   DDRA
#define DIO24_PWM   nullptr

#define DIO25_PIN   PINA3
#define DIO25_RPORT PINA
#define DIO25_WPORT PORTA
#define DIO25_DDR   DDRA
#define DIO25_PWM   nullptr

#define DIO26_PIN   PINA4
#define DIO26_RPORT PINA
#define DIO26_WPORT PORTA
#define DIO26_DDR   DDRA
#define DIO26_PWM   nullptr

#define DIO27_PIN   PINA5
#define DIO27_RPORT PINA
#define DIO27_WPORT PORTA
#define DIO27_DDR   DDRA
#define DIO27_PWM   nullptr

#define DIO28_PIN   PINA6
#define DIO28_RPORT PINA
#define DIO28_WPORT PORTA
#define DIO28_DDR   DDRA
#define DIO28_PWM   nullptr

#define DIO29_PIN   PINA7
#define DIO29_RPORT PINA
#define DIO29_WPORT PORTA
#define DIO29_DDR   DDRA
#define DIO29_PWM   nullptr

#define DIO30_PIN   PINC7
#define DIO30_RPORT PINC
#define DIO30_WPORT PORTC
#define DIO30_DDR   DDRC
#define DIO30_PWM   nullptr

#define DIO31_PIN   PINC6
#define DIO31_RPORT PINC
#define DIO31_WPORT PORTC
#define DIO31_DDR   DDRC
#define DIO31_PWM   nullptr

#define DIO32_PIN   PINC5
#define DIO32_RPORT PINC
#define DIO32_WPORT PORTC
#define DIO32_DDR   DDRC
#define DIO32_PWM   nullptr

#define DIO33_PIN   PINC4
#define DIO33_RPORT PINC
#define DIO33_WPORT PORTC
#define DIO33_DDR   DDRC
#define DIO33_PWM   nullptr

#define DIO34_PIN   PINC3
#define DIO34_RPORT PINC
#define DIO34_WPORT PORTC
#define DIO34_DDR   DDRC
#define DIO34_PWM   nullptr

#define DIO35_PIN   PINC2
#define DIO35_RPORT PINC
#define DIO35_WPORT PORTC
#define DIO35_DDR   DDRC
#define DIO35_PWM   nullptr

#define DIO36_PIN   PINC1
#define DIO36_RPORT PINC
#define DIO36_WPORT PORTC
#define DIO36_DDR   DDRC
#define DIO36_PWM   nullptr

#define DIO37_PIN   PINC0
#define DIO37_RPORT PINC
#define DIO37_WPORT PORTC
#define DIO37_DDR   DDRC
#define DIO37_PWM   nullptr

#define DIO38_PIN   PIND7
#define DIO38_RPORT PIND
#define DIO38_WPORT PORTD
#define DIO38_DDR   DDRD
#define DIO38_PWM   nullptr

#define DIO39_PIN   PING2
#define DIO39_RPORT PING
#define DIO39_WPORT PORTG
#define DIO39_DDR   DDRG
#define DIO39_PWM   nullptr

#define DIO40_PIN   PING1
#define DIO40_RPORT PING
#define DIO40_WPORT PORTG
#define DIO40_DDR   DDRG
#define DIO40_PWM   nullptr

#define DIO41_PIN   PING0
#define DIO41_RPORT PING
#define DIO41_WPORT PORTG
#define DIO41_DDR   DDRG
#define DIO41_PWM   nullptr

#define DIO42_PIN   PINL7
#define DIO42_RPORT PINL
#define DIO42_WPORT PORTL
#define DIO42_DDR   DDRL
#define DIO42_PWM   nullptr

#define DIO43_PIN   PINL6
#define DIO43_RPORT PINL
#define DIO43_WPORT PORTL
#define DIO43_DDR   DDRL
#define DIO43_PWM   nullptr

#define DIO44_PIN   PINL5
#define DIO44_RPORT PINL
#define DIO44_WPORT PORTL
#define DIO44_DDR   DDRL
#define DIO44_PWM   &OCR5CL

#define DIO45_PIN   PINL4
#define DIO45_RPORT PINL
#define DIO45_WPORT PORTL
#define DIO45_DDR   DDRL
#define DIO45_PWM   &OCR5BL

#define DIO46_PIN   PINL3
#define DIO46_RPORT PINL
#define DIO46_WPORT PORTL
#define DIO46_DDR   DDRL
#define DIO46_PWM   &OCR5AL

#define DIO47_PIN   PINL2
#define DIO47_RPORT PINL
#define DIO47_WPORT PORTL
#define DIO47_DDR   DDRL
#define DIO47_PWM   nullptr

#define DIO48_PIN   PINL1
#define DIO48_RPORT PINL
#define DIO48_WPORT PORTL
#define DIO48_DDR   DDRL
#define DIO48_PWM   nullptr

#define DIO49_PIN   PINL0
#define DIO49_RPORT PINL
#define DIO49_WPORT PORTL
#define DIO49_DDR   DDRL
#define DIO49_PWM   nullptr

#define DIO50_PIN   PINB3
#define DIO50_RPORT PINB
#define DIO50_WPORT PORTB
#define DIO50_DDR   DDRB
#define DIO50_PWM   nullptr

#define DIO51_PIN   PINB2
#define DIO51_RPORT PINB
#define DIO51_WPORT PORTB
#define DIO51_DDR   DDRB
#define DIO51_PWM   nullptr

#define DIO52_PIN   PINB1
#define DIO52_RPORT PINB
#define DIO52_WPORT PORTB
#define DIO52_DDR   DDRB
#define DIO52_PWM   nullptr

#define DIO53_PIN   PINB0
#define DIO53_RPORT PINB
#define DIO53_WPORT PORTB
#define DIO53_DDR   DDRB
#define DIO53_PWM   nullptr

#define DIO54_PIN   PINF0
#define DIO54_RPORT PINF
#define DIO54_WPORT PORTF
#define DIO54_DDR   DDRF
#define DIO54_PWM   nullptr

#define DIO55_PIN   PINF1
#define DIO55_RPORT PINF
#define DIO55_WPORT PORTF
#define DIO55_DDR   DDRF
#define DIO55_PWM   nullptr

#define DIO56_PIN   PINF2
#define DIO56_RPORT PINF
#define DIO56_WPORT PORTF
#define DIO56_DDR   DDRF
#define DIO56_PWM   nullptr

#define DIO57_PIN   PINF3
#define DIO57_RPORT PINF
#define DIO57_WPORT PORTF
#define DIO57_DDR   DDRF
#define DIO57_PWM   nullptr

#define DIO58_PIN   PINF4
#define DIO58_RPORT PINF
#define DIO58_WPORT PORTF
#define DIO58_DDR   DDRF
#define DIO58_PWM   nullptr

#define DIO59_PIN   PINF5
#define DIO59_RPORT PINF
#define DIO59_WPORT PORTF
#define DIO59_DDR   DDRF
#define DIO59_PWM   nullptr

#define DIO60_PIN   PINF6
#define DIO60_RPORT PINF
#define DIO60_WPORT PORTF
#define DIO60_DDR   DDRF
#define DIO60_PWM   nullptr

#define DIO61_PIN   PINF7
#define DIO61_RPORT PINF
#define DIO61_WPORT PORTF
#define DIO61_DDR   DDRF
#define DIO61_PWM   nullptr

#define DIO62_PIN   PINK0
#define DIO62_RPORT PINK
#define DIO62_WPORT PORTK
#define DIO62_DDR   DDRK
#define DIO62_PWM   nullptr

#define DIO63_PIN   PINK1
#define DIO63_RPORT PINK
#define DIO63_WPORT PORTK
#define DIO63_DDR   DDRK
#define DIO63_PWM   nullptr

#define DIO64_PIN   PINK2
#define DIO64_RPORT PINK
#define DIO64_WPORT PORTK
#define DIO64_DDR   DDRK
#define DIO64_PWM   nullptr

#define DIO65_PIN   PINK3
#define DIO65_RPORT PINK
#define DIO65_WPORT PORTK
#define DIO65_DDR   DDRK
#define DIO65_PWM   nullptr

#define DIO66_PIN   PINK4
#define DIO66_RPORT PINK
#define DIO66_WPORT PORTK
#define DIO66_DDR   DDRK
#define DIO66_PWM   nullptr

#define DIO67_PIN   PINK5
#define DIO67_RPORT PINK
#define DIO67_WPORT PORTK
#define DIO67_DDR   DDRK
#define DIO67_PWM   nullptr

#define DIO68_PIN   PINK6
#define DIO68_RPORT PINK
#define DIO68_WPORT PORTK
#define DIO68_DDR   DDRK
#define DIO68_PWM   nullptr

#define DIO69_PIN   PINK7
#define DIO69_RPORT PINK
#define DIO69_WPORT PORTK
#define DIO69_DDR   DDRK
#define DIO69_PWM   nullptr

//#define FASTIO_EXT_START 70
//#define FASTIO_EXT_END   85

#define DIO70_PIN   PING4
#define DIO70_RPORT PING
#define DIO70_WPORT PORTG
#define DIO70_DDR   DDRG
#define DIO70_PWM   nullptr

#define DIO71_PIN   PING3
#define DIO71_RPORT PING
#define DIO71_WPORT PORTG
#define DIO71_DDR   DDRG
#define DIO71_PWM   nullptr

#define DIO72_PIN   PINJ2
#define DIO72_RPORT PINJ
#define DIO72_WPORT PORTJ
#define DIO72_DDR   DDRJ
#define DIO72_PWM   nullptr

#define DIO73_PIN   PINJ3
#define DIO73_RPORT PINJ
#define DIO73_WPORT PORTJ
#define DIO73_DDR   DDRJ
#define DIO73_PWM   nullptr

#define DIO74_PIN   PINJ7
#define DIO74_RPORT PINJ
#define DIO74_WPORT PORTJ
#define DIO74_DDR   DDRJ
#define DIO74_PWM   nullptr

#define DIO75_PIN   PINJ4
#define DIO75_RPORT PINJ
#define DIO75_WPORT PORTJ
#define DIO75_DDR   DDRJ
#define DIO75_PWM   nullptr

#define DIO76_PIN   PINJ5
#define DIO76_RPORT PINJ
#define DIO76_WPORT PORTJ
#define DIO76_DDR   DDRJ
#define DIO76_PWM   nullptr

#define DIO77_PIN   PINJ6
#define DIO77_RPORT PINJ
#define DIO77_WPORT PORTJ
#define DIO77_DDR   DDRJ
#define DIO77_PWM   nullptr

#define DIO78_PIN   PINE2
#define DIO78_RPORT PINE
#define DIO78_WPORT PORTE
#define DIO78_DDR   DDRE
#define DIO78_PWM   nullptr

#define DIO79_PIN   PINE6
#define DIO79_RPORT PINE
#define DIO79_WPORT PORTE
#define DIO79_DDR   DDRE
#define DIO79_PWM   nullptr

#define DIO80_PIN   PINE7
#define DIO80_RPORT PINE
#define DIO80_WPORT PORTE
#define DIO80_DDR   DDRE
#define DIO80_PWM   nullptr

#define DIO81_PIN   PIND4
#define DIO81_RPORT PIND
#define DIO81_WPORT PORTD
#define DIO81_DDR   DDRD
#define DIO81_PWM   nullptr

#define DIO82_PIN   PIND5
#define DIO82_RPORT PIND
#define DIO82_WPORT PORTD
#define DIO82_DDR   DDRD
#define DIO82_PWM   nullptr

#define DIO83_PIN   PIND6
#define DIO83_RPORT PIND
#define DIO83_WPORT PORTD
#define DIO83_DDR   DDRD
#define DIO83_PWM   nullptr

#define DIO84_PIN   PINH2
#define DIO84_RPORT PINH
#define DIO84_WPORT PORTH
#define DIO84_DDR   DDRH
#define DIO84_PWM   nullptr

#define DIO85_PIN   PINH7
#define DIO85_RPORT PINH
#define DIO85_WPORT PORTH
#define DIO85_DDR   DDRH
#define DIO85_PWM   nullptr

#undef PA0
#define PA0_PIN     PINA0
#define PA0_RPORT   PINA
#define PA0_WPORT   PORTA
#define PA0_DDR     DDRA
#define PA0_PWM     nullptr
#undef PA1
#define PA1_PIN     PINA1
#define PA1_RPORT   PINA
#define PA1_WPORT   PORTA
#define PA1_DDR     DDRA
#define PA1_PWM     nullptr
#undef PA2
#define PA2_PIN     PINA2
#define PA2_RPORT   PINA
#define PA2_WPORT   PORTA
#define PA2_DDR     DDRA
#define PA2_PWM     nullptr
#undef PA3
#define PA3_PIN     PINA3
#define PA3_RPORT   PINA
#define PA3_WPORT   PORTA
#define PA3_DDR     DDRA
#define PA3_PWM     nullptr
#undef PA4
#define PA4_PIN     PINA4
#define PA4_RPORT   PINA
#define PA4_WPORT   PORTA
#define PA4_DDR     DDRA
#define PA4_PWM     nullptr
#undef PA5
#define PA5_PIN     PINA5
#define PA5_RPORT   PINA
#define PA5_WPORT   PORTA
#define PA5_DDR     DDRA
#define PA5_PWM     nullptr
#undef PA6
#define PA6_PIN     PINA6
#define PA6_RPORT   PINA
#define PA6_WPORT   PORTA
#define PA6_DDR     DDRA
#define PA6_PWM     nullptr
#undef PA7
#define PA7_PIN     PINA7
#define PA7_RPORT   PINA
#define PA7_WPORT   PORTA
#define PA7_DDR     DDRA
#define PA7_PWM     nullptr

#undef PB0
#define PB0_PIN     PINB0
#define PB0_RPORT   PINB
#define PB0_WPORT   PORTB
#define PB0_DDR     DDRB
#define PB0_PWM     nullptr
#undef PB1
#define PB1_PIN     PINB1
#define PB1_RPORT   PINB
#define PB1_WPORT   PORTB
#define PB1_DDR     DDRB
#define PB1_PWM     nullptr
#undef PB2
#define PB2_PIN     PINB2
#define PB2_RPORT   PINB
#define PB2_WPORT   PORTB
#define PB2_DDR     DDRB
#define PB2_PWM     nullptr
#undef PB3
#define PB3_PIN     PINB3
#define PB3_RPORT   PINB
#define PB3_WPORT   PORTB
#define PB3_DDR     DDRB
#define PB3_PWM     nullptr
#undef PB4
#define PB4_PIN     PINB4
#define PB4_RPORT   PINB
#define PB4_WPORT   PORTB
#define PB4_DDR     DDRB
#define PB4_PWM     &OCR2A
#undef PB5
#define PB5_PIN     PINB5
#define PB5_RPORT   PINB
#define PB5_WPORT   PORTB
#define PB5_DDR     DDRB
#define PB5_PWM     nullptr
#undef PB6
#define PB6_PIN     PINB6
#define PB6_RPORT   PINB
#define PB6_WPORT   PORTB
#define PB6_DDR     DDRB
#define PB6_PWM     nullptr
#undef PB7
#define PB7_PIN     PINB7
#define PB7_RPORT   PINB
#define PB7_WPORT   PORTB
#define PB7_DDR     DDRB
#define PB7_PWM     &OCR0A

#undef PC0
#define PC0_PIN     PINC0
#define PC0_RPORT   PINC
#define PC0_WPORT   PORTC
#define PC0_DDR     DDRC
#define PC0_PWM     nullptr
#undef PC1
#define PC1_PIN     PINC1
#define PC1_RPORT   PINC
#define PC1_WPORT   PORTC
#define PC1_DDR     DDRC
#define PC1_PWM     nullptr
#undef PC2
#define PC2_PIN     PINC2
#define PC2_RPORT   PINC
#define PC2_WPORT   PORTC
#define PC2_DDR     DDRC
#define PC2_PWM     nullptr
#undef PC3
#define PC3_PIN     PINC3
#define PC3_RPORT   PINC
#define PC3_WPORT   PORTC
#define PC3_DDR     DDRC
#define PC3_PWM     nullptr
#undef PC4
#define PC4_PIN     PINC4
#define PC4_RPORT   PINC
#define PC4_WPORT   PORTC
#define PC4_DDR     DDRC
#define PC4_PWM     nullptr
#undef PC5
#define PC5_PIN     PINC5
#define PC5_RPORT   PINC
#define PC5_WPORT   PORTC
#define PC5_DDR     DDRC
#define PC5_PWM     nullptr
#undef PC6
#define PC6_PIN     PINC6
#define PC6_RPORT   PINC
#define PC6_WPORT   PORTC
#define PC6_DDR     DDRC
#define PC6_PWM     nullptr
#undef PC7
#define PC7_PIN     PINC7
#define PC7_RPORT   PINC
#define PC7_WPORT   PORTC
#define PC7_DDR     DDRC
#define PC7_PWM     nullptr

#undef PD0
#define PD0_PIN     PIND0
#define PD0_RPORT   PIND
#define PD0_WPORT   PORTD
#define PD0_DDR     DDRD
#define PD0_PWM     nullptr
#undef PD1
#define PD1_PIN     PIND1
#define PD1_RPORT   PIND
#define PD1_WPORT   PORTD
#define PD1_DDR     DDRD
#define PD1_PWM     nullptr
#undef PD2
#define PD2_PIN     PIND2
#define PD2_RPORT   PIND
#define PD2_WPORT   PORTD
#define PD2_DDR     DDRD
#define PD2_PWM     nullptr
#undef PD3
#define PD3_PIN     PIND3
#define PD3_RPORT   PIND
#define PD3_WPORT   PORTD
#define PD3_DDR     DDRD
#define PD3_PWM     nullptr
#undef PD4
#define PD4_PIN     PIND4
#define PD4_RPORT   PIND
#define PD4_WPORT   PORTD
#define PD4_DDR     DDRD
#define PD4_PWM     nullptr
#undef PD5
#define PD5_PIN     PIND5
#define PD5_RPORT   PIND
#define PD5_WPORT   PORTD
#define PD5_DDR     DDRD
#define PD5_PWM     nullptr
#undef PD6
#define PD6_PIN     PIND6
#define PD6_RPORT   PIND
#define PD6_WPORT   PORTD
#define PD6_DDR     DDRD
#define PD6_PWM     nullptr
#undef PD7
#define PD7_PIN     PIND7
#define PD7_RPORT   PIND
#define PD7_WPORT   PORTD
#define PD7_DDR     DDRD
#define PD7_PWM     nullptr

#undef PE0
#define PE0_PIN     PINE0
#define PE0_RPORT   PINE
#define PE0_WPORT   PORTE
#define PE0_DDR     DDRE
#define PE0_PWM     nullptr
#undef PE1
#define PE1_PIN     PINE1
#define PE1_RPORT   PINE
#define PE1_WPORT   PORTE
#define PE1_DDR     DDRE
#define PE1_PWM     nullptr
#undef PE2
#define PE2_PIN     PINE2
#define PE2_RPORT   PINE
#define PE2_WPORT   PORTE
#define PE2_DDR     DDRE
#define PE2_PWM     nullptr
#undef PE3
#define PE3_PIN     PINE3
#define PE3_RPORT   PINE
#define PE3_WPORT   PORTE
#define PE3_DDR     DDRE
#define PE3_PWM     &OCR3AL
#undef PE4
#define PE4_PIN     PINE4
#define PE4_RPORT   PINE
#define PE4_WPORT   PORTE
#define PE4_DDR     DDRE
#define PE4_PWM     &OCR3BL
#undef PE5
#define PE5_PIN     PINE5
#define PE5_RPORT   PINE
#define PE5_WPORT   PORTE
#define PE5_DDR     DDRE
#define PE5_PWM     &OCR3CL
#undef PE6
#define PE6_PIN     PINE6
#define PE6_RPORT   PINE
#define PE6_WPORT   PORTE
#define PE6_DDR     DDRE
#define PE6_PWM     nullptr
#undef PE7
#define PE7_PIN     PINE7
#define PE7_RPORT   PINE
#define PE7_WPORT   PORTE
#define PE7_DDR     DDRE
#define PE7_PWM     nullptr

#undef PF0
#define PF0_PIN     PINF0
#define PF0_RPORT   PINF
#define PF0_WPORT   PORTF
#define PF0_DDR     DDRF
#define PF0_PWM     nullptr
#undef PF1
#define PF1_PIN     PINF1
#define PF1_RPORT   PINF
#define PF1_WPORT   PORTF
#define PF1_DDR     DDRF
#define PF1_PWM     nullptr
#undef PF2
#define PF2_PIN     PINF2
#define PF2_RPORT   PINF
#define PF2_WPORT   PORTF
#define PF2_DDR     DDRF
#define PF2_PWM     nullptr
#undef PF3
#define PF3_PIN     PINF3
#define PF3_RPORT   PINF
#define PF3_WPORT   PORTF
#define PF3_DDR     DDRF
#define PF3_PWM     nullptr
#undef PF4
#define PF4_PIN     PINF4
#define PF4_RPORT   PINF
#define PF4_WPORT   PORTF
#define PF4_DDR     DDRF
#define PF4_PWM     nullptr
#undef PF5
#define PF5_PIN     PINF5
#define PF5_RPORT   PINF
#define PF5_WPORT   PORTF
#define PF5_DDR     DDRF
#define PF5_PWM     nullptr
#undef PF6
#define PF6_PIN     PINF6
#define PF6_RPORT   PINF
#define PF6_WPORT   PORTF
#define PF6_DDR     DDRF
#define PF6_PWM     nullptr
#undef PF7
#define PF7_PIN     PINF7
#define PF7_RPORT   PINF
#define PF7_WPORT   PORTF
#define PF7_DDR     DDRF
#define PF7_PWM     nullptr

#undef PG0
#define PG0_PIN     PING0
#define PG0_RPORT   PING
#define PG0_WPORT   PORTG
#define PG0_DDR     DDRG
#define PG0_PWM     nullptr
#undef PG1
#define PG1_PIN     PING1
#define PG1_RPORT   PING
#define PG1_WPORT   PORTG
#define PG1_DDR     DDRG
#define PG1_PWM     nullptr
#undef PG2
#define PG2_PIN     PING2
#define PG2_RPORT   PING
#define PG2_WPORT   PORTG
#define PG2_DDR     DDRG
#define PG2_PWM     nullptr
#undef PG3
#define PG3_PIN     PING3
#define PG3_RPORT   PING
#define PG3_WPORT   PORTG
#define PG3_DDR     DDRG
#define PG3_PWM     nullptr
#undef PG4
#define PG4_PIN     PING4
#define PG4_RPORT   PING
#define PG4_WPORT   PORTG
#define PG4_DDR     DDRG
#define PG4_PWM     nullptr
#undef PG5
#define PG5_PIN     PING5
#define PG5_RPORT   PING
#define PG5_WPORT   PORTG
#define PG5_DDR     DDRG
#define PG5_PWM     &OCR0B

#undef PH0
#define PH0_PIN     PINH0
#define PH0_RPORT   PINH
#define PH0_WPORT   PORTH
#define PH0_DDR     DDRH
#define PH0_PWM     nullptr
#undef PH1
#define PH1_PIN     PINH1
#define PH1_RPORT   PINH
#define PH1_WPORT   PORTH
#define PH1_DDR     DDRH
#define PH1_PWM     nullptr
#undef PH2
#define PH2_PIN     PINH2
#define PH2_RPORT   PINH
#define PH2_WPORT   PORTH
#define PH2_DDR     DDRH
#define PH2_PWM     nullptr
#undef PH3
#define PH3_PIN     PINH3
#define PH3_RPORT   PINH
#define PH3_WPORT   PORTH
#define PH3_DDR     DDRH
#define PH3_PWM     &OCR4AL
#undef PH4
#define PH4_PIN     PINH4
#define PH4_RPORT   PINH
#define PH4_WPORT   PORTH
#define PH4_DDR     DDRH
#define PH4_PWM     &OCR4BL
#undef PH5
#define PH5_PIN     PINH5
#define PH5_RPORT   PINH
#define PH5_WPORT   PORTH
#define PH5_DDR     DDRH
#define PH5_PWM     &OCR4CL
#undef PH6
#define PH6_PIN     PINH6
#define PH6_RPORT   PINH
#define PH6_WPORT   PORTH
#define PH6_DDR     DDRH
#define PH6_PWM     &OCR2B
#undef PH7
#define PH7_PIN     PINH7
#define PH7_RPORT   PINH
#define PH7_WPORT   PORTH
#define PH7_DDR     DDRH
#define PH7_PWM     nullptr

#undef PJ0
#define PJ0_PIN     PINJ0
#define PJ0_RPORT   PINJ
#define PJ0_WPORT   PORTJ
#define PJ0_DDR     DDRJ
#define PJ0_PWM     nullptr
#undef PJ1
#define PJ1_PIN     PINJ1
#define PJ1_RPORT   PINJ
#define PJ1_WPORT   PORTJ
#define PJ1_DDR     DDRJ
#define PJ1_PWM     nullptr
#undef PJ2
#define PJ2_PIN     PINJ2
#define PJ2_RPORT   PINJ
#define PJ2_WPORT   PORTJ
#define PJ2_DDR     DDRJ
#define PJ2_PWM     nullptr
#undef PJ3
#define PJ3_PIN     PINJ3
#define PJ3_RPORT   PINJ
#define PJ3_WPORT   PORTJ
#define PJ3_DDR     DDRJ
#define PJ3_PWM     nullptr
#undef PJ4
#define PJ4_PIN     PINJ4
#define PJ4_RPORT   PINJ
#define PJ4_WPORT   PORTJ
#define PJ4_DDR     DDRJ
#define PJ4_PWM     nullptr
#undef PJ5
#define PJ5_PIN     PINJ5
#define PJ5_RPORT   PINJ
#define PJ5_WPORT   PORTJ
#define PJ5_DDR     DDRJ
#define PJ5_PWM     nullptr
#undef PJ6
#define PJ6_PIN     PINJ6
#define PJ6_RPORT   PINJ
#define PJ6_WPORT   PORTJ
#define PJ6_DDR     DDRJ
#define PJ6_PWM     nullptr
#undef PJ7
#define PJ7_PIN     PINJ7
#define PJ7_RPORT   PINJ
#define PJ7_WPORT   PORTJ
#define PJ7_DDR     DDRJ
#define PJ7_PWM     nullptr

#undef PK0
#define PK0_PIN     PINK0
#define PK0_RPORT   PINK
#define PK0_WPORT   PORTK
#define PK0_DDR     DDRK
#define PK0_PWM     nullptr
#undef PK1
#define PK1_PIN     PINK1
#define PK1_RPORT   PINK
#define PK1_WPORT   PORTK
#define PK1_DDR     DDRK
#define PK1_PWM     nullptr
#undef PK2
#define PK2_PIN     PINK2
#define PK2_RPORT   PINK
#define PK2_WPORT   PORTK
#define PK2_DDR     DDRK
#define PK2_PWM     nullptr
#undef PK3
#define PK3_PIN     PINK3
#define PK3_RPORT   PINK
#define PK3_WPORT   PORTK
#define PK3_DDR     DDRK
#define PK3_PWM     nullptr
#undef PK4
#define PK4_PIN     PINK4
#define PK4_RPORT   PINK
#define PK4_WPORT   PORTK
#define PK4_DDR     DDRK
#define PK4_PWM     nullptr
#undef PK5
#define PK5_PIN     PINK5
#define PK5_RPORT   PINK
#define PK5_WPORT   PORTK
#define PK5_DDR     DDRK
#define PK5_PWM     nullptr
#undef PK6
#define PK6_PIN     PINK6
#define PK6_RPORT   PINK
#define PK6_WPORT   PORTK
#define PK6_DDR     DDRK
#define PK6_PWM     nullptr
#undef PK7
#define PK7_PIN     PINK7
#define PK7_RPORT   PINK
#define PK7_WPORT   PORTK
#define PK7_DDR     DDRK
#define PK7_PWM     nullptr

#undef PL0
#define PL0_PIN     PINL0
#define PL0_RPORT   PINL
#define PL0_WPORT   PORTL
#define PL0_DDR     DDRL
#define PL0_PWM     nullptr
#undef PL1
#define PL1_PIN     PINL1
#define PL1_RPORT   PINL
#define PL1_WPORT   PORTL
#define PL1_DDR     DDRL
#define PL1_PWM     nullptr
#undef PL2
#define PL2_PIN     PINL2
#define PL2_RPORT   PINL
#define PL2_WPORT   PORTL
#define PL2_DDR     DDRL
#define PL2_PWM     nullptr
#undef PL3
#define PL3_PIN     PINL3
#define PL3_RPORT   PINL
#define PL3_WPORT   PORTL
#define PL3_DDR     DDRL
#define PL3_PWM     &OCR5AL
#undef PL4
#define PL4_PIN     PINL4
#define PL4_RPORT   PINL
#define PL4_WPORT   PORTL
#define PL4_DDR     DDRL
#define PL4_PWM     &OCR5BL
#undef PL5
#define PL5_PIN     PINL5
#define PL5_RPORT   PINL
#define PL5_WPORT   PORTL
#define PL5_DDR     DDRL
#define PL5_PWM     &OCR5CL
#undef PL6
#define PL6_PIN     PINL6
#define PL6_RPORT   PINL
#define PL6_WPORT   PORTL
#define PL6_DDR     DDRL
#define PL6_PWM     nullptr
#undef PL7
#define PL7_PIN     PINL7
#define PL7_RPORT   PINL
#define PL7_WPORT   PORTL
#define PL7_DDR     DDRL
#define PL7_PWM     nullptr
