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
  This code contributed by Triffid_Hunter and modified by Kliment
  why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
*/

#ifndef _FASTIO_ARDUINO_H
#define _FASTIO_ARDUINO_H

#include <avr/io.h>

/**
  utility functions
*/

#ifndef MASK
  #define MASK(PIN)  (1 << PIN)
#endif

/**
  magic I/O routines
  now you can simply SET_OUTPUT(STEP); WRITE(STEP, 1); WRITE(STEP, 0);
*/

/// Read a pin
#define _READ(IO) ((bool)(DIO ## IO ## _RPORT & MASK(DIO ## IO ## _PIN)))
/// write to a pin
// On some boards pins > 0x100 are used. These are not converted to atomic actions. An critical section is needed.

#define _WRITE_NC(IO, v)  do { if (v) {DIO ##  IO ## _WPORT |= MASK(DIO ## IO ## _PIN); } else {DIO ##  IO ## _WPORT &= ~MASK(DIO ## IO ## _PIN); }; } while (0)

#define _WRITE_C(IO, v)   do { if (v) { \
                                         CRITICAL_SECTION_START; \
                                         {DIO ##  IO ## _WPORT |= MASK(DIO ## IO ## _PIN); } \
                                         CRITICAL_SECTION_END; \
                                       } \
                                       else { \
                                         CRITICAL_SECTION_START; \
                                         {DIO ##  IO ## _WPORT &= ~MASK(DIO ## IO ## _PIN); } \
                                         CRITICAL_SECTION_END; \
                                       } \
                                     } \
                                     while (0)

#define _WRITE(IO, v)  do {  if (&(DIO ##  IO ## _RPORT) >= (uint8_t *)0x100) {_WRITE_C(IO, v); } else {_WRITE_NC(IO, v); }; } while (0)

/// toggle a pin
#define _TOGGLE(IO)  do {DIO ##  IO ## _RPORT = MASK(DIO ## IO ## _PIN); } while (0)

/// set pin as input
#define _SET_INPUT(IO) do {DIO ##  IO ## _DDR &= ~MASK(DIO ## IO ## _PIN); } while (0)
/// set pin as output
#define _SET_OUTPUT(IO) do {DIO ##  IO ## _DDR |=  MASK(DIO ## IO ## _PIN); } while (0)

/// check if pin is an input
#define _GET_INPUT(IO)  ((DIO ## IO ## _DDR & MASK(DIO ## IO ## _PIN)) == 0)
/// check if pin is an output
#define _GET_OUTPUT(IO)  ((DIO ## IO ## _DDR & MASK(DIO ## IO ## _PIN)) != 0)

/// check if pin is an timer
#define _GET_TIMER(IO)  (DIO ## IO ## _PWM)

//  why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html

/// Read a pin wrapper
#define READ(IO)  _READ(IO)
/// Write to a pin wrapper
#define WRITE(IO, v)  _WRITE(IO, v)

/// toggle a pin wrapper
#define TOGGLE(IO)  _TOGGLE(IO)

/// set pin as input wrapper
#define SET_INPUT(IO)  _SET_INPUT(IO)
/// set pin as input with pullup wrapper
#define SET_INPUT_PULLUP(IO) do{ _SET_INPUT(IO); _WRITE(IO, HIGH); }while(0)
/// set pin as output wrapper
#define SET_OUTPUT(IO)  _SET_OUTPUT(IO)

/// check if pin is an input wrapper
#define GET_INPUT(IO)  _GET_INPUT(IO)
/// check if pin is an output wrapper
#define GET_OUTPUT(IO)  _GET_OUTPUT(IO)

/// check if pin is an timer wrapper
#define GET_TIMER(IO)  _GET_TIMER(IO)

// Shorthand
#define OUT_WRITE(IO, v) { SET_OUTPUT(IO); WRITE(IO, v); }

/**
  ports and functions

  added as necessary or if I feel like it- not a comprehensive list!
*/

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
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

  // timers and PWM
  #define OC0A        DIO6
  #define OC0B        DIO5
  #define OC1A        DIO9
  #define OC1B        DIO10
  #define OC2A        DIO11
  #define OC2B        DIO3

  #define DEBUG_LED   AIO5

  /**
  pins
  */

  #define DIO0_PIN    PIND0
  #define DIO0_RPORT  PIND
  #define DIO0_WPORT  PORTD
  #define DIO0_DDR    DDRD
  #define DIO0_PWM    NULL

  #define DIO1_PIN    PIND1
  #define DIO1_RPORT  PIND
  #define DIO1_WPORT  PORTD
  #define DIO1_DDR    DDRD
  #define DIO1_PWM    NULL

  #define DIO2_PIN    PIND2
  #define DIO2_RPORT  PIND
  #define DIO2_WPORT  PORTD
  #define DIO2_DDR    DDRD
  #define DIO2_PWM    NULL

  #define DIO3_PIN    PIND3
  #define DIO3_RPORT  PIND
  #define DIO3_WPORT  PORTD
  #define DIO3_DDR    DDRD
  #define DIO3_PWM    &OCR2B

  #define DIO4_PIN    PIND4
  #define DIO4_RPORT  PIND
  #define DIO4_WPORT  PORTD
  #define DIO4_DDR    DDRD
  #define DIO4_PWM    NULL

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
  #define DIO7_PWM    NULL

  #define DIO8_PIN    PINB0
  #define DIO8_RPORT  PINB
  #define DIO8_WPORT  PORTB
  #define DIO8_DDR    DDRB
  #define DIO8_PWM    NULL

  #define DIO9_PIN    PINB1
  #define DIO9_RPORT  PINB
  #define DIO9_WPORT  PORTB
  #define DIO9_DDR    DDRB
  #define DIO9_PWM    NULL

  #define DIO10_PIN   PINB2
  #define DIO10_RPORT PINB
  #define DIO10_WPORT PORTB
  #define DIO10_DDR   DDRB
  #define DIO10_PWM   NULL

  #define DIO11_PIN   PINB3
  #define DIO11_RPORT PINB
  #define DIO11_WPORT PORTB
  #define DIO11_DDR   DDRB
  #define DIO11_PWM   &OCR2A

  #define DIO12_PIN   PINB4
  #define DIO12_RPORT PINB
  #define DIO12_WPORT PORTB
  #define DIO12_DDR   DDRB
  #define DIO12_PWM   NULL

  #define DIO13_PIN   PINB5
  #define DIO13_RPORT PINB
  #define DIO13_WPORT PORTB
  #define DIO13_DDR   DDRB
  #define DIO13_PWM   NULL


  #define DIO14_PIN   PINC0
  #define DIO14_RPORT PINC
  #define DIO14_WPORT PORTC
  #define DIO14_DDR   DDRC
  #define DIO14_PWM   NULL

  #define DIO15_PIN   PINC1
  #define DIO15_RPORT PINC
  #define DIO15_WPORT PORTC
  #define DIO15_DDR   DDRC
  #define DIO15_PWM   NULL

  #define DIO16_PIN   PINC2
  #define DIO16_RPORT PINC
  #define DIO16_WPORT PORTC
  #define DIO16_DDR   DDRC
  #define DIO16_PWM   NULL

  #define DIO17_PIN   PINC3
  #define DIO17_RPORT PINC
  #define DIO17_WPORT PORTC
  #define DIO17_DDR   DDRC
  #define DIO17_PWM   NULL

  #define DIO18_PIN   PINC4
  #define DIO18_RPORT PINC
  #define DIO18_WPORT PORTC
  #define DIO18_DDR   DDRC
  #define DIO18_PWM   NULL

  #define DIO19_PIN   PINC5
  #define DIO19_RPORT PINC
  #define DIO19_WPORT PORTC
  #define DIO19_DDR   DDRC
  #define DIO19_PWM   NULL

  #define DIO20_PIN   PINC6
  #define DIO20_RPORT PINC
  #define DIO20_WPORT PORTC
  #define DIO20_DDR   DDRC
  #define DIO20_PWM   NULL

  #define DIO21_PIN   PINC7
  #define DIO21_RPORT PINC
  #define DIO21_WPORT PORTC
  #define DIO21_DDR   DDRC
  #define DIO21_PWM   NULL

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
  #define PB3_PWM     &OCR2A

  #undef PB4
  #define PB4_PIN     PINB4
  #define PB4_RPORT   PINB
  #define PB4_WPORT   PORTB
  #define PB4_DDR     DDRB
  #define PB4_PWM     NULL

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
  #define PB7_PWM     NULL


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
  #define PD3_PWM     &OCR2B

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
  #define PD7_PWM     NULL
#endif  /*  _AVR_ATmega{168,328,328P}__ */

