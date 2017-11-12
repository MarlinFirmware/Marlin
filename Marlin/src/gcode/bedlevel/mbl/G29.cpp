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

/**
 * G29.cpp - Mesh Bed Leveling
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(MESH_BED_LEVELING)

#include "../../../feature/bedlevel/bedlevel.h"

#include "../../gcode.h"
#include "../../queue.h"

#include "../../../libs/buzzer.h"
#include "../../../lcd/ultralcd.h"
#include "../../../module/motion.h"
#include "../../../module/stepper.h"

// Save 130 bytes with non-duplication of PSTR
void echo_not_entered() { SERIAL_PROTOCOLLNPGM(" not entered."); }

void mesh_probing_done() {
  mbl.has_mesh = true;
  gcode.home_all_axes();
  set_bed_leveling_enabled(true);
  #if ENABLED(MESH_G28_REST_ORIGIN)
    current_position[Z_AXIS] = Z_MIN_POS;
    set_destination_from_current();
    buffer_line_to_destination(homing_feedrate(Z_AXIS));
    stepper.synchronize();
  #endif
}

/**
 * G29: Mesh-based Z probe, probes a grid and produces a
 *      mesh to compensate for variable bed height
 *
 * Parameters With MESH_BED_LEVELING:
 *
 *  S0              Produce a mesh report
 *  S1              Start probing mesh points
 *  S2              Probe the next mesh point
 *  S3 Xn Yn Zn.nn  Manually modify a single point
 *  S4 Zn.nn        Set z offset. Positive away from bed, negative closer to bed.
 *  S5              Reset and disable mesh
 *
 * The S0 report the points as below
 *
 *  +----> X-axis  1-n
 *  |
 *  |
 *  v Y-axis  1-n
 *
 */
void GcodeSuite::G29() {

  static int mbl_probe_index = -1;
  #if HAS_SOFTWARE_ENDSTOPS
    static bool enable_soft_endstops;
  #endif

  const MeshLevelingState state = (MeshLevelingState)parser.byteval('S', (int8_t)MeshReport);
  if (!WITHIN(state, 0, 5)) {
    SERIAL_PROTOCOLLNPGM("S out of range (0-5).");
    return;
  }

  int8_t px, py;

  switch (state) {
    case MeshReport:
      if (leveling_is_valid()) {
        SERIAL_PROTOCOLLNPAIR("State: ", planner.leveling_active ? MSG_ON : MSG_OFF);
        mbl_mesh_report();
      }
      else
        SERIAL_PROTOCOLLNPGM("Mesh bed leveling has no data.");
      break;

    case MeshStart:
      mbl.reset();
      mbl_probe_index = 0;
      enqueue_and_echo_commands_P(PSTR("G28\nG29 S2"));
      break;

    case MeshNext:
      if (mbl_probe_index < 0) {
        SERIAL_PROTOCOLLNPGM("Start mesh probing with \"G29 S1\" first.");
        return;
      }
      // For each G29 S2...
      if (mbl_probe_index == 0) {
        #if HAS_SOFTWARE_ENDSTOPS
          // For the initial G29 S2 save software endstop state
          enable_soft_endstops = soft_endstops_enabled;
        #endif
      }
      else {
        // For G29 S2 after adjusting Z.
        mbl.set_zigzag_z(mbl_probe_index - 1, current_position[Z_AXIS]);
        #if HAS_SOFTWARE_ENDSTOPS
          soft_endstops_enabled = enable_soft_endstops;
        #endif
      }
      // If there's another point to sample, move there with optional lift.
      if (mbl_probe_index < GRID_MAX_POINTS) {
        mbl.zigzag(mbl_probe_index, px, py);
        _manual_goto_xy(mbl.index_to_xpos[px], mbl.index_to_ypos[py]);

        #if HAS_SOFTWARE_ENDSTOPS
          // Disable software endstops to allow manual adjustment
          // If G29 is not completed, they will not be re-enabled
          soft_endstops_enabled = false;
        #endif

        mbl_probe_index++;
      }
      else {
        // One last "return to the bed" (as originally coded) at completion
        current_position[Z_AXIS] = Z_MIN_POS + MANUAL_PROBE_HEIGHT;
        line_to_current_position();
        stepper.synchronize();

        // After recording the last point, activate home and activate
        mbl_probe_index = -1;
        SERIAL_PROTOCOLLNPGM("Mesh probing done.");
        BUZZ(100, 659);
        BUZZ(100, 698);
        mesh_probing_done();
      }
      break;

    case MeshSet:
      if (parser.seenval('X')) {
        px = parser.value_int() - 1;
        if (!WITHIN(px, 0, GRID_MAX_POINTS_X - 1)) {
          SERIAL_PROTOCOLLNPGM("X out of range (1-" STRINGIFY(GRID_MAX_POINTS_X) ").");
          return;
        }
      }
      else {
        SERIAL_CHAR('X'); echo_not_entered();
        return;
      }

      if (parser.seenval('Y')) {
        py = parser.value_int() - 1;
        if (!WITHIN(py, 0, GRID_MAX_POINTS_Y - 1)) {
          SERIAL_PROTOCOLLNPGM("Y out of range (1-" STRINGIFY(GRID_MAX_POINTS_Y) ").");
          return;
        }
      }
      else {
        SERIAL_CHAR('Y'); echo_not_entered();
        return;
      }

      if (parser.seenval('Z')) {
        mbl.z_values[px][py] = parser.value_linear_units();
      }
      else {
        SERIAL_CHAR('Z'); echo_not_entered();
        return;
      }
      break;

    case MeshSetZOffset:
      if (parser.seenval('Z')) {
        mbl.z_offset = parser.value_linear_units();
      }
      else {
        SERIAL_CHAR('Z'); echo_not_entered();
        return;
      }
      break;

    case MeshReset:
      reset_bed_level();
      break;

  } // switch(state)

  report_current_position();
}

#endif // MESH_BED_LEVELING
