/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Pin mapping for the 1281 and 2561
 *
 *   1281     38 39 40 41 42 43 44 45 16 10 11 12 06 07 08 09 30 31 32 33 34 35 36 37 17 18 19 20 21 22 23 24 00 01 13 05 02 03 14 15 46 47 48 49 50 51 52 53 25 26 27 28 29 04
 *   Port     A0 A1 A2 A3 A4 A5 A6 A7 B0 B1 B2 B3 B4 B5 B6 B7 C0 C1 C2 C3 C4 C5 C6 C7 D0 D1 D2 D3 D4 D5 D6 D7 E0 E1 E2 E3 E4 E5 E6 E7 F0 F1 F2 F3 F4 F5 F6 F7 G0 G1 G2 G3 G4 G5
 *   Marlin   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53
 */

#ifndef _FASTIO_1281_H_
#define _FASTIO_1281_H_

#include "fastio_AVR.h"

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
#define DIO0_PWM    NULL

#define DIO1_PIN    PINE1
#define DIO1_RPORT  PINE
#define DIO1_WPORT  PORTE
#define DIO1_DDR    DDRE
#define DIO1_PWM    NULL

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

#define DIO6_PIN    PINB4
#define DIO6_RPORT  PINB
#define DIO6_WPORT  PORTB
#define DIO6_DDR    DDRB
#define DIO6_PWM    &OCR2AL

#define DIO7_PIN    PINB5
#define DIO7_RPORT  PINB
#define DIO7_WPORT  PORTB
#define DIO7_DDR    DDRB
#define DIO7_PWM    &OCR1AL

#define DIO8_PIN    PINB6
#define DIO8_RPORT  PINB
#define DIO8_WPORT  PORTB
#define DIO8_DDR    DDRB
#define DIO8_PWM    &OCR1BL

#define DIO9_PIN    PINB7
#define DIO9_RPORT  PINB
#define DIO9_WPORT  PORTB
#define DIO9_DDR    DDRB
#define DIO9_PWM    &OCR0AL

#define DIO10_PIN   PINB1
#define DIO10_RPORT PINB
#define DIO10_WPORT PORTB
#define DIO10_DDR   DDRB
#define DIO10_PWM   NULL

#define DIO11_PIN   PINB2
#define DIO11_RPORT PINB
#define DIO11_WPORT PORTB
#define DIO11_DDR   DDRB
#define DIO11_PWM   NULL

#define DIO12_PIN   PINB3
#define DIO12_RPORT PINB
#define DIO12_WPORT PORTB
#define DIO12_DDR   DDRB
#define DIO12_PWM   NULL

#define DIO13_PIN   PINE2
#define DIO13_RPORT PINE
#define DIO13_WPORT PORTE
#define DIO13_DDR   DDRE
#define DIO13_PWM   NULL

#define DIO14_PIN   PINE6
#define DIO14_RPORT PINE
#define DIO14_WPORT PORTE
#define DIO14_DDR   DDRE
#define DIO14_PWM   NULL

#define DIO15_PIN   PINE7
#define DIO15_RPORT PINE
#define DIO15_WPORT PORTE
#define DIO15_DDR   DDRE
#define DIO15_PWM   NULL

#define DIO16_PIN   PINB0
#define DIO16_RPORT PINB
#define DIO16_WPORT PORTB
#define DIO16_DDR   DDRB
#define DIO16_PWM   NULL

#define DIO17_PIN   PIND0
#define DIO17_RPORT PIND
#define DIO17_WPORT PORTD
#define DIO17_DDR   DDRD
#define DIO17_PWM   NULL

#define DIO18_PIN   PIND1
#define DIO18_RPORT PIND
#define DIO18_WPORT PORTD
#define DIO18_DDR   DDRD
#define DIO18_PWM   NULL

#define DIO19_PIN   PIND2
#define DIO19_RPORT PIND
#define DIO19_WPORT PORTD
#define DIO19_DDR   DDRD
#define DIO19_PWM   NULL