#if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284P__)
  // UART
  #define RXD         DIO8
  #define TXD         DIO9
  #define RXD0        DIO8
  #define TXD0        DIO9

  #define RXD1        DIO10
  #define TXD1        DIO11

  // SPI
  #define SCK         DIO7
  #define MISO        DIO6
  #define MOSI        DIO5
  #define SS          DIO4

  // TWI (I2C)
  #define SCL         DIO16
  #define SDA         DIO17

  // timers and PWM
  #define OC0A        DIO3
  #define OC0B        DIO4
  #define OC1A        DIO13
  #define OC1B        DIO12
  #define OC2A        DIO15
  #define OC2B        DIO14

  #define DEBUG_LED   DIO0
  /**
  pins
  */

  #define DIO0_PIN    PINB0
  #define DIO0_RPORT  PINB
  #define DIO0_WPORT  PORTB
  #define DIO0_DDR    DDRB
  #define DIO0_PWM    NULL

  #define DIO1_PIN    PINB1
  #define DIO1_RPORT  PINB
  #define DIO1_WPORT  PORTB
  #define DIO1_DDR    DDRB
  #define DIO1_PWM    NULL

  #define DIO2_PIN    PINB2
  #define DIO2_RPORT  PINB
  #define DIO2_WPORT  PORTB
  #define DIO2_DDR    DDRB
  #define DIO2_PWM    NULL

  #define DIO3_PIN    PINB3
  #define DIO3_RPORT  PINB
  #define DIO3_WPORT  PORTB
  #define DIO3_DDR    DDRB
  #define DIO3_PWM    OCR0A

  #define DIO4_PIN    PINB4
  #define DIO4_RPORT  PINB
  #define DIO4_WPORT  PORTB
  #define DIO4_DDR    DDRB
  #define DIO4_PWM    OCR0B

  #define DIO5_PIN    PINB5
  #define DIO5_RPORT  PINB
  #define DIO5_WPORT  PORTB
  #define DIO5_DDR    DDRB
  #define DIO5_PWM    NULL

  #define DIO6_PIN    PINB6
  #define DIO6_RPORT  PINB
  #define DIO6_WPORT  PORTB
  #define DIO6_DDR    DDRB
  #define DIO6_PWM    NULL

  #define DIO7_PIN    PINB7
  #define DIO7_RPORT  PINB
  #define DIO7_WPORT  PORTB
  #define DIO7_DDR    DDRB
  #define DIO7_PWM    NULL

  #define DIO8_PIN    PIND0
  #define DIO8_RPORT  PIND
  #define DIO8_WPORT  PORTD
  #define DIO8_DDR    DDRD
  #define DIO8_PWM    NULL

  #define DIO9_PIN    PIND1
  #define DIO9_RPORT  PIND
  #define DIO9_WPORT  PORTD
  #define DIO9_DDR    DDRD
  #define DIO9_PWM    NULL

  #define DIO10_PIN   PIND2
  #define DIO10_RPORT PIND
  #define DIO10_WPORT PORTD
  #define DIO10_DDR   DDRD
  #define DIO10_PWM   NULL

  #define DIO11_PIN   PIND3
  #define DIO11_RPORT PIND
  #define DIO11_WPORT PORTD
  #define DIO11_DDR   DDRD
  #define DIO11_PWM   NULL

  #define DIO12_PIN   PIND4
  #define DIO12_RPORT PIND
  #define DIO12_WPORT PORTD
  #define DIO12_DDR   DDRD
  #define DIO12_PWM   OCR1B

  #define DIO13_PIN   PIND5
  #define DIO13_RPORT PIND
  #define DIO13_WPORT PORTD
  #define DIO13_DDR   DDRD
  #define DIO13_PWM   OCR1A

  #define DIO14_PIN   PIND6
  #define DIO14_RPORT PIND
  #define DIO14_WPORT PORTD
  #define DIO14_DDR   DDRD
  #define DIO14_PWM   OCR2B

  #define DIO15_PIN   PIND7
  #define DIO15_RPORT PIND
  #define DIO15_WPORT PORTD
  #define DIO15_DDR   DDRD
  #define DIO15_PWM   OCR2A

  #define DIO16_PIN   PINC0
  #define DIO16_RPORT PINC
  #define DIO16_WPORT PORTC
  #define DIO16_DDR   DDRC
  #define DIO16_PWM   NULL

  #define DIO17_PIN   PINC1
  #define DIO17_RPORT PINC
  #define DIO17_WPORT PORTC
  #define DIO17_DDR   DDRC
  #define DIO17_PWM   NULL

  #define DIO18_PIN   PINC2
  #define DIO18_RPORT PINC
  #define DIO18_WPORT PORTC
  #define DIO18_DDR   DDRC
  #define DIO18_PWM   NULL

  #define DIO19_PIN   PINC3
  #define DIO19_RPORT PINC
  #define DIO19_WPORT PORTC
  #define DIO19_DDR   DDRC
  #define DIO19_PWM   NULL

  #define DIO20_PIN   PINC4
  #define DIO20_RPORT PINC
  #define DIO20_WPORT PORTC
  #define DIO20_DDR   DDRC
  #define DIO20_PWM   NULL

  #define DIO21_PIN   PINC5
  #define DIO21_RPORT PINC
  #define DIO21_WPORT PORTC
  #define DIO21_DDR   DDRC
  #define DIO21_PWM   NULL

  #define DIO22_PIN   PINC6
  #define DIO22_RPORT PINC
  #define DIO22_WPORT PORTC
  #define DIO22_DDR   DDRC
  #define DIO22_PWM   NULL

  #define DIO23_PIN   PINC7
  #define DIO23_RPORT PINC
  #define DIO23_WPORT PORTC
  #define DIO23_DDR   DDRC
  #define DIO23_PWM   NULL

  #define DIO24_PIN   PINA7
  #define DIO24_RPORT PINA
  #define DIO24_WPORT PORTA
  #define DIO24_DDR   DDRA
  #define DIO24_PWM   NULL

  #define DIO25_PIN   PINA6
  #define DIO25_RPORT PINA
  #define DIO25_WPORT PORTA
  #define DIO25_DDR   DDRA
  #define DIO25_PWM   NULL

  #define DIO26_PIN   PINA5
  #define DIO26_RPORT PINA
  #define DIO26_WPORT PORTA
  #define DIO26_DDR   DDRA
  #define DIO26_PWM   NULL

  #define DIO27_PIN   PINA4
  #define DIO27_RPORT PINA
  #define DIO27_WPORT PORTA
  #define DIO27_DDR   DDRA
  #define DIO27_PWM   NULL

  #define DIO28_PIN   PINA3
  #define DIO28_RPORT PINA
  #define DIO28_WPORT PORTA
  #define DIO28_DDR   DDRA
  #define DIO28_PWM   NULL

  #define DIO29_PIN   PINA2
  #define DIO29_RPORT PINA
  #define DIO29_WPORT PORTA
  #define DIO29_DDR   DDRA
  #define DIO29_PWM   NULL

  #define DIO30_PIN   PINA1
  #define DIO30_RPORT PINA
  #define DIO30_WPORT PORTA
  #define DIO30_DDR   DDRA
  #define DIO30_PWM   NULL

  #define DIO31_PIN   PINA0
  #define DIO31_RPORT PINA
  #define DIO31_WPORT PORTA
  #define DIO31_DDR   DDRA
  #define DIO31_PWM   NULL

  #define AIO0_PIN    PINA0
  #define AIO0_RPORT  PINA
  #define AIO0_WPORT  PORTA
  #define AIO0_DDR    DDRA
  #define AIO0_PWM    NULL

  #define AIO1_PIN    PINA1
  #define AIO1_RPORT  PINA
  #define AIO1_WPORT  PORTA
  #define AIO1_DDR    DDRA
  #define AIO1_PWM    NULL

  #define AIO2_PIN    PINA2
  #define AIO2_RPORT  PINA
  #define AIO2_WPORT  PORTA
  #define AIO2_DDR    DDRA
  #define AIO2_PWM    NULL

  #define AIO3_PIN    PINA3
  #define AIO3_RPORT  PINA
  #define AIO3_WPORT  PORTA
  #define AIO3_DDR    DDRA
  #define AIO3_PWM    NULL

  #define AIO4_PIN    PINA4
  #define AIO4_RPORT  PINA
  #define AIO4_WPORT  PORTA
  #define AIO4_DDR    DDRA
  #define AIO4_PWM    NULL

  #define AIO5_PIN    PINA5
  #define AIO5_RPORT  PINA
  #define AIO5_WPORT  PORTA
  #define AIO5_DDR    DDRA
  #define AIO5_PWM    NULL

  #define AIO6_PIN    PINA6
  #define AIO6_RPORT  PINA
  #define AIO6_WPORT  PORTA
  #define AIO6_DDR    DDRA
  #define AIO6_PWM    NULL

  #define AIO7_PIN    PINA7
  #define AIO7_RPORT  PINA
  #define AIO7_WPORT  PORTA
  #define AIO7_DDR    DDRA
  #define AIO7_PWM    NULL



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
  #define PB3_PWM     OCR0A

  #undef PB4
  #define PB4_PIN     PINB4
  #define PB4_RPORT   PINB
  #define PB4_WPORT   PORTB
  #define PB4_DDR     DDRB
  #define PB4_PWM     OCR0B

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
  #define PB7_PWM     NULL


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
  #define PD6_PWM     OCR2B

  #undef PD7
  #define PD7_PIN     PIND7
  #define PD7_RPORT   PIND
  #define PD7_WPORT   PORTD
  #define PD7_DDR     DDRD
  #define PD7_PWM     OCR2A
