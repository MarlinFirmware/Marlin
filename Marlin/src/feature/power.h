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
 * power.h - power control
 */

#if ENABLED(AUTO_POWER_CONTROL)
  #include "../core/millis_t.h"
#endif

class Power {
  public:
    static bool psu_on;

    static void init();
    static void power_on();
    static void power_off();

  #if ENABLED(AUTO_POWER_CONTROL) && POWER_OFF_DELAY > 0
    static void power_off_soon();
  #else
    static inline void power_off_soon() { power_off(); }
  #endif

  #if ENABLED(AUTO_POWER_CONTROL)
    static void check(const bool pause);

    private:
      static millis_t lastPowerOn;
      static bool is_power_needed();

  #endif
};

extern Power powerManager;
