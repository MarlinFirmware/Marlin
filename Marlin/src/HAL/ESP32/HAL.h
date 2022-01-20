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

/**
 * HAL for Espressif ESP32 WiFi
 */

#define CPU_32_BIT

#include <stdint.h>

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio.h"
#include "watchdog.h"
#include "i2s.h"

#if ENABLED(WIFISUPPORT)
  #include "WebSocketSerial.h"
#endif

#if ENABLED(ESP3D_WIFISUPPORT)
  #include "esp3dlib.h"
#endif

#include "FlushableHardwareSerial.h"

// ------------------------
// Defines
// ------------------------

#define MYSERIAL1 flushableSerial

#if EITHER(WIFISUPPORT, ESP3D_WIFISUPPORT)
  #if ENABLED(ESP3D_WIFISUPPORT)
    typedef ForwardSerial1Class< decltype(Serial2Socket) > DefaultSerial1;
    extern DefaultSerial1 MSerial0;
    #define MYSERIAL2 MSerial0
  #else
    #define MYSERIAL2 webSocketSerial
  #endif
#endif

#define CRITICAL_SECTION_START() portENTER_CRITICAL(&spinlock)
#define CRITICAL_SECTION_END()   portEXIT_CRITICAL(&spinlock)

// ------------------------
// Types
// ------------------------

typedef int16_t pin_t;

class Servo;
typedef Servo hal_servo_t;

// ------------------------
// Public functions
// ------------------------

//
// Tone
//
void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration=0);
void noTone(const pin_t _pin);

void analogWrite(pin_t pin, int value);

//
// Pin Mapping for M42, M43, M226
//
#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#if ENABLED(USE_ESP32_EXIO)
  void Write_EXIO(uint8_t IO, uint8_t v);
#endif

//
// Delay in cycles (used by DELAY_NS / DELAY_US)
//
FORCE_INLINE static void DELAY_CYCLES(uint32_t x) {
  unsigned long start, ccount, stop;

  /**
   * It's important to care for race conditions (and overflows) here.
   * Race condition example: If `stop` calculates to being close to the upper boundary of
   * `uint32_t` and if at the same time a longer loop interruption kicks in (e.g. due to other
   * FreeRTOS tasks or interrupts), `ccount` might overflow (and therefore be below `stop` again)
   * without the loop ever being able to notice that `ccount` had already been above `stop` once
   * (and that therefore the number of cycles to delay has already passed).
   * As DELAY_CYCLES (through DELAY_NS / DELAY_US) is used by software SPI bit banging to drive
   * LCDs and therefore might be called very, very often, this seemingly improbable situation did
   * actually happen in reality. It resulted in apparently random print pauses of ~17.9 seconds
   * (0x100000000 / 240 MHz) or multiples thereof, essentially ruining the current print by causing
   * large blobs of filament.
   */

  __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (start) );
  stop = start + x;
  ccount = start;

  if (stop >= start) {
    // no overflow, so only loop while in between start and stop:
    // 0x00000000 -----------------start****stop-- 0xFFFFFFFF
    while (ccount >= start && ccount < stop) {
      __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (ccount) );
    }
  }
  else {
    // stop did overflow, so only loop while outside of stop and start:
    // 0x00000000 **stop-------------------start** 0xFFFFFFFF
    while (ccount >= start || ccount < stop) {
      __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (ccount) );
    }
  }

}

// ------------------------
// Class Utilities
// ------------------------

#pragma GCC diagnostic push
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

int freeMemory();

#pragma GCC diagnostic pop

void _delay_ms(const int ms);

// ------------------------
// MarlinHAL Class
// ------------------------

#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10

class MarlinHAL {
public:

  // Earliest possible init, before setup()
  MarlinHAL() {}

  static void init() {}  // Called early in setup()
  static void init_board();     // Called less early in setup()
  static void reboot();         // Restart the firmware

  // Interrupts
  static portMUX_TYPE spinlock;
  static bool isr_state() { return spinlock.owner == portMUX_FREE_VAL; }
  static void isr_on()  { if (spinlock.owner != portMUX_FREE_VAL) portEXIT_CRITICAL(&spinlock); }
  static void isr_off() { portENTER_CRITICAL(&spinlock); }

  static void delay_ms(const int ms) { _delay_ms(ms); }

  // Tasks, called from idle()
  static void idletask();

  // Reset
  static uint8_t get_reset_source();
  static void clear_reset_source() {}

  // Free SRAM
  static int freeMemory();

  //
  // ADC Methods
  //

  static uint16_t adc_result;

  // Called by Temperature::init once at startup
  static void adc_init();

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const pin_t pin) {}

  // Begin ADC sampling on the given channel
  static void adc_start(const pin_t pin);

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