#endif  /*  _AVR_ATmega{644,644P,644PA}__ */

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
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

  // timers and PWM
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

  // change for your board
  #define DEBUG_LED   DIO21

  /**
  pins
  */
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
  #define DIO11_PWM   NULL

  #define DIO12_PIN   PINB6
  #define DIO12_RPORT PINB
  #define DIO12_WPORT PORTB
  #define DIO12_DDR   DDRB
  #define DIO12_PWM   NULL

  #define DIO13_PIN   PINB7
  #define DIO13_RPORT PINB
  #define DIO13_WPORT PORTB
  #define DIO13_DDR   DDRB
  #define DIO13_PWM   &OCR0A

  #define DIO14_PIN   PINJ1
  #define DIO14_RPORT PINJ
  #define DIO14_WPORT PORTJ
  #define DIO14_DDR   DDRJ
  #define DIO14_PWM   NULL

  #define DIO15_PIN   PINJ0
  #define DIO15_RPORT PINJ
  #define DIO15_WPORT PORTJ
  #define DIO15_DDR   DDRJ
  #define DIO15_PWM   NULL

  #define DIO16_PIN   PINH1
  #define DIO16_RPORT PINH
  #define DIO16_WPORT PORTH
  #define DIO16_DDR   DDRH
  #define DIO16_PWM   NULL

  #define DIO17_PIN   PINH0
  #define DIO17_RPORT PINH
  #define DIO17_WPORT PORTH
  #define DIO17_DDR   DDRH
  #define DIO17_PWM   NULL

  #define DIO18_PIN   PIND3
  #define DIO18_RPORT PIND
  #define DIO18_WPORT PORTD
  #define DIO18_DDR   DDRD
  #define DIO18_PWM   NULL

  #define DIO19_PIN   PIND2
  #define DIO19_RPORT PIND
  #define DIO19_WPORT PORTD
  #define DIO19_DDR   DDRD
  #define DIO19_PWM   NULL

  #define DIO20_PIN   PIND1
  #define DIO20_RPORT PIND
  #define DIO20_WPORT PORTD
  #define DIO20_DDR   DDRD
  #define DIO20_PWM   NULL

  #define DIO21_PIN   PIND0
  #define DIO21_RPORT PIND
  #define DIO21_WPORT PORTD
  #define DIO21_DDR   DDRD
  #define DIO21_PWM   NULL

  #define DIO22_PIN   PINA0
  #define DIO22_RPORT PINA
  #define DIO22_WPORT PORTA
  #define DIO22_DDR   DDRA
  #define DIO22_PWM   NULL

  #define DIO23_PIN   PINA1
  #define DIO23_RPORT PINA
  #define DIO23_WPORT PORTA
  #define DIO23_DDR   DDRA
  #define DIO23_PWM   NULL

  #define DIO24_PIN   PINA2
  #define DIO24_RPORT PINA
  #define DIO24_WPORT PORTA
  #define DIO24_DDR   DDRA
  #define DIO24_PWM   NULL

  #define DIO25_PIN   PINA3
  #define DIO25_RPORT PINA
  #define DIO25_WPORT PORTA
  #define DIO25_DDR   DDRA
  #define DIO25_PWM   NULL

  #define DIO26_PIN   PINA4
  #define DIO26_RPORT PINA
  #define DIO26_WPORT PORTA
  #define DIO26_DDR   DDRA
  #define DIO26_PWM   NULL

  #define DIO27_PIN   PINA5
  #define DIO27_RPORT PINA
  #define DIO27_WPORT PORTA
  #define DIO27_DDR   DDRA
  #define DIO27_PWM   NULL

  #define DIO28_PIN   PINA6
  #define DIO28_RPORT PINA
  #define DIO28_WPORT PORTA
  #define DIO28_DDR   DDRA
  #define DIO28_PWM   NULL

  #define DIO29_PIN   PINA7
  #define DIO29_RPORT PINA
  #define DIO29_WPORT PORTA
  #define DIO29_DDR   DDRA
  #define DIO29_PWM   NULL

  #define DIO30_PIN   PINC7
  #define DIO30_RPORT PINC
  #define DIO30_WPORT PORTC
  #define DIO30_DDR   DDRC
  #define DIO30_PWM   NULL

  #define DIO31_PIN   PINC6
  #define DIO31_RPORT PINC
  #define DIO31_WPORT PORTC
  #define DIO31_DDR   DDRC
  #define DIO31_PWM   NULL

  #define DIO32_PIN   PINC5
  #define DIO32_RPORT PINC
  #define DIO32_WPORT PORTC
  #define DIO32_DDR   DDRC
  #define DIO32_PWM   NULL

  #define DIO33_PIN   PINC4
  #define DIO33_RPORT PINC
  #define DIO33_WPORT PORTC
  #define DIO33_DDR   DDRC
  #define DIO33_PWM   NULL

  #define DIO34_PIN   PINC3
  #define DIO34_RPORT PINC
  #define DIO34_WPORT PORTC
  #define DIO34_DDR   DDRC
  #define DIO34_PWM   NULL

  #define DIO35_PIN   PINC2
  #define DIO35_RPORT PINC
  #define DIO35_WPORT PORTC
  #define DIO35_DDR   DDRC
  #define DIO35_PWM   NULL

  #define DIO36_PIN   PINC1
  #define DIO36_RPORT PINC
  #define DIO36_WPORT PORTC
  #define DIO36_DDR   DDRC
  #define DIO36_PWM   NULL

  #define DIO37_PIN   PINC0
  #define DIO37_RPORT PINC
  #define DIO37_WPORT PORTC
  #define DIO37_DDR   DDRC
  #define DIO37_PWM   NULL

  #define DIO38_PIN   PIND7
  #define DIO38_RPORT PIND
  #define DIO38_WPORT PORTD
  #define DIO38_DDR   DDRD
  #define DIO38_PWM   NULL

  #define DIO39_PIN   PING2
  #define DIO39_RPORT PING
  #define DIO39_WPORT PORTG
  #define DIO39_DDR   DDRG
  #define DIO39_PWM   NULL

  #define DIO40_PIN   PING1
  #define DIO40_RPORT PING
  #define DIO40_WPORT PORTG
  #define DIO40_DDR   DDRG
  #define DIO40_PWM   NULL

  #define DIO41_PIN   PING0
  #define DIO41_RPORT PING
  #define DIO41_WPORT PORTG
  #define DIO41_DDR   DDRG
  #define DIO41_PWM   NULL

  #define DIO42_PIN   PINL7
  #define DIO42_RPORT PINL
  #define DIO42_WPORT PORTL
  #define DIO42_DDR   DDRL
  #define DIO42_PWM   NULL

  #define DIO43_PIN   PINL6
  #define DIO43_RPORT PINL
  #define DIO43_WPORT PORTL
  #define DIO43_DDR   DDRL
  #define DIO43_PWM   NULL

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
  #define DIO47_PWM   NULL

  #define DIO48_PIN   PINL1
  #define DIO48_RPORT PINL
  #define DIO48_WPORT PORTL
  #define DIO48_DDR   DDRL
  #define DIO48_PWM   NULL

  #define DIO49_PIN   PINL0
  #define DIO49_RPORT PINL
  #define DIO49_WPORT PORTL
  #define DIO49_DDR   DDRL
  #define DIO49_PWM   NULL

  #define DIO50_PIN   PINB3
  #define DIO50_RPORT PINB
  #define DIO50_WPORT PORTB
  #define DIO50_DDR   DDRB
  #define DIO50_PWM   NULL

  #define DIO51_PIN   PINB2
  #define DIO51_RPORT PINB
  #define DIO51_WPORT PORTB
  #define DIO51_DDR   DDRB
  #define DIO51_PWM   NULL

  #define DIO52_PIN   PINB1
  #define DIO52_RPORT PINB
  #define DIO52_WPORT PORTB
  #define DIO52_DDR   DDRB
  #define DIO52_PWM   NULL

  #define DIO53_PIN   PINB0
  #define DIO53_RPORT PINB
  #define DIO53_WPORT PORTB
  #define DIO53_DDR   DDRB
  #define DIO53_PWM   NULL

  #define DIO54_PIN   PINF0
  #define DIO54_RPORT PINF
  #define DIO54_WPORT PORTF
  #define DIO54_DDR   DDRF
  #define DIO54_PWM   NULL

  #define DIO55_PIN   PINF1
  #define DIO55_RPORT PINF
  #define DIO55_WPORT PORTF
  #define DIO55_DDR   DDRF
  #define DIO55_PWM   NULL

  #define DIO56_PIN   PINF2
  #define DIO56_RPORT PINF
  #define DIO56_WPORT PORTF
  #define DIO56_DDR   DDRF
  #define DIO56_PWM   NULL

  #define DIO57_PIN   PINF3
  #define DIO57_RPORT PINF
  #define DIO57_WPORT PORTF
  #define DIO57_DDR   DDRF
  #define DIO57_PWM   NULL

  #define DIO58_PIN   PINF4
  #define DIO58_RPORT PINF
  #define DIO58_WPORT PORTF
  #define DIO58_DDR   DDRF
  #define DIO58_PWM   NULL

  #define DIO59_PIN   PINF5
  #define DIO59_RPORT PINF
  #define DIO59_WPORT PORTF
  #define DIO59_DDR   DDRF
  #define DIO59_PWM   NULL

  #define DIO60_PIN   PINF6
  #define DIO60_RPORT PINF
  #define DIO60_WPORT PORTF
  #define DIO60_DDR   DDRF
  #define DIO60_PWM   NULL

  #define DIO61_PIN   PINF7
  #define DIO61_RPORT PINF
  #define DIO61_WPORT PORTF
  #define DIO61_DDR   DDRF
  #define DIO61_PWM   NULL

  #define DIO62_PIN   PINK0
  #define DIO62_RPORT PINK
  #define DIO62_WPORT PORTK
  #define DIO62_DDR   DDRK
  #define DIO62_PWM   NULL

  #define DIO63_PIN   PINK1
  #define DIO63_RPORT PINK
  #define DIO63_WPORT PORTK
  #define DIO63_DDR   DDRK
  #define DIO63_PWM   NULL

  #define DIO64_PIN   PINK2
  #define DIO64_RPORT PINK
  #define DIO64_WPORT PORTK
  #define DIO64_DDR   DDRK
  #define DIO64_PWM   NULL

  #define DIO65_PIN   PINK3
  #define DIO65_RPORT PINK
  #define DIO65_WPORT PORTK
  #define DIO65_DDR   DDRK
  #define DIO65_PWM   NULL

  #define DIO66_PIN   PINK4
  #define DIO66_RPORT PINK
  #define DIO66_WPORT PORTK
  #define DIO66_DDR   DDRK
  #define DIO66_PWM   NULL

  #define DIO67_PIN   PINK5
  #define DIO67_RPORT PINK
  #define DIO67_WPORT PORTK
  #define DIO67_DDR   DDRK
  #define DIO67_PWM   NULL

  #define DIO68_PIN   PINK6
  #define DIO68_RPORT PINK
  #define DIO68_WPORT PORTK
  #define DIO68_DDR   DDRK
  #define DIO68_PWM   NULL

  #define DIO69_PIN   PINK7
  #define DIO69_RPORT PINK
  #define DIO69_WPORT PORTK
  #define DIO69_DDR   DDRK
  #define DIO69_PWM   NULL

  #define DIO70_PIN   PING4
  #define DIO70_RPORT PING
  #define DIO70_WPORT PORTG
  #define DIO70_DDR   DDRG
  #define DIO70_PWM   NULL

  #define DIO71_PIN   PING3
  #define DIO71_RPORT PING
  #define DIO71_WPORT PORTG
  #define DIO71_DDR   DDRG
  #define DIO71_PWM   NULL

  #define DIO72_PIN   PINJ2
  #define DIO72_RPORT PINJ
  #define DIO72_WPORT PORTJ
  #define DIO72_DDR   DDRJ
  #define DIO72_PWM   NULL

  #define DIO73_PIN   PINJ3
  #define DIO73_RPORT PINJ
  #define DIO73_WPORT PORTJ
  #define DIO73_DDR   DDRJ
  #define DIO73_PWM   NULL

  #define DIO74_PIN   PINJ7
  #define DIO74_RPORT PINJ
  #define DIO74_WPORT PORTJ
  #define DIO74_DDR   DDRJ
  #define DIO74_PWM   NULL

  #define DIO75_PIN   PINJ4
  #define DIO75_RPORT PINJ
  #define DIO75_WPORT PORTJ
  #define DIO75_DDR   DDRJ
  #define DIO75_PWM   NULL

  #define DIO76_PIN   PINJ5
  #define DIO76_RPORT PINJ
  #define DIO76_WPORT PORTJ
  #define DIO76_DDR   DDRJ
  #define DIO76_PWM   NULL

  #define DIO77_PIN   PINJ6
  #define DIO77_RPORT PINJ
  #define DIO77_WPORT PORTJ
  #define DIO77_DDR   DDRJ
  #define DIO77_PWM   NULL

  #define DIO78_PIN   PINE2
  #define DIO78_RPORT PINE
  #define DIO78_WPORT PORTE
  #define DIO78_DDR   DDRE
  #define DIO78_PWM   NULL

  #define DIO79_PIN   PINE6
  #define DIO79_RPORT PINE
  #define DIO79_WPORT PORTE
  #define DIO79_DDR   DDRE
  #define DIO79_PWM   NULL

  #define DIO80_PIN   PINE7
  #define DIO80_RPORT PINE
  #define DIO80_WPORT PORTE
  #define DIO80_DDR   DDRE
  #define DIO80_PWM   NULL

  #define DIO81_PIN   PIND4
  #define DIO81_RPORT PIND
  #define DIO81_WPORT PORTD
  #define DIO81_DDR   DDRD
  #define DIO81_PWM   NULL

  #define DIO82_PIN   PIND5
  #define DIO82_RPORT PIND
  #define DIO82_WPORT PORTD
  #define DIO82_DDR   DDRD
  #define DIO82_PWM   NULL

  #define DIO83_PIN   PIND6
  #define DIO83_RPORT PIND
  #define DIO83_WPORT PORTD
  #define DIO83_DDR   DDRD
  #define DIO83_PWM   NULL

  #define DIO84_PIN   PINH2
  #define DIO84_RPORT PINH
  #define DIO84_WPORT PORTH
  #define DIO84_DDR   DDRH
  #define DIO84_PWM   NULL

  #define DIO85_PIN   PINH7
  #define DIO85_RPORT PINH
  #define DIO85_WPORT PORTH
  #define DIO85_DDR   DDRH
  #define DIO85_PWM   NULL

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

  #undef PH0
  #define PH0_PIN     PINH0
  #define PH0_RPORT   PINH
  #define PH0_WPORT   PORTH
  #define PH0_DDR     DDRH
  #define PH0_PWM     NULL
  #undef PH1
  #define PH1_PIN     PINH1
  #define PH1_RPORT   PINH
  #define PH1_WPORT   PORTH
  #define PH1_DDR     DDRH
  #define PH1_PWM     NULL
  #undef PH2
  #define PH2_PIN     PINH2
  #define PH2_RPORT   PINH
  #define PH2_WPORT   PORTH
  #define PH2_DDR     DDRH
  #define PH2_PWM     NULL
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
  #define PH7_PWM     NULL

  #undef PJ0
  #define PJ0_PIN     PINJ0
  #define PJ0_RPORT   PINJ
  #define PJ0_WPORT   PORTJ
  #define PJ0_DDR     DDRJ
  #define PJ0_PWM     NULL
  #undef PJ1
  #define PJ1_PIN     PINJ1
  #define PJ1_RPORT   PINJ
  #define PJ1_WPORT   PORTJ
  #define PJ1_DDR     DDRJ
  #define PJ1_PWM     NULL
  #undef PJ2
  #define PJ2_PIN     PINJ2
  #define PJ2_RPORT   PINJ
  #define PJ2_WPORT   PORTJ
  #define PJ2_DDR     DDRJ
  #define PJ2_PWM     NULL
  #undef PJ3
  #define PJ3_PIN     PINJ3
  #define PJ3_RPORT   PINJ
  #define PJ3_WPORT   PORTJ
  #define PJ3_DDR     DDRJ
  #define PJ3_PWM     NULL
  #undef PJ4
  #define PJ4_PIN     PINJ4
  #define PJ4_RPORT   PINJ
  #define PJ4_WPORT   PORTJ
  #define PJ4_DDR     DDRJ
  #define PJ4_PWM     NULL
  #undef PJ5
  #define PJ5_PIN     PINJ5
  #define PJ5_RPORT   PINJ
  #define PJ5_WPORT   PORTJ
  #define PJ5_DDR     DDRJ
  #define PJ5_PWM     NULL
  #undef PJ6
  #define PJ6_PIN     PINJ6
  #define PJ6_RPORT   PINJ
  #define PJ6_WPORT   PORTJ
  #define PJ6_DDR     DDRJ
  #define PJ6_PWM     NULL
  #undef PJ7
  #define PJ7_PIN     PINJ7
  #define PJ7_RPORT   PINJ
  #define PJ7_WPORT   PORTJ
  #define PJ7_DDR     DDRJ
  #define PJ7_PWM     NULL

  #undef PK0
  #define PK0_PIN     PINK0
  #define PK0_RPORT   PINK
  #define PK0_WPORT   PORTK
  #define PK0_DDR     DDRK
  #define PK0_PWM     NULL
  #undef PK1
  #define PK1_PIN     PINK1
  #define PK1_RPORT   PINK
  #define PK1_WPORT   PORTK
  #define PK1_DDR     DDRK
  #define PK1_PWM     NULL
  #undef PK2
  #define PK2_PIN     PINK2
  #define PK2_RPORT   PINK
  #define PK2_WPORT   PORTK
  #define PK2_DDR     DDRK
  #define PK2_PWM     NULL
  #undef PK3
  #define PK3_PIN     PINK3
  #define PK3_RPORT   PINK
  #define PK3_WPORT   PORTK
  #define PK3_DDR     DDRK
  #define PK3_PWM     NULL
  #undef PK4
  #define PK4_PIN     PINK4
  #define PK4_RPORT   PINK
  #define PK4_WPORT   PORTK
  #define PK4_DDR     DDRK
  #define PK4_PWM     NULL
  #undef PK5
  #define PK5_PIN     PINK5
  #define PK5_RPORT   PINK
  #define PK5_WPORT   PORTK
  #define PK5_DDR     DDRK
  #define PK5_PWM     NULL
  #undef PK6
  #define PK6_PIN     PINK6
  #define PK6_RPORT   PINK
  #define PK6_WPORT   PORTK
  #define PK6_DDR     DDRK
  #define PK6_PWM     NULL
  #undef PK7
  #define PK7_PIN     PINK7
  #define PK7_RPORT   PINK
  #define PK7_WPORT   PORTK
  #define PK7_DDR     DDRK
  #define PK7_PWM     NULL

  #undef PL0
  #define PL0_PIN     PINL0
  #define PL0_RPORT   PINL
  #define PL0_WPORT   PORTL
  #define PL0_DDR     DDRL
  #define PL0_PWM     NULL
  #undef PL1
  #define PL1_PIN     PINL1
  #define PL1_RPORT   PINL
  #define PL1_WPORT   PORTL
  #define PL1_DDR     DDRL
  #define PL1_PWM     NULL
  #undef PL2
  #define PL2_PIN     PINL2
  #define PL2_RPORT   PINL
  #define PL2_WPORT   PORTL
  #define PL2_DDR     DDRL
  #define PL2_PWM     NULL
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
  #define PL6_PWM     NULL
  #undef PL7
  #define PL7_PIN     PINL7
  #define PL7_RPORT   PINL
  #define PL7_WPORT   PORTL
  #define PL7_DDR     DDRL
  #define PL7_PWM     NULL