#define DIO20_PIN   PIND3
#define DIO20_RPORT PIND
#define DIO20_WPORT PORTD
#define DIO20_DDR   DDRD
#define DIO20_PWM   NULL

#define DIO21_PIN   PIND4
#define DIO21_RPORT PIND
#define DIO21_WPORT PORTD
#define DIO21_DDR   DDRD
#define DIO21_PWM   NULL

#define DIO22_PIN   PIND5
#define DIO22_RPORT PIND
#define DIO22_WPORT PORTD
#define DIO22_DDR   DDRD
#define DIO22_PWM   NULL

#define DIO23_PIN   PIND6
#define DIO23_RPORT PIND
#define DIO23_WPORT PORTD
#define DIO23_DDR   DDRD
#define DIO23_PWM   NULL

#define DIO24_PIN   PIND7
#define DIO24_RPORT PIND
#define DIO24_WPORT PORTD
#define DIO24_DDR   DDRD
#define DIO24_PWM   NULL

#define DIO25_PIN   PING0
#define DIO25_RPORT PING
#define DIO25_WPORT PORTG
#define DIO25_DDR   DDRG
#define DIO25_PWM   NULL

#define DIO26_PIN   PING1
#define DIO26_RPORT PING
#define DIO26_WPORT PORTG
#define DIO26_DDR   DDRG
#define DIO26_PWM   NULL

#define DIO27_PIN   PING2
#define DIO27_RPORT PING
#define DIO27_WPORT PORTG
#define DIO27_DDR   DDRG
#define DIO27_PWM   NULL

#define DIO28_PIN   PING3
#define DIO28_RPORT PING
#define DIO28_WPORT PORTG
#define DIO28_DDR   DDRG
#define DIO28_PWM   NULL

#define DIO29_PIN   PING4
#define DIO29_RPORT PING
#define DIO29_WPORT PORTG
#define DIO29_DDR   DDRG
#define DIO29_PWM   NULL

#define DIO30_PIN   PINC0
#define DIO30_RPORT PINC
#define DIO30_WPORT PORTC
#define DIO30_DDR   DDRC
#define DIO30_PWM   NULL

#define DIO31_PIN   PINC1
#define DIO31_RPORT PINC
#define DIO31_WPORT PORTC
#define DIO31_DDR   DDRC
#define DIO31_PWM   NULL

#define DIO32_PIN   PINC2
#define DIO32_RPORT PINC
#define DIO32_WPORT PORTC
#define DIO32_DDR   DDRC
#define DIO32_PWM   NULL

#define DIO33_PIN   PINC3
#define DIO33_RPORT PINC
#define DIO33_WPORT PORTC
#define DIO33_DDR   DDRC
#define DIO33_PWM   NULL

#define DIO34_PIN   PINC4
#define DIO34_RPORT PINC
#define DIO34_WPORT PORTC
#define DIO34_DDR   DDRC
#define DIO34_PWM   NULL

#define DIO35_PIN   PINC5
#define DIO35_RPORT PINC
#define DIO35_WPORT PORTC
#define DIO35_DDR   DDRC
#define DIO35_PWM   NULL

#define DIO36_PIN   PINC6
#define DIO36_RPORT PINC
#define DIO36_WPORT PORTC
#define DIO36_DDR   DDRC
#define DIO36_PWM   NULL

#define DIO37_PIN   PINC7
#define DIO37_RPORT PINC
#define DIO37_WPORT PORTC
#define DIO37_DDR   DDRC
#define DIO37_PWM   NULL

#define DIO38_PIN   PINA0
#define DIO38_RPORT PINA
#define DIO38_WPORT PORTA
#define DIO38_DDR   DDRA
#define DIO38_PWM   NULL

#define DIO39_PIN   PINA1
#define DIO39_RPORT PINA
#define DIO39_WPORT PORTA
#define DIO39_DDR   DDRA
#define DIO39_PWM   NULL

#define DIO40_PIN   PINA2
#define DIO40_RPORT PINA
#define DIO40_WPORT PORTA
#define DIO40_DDR   DDRA
#define DIO40_PWM   NULL

