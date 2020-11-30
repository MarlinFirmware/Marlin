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

#include "../inc/MarlinConfigPre.h"

#if ENABLED(CASE_LIGHT_USE_NEOPIXEL)
  #include "leds/leds.h"
#endif

#if DISABLED(CASE_LIGHT_NO_BRIGHTNESS) || ENABLED(CASE_LIGHT_USE_NEOPIXEL)
  #define CASELIGHT_USES_BRIGHTNESS 1
#endif

class CaseLight {
public:
  #if CASELIGHT_USES_BRIGHTNESS
    static uint8_t brightness;
  #endif
  static bool on;

  static void update(const bool sflag);
  static inline void update_brightness() { update(false); }
  static inline void update_enabled() { update(true); }

private:
  #if ENABLED(CASE_LIGHT_USE_NEOPIXEL)
    static LEDColor color;
  #endif
};

extern CaseLight caselight;
