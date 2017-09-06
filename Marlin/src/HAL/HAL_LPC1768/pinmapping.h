/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifndef __HAL_PINMAPPING_H__
#define __HAL_PINMAPPING_H__
#include "../../core/macros.h"

struct pin_data { uint8_t port, pin; };
struct adc_pin_data { uint8_t port, pin, adc; };

#if ENABLED(IS_REARM)
  #include "pinmap_re_arm.h"
#else
  #error "HAL: LPC1768: No defined pin-mapping"
#endif

#endif // __HAL_PINMAPPING_H__
