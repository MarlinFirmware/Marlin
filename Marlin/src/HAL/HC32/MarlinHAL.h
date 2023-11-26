/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <core_types.h>
#include <stdint.h>

typedef gpio_pin_t pin_t;

#if TEMP_SENSOR_SOC
  /**
   * Convert ots measurement float to uint16_t for adc_value()
   *
   * @note returns float as integer in degrees C * 10, if T > 0
   */
  #define OTS_FLOAT_TO_ADC_READING(T) ((T) > 0 ? ((uint16_t)((T) * 10.0f)) : 0)

  /**
   * Convert adc_value() uint16_t to ots measurement float
   *
   * @note see OTS_FLOAT_TO_ADC_READING for inverse
   *
   * @note RAW is oversampled by OVERSAMPLENR, so we need to divide first
   */
  #define TEMP_SOC_SENSOR(RAW) ((float)(((RAW) / OVERSAMPLENR) / 10))
#endif

/**
 * HAL class for Marlin on HC32F460
 */
class MarlinHAL {
public:
  // Earliest possible init, before setup()
  MarlinHAL();

  // Watchdog
  static void watchdog_init();
  static void watchdog_refresh();

  static void init();       // Called early in setup()
  static void init_board(); // Called less early in setup()
  static void reboot();     // Restart the firmware from 0x0

  // Interrupts
  static bool isr_state();
  static void isr_on();
  static void isr_off();

  static void delay_ms(const int ms);

  // Tasks, called from idle()
  static void idletask();

  // Reset
  static uint8_t get_reset_source();
  static void clear_reset_source();

  // Free SRAM
  static int freeMemory();

  //
  // ADC Methods
  //

  // Called by Temperature::init once at startup
  static void adc_init();

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const pin_t pin);

  // Begin ADC sampling on the given pin. Called from Temperature::isr!
  static void adc_start(const pin_t pin);

  // Is the ADC ready for reading?
  static bool adc_ready();

  // The current value of the ADC register
  static uint16_t adc_value();

  /**
   * Set the PWM duty cycle for the pin to the given value.
   * Optionally invert the duty cycle [default = false]
   * Optionally change the maximum size of the provided value to enable finer PWM duty control [default = 255]
   * The timer must be pre-configured with set_pwm_frequency() if the default frequency is not desired.
   */
  static void set_pwm_duty(const pin_t pin, const uint16_t value, const uint16_t scale = 255, const bool invert = false);

  /**
   * Set the frequency of the timer for the given pin.
   * All Timer PWM pins run at the same frequency.
   */
  static void set_pwm_frequency(const pin_t pin, const uint16_t f_desired);

private:
  /**
   * Pin number of the last pin that was used with adc_start()
   */
  static pin_t last_adc_pin;

  #if TEMP_SENSOR_SOC
    /**
     * On-chip temperature sensor value
     */
    static float soc_temp;
  #endif
};

// M997: Trigger a firmware update from SD card (after upload).
//       On HC32F460, a reboot is enough to do this.
#ifndef PLATFORM_M997_SUPPORT
  #define PLATFORM_M997_SUPPORT
#endif

void flashFirmware(const int16_t);
