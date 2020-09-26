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

#if ENABLED(Z_STEPPER_AUTO_ALIGN)

#include "../../feature/z_stepper_align.h"

#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/motion.h"
#include "../../module/probe.h"

#if HAS_MULTI_HOTEND
  #include "../../module/tool_change.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
   #include "../../libs/least_squares_fit.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

/**
 * G34: Z-Stepper automatic alignment
 *
 *   I<iterations>
 *   T<accuracy>
 *   A<amplification>
 *   R<recalculate> points based on current probe offsets
 */
void GcodeSuite::G34() {
  DEBUG_SECTION(log_G34, "G34", DEBUGGING(LEVELING));
  if (DEBUGGING(LEVELING)) log_machine_info();

  do { // break out on error

    #if NUM_Z_STEPPER_DRIVERS == 4
      SERIAL_ECHOLNPGM("Alignment for 4 steppers is Experimental!");
    #elif NUM_Z_STEPPER_DRIVERS > 4
      SERIAL_ECHOLNPGM("Alignment not supported for over 4 steppers");
      break;
    #endif

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

    const float z_auto_align_amplification =
      #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
        Z_STEPPER_ALIGN_AMP;
      #else
        parser.floatval('A', Z_STEPPER_ALIGN_AMP);
        if (!WITHIN(ABS(z_auto_align_amplification), 0.5f, 2.0f)) {
          SERIAL_ECHOLNPGM("?(A)mplification out of bounds (0.5-2.0).");
          break;
        }
      #endif

    if (parser.seen('R')) z_stepper_align.reset_to_default();

    const ProbePtRaise raise_after = parser.boolval('E') ? PROBE_PT_STOW : PROBE_PT_RAISE;

    // Wait for planner moves to finish!
    planner.synchronize();

    // Disable the leveling matrix before auto-aligning
    #if HAS_LEVELING
      TERN_(RESTORE_LEVELING_AFTER_G34, const bool leveling_was_active = planner.leveling_active);
      set_bed_leveling_enabled(false);
    #endif

    TERN_(CNC_WORKSPACE_PLANES, workspace_plane = PLANE_XY);

    // Always home with tool 0 active
    #if HAS_MULTI_HOTEND
      const uint8_t old_tool_index = active_extruder;
      tool_change(0, true);
    #endif

    TERN_(HAS_DUPLICATION_MODE, extruder_duplication_enabled = false);

    // In BLTOUCH HS mode, the probe travels in a deployed state.
    // Users of G34 might have a badly misaligned bed, so raise Z by the
    // length of the deployed pin (BLTOUCH stroke < 7mm)
    #define Z_BASIC_CLEARANCE Z_CLEARANCE_BETWEEN_PROBES + 7.0f * BOTH(BLTOUCH, BLTOUCH_HS_MODE)

    // Compute a worst-case clearance height to probe from. After the first
    // iteration this will be re-calculated based on the actual bed position
    auto magnitude2 = [&](const uint8_t i, const uint8_t j) {
      const xy_pos_t diff = z_stepper_align.xy[i] - z_stepper_align.xy[j];
      return HYPOT2(diff.x, diff.y);
    };
    float z_probe = Z_BASIC_CLEARANCE + (G34_MAX_GRADE) * 0.01f * SQRT(
      #if NUM_Z_STEPPER_DRIVERS == 3
         _MAX(magnitude2(0, 1), magnitude2(1, 2), magnitude2(2, 0))
      #elif NUM_Z_STEPPER_DRIVERS == 4
         _MAX(magnitude2(0, 1), magnitude2(1, 2), magnitude2(2, 3),
              magnitude2(3, 0), magnitude2(0, 2), magnitude2(1, 3))
      #else
         magnitude2(0, 1)
      #endif
    );

    // Home before the alignment procedure
    if (!all_axes_known()) home_all_axes();

    // Move the Z coordinate realm towards the positive - dirty trick
    current_position.z += z_probe * 0.5f;
    sync_plan_position();
    // Now, the Z origin lies below the build plate. That allows to probe deeper, before run_z_probe throws an error.
    // This hack is un-done at the end of G34 - either by re-homing, or by using the probed heights of the last iteration.

    #if DISABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      float last_z_align_move[NUM_Z_STEPPER_DRIVERS] = ARRAY_N(NUM_Z_STEPPER_DRIVERS, 10000.0f, 10000.0f, 10000.0f, 10000.0f);
    #else
      float last_z_align_level_indicator = 10000.0f;
    #endif
    float z_measured[NUM_Z_STEPPER_DRIVERS] = { 0 },
          z_maxdiff = 0.0f,
          amplification = z_auto_align_amplification;

    // These are needed after the for-loop
    uint8_t iteration;
    bool err_break = false;
    float z_measured_min;

    #if DISABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      bool adjustment_reverse = false;
    #endif

    // 'iteration' is declared above and is also used after the for-loop.
    // *not* the same as LOOP_L_N(iteration, z_auto_align_iterations)
    for (iteration = 0; iteration < z_auto_align_iterations; ++iteration) {
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> probing all positions.");

      SERIAL_ECHOLNPAIR("\nITERATION: ", int(iteration + 1));

      // Initialize minimum value
      z_measured_min =  100000.0f;
      float z_measured_max = -100000.0f;

      // Probe all positions (one per Z-Stepper)
      LOOP_L_N(i, NUM_Z_STEPPER_DRIVERS) {
        // iteration odd/even --> downward / upward stepper sequence
        const uint8_t iprobe = (iteration & 1) ? NUM_Z_STEPPER_DRIVERS - 1 - i : i;

        // Safe clearance even on an incline
        if ((iteration == 0 || i > 0) && z_probe > current_position.z) do_blocking_move_to_z(z_probe);

        if (DEBUGGING(LEVELING))
          DEBUG_ECHOLNPAIR_P(PSTR("Probing X"), z_stepper_align.xy[iprobe].x, SP_Y_STR, z_stepper_align.xy[iprobe].y);

        // Probe a Z height for each stepper.
        // Probing sanity check is disabled, as it would trigger even in normal cases because
        // current_position.z has been manually altered in the "dirty trick" above.
        const float z_probed_height = probe.probe_at_point(z_stepper_align.xy[iprobe], raise_after, 0, true, false);
        if (isnan(z_probed_height)) {
          SERIAL_ECHOLNPGM("Probing failed.");
          err_break = true;
          break;
        }

        // Add height to each value, to provide a more useful target height for
        // the next iteration of probing. This allows adjustments to be made away from the bed.
        z_measured[iprobe] = z_probed_height + Z_CLEARANCE_BETWEEN_PROBES;

        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(iprobe + 1), " measured position is ", z_measured[iprobe]);

        // Remember the minimum measurement to calculate the correction later on
        z_measured_min = _MIN(z_measured_min, z_measured[iprobe]);
        z_measured_max = _MAX(z_measured_max, z_measured[iprobe]);
      } // for (i)

      if (err_break) break;

      // Adapt the next probe clearance height based on the new measurements.
      // Safe_height = lowest distance to bed (= highest measurement) plus highest measured misalignment.
      z_maxdiff = z_measured_max - z_measured_min;
      z_probe = Z_BASIC_CLEARANCE + z_measured_max + z_maxdiff;

      #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
        // Replace the initial values in z_measured with calculated heights at
        // each stepper position. This allows the adjustment algorithm to be
        // shared between both possible probing mechanisms.

        // This must be done after the next z_probe height is calculated, so that
        // the height is calculated from actual print area positions, and not
        // extrapolated motor movements.

        // Compute the least-squares fit for all probed points.
        // Calculate the Z position of each stepper and store it in z_measured.
        // This allows the actual adjustment logic to be shared by both algorithms.
        linear_fit_data lfd;
        incremental_LSF_reset(&lfd);
        LOOP_L_N(i, NUM_Z_STEPPER_DRIVERS) {
          SERIAL_ECHOLNPAIR("PROBEPT_", int(i), ": ", z_measured[i]);
          incremental_LSF(&lfd, z_stepper_align.xy[i], z_measured[i]);
        }
        finish_incremental_LSF(&lfd);

        z_measured_min = 100000.0f;
        LOOP_L_N(i, NUM_Z_STEPPER_DRIVERS) {
          z_measured[i] = -(lfd.A * z_stepper_align.stepper_xy[i].x + lfd.B * z_stepper_align.stepper_xy[i].y + lfd.D);
          z_measured_min = _MIN(z_measured_min, z_measured[i]);
        }

        SERIAL_ECHOLNPAIR("CALCULATED STEPPER POSITIONS: Z1=", z_measured[0], " Z2=", z_measured[1], " Z3=", z_measured[2]);
      #endif

      SERIAL_ECHOLNPAIR("\n"
        "DIFFERENCE Z1-Z2=", ABS(z_measured[0] - z_measured[1])
        #if NUM_Z_STEPPER_DRIVERS == 3
          , " Z2-Z3=", ABS(z_measured[1] - z_measured[2])
          , " Z3-Z1=", ABS(z_measured[2] - z_measured[0])
        #endif
      );

      #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
        // Check if the applied corrections go in the correct direction.
        // Calculate the sum of the absolute deviations from the mean of the probe measurements.
        // Compare to the last iteration to ensure it's getting better.

        // Calculate mean value as a reference
        float z_measured_mean = 0.0f;
        LOOP_L_N(zstepper, NUM_Z_STEPPER_DRIVERS) z_measured_mean += z_measured[zstepper];
        z_measured_mean /= NUM_Z_STEPPER_DRIVERS;

        // Calculate the sum of the absolute deviations from the mean value
        float z_align_level_indicator = 0.0f;
        LOOP_L_N(zstepper, NUM_Z_STEPPER_DRIVERS)
          z_align_level_indicator += ABS(z_measured[zstepper] - z_measured_mean);

        // If it's getting worse, stop and throw an error
        if (last_z_align_level_indicator < z_align_level_indicator * 0.7f) {
          SERIAL_ECHOLNPGM("Decreasing accuracy detected.");
          err_break = true;
          break;
        }

        last_z_align_level_indicator = z_align_level_indicator;
      #endif

      // The following correction actions are to be enabled for select Z-steppers only
      stepper.set_separate_multi_axis(true);

      bool success_break = true;
      // Correct the individual stepper offsets
      LOOP_L_N(zstepper, NUM_Z_STEPPER_DRIVERS) {
        // Calculate current stepper move
        float z_align_move = z_measured[zstepper] - z_measured_min;
        const float z_align_abs = ABS(z_align_move);

        #if DISABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
          // Optimize one iteration's correction based on the first measurements
          if (z_align_abs) amplification = (iteration == 1) ? _MIN(last_z_align_move[zstepper] / z_align_abs, 2.0f) : z_auto_align_amplification;

          // Check for less accuracy compared to last move
          if (last_z_align_move[zstepper] < z_align_abs * 0.7f) {
            SERIAL_ECHOLNPGM("Decreasing accuracy detected.");
            if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(zstepper + 1), " last_z_align_move = ", last_z_align_move[zstepper]);
            if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(zstepper + 1), " z_align_abs = ", z_align_abs);
            adjustment_reverse = !adjustment_reverse;
          }

          // Remember the alignment for the next iteration, but only if steppers move,
          // otherwise it would be just zero (in case this stepper was at z_measured_min already)
          if (z_align_abs > 0) last_z_align_move[zstepper] = z_align_abs;
        #endif

        // Stop early if all measured points achieve accuracy target
        if (z_align_abs > z_auto_align_accuracy) success_break = false;

        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(zstepper + 1), " corrected by ", z_align_move);

        // Lock all steppers except one
        stepper.set_all_z_lock(true, zstepper);

        #if DISABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
          // Decreasing accuracy was detected so move was inverted.
          // Will match reversed Z steppers on dual steppers. Triple will need more work to map.
          if (adjustment_reverse) {
            z_align_move = -z_align_move;
            if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(zstepper + 1), " correction reversed to ", z_align_move);
          }
        #endif

        // Do a move to correct part of the misalignment for the current stepper
        do_blocking_move_to_z(amplification * z_align_move + current_position.z);
      } // for (zstepper)

      // Back to normal stepper operations
      stepper.set_all_z_lock(false);
      stepper.set_separate_multi_axis(false);

      if (err_break) break;

      if (success_break) { SERIAL_ECHOLNPGM("Target accuracy achieved."); break; }

    } // for (iteration)

    if (err_break)
      SERIAL_ECHOLNPGM("G34 aborted.");
    else {
      SERIAL_ECHOLNPAIR("Did ", int(iteration + (iteration != z_auto_align_iterations)), " of ", int(z_auto_align_iterations));
      SERIAL_ECHOLNPAIR_F("Accuracy: ", z_maxdiff);
    }

    // Stow the probe, as the last call to probe.probe_at_point(...) left
    // the probe deployed if it was successful.
    probe.stow();

    #if ENABLED(HOME_AFTER_G34)
      // After this operation the z position needs correction
      set_axis_never_homed(Z_AXIS);
      // Home Z after the alignment procedure
      process_subcommands_now_P(PSTR("G28Z"));
    #else
      // Use the probed height from the last iteration to determine the Z height.
      // z_measured_min is used, because all steppers are aligned to z_measured_min.
      // Ideally, this would be equal to the 'z_probe * 0.5f' which was added earlier.
      current_position.z -= z_measured_min - (float)Z_CLEARANCE_BETWEEN_PROBES;
      sync_plan_position();
    #endif

    // Restore the active tool after homing
    TERN_(HAS_MULTI_HOTEND, tool_change(old_tool_index, DISABLED(PARKING_EXTRUDER))); // Fetch previous tool for parking extruder

    #if BOTH(HAS_LEVELING, RESTORE_LEVELING_AFTER_G34)
      set_bed_leveling_enabled(leveling_was_active);
    #endif

  }while(0);
}

