/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
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
 */

#ifndef _HAL_AVR_H_
#define _HAL_AVR_H_

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

#include <Arduino.h>

#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "../HAL_SPI.h"
#include "fastio_AVR.h"
#include "watchdog_AVR.h"
#include "math_AVR.h"

#ifdef USBCON
  #include "HardwareSerial.h"
#else
  #include "MarlinSerial.h"
#endif

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

//#define analogInputToDigitalPin(IO) IO

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli()
  #define CRITICAL_SECTION_END    SREG = _sreg
#endif
#define ISRS_ENABLED() TEST(SREG, SREG_I)
#define ENABLE_ISRS()  sei()
#define DISABLE_ISRS() cli()

// On AVR this is in math.h?
//#define square(x) ((x)*(x))

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef uint16_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFF

typedef int8_t pin_t;

#define HAL_SERVO_LIB Servo

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

//extern uint8_t MCUSR;

#define NUM_SERIAL 1

#ifdef USBCON
  #if ENABLED(BLUETOOTH)
    #define MYSERIAL0 bluetoothSerial
  #else
    #define MYSERIAL0 Serial
  #endif
#else
  #define MYSERIAL0 customizedSerial
#endif

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

//void cli(void);

//void _delay_ms(const int delay);

inline void HAL_clear_reset_source(void) { MCUSR = 0; }
inline uint8_t HAL_get_reset_source(void) { return MCUSR; }

extern "C" {
  int freeMemory(void);
}

// timers
#define HAL_TIMER_RATE          ((F_CPU) / 8)    // i.e., 2MHz or 2.5MHz

#define STEP_TIMER_NUM          1
#define TEMP_TIMER_NUM          0
#define PULSE_TIMER_NUM         STEP_TIMER_NUM

#define TEMP_TIMER_FREQUENCY    ((F_CPU) / 64.0 / 256.0)

#define STEPPER_TIMER_RATE      HAL_TIMER_RATE
#define STEPPER_TIMER_PRESCALE  8
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000) // Cannot be of type double

#define PULSE_TIMER_RATE       STEPPER_TIMER_RATE   // frequency of pulse timer
#define PULSE_TIMER_PRESCALE   STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  SBI(TIMSK1, OCIE1A)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() CBI(TIMSK1, OCIE1A)
#define STEPPER_ISR_ENABLED()             TEST(TIMSK1, OCIE1A)

#define ENABLE_TEMPERATURE_INTERRUPT()     SBI(TIMSK0, OCIE0B)
#define DISABLE_TEMPERATURE_INTERRUPT()    CBI(TIMSK0, OCIE0B)
#define TEMPERATURE_ISR_ENABLED()         TEST(TIMSK0, OCIE0B)

FORCE_INLINE void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  UNUSED(frequency);
  switch (timer_num) {
    case STEP_TIMER_NUM:
      // waveform generation = 0100 = CTC
      SET_WGM(1, CTC_OCRnA);

      // output mode = 00 (disconnected)
      SET_COMA(1, NORMAL);

      // Set the timer pre-scaler
      // Generally we use a divider of 8, resulting in a 2MHz timer
      // frequency on a 16MHz MCU. If you are going to change this, be
      // sure to regenerate speed_lookuptable.h with
      // create_speed_lookuptable.py
      SET_CS(1, PRESCALER_8);  //  CS 2 = 1/8 prescaler

      // Init Stepper ISR to 122 Hz for quick starting
      // (F_CPU) / (STEPPER_TIMER_PRESCALE) / frequency
      OCR1A = 0x4000;
      TCNT1 = 0;
      break;

    case TEMP_TIMER_NUM:
      // Use timer0 for temperature measurement
      // Interleave temperature interrupt with millies interrupt
      OCR0B = 128;
      break;
  }
}

#define TIMER_OCR_1             OCR1A
#define TIMER_COUNTER_1         TCNT1

#define TIMER_OCR_0             OCR0A
#define TIMER_COUNTER_0         TCNT0

#define _CAT(a, ...) a ## __VA_ARGS__
#define HAL_timer_set_compare(timer, compare) (_CAT(TIMER_OCR_, timer) = compare)
#define HAL_timer_restrain(timer, interval_ticks) NOLESS(_CAT(TIMER_OCR_, timer), _CAT(TIMER_COUNTER_, timer) + interval_ticks)

#define HAL_timer_get_compare(timer) _CAT(TIMER_OCR_, timer)
#define HAL_timer_get_count(timer) _CAT(TIMER_COUNTER_, timer)

/**
 * On AVR there is no hardware prioritization and preemption of
 * interrupts, so this emulates it. The UART has first priority
 * (otherwise, characters will be lost due to UART overflow).
 * Then: Stepper, Endstops, Temperature, and -finally- all others.
 */
