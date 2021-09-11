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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SKEW_CORRECTION_GCODE)

#include "../gcode.h"
#include "../../module/planner.h"

/**
 * M852: Get or set the machine skew factors. Reports current values with no arguments.
 *
 *  S[xy_factor] - Alias for 'I'
 *  I[xy_factor] - New XY skew factor
 *  J[xz_factor] - New XZ skew factor
 *  K[yz_factor] - New YZ skew factor
 */
void GcodeSuite::M852() {
  uint8_t ijk = 0, badval = 0, setval = 0;

  if (parser.seen('I') || parser.seen('S')) {
    ++ijk;
    const float value = parser.value_linear_units();
    if (WITHIN(value, SKEW_FACTOR_MIN, SKEW_FACTOR_MAX)) {
      if (planner.skew_factor.xy != value) {
        planner.skew_factor.xy = value;
        ++setval;
      }
    }
    else
      ++badval;
  }

  #if ENABLED(SKEW_CORRECTION_FOR_Z)

    if (parser.seen('J')) {
      ++ijk;
      const float value = parser.value_linear_units();
      if (WITHIN(value, SKEW_FACTOR_MIN, SKEW_FACTOR_MAX)) {
        if (planner.skew_factor.xz != value) {
          planner.skew_factor.xz = value;
          ++setval;
        }
      }
      else
        ++badval;
    }

    if (parser.seen('K')) {
      ++ijk;
      const float value = parser.value_linear_units();
      if (WITHIN(value, SKEW_FACTOR_MIN, SKEW_FACTOR_MAX)) {
        if (planner.skew_factor.yz != value) {
          planner.skew_factor.yz = value;
          ++setval;
        }
      }
      else
        ++badval;
    }

  #endif

  if (badval)
    SERIAL_ECHOLNPGM(STR_SKEW_MIN " " STRINGIFY(SKEW_FACTOR_MIN) " " STR_SKEW_MAX " " STRINGIFY(SKEW_FACTOR_MAX));

  // When skew is changed the current position changes
  if (setval) {
    set_current_from_steppers_for_axis(ALL_AXES_ENUM);
    sync_plan_position();
    report_current_position();
  }

  if (!ijk) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Skew Factor");
    SERIAL_ECHOPAIR_F(" XY: ", planner.skew_factor.xy, 6);
    #if ENABLED(SKEW_CORRECTION_FOR_Z)
      SERIAL_ECHOPAIR_F(" XZ: ", planner.skew_factor.xz, 6);
      SERIAL_ECHOPAIR_F(" YZ: ", planner.skew_factor.yz, 6);
    #endif
    SERIAL_EOL();
  }
}

#endif // SKEW_CORRECTION_GCODE
