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
 * joystick.h - joystick input / jogging
 */

#include "../inc/MarlinConfigPre.h"
#include "../core/types.h"
#include "../module/temperature.h"

class Joystick {
  friend class Temperature;
  private:
    #if ENABLED(HAS_JOY_ADC_X)
      static temp_info_t x;
    #endif
    #if ENABLED(HAS_JOY_ADC_Y)
      static temp_info_t y;
    #endif
    #if ENABLED(HAS_JOY_ADC_Z)
      static temp_info_t z;
    #endif
  public:
    #if ENABLED(JOYSTICK_DEBUG)
      static void report();
    #endif
    static void calculate(xyz_float_t &norm_jog);
    static void inject_jog_moves();
};

extern Joystick joystick;
