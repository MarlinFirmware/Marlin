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
 * For ARDUINO_ARCH_AVR
 */


#ifndef _HAL_AVR_H
#define _HAL_AVR_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "fastio_AVR.h"
#include "watchdog_AVR.h"
#include "math_AVR.h"

#ifdef USBCON
  #include "HardwareSerial.h"
  #if ENABLED(BLUETOOTH)
    #define MYSERIAL bluetoothSerial
  #else
    #define MYSERIAL Serial
  #endif // BLUETOOTH
#else
  #include "MarlinSerial.h"
  #define MYSERIAL customizedSerial
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

#define HAL_TIMER_TYPE unsigned short

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

//extern uint8_t MCUSR;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

//void cli(void);

//void _delay_ms(int delay);

inline void HAL_clear_reset_source(void) { MCUSR = 0; }
inline uint8_t HAL_get_reset_source(void) { return MCUSR; }

int freeMemory(void);

// eeprom
//void eeprom_write_byte(unsigned char *pos, unsigned char value);
//unsigned char eeprom_read_byte(unsigned char *pos);


// timers
#define STEP_TIMER_NUM OCR1A
#define TEMP_TIMER_NUM 0

#define HAL_TIMER_RATE 	((F_CPU) / 8.0)

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  SBI(TIMSK1, OCIE1A)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() CBI(TIMSK1, OCIE1A)

#define ENABLE_TEMPERATURE_INTERRUPT()  SBI(TIMSK0, OCIE0B)
#define DISABLE_TEMPERATURE_INTERRUPT() CBI(TIMSK0, OCIE0B)

//void HAL_timer_start (uint8_t timer_num, uint32_t frequency);
#define HAL_timer_start (timer_num,frequency)

//void HAL_timer_set_count (uint8_t timer_num, uint16_t count);
#define HAL_timer_set_count(timer,count) timer = (count)

//void HAL_timer_isr_prologue (uint8_t timer_num);
#define HAL_timer_isr_prologue(timer_num)

#define HAL_STEP_TIMER_ISR 	ISR(TIMER1_COMPA_vect)
#define HAL_TEMP_TIMER_ISR  ISR(TIMER0_COMPB_vect)


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


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // _HAL_AVR_H

