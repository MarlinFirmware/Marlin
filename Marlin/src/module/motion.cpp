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
 * motion.cpp
 */

#include "motion.h"

#include "../gcode/gcode.h"
// #include "../module/planner.h"
// #include "../Marlin.h"
// #include "../inc/MarlinConfig.h"

#include "../core/serial.h"
#include "../module/stepper.h"
#include "../module/temperature.h"

#if IS_SCARA
  #include "../libs/buzzer.h"
  #include "../lcd/ultralcd.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../feature/ubl/ubl.h"
#endif

#define XYZ_CONSTS(type, array, CONFIG) const PROGMEM type array##_P[XYZ] = { X_##CONFIG, Y_##CONFIG, Z_##CONFIG }

XYZ_CONSTS(float, base_min_pos,   MIN_POS);
XYZ_CONSTS(float, base_max_pos,   MAX_POS);
XYZ_CONSTS(float, base_home_pos,  HOME_POS);
XYZ_CONSTS(float, max_length,     MAX_LENGTH);
XYZ_CONSTS(float, home_bump_mm,   HOME_BUMP_MM);
XYZ_CONSTS(signed char, home_dir, HOME_DIR);

// Relative Mode. Enable with G91, disable with G90.
bool relative_mode = false;

/**
 * Cartesian Current Position
 *   Used to track the logical position as moves are queued.
 *   Used by 'line_to_current_position' to do a move after changing it.
 *   Used by 'SYNC_PLAN_POSITION_KINEMATIC' to update 'planner.position'.
 */
float current_position[XYZE] = { 0.0 };

/**
 * Cartesian Destination
 *   A temporary position, usually applied to 'current_position'.
 *   Set with 'get_destination_from_command' or 'set_destination_to_current'.
 *   'line_to_destination' sets 'current_position' to 'destination'.
 */
float destination[XYZE] = { 0.0 };

// The active extruder (tool). Set with T<extruder> command.
uint8_t active_extruder = 0;

// The feedrate for the current move, often used as the default if
// no other feedrate is specified. Overridden for special moves.
// Set by the last G0 through G5 command's "F" parameter.
// Functions that override this for custom moves *must always* restore it!
float feedrate_mm_s = MMM_TO_MMS(1500.0);

/**
 * sync_plan_position
 *
 * Set the planner/stepper positions directly from current_position with
 * no kinematic translation. Used for homing axes and cartesian/core syncing.
 */
void sync_plan_position() {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("sync_plan_position", current_position);
  #endif
  planner.set_position_mm(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);
}

void sync_plan_position_e() { planner.set_e_position_mm(current_position[E_AXIS]); }

/**
 * Move the planner to the current position from wherever it last moved
 * (or from wherever it has been told it is located).
 */
void line_to_current_position() {
  planner.buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate_mm_s, active_extruder);
}

/**
 * Move the planner to the position stored in the destination array, which is
 * used by G0/G1/G2/G3/G5 and many other functions to set a destination.
 */
void line_to_destination(const float fr_mm_s) {
  planner.buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], fr_mm_s, active_extruder);
}

#if IS_KINEMATIC

  void sync_plan_position_kinematic() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("sync_plan_position_kinematic", current_position);
    #endif
    planner.set_position_mm_kinematic(current_position);
  }

  /**
   * Calculate delta, start a line, and set current_position to destination
   */
  void prepare_uninterpolated_move_to_destination(const float fr_mm_s/*=0.0*/) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("prepare_uninterpolated_move_to_destination", destination);
    #endif

    gcode.refresh_cmd_timeout();

    #if UBL_DELTA
      // ubl segmented line will do z-only moves in single segment
      ubl.prepare_segmented_line_to(destination, MMS_SCALED(fr_mm_s ? fr_mm_s : feedrate_mm_s));
    #else
      if ( current_position[X_AXIS] == destination[X_AXIS]
        && current_position[Y_AXIS] == destination[Y_AXIS]
        && current_position[Z_AXIS] == destination[Z_AXIS]
        && current_position[E_AXIS] == destination[E_AXIS]
      ) return;

      planner.buffer_line_kinematic(destination, MMS_SCALED(fr_mm_s ? fr_mm_s : feedrate_mm_s), active_extruder);
    #endif

    set_current_to_destination();
  }

#endif // IS_KINEMATIC

