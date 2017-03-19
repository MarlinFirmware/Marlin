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

#if HOTENDS > 2
  #error "Zrib v2 supports up to 2 hotends. Comment this line to keep going."
#endif

#define BOARD_NAME "ZRIB v2"

#include "pins_RAMPS.h"

#undef FAN1_PIN
#define FAN1_PIN 6

#if HOTENDS == 2
  #undef HEATER_1_PIN
  #define HEATER_1_PIN 7
#endif

//switch Z_MIN_ZMAX and Z_MAX_PIN for auto bed leveling
#ifdef Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #undef Z_MIN_PIN
  #define Z_MIN_PIN 19 //Z_MAX_PIN
#endif

#ifdef FILAMENT_RUNOUT_SENSOR
  #undef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN 15 //Y_MAX_PIN
#endif
