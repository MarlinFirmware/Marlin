/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(Z_STEPPER_AUTO_ALIGN)

#include "../gcode.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/motion.h"
#include "../../module/probe.h"

#if HOTENDS > 1
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

//
// Sanity check G34 / M422 settings
//
constexpr xy_pos_t test_z_stepper_align_xy[] = Z_STEPPER_ALIGN_XY;

#if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)

  static_assert(COUNT(test_z_stepper_align_xy) >= NUM_Z_STEPPER_DRIVERS,
    "Z_STEPPER_ALIGN_XY requires at least three {X,Y} entries (Z, Z2, Z3, ...)."
  );

  constexpr float test_z_stepper_align_stepper_xy[][XY] = Z_STEPPER_ALIGN_STEPPER_XY;
  static_assert(
    COUNT(test_z_stepper_align_stepper_xy) == NUM_Z_STEPPER_DRIVERS,
    "Z_STEPPER_ALIGN_STEPPER_XY requires three {X,Y} entries (one per Z stepper)."
  );

#else

  static_assert(COUNT(test_z_stepper_align_xy) == NUM_Z_STEPPER_DRIVERS,
    #if NUM_Z_STEPPER_DRIVERS == 4
      "Z_STEPPER_ALIGN_XY requires four {X,Y} entries (Z, Z2, Z3, and Z4)."
    #elif NUM_Z_STEPPER_DRIVERS == 3
      "Z_STEPPER_ALIGN_XY requires three {X,Y} entries (Z, Z2, and Z3)."
    #else
      "Z_STEPPER_ALIGN_XY requires two {X,Y} entries (Z and Z2)."
    #endif
  );

#endif

constexpr xyz_pos_t dpo = NOZZLE_TO_PROBE_OFFSET;

#define LTEST(N) (test_z_stepper_align_xy[N].x >= _MAX(X_MIN_BED + MIN_PROBE_EDGE_LEFT,  X_MIN_POS + dpo.x) - 0.00001f)
#define RTEST(N) (test_z_stepper_align_xy[N].x <= _MIN(X_MAX_BED - MIN_PROBE_EDGE_RIGHT, X_MAX_POS + dpo.x) + 0.00001f)
#define FTEST(N) (test_z_stepper_align_xy[N].y >= _MAX(Y_MIN_BED + MIN_PROBE_EDGE_FRONT, Y_MIN_POS + dpo.y) - 0.00001f)
#define BTEST(N) (test_z_stepper_align_xy[N].y <= _MIN(Y_MAX_BED - MIN_PROBE_EDGE_BACK,  Y_MAX_POS + dpo.y) + 0.00001f)

static_assert(LTEST(0) && RTEST(0), "The 1st Z_STEPPER_ALIGN_XY X is unreachable with the default probe X offset.");
static_assert(FTEST(0) && BTEST(0), "The 1st Z_STEPPER_ALIGN_XY Y is unreachable with the default probe Y offset.");
static_assert(LTEST(1) && RTEST(1), "The 2nd Z_STEPPER_ALIGN_XY X is unreachable with the default probe X offset.");
static_assert(FTEST(1) && BTEST(1), "The 2nd Z_STEPPER_ALIGN_XY Y is unreachable with the default probe Y offset.");
#if NUM_Z_STEPPER_DRIVERS >= 3
  static_assert(LTEST(2) && RTEST(2), "The 3rd Z_STEPPER_ALIGN_XY X is unreachable with the default probe X offset.");
  static_assert(FTEST(2) && BTEST(2), "The 3rd Z_STEPPER_ALIGN_XY Y is unreachable with the default probe Y offset.");
  #if NUM_Z_STEPPER_DRIVERS >= 4
    static_assert(LTEST(3) && RTEST(3), "The 4th Z_STEPPER_ALIGN_XY X is unreachable with the default probe X offset.");
    static_assert(FTEST(3) && BTEST(3), "The 4th Z_STEPPER_ALIGN_XY Y is unreachable with the default probe Y offset.");
  #endif
#endif

//
// G34 / M422 shared data
//
static xy_pos_t z_stepper_align_pos[] = Z_STEPPER_ALIGN_XY;

#if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
  static xy_pos_t z_stepper_align_stepper_pos[] = Z_STEPPER_ALIGN_STEPPER_XY;
#endif

#define G34_PROBE_COUNT COUNT(z_stepper_align_pos)

inline void set_all_z_lock(const bool lock) {
  stepper.set_z_lock(lock);
  stepper.set_z2_lock(lock);
  #if NUM_Z_STEPPER_DRIVERS >= 3
    stepper.set_z3_lock(lock);
    #if NUM_Z_STEPPER_DRIVERS >= 4
      stepper.set_z4_lock(lock);
    #endif
  #endif
}