// Software Endstops are based on the configured limits.
float soft_endstop_min[XYZ] = { X_MIN_BED, Y_MIN_BED, Z_MIN_POS },
      soft_endstop_max[XYZ] = { X_MAX_BED, Y_MAX_BED, Z_MAX_POS };

#if HAS_SOFTWARE_ENDSTOPS

  // Software Endstops are based on the configured limits.
  bool soft_endstops_enabled = true;

  /**
   * Constrain the given coordinates to the software endstops.
   */

  // NOTE: This makes no sense for delta beds other than Z-axis.
  //       For delta the X/Y would need to be clamped at
  //       DELTA_PRINTABLE_RADIUS from center of bed, but delta
  //       now enforces is_position_reachable for X/Y regardless
  //       of HAS_SOFTWARE_ENDSTOPS, so that enforcement would be
  //       redundant here.

  void clamp_to_software_endstops(float target[XYZ]) {
    if (!soft_endstops_enabled) return;
    #if ENABLED(MIN_SOFTWARE_ENDSTOPS)
      #if DISABLED(DELTA)
        NOLESS(target[X_AXIS], soft_endstop_min[X_AXIS]);
        NOLESS(target[Y_AXIS], soft_endstop_min[Y_AXIS]);
      #endif
      NOLESS(target[Z_AXIS], soft_endstop_min[Z_AXIS]);
    #endif
    #if ENABLED(MAX_SOFTWARE_ENDSTOPS)
      #if DISABLED(DELTA)
        NOMORE(target[X_AXIS], soft_endstop_max[X_AXIS]);
        NOMORE(target[Y_AXIS], soft_endstop_max[Y_AXIS]);
      #endif
      NOMORE(target[Z_AXIS], soft_endstop_max[Z_AXIS]);
    #endif
  }

#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR) && !IS_KINEMATIC

  #define CELL_INDEX(A,V) ((RAW_##A##_POSITION(V) - bilinear_start[A##_AXIS]) * ABL_BG_FACTOR(A##_AXIS))

  /**
   * Prepare a bilinear-leveled linear move on Cartesian,
   * splitting the move where it crosses grid borders.
   */
  void bilinear_line_to_destination(const float fr_mm_s, uint16_t x_splits=0xFFFF, uint16_t y_splits=0xFFFF);
    int cx1 = CELL_INDEX(X, current_position[X_AXIS]),
        cy1 = CELL_INDEX(Y, current_position[Y_AXIS]),
        cx2 = CELL_INDEX(X, destination[X_AXIS]),
        cy2 = CELL_INDEX(Y, destination[Y_AXIS]);
    cx1 = constrain(cx1, 0, ABL_BG_POINTS_X - 2);
    cy1 = constrain(cy1, 0, ABL_BG_POINTS_Y - 2);
    cx2 = constrain(cx2, 0, ABL_BG_POINTS_X - 2);
    cy2 = constrain(cy2, 0, ABL_BG_POINTS_Y - 2);

    if (cx1 == cx2 && cy1 == cy2) {
      // Start and end on same mesh square
      line_to_destination(fr_mm_s);
      set_current_to_destination();
      return;
    }

    #define LINE_SEGMENT_END(A) (current_position[A ##_AXIS] + (destination[A ##_AXIS] - current_position[A ##_AXIS]) * normalized_dist)

    float normalized_dist, end[XYZE];

    // Split at the left/front border of the right/top square
    const int8_t gcx = max(cx1, cx2), gcy = max(cy1, cy2);
    if (cx2 != cx1 && TEST(x_splits, gcx)) {
      COPY(end, destination);
      destination[X_AXIS] = LOGICAL_X_POSITION(bilinear_start[X_AXIS] + ABL_BG_SPACING(X_AXIS) * gcx);
      normalized_dist = (destination[X_AXIS] - current_position[X_AXIS]) / (end[X_AXIS] - current_position[X_AXIS]);
      destination[Y_AXIS] = LINE_SEGMENT_END(Y);
      CBI(x_splits, gcx);
    }
    else if (cy2 != cy1 && TEST(y_splits, gcy)) {
      COPY(end, destination);
      destination[Y_AXIS] = LOGICAL_Y_POSITION(bilinear_start[Y_AXIS] + ABL_BG_SPACING(Y_AXIS) * gcy);
      normalized_dist = (destination[Y_AXIS] - current_position[Y_AXIS]) / (end[Y_AXIS] - current_position[Y_AXIS]);
      destination[X_AXIS] = LINE_SEGMENT_END(X);
      CBI(y_splits, gcy);
    }
    else {
      // Already split on a border
      line_to_destination(fr_mm_s);
      set_current_to_destination();
      return;
    }

    destination[Z_AXIS] = LINE_SEGMENT_END(Z);
    destination[E_AXIS] = LINE_SEGMENT_END(E);

    // Do the split and look for more borders
    bilinear_line_to_destination(fr_mm_s, x_splits, y_splits);

    // Restore destination from stack
    COPY(destination, end);
    bilinear_line_to_destination(fr_mm_s, x_splits, y_splits);
  }

