/* **************************************************************************

 Marlin 3D Printer Firmware
 Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]

 Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
 * Description: HAL for AVR
 *
 * For __AVR__
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
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif


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

// eeprom
//void eeprom_write_byte(unsigned char *pos, unsigned char value);
//unsigned char eeprom_read_byte(unsigned char *pos);


// timers
#define HAL_TIMER_RATE          ((F_CPU) / 8)    // i.e., 2MHz or 2.5MHz
#define HAL_TICKS_PER_US        ((HAL_STEPPER_TIMER_RATE) / 1000000) // Cannot be of type double

#define TEMP_TIMER_FREQUENCY    ((F_CPU) / 64.0 / 256.0)

#define HAL_STEPPER_TIMER_RATE  HAL_TIMER_RATE
#define STEPPER_TIMER_PRESCALE  8

#define STEP_TIMER_NUM          1
#define TIMER_OCR_1             OCR1A
#define TIMER_COUNTER_1         TCNT1

#define TEMP_TIMER_NUM          0
#define TIMER_OCR_0             OCR0A
#define TIMER_COUNTER_0         TCNT0

#define PULSE_TIMER_NUM         TEMP_TIMER_NUM
#define PULSE_TIMER_PRESCALE    8

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  SBI(TIMSK1, OCIE1A)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() CBI(TIMSK1, OCIE1A)
#define STEPPER_ISR_ENABLED() TEST(TIMSK1, OCIE1A)

#define ENABLE_TEMPERATURE_INTERRUPT()  SBI(TIMSK0, OCIE0B)
#define DISABLE_TEMPERATURE_INTERRUPT() CBI(TIMSK0, OCIE0B)

#define HAL_timer_start(timer_num, frequency)

#define _CAT(a, ...) a ## __VA_ARGS__
#define HAL_timer_set_compare(timer, compare) (_CAT(TIMER_OCR_, timer) = compare)
#define HAL_timer_get_compare(timer) _CAT(TIMER_OCR_, timer)
#define HAL_timer_set_count(timer, count) (_CAT(TIMER_COUNTER_, timer) = count)
#define HAL_timer_get_count(timer) _CAT(TIMER_COUNTER_, timer)

#define HAL_timer_isr_prologue(timer_num)

#define HAL_STEP_TIMER_ISR ISR(TIMER1_COMPA_vect)
#define HAL_TEMP_TIMER_ISR ISR(TIMER0_COMPB_vect)

#define HAL_ENABLE_ISRs() do { cli(); if (thermalManager.in_temp_isr) DISABLE_TEMPERATURE_INTERRUPT(); else ENABLE_TEMPERATURE_INTERRUPT(); ENABLE_STEPPER_DRIVER_INTERRUPT(); } while(0)

// ADC
#ifdef DIDR2
  #define HAL_ANALOG_SELECT(pin) do{ if (pin < 8) SBI(DIDR0, pin); else SBI(DIDR2, pin - 8); }while(0)
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

#define HAL_READ_ADC ADC

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#define HAL_SENSITIVE_PINS 0, 1

#endif // _HAL_AVR_H_