#define DIO41_PIN   PINA3
#define DIO41_RPORT PINA
#define DIO41_WPORT PORTA
#define DIO41_DDR   DDRA
#define DIO41_PWM   NULL

#define DIO42_PIN   PINA4
#define DIO42_RPORT PINA
#define DIO42_WPORT PORTA
#define DIO42_DDR   DDRA
#define DIO42_PWM   NULL

#define DIO43_PIN   PINA5
#define DIO43_RPORT PINA
#define DIO43_WPORT PORTA
#define DIO43_DDR   DDRA
#define DIO43_PWM   NULL

#define DIO44_PIN   PINA6
#define DIO44_RPORT PINA
#define DIO44_WPORT PORTA
#define DIO44_DDR   DDRA
#define DIO44_PWM   NULL

#define DIO45_PIN   PINA7
#define DIO45_RPORT PINA
#define DIO45_WPORT PORTA
#define DIO45_DDR   DDRA
#define DIO45_PWM   NULL

#define DIO46_PIN   PINF0
#define DIO46_RPORT PINF
#define DIO46_WPORT PORTF
#define DIO46_DDR   DDRF
#define DIO46_PWM   NULL

#define DIO47_PIN   PINF1
#define DIO47_RPORT PINF
#define DIO47_WPORT PORTF
#define DIO47_DDR   DDRF
#define DIO47_PWM   NULL

#define DIO48_PIN   PINF2
#define DIO48_RPORT PINF
#define DIO48_WPORT PORTF
#define DIO48_DDR   DDRF
#define DIO48_PWM   NULL

#define DIO49_PIN   PINF3
#define DIO49_RPORT PINF
#define DIO49_WPORT PORTF
#define DIO49_DDR   DDRF
#define DIO49_PWM   NULL

#define DIO50_PIN   PINF4
#define DIO50_RPORT PINF
#define DIO50_WPORT PORTF
#define DIO50_DDR   DDRF
#define DIO50_PWM   NULL

#define DIO51_PIN   PINF5
#define DIO51_RPORT PINF
#define DIO51_WPORT PORTF
#define DIO51_DDR   DDRF
#define DIO51_PWM   NULL

#define DIO52_PIN   PINF6
#define DIO52_RPORT PINF
#define DIO52_WPORT PORTF
#define DIO52_DDR   DDRF
#define DIO52_PWM   NULL

#define DIO53_PIN   PINF7
#define DIO53_RPORT PINF
#define DIO53_WPORT PORTF
#define DIO53_DDR   DDRF
#define DIO53_PWM   NULL

#undef PA0
#define PA0_PIN     PINA0
#define PA0_RPORT   PINA
#define PA0_WPORT   PORTA
#define PA0_DDR     DDRA
#define PA0_PWM     NULL
#undef PA1
#define PA1_PIN     PINA1
#define PA1_RPORT   PINA
#define PA1_WPORT   PORTA
#define PA1_DDR     DDRA
#define PA1_PWM     NULL
#undef PA2
#define PA2_PIN     PINA2
#define PA2_RPORT   PINA
#define PA2_WPORT   PORTA
#define PA2_DDR     DDRA
#define PA2_PWM     NULL
#undef PA3
#define PA3_PIN     PINA3
#define PA3_RPORT   PINA
#define PA3_WPORT   PORTA
#define PA3_DDR     DDRA
#define PA3_PWM     NULL
#undef PA4
#define PA4_PIN     PINA4
#define PA4_RPORT   PINA
#define PA4_WPORT   PORTA
#define PA4_DDR     DDRA
#define PA4_PWM     NULL
#undef PA5
#define PA5_PIN     PINA5
#define PA5_RPORT   PINA
#define PA5_WPORT   PORTA
#define PA5_DDR     DDRA
#define PA5_PWM     NULL
#undef PA6
#define PA6_PIN     PINA6
#define PA6_RPORT   PINA
#define PA6_WPORT   PORTA
#define PA6_DDR     DDRA
#define PA6_PWM     NULL
#undef PA7
#define PA7_PIN     PINA7
#define PA7_RPORT   PINA
#define PA7_WPORT   PORTA
#define PA7_DDR     DDRA
#define PA7_PWM     NULL