#endif // AUTO_BED_LEVELING_BILINEAR

#if IS_KINEMATIC && !UBL_DELTA

  /**
   * Prepare a linear move in a DELTA or SCARA setup.
   *
   * This calls planner.buffer_line several times, adding
   * small incremental moves for DELTA or SCARA.
   */
  inline bool prepare_kinematic_move_to(float ltarget[XYZE]) {

    // Get the top feedrate of the move in the XY plane
    const float _feedrate_mm_s = MMS_SCALED(feedrate_mm_s);

    // If the move is only in Z/E don't split up the move
    if (ltarget[X_AXIS] == current_position[X_AXIS] && ltarget[Y_AXIS] == current_position[Y_AXIS]) {
      planner.buffer_line_kinematic(ltarget, _feedrate_mm_s, active_extruder);
      return false;
    }

    // Fail if attempting move outside printable radius
    if (!position_is_reachable_xy(ltarget[X_AXIS], ltarget[Y_AXIS])) return true;

    // Get the cartesian distances moved in XYZE
    const float difference[XYZE] = {
      ltarget[X_AXIS] - current_position[X_AXIS],
      ltarget[Y_AXIS] - current_position[Y_AXIS],
      ltarget[Z_AXIS] - current_position[Z_AXIS],
      ltarget[E_AXIS] - current_position[E_AXIS]
    };

    // Get the linear distance in XYZ
    float cartesian_mm = SQRT(sq(difference[X_AXIS]) + sq(difference[Y_AXIS]) + sq(difference[Z_AXIS]));

    // If the move is very short, check the E move distance
    if (UNEAR_ZERO(cartesian_mm)) cartesian_mm = FABS(difference[E_AXIS]);

    // No E move either? Game over.
    if (UNEAR_ZERO(cartesian_mm)) return true;

    // Minimum number of seconds to move the given distance
    const float seconds = cartesian_mm / _feedrate_mm_s;

    // The number of segments-per-second times the duration
    // gives the number of segments
    uint16_t segments = delta_segments_per_second * seconds;

    // For SCARA minimum segment size is 0.25mm
    #if IS_SCARA
      NOMORE(segments, cartesian_mm * 4);
    #endif

    // At least one segment is required
    NOLESS(segments, 1);

    // The approximate length of each segment
    const float inv_segments = 1.0 / float(segments),
                segment_distance[XYZE] = {
                  difference[X_AXIS] * inv_segments,
                  difference[Y_AXIS] * inv_segments,
                  difference[Z_AXIS] * inv_segments,
                  difference[E_AXIS] * inv_segments
                };

    // SERIAL_ECHOPAIR("mm=", cartesian_mm);
    // SERIAL_ECHOPAIR(" seconds=", seconds);
    // SERIAL_ECHOLNPAIR(" segments=", segments);

    #if IS_SCARA && ENABLED(SCARA_FEEDRATE_SCALING)
      // SCARA needs to scale the feed rate from mm/s to degrees/s
      const float inv_segment_length = min(10.0, float(segments) / cartesian_mm), // 1/mm/segs
                  feed_factor = inv_segment_length * _feedrate_mm_s;
      float oldA = stepper.get_axis_position_degrees(A_AXIS),
            oldB = stepper.get_axis_position_degrees(B_AXIS);
    #endif

    // Get the logical current position as starting point
    float logical[XYZE];
    COPY(logical, current_position);

    // Drop one segment so the last move is to the exact target.
    // If there's only 1 segment, loops will be skipped entirely.
    --segments;

    // Calculate and execute the segments
    for (uint16_t s = segments + 1; --s;) {
      LOOP_XYZE(i) logical[i] += segment_distance[i];
      #if ENABLED(DELTA)
        DELTA_LOGICAL_IK(); // Delta can inline its kinematics
      #else
        inverse_kinematics(logical);
      #endif

      ADJUST_DELTA(logical); // Adjust Z if bed leveling is enabled

      #if IS_SCARA && ENABLED(SCARA_FEEDRATE_SCALING)
        // For SCARA scale the feed rate from mm/s to degrees/s
        // Use ratio between the length of the move and the larger angle change
        const float adiff = abs(delta[A_AXIS] - oldA),
                    bdiff = abs(delta[B_AXIS] - oldB);
        planner.buffer_line(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], logical[E_AXIS], max(adiff, bdiff) * feed_factor, active_extruder);
        oldA = delta[A_AXIS];
        oldB = delta[B_AXIS];
      #else
        planner.buffer_line(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], logical[E_AXIS], _feedrate_mm_s, active_extruder);
      #endif
    }

    // Since segment_distance is only approximate,
    // the final move must be to the exact destination.

    #if IS_SCARA && ENABLED(SCARA_FEEDRATE_SCALING)
      // For SCARA scale the feed rate from mm/s to degrees/s
      // With segments > 1 length is 1 segment, otherwise total length
      inverse_kinematics(ltarget);
      ADJUST_DELTA(ltarget);
      const float adiff = abs(delta[A_AXIS] - oldA),
                  bdiff = abs(delta[B_AXIS] - oldB);
      planner.buffer_line(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], logical[E_AXIS], max(adiff, bdiff) * feed_factor, active_extruder);
    #else
      planner.buffer_line_kinematic(ltarget, _feedrate_mm_s, active_extruder);
    #endif

    return false;
  }