#define HAL_timer_isr_prologue(TIMER_NUM)
#define HAL_timer_isr_epilogue(TIMER_NUM)

/* 18 cycles maximum latency */
#define HAL_STEP_TIMER_ISR \
extern "C" void TIMER1_COMPA_vect (void) __attribute__ ((signal, naked, used, externally_visible)); \
extern "C" void TIMER1_COMPA_vect_bottom (void) asm ("TIMER1_COMPA_vect_bottom") __attribute__ ((used, externally_visible, noinline)); \
void TIMER1_COMPA_vect (void) { \
  __asm__ __volatile__ ( \
    A("push r16")                      /* 2 Save R16 */ \
    A("in r16, __SREG__")              /* 1 Get SREG */ \
    A("push r16")                      /* 2 Save SREG into stack */ \
    A("lds r16, %[timsk0]")            /* 2 Load into R0 the Temperature timer Interrupt mask register */ \
    A("push r16")                      /* 2 Save TIMSK0 into the stack */ \
    A("andi r16,~%[msk0]")             /* 1 Disable the temperature ISR */ \
    A("sts %[timsk0], r16")            /* 2 And set the new value */ \
    A("lds r16, %[timsk1]")            /* 2 Load into R0 the stepper timer Interrupt mask register [TIMSK1] */ \
    A("andi r16,~%[msk1]")             /* 1 Disable the stepper ISR */ \
    A("sts %[timsk1], r16")            /* 2 And set the new value */ \
    A("push r16")                      /* 2 Save TIMSK1 into stack */ \
    A("in r16, 0x3B")                  /* 1 Get RAMPZ register */ \
    A("push r16")                      /* 2 Save RAMPZ into stack */ \
    A("in r16, 0x3C")                  /* 1 Get EIND register */ \
    A("push r0")                       /* C runtime can modify all the following registers without restoring them */ \
    A("push r1")                       \
    A("push r18")                      \
    A("push r19")                      \
    A("push r20")                      \
    A("push r21")                      \
    A("push r22")                      \
    A("push r23")                      \
    A("push r24")                      \
    A("push r25")                      \
    A("push r26")                      \
    A("push r27")                      \
    A("push r30")                      \
    A("push r31")                      \
    A("clr r1")                        /* C runtime expects this register to be 0 */ \
    A("call TIMER1_COMPA_vect_bottom") /* Call the bottom handler - No inlining allowed, otherwise registers used are not saved */   \
    A("pop r31")                       \
    A("pop r30")                       \
    A("pop r27")                       \
    A("pop r26")                       \
    A("pop r25")                       \
    A("pop r24")                       \
    A("pop r23")                       \
    A("pop r22")                       \
    A("pop r21")                       \
    A("pop r20")                       \
    A("pop r19")                       \
    A("pop r18")                       \
    A("pop r1")                        \
    A("pop r0")                        \
    A("out 0x3C, r16")                 /* 1 Restore EIND register */ \
    A("pop r16")                       /* 2 Get the original RAMPZ register value */ \
    A("out 0x3B, r16")                 /* 1 Restore RAMPZ register to its original value */ \
    A("pop r16")                       /* 2 Get the original TIMSK1 value but with stepper ISR disabled */ \
    A("ori r16,%[msk1]")               /* 1 Reenable the stepper ISR */ \
    A("cli")                           /* 1 Disable global interrupts - Reenabling Stepper ISR can reenter amd temperature can reenter, and we want that, if it happens, after this ISR has ended */ \
    A("sts %[timsk1], r16")            /* 2 And restore the old value - This reenables the stepper ISR */ \
    A("pop r16")                       /* 2 Get the temperature timer Interrupt mask register [TIMSK0] */ \
    A("sts %[timsk0], r16")            /* 2 And restore the old value - This reenables the temperature ISR */ \
    A("pop r16")                       /* 2 Get the old SREG value */ \
    A("out __SREG__, r16")             /* 1 And restore the SREG value */ \
    A("pop r16")                       /* 2 Restore R16 value */ \
    A("reti")                          /* 4 Return from interrupt */ \
    :                                   \
    : [timsk0] "i" ((uint16_t)&TIMSK0), \
      [timsk1] "i" ((uint16_t)&TIMSK1), \
      [msk0] "M" ((uint8_t)(1<<OCIE0B)),\
      [msk1] "M" ((uint8_t)(1<<OCIE1A)) \
    : \
  ); \
} \
void TIMER1_COMPA_vect_bottom(void)

