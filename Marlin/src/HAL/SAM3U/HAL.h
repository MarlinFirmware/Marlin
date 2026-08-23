/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * HAL for the Atmel SAM3U (AT91SAM3U4E) as used on the 4pi controller.
 *
 * This is a bare-metal HAL on CMSIS - there is no Arduino core for the SAM3U -
 * so include/Arduino.h stands in for the Arduino API. It is closely related to
 * the DUE (SAM3X8E) HAL, but note the differences that matter:
 *
 *   - 16-bit timer/counter channels, and only three of them (see timers.h)
 *   - 256KB flash and 48KB usable SRAM, against the Due's 512KB/96KB
 *   - PIO pull-ups only, no pull-downs
 *   - Separate 12-bit (ADC12B) and 10-bit converters; this HAL uses ADC12B
 *   - USB is UDPHS, not the Due's UOTGHS
 */

#define CPU_32_BIT

#include "../shared/Marduino.h"
#include "../shared/eeprom_if.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "clock.h"
#include "adc.h"

#include <stdint.h>

//
// Serial Ports
//

#include "MarlinSerial.h"

// ------------------------
// Types
// ------------------------

// pin_t comes from include/pinmapping.h

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
#define HAL_ADC_VREF_MV   3300
#define HAL_ADC_RESOLUTION  12

// Pins are addressed natively, so an "analog input" is just its pin
#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) pin_t(p)
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

// Bring up PLLA and the SysTick time base. Called at the top of main().
void sam3u_systick_init();

// Return free RAM between the end of the heap (or .bss) and the stack
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

  // Tasks, called from marlin.idle()
  static void idletask() {}

  // Reset
  static uint8_t get_reset_source();
  static void clear_reset_source();

  // Free SRAM
  static int freeMemory() { return ::freeMemory(); }

  //
  // ADC Methods
  //

  // Called by Temperature::init once at startup
  static void adc_init() { sam3u_adc_init(); }

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const pin_t pin) { sam3u_adc_enable_pin(pin); }

  // Begin ADC sampling on the given pin. Called from Temperature::isr!
  static void adc_start(const pin_t pin) { sam3u_adc_start_pin(pin); }

  // Is the ADC ready for reading?
  static bool adc_ready() { return sam3u_adc_ready(); }

  // The current value of the ADC register
  static uint16_t adc_value() { return sam3u_adc_value(); }

  /**
   * Set the PWM duty cycle for the pin to the given value.
   *
   * The SAM3U's four PWM channels do not reach the 4pi's heater and FET
   * outputs, so this reduces to an on/off threshold and the real work is done
   * by Marlin's software PWM - the same approach the original 4pi firmware
   * took. See inc/Conditionals_adv.h, which turns on FAN_SOFT_PWM.
   */
  static void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size=255, const bool invert=false) {
    const bool on = (invert ? v_size - v : v) > v_size / 2;
    digitalWrite(pin, on ? HIGH : LOW);
  }

};