/**
 * M422: Set a Z-Stepper automatic alignment XY point.
 *       Use repeatedly to set multiple points.
 *
 *   S<index> : Index of the probe point to set
 *
 * With Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS:
 *   W<index> : Index of the Z stepper position to set
 *              The W and S parameters may not be combined.
 *
 * S and W require an X and/or Y parameter
 *   X<pos>   : X position to set (Unchanged if omitted)
 *   Y<pos>   : Y position to set (Unchanged if omitted)
 *
 * R : Recalculate points based on current probe offsets
 */
void GcodeSuite::M422() {

  if (parser.seen('R')) {
    z_stepper_align.reset_to_default();
    return;
  }

  if (!parser.seen_any()) {
    LOOP_L_N(i, NUM_Z_STEPPER_DRIVERS)
      SERIAL_ECHOLNPAIR_P(PSTR("M422 S"), int(i + 1), SP_X_STR, z_stepper_align.xy[i].x, SP_Y_STR, z_stepper_align.xy[i].y);
    #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      LOOP_L_N(i, NUM_Z_STEPPER_DRIVERS)
        SERIAL_ECHOLNPAIR_P(PSTR("M422 W"), int(i + 1), SP_X_STR, z_stepper_align.stepper_xy[i].x, SP_Y_STR, z_stepper_align.stepper_xy[i].y);
    #endif
    return;
  }

  const bool is_probe_point = parser.seen('S');

  #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
    if (is_probe_point && parser.seen('W')) {
      SERIAL_ECHOLNPGM("?(S) and (W) may not be combined.");
      return;
    }
  #endif

  xy_pos_t *pos_dest = (
    #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      !is_probe_point ? z_stepper_align.stepper_xy :
    #endif
    z_stepper_align.xy
  );

  if (!is_probe_point
    #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      && !parser.seen('W')
    #endif
  ) {
    SERIAL_ECHOLNPGM(
      #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
        "?(S) or (W) is required."
      #else
        "?(S) is required."
      #endif
    );
    return;
  }

  // Get the Probe Position Index or Z Stepper Index
  int8_t position_index;
  if (is_probe_point) {
    position_index = parser.intval('S') - 1;
    if (!WITHIN(position_index, 0, int8_t(NUM_Z_STEPPER_DRIVERS) - 1)) {
      SERIAL_ECHOLNPGM("?(S) Z-ProbePosition index invalid.");
      return;
    }
  }
  else {
    #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      position_index = parser.intval('W') - 1;
      if (!WITHIN(position_index, 0, NUM_Z_STEPPER_DRIVERS - 1)) {
        SERIAL_ECHOLNPGM("?(W) Z-Stepper index invalid.");
        return;
      }
    #endif
  }

  const xy_pos_t pos = {
    parser.floatval('X', pos_dest[position_index].x),
    parser.floatval('Y', pos_dest[position_index].y)
  };

  if (is_probe_point) {
    if (!probe.can_reach(pos.x, Y_CENTER)) {
      SERIAL_ECHOLNPGM("?(X) out of bounds.");
      return;
    }
    if (!probe.can_reach(pos)) {
      SERIAL_ECHOLNPGM("?(Y) out of bounds.");
      return;
    }
  }

  pos_dest[position_index] = pos;
}

#endif // Z_STEPPER_AUTO_ALIGN
