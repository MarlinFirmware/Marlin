/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../shared/Marduino.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "watchdog.h"
#include "math.h"

#ifdef USBCON
  #include <HardwareSerial.h>
#else
  #define HardwareSerial_h // Hack to prevent HardwareSerial.h header inclusion
  #include "MarlinSerial.h"
#endif

#include <stdint.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#ifndef pgm_read_ptr
  // Compatibility for avr-libc 1.8.0-4.1 included with Ubuntu for
  // Windows Subsystem for Linux on Windows 10 as of 10/18/2019
  #define pgm_read_ptr_far(address_long) (void*)__ELPM_word((uint32_t)(address_long))
  #define pgm_read_ptr_near(address_short) (void*)__LPM_word((uint16_t)(address_short))
  #define pgm_read_ptr(address_short) pgm_read_ptr_near(address_short)
#endif

// ------------------------
// Defines
// ------------------------

// AVR PROGMEM extension for sprintf_P
#define S_FMT "%S"

// AVR PROGMEM extension for string define
#define PGMSTR(NAM,STR) const char NAM[] PROGMEM = STR

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START()  unsigned char _sreg = SREG; cli()
  #define CRITICAL_SECTION_END()    SREG = _sreg
#endif
#define ISRS_ENABLED() TEST(SREG, SREG_I)
#define ENABLE_ISRS()  sei()
#define DISABLE_ISRS() cli()

// ------------------------
// Types
// ------------------------

typedef int8_t pin_t;

#define SHARED_SERVOS HAS_SERVOS
#define HAL_SERVO_LIB Servo

// ------------------------
// Public Variables
// ------------------------

//extern uint8_t MCUSR;

// Serial ports
#ifdef USBCON
  #define MYSERIAL0 TERN(BLUETOOTH, bluetoothSerial, Serial)
#else
  #if !WITHIN(SERIAL_PORT, -1, 3)
    #error "SERIAL_PORT must be from -1 to 3. Please update your configuration."
  #endif
  #define MYSERIAL0 customizedSerial1

  #ifdef SERIAL_PORT_2
    #if !WITHIN(SERIAL_PORT_2, -1, 3)
      #error "SERIAL_PORT_2 must be from -1 to 3. Please update your configuration."
    #endif
    #define MYSERIAL1 customizedSerial2
  #endif
#endif

#ifdef MMU2_SERIAL_PORT
  #if !WITHIN(MMU2_SERIAL_PORT, -1, 3)
    #error "MMU2_SERIAL_PORT must be from -1 to 3. Please update your configuration."
  #endif
  #define MMU2_SERIAL mmuSerial
#endif

#ifdef LCD_SERIAL_PORT
  #if !WITHIN(LCD_SERIAL_PORT, -1, 3)
    #error "LCD_SERIAL_PORT must be from -1 to 3. Please update your configuration."
  #endif
  #define LCD_SERIAL lcdSerial
  #if HAS_DGUS_LCD
    #define SERIAL_GET_TX_BUFFER_FREE() LCD_SERIAL.get_tx_buffer_free()
  #endif
#endif

// ------------------------
// Public functions
// ------------------------

void HAL_init();

//void cli();

//void _delay_ms(const int delay);

inline void HAL_clear_reset_source() { MCUSR = 0; }
inline uint8_t HAL_get_reset_source() { return MCUSR; }

inline void HAL_reboot() {}  // reboot the board or restart the bootloader

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

extern "C" int freeMemory();

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic pop
#endif

// ADC
#ifdef DIDR2
  #define HAL_ANALOG_SELECT(ind) do{ if (ind < 8) SBI(DIDR0, ind); else SBI(DIDR2, ind & 0x07); }while(0)
#else
  #define HAL_ANALOG_SELECT(ind) SBI(DIDR0, ind);
#endif

inline void HAL_adc_init() {
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADIF) | 0x07;
  DIDR0 = 0;
  #ifdef DIDR2
    DIDR2 = 0;
  #endif
}

#define SET_ADMUX_ADCSRA(ch) ADMUX = _BV(REFS0) | (ch & 0x07); SBI(ADCSRA, ADSC)
#ifdef MUX5
  #define HAL_START_ADC(ch) if (ch > 7) ADCSRB = _BV(MUX5); else ADCSRB = 0; SET_ADMUX_ADCSRA(ch)
#else
  #define HAL_START_ADC(ch) ADCSRB = 0; SET_ADMUX_ADCSRA(ch)
#endif

#define HAL_ADC_VREF        5.0
#define HAL_ADC_RESOLUTION 10
#define HAL_READ_ADC()  ADC
#define HAL_ADC_READY() !TEST(ADCSRA, ADSC)

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#define HAL_SENSITIVE_PINS 0, 1

#ifdef __AVR_AT90USB1286__
  #define JTAG_DISABLE() do{ MCUCR = 0x80; MCUCR = 0x80; }while(0)
#endif

// AVR compatibility
#define strtof strtod

#define HAL_CAN_SET_PWM_FREQ   // This HAL supports PWM Frequency adjustment

/**
 *  set_pwm_frequency
 *  Sets the frequency of the timer corresponding to the provided pin
 *  as close as possible to the provided desired frequency. Internally
 *  calculates the required waveform generation mode, prescaler and
 *  resolution values required and sets the timer registers accordingly.
 *  NOTE that the frequency is applied to all pins on the timer (Ex OC3A, OC3B and OC3B)
 *  NOTE that there are limitations, particularly if using TIMER2. (see Configuration_adv.h -> FAST FAN PWM Settings)
 */
void set_pwm_frequency(const pin_t pin, int f_desired);

/**
 * set_pwm_duty
 *  Sets the PWM duty cycle of the provided pin to the provided value
 *  Optionally allows inverting the duty cycle [default = false]
 *  Optionally allows changing the maximum size of the provided value to enable finer PWM duty control [default = 255]
 */
void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size=255, const bool invert=false);