/* 14 cycles maximum latency */
#define HAL_TEMP_TIMER_ISR \
extern "C" void TIMER0_COMPB_vect (void) __attribute__ ((signal, naked, used, externally_visible)); \
extern "C" void TIMER0_COMPB_vect_bottom(void)  asm ("TIMER0_COMPB_vect_bottom") __attribute__ ((used, externally_visible, noinline)); \
void TIMER0_COMPB_vect (void) { \
  __asm__ __volatile__ ( \
    A("push r16")                       /* 2 Save R16 */ \
    A("in r16, __SREG__")               /* 1 Get SREG */ \
    A("push r16")                       /* 2 Save SREG into stack */ \
    A("lds r16, %[timsk0]")             /* 2 Load into R0 the Temperature timer Interrupt mask register */ \
    A("andi r16,~%[msk0]")              /* 1 Disable the temperature ISR */ \
    A("sts %[timsk0], r16")             /* 2 And set the new value */ \
    A("sei")                            /* 1 Enable global interrupts - It is safe, as the temperature ISR is disabled, so we cannot reenter it */    \
    A("push r16")                       /* 2 Save TIMSK0 into stack */ \
    A("in r16, 0x3B")                   /* 1 Get RAMPZ register */ \
    A("push r16")                       /* 2 Save RAMPZ into stack */ \
    A("in r16, 0x3C")                   /* 1 Get EIND register */ \
    A("push r0")                        /* C runtime can modify all the following registers without restoring them */ \
    A("push r1")                        \
    A("push r18")                       \
    A("push r19")                       \
    A("push r20")                       \
    A("push r21")                       \
    A("push r22")                       \
    A("push r23")                       \
    A("push r24")                       \
    A("push r25")                       \
    A("push r26")                       \
    A("push r27")                       \
    A("push r30")                       \
    A("push r31")                       \
    A("clr r1")                         /* C runtime expects this register to be 0 */ \
    A("call TIMER0_COMPB_vect_bottom")  /* Call the bottom handler - No inlining allowed, otherwise registers used are not saved */   \
    A("pop r31")                        \
    A("pop r30")                        \
    A("pop r27")                        \
    A("pop r26")                        \
    A("pop r25")                        \
    A("pop r24")                        \
    A("pop r23")                        \
    A("pop r22")                        \
    A("pop r21")                        \
    A("pop r20")                        \
    A("pop r19")                        \
    A("pop r18")                        \
    A("pop r1")                         \
    A("pop r0")                         \
    A("out 0x3C, r16")                  /* 1 Restore EIND register */ \
    A("pop r16")                        /* 2 Get the original RAMPZ register value */ \
    A("out 0x3B, r16")                  /* 1 Restore RAMPZ register to its original value */ \
    A("pop r16")                        /* 2 Get the original TIMSK0 value but with temperature ISR disabled */ \
    A("ori r16,%[msk0]")                /* 1 Enable temperature ISR */ \
    A("cli")                            /* 1 Disable global interrupts - We must do this, as we will reenable the temperature ISR, and we don't want to reenter this handler until the current one is done */ \
    A("sts %[timsk0], r16")             /* 2 And restore the old value */ \
    A("pop r16")                        /* 2 Get the old SREG */ \
    A("out __SREG__, r16")              /* 1 And restore the SREG value */ \
    A("pop r16")                        /* 2 Restore R16 */ \
    A("reti")                           /* 4 Return from interrupt */ \
    :                                   \
    : [timsk0] "i"((uint16_t)&TIMSK0),  \
      [msk0] "M" ((uint8_t)(1<<OCIE0B)) \
    : \
  ); \
} \
void TIMER0_COMPB_vect_bottom(void)

// ADC
#ifdef DIDR2
  #define HAL_ANALOG_SELECT(pin) do{ if (pin < 8) SBI(DIDR0, pin); else SBI(DIDR2, pin & 0x07); }while(0)
#else
  #define HAL_ANALOG_SELECT(pin) do{ SBI(DIDR0, pin); }while(0)
#endif

inline void HAL_adc_init(void) {
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | 0x07;
  DIDR0 = 0;
  #ifdef DIDR2
    DIDR2 = 0;
  #endif
}

#define SET_ADMUX_ADCSRA(pin) ADMUX = _BV(REFS0) | (pin & 0x07); SBI(ADCSRA, ADSC)
#ifdef MUX5
  #define HAL_START_ADC(pin) if (pin > 7) ADCSRB = _BV(MUX5); else ADCSRB = 0; SET_ADMUX_ADCSRA(pin)
#else
  #define HAL_START_ADC(pin) ADCSRB = 0; SET_ADMUX_ADCSRA(pin)
#endif

#define HAL_READ_ADC()  ADC
#define HAL_ADC_READY() !TEST(ADCSRA, ADSC)

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#define HAL_SENSITIVE_PINS 0, 1

// AVR compatibility
#define strtof strtod

#endif // _HAL_AVR_H_
