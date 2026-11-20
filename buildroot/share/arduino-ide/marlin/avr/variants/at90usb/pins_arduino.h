/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis
  Modified 2012 by Fredrik Hubinette
  Modified 2014-2015 by Matthew Wilson

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

// PE7 is our status LED
#define TX_RX_LED_INIT		DDRE |= (1<<7)
#define TXLED0			0
#define TXLED1			0
#define RXLED0			PORTE |= (1<<7)
#define RXLED1			PORTE &= ~(1<<7)

static const uint8_t SDA = 1;
static const uint8_t SCL = 0;

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

#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 38 : -1)

// Pins below still needs to be configured - Hubbe.

#define digitalPinToPCICR(p)    ((((p) >= 0 && (p) <= 3) || ((p) >= 18 && (p) <= 19) || ((p) >= 36 && (p) <= 37)) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) 0
#define digitalPinToPCMSK(p)    ((((p) >= 0 && (p) <= 3) || ((p) >= 18 && (p) <= 19) || ((p) >= 36 && (p) <= 37)) ? (&PCMSK0) : ((uint8_t *)0))
#define digitalPinToPCMSKbit(p) ( ((p) >= 0 && (p) <= 3) ? (p) : ((p) == 18 ? 6 : ((p) == 19 ? 7 : ((p) == 36 ? 4 : ((p) == 37 ? 5 : (-1))))))

#define digitalPinHasPWM(p)     ((p) == 0 || (p) == 1 || (p) == 14 || (p) == 15 || (p) == 16 || (p) == 24 || (p) == 25 || (p) == 26 || (p) == 27)

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
  PD, // 0  - PD0 - INT0 - PWM
  PD, // 1  - PD1 - INT1 - PWM
  PD, // 2  - PD2 - INT2 - RX
  PD, // 3  - PD3 - INT3 - TX
  PD, // 4  - PD4
  PD, // 5  - PD5
  PD, // 6  - PD6
  PD, // 7  - PD7
  PE, // 8  - PE0
  PE, // 9  - PE1
  PC, // 10 - PC0
  PC, // 11 - PC1
  PC, // 12 - PC2
  PC, // 13 - PC3
  PC, // 14 - PC4 - PWM
  PC, // 15 - PC5 - PWM
  PC, // 16 - PC6 - PWM
  PC, // 17 - PC7
  PE, // 18 - PE6 - INT6
  PE, // 19 - PE7 - INT7
  PB, // 20 - PB0
  PB, // 21 - PB1
  PB, // 22 - PB2
  PB, // 23 - PB3
  PB, // 24 - PB4 - PWM
  PB, // 25 - PB5 - PWM
  PB, // 26 - PB6 - PWM
  PB, // 27 - PB7 - PWM
  PA, // 28 - PA0
  PA, // 29 - PA1
  PA, // 30 - PA2
  PA, // 31 - PA3
  PA, // 32 - PA4
  PA, // 33 - PA5
  PA, // 34 - PA6
  PA, // 35 - PA7
  PE, // 36 - PE4 - INT4
  PE, // 37 - PE5 - INT5
  PF, // 38 - PF0 - A0
  PF, // 39 - PF1 - A1
  PF, // 40 - PF2 - A2
  PF, // 41 - PF3 - A3
  PF, // 42 - PF4 - A4
  PF, // 43 - PF5 - A5
  PF, // 44 - PF6 - A6
  PF, // 45 - PF7 - A7
  PE, // 46 - PE2 (not defined in teensy)
  PE, // 47 - PE3 (not defined in teensy)
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(0), // 0  - PD0 - INT0 - PWM
  _BV(1), // 1  - PD1 - INT1 - PWM
  _BV(2), // 2  - PD2 - INT2 - RX
  _BV(3), // 3  - PD3 - INT3 - TX
  _BV(4), // 4  - PD4
  _BV(5), // 5  - PD5
  _BV(6), // 6  - PD6
  _BV(7), // 7  - PD7
  _BV(0), // 8  - PE0
  _BV(1), // 9  - PE1
  _BV(0), // 10 - PC0
  _BV(1), // 11 - PC1
  _BV(2), // 12 - PC2
  _BV(3), // 13 - PC3
  _BV(4), // 14 - PC4 - PWM
  _BV(5), // 15 - PC5 - PWM
  _BV(6), // 16 - PC6 - PWM
  _BV(7), // 17 - PC7
  _BV(6), // 18 - PE6 - INT6
  _BV(7), // 19 - PE7 - INT7
  _BV(0), // 20 - PB0
  _BV(1), // 21 - PB1
  _BV(2), // 22 - PB2
  _BV(3), // 23 - PB3
  _BV(4), // 24 - PB4 - PWM
  _BV(5), // 25 - PB5 - PWM
  _BV(6), // 26 - PB6 - PWM
  _BV(7), // 27 - PB7 - PWM
  _BV(0), // 28 - PA0
  _BV(1), // 29 - PA1
  _BV(2), // 30 - PA2
  _BV(3), // 31 - PA3
  _BV(4), // 32 - PA4
  _BV(5), // 33 - PA5
  _BV(6), // 34 - PA6
  _BV(7), // 35 - PA7
  _BV(4), // 36 - PE4 - INT4
  _BV(5), // 37 - PE5 - INT5
  _BV(0), // 38 - PF0 - A0
  _BV(1), // 39 - PF1 - A1
  _BV(2), // 40 - PF2 - A2
  _BV(3), // 41 - PF3 - A3
  _BV(4), // 42 - PF4 - A4
  _BV(5), // 43 - PF5 - A5
  _BV(6), // 44 - PF6 - A6
  _BV(7), // 45 - PF7 - A7
  _BV(2), // 46 - PE2 (not defined in teensy)
  _BV(3), // 47 - PE3 (not defined in teensy)
};