#else // !IS_KINEMATIC || UBL_DELTA

  /**
   * Prepare a linear move in a Cartesian setup.
   * If Mesh Bed Leveling is enabled, perform a mesh move.
   *
   * Returns true if the caller didn't update current_position.
   */
  inline bool prepare_move_to_destination_cartesian() {
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      const float fr_scaled = MMS_SCALED(feedrate_mm_s);
      if (ubl.state.active) { // direct use of ubl.state.active for speed
        ubl.line_to_destination_cartesian(fr_scaled, active_extruder);
        return true;
      }
      else
        line_to_destination(fr_scaled);
    #else
      // Do not use feedrate_percentage for E or Z only moves
      if (current_position[X_AXIS] == destination[X_AXIS] && current_position[Y_AXIS] == destination[Y_AXIS])
        line_to_destination();
      else {
        const float fr_scaled = MMS_SCALED(feedrate_mm_s);
        #if ENABLED(MESH_BED_LEVELING)
          if (mbl.active()) { // direct used of mbl.active() for speed
            mesh_line_to_destination(fr_scaled);
            return true;
          }
          else
        #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
          if (planner.abl_enabled) { // direct use of abl_enabled for speed
            bilinear_line_to_destination(fr_scaled);
            return true;
          }
          else
        #endif
            line_to_destination(fr_scaled);
      }
    #endif
    return false;
  }

