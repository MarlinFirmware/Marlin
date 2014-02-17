#ifndef pins_macros_for_arduino_compatibility_h
#define pins_macros_for_arduino_compatibility_h

#include <avr/pgmspace.h>
#include "core_pins.h"

#if defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
const static uint8_t A0 = CORE_ANALOG0_PIN;
const static uint8_t A1 = CORE_ANALOG1_PIN;
const static uint8_t A2 = CORE_ANALOG2_PIN;
const static uint8_t A3 = CORE_ANALOG3_PIN;
const static uint8_t A4 = CORE_ANALOG4_PIN;
const static uint8_t A5 = CORE_ANALOG5_PIN;
const static uint8_t A6 = CORE_ANALOG6_PIN;
const static uint8_t A7 = CORE_ANALOG7_PIN;
#if defined(__AVR_ATmega32U4__)
const static uint8_t A8 = CORE_ANALOG8_PIN;
const static uint8_t A9 = CORE_ANALOG9_PIN;
const static uint8_t A10 = 10;
const static uint8_t A11 = CORE_ANALOG11_PIN;
#endif
#endif

const static uint8_t SS   = CORE_SS0_PIN;
const static uint8_t MOSI = CORE_MOSI0_PIN;
const static uint8_t MISO = CORE_MISO0_PIN;
const static uint8_t SCK  = CORE_SCLK0_PIN;
const static uint8_t LED_BUILTIN = CORE_LED0_PIN;
#if defined(CORE_SDA0_PIN)
const static uint8_t SDA  = CORE_SDA0_PIN;
#endif
#if defined(CORE_SCL0_PIN)
const static uint8_t SCL  = CORE_SCL0_PIN;
#endif

#define NUM_DIGITAL_PINS                CORE_NUM_TOTAL_PINS
#define NUM_ANALOG_INPUTS               CORE_NUM_ANALOG


// This allows CapSense to work.  Do any libraries
// depend on these to be zero?
#define NOT_A_PORT 127
#define NOT_A_PIN 127

#define digitalPinToPort(P) (P)
#define portInputRegister(P) ((volatile uint8_t *)((int)pgm_read_byte(digital_pin_table_PGM+(P)*2+1)))
#define portModeRegister(P) (portInputRegister(P) + 1)
#define portOutputRegister(P) (portInputRegister(P) + 2)
#define digitalPinToBitMask(P) (pgm_read_byte(digital_pin_table_PGM+(P)*2))
extern const uint8_t PROGMEM digital_pin_table_PGM[];

#if defined(__AVR_AT90USB162__)
#define analogInputToDigitalPin(ch)	(-1)
#define digitalPinHasPWM(p)		((p) == 0 || (p) == 15 || (p) == 17 || (p) == 18)
#elif defined(__AVR_ATmega32U4__)
#define analogInputToDigitalPin(ch)	((ch) <= 10 ? 21 - (ch) : ((ch) == 11 ? 22 : -1))
#define digitalPinHasPWM(p)		((p) == 4 || (p) == 5 || (p) == 9 || (p) == 10 || (p) == 12 || (p) == 14 || (p) == 15)
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define analogInputToDigitalPin(ch)	((ch) <= 7 ? (ch) + 38 : -1)
#define digitalPinHasPWM(p)		(((p) >= 14 && (p) <= 16) || ((p) >= 24 && (p) <= 27) || (p) == 0 || (p) == 1)
#endif

#if defined(__AVR_AT90USB162__)
#define digitalPinToPortReg(p) (((p) <= 7) ? &PORTD : (((p) <= 15) ? &PORTB : &PORTC))
#define digitalPinToBit(p) \
	(((p) <= 7) ? (p) : (((p) <= 15) ? (p) - 8 : (((p) <= 19) ? 23 - (p) : 2)))
#define digitalPinToPCICR(p) \
	((((p) >= 8 && (p) <= 15) || ((p) >= 17 && (p) <= 20) || (p) == 5) ? &PCICR : NULL)
#define digitalPinToPCICRbit(p) (((p) >= 8 && (p) <= 15) ? 0 : 1)
#define digitalPinToPCIFR(p) \
	((((p) >= 8 && (p) <= 15) || ((p) >= 17 && (p) <= 20) || (p) == 5) ? &PCIFR : NULL)
#define digitalPinToPCIFRbit(p)	(((p) >= 8 && (p) <= 15) ? 0 : 1)
#define digitalPinToPCMSK(p) \
	(((p) >= 8 && (p) <= 15) ? &PCMSK0 : ((((p) >= 17 && (p) <= 20) || (p) == 5) ? &PCMSK1 : NULL))
