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
#include "../../feature/bedlevel/bedlevel.h"
#include "../../lcd/marlinui.h"


inline bool G38_run_probe(const ProbePtRaise raise_after) {

  // Move flag value
  #if ENABLED(G38_PROBE_AWAY)
    const uint8_t move_value = parser.subcode;
  #else
    constexpr uint8_t move_value = 1;
  #endif

  const xyz_pos_t measured = probe.probe_safely(destination, raise_after, move_value, 0, true, true, Z_TWEEN_SAFE_CLEARANCE, true, true);
  
  LOOP_NUM_AXES(a) {
    if (isnan(measured[a])) return true;
  }

  // Report a good probe result in machine coordinate system to the host and LCD
  SString<30> msg(
    F("Machine X:"), p_float_t(measured.x, 2),
    F(" Y:"), p_float_t(measured.y, 2),
    F(" Z:"), p_float_t(measured.z, 3)
  );
  msg.echoln();
  TERN_(VERBOSE_SINGLE_PROBE, ui.set_status(msg));

    // If the probe is stowed, move the nozzle to the position of the probe
  const xyz_pos_t offs = DIFF_TERN(HAS_HOTEND_OFFSET, probe.offset, hotend_offset[active_extruder]);
  if ((!endstops.z_probe_enabled) && (probe.offset.z >= TERN0(HAS_HOTEND_OFFSET, hotend_offset[active_extruder].z))) {
    if ((!NEAR_ZERO(offs.x)) || (!NEAR_ZERO(offs.y)) || offs.z > 0.0f) {
      do_z_clearance_by(Z_TWEEN_SAFE_CLEARANCE);
    }
    destination = measured;
    do_blocking_move_to(destination);
    planner.synchronize();
  }

  report_current_position();
  return false;
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
 *
 * Parameters:
 *
 *   X   Probe X position (default current X)
 *   Y   Probe Y position (default current Y)
 *   Z   Probe Z position (default current Z)
 *   S   Stow the probe after probing (default: 0)
 */
void GcodeSuite::G38(const int8_t subcode) {

  // Get X Y Z E F
  get_destination_from_command();
  
  probe.use_probing_tool();

  #if HAS_LEVELING
    // Temporarily disable leveling so the planner won't mess with us
    TEMPORARY_BED_LEVELING_STATE(false);
  #endif

  remember_feedrate_scaling_off();

  // Raise after based on the 'S' parameter
  const ProbePtRaise raise_after = parser.boolval('S', false) ? PROBE_PT_STOW : PROBE_PT_NONE;

  const bool error_on_fail = TERN(G38_PROBE_AWAY, !TEST(subcode, 0), subcode == 2);

  // If any axis has enough movement, do the move
  LOOP_NUM_AXES(i) {
    if (ABS(destination[i] - current_position[i]) >= G38_MINIMUM_MOVE) {
      if (!parser.seenval('F')) feedrate_mm_s = homing_feedrate((AxisEnum)i);
      // If G38.2 fails throw an error
      if (G38_run_probe(raise_after) && error_on_fail) {
        SERIAL_ERROR_MSG("Failed to reach target");
      }
      break;
    }
  }
  restore_feedrate_and_scaling();
  probe.use_probing_tool(false);
}

#endif // G38_PROBE_TARGET