#endif // !IS_KINEMATIC || UBL_DELTA

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  bool extruder_duplication_enabled = false;                              // Used in Dual X mode 2
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  DualXMode dual_x_carriage_mode         = DEFAULT_DUAL_X_CARRIAGE_MODE;
  float inactive_extruder_x_pos          = X2_MAX_POS,                    // used in mode 0 & 1
        raised_parked_position[XYZE],                                     // used in mode 1
        duplicate_extruder_x_offset      = DEFAULT_DUPLICATION_X_OFFSET;  // used in mode 2
  bool active_extruder_parked            = false;                         // used in mode 1 & 2
  millis_t delayed_move_time             = 0;                             // used in mode 1
  int16_t duplicate_extruder_temp_offset = 0;                             // used in mode 2

  float x_home_pos(const int extruder) {
    if (extruder == 0)
      return LOGICAL_X_POSITION(base_home_pos(X_AXIS));
    else
      /**
       * In dual carriage mode the extruder offset provides an override of the
       * second X-carriage position when homed - otherwise X2_HOME_POS is used.
       * This allows soft recalibration of the second extruder home position
       * without firmware reflash (through the M218 command).
       */
      return LOGICAL_X_POSITION(hotend_offset[X_AXIS][1] > 0 ? hotend_offset[X_AXIS][1] : X2_HOME_POS);
  }

  /**
   * Prepare a linear move in a dual X axis setup
   */
  inline bool prepare_move_to_destination_dualx() {
    if (active_extruder_parked) {
      switch (dual_x_carriage_mode) {
        case DXC_FULL_CONTROL_MODE:
          break;
        case DXC_AUTO_PARK_MODE:
          if (current_position[E_AXIS] == destination[E_AXIS]) {
            // This is a travel move (with no extrusion)
            // Skip it, but keep track of the current position
            // (so it can be used as the start of the next non-travel move)
            if (delayed_move_time != 0xFFFFFFFFUL) {
              set_current_to_destination();
              NOLESS(raised_parked_position[Z_AXIS], destination[Z_AXIS]);
              delayed_move_time = millis();
              return true;
            }
          }
          // unpark extruder: 1) raise, 2) move into starting XY position, 3) lower
          for (uint8_t i = 0; i < 3; i++)
            planner.buffer_line(
              i == 0 ? raised_parked_position[X_AXIS] : current_position[X_AXIS],
              i == 0 ? raised_parked_position[Y_AXIS] : current_position[Y_AXIS],
              i == 2 ? current_position[Z_AXIS] : raised_parked_position[Z_AXIS],
              current_position[E_AXIS],
              i == 1 ? PLANNER_XY_FEEDRATE() : planner.max_feedrate_mm_s[Z_AXIS],
              active_extruder
            );
          delayed_move_time = 0;
          active_extruder_parked = false;
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Clear active_extruder_parked");
          #endif
          break;
        case DXC_DUPLICATION_MODE:
          if (active_extruder == 0) {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOPAIR("Set planner X", LOGICAL_X_POSITION(inactive_extruder_x_pos));
                SERIAL_ECHOLNPAIR(" ... Line to X", current_position[X_AXIS] + duplicate_extruder_x_offset);
              }
            #endif
            // move duplicate extruder into correct duplication position.
            planner.set_position_mm(
              LOGICAL_X_POSITION(inactive_extruder_x_pos),
              current_position[Y_AXIS],
              current_position[Z_AXIS],
              current_position[E_AXIS]
            );
            planner.buffer_line(
              current_position[X_AXIS] + duplicate_extruder_x_offset,
              current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS],
              planner.max_feedrate_mm_s[X_AXIS], 1
            );
            SYNC_PLAN_POSITION_KINEMATIC();
            stepper.synchronize();
            extruder_duplication_enabled = true;
            active_extruder_parked = false;
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Set extruder_duplication_enabled\nClear active_extruder_parked");
            #endif
          }
          else {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Active extruder not 0");
            #endif
          }
          break;
      }
    }
    return false;
  }

#endif // DUAL_X_CARRIAGE

/**
 * Prepare a single move and get ready for the next one
 *
 * This may result in several calls to planner.buffer_line to
 * do smaller moves for DELTA, SCARA, mesh moves, etc.
 */
void prepare_move_to_destination() {
  clamp_to_software_endstops(destination);
  gcode.refresh_cmd_timeout();

  #if ENABLED(PREVENT_COLD_EXTRUSION)

    if (!DEBUGGING(DRYRUN)) {
      if (destination[E_AXIS] != current_position[E_AXIS]) {
        if (thermalManager.tooColdToExtrude(active_extruder)) {
          current_position[E_AXIS] = destination[E_AXIS]; // Behave as if the move really took place, but ignore E part
          SERIAL_ECHO_START();
          SERIAL_ECHOLNPGM(MSG_ERR_COLD_EXTRUDE_STOP);
        }
        #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
          if (destination[E_AXIS] - current_position[E_AXIS] > EXTRUDE_MAXLENGTH) {
            current_position[E_AXIS] = destination[E_AXIS]; // Behave as if the move really took place, but ignore E part
            SERIAL_ECHO_START();
            SERIAL_ECHOLNPGM(MSG_ERR_LONG_EXTRUDE_STOP);
          }
        #endif
      }
    }

  #endif

  if (
    #if UBL_DELTA // Also works for CARTESIAN (smaller segments follow mesh more closely)
      ubl.prepare_segmented_line_to(destination, feedrate_mm_s)
    #elif IS_KINEMATIC
      prepare_kinematic_move_to(destination)
    #elif ENABLED(DUAL_X_CARRIAGE)
      prepare_move_to_destination_dualx() || prepare_move_to_destination_cartesian()
    #else
      prepare_move_to_destination_cartesian()
    #endif
  ) return;

  set_current_to_destination();
}
