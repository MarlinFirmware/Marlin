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

#if ENABLED(EXTENSIBLE_UI)
  #include "../../../lcd/extui/ui_api.h"
#endif

// Save 130 bytes with non-duplication of PSTR
inline void echo_not_entered(const char c) { SERIAL_CHAR(c); SERIAL_ECHOLNPGM(" not entered."); }

/**
 * G29: Mesh-based Z probe, probes a grid and produces a
 *      mesh to compensate for variable bed height
 *
 * Parameters With MESH_BED_LEVELING:
 *
 *  S0              Report the current mesh values
 *  S1              Start probing mesh points
 *  S2              Probe the next mesh point
 *  S3 In Jn Zn.nn  Manually modify a single point
 *  S4 Zn.nn        Set z offset. Positive away from bed, negative closer to bed.
 *  S5              Reset and disable mesh
 */
void GcodeSuite::G29() {

  static int mbl_probe_index = -1;
  TERN_(HAS_SOFTWARE_ENDSTOPS, static bool saved_soft_endstops_state);

  MeshLevelingState state = (MeshLevelingState)parser.byteval('S', (int8_t)MeshReport);
  if (!WITHIN(state, 0, 5)) {
    SERIAL_ECHOLNPGM("S out of range (0-5).");
    return;
  }

  int8_t ix, iy;

  switch (state) {
    case MeshReport:
      SERIAL_ECHOPGM("Mesh Bed Leveling ");
      if (leveling_is_valid()) {
        serialprintln_onoff(planner.leveling_active);
        mbl.report_mesh();
      }
      else
        SERIAL_ECHOLNPGM("has no data.");
      break;

    case MeshStart:
      mbl.reset();
      mbl_probe_index = 0;
      if (!ui.wait_for_move) {
        queue.inject_P(PSTR("G28\nG29 S2"));
        return;
      }
      state = MeshNext;

    case MeshNext:
      if (mbl_probe_index < 0) {
        SERIAL_ECHOLNPGM("Start mesh probing with \"G29 S1\" first.");
        return;
      }
      // For each G29 S2...
      if (mbl_probe_index == 0) {
        #if HAS_SOFTWARE_ENDSTOPS
          // For the initial G29 S2 save software endstop state
          saved_soft_endstops_state = soft_endstops_enabled;
        #endif
        // Move close to the bed before the first point
        do_blocking_move_to_z(0);
      }
      else {
        // Save Z for the previous mesh position
        mbl.set_zigzag_z(mbl_probe_index - 1, current_position.z);
        TERN_(HAS_SOFTWARE_ENDSTOPS, soft_endstops_enabled = saved_soft_endstops_state);
      }
      // If there's another point to sample, move there with optional lift.
      if (mbl_probe_index < GRID_MAX_POINTS) {
        #if HAS_SOFTWARE_ENDSTOPS
          // Disable software endstops to allow manual adjustment
          // If G29 is not completed, they will not be re-enabled
          soft_endstops_enabled = false;
        #endif

        mbl.zigzag(mbl_probe_index++, ix, iy);
        _manual_goto_xy({ mbl.index_to_xpos[ix], mbl.index_to_ypos[iy] });
      }
      else {
        // One last "return to the bed" (as originally coded) at completion
        current_position.z = MANUAL_PROBE_HEIGHT;
        line_to_current_position();
        planner.synchronize();

        // After recording the last point, activate home and activate
        mbl_probe_index = -1;
        SERIAL_ECHOLNPGM("Mesh probing done.");
        BUZZ(100, 659);
        BUZZ(100, 698);

        home_all_axes();
        set_bed_leveling_enabled(true);

        #if ENABLED(MESH_G28_REST_ORIGIN)
          current_position.z = 0;
          line_to_current_position(homing_feedrate(Z_AXIS));
          planner.synchronize();
        #endif

        TERN_(LCD_BED_LEVELING, ui.wait_for_move = false);
      }
      break;

    case MeshSet:
      if (parser.seenval('I')) {
        ix = parser.value_int();
        if (!WITHIN(ix, 0, GRID_MAX_POINTS_X - 1)) {
          SERIAL_ECHOPAIR("I out of range (0-", int(GRID_MAX_POINTS_X - 1));
          SERIAL_ECHOLNPGM(")");
          return;
        }
      }
      else
        return echo_not_entered('J');

      if (parser.seenval('J')) {
        iy = parser.value_int();
        if (!WITHIN(iy, 0, GRID_MAX_POINTS_Y - 1)) {
          SERIAL_ECHOPAIR("J out of range (0-", int(GRID_MAX_POINTS_Y - 1));
          SERIAL_ECHOLNPGM(")");
          return;
        }
      }
      else
        return echo_not_entered('J');

      if (parser.seenval('Z')) {
        mbl.z_values[ix][iy] = parser.value_linear_units();
        TERN_(EXTENSIBLE_UI, ExtUI::onMeshUpdate(ix, iy, mbl.z_values[ix][iy]));
      }
      else
        return echo_not_entered('Z');
      break;

    case MeshSetZOffset:
      if (parser.seenval('Z'))
        mbl.z_offset = parser.value_linear_units();
      else
        return echo_not_entered('Z');
      break;

    case MeshReset:
      reset_bed_level();
      break;

  } // switch(state)

  if (state == MeshNext) {
    SERIAL_ECHOPAIR("MBL G29 point ", _MIN(mbl_probe_index, GRID_MAX_POINTS));
    SERIAL_ECHOLNPAIR(" of ", int(GRID_MAX_POINTS));
  }

  report_current_position();
}

#endif // MESH_BED_LEVELING
