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
#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfigPre.h"
#include <pwm.h>

void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  if (!LPC176x::pin_is_valid(pin)) return;
  if (LPC176x::pwm_attach_pin(pin)) 
    LPC176x::pwm_write_ratio(pin, invert ? 1.0f - (float)v / v_size : (float)v / v_size);  // map 1-254 onto PWM range
}

#if NEEDS_HARDWARE_PWM // Specific meta-flag for features that mandate PWM

  void set_pwm_frequency(const pin_t pin, int f_desired) {
    LPC176x::pwm_set_frequency(pin, f_desired);
  }

#endif

#endif // TARGET_LPC1768
