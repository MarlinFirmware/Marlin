/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(CNC_DRILLING_CYCLE)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../Marlin.h"

/**
 * G81: Drilling cycle
 */
void GcodeSuite::G81() {
  if (!IsRunning()
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      || axis_unhomed_error(parser.seen('X'), parser.seen('Y'), parser.seen('Z'))
    #endif
  ) return;

  get_destination_from_command();

  const float slow_feedrate = feedrate_mm_s,
              fast_feedrate = (
                #if ENABLED(VARIABLE_G0_FEEDRATE)
                  saved_g0_feedrate_mm_s
                #elif defined(G0_FEEDRATE)
                  MMM_TO_MMS(G0_FEEDRATE)
                #else
                  slow_feedrate
                #endif
              );

  // Store current Z and drill depth Z
  const float initial_z = current_position[Z_AXIS],
              depth_z = destination[Z_AXIS];

  // Rapid move XY plane only at current Z height
  feedrate_mm_s = fast_feedrate;
  destination[Z_AXIS] = initial_z;
  prepare_move_to_destination();

  // Rapid move to Z retract height, if provided
  float ret_z = initial_z;
  if (parser.seenval('R')) {
    ret_z = LOGICAL_TO_NATIVE(parser.value_float(), Z_AXIS);
    destination[Z_AXIS] = ret_z;
    prepare_move_to_destination();
  }

  // Move Z only to target depth at defined feedrate
  feedrate_mm_s = slow_feedrate;
  destination[Z_AXIS] = depth_z;
  prepare_move_to_destination();

  // Retract to specified height or starting height
  feedrate_mm_s = fast_feedrate;
  destination[Z_AXIS] = ret_z;
  prepare_move_to_destination();

  feedrate_mm_s = slow_feedrate;
}

#endif // CNC_DRILLING_CYCLE