// TODO(unrepentantgeek) complete this map of PWM capable pins
const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  TIMER0A,      // 0  - PD0 - INT0 - PWM
  TIMER2B,      // 1  - PD1 - INT1 - PWM
  NOT_ON_TIMER, // 2  - PD2 - INT2 - RX
  NOT_ON_TIMER, // 3  - PD3 - INT3 - TX
  NOT_ON_TIMER, // 4  - PD4
  NOT_ON_TIMER, // 5  - PD5
  NOT_ON_TIMER, // 6  - PD6
  NOT_ON_TIMER, // 7  - PD7
  NOT_ON_TIMER, // 8  - PE0
  NOT_ON_TIMER, // 9  - PE1
  NOT_ON_TIMER, // 10 - PC0
  NOT_ON_TIMER, // 11 - PC1
  NOT_ON_TIMER, // 12 - PC2
  NOT_ON_TIMER, // 13 - PC3
  TIMER3C,      // 14 - PC4 - PWM
  TIMER3B,      // 15 - PC5 - PWM
  TIMER3A,      // 16 - PC6 - PWM
  NOT_ON_TIMER, // 17 - PC7
  NOT_ON_TIMER, // 18 - PE6 - INT6
  NOT_ON_TIMER, // 19 - PE7 - INT7
  NOT_ON_TIMER, // 20 - PB0
  NOT_ON_TIMER, // 21 - PB1
  NOT_ON_TIMER, // 22 - PB2
  NOT_ON_TIMER, // 23 - PB3
  TIMER2A,      // 24 - PB4 - PWM
  TIMER1A,      // 25 - PB5 - PWM
  TIMER1B,      // 26 - PB6 - PWM
  NOT_ON_TIMER, // 27 - PB7 - PWM  // This should be on TIMER1C
  NOT_ON_TIMER, // 28 - PA0
  NOT_ON_TIMER, // 29 - PA1
  NOT_ON_TIMER, // 30 - PA2
  NOT_ON_TIMER, // 31 - PA3
  NOT_ON_TIMER, // 32 - PA4
  NOT_ON_TIMER, // 33 - PA5
  NOT_ON_TIMER, // 34 - PA6
  NOT_ON_TIMER, // 35 - PA7
  NOT_ON_TIMER, // 36 - PE4 - INT4
  NOT_ON_TIMER, // 37 - PE5 - INT5
  NOT_ON_TIMER, // 38 - PF0 - A0
  NOT_ON_TIMER, // 39 - PF1 - A1
  NOT_ON_TIMER, // 40 - PF2 - A2
  NOT_ON_TIMER, // 41 - PF3 - A3
  NOT_ON_TIMER, // 42 - PF4 - A4
  NOT_ON_TIMER, // 43 - PF5 - A5
  NOT_ON_TIMER, // 44 - PF6 - A6
  NOT_ON_TIMER, // 45 - PF7 - A7
  NOT_ON_TIMER, // 46 - PE2 (not defined in teensy)
  NOT_ON_TIMER, // 47 - PE3 (not defined in teensy)
};

const uint8_t PROGMEM analog_pin_to_channel_PGM[8] = {
	0,	// A0		PF0	ADC0
	1,	// A1		PF1	ADC1
	2,	// A2		PF2	ADC2
	3,	// A3		PF3	ADC3
	4,	// A4		PF4	ADC4
	5,	// A5		PF5	ADC5
	6,	// A6	  	PD6	ADC6
	7,	// A7		PD7	ADC7
};

#endif /* ARDUINO_MAIN */
#endif /* Pins_Arduino_h */
