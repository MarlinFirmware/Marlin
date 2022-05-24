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
 * HAL for Arduino Due and compatible (SAM3X8E)
 */

#define CPU_32_BIT

#include "../shared/Marduino.h"
#include "../shared/eeprom_if.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"

#include <stdint.h>

#include "../../core/serial_hook.h"

// ------------------------
// Serial ports
// ------------------------

typedef ForwardSerial1Class< decltype(Serial) > DefaultSerial1;
typedef ForwardSerial1Class< decltype(Serial1) > DefaultSerial2;
typedef ForwardSerial1Class< decltype(Serial2) > DefaultSerial3;
typedef ForwardSerial1Class< decltype(Serial3) > DefaultSerial4;
extern DefaultSerial1 MSerial0;
extern DefaultSerial2 MSerial1;
extern DefaultSerial3 MSerial2;
extern DefaultSerial4 MSerial3;

#define _MSERIAL(X) MSerial##X
#define MSERIAL(X) _MSERIAL(X)

#if SERIAL_PORT == -1 || ENABLED(EMERGENCY_PARSER)
  #define MYSERIAL1 customizedSerial1
#elif WITHIN(SERIAL_PORT, 0, 3)
  #define MYSERIAL1 MSERIAL(SERIAL_PORT)
#else
  #error "The required SERIAL_PORT must be from 0 to 3, or -1 for USB Serial."
#endif

#ifdef SERIAL_PORT_2
  #if SERIAL_PORT_2 == -1 || ENABLED(EMERGENCY_PARSER)
    #define MYSERIAL2 customizedSerial2
  #elif WITHIN(SERIAL_PORT_2, 0, 3)
    #define MYSERIAL2 MSERIAL(SERIAL_PORT_2)
  #else
    #error "SERIAL_PORT_2 must be from 0 to 3, or -1 for USB Serial."
  #endif
#endif

#ifdef SERIAL_PORT_3
  #if SERIAL_PORT_3 == -1 || ENABLED(EMERGENCY_PARSER)
    #define MYSERIAL3 customizedSerial3
  #elif WITHIN(SERIAL_PORT_3, 0, 3)
    #define MYSERIAL3 MSERIAL(SERIAL_PORT_3)
  #else
    #error "SERIAL_PORT_3 must be from 0 to 3, or -1 for USB Serial."
  #endif
#endif

#ifdef MMU2_SERIAL_PORT
  #if WITHIN(MMU2_SERIAL_PORT, 0, 3)
    #define MMU2_SERIAL MSERIAL(MMU2_SERIAL_PORT)
  #else
    #error "MMU2_SERIAL_PORT must be from 0 to 3."
  #endif
#endif

#ifdef LCD_SERIAL_PORT
  #if WITHIN(LCD_SERIAL_PORT, 0, 3)
    #define LCD_SERIAL MSERIAL(LCD_SERIAL_PORT)
  #else
    #error "LCD_SERIAL_PORT must be from 0 to 3."
  #endif
#endif

#include "MarlinSerial.h"
#include "MarlinSerialUSB.h"

// ------------------------
// Types
// ------------------------

typedef int8_t pin_t;

#define SHARED_SERVOS HAS_SERVOS  // Use shared/servos.cpp

class Servo;
typedef Servo hal_servo_t;

//
// Interrupts
//
#define sei() interrupts()
#define cli() noInterrupts()

#define CRITICAL_SECTION_START()  const bool _irqon = hal.isr_state(); hal.isr_off()
#define CRITICAL_SECTION_END()    if (_irqon) hal.isr_on()

//
// ADC
//
#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p < 12U) ? (p) + 54U : -1)
#endif

//
// Pin Mapping for M42, M43, M226
//
#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

//
// Tone
//
void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration=0);
void noTone(const pin_t _pin);

// ------------------------
// Class Utilities
// ------------------------

#pragma GCC diagnostic push
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

#pragma GCC diagnostic pop

#ifdef __cplusplus
  extern "C" {
#endif
char *dtostrf(double __val, signed char __width, unsigned char __prec, char *__s);
#ifdef __cplusplus
  }
#endif

// Return free RAM between end of heap (or end bss) and whatever is current
int freeMemory();

// ------------------------
// MarlinHAL Class
// ------------------------

class MarlinHAL {
public:

  // Earliest possible init, before setup()
  MarlinHAL() {}

  // Watchdog
  static void watchdog_init()    IF_DISABLED(USE_WATCHDOG, {});
  static void watchdog_refresh() IF_DISABLED(USE_WATCHDOG, {});

  static void init();          // Called early in setup()
  static void init_board();    // Called less early in setup()
  static void reboot();        // Restart the firmware

  // Interrupts
  static bool isr_state() { return !__get_PRIMASK(); }
  static void isr_on()  { __enable_irq(); }
  static void isr_off() { __disable_irq(); }

  static void delay_ms(const int ms) { delay(ms); }

  // Tasks, called from idle()
  static void idletask();

  // Reset
  static uint8_t get_reset_source();
  static void clear_reset_source() {}

  // Free SRAM
  static int freeMemory() { return ::freeMemory(); }

  //
  // ADC Methods
  //

  static uint16_t adc_result;

  // Called by Temperature::init once at startup
  static void adc_init() {}

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const uint8_t ch) {}

  // Begin ADC sampling on the given channel. Called from Temperature::isr!
  static void adc_start(const uint8_t ch) { adc_result = analogRead(ch); }

  // Is the ADC ready for reading?
  static bool adc_ready() { return true; }

  // The current value of the ADC register
  static uint16_t adc_value() { return adc_result; }

  /**
   * Set the PWM duty cycle for the pin to the given value.
   * No inverting the duty cycle in this HAL.
   * No changing the maximum size of the provided value to enable finer PWM duty control in this HAL.
   */
  static void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t=255, const bool=false) {
    analogWrite(pin, v);
  }

};
