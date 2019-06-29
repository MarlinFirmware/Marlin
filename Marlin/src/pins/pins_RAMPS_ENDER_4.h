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

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "Ender-4 supports only 1 hotend / E-stepper. Comment out this line to continue."
#endif

#define BOARD_NAME "Ender-4"

#include "pins_RAMPS.h"

// The board only has one PWM fan connector. The others are 12V always-on.
// The default config uses this pin to control the brightness of the LED
// band (case light). Thus the hotend and controller fans are always-on.

#if ENABLED(CASE_LIGHT_ENABLE)
  #undef FAN_PIN
  #ifndef CASE_LIGHT_PIN
    #define CASE_LIGHT_PIN RAMPS_D9_PIN
  #endif
#endif
