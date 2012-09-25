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

  $Id: pins_arduino.c 254 2007-04-20 23:17:38Z mellis $
*/

#include <avr/io.h>
#include "wiring_private.h"
#include "pins_arduino.h"

// On the Sanguino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

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

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint8_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
        (uint8_t) &DDRA,
	(uint8_t) &DDRB,
	(uint8_t) &DDRC,
	(uint8_t) &DDRD,
};

const uint8_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	(uint8_t) &PORTA,
	(uint8_t) &PORTB,
	(uint8_t) &PORTC,
	(uint8_t) &PORTD,
};

const uint8_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PORT,
	(uint8_t) &PINA,
	(uint8_t) &PINB,
	(uint8_t) &PINC,
	(uint8_t) &PIND,
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
	NOT_ON_TIMER, 	/* 0  - PB0 */
	NOT_ON_TIMER, 	/* 1  - PB1 */
	NOT_ON_TIMER, 	/* 2  - PB2 */
	TIMER0A,     	/* 3  - PB3 */
	TIMER0B, 		/* 4  - PB4 */
	NOT_ON_TIMER, 	/* 5  - PB5 */
	NOT_ON_TIMER, 	/* 6  - PB6 */
	NOT_ON_TIMER,	/* 7  - PB7 */
	NOT_ON_TIMER, 	/* 8  - PD0 */
	NOT_ON_TIMER, 	/* 9  - PD1 */
	NOT_ON_TIMER, 	/* 10 - PD2 */
	NOT_ON_TIMER, 	/* 11 - PD3 */
	TIMER1B,     	/* 12 - PD4 */
	TIMER1A,     	/* 13 - PD5 */
	TIMER2B,     	/* 14 - PD6 */
	TIMER2A,     	/* 15 - PD7 */
	NOT_ON_TIMER, 	/* 16 - PC0 */
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
