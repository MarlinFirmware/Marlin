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

#ifdef __IMXRT1062__

#include "../../inc/MarlinConfig.h"

#include "HAL.h"

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t value, const uint16_t scale/*=255*/, const bool invert/*=false*/) {

  uint32_t bits = 1;
  uint16_t v = _MIN(value, scale);

  if (invert) v = scale - v;

  // Choose v_size as smallest power of 2 which holds scale.
  uint32_t v_size = 1;
  while (v_size < scale) {
    bits++;
    v_size *= 2;
  }

  uint32_t scaled_val = v_size * v / scale;

  uint32_t prior = analogWriteResolution(bits);
  analogWrite(pin, scaled_val);
  analogWriteResolution(prior);
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired) {
  analogWriteFrequency(pin, f_desired);
}

#endif // __IMXRT1062__