/**
 * G34: Z-Stepper automatic alignment
 *
 *   I<iterations>
 *   T<accuracy>
 *   A<amplification>
 */
void GcodeSuite::G34() {
  if (DEBUGGING(LEVELING)) {
    DEBUG_ECHOLNPGM(">>> G34");
    log_machine_info();
  }

  do { // break out on error

    #if NUM_Z_STEPPER_DRIVERS == 4
      SERIAL_ECHOLNPGM("Quad Z Stepper Leveling not Yet Supported");
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

    const ProbePtRaise raise_after = parser.boolval('E') ? PROBE_PT_STOW : PROBE_PT_RAISE;

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

    // Always home with tool 0 active
    #if HOTENDS > 1
      const uint8_t old_tool_index = active_extruder;
      tool_change(0, true);
    #endif

    #if HAS_DUPLICATION_MODE
      extruder_duplication_enabled = false;
    #endif

    #if BOTH(BLTOUCH, BLTOUCH_HS_MODE)
        // In BLTOUCH HS mode, the probe travels in a deployed state.
        // Users of G34 might have a badly misaligned bed, so raise Z by the
        // length of the deployed pin (BLTOUCH stroke < 7mm)
      #define Z_BASIC_CLEARANCE Z_CLEARANCE_BETWEEN_PROBES + 7.0f
    #else
      #define Z_BASIC_CLEARANCE Z_CLEARANCE_BETWEEN_PROBES
    #endif

    // Compute a worst-case clearance height to probe from. After the first
    // iteration this will be re-calculated based on the actual bed position
    float z_probe = Z_BASIC_CLEARANCE + (G34_MAX_GRADE) * 0.01f * (
      #if NUM_Z_STEPPER_DRIVERS == 3
         SQRT(_MAX(HYPOT2(z_stepper_align_pos[0].x - z_stepper_align_pos[0].y, z_stepper_align_pos[1].x - z_stepper_align_pos[1].y),
                   HYPOT2(z_stepper_align_pos[1].x - z_stepper_align_pos[1].y, z_stepper_align_pos[2].x - z_stepper_align_pos[2].y),
                   HYPOT2(z_stepper_align_pos[2].x - z_stepper_align_pos[2].y, z_stepper_align_pos[0].x - z_stepper_align_pos[0].y)))
      #else
         HYPOT(z_stepper_align_pos[0].x - z_stepper_align_pos[0].y, z_stepper_align_pos[1].x - z_stepper_align_pos[1].y)
      #endif
    );

    // Home before the alignment procedure
    if (!all_axes_known()) home_all_axes();

    // Move the Z coordinate realm towards the positive - dirty trick
    current_position.z -= z_probe * 0.5f;

    float last_z_align_move[NUM_Z_STEPPER_DRIVERS] = ARRAY_N(NUM_Z_STEPPER_DRIVERS, 10000.0f, 10000.0f, 10000.0f),
          z_measured[G34_PROBE_COUNT] = { 0 },
          z_maxdiff = 0.0f,
          amplification = z_auto_align_amplification;

    uint8_t iteration;
    bool err_break = false;
    for (iteration = 0; iteration < z_auto_align_iterations; ++iteration) {
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("> probing all positions.");

      SERIAL_ECHOLNPAIR("\nITERATION: ", int(iteration + 1));

      // Initialize minimum value
      float z_measured_min = 100000.0f,
            z_measured_max = -100000.0f;

      // Probe all positions (one per Z-Stepper)
      for (uint8_t i = 0; i < G34_PROBE_COUNT; ++i) {
        // iteration odd/even --> downward / upward stepper sequence
        const uint8_t iprobe = (iteration & 1) ? G34_PROBE_COUNT - 1 - i : i;

        // Safe clearance even on an incline
        if (iteration == 0 || i > 0) do_blocking_move_to_z(z_probe);

        // Probe a Z height for each stepper.
        const float z_probed_height = probe_at_point(z_stepper_align_pos[iprobe], raise_after, 0, true);
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
        for (uint8_t i = 0; i < G34_PROBE_COUNT; ++i) {
          SERIAL_ECHOLNPAIR("PROBEPT_", int(i + 1), ": ", z_measured[i]);
          incremental_LSF(&lfd, z_stepper_align_pos[i], z_measured[i]);
        }
        finish_incremental_LSF(&lfd);

        z_measured_min = 100000.0f;
        for (uint8_t i = 0; i < NUM_Z_STEPPER_DRIVERS; ++i) {
          z_measured[i] = -(lfd.A * z_stepper_align_stepper_pos[i].x + lfd.B * z_stepper_align_stepper_pos[i].y);
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

      // The following correction actions are to be enabled for select Z-steppers only
      stepper.set_separate_multi_axis(true);

      bool success_break = true;
      // Correct the individual stepper offsets
      for (uint8_t zstepper = 0; zstepper < NUM_Z_STEPPER_DRIVERS; ++zstepper) {
        // Calculate current stepper move
        const float z_align_move = z_measured[zstepper] - z_measured_min,
                    z_align_abs = ABS(z_align_move);

        #if DISABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
          // Optimize one iteration's correction based on the first measurements
          if (z_align_abs > 0.0f) amplification = iteration == 1 ? _MIN(last_z_align_move[zstepper] / z_align_abs, 2.0f) : z_auto_align_amplification;
        #endif

        // Check for less accuracy compared to last move
        if (last_z_align_move[zstepper] < z_align_abs - 1.0) {
          SERIAL_ECHOLNPGM("Decreasing accuracy detected.");
          err_break = true;
          break;
        }

        // Remember the alignment for the next iteration
        last_z_align_move[zstepper] = z_align_abs;

        // Stop early if all measured points achieve accuracy target
        if (z_align_abs > z_auto_align_accuracy) success_break = false;

        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("> Z", int(zstepper + 1), " corrected by ", z_align_move);

        // Lock all steppers except one
        set_all_z_lock(true);
        switch (zstepper) {
          case 0: stepper.set_z_lock(false); break;
          case 1: stepper.set_z2_lock(false); break;
          #if NUM_Z_STEPPER_DRIVERS == 3
            case 2: stepper.set_z3_lock(false); break;
          #endif
        }

        // Do a move to correct part of the misalignment for the current stepper
        do_blocking_move_to_z(amplification * z_align_move + current_position.z);
      } // for (zstepper)

      // Back to normal stepper operations
      set_all_z_lock(false);
      stepper.set_separate_multi_axis(false);

      if (err_break) break;

      if (success_break) { SERIAL_ECHOLNPGM("Target accuracy achieved."); break; }

    } // for (iteration)

    if (err_break) { SERIAL_ECHOLNPGM("G34 aborted."); break; }

    SERIAL_ECHOLNPAIR("Did ", int(iteration + (iteration != z_auto_align_iterations)), " iterations of ", int(z_auto_align_iterations));
    SERIAL_ECHOLNPAIR_F("Accuracy: ", z_maxdiff);

    // Restore the active tool after homing
    #if HOTENDS > 1
      tool_change(old_tool_index, (
        #if ENABLED(PARKING_EXTRUDER)
          false // Fetch the previous toolhead
        #else
          true
        #endif
      ));
    #endif

    #if HAS_LEVELING && ENABLED(RESTORE_LEVELING_AFTER_G34)
      set_bed_leveling_enabled(leveling_was_active);
    #endif

    // After this operation the z position needs correction
    set_axis_is_not_at_home(Z_AXIS);

    // Stow the probe, as the last call to probe_at_point(...) left
    // the probe deployed if it was successful.
    STOW_PROBE();

    // Home Z after the alignment procedure
    process_subcommands_now_P(PSTR("G28 Z"));

  }while(0);

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("<<< G34");
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
 */
void GcodeSuite::M422() {
  if (!parser.seen_any()) {
    for (uint8_t i = 0; i < G34_PROBE_COUNT; ++i)
      SERIAL_ECHOLNPAIR_P(PSTR("M422 S"), i + 1, SP_X_STR, z_stepper_align_pos[i].x, SP_Y_STR, z_stepper_align_pos[i].y);
    #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
      for (uint8_t i = 0; i < NUM_Z_STEPPER_DRIVERS; ++i)
        SERIAL_ECHOLNPAIR_P(PSTR("M422 W"), i + 1, SP_X_STR, z_stepper_align_stepper_pos[i].x, SP_Y_STR, z_stepper_align_stepper_pos[i].y);
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
      !is_probe_point ? z_stepper_align_stepper_pos :
    #endif
    z_stepper_align_pos
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
    if (!WITHIN(position_index, 0, int8_t(G34_PROBE_COUNT) - 1)) {
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
    if (!position_is_reachable_by_probe(pos.x, Y_CENTER)) {
      SERIAL_ECHOLNPGM("?(X) out of bounds.");
      return;
    }
    if (!position_is_reachable_by_probe(pos)) {
      SERIAL_ECHOLNPGM("?(Y) out of bounds.");
      return;
    }
  }

  pos_dest[position_index] = pos;
}

#endif // Z_STEPPER_AUTO_ALIGN
