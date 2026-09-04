/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#ifdef TARGET_LPC5528

#include "../../inc/MarlinConfig.h"
#include <pwm.h>

/**
 * Hardware PWM via the Arduino core's pwm API, which drives either an SCTimer
 * output or a CTIMER match output depending on the pin.
 *
 * All ten SCTimer outputs share one cycle length, while CTIMER2 and CTIMER3
 * each have their own, so set_pwm_frequency() on an SCTimer pin retunes every
 * other SCTimer PWM pin. That is a property of the hardware
 * (UM11126 27 "features", 25.6.12), not of this implementation.
 *
 * Pins with no timer output at all - FAN0_PIN (P0_01) on MKS OWL is one - fall
 * back to on/off here and need FAN_SOFT_PWM for proper control.
 */

// Which pins have been attached to a timer output. Pin numbers are
// port << 5 | bit, so 0..63.
static uint64_t pwm_attached = 0;

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  if (pin < 0 || pin >= 64 || v_size == 0) return;

  const uint8_t p = uint8_t(pin);
  const uint16_t duty = invert ? v_size - _MIN(v, v_size) : _MIN(v, v_size);

  if (!pwm_pin_has_hardware(p)) {
    // No timer output on this pin - all we can do is on/off.
    pinMode(p, OUTPUT);
    digitalWrite(p, duty > (v_size >> 1) ? HIGH : LOW);
    return;
  }

  const uint64_t bit = uint64_t(1) << p;
  if (!(pwm_attached & bit)) {
    pwm_attach_pin(p, 0);
    pwm_attached |= bit;
  }
  pwm_write_scaled(p, duty, v_size);
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired) {
  if (pin < 0 || pin >= 64 || f_desired == 0) return;
  pwm_set_frequency(uint8_t(pin), f_desired);
}

#endif // TARGET_LPC5528
