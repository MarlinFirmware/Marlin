/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis
  Modified 2012 by Fredrik Hubinette

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

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            48
#define NUM_ANALOG_INPUTS           8

#define TX_RX_LED_INIT		DDRE |= (1<<7)
#define TXLED0			0
#define TXLED1			0
#define RXLED0			PORTE |= (1<<7)
#define RXLED1			PORTE &= ~(1<<7)

static const uint8_t SDA = 0;
static const uint8_t SCL = 1;

// Map SPI port to 'new' pins D14..D17
static const uint8_t SS   = 20;
static const uint8_t MOSI = 22;
static const uint8_t MISO = 23;
static const uint8_t SCK  = 21;

// Mapping of analog pins as digital I/O
// A6-A11 share with digital pins
static const uint8_t A0 = 38;   // F0
static const uint8_t A1 = 39;   // F1
static const uint8_t A2 = 40;   // F2
static const uint8_t A3 = 41;   // F3
static const uint8_t A4 = 42;   // F4
static const uint8_t A5 = 43;   // F5
static const uint8_t A6 = 44;	// F6
static const uint8_t A7 = 45;	// F7

#define analogInputToDigitalPin(p)  ((p < 16) ? (p) + 54 : -1)

// Pins below still needs to be configured - Hubbe.

#define digitalPinToPCICR(p)    ((((p) >= 8 && (p) <= 11) || ((p) >= 14 && (p) <= 17) || ((p) >= A8 && (p) <= A10)) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) 0
#define digitalPinToPCMSK(p)    ((((p) >= 8 && (p) <= 11) || ((p) >= 14 && (p) <= 17) || ((p) >= A8 && (p) <= A10)) ? (&PCMSK0) : ((uint8_t *)0))
#define digitalPinToPCMSKbit(p) ( ((p) >= 8 && (p) <= 11) ? (p) - 4 : ((p) == 14 ? 3 : ((p) == 15 ? 1 : ((p) == 16 ? 2 : ((p) == 17 ? 0 : (p - A8 + 4))))))


#define digitalPinHasPWM(p)         ((p) == 12 || (p) == 13 || (p) == 14 || (p) == 20 || (p) == 21 || (p) == 22)

//	__AVR_ATmega32U4__ has an unusual mapping of pins to channels
extern const uint8_t PROGMEM analog_pin_to_channel_PGM[];
#define analogPinToChannel(P)  ( pgm_read_byte( analog_pin_to_channel_PGM + (P) ) )

#ifdef ARDUINO_MAIN


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
// Note PA == 1, PB == 2, etc. (GAH!)
const uint16_t PROGMEM port_to_mode_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &DDRA,
  (uint16_t) &DDRB,
  (uint16_t) &DDRC,
  (uint16_t) &DDRD,
  (uint16_t) &DDRE,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &PORTA,
  (uint16_t) &PORTB,
  (uint16_t) &PORTC,
  (uint16_t) &PORTD,
  (uint16_t) &PORTE,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
  NOT_A_PORT,
  (uint16_t) &PINA,
  (uint16_t) &PINB,
  (uint16_t) &PINC,
  (uint16_t) &PIND,
  (uint16_t) &PINE,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
  PA, // 0
  PA,
  PA,
  PA,
  PA,
  PA,
  PA,
  PA,
  PB, // 8
  PB,
  PB,
  PB,
  PB,
  PB,
  PB,
  PB,
  PC, // 16
  PC,
  PC,
  PC,
  PC,
  PC,
  PC,
  PC,
  PD, // 24
  PD,
  PD,
  PD,
  PD,
  PD,
  PD,
  PD,
  PE, // 32
  PE,
  PE,
  PE,
  PE,
  PE,
  PE,
  PE, // 39 - PE7
  PF, // 40 - A0 - PF0
  PF,
  PF,
  PF,
  PF,
  PF,
  PF,
  PF, // 47 - A7 - PF7
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(0), // PA0
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0), // PB0
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0), // PC0
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0), // PD0
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0), // PE0
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
  _BV(0), // PF0
  _BV(1),
  _BV(2),
  _BV(3),
  _BV(4),
  _BV(5),
  _BV(6),
  _BV(7),
};


// TODO(unrepentantgeek) complete this map of PWM capable pins
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER, //  0 - PA0
  NOT_ON_TIMER, //  1 - PA1
  NOT_ON_TIMER, //  2 - PA2
  NOT_ON_TIMER, //  3 - PA3
  NOT_ON_TIMER, //  4 - PA4
  NOT_ON_TIMER, //  5 - PA5
  NOT_ON_TIMER, //  6 - PA6
  NOT_ON_TIMER, //  7 - PA7
  NOT_ON_TIMER, //  8 - PB0
  NOT_ON_TIMER, //  9 - PB1
  NOT_ON_TIMER, // 10 - PB2
  NOT_ON_TIMER, // 11 - PB3
  TIMER2A,      // 12 - PB4
  TIMER1A,      // 13 - PB5
  TIMER1B,      // 14 - PB6
  NOT_ON_TIMER, // 15 - PB7
  NOT_ON_TIMER, // 16 - PC0
  NOT_ON_TIMER, // 17 - PC1
  NOT_ON_TIMER, // 18 - PC2
  NOT_ON_TIMER, // 19 - PC3
  TIMER3C,      // 20 - PC4
  TIMER3B,      // 21 - PC5
  TIMER3A,      // 22 - PC6
  NOT_ON_TIMER, // 23 - PC7
  NOT_ON_TIMER, // 24 - PD0
  NOT_ON_TIMER, // 25 - PD1
  NOT_ON_TIMER, // 26 - PD2
  NOT_ON_TIMER, // 27 - PD3
  NOT_ON_TIMER, // 28 - PD4
  NOT_ON_TIMER, // 29 - PD5
  NOT_ON_TIMER, // 30 - PD6
  NOT_ON_TIMER, // 31 - PD7
  NOT_ON_TIMER, // 32 - PE0
  NOT_ON_TIMER, // 33 - PE1
  NOT_ON_TIMER, // 34 - PE2
  NOT_ON_TIMER, // 35 - PE3
  NOT_ON_TIMER, // 36 - PE4
  NOT_ON_TIMER, // 37 - PE5
  NOT_ON_TIMER, // 38 - PE6
  NOT_ON_TIMER, // 39 - PE7
  NOT_ON_TIMER, // 40 - A0 - PF0
  NOT_ON_TIMER, // 41 - A1 - PF1
  NOT_ON_TIMER, // 42 - A2 - PF2
  NOT_ON_TIMER, // 43 - A3 - PF3
  NOT_ON_TIMER, // 44 - A4 - PF4
  NOT_ON_TIMER, // 45 - A5 - PF5
  NOT_ON_TIMER, // 46 - A6 - PF6
  NOT_ON_TIMER, // 47 - A7 - PF7
};

const uint8_t PROGMEM analog_pin_to_channel_PGM[12] = {
	7,	// A0				PF7					ADC7
	6,	// A1				PF6					ADC6
	5,	// A2				PF5					ADC5
	4,	// A3				PF4					ADC4
	1,	// A4				PF1					ADC1
	0,	// A5				PF0					ADC0
	8,	// A6		D4		PD4					ADC8
	10,	// A7		D6		PD7					ADC10
	11,	// A8		D8		PB4					ADC11
	12,	// A9		D9		PB5					ADC12
	13,	// A10		D10		PB6					ADC13
	9	// A11		D12		PD6					ADC9
};

#endif /* ARDUINO_MAIN */
#endif /* Pins_Arduino_h */
