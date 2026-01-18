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
 * speed_dial.h - speed dial feature
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(SPEED_DIAL_FEATURE)

class SpeedDial {
  public:
    static inline uint8_t    current()                       { return current_speed_dial; }
    static inline raw_adc_t  raw()                           { return current_speed_dial_raw; }
    static void              set(raw_adc_t value);

  private:
    static void       updateStepper();
    
    static raw_adc_t  current_speed_dial_raw;
    static uint8_t    current_speed_dial;
    static uint8_t    next_value_change;
};

extern SpeedDial speedDial;

#endif
