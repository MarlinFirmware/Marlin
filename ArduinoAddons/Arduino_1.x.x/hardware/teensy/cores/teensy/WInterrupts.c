/* Interrupt functions for the Teensy and Teensy++
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008-2010 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#include "wiring.h"
#include "wiring_private.h"

#if defined(__AVR_ATmega32U4__)
#define NUM_INTERRUPT 4
#else
#define NUM_INTERRUPT 8
#endif

volatile static voidFuncPtr intFunc[NUM_INTERRUPT];

static const uint8_t PROGMEM interrupt_mode_mask[] = {0xFC, 0xF3, 0xCF, 0x3F};

static uint8_t pin2int(uint8_t pin)
{
	switch (pin) {
		case CORE_INT0_PIN: return 0;
		case CORE_INT1_PIN: return 1;
		case CORE_INT2_PIN: return 2;
		case CORE_INT3_PIN: return 3;
#if !defined(__AVR_ATmega32U4__)
		case CORE_INT4_PIN: return 4;
		case CORE_INT5_PIN: return 5;
		case CORE_INT6_PIN: return 6;
		case CORE_INT7_PIN: return 7;
#endif
		default: return 255;
	}
}

#if defined(__AVR_ATmega32U4__)
void attachInterrupt(uint8_t inum, void (*userFunc)(void), uint8_t mode)
{
	uint8_t mask;

	if (inum >= NUM_INTERRUPT) {
		inum = pin2int(inum);
		if (inum >= NUM_INTERRUPT) return;
	}
	intFunc[inum] = userFunc;
	mask = pgm_read_byte(interrupt_mode_mask + inum);
	mode &= 0x03;
	EICRA = (EICRA & mask) | (mode << (inum * 2));
	EIMSK |= (1 << inum);
}
#else
void attachInterrupt(uint8_t inum, void (*userFunc)(void), uint8_t mode)
{
	uint8_t mask, index;

	if (inum >= NUM_INTERRUPT) {
		inum = pin2int(inum);
		if (inum >= NUM_INTERRUPT) return;
	}
	intFunc[inum] = userFunc;
	index = inum & 3;
	mask = pgm_read_byte(interrupt_mode_mask + index);
	mode &= 0x03;
	if (inum & 4) {
		EICRB = (EICRB & mask) | (mode << (index * 2));
	} else {
		EICRA = (EICRA & mask) | (mode << (index * 2));
	}
	EIMSK |= (1 << inum);
}
#endif

void detachInterrupt(uint8_t inum)
{
	if (inum >= NUM_INTERRUPT) {
		inum = pin2int(inum);
		if (inum >= NUM_INTERRUPT) return;
	}
	EIMSK &= ~(1 << inum);
	intFunc[inum] = 0;
}

SIGNAL(INT0_vect) {
	if (intFunc[0]) intFunc[0]();	// INT0 is pin 0 (PD0)
}
SIGNAL(INT1_vect) {
	if (intFunc[1]) intFunc[1]();	// INT1 is pin 1 (PD1)
}
SIGNAL(INT2_vect) {
	if (intFunc[2]) intFunc[2]();	// INT2 is pin 2 (PD2) (also Serial RX)
}
SIGNAL(INT3_vect) {
	if (intFunc[3]) intFunc[3]();	// INT3 is pin 3 (PD3) (also Serial TX)
}
#if !defined(__AVR_ATmega32U4__)
SIGNAL(INT4_vect) {
	if (intFunc[4]) intFunc[4]();	// INT4 is pin 20 (PC7)
}
SIGNAL(INT5_vect) {
	if (intFunc[5]) intFunc[5]();	// INT5 is pin 4 (PD4)
}
SIGNAL(INT6_vect) {
	if (intFunc[6]) intFunc[6]();	// INT6 is pin 6 (PD6)
}
SIGNAL(INT7_vect) {
	if (intFunc[7]) intFunc[7]();	// INT7 is pin 7 (PD7)
}
#endif