#define digitalPinToPCMSKbit(p) \
	(((p) >= 8 && (p) <= 15) ? (p) - 8 : (((p) >= 17 && (p) <= 20) ? (p) - 17 : 4))

#elif defined(__AVR_ATmega32U4__)
#define digitalPinToPortReg(p) \
	(((p) <= 4) ? &PORTB : (((p) <= 8) ? &PORTD : (((p) <= 10) ? &PORTC : (((p) <= 12) ? &PORTD : \
	(((p) <= 15) ? &PORTB : (((p) <= 21) ? &PORTF : (((p) <= 23) ? &PORTD : &PORTE)))))))
#define digitalPinToBit(p) \
	(((p) <= 3) ? (p) : (((p) == 4) ? 7 : (((p) <= 8) ? (p) - 5 : (((p) <= 10) ? (p) - 3 : \
	(((p) <= 12) ? (p) - 5 : (((p) <= 15) ? (p) - 9 : (((p) <= 19) ? 23 - (p) : \
	(((p) <= 21) ? 21 - (p) : (((p) <= 23) ? (p) - 18 : 6)))))))))
#define digitalPinToPCICR(p)	((((p) >= 0 && (p) <= 4) || ((p) >= 13 && (p) <= 15)) ? &PCICR : NULL)
#define digitalPinToPCICRbit(p)	(0)
#define digitalPinToPCIFR(p)	((((p) >= 0 && (p) <= 4) || ((p) >= 13 && (p) <= 15)) ? &PCIFR : NULL)
#define digitalPinToPCIFRbit(p)	(0)
#define digitalPinToPCMSK(p)	((((p) >= 0 && (p) <= 4) || ((p) >= 13 && (p) <= 15)) ? &PCMSK0 : NULL)
#define digitalPinToPCMSKbit(p) \
	(((p) >= 0 && (p) <= 3) ? (p) : (((p) >= 13 && (p) <= 15) ? (p) - 9 : 7))

#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define digitalPinToPortReg(p) \
	(((p) >= 0 && (p) <= 7) ? &PORTD : (((p) >= 10 && (p) <= 17) ? &PORTC : \
	(((p) >= 20 && (p) <= 27) ? &PORTB : (((p) >= 28 && (p) <= 35) ? &PORTA : \
	(((p) >= 38 && (p) <= 45) ? &PORTF : &PORTE)))))
#define digitalPinToBit(p) \
	(((p) <= 7) ? (p) : (((p) <= 9) ? (p) - 8 : (((p) <= 17) ? (p) - 10 : \
	(((p) <= 19) ? (p) - 12 : (((p) <= 27) ? (p) - 20 : (((p) <= 35) ? (p) - 28 : \
	(((p) <= 37) ? (p) - 32 : (((p) <= 45) ? (p) - 38 : 2))))))))
#define digitalPinToPCICR(p)	(((p) >= 20 && (p) <= 27) ? &PCICR : NULL)
#define digitalPinToPCICRbit(p)	(0)
#define digitalPinToPCIFR(p)	(((p) >= 20 && (p) <= 27) ? &PCIFR : NULL)
#define digitalPinToPCIFRbit(p)	(0)
#define digitalPinToPCMSK(p)	(((p) >= 20 && (p) <= 27) ? &PCMSK0 : NULL)
#define digitalPinToPCMSKbit(p)	(((p) - 20) & 7)
#endif

#define NOT_ON_TIMER 0
static inline uint8_t digitalPinToTimer(uint8_t) __attribute__((always_inline, unused));
static inline uint8_t digitalPinToTimer(uint8_t pin)
{
	switch (pin) {
	#ifdef CORE_PWM0_PIN
	case CORE_PWM0_PIN: return 1;
	#endif
	#ifdef CORE_PWM1_PIN
	case CORE_PWM1_PIN: return 2;
	#endif
	#ifdef CORE_PWM2_PIN
	case CORE_PWM2_PIN: return 3;
	#endif
	#ifdef CORE_PWM3_PIN
	case CORE_PWM3_PIN: return 4;
	#endif
	#ifdef CORE_PWM4_PIN
	case CORE_PWM4_PIN: return 5;
	#endif
	#ifdef CORE_PWM5_PIN
	case CORE_PWM5_PIN: return 6;
	#endif
	#ifdef CORE_PWM6_PIN
	case CORE_PWM6_PIN: return 7;
	#endif
	#ifdef CORE_PWM7_PIN
	case CORE_PWM7_PIN: return 8;
	#endif
	#ifdef CORE_PWM8_PIN
	case CORE_PWM8_PIN: return 9;
	#endif
	default: return NOT_ON_TIMER;
	}
}

#endif
