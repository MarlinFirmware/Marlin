/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <stdint.h>

// AT32 pin type: numeric index into PIN_MAP[] (uint8_t, values PA0=0 .. PD15=63)
typedef uint8_t pin_t;

#ifndef HAL_ADC_RESOLUTION
  #define HAL_ADC_RESOLUTION 12
#endif
#define HAL_ADC_VREF_MV 3300

/**
 * HAL class for Marlin on AT32F403
 */
class MarlinHAL {
public:
  MarlinHAL() = default;

  // Watchdog — AT32 core provides WDG_Init / WDG_ReloadCounter
  static void watchdog_init()    IF_DISABLED(USE_WATCHDOG, {});
  static void watchdog_refresh() IF_DISABLED(USE_WATCHDOG, {});

  static void init();
  static void init_board() {}
  static void reboot() { NVIC_SystemReset(); }

  static bool isr_state() { return !__get_PRIMASK(); }
  static void isr_on()    { __enable_irq(); }
  static void isr_off()   { __disable_irq(); }
  static void delay_ms(const int ms) { delay(ms); }

  static void idletask() {}

  static uint8_t get_reset_source();
  static void    clear_reset_source() {}

  static int freeMemory();

  // ADC — AT32 core analogRead() is synchronous; result is stored here
  static uint16_t adc_result;

  static void adc_init() {}
  static void adc_enable(const pin_t pin) { pinMode(pin, INPUT_ANALOG); }
  static void adc_start(const pin_t pin)  { adc_result = (uint16_t)analogRead(pin); }
  static bool adc_ready()                 { return true; }
  static uint16_t adc_value()             { return adc_result; }

  // PWM — AT32 core: PWM_Init(pin, resolution, freq) / PWM_Write(pin, value)
  static void set_pwm_duty(const pin_t pin, const uint16_t value, const uint16_t scale = 255, const bool invert = false);
  static void set_pwm_frequency(const pin_t pin, const uint16_t f_desired);
};

// M997 SD firmware update (just reboots; let the bootloader do it)
#ifndef PLATFORM_M997_SUPPORT
  #define PLATFORM_M997_SUPPORT
#endif

void flashFirmware(const int16_t);
