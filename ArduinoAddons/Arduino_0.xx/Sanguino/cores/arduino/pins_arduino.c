/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Changelog
  -----------
  11/25/11  - ryan@ryanmsutton.com - Add pins for Sanguino 644P and 1284P
  $Id$
*/

#include <avr/io.h>
#include "wiring_private.h"
#include "pins_arduino.h"

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)

// ATMEL ATMEGA1280 / ARDUINO
//
// 0-7 PE0-PE7   works
// 8-13 PB0-PB5  works
// 14-21 PA0-PA7 works 
// 22-29 PH0-PH7 works
// 30-35 PG5-PG0 works
// 36-43 PC7-PC0 works
// 44-51 PJ7-PJ0 works
// 52-59 PL7-PL0 works
// 60-67 PD7-PD0 works
// A0-A7 PF0-PF7
// A8-A15 PK0-PK7
// ATMEL ATMEGA644P / SANGUINO
//
//                   +---\/---+
//  INT0 (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
//  INT1 (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
//  INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
//   PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
//   PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
//  MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
//  MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
//   SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
//             RST  9|        |32  AREF
//             VCC 10|        |31  GND 
//             GND 11|        |30  AVCC
//           XTAL2 12|        |29  PC7 (D 23)
//           XTAL1 13|        |28  PC6 (D 22)
//  RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
//  TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
//  RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
//  TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
//  PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
//  PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
//  PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
//                   +--------+
//
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12


