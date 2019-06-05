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

#include "../gcode.h"
#include "../../module/motion.h"

#include "../../Marlin.h"

#include "../../module/planner.h"

#if BOTH(FWRETRACT, FWRETRACT_AUTORETRACT)
  #include "../../feature/fwretract.h"
#endif

#include "../../sd/cardreader.h"

#if ENABLED(NANODLP_Z_SYNC)
  #include "../../module/stepper.h"
#endif

extern float destination[XYZE];

#if ENABLED(VARIABLE_G0_FEEDRATE)
  float saved_g0_feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);
#endif

/**
 * G81: Drilling cycle
 */
void GcodeSuite::G81_G83(
  #if IS_SCARA || defined(G0_FEEDRATE)
    bool fast_move/*=false*/
  #endif
) {

  if (IsRunning()
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      && !axis_unhomed_error(parser.seen('X'), parser.seen('Y'), parser.seen('Z'))
    #endif
  ) {

    #ifdef G0_FEEDRATE
      float saved_feedrate_mm_s;
      #if ENABLED(VARIABLE_G0_FEEDRATE)
        if (fast_move) {
          saved_feedrate_mm_s = feedrate_mm_s;      // Back up the (old) motion mode feedrate
          feedrate_mm_s = saved_g0_feedrate_mm_s;   // Get G0 feedrate from last usage
        }
      #endif
    #endif

    // rapid move XY plane only at current Z height
    const bool hasX = parser.seenval('X');
    const float x = hasX ? parser.value_long() : current_position[X_AXIS];
    const bool hasY = parser.seenval('Y');
    const float y = hasY ? parser.value_long() : current_position[Y_AXIS];

    const bool hasR = parser.seenval('R');
    const float r = hasR ? parser.value_long() : current_position[Z_AXIS];

    const bool hasF = parser.seenval('F');
    const float f = hasF ? parser.value_long() : 0; // fix feed rate
    
    destination[X_AXIS] = x;
    destination[Y_AXIS] = y;
    destination[Z_AXIS] = current_position[Z_AXIS];
    const long initial_z = current_position[Z_AXIS];
    prepare_move_to_destination();
    planner.synchronize();

    // rapid move to Z retract height

    destination[X_AXIS] = current_position[X_AXIS];
    destination[Y_AXIS] = current_position[Y_AXIS];
    destination[Z_AXIS] = r;
    prepare_move_to_destination();
    planner.synchronize();
    
    if(parser.seenval('Z'))
    {
      // move Z only to target depth
      destination[X_AXIS] = current_position[X_AXIS];
      destination[Y_AXIS] = current_position[Y_AXIS];
      destination[Z_AXIS] = parser.value_long();
      prepare_move_to_destination();
      planner.synchronize();

      // retract to previous height, or retract height specified
      destination[X_AXIS] = current_position[X_AXIS];
      destination[Y_AXIS] = current_position[Y_AXIS];
      destination[Z_AXIS] = initial_z;
      prepare_move_to_destination();
      planner.synchronize();
    }

  }
}
