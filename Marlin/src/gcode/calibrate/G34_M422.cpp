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

#include "../../inc/MarlinConfig.h"

#if ENABLED(Z_STEPPER_AUTO_ALIGN)

#include "../gcode.h"
#include "../../module/delta.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/endstops.h"

#if HOTENDS > 1
  #include "../../module/tool_change.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../feature/bltouch.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

float z_auto_align_xpos[Z_STEPPER_COUNT] = Z_STEPPER_ALIGN_X,
      z_auto_align_ypos[Z_STEPPER_COUNT] = Z_STEPPER_ALIGN_Y;

inline void set_all_z_lock(const bool lock) {
  stepper.set_z_lock(lock);
  stepper.set_z2_lock(lock);
  #if ENABLED(Z_TRIPLE_STEPPER_DRIVERS)
    stepper.set_z3_lock(lock);
  #endif
}

/**
 * G34: Z-Stepper automatic alignment
 *
 * Parameters: I<iterations> T<accuracy> A<amplification>
 */
void GcodeSuite::G34() {
  if (DEBUGGING(LEVELING)) {
    DEBUG_ECHOLNPGM(">>> G34");
    log_machine_info();
  }

  do { // break out on error

    if (!TEST(axis_known_position, X_AXIS) || !TEST(axis_known_position, Y_AXIS)) {
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> XY homing required.");
      break;
    }

    const int8_t z_auto_align_iterations = parser.intval('I', Z_STEPPER_ALIGN_ITERATIONS);
    if (!WITHIN(z_auto_align_iterations, 1, 30)) {
      SERIAL_ECHOLNPGM("?(I)teration out of bounds (1-30).");
      break;
    }

    const float z_auto_align_accuracy = parser.floatval('T', Z_STEPPER_ALIGN_ACC);
    if (!WITHIN(z_auto_align_accuracy, 0.01f, 1.0f)) {
      SERIAL_ECHOLNPGM("?(T)arget accuracy out of bounds (0.01-1.0).");
      break;
    }

    const float z_auto_align_amplification = parser.floatval('A', Z_STEPPER_ALIGN_AMP);
    if (!WITHIN(ABS(z_auto_align_amplification), 0.5f, 2.0f)) {
      SERIAL_ECHOLNPGM("?(A)mplification out of bounds (0.5-2.0).");
      break;
    }

    // Wait for planner moves to finish!
    planner.synchronize();

    // Disable the leveling matrix before auto-aligning
    #if HAS_LEVELING
      #if ENABLED(RESTORE_LEVELING_AFTER_G34)
        const bool leveling_was_active = planner.leveling_active;
      #endif
      set_bed_leveling_enabled(false);
    #endif

    #if ENABLED(CNC_WORKSPACE_PLANES)
      workspace_plane = PLANE_XY;
    #endif

    #if ENABLED(BLTOUCH)
      bltouch.init();
    #endif

    // Always home with tool 0 active
    #if HOTENDS > 1
      const uint8_t old_tool_index = active_extruder;
      tool_change(0, 0, true);
    #endif

    #if HAS_DUPLICATION_MODE
      extruder_duplication_enabled = false;
    #endif

    // Before moving other axes raise Z, if needed. Never lower Z.
    if (current_position[Z_AXIS] < Z_CLEARANCE_BETWEEN_PROBES) {
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("Raise Z (before moving to probe pos) to ", Z_CLEARANCE_BETWEEN_PROBES);
      do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
    }

    // Remember corrections to determine errors on each iteration
    float last_z_align_move[Z_STEPPER_COUNT] = ARRAY_N(Z_STEPPER_COUNT, 10000.0f, 10000.0f, 10000.0f),
          z_measured[Z_STEPPER_COUNT] = { 0 };
    bool err_break = false;
    for (uint8_t iteration = 0; iteration < z_auto_align_iterations; ++iteration) {
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> probing all positions.");

      // Reset minimum value
      float z_measured_min = 100000.0f;
      // For each iteration go through all probe positions (one per Z-Stepper)
      for (uint8_t zstepper = 0; zstepper < Z_STEPPER_COUNT; ++zstepper) {
        // Probe a Z height for each stepper
        z_measured[zstepper] = probe_pt(z_auto_align_xpos[zstepper], z_auto_align_ypos[zstepper], PROBE_PT_RAISE, false);

        // Stop on error
        if (isnan(z_measured[zstepper])) {
          if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> PROBING FAILED!");
          err_break = true;
          break;
        }

        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(zstepper + 1), " measured position is ", z_measured[zstepper]);

        // Remember the maximum position to calculate the correction
        z_measured_min = MIN(z_measured_min, z_measured[zstepper]);
      }

      if (err_break) break;

      // Remember the current z position to return to
      float z_original_position = current_position[Z_AXIS];

      // Iterations can stop early if all corrections are below required accuracy
      bool success_break = true;
      // Correct stepper offsets and re-iterate
      for (uint8_t zstepper = 0; zstepper < Z_STEPPER_COUNT; ++zstepper) {
        stepper.set_separate_multi_axis(true);
        set_all_z_lock(true); // Steppers will be enabled separately

        // Calculate current stepper move
        const float z_align_move = z_measured[zstepper] - z_measured_min,
                    z_align_abs = ABS(z_align_move);

        // Check for lost accuracy compared to last move
        if (last_z_align_move[zstepper] < z_align_abs - 1.0) {
          // Stop here
          if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> detected decreasing accuracy.");
          err_break = true;
          break;
        }
        else
          last_z_align_move[zstepper] = z_align_abs;

        // Only stop early if all measured points achieve accuracy target
        if (z_align_abs > z_auto_align_accuracy) success_break = false;

        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(zstepper + 1), " corrected by ", z_align_move);

        switch (zstepper) {
          case 0: stepper.set_z_lock(false); break;
          case 1: stepper.set_z2_lock(false); break;
          #if ENABLED(Z_TRIPLE_STEPPER_DRIVERS)
            case 2: stepper.set_z3_lock(false); break;
          #endif
        }

        // This will lose home position and require re-homing
        do_blocking_move_to_z(z_auto_align_amplification * z_align_move + current_position[Z_AXIS]);
      }

      if (err_break) break;

      // Move Z back to previous position
      set_all_z_lock(true);
      do_blocking_move_to_z(z_original_position);
      set_all_z_lock(false);

      stepper.set_separate_multi_axis(false);

      if (success_break) {
        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> achieved target accuracy.");
        break;
      }
    }

    if (err_break) break;

    // Restore the active tool after homing
    #if HOTENDS > 1
      tool_change(old_tool_index, 0, (
        #if ENABLED(PARKING_EXTRUDER)
          false // Fetch the previous toolhead
        #else
          true
        #endif
      ));
    #endif

    #if HAS_LEVELING
      #if ENABLED(RESTORE_LEVELING_AFTER_G34)
        set_bed_leveling_enabled(leveling_was_active);
      #endif
    #endif

    // After this operation the z position needs correction
    set_axis_is_not_at_home(Z_AXIS);

    gcode.G28(false);

  } while(0);

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G34");
}

/**
 * M422: Z-Stepper automatic alignment parameter selection
 */
void GcodeSuite::M422() {
  const int8_t zstepper = parser.intval('S') - 1;
  if (!WITHIN(zstepper, 0, Z_STEPPER_COUNT - 1)) {
    SERIAL_ECHOLNPGM("?(S) Z-Stepper index invalid.");
    return;
  }

  const float x_pos = parser.floatval('X', z_auto_align_xpos[zstepper]);
  if (!WITHIN(x_pos, X_MIN_POS, X_MAX_POS)) {
    SERIAL_ECHOLNPGM("?(X) out of bounds.");
    return;
  }

  const float y_pos = parser.floatval('Y', z_auto_align_ypos[zstepper]);
  if (!WITHIN(y_pos, Y_MIN_POS, Y_MAX_POS)) {
    SERIAL_ECHOLNPGM("?(Y) out of bounds.");
    return;
  }

  z_auto_align_xpos[zstepper] = x_pos;
  z_auto_align_ypos[zstepper] = y_pos;
}

#endif // Z_STEPPER_AUTO_ALIGN
