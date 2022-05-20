/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * HAL for Teensy 4.0 (IMXRT1062DVL6A) / 4.1 (IMXRT1062DVJ6A)
 */

#define CPU_32_BIT

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio.h"
#include "watchdog.h"

#include <stdint.h>
#include <util/atomic.h>

#if HAS_ETHERNET
  #include "../../feature/ethernet.h"
#endif

// ------------------------
// Defines
// ------------------------

#define IS_32BIT_TEENSY 1
#define IS_TEENSY_40_41 1
#ifndef IS_TEENSY40
  #define IS_TEENSY41 1
#endif

#define CPU_ST7920_DELAY_1 600
#define CPU_ST7920_DELAY_2 750
#define CPU_ST7920_DELAY_3 750

#undef sq
#define sq(x) ((x)*(x))

// Don't place string constants in PROGMEM
#undef PSTR
#define PSTR(str) ({static const char *data = (str); &data[0];})

// ------------------------
// Serial ports
// ------------------------

#include "../../core/serial_hook.h"

#define Serial0 Serial
#define _DECLARE_SERIAL(X) \
  typedef ForwardSerial1Class<decltype(Serial##X)> DefaultSerial##X; \
  extern DefaultSerial##X MSerial##X
#define DECLARE_SERIAL(X) _DECLARE_SERIAL(X)

typedef ForwardSerial1Class<decltype(SerialUSB)> USBSerialType;
extern USBSerialType USBSerial;

#define _MSERIAL(X) MSerial##X
#define MSERIAL(X) _MSERIAL(X)

#if SERIAL_PORT == -1
  #define MYSERIAL1 SerialUSB
#elif WITHIN(SERIAL_PORT, 0, 8)
  DECLARE_SERIAL(SERIAL_PORT);
  #define MYSERIAL1 MSERIAL(SERIAL_PORT)
#else
  #error "The required SERIAL_PORT must be from 0 to 8, or -1 for Native USB."
#endif

#ifdef SERIAL_PORT_2
  #if SERIAL_PORT_2 == -1
    #define MYSERIAL2 usbSerial
  #elif SERIAL_PORT_2 == -2
    #define MYSERIAL2 ethernet.telnetClient
  #elif WITHIN(SERIAL_PORT_2, 0, 8)
    #define MYSERIAL2 MSERIAL(SERIAL_PORT_2)
  #else
    #error "SERIAL_PORT_2 must be from 0 to 8, or -1 for Native USB, or -2 for Ethernet."
  #endif
#endif

// ------------------------
// Types
// ------------------------

class libServo;
typedef libServo hal_servo_t;

typedef int8_t pin_t;

// ------------------------
// Interrupts
// ------------------------

#define CRITICAL_SECTION_START()  const bool irqon = !__get_primask(); __disable_irq()
#define CRITICAL_SECTION_END()    if (irqon) __enable_irq()

// ------------------------
// ADC
// ------------------------

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p < 12U) ? (p) + 54U : -1)
#endif

#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10
#define HAL_ADC_FILTERED      // turn off ADC oversampling

//
// Pin Mapping for M42, M43, M226
//
#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

// FastIO
bool is_output(pin_t pin);

// ------------------------
// Free Memory Accessor
// ------------------------

#pragma GCC diagnostic push
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

extern "C" uint32_t freeMemory();

#pragma GCC diagnostic pop

// ------------------------
// MarlinHAL Class
// ------------------------

class MarlinHAL {
public:

  // Earliest possible init, before setup()
  MarlinHAL() {}

  static void init() {}        // Called early in setup()
  static void init_board() {}  // Called less early in setup()
  static void reboot();        // Restart the firmware from 0x0

  // Interrupts
  static bool isr_state() { return !__get_primask(); }
  static void isr_on()  { __enable_irq(); }
  static void isr_off() { __disable_irq(); }

  static void delay_ms(const int ms) { delay(ms); }

  // Tasks, called from idle()
  static void idletask() {}

  // Reset
  static uint8_t get_reset_source();
  static void clear_reset_source();

  // Free SRAM
  static int freeMemory() { return ::freeMemory(); }

  //
  // ADC Methods
  //

  static int8_t adc_select;

  // Called by Temperature::init once at startup
  static void adc_init();

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const pin_t pin) {}

  // Begin ADC sampling on the given pin. Called from Temperature::isr!
  static void adc_start(const pin_t pin);

  // Is the ADC ready for reading?
  static bool adc_ready() { return true; }

  // The current value of the ADC register
  static uint16_t adc_value();

  /**
   * Set the PWM duty cycle for the pin to the given value.
   * No option to invert the duty cycle [default = false]
   * No option to change the scale of the provided value to enable finer PWM duty control [default = 255]
   */
  static void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t=255, const bool=false) {
    analogWrite(pin, v);
  }

};