#undef PB0
#define PB0_PIN     PINB0
#define PB0_RPORT   PINB
#define PB0_WPORT   PORTB
#define PB0_DDR     DDRB
#define PB0_PWM     NULL
#undef PB1
#define PB1_PIN     PINB1
#define PB1_RPORT   PINB
#define PB1_WPORT   PORTB
#define PB1_DDR     DDRB
#define PB1_PWM     NULL
#undef PB2
#define PB2_PIN     PINB2
#define PB2_RPORT   PINB
#define PB2_WPORT   PORTB
#define PB2_DDR     DDRB
#define PB2_PWM     NULL
#undef PB3
#define PB3_PIN     PINB3
#define PB3_RPORT   PINB
#define PB3_WPORT   PORTB
#define PB3_DDR     DDRB
#define PB3_PWM     NULL
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
#define PB5_PWM     NULL
#undef PB6
#define PB6_PIN     PINB6
#define PB6_RPORT   PINB
#define PB6_WPORT   PORTB
#define PB6_DDR     DDRB
#define PB6_PWM     NULL
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
#define PC0_PWM     NULL
#undef PC1
#define PC1_PIN     PINC1
#define PC1_RPORT   PINC
#define PC1_WPORT   PORTC
#define PC1_DDR     DDRC
#define PC1_PWM     NULL
#undef PC2
#define PC2_PIN     PINC2
#define PC2_RPORT   PINC
#define PC2_WPORT   PORTC
#define PC2_DDR     DDRC
#define PC2_PWM     NULL
#undef PC3
#define PC3_PIN     PINC3
#define PC3_RPORT   PINC
#define PC3_WPORT   PORTC
#define PC3_DDR     DDRC
#define PC3_PWM     NULL
#undef PC4
#define PC4_PIN     PINC4
#define PC4_RPORT   PINC
#define PC4_WPORT   PORTC
#define PC4_DDR     DDRC
#define PC4_PWM     NULL
#undef PC5
#define PC5_PIN     PINC5
#define PC5_RPORT   PINC
#define PC5_WPORT   PORTC
#define PC5_DDR     DDRC
#define PC5_PWM     NULL
#undef PC6
#define PC6_PIN     PINC6
#define PC6_RPORT   PINC
#define PC6_WPORT   PORTC
#define PC6_DDR     DDRC
#define PC6_PWM     NULL
#undef PC7
#define PC7_PIN     PINC7
#define PC7_RPORT   PINC
#define PC7_WPORT   PORTC
#define PC7_DDR     DDRC
#define PC7_PWM     NULL

#undef PD0
#define PD0_PIN     PIND0
#define PD0_RPORT   PIND
#define PD0_WPORT   PORTD
#define PD0_DDR     DDRD
#define PD0_PWM     NULL
#undef PD1
#define PD1_PIN     PIND1
#define PD1_RPORT   PIND
#define PD1_WPORT   PORTD
#define PD1_DDR     DDRD
#define PD1_PWM     NULL
#undef PD2
#define PD2_PIN     PIND2
#define PD2_RPORT   PIND
#define PD2_WPORT   PORTD
#define PD2_DDR     DDRD
#define PD2_PWM     NULL
#undef PD3
#define PD3_PIN     PIND3
#define PD3_RPORT   PIND
#define PD3_WPORT   PORTD
#define PD3_DDR     DDRD
#define PD3_PWM     NULL
#undef PD4
#define PD4_PIN     PIND4
#define PD4_RPORT   PIND
#define PD4_WPORT   PORTD
#define PD4_DDR     DDRD
#define PD4_PWM     NULL
#undef PD5
#define PD5_PIN     PIND5
#define PD5_RPORT   PIND
#define PD5_WPORT   PORTD
#define PD5_DDR     DDRD
#define PD5_PWM     NULL
#undef PD6
#define PD6_PIN     PIND6
#define PD6_RPORT   PIND
#define PD6_WPORT   PORTD
#define PD6_DDR     DDRD
#define PD6_PWM     NULL
#undef PD7
#define PD7_PIN     PIND7
#define PD7_RPORT   PIND
#define PD7_WPORT   PORTD
#define PD7_DDR     DDRD
#define PD7_PWM     NULL

