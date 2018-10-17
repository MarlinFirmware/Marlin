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

#include "../gcode.h"
#include "../../module/planner.h"

/**
 * M92: Set axis steps-per-unit for one or more axes, X, Y, Z, and E.
 *      (Follows the same syntax as G92)
 *
 *      With multiple extruders use T to specify which one.
 */
void GcodeSuite::M92() {

  GET_TARGET_EXTRUDER();

  LOOP_XYZE(i) {
    if (parser.seen(axis_codes[i])) {
      if (i == E_AXIS) {
        const float value = parser.value_per_axis_units((AxisEnum)(E_AXIS + TARGET_EXTRUDER));
        if (value < 20) {
          float factor = planner.settings.axis_steps_per_mm[E_AXIS + TARGET_EXTRUDER] / value; // increase e constants if M92 E14 is given for netfab.
          #if HAS_CLASSIC_JERK && (DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE))
            planner.max_jerk[E_AXIS] *= factor;
          #endif
          planner.settings.max_feedrate_mm_s[E_AXIS + TARGET_EXTRUDER] *= factor;
          planner.max_acceleration_steps_per_s2[E_AXIS + TARGET_EXTRUDER] *= factor;
        }
        planner.settings.axis_steps_per_mm[E_AXIS + TARGET_EXTRUDER] = value;
      }
      else {
        planner.settings.axis_steps_per_mm[i] = parser.value_per_axis_units((AxisEnum)i);
      }
    }
  }
  planner.refresh_positioning();
}
