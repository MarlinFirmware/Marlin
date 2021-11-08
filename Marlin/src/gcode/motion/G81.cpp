/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(CNC_DRILLING_CYCLE)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../MarlinCore.h"

/**
 * G81: Drilling cycle
 */
void GcodeSuite::G81() {
  if (!IsRunning()
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      || homing_needed_error(
          (parser.seen('X') ? _BV(X_AXIS) : 0)
        | (parser.seen('Y') ? _BV(Y_AXIS) : 0)
        | (parser.seen('Z') ? _BV(Z_AXIS) : 0) )
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
                  feedrate_mm_s
                #endif
              );

  auto line_to_z = [](const float &z, const feedRate_t fr_mm_s) {
    REMEMBER(fr, feedrate_mm_s, fr_mm_s);
    destination.z = z;
    prepare_line_to_destination();
  };

  // Store current Z and drill depth Z
  const float initial_z = current_position.z,
              depth_z = destination.z;
  float retract_z = initial_z;

  // Rapid move XY plane only at current Z height
  line_to_z(initial_z, fast_feedrate);

  // Rapid move to Z retract height, if provided
  if (parser.seenval('R')) {
    retract_z = LOGICAL_TO_NATIVE(parser.value_float(), Z_AXIS);
    line_to_z(retract_z, fast_feedrate);
  }

  // Move Z only to target depth at defined feedrate
  line_to_z(depth_z, slow_feedrate);

  // Retract to specified height or starting height
  line_to_z(retract_z, fast_feedrate);
}

#endif // CNC_DRILLING_CYCLE
