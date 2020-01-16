/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(FAST_PWM_FAN) || SPINDLE_LASER_PWM

#include <pwm.h>

void set_pwm_frequency(const pin_t pin, int f_desired) {
  LPC176x::pwm_set_frequency(pin, f_desired);
}

void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  LPC176x::pwm_write_ratio(pin, invert ? 1.0f - (float)v / v_size : (float)v / v_size);
}

#endif // FAST_PWM_FAN || SPINDLE_LASER_PWM
#endif // TARGET_LPC1768
