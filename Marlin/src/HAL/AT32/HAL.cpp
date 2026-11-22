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

#ifdef ARDUINO_ARCH_AT32

#include "HAL.h"
#include "../../core/mstring.h"
#include "../../core/serial.h"
#include "../../core/language.h"
#include "../../core/utility.h"
#include "../../core/endianness.h"
#include "../../MarlinCore.h"
#include "core_hooks.h"

extern "C" char* _sbrk(int incr);

#if ENABLED(MARLIN_DEV_MODE)
  inline void HAL_clock_frequencies_dump() {
    SERIAL_ECHOPGM("-- clocks dump --\nF_CPU=", F_CPU);
    SERIAL_EOL();
  }
#endif

uint16_t MarlinHAL::adc_result = 0;

void MarlinHAL::watchdog_init() {
  #if ENABLED(USE_WATCHDOG)
    WDG_Init(5000); // 5-second timeout
  #endif
}

void MarlinHAL::watchdog_refresh() {
  #if ENABLED(USE_WATCHDOG)
    WDG_ReloadCounter();
  #endif
}

void MarlinHAL::init() {
  NVIC_SetPriorityGrouping(0x3);
  TERN_(MARLIN_DEV_MODE, HAL_clock_frequencies_dump());
  #if ENABLED(MARLIN_DEV_MODE)
    if (freeMemory() < 1024) SERIAL_ECHOLNPGM("HAL: low memory after init!");
  #endif
}

uint8_t MarlinHAL::get_reset_source() {
  return 1; // Simplified: always report power-on reset
}

int MarlinHAL::freeMemory() {
  volatile char top;
  return &top - _sbrk(0);
}

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t value, const uint16_t scale, const bool invert) {
  const uint16_t val = invert ? scale - value : value;
  // Map from [0,scale] to [0,255] for PWM_Write
  const uint16_t duty = (scale == 255) ? val : (uint16_t)map(val, 0, scale, 0, 255);
  PWM_Write(pin, duty);
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired) {
  PWM_Init(pin, 255, f_desired);
}

void flashFirmware(const int16_t) { MarlinHAL::reboot(); }

// Emergency parser integration is handled via core_hook_usart_rx_irq
// in core_hooks.h (weak alias pattern). The actual implementation
// is in the AT32 core's serial IRQ handler.

#endif // ARDUINO_ARCH_AT32
