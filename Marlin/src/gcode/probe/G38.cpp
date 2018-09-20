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

#include "../../inc/MarlinConfig.h"

#if ENABLED(G38_PROBE_TARGET)

#include "../gcode.h"

#include "../../module/endstops.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/probe.h"

static bool G38_run_probe() {

  bool G38_pass_fail = false;

  #if MULTIPLE_PROBING > 1
    // Get direction of move and retract
    float retract_mm[XYZ];
    LOOP_XYZ(i) {
      const float dist = destination[i] - current_position[i];
      retract_mm[i] = ABS(dist) < G38_MINIMUM_MOVE ? 0 : home_bump_mm((AxisEnum)i) * (dist > 0 ? -1 : 1);
    }
  #endif

  planner.synchronize();  // wait until the machine is idle

  // Move until destination reached or target hit
  endstops.enable(true);
  G38_move = true;
  G38_endstop_hit = false;
  prepare_move_to_destination();
  planner.synchronize();
  G38_move = false;

  endstops.hit_on_purpose();
  set_current_from_steppers_for_axis(ALL_AXES);
  sync_plan_position();

  if (G38_endstop_hit) {

    G38_pass_fail = true;

    #if MULTIPLE_PROBING > 1
      // Move away by the retract distance
      set_destination_from_current();
      LOOP_XYZ(i) destination[i] += retract_mm[i];
      endstops.enable(false);
      prepare_move_to_destination();
      planner.synchronize();

      feedrate_mm_s /= 4;

      // Bump the target more slowly
      LOOP_XYZ(i) destination[i] -= retract_mm[i] * 2;

      endstops.enable(true);
      G38_move = true;
      prepare_move_to_destination();
      planner.synchronize();
      G38_move = false;

      set_current_from_steppers_for_axis(ALL_AXES);
      sync_plan_position();
    #endif
  }

  endstops.hit_on_purpose();
  endstops.not_homing();
  return G38_pass_fail;
}

/**
 * G38.2 - probe toward workpiece, stop on contact, signal error if failure
 * G38.3 - probe toward workpiece, stop on contact
 *
 * Like G28 except uses Z min probe for all axes
 */
void GcodeSuite::G38(const bool is_38_2) {
  // Get X Y Z E F
  get_destination_from_command();

  setup_for_endstop_or_probe_move();

  // If any axis has enough movement, do the move
  LOOP_XYZ(i)
    if (ABS(destination[i] - current_position[i]) >= G38_MINIMUM_MOVE) {
      if (!parser.seenval('F')) feedrate_mm_s = homing_feedrate((AxisEnum)i);
      // If G38.2 fails throw an error
      if (!G38_run_probe() && is_38_2) {
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM("Failed to reach target");
      }
      break;
    }

  clean_up_after_endstop_or_probe_move();
}

#endif // G38_PROBE_TARGET
