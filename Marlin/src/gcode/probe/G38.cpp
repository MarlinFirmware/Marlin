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

#if ENABLED(G38_PROBE_TARGET)

#include "../gcode.h"

#include "../../module/endstops.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"

inline void G38_single_probe(const uint8_t move_value) {
  endstops.enable(true);
  G38_move = move_value;
  prepare_line_to_destination();
  planner.synchronize();
  G38_move = 0;
  endstops.hit_on_purpose();
  set_current_from_steppers_for_axis(ALL_AXES_ENUM);
  sync_plan_position();
}

inline bool G38_run_probe() {

  bool G38_pass_fail = false;

  #if MULTIPLE_PROBING > 1
    // Get direction of move and retract
    xyz_float_t retract_mm;
    LOOP_NUM_AXES(i) {
      const float dist = destination[i] - current_position[i];
      retract_mm[i] = ABS(dist) < G38_MINIMUM_MOVE ? 0 : home_bump_mm((AxisEnum)i) * (dist > 0 ? -1 : 1);
    }
  #endif

  planner.synchronize();  // wait until the machine is idle

  // Move flag value
  #if ENABLED(G38_PROBE_AWAY)
    const uint8_t move_value = parser.subcode;
  #else
    constexpr uint8_t move_value = 1;
  #endif

  G38_did_trigger = false;

  // Move until destination reached or target hit
  G38_single_probe(move_value);

  if (G38_did_trigger) {

    G38_pass_fail = true;

    #if MULTIPLE_PROBING > 1
      // Move away by the retract distance
      destination = current_position + retract_mm;
      endstops.enable(false);
      prepare_line_to_destination();
      planner.synchronize();

      REMEMBER(fr, feedrate_mm_s, feedrate_mm_s * 0.25);

      // Bump the target more slowly
      destination -= retract_mm * 2;

      G38_single_probe(move_value);
    #endif
  }

  endstops.not_homing();
  return G38_pass_fail;
}

/**
 * G38 Probe Target
 *
 *  G38.2 - Probe toward workpiece, stop on contact, signal error if failure
 *  G38.3 - Probe toward workpiece, stop on contact
 *
 * With G38_PROBE_AWAY:
 *
 *  G38.4 - Probe away from workpiece, stop on contact break, signal error if failure
 *  G38.5 - Probe away from workpiece, stop on contact break
 */
void GcodeSuite::G38(const int8_t subcode) {

  // Get X Y Z E F
  get_destination_from_command();

  remember_feedrate_scaling_off();

  const bool error_on_fail = TERN(G38_PROBE_AWAY, !TEST(subcode, 0), subcode == 2);

  // If any axis has enough movement, do the move
  LOOP_NUM_AXES(i)
    if (ABS(destination[i] - current_position[i]) >= G38_MINIMUM_MOVE) {
      if (!parser.seenval('F')) feedrate_mm_s = homing_feedrate((AxisEnum)i);
      // If G38.2 fails throw an error
      if (!G38_run_probe() && error_on_fail) SERIAL_ERROR_MSG("Failed to reach target");
      break;
    }

  restore_feedrate_and_scaling();
}

#endif // G38_PROBE_TARGET
