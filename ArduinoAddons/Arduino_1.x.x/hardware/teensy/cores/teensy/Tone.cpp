/* Tone generation for the Teensy and Teensy++
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2010 PJRC.COM, LLC
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

#include <avr/interrupt.h>
#include "wiring.h"
#include "core_pins.h"
#include "pins_arduino.h"

static uint8_t timer_acquired = 0;

static uint8_t *tone1_reg = (uint8_t *)0;
static uint8_t  tone1_mask = 0;
static uint16_t tone1_inc = 0;
static uint32_t tone1_count = 0;

static uint8_t *tone2_reg = (uint8_t *)0;
static uint8_t  tone2_mask = 0;
static uint16_t tone2_inc = 0;
static uint32_t tone2_count = 0;

static uint8_t *tone3_reg = (uint8_t *)0;
static uint8_t  tone3_mask = 0;
static uint16_t tone3_inc = 0;
static uint32_t tone3_count = 0;

#define MAX_FREQ (F_CPU / 16 / 25)
#define MIN_FREQ (F_CPU / 16 / 65535 + 1)



#define PIN_REG_AND_MASK_LOOKUP(pin, reg, mask) \
        asm volatile(                           \
                "lsl %2"                "\n\t"  \
                "add %A3, %2"           "\n\t"  \
                "adc %B3, __zero_reg__" "\n\n"  \
                "lpm %1, Z+"            "\n\t"  \
                "lpm %A0, Z"            "\n\t"  \
                "ldi %B0, 0"            "\n"    \
                : "=z" (reg), "=r" (mask), "+r" (pin)   \
                : "z" (digital_pin_table_PGM), "2" (pin))

#if defined(__AVR_ATmega32U4__)
//#define TONE_USE_TIMER1
#define TONE_USE_TIMER3
#elif defined(__AVR_AT90USB1286__)
//#define TONE_USE_TIMER1
#define TONE_USE_TIMER3
#elif defined(__AVR_AT90USB162__)
#define TONE_USE_TIMER1
#elif defined(__AVR_AT90USB646__)
//#define TONE_USE_TIMER1
#define TONE_USE_TIMER3
#endif

#ifdef TONE_USE_TIMER3
#define TIMSKx	TIMSK3
#define OCIExA	OCIE3A
#define OCIExB	OCIE3B
#define OCIExC	OCIE3C
#define TCCRxA	TCCR3A
#define WGMx0	WGM30
#define TCCRxB	TCCR3B
#define	CSx1	CS31
#define TCNTx	TCNT3
#define OCRxA	OCR3A
#define OCRxB	OCR3B
#define OCRxC	OCR3C
#define TIFRx	TIFR3
#define OCFxA	OCF3A
#define OCFxB	OCF3B
#define OCFxC	OCF3C
#define VECTxA	TIMER3_COMPA_vect
#define VECTxB	TIMER3_COMPB_vect
#define VECTxC	TIMER3_COMPC_vect
#endif
#ifdef TONE_USE_TIMER1
#define TIMSKx	TIMSK1
#define OCIExA	OCIE1A
#define OCIExB	OCIE1B
#define OCIExC	OCIE1C
#define TCCRxA	TCCR1A
#define WGMx0	WGM10
#define TCCRxB	TCCR1B
#define	CSx1	CS11
#define TCNTx	TCNT1
#define OCRxA	OCR1A
#define OCRxB	OCR1B
#define OCRxC	OCR1C
#define TIFRx	TIFR1
#define OCFxA	OCF1A
#define OCFxB	OCF1B
#define OCFxC	OCF1C
#define VECTxA	TIMER1_COMPA_vect
#define VECTxB	TIMER1_COMPB_vect
#define VECTxC	TIMER1_COMPC_vect
#endif


void tone(uint8_t pin, uint16_t frequency, uint32_t duration)
{
	uint8_t *reg;
	uint8_t mask;
	uint16_t inc;
	uint32_t count;

	if (pin >= CORE_NUM_TOTAL_PINS) return;
	if (frequency < MIN_FREQ) {
		frequency = MIN_FREQ;
	} else if (frequency > MAX_FREQ) {
		frequency = MAX_FREQ;
	}
	inc = (F_CPU / 16 + frequency / 2) / frequency;
	if (duration) {
		count = duration * frequency / 500;
	} else {
		count = 0;
	}
	if (!timer_acquired) {
		TIMSKx = 0;			// disable all interrupts
		TCCRxA = 0;
		TCCRxB = (1<<CSx1);		// normal mode, div8 prescale
		timer_acquired = 1;
	}
	PIN_REG_AND_MASK_LOOKUP(pin, reg, mask);

	if (!tone1_mask || (tone1_mask == mask && tone1_reg == reg)) {
		TIMSKx &= ~(1<<OCIExA);	// disable compare interrupt
		tone1_reg = reg;
		tone1_mask = mask;
		tone1_count = count;
		tone1_inc = inc;
		cli();
		*(reg + 2) &= ~mask;	// clear pin
		*(reg + 1) |= mask;	// output mode
		OCRxA = TCNTx + inc;
		TIFRx |= (1<<OCFxA);	// clear any pending compare match
		sei();
		TIMSKx |= (1<<OCIExA);	// enable compare interrupt
		return;
	}
	if (!tone2_mask || (tone2_mask == mask && tone2_reg == reg)) {
		TIMSKx &= ~(1<<OCIExB);	// disable compare interrupt
		tone2_reg = reg;
		tone2_mask = mask;
		tone2_count = count;
		tone2_inc = inc;
		cli();
		*(reg + 2) &= ~mask;	// clear pin
		*(reg + 1) |= mask;	// output mode
		OCRxB = TCNTx + inc;
		TIFRx |= (1<<OCFxB);	// clear any pending compare match
		sei();
		TIMSKx |= (1<<OCIExB);	// enable compare interrupt
		return;
	}
	if (!tone3_mask || (tone3_mask == mask && tone3_reg == reg)) {
		TIMSKx &= ~(1<<OCIExC);	// disable compare interrupt
		tone3_reg = reg;
		tone3_mask = mask;
		tone3_count = count;
		tone3_inc = inc;
		cli();
		*(reg + 2) &= ~mask;	// clear pin
		*(reg + 1) |= mask;	// output mode
		OCRxC = TCNTx + inc;
		TIFRx |= (1<<OCFxC);	// clear any pending compare match
		sei();
		TIMSKx |= (1<<OCIExC);	// enable compare interrupt
		return;
	}
}

void noTone(uint8_t pin)
{
	uint8_t *reg;
	uint8_t mask;

	if (pin >= CORE_NUM_TOTAL_PINS) return;
	PIN_REG_AND_MASK_LOOKUP(pin, reg, mask);

	if (tone1_mask == mask && tone1_reg == reg) {
		TIMSKx &= ~(1<<OCIExA);
		tone1_mask = 0;
	} else if (tone2_mask == mask && tone2_reg == reg) {
		TIMSKx &= ~(1<<OCIExB);
		tone2_mask = 0;
	} else if (tone3_mask == mask && tone3_reg == reg) {
		TIMSKx &= ~(1<<OCIExC);
		tone3_mask = 0;
	}
	if (!tone1_mask && !tone2_mask && !tone3_mask) {
		TCCRxA = (1<<WGMx0);	// restore timer
		timer_acquired = 0;
	}
}


ISR(VECTxA)
{
	OCRxA += tone1_inc;
	*(tone1_reg) = tone1_mask;
	if (tone1_count > 0) {
		if ((--tone1_count) == 0) {
			*(tone1_reg + 2) &= ~tone1_mask;
			TIMSKx &= ~(1<<OCIExA);
			tone1_mask = 0;
			if (!tone2_mask && !tone3_mask) {
				TCCRxA = (1<<WGMx0);
				timer_acquired = 0;
			}
		}
	}
}

ISR(VECTxB)
{
	OCRxB += tone2_inc;
	*(tone2_reg) = tone2_mask;
	if (tone2_count > 0) {
		if ((--tone2_count) == 0) {
			*(tone2_reg + 2) &= ~tone2_mask;
			TIMSKx &= ~(1<<OCIExB);
			tone2_mask = 0;
			if (!tone1_mask && !tone3_mask) {
				TCCRxA = (1<<WGMx0);
				timer_acquired = 0;
			}
		}
	}
}

ISR(VECTxC)
{
	OCRxC += tone3_inc;
	*(tone3_reg) = tone3_mask;
	if (tone3_count > 0) {
		if ((--tone3_count) == 0) {
			*(tone3_reg + 2) &= ~tone3_mask;
			TIMSKx &= ~(1<<OCIExC);
			tone3_mask = 0;
			if (!tone1_mask && !tone2_mask) {
				TCCRxA = (1<<WGMx0);
				timer_acquired = 0;
			}
		}
	}
}


