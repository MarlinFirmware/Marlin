/* Pin functions for the Teensy and Teensy++
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
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include "wiring_private.h"
#include "pins_arduino.h"
#include "usb_private.h"
#include "core_pins.h"

// this doubles the analog input speed
#define USE_ADC_HIGH_SPEED


#ifdef USE_ADC_HIGH_SPEED
#define DEFAULT_ADCSRB		0x80  // ADHSM
#define ADC_PRESCALE_ADJUST	(-1)
#else 
#define DEFAULT_ADCSRB		0
#define ADC_PRESCALE_ADJUST	0
#endif

void _init_Teensyduino_internal_(void)
{
	cli();
	CLKPR = 0x80;
	CLKPR = CPU_PRESCALER;
	// timer 0, fast pwm mode
	TCCR0A = (1<<WGM01) | (1<<WGM00);
	TCCR0B = (1<<CS01) | (1<<CS00);		// div 64 prescaler
	sbi(TIMSK0, TOIE0);
	// timer 1, 8 bit phase correct pwm
	TCCR1A = (1<<WGM10);
	TCCR1B = (1<<CS11);			// div 8 prescaler
#if defined(__AVR_ATmega32U4__)
	// timer 3, 8 bit phase correct pwm
	TCCR3A = (1<<WGM30);
	TCCR3B = (1<<CS31);			// div 8 prescaler
	// timer 4, 8 bit phase correct pwm
	TCCR4A = (1<<PWM4A);
	TCCR4B = (1<<CS42);			// div 8 prescaler
	TCCR4C = (1<<PWM4D);
	TCCR4D = (1<<WGM40);			// phase correct pwm
	TCCR4E = 0;
	// ADC
	ADCSRA = (1<<ADEN) | (ADC_PRESCALER + ADC_PRESCALE_ADJUST);
	ADCSRB = DEFAULT_ADCSRB;
	DIDR0 = 0;
	DIDR2 = 0;
#endif
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	// timer 2, 8 bit phase correct pwm
	TCCR2A = (1<<WGM20);
	TCCR2B = (1<<CS21);			// div 8 prescaler
	// timer 3, 8 bit phase correct pwm
	TCCR3A = (1<<WGM30);
	TCCR3B = (1<<CS31);			// div 8 prescaler
	// ADC
	ADCSRA = (1<<ADEN) | (ADC_PRESCALER + ADC_PRESCALE_ADJUST);
	ADCSRB = DEFAULT_ADCSRB;
	DIDR0 = 0;
#endif
	// initialize USB
	usb_init();
	sei();
}


void _analogWrite(uint8_t pin, int val)
{
	pinMode(pin, OUTPUT);

	switch (pin) {
	case CORE_OC0B_PIN:
		if (val == 0) {
			CORE_PORTREG(CORE_OC0B_PIN) &= ~CORE_BITMASK(CORE_OC0B_PIN);
			cbi(TCCR0A, COM0B1);
		} else {
			OCR0B = val;
			sbi(TCCR0A, COM0B1);
		}
		break;
	case CORE_OC1A_PIN: //TIMER1A:
		OCR1A = val;
		sbi(TCCR1A, COM1A1);
		break;
	case CORE_OC1B_PIN: //TIMER1B:
		OCR1B = val;
		sbi(TCCR1A, COM1B1);
		break;
	case CORE_OC1C_PIN: //TIMER1C:
		OCR1C = val;
		sbi(TCCR1A, COM1C1);
		break;
#if defined(__AVR_ATmega32U4__)
	case CORE_OC3A_PIN: //TIMER3A:
		OCR3A = val;
		sbi(TCCR3A, COM3A1);
		break;
	case CORE_OC4A_PIN: //TIMER4A
		OCR4A = val;
		sbi(TCCR4A, COM4A1);
		break;
	case CORE_OC4D_PIN: //TIMER4D
		OCR4D = val;
		sbi(TCCR4C, COM4D1);
		break;
#endif
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	case CORE_OC2A_PIN: //TIMER2A:
		OCR2A = val;
		sbi(TCCR2A, COM2A1);
		break;
	case CORE_OC2B_PIN: //TIMER2B:
		OCR2B = val;
		sbi(TCCR2A, COM2B1);
		break;
	case CORE_OC3A_PIN: //TIMER3A:
		OCR3A = val;
		sbi(TCCR3A, COM3A1);
		break;
	case CORE_OC3B_PIN: //TIMER3B:
		OCR3B = val;
		sbi(TCCR3A, COM3B1);
		break;
	case CORE_OC3C_PIN: //TIMER3C:
		OCR3C = val;
		sbi(TCCR3A, COM3C1);
		break;
#endif
	default:
		if (val < 128) {
			digitalWrite(pin, LOW);
		} else {
			digitalWrite(pin, HIGH);
		}
	}
}



#if defined(__AVR_ATmega32U4__)
uint8_t w_analog_reference = 0x40;

static const uint8_t PROGMEM adc_mapping[] = {
// 0, 1, 4, 5, 6, 7, 13, 12, 11, 10, 9, 8
   0, 1, 4, 5, 6, 7, 13, 12, 11, 10, 9, 8, 10, 11, 12, 13, 7, 6, 5, 4, 1, 0, 8 
};

int analogRead(uint8_t pin)
{
	uint8_t low, high, adc;

	if (pin >= sizeof(adc_mapping)) return 0;
	adc = pgm_read_byte(adc_mapping + pin);
	if (adc < 8) {
		DIDR0 |= (1 << adc);
		//DDRF &= ~(1 << adc);
		//PORTF &= ~(1 << adc);
		ADCSRB = DEFAULT_ADCSRB;
		ADMUX = w_analog_reference | adc;
	} else {
		adc -= 8;
		DIDR2 |= (1 << adc);
		ADCSRB = DEFAULT_ADCSRB | (1<<MUX5);
		ADMUX = w_analog_reference | adc;
	}
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC)) ;
	low = ADCL;
	high = ADCH;
	return (high << 8) | low;
}

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)

uint8_t w_analog_reference = 0x40;

int analogRead(uint8_t pin)
{
	uint8_t low, high;

	if (pin >= PIN_F0 && pin <= PIN_F7) pin -= PIN_F0;
	if (pin < 8) {
		DIDR0 |= (1 << pin);
		//DDRF &= ~(1 << pin);
		//PORTF &= ~(1 << pin);
	}
	ADMUX = w_analog_reference | (pin & 0x1F);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC)) ;
	low = ADCL;
	high = ADCH;
	return (high << 8) | low;
}

#elif defined(__AVR_AT90USB162__)
int analogRead(uint8_t pin)
{
	return 0;
}
#endif



void _pinMode(uint8_t pin, uint8_t mode)
{
	if (mode == OUTPUT) {
		_pinMode_output(pin);
	} else if (mode == INPUT_PULLUP) {
		_pinMode_input_pullup(pin);
	} else {
		_pinMode_input(pin);
	}
}

#define PIN_REG_AND_MASK_LOOKUP(pin, reg, mask) \
	asm volatile(				\
		"lsl %2"		"\n\t"	\
		"add %A3, %2"		"\n\t"	\
		"adc %B3, __zero_reg__"	"\n\n"	\
		"lpm %1, Z+"		"\n\t"	\
		"lpm %A0, Z"		"\n\t"	\
		"ldi %B0, 0"		"\n"	\
		: "=z" (reg), "=r" (mask), "+r" (pin)	\
		: "z" (digital_pin_table_PGM), "2" (pin))

void _pinMode_output(uint8_t pin)
{
	volatile uint8_t *reg;
	uint8_t mask, status;

	if (pin >= CORE_NUM_TOTAL_PINS) return;
	PIN_REG_AND_MASK_LOOKUP(pin, reg, mask);
	status = SREG;
	cli();
	*(reg + 1) |= mask;
	SREG = status;
}

#if defined(__AVR_ATmega32U4__)
static const uint8_t PROGMEM didr_table_PGM[] = {
	(int)&DIDR2, ~0x02,
	(int)&DIDR2, ~0x04,
	(int)&DIDR2, ~0x08,
	(int)&DIDR2, ~0x10,
	(int)&DIDR2, ~0x20,
	(int)&DIDR0, 0x7F,  // ~0x80,
	(int)&DIDR0, ~0x40,
	(int)&DIDR0, ~0x20,
	(int)&DIDR0, ~0x10,
	(int)&DIDR0, ~0x02,
	(int)&DIDR0, ~0x01,
	(int)&DIDR2, ~0x01
};
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
static const uint8_t PROGMEM didr_table_PGM[] = {
	(int)&DIDR0, ~0x01,
	(int)&DIDR0, ~0x02,
	(int)&DIDR0, ~0x04,
	(int)&DIDR0, ~0x08,
	(int)&DIDR0, ~0x10,
	(int)&DIDR0, ~0x20,
	(int)&DIDR0, ~0x40,
	(int)&DIDR0, 0x7F  // ~0x80
};
#endif

#define PIN_DIDR_AND_MASK_LOOKUP(pin, didreg, didmask)	\
	asm volatile(					\
		"lsl	%3"			"\n\t"	\
		"add	%A2, %3"		"\n\t"	\
		"adc	%B2, __zero_reg__"	"\n\n"	\
		"lpm	%A0, Z+"		"\n\t"	\
		"ldi	%B0, 0"			"\n\t"	\
		"lpm	%1, Z+"			"\n\t"	\
		: "=x" (didreg), "=r" (didmask)		\
		: "z" (didr_table_PGM), "r" (pin))


void _pinMode_input(uint8_t pin)
{
	volatile uint8_t *reg, *didr=0;
	uint8_t mask, didrmask=0xFF, status;

	if (pin >= CORE_NUM_TOTAL_PINS) return;
	#if defined(__AVR_ATmega32U4__)
	if (pin >= 11 && pin <= 22) {
		PIN_DIDR_AND_MASK_LOOKUP((uint8_t)(pin - 11), didr, didrmask);
	}
	#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	if (pin >= PIN_F0 && pin <= PIN_F7) {
		PIN_DIDR_AND_MASK_LOOKUP((uint8_t)(pin - PIN_F0), didr, didrmask);
	}
	#endif
	PIN_REG_AND_MASK_LOOKUP(pin, reg, mask);
	status = SREG;
	cli();
	*(reg + 1) &= ~mask;
	*(reg + 2) &= ~mask;
	*didr &= didrmask;
	SREG = status;
}


void _pinMode_input_pullup(uint8_t pin)
{
	volatile uint8_t *reg, *didr=0;
	uint8_t mask, didrmask=0xFF, status;

	if (pin >= CORE_NUM_TOTAL_PINS) return;
	#if defined(__AVR_ATmega32U4__)
	if (pin >= 11 && pin <= 22) {
		PIN_DIDR_AND_MASK_LOOKUP((uint8_t)(pin - 11), didr, didrmask);
	}
	#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
	if (pin >= PIN_F0 && pin <= PIN_F7) {
		PIN_DIDR_AND_MASK_LOOKUP((uint8_t)(pin - PIN_F0), didr, didrmask);
	}
	#endif
	PIN_REG_AND_MASK_LOOKUP(pin, reg, mask);
	status = SREG;
	cli();
	*(reg + 1) &= ~mask;
	*(reg + 2) |= mask;
	*didr &= didrmask;
	SREG = status;
}



void _digitalWrite(void) __attribute__((naked));
void _digitalWrite_HIGH(void) __attribute__((naked));
void _digitalWrite_LOW(void) __attribute__((naked));
void _digitalWrite_bailout(void);
void _digitalWrite_HIGH_TABLE(void) __attribute__((naked));
void _digitalWrite_LOW_TABLE(void) __attribute__((naked));
void _digitalWrite_OC0B(void) __attribute__((naked));
void _digitalWrite_OC1A(void) __attribute__((naked));
void _digitalWrite_OC1B(void) __attribute__((naked));
void _digitalWrite_OC1C(void) __attribute__((naked));
void _digitalWrite_OC2A(void) __attribute__((naked));
void _digitalWrite_OC2B(void) __attribute__((naked));
void _digitalWrite_OC3A(void) __attribute__((naked));
void _digitalWrite_OC3B(void) __attribute__((naked));
void _digitalWrite_OC3C(void) __attribute__((naked));
void _digitalWrite_OC4A(void) __attribute__((naked));
void _digitalWrite_OC4D(void) __attribute__((naked));


void _digitalWrite(void)
{
	asm volatile(
		"tst	r0"				"\n\t"
		"breq	_digitalWrite_LOW"		"\n\t"
		"rjmp	_digitalWrite_HIGH"		"\n\t"
	);
}

void _digitalWrite_HIGH(void)
{
	asm volatile (
		#if !defined(DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK)
		"cpi	r30, %0"					"\n\t"
		"brsh	_digitalWrite_bailout"				"\n\t"
		#endif
		"lsl	r30"						"\n\t"
		//"clr	r31"						"\n\t"
		"subi	r30, lo8(-(pm(_digitalWrite_HIGH_TABLE)))"	"\n\t"
		"sbci	r31, hi8(-(pm(_digitalWrite_HIGH_TABLE)))"	"\n\t"
		"ijmp"							"\n\t"
		: : "M" (CORE_NUM_TOTAL_PINS)
	);
}

void _digitalWrite_LOW(void)
{
	asm volatile (
		#if !defined(DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK)
		"cpi	r30, %0"					"\n\t"
		"brsh	_digitalWrite_bailout"				"\n\t"
		#endif
		"lsl	r30"						"\n\t"
		//"clr	r31"						"\n\t"
		"subi	r30, lo8(-(pm(_digitalWrite_LOW_TABLE)))"	"\n\t"
		"sbci	r31, hi8(-(pm(_digitalWrite_LOW_TABLE)))"	"\n\t"
		"ijmp"							"\n\t"
		: : "M" (CORE_NUM_TOTAL_PINS)
	);
}

void _digitalWrite_bailout(void) {}

#if defined(__AVR_ATmega32U4__)
void _digitalWrite_HIGH_TABLE(void)
{
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 0
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 1
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 2
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 3
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 4
		"rjmp	_digitalWrite_OC1C"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN0_PORTREG)), "I" (CORE_PIN0_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN1_PORTREG)), "I" (CORE_PIN1_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN2_PORTREG)), "I" (CORE_PIN2_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN3_PORTREG)), "I" (CORE_PIN3_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN4_PORTREG)), "I" (CORE_PIN4_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 5
		"rjmp	_digitalWrite_OC0B"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 6
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 7
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 8
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 9
		"rjmp	_digitalWrite_OC3A"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN5_PORTREG)), "I" (CORE_PIN5_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN6_PORTREG)), "I" (CORE_PIN6_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN7_PORTREG)), "I" (CORE_PIN7_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN8_PORTREG)), "I" (CORE_PIN8_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN9_PORTREG)), "I" (CORE_PIN9_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 10
		"rjmp	_digitalWrite_OC4A"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 11
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 12
		"rjmp	_digitalWrite_OC4D"			"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 13
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 14
		"rjmp	_digitalWrite_OC1A"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN10_PORTREG)), "I" (CORE_PIN10_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN11_PORTREG)), "I" (CORE_PIN11_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN12_PORTREG)), "I" (CORE_PIN12_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN13_PORTREG)), "I" (CORE_PIN13_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN14_PORTREG)), "I" (CORE_PIN14_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 15
		"rjmp	_digitalWrite_OC1B"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 16
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 17
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 18
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 19
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN15_PORTREG)), "I" (CORE_PIN15_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN16_PORTREG)), "I" (CORE_PIN16_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN17_PORTREG)), "I" (CORE_PIN17_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN18_PORTREG)), "I" (CORE_PIN18_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN19_PORTREG)), "I" (CORE_PIN19_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 20
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 21
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 22
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 23
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 24
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN20_PORTREG)), "I" (CORE_PIN20_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN21_PORTREG)), "I" (CORE_PIN21_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN22_PORTREG)), "I" (CORE_PIN22_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN23_PORTREG)), "I" (CORE_PIN23_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN24_PORTREG)), "I" (CORE_PIN24_BIT)
	);
}
void _digitalWrite_LOW_TABLE(void)
{
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 0
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 1
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 2
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 3
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 4
		"rjmp	_digitalWrite_OC1C"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN0_PORTREG)), "I" (CORE_PIN0_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN1_PORTREG)), "I" (CORE_PIN1_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN2_PORTREG)), "I" (CORE_PIN2_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN3_PORTREG)), "I" (CORE_PIN3_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN4_PORTREG)), "I" (CORE_PIN4_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 5
		"rjmp	_digitalWrite_OC0B"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 6
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 7
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 8
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 9
		"rjmp	_digitalWrite_OC3A"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN5_PORTREG)), "I" (CORE_PIN5_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN6_PORTREG)), "I" (CORE_PIN6_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN7_PORTREG)), "I" (CORE_PIN7_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN8_PORTREG)), "I" (CORE_PIN8_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN9_PORTREG)), "I" (CORE_PIN9_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 10
		"rjmp	_digitalWrite_OC4A"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 11
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 12
		"rjmp	_digitalWrite_OC4D"			"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 13
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 14
		"rjmp	_digitalWrite_OC1A"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN10_PORTREG)), "I" (CORE_PIN10_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN11_PORTREG)), "I" (CORE_PIN11_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN12_PORTREG)), "I" (CORE_PIN12_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN13_PORTREG)), "I" (CORE_PIN13_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN14_PORTREG)), "I" (CORE_PIN14_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 15
		"rjmp	_digitalWrite_OC1B"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 16
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 17
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 18
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 19
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN15_PORTREG)), "I" (CORE_PIN15_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN16_PORTREG)), "I" (CORE_PIN16_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN17_PORTREG)), "I" (CORE_PIN17_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN18_PORTREG)), "I" (CORE_PIN18_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN19_PORTREG)), "I" (CORE_PIN19_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 20
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 21
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 22
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 23
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 24
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN20_PORTREG)), "I" (CORE_PIN20_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN21_PORTREG)), "I" (CORE_PIN21_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN22_PORTREG)), "I" (CORE_PIN22_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN23_PORTREG)), "I" (CORE_PIN23_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN24_PORTREG)), "I" (CORE_PIN24_BIT)
	);
}
#elif defined(__AVR_AT90USB162__)
void _digitalWrite_HIGH_TABLE(void)
{
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 0
		"rjmp	_digitalWrite_OC0B"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 1
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 2
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 3
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 4
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN0_PORTREG)), "I" (CORE_PIN0_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN1_PORTREG)), "I" (CORE_PIN1_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN2_PORTREG)), "I" (CORE_PIN2_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN3_PORTREG)), "I" (CORE_PIN3_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN4_PORTREG)), "I" (CORE_PIN4_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 5
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 6
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 7
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 8
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 9
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN5_PORTREG)), "I" (CORE_PIN5_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN6_PORTREG)), "I" (CORE_PIN6_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN7_PORTREG)), "I" (CORE_PIN7_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN8_PORTREG)), "I" (CORE_PIN8_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN9_PORTREG)), "I" (CORE_PIN9_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 10
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 11
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 12
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 13
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 14
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN10_PORTREG)), "I" (CORE_PIN10_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN11_PORTREG)), "I" (CORE_PIN11_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN12_PORTREG)), "I" (CORE_PIN12_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN13_PORTREG)), "I" (CORE_PIN13_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN14_PORTREG)), "I" (CORE_PIN14_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 15
		"rjmp	_digitalWrite_OC1C"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 16
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 17
		"rjmp	_digitalWrite_OC1A"			"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 18
		"rjmp	_digitalWrite_OC1B"			"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 19
		"ret"						"\n\t"
		"sbi	%10, %11"				"\n\t"	// pin 20
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN15_PORTREG)), "I" (CORE_PIN15_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN16_PORTREG)), "I" (CORE_PIN16_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN17_PORTREG)), "I" (CORE_PIN17_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN18_PORTREG)), "I" (CORE_PIN18_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN19_PORTREG)), "I" (CORE_PIN19_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN20_PORTREG)), "I" (CORE_PIN20_BIT)
	);
}
void _digitalWrite_LOW_TABLE(void)
{
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 0
		"rjmp	_digitalWrite_OC0B"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 1
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 2
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 3
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 4
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN0_PORTREG)), "I" (CORE_PIN0_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN1_PORTREG)), "I" (CORE_PIN1_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN2_PORTREG)), "I" (CORE_PIN2_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN3_PORTREG)), "I" (CORE_PIN3_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN4_PORTREG)), "I" (CORE_PIN4_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 5
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 6
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 7
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 8
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 9
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN5_PORTREG)), "I" (CORE_PIN5_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN6_PORTREG)), "I" (CORE_PIN6_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN7_PORTREG)), "I" (CORE_PIN7_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN8_PORTREG)), "I" (CORE_PIN8_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN9_PORTREG)), "I" (CORE_PIN9_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 10
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 11
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 12
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 13
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 14
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN10_PORTREG)), "I" (CORE_PIN10_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN11_PORTREG)), "I" (CORE_PIN11_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN12_PORTREG)), "I" (CORE_PIN12_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN13_PORTREG)), "I" (CORE_PIN13_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN14_PORTREG)), "I" (CORE_PIN14_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 15
		"rjmp	_digitalWrite_OC1C"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 16
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 17
		"rjmp	_digitalWrite_OC1A"			"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 18
		"rjmp	_digitalWrite_OC1B"			"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 19
		"ret"						"\n\t"
		"cbi	%10, %11"				"\n\t"	// pin 20
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN15_PORTREG)), "I" (CORE_PIN15_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN16_PORTREG)), "I" (CORE_PIN16_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN17_PORTREG)), "I" (CORE_PIN17_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN18_PORTREG)), "I" (CORE_PIN18_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN19_PORTREG)), "I" (CORE_PIN19_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN20_PORTREG)), "I" (CORE_PIN20_BIT)
	);
}



#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
void _digitalWrite_HIGH_TABLE(void)
{
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 0
		"rjmp	_digitalWrite_OC0B"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 1
		"rjmp	_digitalWrite_OC2B"			"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 2
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 3
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 4
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN0_PORTREG)), "I" (CORE_PIN0_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN1_PORTREG)), "I" (CORE_PIN1_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN2_PORTREG)), "I" (CORE_PIN2_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN3_PORTREG)), "I" (CORE_PIN3_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN4_PORTREG)), "I" (CORE_PIN4_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 5
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 6
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 7
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 8
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 9
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN5_PORTREG)), "I" (CORE_PIN5_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN6_PORTREG)), "I" (CORE_PIN6_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN7_PORTREG)), "I" (CORE_PIN7_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN8_PORTREG)), "I" (CORE_PIN8_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN9_PORTREG)), "I" (CORE_PIN9_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 10
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 11
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 12
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 13
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 14
		"rjmp	_digitalWrite_OC3C"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN10_PORTREG)), "I" (CORE_PIN10_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN11_PORTREG)), "I" (CORE_PIN11_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN12_PORTREG)), "I" (CORE_PIN12_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN13_PORTREG)), "I" (CORE_PIN13_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN14_PORTREG)), "I" (CORE_PIN14_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 15
		"rjmp	_digitalWrite_OC3B"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 16
		"rjmp	_digitalWrite_OC3A"			"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 17
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 18
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 19
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN15_PORTREG)), "I" (CORE_PIN15_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN16_PORTREG)), "I" (CORE_PIN16_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN17_PORTREG)), "I" (CORE_PIN17_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN18_PORTREG)), "I" (CORE_PIN18_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN19_PORTREG)), "I" (CORE_PIN19_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 20
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 21
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 22
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 23
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 24
		"rjmp	_digitalWrite_OC2A"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN20_PORTREG)), "I" (CORE_PIN20_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN21_PORTREG)), "I" (CORE_PIN21_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN22_PORTREG)), "I" (CORE_PIN22_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN23_PORTREG)), "I" (CORE_PIN23_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN24_PORTREG)), "I" (CORE_PIN24_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 25
		"rjmp	_digitalWrite_OC1A"			"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 26
		"rjmp	_digitalWrite_OC1B"			"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 27
		"rjmp	_digitalWrite_OC1C"			"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 28
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 29
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN25_PORTREG)), "I" (CORE_PIN25_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN26_PORTREG)), "I" (CORE_PIN26_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN27_PORTREG)), "I" (CORE_PIN27_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN28_PORTREG)), "I" (CORE_PIN28_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN29_PORTREG)), "I" (CORE_PIN29_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 30
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 31
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 32
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 33
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 34
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN30_PORTREG)), "I" (CORE_PIN30_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN31_PORTREG)), "I" (CORE_PIN31_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN32_PORTREG)), "I" (CORE_PIN32_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN33_PORTREG)), "I" (CORE_PIN33_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN34_PORTREG)), "I" (CORE_PIN34_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 35
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 36
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 37
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 38
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 39
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN35_PORTREG)), "I" (CORE_PIN35_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN36_PORTREG)), "I" (CORE_PIN36_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN37_PORTREG)), "I" (CORE_PIN37_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN38_PORTREG)), "I" (CORE_PIN38_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN39_PORTREG)), "I" (CORE_PIN39_BIT)
	);
	asm volatile (
		"sbi	%0, %1"					"\n\t"	// pin 40
		"ret"						"\n\t"
		"sbi	%2, %3"					"\n\t"	// pin 41
		"ret"						"\n\t"
		"sbi	%4, %5"					"\n\t"	// pin 42
		"ret"						"\n\t"
		"sbi	%6, %7"					"\n\t"	// pin 43
		"ret"						"\n\t"
		"sbi	%8, %9"					"\n\t"	// pin 44
		"ret"						"\n\t"
		"sbi	%10, %11"				"\n\t"	// pin 45
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN40_PORTREG)), "I" (CORE_PIN40_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN41_PORTREG)), "I" (CORE_PIN41_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN42_PORTREG)), "I" (CORE_PIN42_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN43_PORTREG)), "I" (CORE_PIN43_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN44_PORTREG)), "I" (CORE_PIN44_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN45_PORTREG)), "I" (CORE_PIN45_BIT)
	);
}
void _digitalWrite_LOW_TABLE(void)
{
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 0
		"rjmp	_digitalWrite_OC0B"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 1
		"rjmp	_digitalWrite_OC2B"			"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 2
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 3
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 4
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN0_PORTREG)), "I" (CORE_PIN0_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN1_PORTREG)), "I" (CORE_PIN1_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN2_PORTREG)), "I" (CORE_PIN2_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN3_PORTREG)), "I" (CORE_PIN3_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN4_PORTREG)), "I" (CORE_PIN4_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 5
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 6
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 7
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 8
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 9
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN5_PORTREG)), "I" (CORE_PIN5_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN6_PORTREG)), "I" (CORE_PIN6_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN7_PORTREG)), "I" (CORE_PIN7_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN8_PORTREG)), "I" (CORE_PIN8_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN9_PORTREG)), "I" (CORE_PIN9_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 10
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 11
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 12
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 13
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 14
		"rjmp	_digitalWrite_OC3C"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN10_PORTREG)), "I" (CORE_PIN10_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN11_PORTREG)), "I" (CORE_PIN11_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN12_PORTREG)), "I" (CORE_PIN12_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN13_PORTREG)), "I" (CORE_PIN13_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN14_PORTREG)), "I" (CORE_PIN14_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 15
		"rjmp	_digitalWrite_OC3B"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 16
		"rjmp	_digitalWrite_OC3A"			"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 17
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 18
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 19
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN15_PORTREG)), "I" (CORE_PIN15_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN16_PORTREG)), "I" (CORE_PIN16_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN17_PORTREG)), "I" (CORE_PIN17_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN18_PORTREG)), "I" (CORE_PIN18_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN19_PORTREG)), "I" (CORE_PIN19_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 20
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 21
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 22
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 23
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 24
		"rjmp	_digitalWrite_OC2A"			"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN20_PORTREG)), "I" (CORE_PIN20_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN21_PORTREG)), "I" (CORE_PIN21_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN22_PORTREG)), "I" (CORE_PIN22_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN23_PORTREG)), "I" (CORE_PIN23_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN24_PORTREG)), "I" (CORE_PIN24_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 25
		"rjmp	_digitalWrite_OC1A"			"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 26
		"rjmp	_digitalWrite_OC1B"			"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 27
		"rjmp	_digitalWrite_OC1C"			"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 28
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 29
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN25_PORTREG)), "I" (CORE_PIN25_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN26_PORTREG)), "I" (CORE_PIN26_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN27_PORTREG)), "I" (CORE_PIN27_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN28_PORTREG)), "I" (CORE_PIN28_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN29_PORTREG)), "I" (CORE_PIN29_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 30
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 31
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 32
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 33
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 34
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN30_PORTREG)), "I" (CORE_PIN30_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN31_PORTREG)), "I" (CORE_PIN31_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN32_PORTREG)), "I" (CORE_PIN32_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN33_PORTREG)), "I" (CORE_PIN33_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN34_PORTREG)), "I" (CORE_PIN34_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 35
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 36
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 37
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 38
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 39
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN35_PORTREG)), "I" (CORE_PIN35_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN36_PORTREG)), "I" (CORE_PIN36_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN37_PORTREG)), "I" (CORE_PIN37_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN38_PORTREG)), "I" (CORE_PIN38_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN39_PORTREG)), "I" (CORE_PIN39_BIT)
	);
	asm volatile (
		"cbi	%0, %1"					"\n\t"	// pin 40
		"ret"						"\n\t"
		"cbi	%2, %3"					"\n\t"	// pin 41
		"ret"						"\n\t"
		"cbi	%4, %5"					"\n\t"	// pin 42
		"ret"						"\n\t"
		"cbi	%6, %7"					"\n\t"	// pin 43
		"ret"						"\n\t"
		"cbi	%8, %9"					"\n\t"	// pin 44
		"ret"						"\n\t"
		"cbi	%10, %11"				"\n\t"	// pin 45
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN40_PORTREG)), "I" (CORE_PIN40_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN41_PORTREG)), "I" (CORE_PIN41_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN42_PORTREG)), "I" (CORE_PIN42_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN43_PORTREG)), "I" (CORE_PIN43_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN44_PORTREG)), "I" (CORE_PIN44_BIT),
		   "I" (_SFR_IO_ADDR(CORE_PIN45_PORTREG)), "I" (CORE_PIN45_BIT)
	);
}
#endif

void _digitalWrite_OC0B(void)
{
	asm volatile (
		"in	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"out	%0, r30"				"\n\t"
		"ret"
		:: "I" (_SFR_IO_ADDR(TCCR0A)), "I" (COM0B1)
	);
}

void _digitalWrite_OC1A(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR1A), "I" (COM1A1)
	);
}

void _digitalWrite_OC1B(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR1A), "I" (COM1B1)
	);
}

void _digitalWrite_OC1C(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR1A), "I" (COM1C1)
	);
}

#if defined(__AVR_ATmega32U4__)
void _digitalWrite_OC3A(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR3A), "I" (COM3A1)
	);
}

void _digitalWrite_OC4A(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR4A), "I" (COM4A1)
	);
}

void _digitalWrite_OC4D(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR4C), "I" (COM4D1)
	);
}
#endif
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
void _digitalWrite_OC2A(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR2A), "I" (COM3A1)
	);
}

void _digitalWrite_OC2B(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR2A), "I" (COM3B1)
	);
}

void _digitalWrite_OC3A(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR3A), "I" (COM3A1)
	);
}

void _digitalWrite_OC3B(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR3A), "I" (COM3B1)
	);
}

void _digitalWrite_OC3C(void)
{
	asm volatile (
		"lds	r30, %0"				"\n\t"
		"andi	r30, ~(1 << %1)"			"\n\t"
		"sts	%0, r30"				"\n\t"
		"ret"
		:: "M" (&TCCR3A), "I" (COM3C1)
	);
}

#endif

void _digitalRead(void) __attribute__((naked));
void _digitalRead_false(void) __attribute__((naked));
void _digitalRead_true(void) __attribute__((naked));
void _digitalRead_TABLE(void) __attribute__((naked));
void _digitalRead_true2(void) __attribute__((naked));
void _digitalRead_TABLE2(void) __attribute__((naked));

void _digitalRead(void)
{
	asm volatile (
		#if !defined(DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK)
		"cpi	r30, %0"					"\n\t"
		"brsh	_digitalRead_false"				"\n\t"
		#endif
		"lsl	r30"						"\n\t"
		"lsl	r30"						"\n\t"
		"clr	r31"						"\n\t"
		#if CORE_NUM_TOTAL_PINS > 25
		"cpi	r30, 124"					"\n\t"
		"brsh	L%=2"						"\n\t"
		#endif
		"subi	r30, lo8(-(pm(_digitalRead_TABLE)))"		"\n\t"
		"sbci	r31, hi8(-(pm(_digitalRead_TABLE)))"		"\n\t"
		"ijmp"							"\n"
		#if CORE_NUM_TOTAL_PINS > 25
		"L%=2:"							"\n\t"
		"subi	r30, lo8(-(pm(_digitalRead_TABLE2 - 248)))"	"\n\t"
		"sbci	r31, hi8(-(pm(_digitalRead_TABLE2 - 248)))"	"\n\t"
		"ijmp"							"\n"
		#endif
		: : "M" (CORE_NUM_TOTAL_PINS)
	);
}

#if !defined(DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK)
void _digitalRead_false(void)
{
	asm volatile (
		"clr	r30"						"\n\t"
		"ret"							"\n"
	);
}
#endif

void _digitalRead_true(void)
{
	asm volatile (
		"ldi	r30, 1"						"\n\t"
		"ret"							"\n"
	);
}

void _digitalRead_TABLE(void)
{
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 0
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 1
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 2
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 3
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %8"				"\n\t"	// pin 4
		"andi	r30, %9"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN0_PINREG)), "M" (CORE_PIN0_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN1_PINREG)), "M" (CORE_PIN1_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN2_PINREG)), "M" (CORE_PIN2_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN3_PINREG)), "M" (CORE_PIN3_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN4_PINREG)), "M" (CORE_PIN4_BITMASK)
	);
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 5
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 6
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 7
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 8
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %8"				"\n\t"	// pin 9
		"andi	r30, %9"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN5_PINREG)), "M" (CORE_PIN5_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN6_PINREG)), "M" (CORE_PIN6_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN7_PINREG)), "M" (CORE_PIN7_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN8_PINREG)), "M" (CORE_PIN8_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN9_PINREG)), "M" (CORE_PIN9_BITMASK)
	);
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 10
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 11
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 12
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 13
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %8"				"\n\t"	// pin 14
		"andi	r30, %9"				"\n\t"
		"brne	_digitalRead_true"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN10_PINREG)), "M" (CORE_PIN10_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN11_PINREG)), "M" (CORE_PIN11_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN12_PINREG)), "M" (CORE_PIN12_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN13_PINREG)), "M" (CORE_PIN13_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN14_PINREG)), "M" (CORE_PIN14_BITMASK)
	);
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 15
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 16
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 17
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 18
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %8"				"\n\t"	// pin 19
		"andi	r30, %9"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %10"				"\n\t"	// pin 20
		"andi	r30, %11"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN15_PINREG)), "M" (CORE_PIN15_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN16_PINREG)), "M" (CORE_PIN16_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN17_PINREG)), "M" (CORE_PIN17_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN18_PINREG)), "M" (CORE_PIN18_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN19_PINREG)), "M" (CORE_PIN19_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN20_PINREG)), "M" (CORE_PIN20_BITMASK)
	);
	#if CORE_NUM_TOTAL_PINS > 21
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 21
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 22
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 23
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 24
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN21_PINREG)), "M" (CORE_PIN21_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN22_PINREG)), "M" (CORE_PIN22_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN23_PINREG)), "M" (CORE_PIN23_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN24_PINREG)), "M" (CORE_PIN24_BITMASK)
	);
	#endif
	#if CORE_NUM_TOTAL_PINS > 25
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 25
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 26
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 27
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 28
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %8"				"\n\t"	// pin 29
		"andi	r30, %9"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %10"				"\n\t"	// pin 30
		"andi	r30, %11"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN25_PINREG)), "M" (CORE_PIN25_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN26_PINREG)), "M" (CORE_PIN26_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN27_PINREG)), "M" (CORE_PIN27_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN28_PINREG)), "M" (CORE_PIN28_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN29_PINREG)), "M" (CORE_PIN29_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN30_PINREG)), "M" (CORE_PIN30_BITMASK)
	);
	#endif
}

void _digitalRead_true2(void)
{
	asm volatile (
		"ldi	r30, 1"						"\n\t"
		"ret"							"\n"
	);
}

#if CORE_NUM_TOTAL_PINS > 25
void _digitalRead_TABLE2(void)
{
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 31
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 32
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 33
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 34
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN31_PINREG)), "M" (CORE_PIN31_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN32_PINREG)), "M" (CORE_PIN32_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN33_PINREG)), "M" (CORE_PIN33_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN34_PINREG)), "M" (CORE_PIN34_BITMASK)
	);
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 35
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 36
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 37
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 38
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %8"				"\n\t"	// pin 39
		"andi	r30, %9"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN35_PINREG)), "M" (CORE_PIN35_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN36_PINREG)), "M" (CORE_PIN36_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN37_PINREG)), "M" (CORE_PIN37_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN38_PINREG)), "M" (CORE_PIN38_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN39_PINREG)), "M" (CORE_PIN39_BITMASK)
	);
	asm volatile (
		"in	r30, %0"				"\n\t"	// pin 40
		"andi	r30, %1"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %2"				"\n\t"	// pin 41
		"andi	r30, %3"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %4"				"\n\t"	// pin 42
		"andi	r30, %5"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %6"				"\n\t"	// pin 43
		"andi	r30, %7"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %8"				"\n\t"	// pin 44
		"andi	r30, %9"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		"in	r30, %10"				"\n\t"	// pin 45
		"andi	r30, %11"				"\n\t"
		"brne	_digitalRead_true2"			"\n\t"
		"ret"						"\n\t"
		:: "I" (_SFR_IO_ADDR(CORE_PIN40_PINREG)), "M" (CORE_PIN40_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN41_PINREG)), "M" (CORE_PIN41_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN42_PINREG)), "M" (CORE_PIN42_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN43_PINREG)), "M" (CORE_PIN43_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN44_PINREG)), "M" (CORE_PIN44_BITMASK),
		   "I" (_SFR_IO_ADDR(CORE_PIN45_PINREG)), "M" (CORE_PIN45_BITMASK)
	);
}
#endif



const uint8_t PROGMEM digital_pin_table_PGM[] = {
	CORE_PIN0_BITMASK,	(int)&CORE_PIN0_PINREG,
	CORE_PIN1_BITMASK,	(int)&CORE_PIN1_PINREG,
	CORE_PIN2_BITMASK,	(int)&CORE_PIN2_PINREG,
	CORE_PIN3_BITMASK,	(int)&CORE_PIN3_PINREG,
	CORE_PIN4_BITMASK,	(int)&CORE_PIN4_PINREG,
	CORE_PIN5_BITMASK,	(int)&CORE_PIN5_PINREG,
	CORE_PIN6_BITMASK,	(int)&CORE_PIN6_PINREG,
	CORE_PIN7_BITMASK,	(int)&CORE_PIN7_PINREG,
	CORE_PIN8_BITMASK,	(int)&CORE_PIN8_PINREG,
	CORE_PIN9_BITMASK,	(int)&CORE_PIN9_PINREG,
	CORE_PIN10_BITMASK,	(int)&CORE_PIN10_PINREG,
	CORE_PIN11_BITMASK,	(int)&CORE_PIN11_PINREG,
	CORE_PIN12_BITMASK,	(int)&CORE_PIN12_PINREG,
	CORE_PIN13_BITMASK,	(int)&CORE_PIN13_PINREG,
	CORE_PIN14_BITMASK,	(int)&CORE_PIN14_PINREG,
	CORE_PIN15_BITMASK,	(int)&CORE_PIN15_PINREG,
	CORE_PIN16_BITMASK,	(int)&CORE_PIN16_PINREG,
	CORE_PIN17_BITMASK,	(int)&CORE_PIN17_PINREG,
	CORE_PIN18_BITMASK,	(int)&CORE_PIN18_PINREG,
	CORE_PIN19_BITMASK,	(int)&CORE_PIN19_PINREG,
	CORE_PIN20_BITMASK,	(int)&CORE_PIN20_PINREG,
	#if CORE_NUM_TOTAL_PINS > 21
	CORE_PIN21_BITMASK,	(int)&CORE_PIN21_PINREG,
	CORE_PIN22_BITMASK,	(int)&CORE_PIN22_PINREG,
	CORE_PIN23_BITMASK,	(int)&CORE_PIN23_PINREG,
	CORE_PIN24_BITMASK,	(int)&CORE_PIN24_PINREG,
	#endif
	#if CORE_NUM_TOTAL_PINS > 25
	CORE_PIN25_BITMASK,	(int)&CORE_PIN25_PINREG,
	CORE_PIN26_BITMASK,	(int)&CORE_PIN26_PINREG,
	CORE_PIN27_BITMASK,	(int)&CORE_PIN27_PINREG,
	CORE_PIN28_BITMASK,	(int)&CORE_PIN28_PINREG,
	CORE_PIN29_BITMASK,	(int)&CORE_PIN29_PINREG,
	CORE_PIN30_BITMASK,	(int)&CORE_PIN30_PINREG,
	CORE_PIN31_BITMASK,	(int)&CORE_PIN31_PINREG,
	CORE_PIN32_BITMASK,	(int)&CORE_PIN32_PINREG,
	CORE_PIN33_BITMASK,	(int)&CORE_PIN33_PINREG,
	CORE_PIN34_BITMASK,	(int)&CORE_PIN34_PINREG,
	CORE_PIN35_BITMASK,	(int)&CORE_PIN35_PINREG,
	CORE_PIN36_BITMASK,	(int)&CORE_PIN36_PINREG,
	CORE_PIN37_BITMASK,	(int)&CORE_PIN37_PINREG,
	CORE_PIN38_BITMASK,	(int)&CORE_PIN38_PINREG,
	CORE_PIN39_BITMASK,	(int)&CORE_PIN39_PINREG,
	CORE_PIN40_BITMASK,	(int)&CORE_PIN40_PINREG,
	CORE_PIN41_BITMASK,	(int)&CORE_PIN41_PINREG,
	CORE_PIN42_BITMASK,	(int)&CORE_PIN42_PINREG,
	CORE_PIN43_BITMASK,	(int)&CORE_PIN43_PINREG,
	CORE_PIN44_BITMASK,	(int)&CORE_PIN44_PINREG,
	CORE_PIN45_BITMASK,	(int)&CORE_PIN45_PINREG
        #endif
};

void _shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
{
	if (bitOrder == LSBFIRST) {
		shiftOut_lsbFirst(dataPin, clockPin, value);
	} else {
		shiftOut_msbFirst(dataPin, clockPin, value);
	}
}

void shiftOut_lsbFirst(uint8_t dataPin, uint8_t clockPin, uint8_t value)
{
	uint8_t mask;
	for (mask=0x01; mask; mask <<= 1) {
		digitalWrite(dataPin, value & mask);
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

void shiftOut_msbFirst(uint8_t dataPin, uint8_t clockPin, uint8_t value)
{
	uint8_t mask;
	for (mask=0x80; mask; mask >>= 1) {
		digitalWrite(dataPin, value & mask);
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

uint8_t _shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
	if (bitOrder == LSBFIRST) {
		return shiftIn_lsbFirst(dataPin, clockPin);
	} else {
		return shiftIn_msbFirst(dataPin, clockPin);
	}
}

uint8_t shiftIn_lsbFirst(uint8_t dataPin, uint8_t clockPin)
{
	uint8_t mask, value=0;
	for (mask=0x01; mask; mask <<= 1) {
		digitalWrite(clockPin, HIGH);
		if (digitalRead(dataPin)) value |= mask;
		digitalWrite(clockPin, LOW);
	}
	return value;
}

uint8_t shiftIn_msbFirst(uint8_t dataPin, uint8_t clockPin)
{
	uint8_t mask, value=0;
	for (mask=0x80; mask; mask >>= 1) {
		digitalWrite(clockPin, HIGH);
		if (digitalRead(dataPin)) value |= mask;
		digitalWrite(clockPin, LOW);
	}
	return value;
}

static void disable_peripherals(void) __attribute__((noinline));
static void disable_peripherals(void)
{
	#if defined(__AVR_AT90USB162__)
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0;
	TIMSK0 = 0; TIMSK1 = 0; UCSR1B = 0;
	DDRB = 0; DDRC = 0; DDRD = 0;
	PORTB = 0; PORTC = 0; PORTD = 0;
	#elif defined(__AVR_ATmega32U4__)
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
	TIMSK0 = 0; TIMSK1 = 0; TIMSK3 = 0; TIMSK4 = 0; UCSR1B = 0; TWCR = 0;
	DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0; TWCR = 0;
	PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
	#elif defined(__AVR_AT90USB646__)
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
	TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;
	DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;
	PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
	#elif defined(__AVR_AT90USB1286__)
	EIMSK = 0; PCICR = 0; SPCR = 0; ACSR = 0; EECR = 0; ADCSRA = 0;
	TIMSK0 = 0; TIMSK1 = 0; TIMSK2 = 0; TIMSK3 = 0; UCSR1B = 0; TWCR = 0;
	DDRA = 0; DDRB = 0; DDRC = 0; DDRD = 0; DDRE = 0; DDRF = 0;
	PORTA = 0; PORTB = 0; PORTC = 0; PORTD = 0; PORTE = 0; PORTF = 0;
	#endif
}


void _reboot_Teensyduino_(void)
{
	cli();
	delayMicroseconds(5000);
	UDCON = 1;
	USBCON = (1<<FRZCLK);
	delayMicroseconds(15000);
	disable_peripherals();
	#if defined(__AVR_AT90USB162__)
	asm volatile("jmp 0x3E00");
	#elif defined(__AVR_ATmega32U4__)
	asm volatile("jmp 0x7E00");
	#elif defined(__AVR_AT90USB646__)
	asm volatile("jmp 0xFC00");
	#elif defined(__AVR_AT90USB1286__)
	asm volatile("jmp 0x1FC00");
	#endif
	//__builtin_unreachable();  // available in gcc 4.5
	while (1) ;
}

void _restart_Teensyduino_(void)
{
	cli();
	disable_peripherals();  // but leave USB intact
	delayMicroseconds(15000);
	asm volatile("jmp 0");
	//__builtin_unreachable();  // available in gcc 4.5
	while (1) ;
}



#if F_CPU == 16000000L
  #define TIMER0_MILLIS_INC  	1
  #define TIMER0_FRACT_INC	3
  #define TIMER0_MICROS_INC  	4
#elif F_CPU == 8000000L
  #define TIMER0_MILLIS_INC  	2
  #define TIMER0_FRACT_INC	6
  #define TIMER0_MICROS_INC  	8
#elif F_CPU == 4000000L
  #define TIMER0_MILLIS_INC  	4
  #define TIMER0_FRACT_INC	12
  #define TIMER0_MICROS_INC  	16
#elif F_CPU == 2000000L
  #define TIMER0_MILLIS_INC  	8
  #define TIMER0_FRACT_INC	24
  #define TIMER0_MICROS_INC  	32
#elif F_CPU == 1000000L
  #define TIMER0_MILLIS_INC  	16
  #define TIMER0_FRACT_INC	48
  #define TIMER0_MICROS_INC  	64
#endif

volatile unsigned long timer0_micros_count = 0;
volatile unsigned long timer0_millis_count = 0;
volatile unsigned char timer0_fract_count = 0;

void TIMER0_OVF_vect() __attribute__((naked));
void TIMER0_OVF_vect()
{
	asm volatile(
		"push	r24"				"\n\t"
		"in	r24, __SREG__"			"\n\t"
		"push	r24"				"\n\t"

		"lds	r24, timer0_fract_count"	"\n\t"
		"subi	r24, 256 - %0"			"\n\t"
		"cpi	r24, 125"			"\n\t"
		"brsh	L_%=_fract_roll"		"\n\t"

	"L_%=_fract_noroll:"				"\n\t"
		"sts	timer0_fract_count, r24"	"\n\t"
		"lds	r24, timer0_millis_count"	"\n\t"
		"subi	r24, 256 - %1"			"\n\t"
		"sts	timer0_millis_count, r24"	"\n\t"
		"brcs	L_%=_ovcount"			"\n\t"

	"L_%=_millis_inc_sext:"
		"lds	r24, timer0_millis_count+1"	"\n\t"
		"sbci	r24, 255"			"\n\t"
		"sts	timer0_millis_count+1, r24"	"\n\t"
		"brcs	L_%=_ovcount"			"\n\t"
		"lds	r24, timer0_millis_count+2"	"\n\t"
		"sbci	r24, 255"			"\n\t"
		"sts	timer0_millis_count+2, r24"	"\n\t"
		"brcs	L_%=_ovcount"			"\n\t"
		"lds	r24, timer0_millis_count+3"	"\n\t"
		"sbci	r24, 255"			"\n\t"
		"sts	timer0_millis_count+3, r24"	"\n\t"
		"rjmp	L_%=_ovcount"			"\n\t"

	"L_%=_fract_roll:"				"\n\t"
		"subi	r24, 125"			"\n\t"
		"sts	timer0_fract_count, r24"	"\n\t"
		"lds	r24, timer0_millis_count"	"\n\t"
		"subi	r24, 256 - %1 - 1"		"\n\t"
		"sts	timer0_millis_count, r24"	"\n\t"
		"brcc	L_%=_millis_inc_sext"		"\n\t"

	"L_%=_ovcount:"
		"lds	r24, timer0_micros_count"	"\n\t"
		"subi	r24, 256 - %2"			"\n\t"
		"sts	timer0_micros_count, r24"	"\n\t"
		"brcs	L_%=_end"			"\n\t"
		"lds	r24, timer0_micros_count+1"	"\n\t"
		"sbci	r24, 255"			"\n\t"
		"sts	timer0_micros_count+1, r24"	"\n\t"
		"brcs	L_%=_end"			"\n\t"
		"lds	r24, timer0_micros_count+2"	"\n\t"
		"sbci	r24, 255"			"\n\t"
		"sts	timer0_micros_count+2, r24"	"\n\t"

	"L_%=_end:"
		"pop	r24"				"\n\t"
		"out	__SREG__, r24"			"\n\t"
		"pop	r24"				"\n\t"
		"reti"
		: 
		: "M" (TIMER0_FRACT_INC), "M" (TIMER0_MILLIS_INC),
		  "M" (TIMER0_MICROS_INC)
	);
}


void delay(uint32_t ms)
{
	uint16_t start = (uint16_t)micros();

	while (ms > 0) {
		if (((uint16_t)micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
#if 0
	// This doesn't save a lot of power on Teensy, which
	// lacks the power saving flash memory of some newer
	// chips, and also usually consumes lots of power for
	// the USB port.  There is also some strange (probably
	// hardware) bug involving the A/D mux for the first
	// conversion after the processor wakes from idle mode.
	uint32_t start;
	if (!(SREG & 0x80)) {
		// if interrupts are disabled, busy loop
		while (ms--) delayMicroseconds(1000);
		return;
	}
	// if interrupt are enabled, use low power idle mode
	cli();
	start = timer0_millis_count;
	do {
		_SLEEP_CONTROL_REG = SLEEP_MODE_IDLE | _SLEEP_ENABLE_MASK;
		sei();
		sleep_cpu();
		_SLEEP_CONTROL_REG = SLEEP_MODE_IDLE;
		cli();
	} while (timer0_millis_count - start <= ms);
	sei();
#endif
}


uint32_t _micros(void)
{
	register uint32_t out asm("r22");
	asm volatile(
		"in	__tmp_reg__, __SREG__"		"\n\t"
		"cli"					"\n\t"
		"in	%A0, %2"			"\n\t"
		"in	__zero_reg__, %3"		"\n\t"
		"lds	%B0, timer0_micros_count"	"\n\t"
		"lds	%C0, timer0_micros_count+1"	"\n\t"
		"lds	%D0, timer0_micros_count+2"	"\n\t"
		"out	__SREG__, __tmp_reg__"		"\n\t"
		"sbrs	__zero_reg__, %4"		"\n\t"
		"rjmp	L_%=_skip"			"\n\t"
		"cpi	%A0, 255"			"\n\t"
		"breq	L_%=_skip"			"\n\t"
		"subi	%B0, 256 - %1"			"\n\t"
		"sbci	%C0, 255"			"\n\t"
		"sbci	%D0, 255"			"\n\t"
	"L_%=_skip:"
		"clr	__zero_reg__"			"\n\t"
		"clr	__tmp_reg__"			"\n\t"
#if F_CPU == 16000000L || F_CPU == 8000000L || F_CPU == 4000000L
		"lsl	%A0"				"\n\t"
		"rol	__tmp_reg__"			"\n\t"
		"lsl	%A0"				"\n\t"
		"rol	__tmp_reg__"			"\n\t"
#if F_CPU == 8000000L || F_CPU == 4000000L
		"lsl	%A0"				"\n\t"
		"rol	__tmp_reg__"			"\n\t"
#endif
#if F_CPU == 4000000L
		"lsl	%A0"				"\n\t"
		"rol	__tmp_reg__"			"\n\t"
#endif
		"or	%B0, __tmp_reg__"		"\n\t"
#endif
#if F_CPU == 1000000L || F_CPU == 2000000L
		"lsr	%A0"				"\n\t"
		"ror	__tmp_reg__"			"\n\t"
		"lsr	%A0"				"\n\t"
		"ror	__tmp_reg__"			"\n\t"
#if F_CPU == 2000000L
		"lsr	%A0"				"\n\t"
		"ror	__tmp_reg__"			"\n\t"
#endif
		"or	%B0, %A0"			"\n\t"
		"mov	%A0, __tmp_reg__"		"\n\t"
#endif
		: "=d" (out)
		: "M" (TIMER0_MICROS_INC),
		  "I" (_SFR_IO_ADDR(TCNT0)),
		  "I" (_SFR_IO_ADDR(TIFR0)),
		  "I" (TOV0)
		: "r0"
	);
	return out;
}



