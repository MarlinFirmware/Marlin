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
 * TODO: Hardware PWM is not implemented yet, so these fall back to plain
 * on/off. See tasks.md P2-1 for the timer allocation this needs:
 *  - FAN0_PIN (P0_01) has no timer output at all and can only ever be
 *    software PWM.
 *  - HEATER_0_PIN (P1_07) and FAN1_PIN (P1_06) have no SCTimer output;
 *    they can only be driven from CTIMER2_MAT2 / CTIMER2_MAT1.
 *  - SERVO0_PIN (P0_10) and HEATER_BED_PIN (P1_09) share SCT0_OUT2, so the
 *    servo has to move to CTIMER2_MAT0.
 */

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  const uint16_t duty = invert ? v_size - v : v;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, duty > (v_size >> 1) ? HIGH : LOW);
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired) {
  UNUSED(pin); UNUSED(f_desired);
}

#endif // TARGET_LPC5528
