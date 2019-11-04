/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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
 *
 */
#pragma once

/**
 * HAL_LPC1768/HAL.h
 * Hardware Abstraction Layer for NXP LPC1768
 */

#define CPU_32_BIT

void HAL_init();

#include <stdint.h>
#include <stdarg.h>
#include <algorithm>

extern "C" volatile uint32_t _millis;

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "watchdog.h"
#include "timers.h"
#include "MarlinSerial.h"

#include <adc.h>
#include <pinmapping.h>
#include <CDCSerial.h>

//
// Default graphical display delays
//
#ifndef ST7920_DELAY_1
  #define ST7920_DELAY_1 DELAY_NS(600)
#endif
#ifndef ST7920_DELAY_2
  #define ST7920_DELAY_2 DELAY_NS(750)
#endif
#ifndef ST7920_DELAY_3
  #define ST7920_DELAY_3 DELAY_NS(750)
#endif

#if !WITHIN(SERIAL_PORT, -1, 3)
  #error "SERIAL_PORT must be from -1 to 3"
#endif

#if SERIAL_PORT == -1
  #define MYSERIAL0 UsbSerial
#elif SERIAL_PORT == 0
  #define MYSERIAL0 MSerial
#elif SERIAL_PORT == 1
  #define MYSERIAL0 MSerial1
#elif SERIAL_PORT == 2
  #define MYSERIAL0 MSerial2
#elif SERIAL_PORT == 3
  #define MYSERIAL0 MSerial3
#endif

#ifdef SERIAL_PORT_2
  #if !WITHIN(SERIAL_PORT_2, -1, 3)
    #error "SERIAL_PORT_2 must be from -1 to 3"
  #elif SERIAL_PORT_2 == SERIAL_PORT
    #error "SERIAL_PORT_2 must be different than SERIAL_PORT"
  #endif
  #define NUM_SERIAL 2
  #if SERIAL_PORT_2 == -1
    #define MYSERIAL1 UsbSerial
  #elif SERIAL_PORT_2 == 0
    #define MYSERIAL1 MSerial
  #elif SERIAL_PORT_2 == 1
    #define MYSERIAL1 MSerial1
  #elif SERIAL_PORT_2 == 2
    #define MYSERIAL1 MSerial2
  #elif SERIAL_PORT_2 == 3
    #define MYSERIAL1 MSerial3
  #endif
#else
  #define NUM_SERIAL 1
#endif

//
// Interrupts
//
#define CRITICAL_SECTION_START  uint32_t primask = __get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END    if (!primask) __enable_irq()
#define ISRS_ENABLED() (!__get_PRIMASK())
#define ENABLE_ISRS()  __enable_irq()
#define DISABLE_ISRS() __disable_irq()

//
// Utility functions
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
int freeMemory();
#pragma GCC diagnostic pop

//
// ADC API
//

#define ADC_MEDIAN_FILTER_SIZE (23) // Higher values increase step delay (phase shift),
                                    // (ADC_MEDIAN_FILTER_SIZE + 1) / 2 sample step delay (12 samples @ 500Hz: 24ms phase shift)
                                    // Memory usage per ADC channel (bytes): (6 * ADC_MEDIAN_FILTER_SIZE) + 16
                                    // 8 * ((6 * 23) + 16 ) = 1232 Bytes for 8 channels

#define ADC_LOWPASS_K_VALUE    (2)  // Higher values increase rise time
                                    // Rise time sample delays for 100% signal convergence on full range step
                                    // (1 : 13, 2 : 32, 3 : 67, 4 : 139, 5 : 281, 6 : 565, 7 : 1135, 8 : 2273)
                                    // K = 6, 565 samples, 500Hz sample rate, 1.13s convergence on full range step
                                    // Memory usage per ADC channel (bytes): 4 (32 Bytes for 8 channels)

#define HAL_ADC_RESULT_BITS    12   // 15 bit maximum, raw temperature is stored as int16_t
#define HAL_ADC_FILTERED            // Disable oversampling done in Marlin as ADC values already filtered in HAL
#define HAL_ADC_RESOLUTION     HAL_ADC_RESULT_BITS

using FilteredADC = LPC176x::ADC<ADC_LOWPASS_K_VALUE, ADC_MEDIAN_FILTER_SIZE>;
extern uint32_t HAL_adc_reading;
[[gnu::always_inline]] inline void HAL_start_adc(const pin_t pin) {
  HAL_adc_reading = FilteredADC::read(pin) >> (16 - HAL_ADC_RESULT_BITS); // returns 16bit value, reduce to required bits
}
[[gnu::always_inline]] inline uint16_t HAL_read_adc() {
  return HAL_adc_reading;
}

#define HAL_adc_init()
#define HAL_ANALOG_SELECT(pin) FilteredADC::enable_channel(pin)
#define HAL_START_ADC(pin)     HAL_start_adc(pin)
#define HAL_READ_ADC()         HAL_read_adc()
#define HAL_ADC_READY()        (true)

// Test whether the pin is valid
constexpr bool VALID_PIN(const pin_t pin) {
  return LPC176x::pin_is_valid(pin);
}

// Get the analog index for a digital pin
constexpr int8_t DIGITAL_PIN_TO_ANALOG_PIN(const pin_t pin) {
  return (LPC176x::pin_is_valid(pin) && LPC176x::pin_has_adc(pin)) ? pin : -1;
}

// Return the index of a pin number
constexpr int16_t GET_PIN_MAP_INDEX(const pin_t pin) {
  return LPC176x::pin_index(pin);
}

// Get the pin number at the given index
constexpr pin_t GET_PIN_MAP_PIN(const int16_t index) {
  return LPC176x::pin_index(index);
}

// Parse a G-code word into a pin index
int16_t PARSED_PIN_INDEX(const char code, const int16_t dval);
// P0.6 thru P0.9 are for the onboard SD card
#define HAL_SENSITIVE_PINS P0_06, P0_07, P0_08, P0_09

#define HAL_IDLETASK 1
void HAL_idletask();

#define PLATFORM_M997_SUPPORT
void flashFirmware(int16_t value);

/**
 * set_pwm_frequency
 *  Set the frequency of the timer corresponding to the provided pin
 *  All Hardware PWM pins run at the same frequency and all
 *  Software PWM pins run at the same frequency
 */
void set_pwm_frequency(const pin_t pin, int f_desired);

/**
 * set_pwm_duty
 *  Set the PWM duty cycle of the provided pin to the provided value
 *  Optionally allows inverting the duty cycle [default = false]
 *  Optionally allows changing the maximum size of the provided value to enable finer PWM duty control [default = 255]
 */
void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size=255, const bool invert=false);

// Reset source
void HAL_clear_reset_source(void);
uint8_t HAL_get_reset_source(void);