#undef PE0
#define PE0_PIN     PINE0
#define PE0_RPORT   PINE
#define PE0_WPORT   PORTE
#define PE0_DDR     DDRE
#define PE0_PWM     NULL
#undef PE1
#define PE1_PIN     PINE1
#define PE1_RPORT   PINE
#define PE1_WPORT   PORTE
#define PE1_DDR     DDRE
#define PE1_PWM     NULL
#undef PE2
#define PE2_PIN     PINE2
#define PE2_RPORT   PINE
#define PE2_WPORT   PORTE
#define PE2_DDR     DDRE
#define PE2_PWM     NULL
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
#define PE6_PWM     NULL
#undef PE7
#define PE7_PIN     PINE7
#define PE7_RPORT   PINE
#define PE7_WPORT   PORTE
#define PE7_DDR     DDRE
#define PE7_PWM     NULL

#undef PF0
#define PF0_PIN     PINF0
#define PF0_RPORT   PINF
#define PF0_WPORT   PORTF
#define PF0_DDR     DDRF
#define PF0_PWM     NULL
#undef PF1
#define PF1_PIN     PINF1
#define PF1_RPORT   PINF
#define PF1_WPORT   PORTF
#define PF1_DDR     DDRF
#define PF1_PWM     NULL
#undef PF2
#define PF2_PIN     PINF2
#define PF2_RPORT   PINF
#define PF2_WPORT   PORTF
#define PF2_DDR     DDRF
#define PF2_PWM     NULL
#undef PF3
#define PF3_PIN     PINF3
#define PF3_RPORT   PINF
#define PF3_WPORT   PORTF
#define PF3_DDR     DDRF
#define PF3_PWM     NULL
#undef PF4
#define PF4_PIN     PINF4
#define PF4_RPORT   PINF
#define PF4_WPORT   PORTF
#define PF4_DDR     DDRF
#define PF4_PWM     NULL
#undef PF5
#define PF5_PIN     PINF5
#define PF5_RPORT   PINF
#define PF5_WPORT   PORTF
#define PF5_DDR     DDRF
#define PF5_PWM     NULL
#undef PF6
#define PF6_PIN     PINF6
#define PF6_RPORT   PINF
#define PF6_WPORT   PORTF
#define PF6_DDR     DDRF
#define PF6_PWM     NULL
#undef PF7
#define PF7_PIN     PINF7
#define PF7_RPORT   PINF
#define PF7_WPORT   PORTF
#define PF7_DDR     DDRF
#define PF7_PWM     NULL

#undef PG0
#define PG0_PIN     PING0
#define PG0_RPORT   PING
#define PG0_WPORT   PORTG
#define PG0_DDR     DDRG
#define PG0_PWM     NULL
#undef PG1
#define PG1_PIN     PING1
#define PG1_RPORT   PING
#define PG1_WPORT   PORTG
#define PG1_DDR     DDRG
#define PG1_PWM     NULL
#undef PG2
#define PG2_PIN     PING2
#define PG2_RPORT   PING
#define PG2_WPORT   PORTG
#define PG2_DDR     DDRG
#define PG2_PWM     NULL
#undef PG3
#define PG3_PIN     PING3
#define PG3_RPORT   PING
#define PG3_WPORT   PORTG
#define PG3_DDR     DDRG
#define PG3_PWM     NULL
#undef PG4
#define PG4_PIN     PING4
#define PG4_RPORT   PING
#define PG4_WPORT   PORTG
#define PG4_DDR     DDRG
#define PG4_PWM     NULL
#undef PG5
#define PG5_PIN     PING5
#define PG5_RPORT   PING
#define PG5_WPORT   PORTG
#define PG5_DDR     DDRG
#define PG5_PWM     &OCR0B

#endif // _FASTIO_1281_H_
