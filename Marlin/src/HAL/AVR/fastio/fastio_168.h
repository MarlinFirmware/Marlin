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
 * Pin mapping for the 168, 328, and 328P
 *
 *   Logical Pin: 08 09 10 11 12 13 14 15 16 17 18 19 20 21 00 01 02 03 04 05 06 07
 *   Port:        B0 B1 B2 B3 B4 B5 C0 C1 C2 C3 C4 C5 C6 C7 D0 D1 D2 D3 D4 D5 D6 D7
 *
 * Arduino Pin Layout video: https://youtu.be/rIqeVCX09FA
 * AVR alternate pin function overview video: https://youtu.be/1yd8wuI5Plg
 */

#include "../fastio.h"

#define DEBUG_LED   AIO5

// UART
#define RXD         DIO0
#define TXD         DIO1

// SPI
#define SCK         DIO13
#define MISO        DIO12
#define MOSI        DIO11
#define SS          DIO10

// TWI (I2C)
#define SCL         AIO5
#define SDA         AIO4

// Timers and PWM
#define OC0A        DIO6
#define OC0B        DIO5
#define OC1A        DIO9
#define OC1B        DIO10
#define OC2A        DIO11
#define OC2B        DIO3

// Digital I/O

#define DIO0_PIN    PIND0
#define DIO0_RPORT  PIND
#define DIO0_WPORT  PORTD
#define DIO0_DDR    DDRD
#define DIO0_PWM    nullptr

#define DIO1_PIN    PIND1
#define DIO1_RPORT  PIND
#define DIO1_WPORT  PORTD
#define DIO1_DDR    DDRD
#define DIO1_PWM    nullptr

#define DIO2_PIN    PIND2
#define DIO2_RPORT  PIND
#define DIO2_WPORT  PORTD
#define DIO2_DDR    DDRD
#define DIO2_PWM    nullptr

#define DIO3_PIN    PIND3
#define DIO3_RPORT  PIND
#define DIO3_WPORT  PORTD
#define DIO3_DDR    DDRD
#define DIO3_PWM    &OCR2B

#define DIO4_PIN    PIND4
#define DIO4_RPORT  PIND
#define DIO4_WPORT  PORTD
#define DIO4_DDR    DDRD
#define DIO4_PWM    nullptr

#define DIO5_PIN    PIND5
#define DIO5_RPORT  PIND
#define DIO5_WPORT  PORTD
#define DIO5_DDR    DDRD
#define DIO5_PWM    &OCR0B

#define DIO6_PIN    PIND6
#define DIO6_RPORT  PIND
#define DIO6_WPORT  PORTD
#define DIO6_DDR    DDRD
#define DIO6_PWM    &OCR0A

#define DIO7_PIN    PIND7
#define DIO7_RPORT  PIND
#define DIO7_WPORT  PORTD
#define DIO7_DDR    DDRD
#define DIO7_PWM    nullptr

#define DIO8_PIN    PINB0
#define DIO8_RPORT  PINB
#define DIO8_WPORT  PORTB
#define DIO8_DDR    DDRB
#define DIO8_PWM    nullptr

#define DIO9_PIN    PINB1
#define DIO9_RPORT  PINB
#define DIO9_WPORT  PORTB
#define DIO9_DDR    DDRB
#define DIO9_PWM    nullptr

#define DIO10_PIN   PINB2
#define DIO10_RPORT PINB
#define DIO10_WPORT PORTB
#define DIO10_DDR   DDRB
#define DIO10_PWM   nullptr

#define DIO11_PIN   PINB3
#define DIO11_RPORT PINB
#define DIO11_WPORT PORTB
#define DIO11_DDR   DDRB
#define DIO11_PWM   &OCR2A

#define DIO12_PIN   PINB4
#define DIO12_RPORT PINB
#define DIO12_WPORT PORTB
#define DIO12_DDR   DDRB
#define DIO12_PWM   nullptr

#define DIO13_PIN   PINB5
#define DIO13_RPORT PINB
#define DIO13_WPORT PORTB
#define DIO13_DDR   DDRB
#define DIO13_PWM   nullptr

#define DIO14_PIN   PINC0
#define DIO14_RPORT PINC
#define DIO14_WPORT PORTC
#define DIO14_DDR   DDRC
#define DIO14_PWM   nullptr

#define DIO15_PIN   PINC1
#define DIO15_RPORT PINC
#define DIO15_WPORT PORTC
#define DIO15_DDR   DDRC
#define DIO15_PWM   nullptr

#define DIO16_PIN   PINC2
#define DIO16_RPORT PINC
#define DIO16_WPORT PORTC
#define DIO16_DDR   DDRC
#define DIO16_PWM   nullptr

#define DIO17_PIN   PINC3
#define DIO17_RPORT PINC
#define DIO17_WPORT PORTC
#define DIO17_DDR   DDRC
#define DIO17_PWM   nullptr

#define DIO18_PIN   PINC4
#define DIO18_RPORT PINC
#define DIO18_WPORT PORTC
#define DIO18_DDR   DDRC
#define DIO18_PWM   nullptr

#define DIO19_PIN   PINC5
#define DIO19_RPORT PINC
#define DIO19_WPORT PORTC
#define DIO19_DDR   DDRC
#define DIO19_PWM   nullptr

#define DIO20_PIN   PINC6
#define DIO20_RPORT PINC
#define DIO20_WPORT PORTC
#define DIO20_DDR   DDRC
#define DIO20_PWM   nullptr

#define DIO21_PIN   PINC7
#define DIO21_RPORT PINC
#define DIO21_WPORT PORTC
#define DIO21_DDR   DDRC
#define DIO21_PWM   nullptr

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
#define PB3_PWM     &OCR2A

#undef PB4
#define PB4_PIN     PINB4
#define PB4_RPORT   PINB
#define PB4_WPORT   PORTB
#define PB4_DDR     DDRB
#define PB4_PWM     nullptr

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
#define PB7_PWM     nullptr

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
#define PD3_PWM     &OCR2B

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
#define PD5_PWM     &OCR0B

#undef PD6
#define PD6_PIN     PIND6
#define PD6_RPORT   PIND
#define PD6_WPORT   PORTD
#define PD6_DDR     DDRD
#define PD6_PWM     &OCR0A

#undef PD7
#define PD7_PIN     PIND7
#define PD7_RPORT   PIND
#define PD7_WPORT   PORTD
#define PD7_DDR     DDRD
#define PD7_PWM     nullptr