#endif

#if defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__)

  // change for your board
  #define DEBUG_LED   DIO31 /* led D5 red */

  /**
  pins
  */

  //#define AT90USBxx_TEENSYPP_ASSIGNMENTS // Use Teensy++ 2.0 assignments
  #ifndef AT90USBxx_TEENSYPP_ASSIGNMENTS // Use traditional Marlin pin assignments

    // SPI
    #define SCK         DIO9   // 21
    #define MISO        DIO11  // 23
    #define MOSI        DIO10  // 22
    #define SS          DIO8   // 20

    #define DIO0_PIN    PINA0
    #define DIO0_RPORT  PINA
    #define DIO0_WPORT  PORTA
    #define DIO0_PWM    NULL
    #define DIO0_DDR    DDRA

    #define DIO1_PIN    PINA1
    #define DIO1_RPORT  PINA
    #define DIO1_WPORT  PORTA
    #define DIO1_PWM    NULL
    #define DIO1_DDR    DDRA

    #define DIO2_PIN    PINA2
    #define DIO2_RPORT  PINA
    #define DIO2_WPORT  PORTA
    #define DIO2_PWM    NULL
    #define DIO2_DDR    DDRA

    #define DIO3_PIN    PINA3
    #define DIO3_RPORT  PINA
    #define DIO3_WPORT  PORTA
    #define DIO3_PWM    NULL
    #define DIO3_DDR    DDRA

    #define DIO4_PIN    PINA4
    #define DIO4_RPORT  PINA
    #define DIO4_WPORT  PORTA
    #define DIO4_PWM    NULL
    #define DIO4_DDR    DDRA

    #define DIO5_PIN    PINA5
    #define DIO5_RPORT  PINA
    #define DIO5_WPORT  PORTA
    #define DIO5_PWM    NULL
    #define DIO5_DDR    DDRA

    #define DIO6_PIN    PINA6
    #define DIO6_RPORT  PINA
    #define DIO6_WPORT  PORTA
    #define DIO6_PWM    NULL
    #define DIO6_DDR    DDRA

    #define DIO7_PIN    PINA7
    #define DIO7_RPORT  PINA
    #define DIO7_WPORT  PORTA
    #define DIO7_PWM    NULL
    #define DIO7_DDR    DDRA

    #define DIO8_PIN    PINB0
    #define DIO8_RPORT  PINB
    #define DIO8_WPORT  PORTB
    #define DIO8_PWM    NULL
    #define DIO8_DDR    DDRB

    #define DIO9_PIN    PINB1
    #define DIO9_RPORT  PINB
    #define DIO9_WPORT  PORTB
    #define DIO9_PWM    NULL
    #define DIO9_DDR    DDRB

    #define DIO10_PIN   PINB2
    #define DIO10_RPORT PINB
    #define DIO10_WPORT PORTB
    #define DIO10_PWM   NULL
    #define DIO10_DDR   DDRB

    #define DIO11_PIN   PINB3
    #define DIO11_RPORT PINB
    #define DIO11_WPORT PORTB
    #define DIO11_PWM   NULL
    #define DIO11_DDR   DDRB

    #define DIO12_PIN   PINB4
    #define DIO12_RPORT PINB
    #define DIO12_WPORT PORTB
    #define DIO12_PWM   NULL
    #define DIO12_DDR   DDRB

    #define DIO13_PIN   PINB5
    #define DIO13_RPORT PINB
    #define DIO13_WPORT PORTB
    #define DIO13_PWM   NULL
    #define DIO13_DDR   DDRB

    #define DIO14_PIN   PINB6
    #define DIO14_RPORT PINB
    #define DIO14_WPORT PORTB
    #define DIO14_PWM   NULL
    #define DIO14_DDR   DDRB

    #define DIO15_PIN   PINB7
    #define DIO15_RPORT PINB
    #define DIO15_WPORT PORTB
    #define DIO15_PWM   NULL
    #define DIO15_DDR   DDRB

    #define DIO16_PIN   PINC0
    #define DIO16_RPORT PINC
    #define DIO16_WPORT PORTC
    #define DIO16_PWM   NULL
    #define DIO16_DDR   DDRC

    #define DIO17_PIN   PINC1
    #define DIO17_RPORT PINC
    #define DIO17_WPORT PORTC
    #define DIO17_PWM   NULL
    #define DIO17_DDR   DDRC

    #define DIO18_PIN   PINC2
    #define DIO18_RPORT PINC
    #define DIO18_WPORT PORTC
    #define DIO18_PWM   NULL
    #define DIO18_DDR   DDRC

    #define DIO19_PIN   PINC3
    #define DIO19_RPORT PINC
    #define DIO19_WPORT PORTC
    #define DIO19_PWM   NULL
    #define DIO19_DDR   DDRC

    #define DIO20_PIN   PINC4
    #define DIO20_RPORT PINC
    #define DIO20_WPORT PORTC
    #define DIO20_PWM   NULL
    #define DIO20_DDR   DDRC

    #define DIO21_PIN   PINC5
    #define DIO21_RPORT PINC
    #define DIO21_WPORT PORTC
    #define DIO21_PWM   NULL
    #define DIO21_DDR   DDRC

    #define DIO22_PIN   PINC6
    #define DIO22_RPORT PINC
    #define DIO22_WPORT PORTC
    #define DIO22_PWM   NULL
    #define DIO22_DDR   DDRC

    #define DIO23_PIN   PINC7
    #define DIO23_RPORT PINC
    #define DIO23_WPORT PORTC
    #define DIO23_PWM   NULL
    #define DIO23_DDR   DDRC

    #define DIO24_PIN   PIND0
    #define DIO24_RPORT PIND
    #define DIO24_WPORT PORTD
    #define DIO24_PWM   NULL
    #define DIO24_DDR   DDRD

    #define DIO25_PIN   PIND1
    #define DIO25_RPORT PIND
    #define DIO25_WPORT PORTD
    #define DIO25_PWM   NULL
    #define DIO25_DDR   DDRD

    #define DIO26_PIN   PIND2
    #define DIO26_RPORT PIND
    #define DIO26_WPORT PORTD
    #define DIO26_PWM   NULL
    #define DIO26_DDR   DDRD

    #define DIO27_PIN   PIND3
    #define DIO27_RPORT PIND
    #define DIO27_WPORT PORTD
    #define DIO27_PWM   NULL
    #define DIO27_DDR   DDRD

    #define DIO28_PIN   PIND4
    #define DIO28_RPORT PIND
    #define DIO28_WPORT PORTD
    #define DIO28_PWM   NULL
    #define DIO28_DDR   DDRD

    #define DIO29_PIN   PIND5
    #define DIO29_RPORT PIND
    #define DIO29_WPORT PORTD
    #define DIO29_PWM   NULL
    #define DIO29_DDR   DDRD

    #define DIO30_PIN   PIND6
    #define DIO30_RPORT PIND
    #define DIO30_WPORT PORTD
    #define DIO30_PWM   NULL
    #define DIO30_DDR   DDRD

    #define DIO31_PIN   PIND7
    #define DIO31_RPORT PIND
    #define DIO31_WPORT PORTD
    #define DIO31_PWM   NULL
    #define DIO31_DDR   DDRD


    #define DIO32_PIN   PINE0
    #define DIO32_RPORT PINE
    #define DIO32_WPORT PORTE
    #define DIO32_PWM   NULL
    #define DIO32_DDR   DDRE

    #define DIO33_PIN   PINE1
    #define DIO33_RPORT PINE
    #define DIO33_WPORT PORTE
    #define DIO33_PWM   NULL
    #define DIO33_DDR   DDRE

    #define DIO34_PIN   PINE2
    #define DIO34_RPORT PINE
    #define DIO34_WPORT PORTE
    #define DIO34_PWM   NULL
    #define DIO34_DDR   DDRE

    #define DIO35_PIN   PINE3
    #define DIO35_RPORT PINE
    #define DIO35_WPORT PORTE
    #define DIO35_PWM   NULL
    #define DIO35_DDR   DDRE

    #define DIO36_PIN   PINE4
    #define DIO36_RPORT PINE
    #define DIO36_WPORT PORTE
    #define DIO36_PWM   NULL
    #define DIO36_DDR   DDRE

    #define DIO37_PIN   PINE5
    #define DIO37_RPORT PINE
    #define DIO37_WPORT PORTE
    #define DIO37_PWM   NULL
    #define DIO37_DDR   DDRE

    #define DIO38_PIN   PINE6
    #define DIO38_RPORT PINE
    #define DIO38_WPORT PORTE
    #define DIO38_PWM   NULL
    #define DIO38_DDR   DDRE

    #define DIO39_PIN   PINE7
    #define DIO39_RPORT PINE
    #define DIO39_WPORT PORTE
    #define DIO39_PWM   NULL
    #define DIO39_DDR   DDRE

    #define AIO0_PIN    PINF0
    #define AIO0_RPORT  PINF
    #define AIO0_WPORT  PORTF
    #define AIO0_PWM    NULL
    #define AIO0_DDR    DDRF

    #define AIO1_PIN    PINF1
    #define AIO1_RPORT  PINF
    #define AIO1_WPORT  PORTF
    #define AIO1_PWM    NULL
    #define AIO1_DDR    DDRF

    #define AIO2_PIN    PINF2
    #define AIO2_RPORT  PINF
    #define AIO2_WPORT  PORTF
    #define AIO2_PWM    NULL
    #define AIO2_DDR    DDRF

    #define AIO3_PIN    PINF3
    #define AIO3_RPORT  PINF
    #define AIO3_WPORT  PORTF
    #define AIO3_PWM    NULL
    #define AIO3_DDR    DDRF

    #define AIO4_PIN    PINF4
    #define AIO4_RPORT  PINF
    #define AIO4_WPORT  PORTF
    #define AIO4_PWM    NULL
    #define AIO4_DDR    DDRF

    #define AIO5_PIN    PINF5
    #define AIO5_RPORT  PINF
    #define AIO5_WPORT  PORTF
    #define AIO5_PWM    NULL
    #define AIO5_DDR    DDRF

    #define AIO6_PIN    PINF6
    #define AIO6_RPORT  PINF
    #define AIO6_WPORT  PORTF
    #define AIO6_PWM    NULL
    #define AIO6_DDR    DDRF

    #define AIO7_PIN    PINF7
    #define AIO7_RPORT  PINF
    #define AIO7_WPORT  PORTF
    #define AIO7_PWM    NULL
    #define AIO7_DDR    DDRF

    #define DIO40_PIN   PINF0
    #define DIO40_RPORT PINF
    #define DIO40_WPORT PORTF
    #define DIO40_PWM   NULL
    #define DIO40_DDR   DDRF

    #define DIO41_PIN   PINF1
    #define DIO41_RPORT PINF
    #define DIO41_WPORT PORTF
    #define DIO41_PWM   NULL
    #define DIO41_DDR   DDRF

    #define DIO42_PIN   PINF2
    #define DIO42_RPORT PINF
    #define DIO42_WPORT PORTF
    #define DIO42_PWM   NULL
    #define DIO42_DDR   DDRF

    #define DIO43_PIN   PINF3
    #define DIO43_RPORT PINF
    #define DIO43_WPORT PORTF
    #define DIO43_PWM   NULL
    #define DIO43_DDR   DDRF

    #define DIO44_PIN   PINF4
    #define DIO44_RPORT PINF
    #define DIO44_WPORT PORTF
    #define DIO44_PWM   NULL
    #define DIO44_DDR   DDRF

    #define DIO45_PIN   PINF5
    #define DIO45_RPORT PINF
    #define DIO45_WPORT PORTF
    #define DIO45_PWM   NULL
    #define DIO45_DDR   DDRF

    #define DIO46_PIN   PINF6
    #define DIO46_RPORT PINF
    #define DIO46_WPORT PORTF
    #define DIO46_PWM   NULL
    #define DIO46_DDR   DDRF

    #define DIO47_PIN   PINF7
    #define DIO47_RPORT PINF
    #define DIO47_WPORT PORTF
    #define DIO47_PWM   NULL
    #define DIO47_DDR   DDRF


    #undef PA0
    #define PA0_PIN     PINA0
    #define PA0_RPORT   PINA
    #define PA0_WPORT   PORTA
    #define PA0_PWM     NULL
    #define PA0_DDR     DDRA
    #undef PA1
    #define PA1_PIN     PINA1
    #define PA1_RPORT   PINA
    #define PA1_WPORT   PORTA
    #define PA1_PWM     NULL
    #define PA1_DDR     DDRA
    #undef PA2
    #define PA2_PIN     PINA2
    #define PA2_RPORT   PINA
    #define PA2_WPORT   PORTA
    #define PA2_PWM     NULL
    #define PA2_DDR     DDRA
    #undef PA3
    #define PA3_PIN     PINA3
    #define PA3_RPORT   PINA
    #define PA3_WPORT   PORTA
    #define PA3_PWM     NULL
    #define PA3_DDR     DDRA
    #undef PA4
    #define PA4_PIN     PINA4
    #define PA4_RPORT   PINA
    #define PA4_WPORT   PORTA
    #define PA4_PWM     NULL
    #define PA4_DDR     DDRA
    #undef PA5
    #define PA5_PIN     PINA5
    #define PA5_RPORT   PINA
    #define PA5_WPORT   PORTA
    #define PA5_PWM     NULL
    #define PA5_DDR     DDRA
    #undef PA6
    #define PA6_PIN     PINA6
    #define PA6_RPORT   PINA
    #define PA6_WPORT   PORTA
    #define PA6_PWM     NULL
    #define PA6_DDR     DDRA
    #undef PA7
    #define PA7_PIN     PINA7
    #define PA7_RPORT   PINA
    #define PA7_WPORT   PORTA
    #define PA7_PWM     NULL
    #define PA7_DDR     DDRA

    #undef PB0
    #define PB0_PIN     PINB0
    #define PB0_RPORT   PINB
    #define PB0_WPORT   PORTB
    #define PB0_PWM     NULL
    #define PB0_DDR     DDRB
    #undef PB1
    #define PB1_PIN     PINB1
    #define PB1_RPORT   PINB
    #define PB1_WPORT   PORTB
    #define PB1_PWM     NULL
    #define PB1_DDR     DDRB
    #undef PB2
    #define PB2_PIN     PINB2
    #define PB2_RPORT   PINB
    #define PB2_WPORT   PORTB
    #define PB2_PWM     NULL
    #define PB2_DDR     DDRB
    #undef PB3
    #define PB3_PIN     PINB3
    #define PB3_RPORT   PINB
    #define PB3_WPORT   PORTB
    #define PB3_PWM     NULL
    #define PB3_DDR     DDRB
    #undef PB4
    #define PB4_PIN     PINB4
    #define PB4_RPORT   PINB
    #define PB4_WPORT   PORTB
    #define PB4_PWM     NULL
    #define PB4_DDR     DDRB
    #undef PB5
    #define PB5_PIN     PINB5
    #define PB5_RPORT   PINB
    #define PB5_WPORT   PORTB
    #define PB5_PWM     NULL
    #define PB5_DDR     DDRB
    #undef PB6
    #define PB6_PIN     PINB6
    #define PB6_RPORT   PINB
    #define PB6_WPORT   PORTB
    #define PB6_PWM     NULL
    #define PB6_DDR     DDRB
    #undef PB7
    #define PB7_PIN     PINB7
    #define PB7_RPORT   PINB
    #define PB7_WPORT   PORTB
    #define PB7_PWM     NULL
    #define PB7_DDR     DDRB

    #undef PC0
    #define PC0_PIN     PINC0
    #define PC0_RPORT   PINC
    #define PC0_WPORT   PORTC
    #define PC0_PWM     NULL
    #define PC0_DDR     DDRC
    #undef PC1
    #define PC1_PIN     PINC1
    #define PC1_RPORT   PINC
    #define PC1_WPORT   PORTC
    #define PC1_PWM     NULL
    #define PC1_DDR     DDRC
    #undef PC2
    #define PC2_PIN     PINC2
    #define PC2_RPORT   PINC
    #define PC2_WPORT   PORTC
    #define PC2_PWM     NULL
    #define PC2_DDR     DDRC
    #undef PC3
    #define PC3_PIN     PINC3
    #define PC3_RPORT   PINC
    #define PC3_WPORT   PORTC
    #define PC3_PWM     NULL
    #define PC3_DDR     DDRC
    #undef PC4
    #define PC4_PIN     PINC4
    #define PC4_RPORT   PINC
    #define PC4_WPORT   PORTC
    #define PC4_PWM     NULL
    #define PC4_DDR     DDRC
    #undef PC5
    #define PC5_PIN     PINC5
    #define PC5_RPORT   PINC
    #define PC5_WPORT   PORTC
    #define PC5_PWM     NULL
    #define PC5_DDR     DDRC
    #undef PC6
    #define PC6_PIN     PINC6
    #define PC6_RPORT   PINC
    #define PC6_WPORT   PORTC
    #define PC6_PWM     NULL
    #define PC6_DDR     DDRC
    #undef PC7
    #define PC7_PIN     PINC7
    #define PC7_RPORT   PINC
    #define PC7_WPORT   PORTC
    #define PC7_PWM     NULL
    #define PC7_DDR     DDRC

    #undef PD0
    #define PD0_PIN     PIND0
    #define PD0_RPORT   PIND
    #define PD0_WPORT   PORTD
    #define PD0_PWM     NULL
    #define PD0_DDR     DDRD
    #undef PD1
    #define PD1_PIN     PIND1
    #define PD1_RPORT   PIND
    #define PD1_WPORT   PORTD
    #define PD1_PWM     NULL
    #define PD1_DDR     DDRD
    #undef PD2
    #define PD2_PIN     PIND2
    #define PD2_RPORT   PIND
    #define PD2_WPORT   PORTD
    #define PD2_PWM     NULL
    #define PD2_DDR     DDRD
    #undef PD3
    #define PD3_PIN     PIND3
    #define PD3_RPORT   PIND
    #define PD3_WPORT   PORTD
    #define PD3_PWM     NULL
    #define PD3_DDR     DDRD
    #undef PD4
    #define PD4_PIN     PIND4
    #define PD4_RPORT   PIND
    #define PD4_WPORT   PORTD
    #define PD4_PWM     NULL
    #define PD4_DDR     DDRD
    #undef PD5
    #define PD5_PIN     PIND5
    #define PD5_RPORT   PIND
    #define PD5_WPORT   PORTD
    #define PD5_PWM     NULL
    #define PD5_DDR     DDRD
    #undef PD6
    #define PD6_PIN     PIND6
    #define PD6_RPORT   PIND
    #define PD6_WPORT   PORTD
    #define PD6_PWM     NULL
    #define PD6_DDR     DDRD
    #undef PD7
    #define PD7_PIN     PIND7
    #define PD7_RPORT   PIND
    #define PD7_WPORT   PORTD
    #define PD7_PWM     NULL
    #define PD7_DDR     DDRD

    #undef PE0
    #define PE0_PIN     PINE0
    #define PE0_RPORT   PINE
    #define PE0_WPORT   PORTE
    #define PE0_PWM     NULL
    #define PE0_DDR     DDRE
    #undef PE1
    #define PE1_PIN     PINE1
    #define PE1_RPORT   PINE
    #define PE1_WPORT   PORTE
    #define PE1_PWM     NULL
    #define PE1_DDR     DDRE
    #undef PE2
    #define PE2_PIN     PINE2
    #define PE2_RPORT   PINE
    #define PE2_WPORT   PORTE
    #define PE2_PWM     NULL
    #define PE2_DDR     DDRE
    #undef PE3
    #define PE3_PIN     PINE3
    #define PE3_RPORT   PINE
    #define PE3_WPORT   PORTE
    #define PE3_PWM     NULL
    #define PE3_DDR     DDRE
    #undef PE4
    #define PE4_PIN     PINE4
    #define PE4_RPORT   PINE
    #define PE4_WPORT   PORTE
    #define PE4_PWM     NULL
    #define PE4_DDR     DDRE
    #undef PE5
    #define PE5_PIN     PINE5
    #define PE5_RPORT   PINE
    #define PE5_WPORT   PORTE
    #define PE5_PWM     NULL
    #define PE5_DDR     DDRE
    #undef PE6
    #define PE6_PIN     PINE6
    #define PE6_RPORT   PINE
    #define PE6_WPORT   PORTE
    #define PE6_PWM     NULL
    #define PE6_DDR     DDRE
    #undef PE7
    #define PE7_PIN     PINE7
    #define PE7_RPORT   PINE
    #define PE7_WPORT   PORTE
    #define PE7_PWM     NULL
    #define PE7_DDR     DDRE

    #undef PF0
    #define PF0_PIN     PINF0
    #define PF0_RPORT   PINF
    #define PF0_WPORT   PORTF
    #define PF0_PWM     NULL
    #define PF0_DDR     DDRF
    #undef PF1
    #define PF1_PIN     PINF1
    #define PF1_RPORT   PINF
    #define PF1_WPORT   PORTF
    #define PF1_PWM     NULL
    #define PF1_DDR     DDRF
    #undef PF2
    #define PF2_PIN     PINF2
    #define PF2_RPORT   PINF
    #define PF2_WPORT   PORTF
    #define PF2_PWM     NULL
    #define PF2_DDR     DDRF
    #undef PF3
    #define PF3_PIN     PINF3
    #define PF3_RPORT   PINF
    #define PF3_WPORT   PORTF
    #define PF3_PWM     NULL
    #define PF3_DDR     DDRF
    #undef PF4
    #define PF4_PIN     PINF4
    #define PF4_RPORT   PINF
    #define PF4_WPORT   PORTF
    #define PF4_PWM     NULL
    #define PF4_DDR     DDRF
    #undef PF5
    #define PF5_PIN     PINF5
    #define PF5_RPORT   PINF
    #define PF5_WPORT   PORTF
    #define PF5_PWM     NULL
    #define PF5_DDR     DDRF
    #undef PF6
    #define PF6_PIN     PINF6
    #define PF6_RPORT   PINF
    #define PF6_WPORT   PORTF
    #define PF6_PWM     NULL
    #define PF6_DDR     DDRF
    #undef PF7
    #define PF7_PIN     PINF7
    #define PF7_RPORT   PINF
    #define PF7_WPORT   PORTF
    #define PF7_PWM     NULL
    #define PF7_DDR     DDRF

  #else // AT90USBxx_TEENSYPP_ASSIGNMENTS -- Use Teensyduino Teensy++2.0 assignments.

    /**

    AT90USB  51 50 49 48 47 46 45 44 10 11 12 13 14 15 16 17 35 36 37 38 39 40 41 42 25 26 27 28 29 30 31 32 33 34 43 09 18 19 01 02 61 60 59 58 57 56 55 54
    Port     A0 A1 A2 A3 A4 A5 A6 A7 B0 B1 B2 B3 B4 B5 B6 B7 C0 C1 C2 C3 C4 C5 C6 C7 D0 D1 D2 D3 D4 D5 D6 D7 E0 E1 E2 E3 E4 E5 E6 E7 F0 F1 F2 F3 F4 F5 F6 F7
    Marlin   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47
    Teensy   28 29 30 31 32 33 34 35 20 21 22 23 24 25 26 27 10 11 12 13 14 15 16 17 00 01 02 03 04 05 06 07 08 09(46*47)36 37 18 19 38 39 40 41 42 43 44 45
             The pins 46 and 47 are not supported by Teensyduino, but are supported below.
    */

    // SPI
    #define SCK         DIO21 //  9
    #define MISO        DIO23 // 11
    #define MOSI        DIO22 // 10
    #define SS          DIO20 //  8

    #define DIO0_PIN    PIND0
    #define DIO0_RPORT  PIND
    #define DIO0_WPORT  PORTD
    #define DIO0_PWM    NULL
    #define DIO0_DDR    DDRD

    #define DIO1_PIN    PIND1
    #define DIO1_RPORT  PIND
    #define DIO1_WPORT  PORTD
    #define DIO1_PWM    NULL
    #define DIO1_DDR    DDRD

    #define DIO2_PIN    PIND2
    #define DIO2_RPORT  PIND
    #define DIO2_WPORT  PORTD
    #define DIO2_PWM    NULL
    #define DIO2_DDR    DDRD

    #define DIO3_PIN    PIND3
    #define DIO3_RPORT  PIND
    #define DIO3_WPORT  PORTD
    #define DIO3_PWM    NULL
    #define DIO3_DDR    DDRD

    #define DIO4_PIN    PIND4
    #define DIO4_RPORT  PIND
    #define DIO4_WPORT  PORTD
    #define DIO4_PWM    NULL
    #define DIO4_DDR    DDRD

    #define DIO5_PIN    PIND5
    #define DIO5_RPORT  PIND
    #define DIO5_WPORT  PORTD
    #define DIO5_PWM    NULL
    #define DIO5_DDR    DDRD

    #define DIO6_PIN    PIND6
    #define DIO6_RPORT  PIND
    #define DIO6_WPORT  PORTD
    #define DIO6_PWM    NULL
    #define DIO6_DDR    DDRD

    #define DIO7_PIN    PIND7
    #define DIO7_RPORT  PIND
    #define DIO7_WPORT  PORTD
    #define DIO7_PWM    NULL
    #define DIO7_DDR    DDRD

    #define DIO8_PIN    PINE0
    #define DIO8_RPORT  PINE
    #define DIO8_WPORT  PORTE
    #define DIO8_PWM    NULL
    #define DIO8_DDR    DDRE

    #define DIO9_PIN    PINE1
    #define DIO9_RPORT  PINE
    #define DIO9_WPORT  PORTE
    #define DIO9_PWM    NULL
    #define DIO9_DDR    DDRE

    #define DIO10_PIN   PINC0
    #define DIO10_RPORT PINC
    #define DIO10_WPORT PORTC
    #define DIO10_PWM   NULL
    #define DIO10_DDR   DDRC

    #define DIO11_PIN   PINC1
    #define DIO11_RPORT PINC
    #define DIO11_WPORT PORTC
    #define DIO11_PWM   NULL
    #define DIO11_DDR   DDRC

    #define DIO12_PIN   PINC2
    #define DIO12_RPORT PINC
    #define DIO12_WPORT PORTC
    #define DIO12_PWM   NULL
    #define DIO12_DDR   DDRC

    #define DIO13_PIN   PINC3
    #define DIO13_RPORT PINC
    #define DIO13_WPORT PORTC
    #define DIO13_PWM   NULL
    #define DIO13_DDR   DDRC

    #define DIO14_PIN   PINC4
    #define DIO14_RPORT PINC
    #define DIO14_WPORT PORTC
    #define DIO14_PWM   NULL
    #define DIO14_DDR   DDRC

    #define DIO15_PIN   PINC5
    #define DIO15_RPORT PINC
    #define DIO15_WPORT PORTC
    #define DIO15_PWM   NULL
    #define DIO15_DDR   DDRC

    #define DIO16_PIN   PINC6
    #define DIO16_RPORT PINC
    #define DIO16_WPORT PORTC
    #define DIO16_PWM   NULL
    #define DIO16_DDR   DDRC

    #define DIO17_PIN   PINC7
    #define DIO17_RPORT PINC
    #define DIO17_WPORT PORTC
    #define DIO17_PWM   NULL
    #define DIO17_DDR   DDRC

    #define DIO18_PIN   PINE6
    #define DIO18_RPORT PINE
    #define DIO18_WPORT PORTE
    #define DIO18_PWM   NULL
    #define DIO18_DDR   DDRE

    #define DIO19_PIN   PINE7
    #define DIO19_RPORT PINE
    #define DIO19_WPORT PORTE
    #define DIO19_PWM   NULL
    #define DIO19_DDR   DDRE

    #define DIO20_PIN   PINB0
    #define DIO20_RPORT PINB
    #define DIO20_WPORT PORTB
    #define DIO20_PWM   NULL
    #define DIO20_DDR   DDRB

    #define DIO21_PIN   PINB1
    #define DIO21_RPORT PINB
    #define DIO21_WPORT PORTB
    #define DIO21_PWM   NULL
    #define DIO21_DDR   DDRB

    #define DIO22_PIN   PINB2
    #define DIO22_RPORT PINB
    #define DIO22_WPORT PORTB
    #define DIO22_PWM   NULL
    #define DIO22_DDR   DDRB

    #define DIO23_PIN   PINB3
    #define DIO23_RPORT PINB
    #define DIO23_WPORT PORTB
    #define DIO23_PWM   NULL
    #define DIO23_DDR   DDRB

    #define DIO24_PIN   PINB4
    #define DIO24_RPORT PINB
    #define DIO24_WPORT PORTB
    #define DIO24_PWM   NULL
    #define DIO24_DDR   DDRB

    #define DIO25_PIN   PINB5
    #define DIO25_RPORT PINB
    #define DIO25_WPORT PORTB
    #define DIO25_PWM   NULL
    #define DIO25_DDR   DDRB

    #define DIO26_PIN   PINB6
    #define DIO26_RPORT PINB
    #define DIO26_WPORT PORTB
    #define DIO26_PWM   NULL
    #define DIO26_DDR   DDRB

    #define DIO27_PIN   PINB7
    #define DIO27_RPORT PINB
    #define DIO27_WPORT PORTB
    #define DIO27_PWM   NULL
    #define DIO27_DDR   DDRB

    #define DIO28_PIN   PINA0
    #define DIO28_RPORT PINA
    #define DIO28_WPORT PORTA
    #define DIO28_PWM   NULL
    #define DIO28_DDR   DDRA

    #define DIO29_PIN   PINA1
    #define DIO29_RPORT PINA
    #define DIO29_WPORT PORTA
    #define DIO29_PWM   NULL
    #define DIO29_DDR   DDRA

    #define DIO30_PIN   PINA2
    #define DIO30_RPORT PINA
    #define DIO30_WPORT PORTA
    #define DIO30_PWM   NULL
    #define DIO30_DDR   DDRA

    #define DIO31_PIN   PINA3
    #define DIO31_RPORT PINA
    #define DIO31_WPORT PORTA
    #define DIO31_PWM   NULL
    #define DIO31_DDR   DDRA

    #define DIO32_PIN   PINA4
    #define DIO32_RPORT PINA
    #define DIO32_WPORT PORTA
    #define DIO32_PWM   NULL
    #define DIO32_DDR   DDRA

    #define DIO33_PIN   PINA5
    #define DIO33_RPORT PINA
    #define DIO33_WPORT PORTA
    #define DIO33_PWM   NULL
    #define DIO33_DDR   DDRA

    #define DIO34_PIN   PINA6
    #define DIO34_RPORT PINA
    #define DIO34_WPORT PORTA
    #define DIO34_PWM   NULL
    #define DIO34_DDR   DDRA

    #define DIO35_PIN   PINA7
    #define DIO35_RPORT PINA
    #define DIO35_WPORT PORTA
    #define DIO35_PWM   NULL
    #define DIO35_DDR   DDRA

    #define DIO36_PIN   PINE4
    #define DIO36_RPORT PINE
    #define DIO36_WPORT PORTE
    #define DIO36_PWM   NULL
    #define DIO36_DDR   DDRE

    #define DIO37_PIN   PINE5
    #define DIO37_RPORT PINE
    #define DIO37_WPORT PORTE
    #define DIO37_PWM   NULL
    #define DIO37_DDR   DDRE

    #define DIO38_PIN   PINF0
    #define DIO38_RPORT PINF
    #define DIO38_WPORT PORTF
    #define DIO38_PWM   NULL
    #define DIO38_DDR   DDRF

    #define DIO39_PIN   PINF1
    #define DIO39_RPORT PINF
    #define DIO39_WPORT PORTF
    #define DIO39_PWM   NULL
    #define DIO39_DDR   DDRF

    #define DIO40_PIN   PINF2
    #define DIO40_RPORT PINF
    #define DIO40_WPORT PORTF
    #define DIO40_PWM   NULL
    #define DIO40_DDR   DDRF

    #define DIO41_PIN   PINF3
    #define DIO41_RPORT PINF
    #define DIO41_WPORT PORTF
    #define DIO41_PWM   NULL
    #define DIO41_DDR   DDRF

    #define DIO42_PIN   PINF4
    #define DIO42_RPORT PINF
    #define DIO42_WPORT PORTF
    #define DIO42_PWM   NULL
    #define DIO42_DDR   DDRF

    #define DIO43_PIN   PINF5
    #define DIO43_RPORT PINF
    #define DIO43_WPORT PORTF
    #define DIO43_PWM   NULL
    #define DIO43_DDR   DDRF

    #define DIO44_PIN   PINF6
    #define DIO44_RPORT PINF
    #define DIO44_WPORT PORTF
    #define DIO44_PWM   NULL
    #define DIO44_DDR   DDRF

    #define DIO45_PIN   PINF7
    #define DIO45_RPORT PINF
    #define DIO45_WPORT PORTF
    #define DIO45_PWM   NULL
    #define DIO45_DDR   DDRF

    #define AIO0_PIN    PINF0
    #define AIO0_RPORT  PINF
    #define AIO0_WPORT  PORTF
    #define AIO0_PWM    NULL
    #define AIO0_DDR    DDRF

    #define AIO1_PIN    PINF1
    #define AIO1_RPORT  PINF
    #define AIO1_WPORT  PORTF
    #define AIO1_PWM    NULL
    #define AIO1_DDR    DDRF

    #define AIO2_PIN    PINF2
    #define AIO2_RPORT  PINF
    #define AIO2_WPORT  PORTF
    #define AIO2_PWM    NULL
    #define AIO2_DDR    DDRF

    #define AIO3_PIN    PINF3
    #define AIO3_RPORT  PINF
    #define AIO3_WPORT  PORTF
    #define AIO3_PWM    NULL
    #define AIO3_DDR    DDRF

    #define AIO4_PIN    PINF4
    #define AIO4_RPORT  PINF
    #define AIO4_WPORT  PORTF
    #define AIO4_PWM    NULL
    #define AIO4_DDR    DDRF

    #define AIO5_PIN    PINF5
    #define AIO5_RPORT  PINF
    #define AIO5_WPORT  PORTF
    #define AIO5_PWM    NULL
    #define AIO5_DDR    DDRF

    #define AIO6_PIN    PINF6
    #define AIO6_RPORT  PINF
    #define AIO6_WPORT  PORTF
    #define AIO6_PWM    NULL
    #define AIO6_DDR    DDRF

    #define AIO7_PIN    PINF7
    #define AIO7_RPORT  PINF
    #define AIO7_WPORT  PORTF
    #define AIO7_PWM    NULL
    #define AIO7_DDR    DDRF

    //-- Begin not supported by Teensyduino
    //-- don't use Arduino functions on these pins pinMode/digitalWrite/etc
    #define DIO46_PIN   PINE2
    #define DIO46_RPORT PINE
    #define DIO46_WPORT PORTE
    #define DIO46_PWM   NULL
    #define DIO46_DDR   DDRE

    #define DIO47_PIN   PINE3
    #define DIO47_RPORT PINE
    #define DIO47_WPORT PORTE
    #define DIO47_PWM   NULL
    #define DIO47_DDR   DDRE
    //-- end not supported by Teensyduino

    #undef PA0
    #define PA0_PIN     PINA0
    #define PA0_RPORT   PINA
    #define PA0_WPORT   PORTA
    #define PA0_PWM     NULL
    #define PA0_DDR     DDRA
    #undef PA1
    #define PA1_PIN     PINA1
    #define PA1_RPORT   PINA
    #define PA1_WPORT   PORTA
    #define PA1_PWM     NULL
    #define PA1_DDR     DDRA
    #undef PA2
    #define PA2_PIN     PINA2
    #define PA2_RPORT   PINA
    #define PA2_WPORT   PORTA
    #define PA2_PWM     NULL
    #define PA2_DDR     DDRA
    #undef PA3
    #define PA3_PIN     PINA3
    #define PA3_RPORT   PINA
    #define PA3_WPORT   PORTA
    #define PA3_PWM     NULL
    #define PA3_DDR     DDRA
    #undef PA4
    #define PA4_PIN     PINA4
    #define PA4_RPORT   PINA
    #define PA4_WPORT   PORTA
    #define PA4_PWM     NULL
    #define PA4_DDR     DDRA
    #undef PA5
    #define PA5_PIN     PINA5
    #define PA5_RPORT   PINA
    #define PA5_WPORT   PORTA
    #define PA5_PWM     NULL
    #define PA5_DDR     DDRA
    #undef PA6
    #define PA6_PIN     PINA6
    #define PA6_RPORT   PINA
    #define PA6_WPORT   PORTA
    #define PA6_PWM     NULL
    #define PA6_DDR     DDRA
    #undef PA7
    #define PA7_PIN     PINA7
    #define PA7_RPORT   PINA
    #define PA7_WPORT   PORTA
    #define PA7_PWM     NULL
    #define PA7_DDR     DDRA

    #undef PB0
    #define PB0_PIN     PINB0
    #define PB0_RPORT   PINB
    #define PB0_WPORT   PORTB
    #define PB0_PWM     NULL
    #define PB0_DDR     DDRB
    #undef PB1
    #define PB1_PIN     PINB1
    #define PB1_RPORT   PINB
    #define PB1_WPORT   PORTB
    #define PB1_PWM     NULL
    #define PB1_DDR     DDRB
    #undef PB2
    #define PB2_PIN     PINB2
    #define PB2_RPORT   PINB
    #define PB2_WPORT   PORTB
    #define PB2_PWM     NULL
    #define PB2_DDR     DDRB
    #undef PB3
    #define PB3_PIN     PINB3
    #define PB3_RPORT   PINB
    #define PB3_WPORT   PORTB
    #define PB3_PWM     NULL
    #define PB3_DDR     DDRB
    #undef PB4
    #define PB4_PIN     PINB4
    #define PB4_RPORT   PINB
    #define PB4_WPORT   PORTB
    #define PB4_PWM     NULL
    #define PB4_DDR     DDRB
    #undef PB5
    #define PB5_PIN     PINB5
    #define PB5_RPORT   PINB
    #define PB5_WPORT   PORTB
    #define PB5_PWM     NULL
    #define PB5_DDR     DDRB
    #undef PB6
    #define PB6_PIN     PINB6
    #define PB6_RPORT   PINB
    #define PB6_WPORT   PORTB
    #define PB6_PWM     NULL
    #define PB6_DDR     DDRB
    #undef PB7
    #define PB7_PIN     PINB7
    #define PB7_RPORT   PINB
    #define PB7_WPORT   PORTB
    #define PB7_PWM     NULL
    #define PB7_DDR     DDRB

    #undef PC0
    #define PC0_PIN     PINC0
    #define PC0_RPORT   PINC
    #define PC0_WPORT   PORTC
    #define PC0_PWM     NULL
    #define PC0_DDR     DDRC
    #undef PC1
    #define PC1_PIN     PINC1
    #define PC1_RPORT   PINC
    #define PC1_WPORT   PORTC
    #define PC1_PWM     NULL
    #define PC1_DDR     DDRC
    #undef PC2
    #define PC2_PIN     PINC2
    #define PC2_RPORT   PINC
    #define PC2_WPORT   PORTC
    #define PC2_PWM     NULL
    #define PC2_DDR     DDRC
    #undef PC3
    #define PC3_PIN     PINC3
    #define PC3_RPORT   PINC
    #define PC3_WPORT   PORTC
    #define PC3_PWM     NULL
    #define PC3_DDR     DDRC
    #undef PC4
    #define PC4_PIN     PINC4
    #define PC4_RPORT   PINC
    #define PC4_WPORT   PORTC
    #define PC4_PWM     NULL
    #define PC4_DDR     DDRC
    #undef PC5
    #define PC5_PIN     PINC5
    #define PC5_RPORT   PINC
    #define PC5_WPORT   PORTC
    #define PC5_PWM     NULL
    #define PC5_DDR     DDRC
    #undef PC6
    #define PC6_PIN     PINC6
    #define PC6_RPORT   PINC
    #define PC6_WPORT   PORTC
    #define PC6_PWM     NULL
    #define PC6_DDR     DDRC
    #undef PC7
    #define PC7_PIN     PINC7
    #define PC7_RPORT   PINC
    #define PC7_WPORT   PORTC
    #define PC7_PWM     NULL
    #define PC7_DDR     DDRC

    #undef PD0
    #define PD0_PIN     PIND0
    #define PD0_RPORT   PIND
    #define PD0_WPORT   PORTD
    #define PD0_PWM     NULL
    #define PD0_DDR     DDRD
    #undef PD1
    #define PD1_PIN     PIND1
    #define PD1_RPORT   PIND
    #define PD1_WPORT   PORTD
    #define PD1_PWM     NULL
    #define PD1_DDR     DDRD
    #undef PD2
    #define PD2_PIN     PIND2
    #define PD2_RPORT   PIND
    #define PD2_WPORT   PORTD
    #define PD2_PWM     NULL
    #define PD2_DDR     DDRD
    #undef PD3
    #define PD3_PIN     PIND3
    #define PD3_RPORT   PIND
    #define PD3_WPORT   PORTD
    #define PD3_PWM     NULL
    #define PD3_DDR     DDRD
    #undef PD4
    #define PD4_PIN     PIND4
    #define PD4_RPORT   PIND
    #define PD4_WPORT   PORTD
    #define PD4_PWM     NULL
    #define PD4_DDR     DDRD
    #undef PD5
    #define PD5_PIN     PIND5
    #define PD5_RPORT   PIND
    #define PD5_WPORT   PORTD
    #define PD5_PWM     NULL
    #define PD5_DDR     DDRD
    #undef PD6
    #define PD6_PIN     PIND6
    #define PD6_RPORT   PIND
    #define PD6_WPORT   PORTD
    #define PD6_PWM     NULL
    #define PD6_DDR     DDRD
    #undef PD7
    #define PD7_PIN     PIND7
    #define PD7_RPORT   PIND
    #define PD7_WPORT   PORTD
    #define PD7_PWM     NULL
    #define PD7_DDR     DDRD

    #undef PE0
    #define PE0_PIN     PINE0
    #define PE0_RPORT   PINE
    #define PE0_WPORT   PORTE
    #define PE0_PWM     NULL
    #define PE0_DDR     DDRE
    #undef PE1
    #define PE1_PIN     PINE1
    #define PE1_RPORT   PINE
    #define PE1_WPORT   PORTE
    #define PE1_PWM     NULL
    #define PE1_DDR     DDRE
    #undef PE2
    #define PE2_PIN     PINE2
    #define PE2_RPORT   PINE
    #define PE2_WPORT   PORTE
    #define PE2_PWM     NULL
    #define PE2_DDR     DDRE
    #undef PE3
    #define PE3_PIN     PINE3
    #define PE3_RPORT   PINE
    #define PE3_WPORT   PORTE
    #define PE3_PWM     NULL
    #define PE3_DDR     DDRE
    #undef PE4
    #define PE4_PIN     PINE4
    #define PE4_RPORT   PINE
    #define PE4_WPORT   PORTE
    #define PE4_PWM     NULL
    #define PE4_DDR     DDRE
    #undef PE5
    #define PE5_PIN     PINE5
    #define PE5_RPORT   PINE
    #define PE5_WPORT   PORTE
    #define PE5_PWM     NULL
    #define PE5_DDR     DDRE
    #undef PE6
    #define PE6_PIN     PINE6
    #define PE6_RPORT   PINE
    #define PE6_WPORT   PORTE
    #define PE6_PWM     NULL
    #define PE6_DDR     DDRE
    #undef PE7
    #define PE7_PIN     PINE7
    #define PE7_RPORT   PINE
    #define PE7_WPORT   PORTE
    #define PE7_PWM     NULL
    #define PE7_DDR     DDRE

    #undef PF0
    #define PF0_PIN     PINF0
    #define PF0_RPORT   PINF
    #define PF0_WPORT   PORTF
    #define PF0_PWM     NULL
    #define PF0_DDR     DDRF
    #undef PF1
    #define PF1_PIN     PINF1
    #define PF1_RPORT   PINF
    #define PF1_WPORT   PORTF
    #define PF1_PWM     NULL
    #define PF1_DDR     DDRF
    #undef PF2
    #define PF2_PIN     PINF2
    #define PF2_RPORT   PINF
    #define PF2_WPORT   PORTF
    #define PF2_PWM     NULL
    #define PF2_DDR     DDRF
    #undef PF3
    #define PF3_PIN     PINF3
    #define PF3_RPORT   PINF
    #define PF3_WPORT   PORTF
    #define PF3_PWM     NULL
    #define PF3_DDR     DDRF
    #undef PF4
    #define PF4_PIN     PINF4
    #define PF4_RPORT   PINF
    #define PF4_WPORT   PORTF
    #define PF4_PWM     NULL
    #define PF4_DDR     DDRF
    #undef PF5
    #define PF5_PIN     PINF5
    #define PF5_RPORT   PINF
    #define PF5_WPORT   PORTF
    #define PF5_PWM     NULL
    #define PF5_DDR     DDRF
    #undef PF6
    #define PF6_PIN     PINF6
    #define PF6_RPORT   PINF
    #define PF6_WPORT   PORTF
    #define PF6_PWM     NULL
    #define PF6_DDR     DDRF
    #undef PF7
    #define PF7_PIN     PINF7
    #define PF7_RPORT   PINF
    #define PF7_WPORT   PORTF
    #define PF7_PWM     NULL
    #define PF7_DDR     DDRF

  #endif // AT90USBxx_TEENSYPP_ASSIGNMENTS Teensyduino assignments
#endif // __AVR_AT90usbxxx__


#if defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__)
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

  // timers and PWM
  #define OC0A        DIO9
  #define OC0B        DIO4
  #define OC1A        DIO7
  #define OC1B        DIO8
  #define OC2A        DIO6
  #define OC3A        DIO5
  #define OC3B        DIO2
  #define OC3C        DIO3


  // change for your board
  #define DEBUG_LED   DIO46

  /**
  pins
  */
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


#endif

#ifndef DIO0_PIN
  #error "pins for this chip not defined in arduino.h! If you write an appropriate pin definition and have this firmware work on your chip, please submit a pull request"
#endif

#endif /* _FASTIO_ARDUINO_H */