#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	&DDRA,
	&DDRB,
	&DDRC,
	&DDRD,
	&DDRE,
	&DDRF,
	&DDRG,
	&DDRH,
	NOT_A_PORT,
	&DDRJ,
	&DDRK,
	&DDRL,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	&PORTA,
	&PORTB,
	&PORTC,
	&PORTD,
	&PORTE,
	&PORTF,
	&PORTG,
	&PORTH,
	NOT_A_PORT,
	&PORTJ,
	&PORTK,
	&PORTL,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	&PINA,
	&PINB,
	&PINC,
	&PIND,
	&PINE,
	&PINF,
	&PING,
	&PINH,
	NOT_A_PIN,
	&PINJ,
	&PINK,
	&PINL,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	// PORTLIST		
	// -------------------------------------------		
	PE	, // PE 0 ** 0 ** USART0_RX	
	PE	, // PE 1 ** 1 ** USART0_TX	
	PE	, // PE 4 ** 2 ** PWM2	
	PE	, // PE 5 ** 3 ** PWM3	
	PG	, // PG 5 ** 4 ** PWM4	
	PE	, // PE 3 ** 5 ** PWM5	
	PH	, // PH 3 ** 6 ** PWM6	
	PH	, // PH 4 ** 7 ** PWM7	
	PH	, // PH 5 ** 8 ** PWM8	
	PH	, // PH 6 ** 9 ** PWM9	
	PB	, // PB 4 ** 10 ** PWM10	
	PB	, // PB 5 ** 11 ** PWM11	
	PB	, // PB 6 ** 12 ** PWM12	
	PB	, // PB 7 ** 13 ** PWM13	
	PJ	, // PJ 1 ** 14 ** USART3_TX	
	PJ	, // PJ 0 ** 15 ** USART3_RX	
	PH	, // PH 1 ** 16 ** USART2_TX	
	PH	, // PH 0 ** 17 ** USART2_RX	
	PD	, // PD 3 ** 18 ** USART1_TX	
	PD	, // PD 2 ** 19 ** USART1_RX	
	PD	, // PD 1 ** 20 ** I2C_SDA	
	PD	, // PD 0 ** 21 ** I2C_SCL	
	PA	, // PA 0 ** 22 ** D22	
	PA	, // PA 1 ** 23 ** D23	
	PA	, // PA 2 ** 24 ** D24	
	PA	, // PA 3 ** 25 ** D25	
	PA	, // PA 4 ** 26 ** D26	
	PA	, // PA 5 ** 27 ** D27	
	PA	, // PA 6 ** 28 ** D28	
	PA	, // PA 7 ** 29 ** D29	
	PC	, // PC 7 ** 30 ** D30	
	PC	, // PC 6 ** 31 ** D31	
	PC	, // PC 5 ** 32 ** D32	
	PC	, // PC 4 ** 33 ** D33	
	PC	, // PC 3 ** 34 ** D34	
	PC	, // PC 2 ** 35 ** D35	
	PC	, // PC 1 ** 36 ** D36	
	PC	, // PC 0 ** 37 ** D37	
	PD	, // PD 7 ** 38 ** D38	
	PG	, // PG 2 ** 39 ** D39	
	PG	, // PG 1 ** 40 ** D40	
	PG	, // PG 0 ** 41 ** D41	
	PL	, // PL 7 ** 42 ** D42	
	PL	, // PL 6 ** 43 ** D43	
	PL	, // PL 5 ** 44 ** D44	
	PL	, // PL 4 ** 45 ** D45	
	PL	, // PL 3 ** 46 ** D46	
	PL	, // PL 2 ** 47 ** D47	
	PL	, // PL 1 ** 48 ** D48	
	PL	, // PL 0 ** 49 ** D49	
	PB	, // PB 3 ** 50 ** SPI_MISO	
	PB	, // PB 2 ** 51 ** SPI_MOSI	
	PB	, // PB 1 ** 52 ** SPI_SCK	
	PB	, // PB 0 ** 53 ** SPI_SS	
	PF	, // PF 0 ** 54 ** A0	
	PF	, // PF 1 ** 55 ** A1	
	PF	, // PF 2 ** 56 ** A2	
	PF	, // PF 3 ** 57 ** A3	
	PF	, // PF 4 ** 58 ** A4	
	PF	, // PF 5 ** 59 ** A5	
	PF	, // PF 6 ** 60 ** A6	
	PF	, // PF 7 ** 61 ** A7	
	PK	, // PK 0 ** 62 ** A8	
	PK	, // PK 1 ** 63 ** A9	
	PK	, // PK 2 ** 64 ** A10	
	PK	, // PK 3 ** 65 ** A11	
	PK	, // PK 4 ** 66 ** A12	
	PK	, // PK 5 ** 67 ** A13	
	PK	, // PK 6 ** 68 ** A14	
	PK	, // PK 7 ** 69 ** A15	
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 0 )	, // PE 0 ** 0 ** USART0_RX	
	_BV( 1 )	, // PE 1 ** 1 ** USART0_TX	
	_BV( 4 )	, // PE 4 ** 2 ** PWM2	
	_BV( 5 )	, // PE 5 ** 3 ** PWM3	
	_BV( 5 )	, // PG 5 ** 4 ** PWM4	
	_BV( 3 )	, // PE 3 ** 5 ** PWM5	
	_BV( 3 )	, // PH 3 ** 6 ** PWM6	
	_BV( 4 )	, // PH 4 ** 7 ** PWM7	
	_BV( 5 )	, // PH 5 ** 8 ** PWM8	
	_BV( 6 )	, // PH 6 ** 9 ** PWM9	
	_BV( 4 )	, // PB 4 ** 10 ** PWM10	
	_BV( 5 )	, // PB 5 ** 11 ** PWM11	
	_BV( 6 )	, // PB 6 ** 12 ** PWM12	
	_BV( 7 )	, // PB 7 ** 13 ** PWM13	
	_BV( 1 )	, // PJ 1 ** 14 ** USART3_TX	
	_BV( 0 )	, // PJ 0 ** 15 ** USART3_RX	
	_BV( 1 )	, // PH 1 ** 16 ** USART2_TX	
	_BV( 0 )	, // PH 0 ** 17 ** USART2_RX	
	_BV( 3 )	, // PD 3 ** 18 ** USART1_TX	
	_BV( 2 )	, // PD 2 ** 19 ** USART1_RX	
	_BV( 1 )	, // PD 1 ** 20 ** I2C_SDA	
	_BV( 0 )	, // PD 0 ** 21 ** I2C_SCL	
	_BV( 0 )	, // PA 0 ** 22 ** D22	
	_BV( 1 )	, // PA 1 ** 23 ** D23	
	_BV( 2 )	, // PA 2 ** 24 ** D24	
	_BV( 3 )	, // PA 3 ** 25 ** D25	
	_BV( 4 )	, // PA 4 ** 26 ** D26	
	_BV( 5 )	, // PA 5 ** 27 ** D27	
	_BV( 6 )	, // PA 6 ** 28 ** D28	
	_BV( 7 )	, // PA 7 ** 29 ** D29	
	_BV( 7 )	, // PC 7 ** 30 ** D30	
	_BV( 6 )	, // PC 6 ** 31 ** D31	
	_BV( 5 )	, // PC 5 ** 32 ** D32	
	_BV( 4 )	, // PC 4 ** 33 ** D33	
	_BV( 3 )	, // PC 3 ** 34 ** D34	
	_BV( 2 )	, // PC 2 ** 35 ** D35	
	_BV( 1 )	, // PC 1 ** 36 ** D36	
	_BV( 0 )	, // PC 0 ** 37 ** D37	
	_BV( 7 )	, // PD 7 ** 38 ** D38	
	_BV( 2 )	, // PG 2 ** 39 ** D39	
	_BV( 1 )	, // PG 1 ** 40 ** D40	
	_BV( 0 )	, // PG 0 ** 41 ** D41	
	_BV( 7 )	, // PL 7 ** 42 ** D42	
	_BV( 6 )	, // PL 6 ** 43 ** D43	
	_BV( 5 )	, // PL 5 ** 44 ** D44	
	_BV( 4 )	, // PL 4 ** 45 ** D45	
	_BV( 3 )	, // PL 3 ** 46 ** D46	
	_BV( 2 )	, // PL 2 ** 47 ** D47	
	_BV( 1 )	, // PL 1 ** 48 ** D48	
	_BV( 0 )	, // PL 0 ** 49 ** D49	
	_BV( 3 )	, // PB 3 ** 50 ** SPI_MISO	
	_BV( 2 )	, // PB 2 ** 51 ** SPI_MOSI	
	_BV( 1 )	, // PB 1 ** 52 ** SPI_SCK	
	_BV( 0 )	, // PB 0 ** 53 ** SPI_SS	
	_BV( 0 )	, // PF 0 ** 54 ** A0	
	_BV( 1 )	, // PF 1 ** 55 ** A1	
	_BV( 2 )	, // PF 2 ** 56 ** A2	
	_BV( 3 )	, // PF 3 ** 57 ** A3	
	_BV( 4 )	, // PF 4 ** 58 ** A4	
	_BV( 5 )	, // PF 5 ** 59 ** A5	
	_BV( 6 )	, // PF 6 ** 60 ** A6	
	_BV( 7 )	, // PF 7 ** 61 ** A7	
	_BV( 0 )	, // PK 0 ** 62 ** A8	
	_BV( 1 )	, // PK 1 ** 63 ** A9	
	_BV( 2 )	, // PK 2 ** 64 ** A10	
	_BV( 3 )	, // PK 3 ** 65 ** A11	
	_BV( 4 )	, // PK 4 ** 66 ** A12	
	_BV( 5 )	, // PK 5 ** 67 ** A13	
	_BV( 6 )	, // PK 6 ** 68 ** A14	
	_BV( 7 )	, // PK 7 ** 69 ** A15	
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------		
	NOT_ON_TIMER	, // PE 0 ** 0 ** USART0_RX	
	NOT_ON_TIMER	, // PE 1 ** 1 ** USART0_TX	
	TIMER3B	, // PE 4 ** 2 ** PWM2	
	TIMER3C	, // PE 5 ** 3 ** PWM3	
	TIMER0B	, // PG 5 ** 4 ** PWM4	
	TIMER3A	, // PE 3 ** 5 ** PWM5	
	TIMER4A	, // PH 3 ** 6 ** PWM6	
	TIMER4B	, // PH 4 ** 7 ** PWM7	
	TIMER4C	, // PH 5 ** 8 ** PWM8	
	TIMER2B	, // PH 6 ** 9 ** PWM9	
	TIMER2A	, // PB 4 ** 10 ** PWM10	
	TIMER1A	, // PB 5 ** 11 ** PWM11	
	TIMER1B	, // PB 6 ** 12 ** PWM12	
	TIMER0A	, // PB 7 ** 13 ** PWM13	
	NOT_ON_TIMER	, // PJ 1 ** 14 ** USART3_TX	
	NOT_ON_TIMER	, // PJ 0 ** 15 ** USART3_RX	
	NOT_ON_TIMER	, // PH 1 ** 16 ** USART2_TX	
	NOT_ON_TIMER	, // PH 0 ** 17 ** USART2_RX	
	NOT_ON_TIMER	, // PD 3 ** 18 ** USART1_TX	
	NOT_ON_TIMER	, // PD 2 ** 19 ** USART1_RX	
	NOT_ON_TIMER	, // PD 1 ** 20 ** I2C_SDA	
	NOT_ON_TIMER	, // PD 0 ** 21 ** I2C_SCL	
	NOT_ON_TIMER	, // PA 0 ** 22 ** D22	
	NOT_ON_TIMER	, // PA 1 ** 23 ** D23	
	NOT_ON_TIMER	, // PA 2 ** 24 ** D24	
	NOT_ON_TIMER	, // PA 3 ** 25 ** D25	
	NOT_ON_TIMER	, // PA 4 ** 26 ** D26	
	NOT_ON_TIMER	, // PA 5 ** 27 ** D27	
	NOT_ON_TIMER	, // PA 6 ** 28 ** D28	
	NOT_ON_TIMER	, // PA 7 ** 29 ** D29	
	NOT_ON_TIMER	, // PC 7 ** 30 ** D30	
	NOT_ON_TIMER	, // PC 6 ** 31 ** D31	
	NOT_ON_TIMER	, // PC 5 ** 32 ** D32	
	NOT_ON_TIMER	, // PC 4 ** 33 ** D33	
	NOT_ON_TIMER	, // PC 3 ** 34 ** D34	
	NOT_ON_TIMER	, // PC 2 ** 35 ** D35	
	NOT_ON_TIMER	, // PC 1 ** 36 ** D36	
	NOT_ON_TIMER	, // PC 0 ** 37 ** D37	
	NOT_ON_TIMER	, // PD 7 ** 38 ** D38	
	NOT_ON_TIMER	, // PG 2 ** 39 ** D39	
	NOT_ON_TIMER	, // PG 1 ** 40 ** D40	
	NOT_ON_TIMER	, // PG 0 ** 41 ** D41	
	NOT_ON_TIMER	, // PL 7 ** 42 ** D42	
	NOT_ON_TIMER	, // PL 6 ** 43 ** D43	
	TIMER5C	, // PL 5 ** 44 ** D44	
	TIMER5B	, // PL 4 ** 45 ** D45	
	TIMER5A	, // PL 3 ** 46 ** D46	
	NOT_ON_TIMER	, // PL 2 ** 47 ** D47	
	NOT_ON_TIMER	, // PL 1 ** 48 ** D48	
	NOT_ON_TIMER	, // PL 0 ** 49 ** D49	
	NOT_ON_TIMER	, // PB 3 ** 50 ** SPI_MISO	
	NOT_ON_TIMER	, // PB 2 ** 51 ** SPI_MOSI	
	NOT_ON_TIMER	, // PB 1 ** 52 ** SPI_SCK	
	NOT_ON_TIMER	, // PB 0 ** 53 ** SPI_SS	
	NOT_ON_TIMER	, // PF 0 ** 54 ** A0	
	NOT_ON_TIMER	, // PF 1 ** 55 ** A1	
	NOT_ON_TIMER	, // PF 2 ** 56 ** A2	
	NOT_ON_TIMER	, // PF 3 ** 57 ** A3	
	NOT_ON_TIMER	, // PF 4 ** 58 ** A4	
	NOT_ON_TIMER	, // PF 5 ** 59 ** A5	
	NOT_ON_TIMER	, // PF 6 ** 60 ** A6	
	NOT_ON_TIMER	, // PF 7 ** 61 ** A7	
	NOT_ON_TIMER	, // PK 0 ** 62 ** A8	
	NOT_ON_TIMER	, // PK 1 ** 63 ** A9	
	NOT_ON_TIMER	, // PK 2 ** 64 ** A10	
	NOT_ON_TIMER	, // PK 3 ** 65 ** A11	
	NOT_ON_TIMER	, // PK 4 ** 66 ** A12	
	NOT_ON_TIMER	, // PK 5 ** 67 ** A13	
	NOT_ON_TIMER	, // PK 6 ** 68 ** A14	
	NOT_ON_TIMER	, // PK 7 ** 69 ** A15	
};
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
        NOT_A_PORT,
    &DDRA,
        &DDRB,
        &DDRC,
        &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
        NOT_A_PORT,
        &PORTA,
        &PORTB,
        &PORTC,
        &PORTD,
};
const uint16_t PROGMEM port_to_input_PGM[] =
{
        NOT_A_PORT,
        &PINA,
        &PINB,
        &PINC,
        &PIND,
};
const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
        PB, /* 0 */
        PB,
        PB,
        PB,
        PB,
        PB,
        PB,
        PB,
        PD, /* 8 */
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
        PD,
        PC, /* 16 */
        PC,
        PC,
        PC,
        PC,
        PC,
        PC,
        PC,
        PA, /* 24 */
        PA,
        PA,
        PA,
        PA,
        PA,
        PA,
        PA  /* 31 */
};
const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
        _BV(0), /* 0, port B */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
        _BV(0), /* 8, port D */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
        _BV(0), /* 16, port C */
        _BV(1),
        _BV(2),
        _BV(3),
        _BV(4),
        _BV(5),
        _BV(6),
        _BV(7),
        _BV(7), /* 24, port A */
        _BV(6),
        _BV(5),
        _BV(4),
        _BV(3),
        _BV(2),
        _BV(1),
        _BV(0)
};
const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
        NOT_ON_TIMER,   /* 0  - PB0 */
        NOT_ON_TIMER,   /* 1  - PB1 */
        NOT_ON_TIMER,   /* 2  - PB2 */
        TIMER0A,        /* 3  - PB3 */
        TIMER0B,                /* 4  - PB4 */
        NOT_ON_TIMER,   /* 5  - PB5 */
        NOT_ON_TIMER,   /* 6  - PB6 */
        NOT_ON_TIMER,   /* 7  - PB7 */
        NOT_ON_TIMER,   /* 8  - PD0 */
        NOT_ON_TIMER,   /* 9  - PD1 */
        NOT_ON_TIMER,   /* 10 - PD2 */
        NOT_ON_TIMER,   /* 11 - PD3 */
        TIMER1B,        /* 12 - PD4 */
        TIMER1A,        /* 13 - PD5 */
        TIMER2B,        /* 14 - PD6 */
        TIMER2A,        /* 15 - PD7 */
        NOT_ON_TIMER,   /* 16 - PC0 */
        NOT_ON_TIMER,   /* 17 - PC1 */
        NOT_ON_TIMER,   /* 18 - PC2 */
        NOT_ON_TIMER,   /* 19 - PC3 */
        NOT_ON_TIMER,   /* 20 - PC4 */
        NOT_ON_TIMER,   /* 21 - PC5 */
        NOT_ON_TIMER,   /* 22 - PC6 */
        NOT_ON_TIMER,   /* 23 - PC7 */
        NOT_ON_TIMER,   /* 24 - PA0 */
        NOT_ON_TIMER,   /* 25 - PA1 */
        NOT_ON_TIMER,   /* 26 - PA2 */
        NOT_ON_TIMER,   /* 27 - PA3 */
        NOT_ON_TIMER,   /* 28 - PA4 */
        NOT_ON_TIMER,   /* 29 - PA5 */
        NOT_ON_TIMER,   /* 30 - PA6 */
        NOT_ON_TIMER   /* 31 - PA7 */
};

#else
// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	&DDRB,
	&DDRC,
	&DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	&PORTB,
	&PORTC,
	&PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	&PINB,
	&PINC,
	&PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PB, /* 8 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PC, /* 14 */
	PC,
	PC,
	PC,
	PC,
	PC,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(0), /* 14, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	// on the ATmega168, digital pin 3 has hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
#else
	TIMER2B,
#endif
	NOT_ON_TIMER,
	// on the ATmega168, digital pins 5 and 6 have hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
	NOT_ON_TIMER,
#else
	TIMER0B,
	TIMER0A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 8 - port B */
	TIMER1A,
	TIMER1B,
#if defined(__AVR_ATmega8__)
	TIMER2,
#else
	TIMER2A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 14 - port C */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};
#endif
