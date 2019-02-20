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

/**
 * motion.cpp
 */

#include "motion.h"
#include "endstops.h"
#include "stepper.h"
#include "planner.h"
#include "temperature.h"

#include "../gcode/gcode.h"

#include "../inc/MarlinConfig.h"

#if IS_SCARA
  #include "../libs/buzzer.h"
  #include "../lcd/ultralcd.h"
#endif

#if HAS_BED_PROBE
  #include "probe.h"
#endif

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if HAS_AXIS_UNHOMED_ERR && (ENABLED(ULTRA_LCD) || ENABLED(EXTENSIBLE_UI))
  #include "../lcd/ultralcd.h"
#endif

#if ENABLED(SENSORLESS_HOMING)
  #include "../feature/tmc_util.h"
#endif

#if ENABLED(FWRETRACT)
  #include "../feature/fwretract.h"
#endif

#define XYZ_CONSTS(type, array, CONFIG) const PROGMEM type array##_P[XYZ] = { X_##CONFIG, Y_##CONFIG, Z_##CONFIG }

XYZ_CONSTS(float, base_min_pos,   MIN_POS);
XYZ_CONSTS(float, base_max_pos,   MAX_POS);
XYZ_CONSTS(float, base_home_pos,  HOME_POS);
XYZ_CONSTS(float, max_length,     MAX_LENGTH);
XYZ_CONSTS(float, home_bump_mm,   HOME_BUMP_MM);
XYZ_CONSTS(signed char, home_dir, HOME_DIR);

/**
 * axis_homed
 *   Flags that each linear axis was homed.
 *   XYZ on cartesian, ABC on delta, ABZ on SCARA.
 *
 * axis_known_position
 *   Flags that the position is known in each linear axis. Set when homed.
 *   Cleared whenever a stepper powers off, potentially losing its position.
 */
uint8_t axis_homed, axis_known_position; // = 0

// Relative Mode. Enable with G91, disable with G90.
bool relative_mode; // = false;

/**
 * Cartesian Current Position
 *   Used to track the native machine position as moves are queued.
 *   Used by 'buffer_line_to_current_position' to do a move after changing it.
 *   Used by 'sync_plan_position' to update 'planner.position'.
 */
float current_position[XYZE] = { 0 };

/**
 * Cartesian Destination
 *   The destination for a move, filled in by G-code movement commands,
 *   and expected by functions like 'prepare_move_to_destination'.
 *   Set with 'get_destination_from_command' or 'set_destination_from_current'.
 */
float destination[XYZE] = { 0 };

// The active extruder (tool). Set with T<extruder> command.
#if EXTRUDERS > 1
  uint8_t active_extruder; // = 0
#endif

// Extruder offsets
#if HAS_HOTEND_OFFSET
  float hotend_offset[XYZ][HOTENDS]; // Initialized by settings.load()
#endif

// The feedrate for the current move, often used as the default if
// no other feedrate is specified. Overridden for special moves.
// Set by the last G0 through G5 command's "F" parameter.
// Functions that override this for custom moves *must always* restore it!
float feedrate_mm_s = MMM_TO_MMS(1500.0f);

int16_t feedrate_percentage = 100;

// Homing feedrate is const progmem - compare to constexpr in the header
const float homing_feedrate_mm_s[XYZ] PROGMEM = {
  #if ENABLED(DELTA)
    MMM_TO_MMS(HOMING_FEEDRATE_Z), MMM_TO_MMS(HOMING_FEEDRATE_Z),
  #else
    MMM_TO_MMS(HOMING_FEEDRATE_XY), MMM_TO_MMS(HOMING_FEEDRATE_XY),
  #endif
  MMM_TO_MMS(HOMING_FEEDRATE_Z)
};

// Cartesian conversion result goes here:
float cartes[XYZ];

#if IS_KINEMATIC
  float delta[ABC];
#endif

#if HAS_SCARA_OFFSET
  float scara_home_offset[ABC];
#endif

/**
 * The workspace can be offset by some commands, or
 * these offsets may be omitted to save on computation.
 */
#if HAS_POSITION_SHIFT
  // The distance that XYZ has been offset by G92. Reset by G28.
  float position_shift[XYZ] = { 0 };
#endif
#if HAS_HOME_OFFSET
  // This offset is added to the configured home position.
  // Set by M206, M428, or menu item. Saved to EEPROM.
  float home_offset[XYZ] = { 0 };
#endif
#if HAS_HOME_OFFSET && HAS_POSITION_SHIFT
  // The above two are combined to save on computes
  float workspace_offset[XYZ] = { 0 };
#endif

#if OLDSCHOOL_ABL
  float xy_probe_feedrate_mm_s = MMM_TO_MMS(XY_PROBE_SPEED);
#endif

/**
 * Output the current position to serial
 */
void report_current_position() {
  SERIAL_ECHOPAIR("X:", LOGICAL_X_POSITION(current_position[X_AXIS]));
  SERIAL_ECHOPAIR(" Y:", LOGICAL_Y_POSITION(current_position[Y_AXIS]));
  SERIAL_ECHOPAIR(" Z:", LOGICAL_Z_POSITION(current_position[Z_AXIS]));
  SERIAL_ECHOPAIR(" E:", current_position[E_AXIS]);

  stepper.report_positions();

  #if IS_SCARA
    scara_report_positions();
  #endif
}

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
 * Get the stepper positions in the cartes[] array.
 * Forward kinematics are applied for DELTA and SCARA.
 *
 * The result is in the current coordinate space with
 * leveling applied. The coordinates need to be run through
 * unapply_leveling to obtain the "ideal" coordinates
 * suitable for current_position, etc.
 */
void get_cartesian_from_steppers() {
  #if ENABLED(DELTA)
    forward_kinematics_DELTA(
      planner.get_axis_position_mm(A_AXIS),
      planner.get_axis_position_mm(B_AXIS),
      planner.get_axis_position_mm(C_AXIS)
    );
  #else
    #if IS_SCARA
      forward_kinematics_SCARA(
        planner.get_axis_position_degrees(A_AXIS),
        planner.get_axis_position_degrees(B_AXIS)
      );
    #else
      cartes[X_AXIS] = planner.get_axis_position_mm(X_AXIS);
      cartes[Y_AXIS] = planner.get_axis_position_mm(Y_AXIS);
    #endif
    cartes[Z_AXIS] = planner.get_axis_position_mm(Z_AXIS);
  #endif
}

/**
 * Set the current_position for an axis based on
 * the stepper positions, removing any leveling that
 * may have been applied.
 *
 * To prevent small shifts in axis position always call
 * sync_plan_position after updating axes with this.
 *
 * To keep hosts in sync, always call report_current_position
 * after updating the current_position.
 */
void set_current_from_steppers_for_axis(const AxisEnum axis) {
  get_cartesian_from_steppers();

  #if HAS_POSITION_MODIFIERS
    float pos[XYZE] = { cartes[X_AXIS], cartes[Y_AXIS], cartes[Z_AXIS], current_position[E_AXIS] };
    planner.unapply_modifiers(pos
      #if HAS_LEVELING
        , true
      #endif
    );
    const float (&cartes)[XYZE] = pos;
  #endif
  if (axis == ALL_AXES)
    COPY(current_position, cartes);
  else
    current_position[axis] = cartes[axis];
}

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
void buffer_line_to_destination(const float fr_mm_s) {
  planner.buffer_line(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], fr_mm_s, active_extruder);
}

#if IS_KINEMATIC

  /**
   * Calculate delta, start a line, and set current_position to destination
   */
  void prepare_uninterpolated_move_to_destination(const float fr_mm_s/*=0.0*/) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("prepare_uninterpolated_move_to_destination", destination);
    #endif

    #if UBL_SEGMENTED
      // ubl segmented line will do z-only moves in single segment
      ubl.prepare_segmented_line_to(destination, MMS_SCALED(fr_mm_s ? fr_mm_s : feedrate_mm_s));
    #else
      if ( current_position[X_AXIS] == destination[X_AXIS]
        && current_position[Y_AXIS] == destination[Y_AXIS]
        && current_position[Z_AXIS] == destination[Z_AXIS]
        && current_position[E_AXIS] == destination[E_AXIS]
      ) return;

      planner.buffer_line(destination, MMS_SCALED(fr_mm_s ? fr_mm_s : feedrate_mm_s), active_extruder);
    #endif

    set_current_from_destination();
  }

#endif // IS_KINEMATIC

/**
 * Plan a move to (X, Y, Z) and set the current_position
 */
void do_blocking_move_to(const float rx, const float ry, const float rz, const float &fr_mm_s/*=0.0*/) {
  const float old_feedrate_mm_s = feedrate_mm_s;

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) print_xyz(PSTR(">>> do_blocking_move_to"), NULL, rx, ry, rz);
  #endif

  const float z_feedrate = fr_mm_s ? fr_mm_s : homing_feedrate(Z_AXIS);

  #if ENABLED(DELTA)

    if (!position_is_reachable(rx, ry)) return;

    feedrate_mm_s = fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;

    set_destination_from_current();          // sync destination at the start

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("set_destination_from_current", destination);
    #endif

    // when in the danger zone
    if (current_position[Z_AXIS] > delta_clip_start_height) {
      if (rz > delta_clip_start_height) {   // staying in the danger zone
        destination[X_AXIS] = rx;           // move directly (uninterpolated)
        destination[Y_AXIS] = ry;
        destination[Z_AXIS] = rz;
        prepare_uninterpolated_move_to_destination(); // set_current_from_destination()
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("danger zone move", current_position);
        #endif
        return;
      }
      destination[Z_AXIS] = delta_clip_start_height;
      prepare_uninterpolated_move_to_destination(); // set_current_from_destination()
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("zone border move", current_position);
      #endif
    }

    if (rz > current_position[Z_AXIS]) {    // raising?
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);   // set_current_from_destination()
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("z raise move", current_position);
      #endif
    }

    destination[X_AXIS] = rx;
    destination[Y_AXIS] = ry;
    prepare_move_to_destination();         // set_current_from_destination()
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("xy move", current_position);
    #endif

    if (rz < current_position[Z_AXIS]) {    // lowering?
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);   // set_current_from_destination()
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) DEBUG_POS("z lower move", current_position);
      #endif
    }

  #elif IS_SCARA

    if (!position_is_reachable(rx, ry)) return;

    set_destination_from_current();

    // If Z needs to raise, do it before moving XY
    if (destination[Z_AXIS] < rz) {
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);
    }

    destination[X_AXIS] = rx;
    destination[Y_AXIS] = ry;
    prepare_uninterpolated_move_to_destination(fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S);

    // If Z needs to lower, do it after moving XY
    if (destination[Z_AXIS] > rz) {
      destination[Z_AXIS] = rz;
      prepare_uninterpolated_move_to_destination(z_feedrate);
    }

  #else

    // If Z needs to raise, do it before moving XY
    if (current_position[Z_AXIS] < rz) {
      feedrate_mm_s = z_feedrate;
      current_position[Z_AXIS] = rz;
      line_to_current_position();
    }

    feedrate_mm_s = fr_mm_s ? fr_mm_s : XY_PROBE_FEEDRATE_MM_S;
    current_position[X_AXIS] = rx;
    current_position[Y_AXIS] = ry;
    line_to_current_position();

    // If Z needs to lower, do it after moving XY
    if (current_position[Z_AXIS] > rz) {
      feedrate_mm_s = z_feedrate;
      current_position[Z_AXIS] = rz;
      line_to_current_position();
    }

  #endif

  feedrate_mm_s = old_feedrate_mm_s;

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< do_blocking_move_to");
  #endif

  planner.synchronize();
}
void do_blocking_move_to_x(const float &rx, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(rx, current_position[Y_AXIS], current_position[Z_AXIS], fr_mm_s);
}
void do_blocking_move_to_z(const float &rz, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], rz, fr_mm_s);
}
void do_blocking_move_to_xy(const float &rx, const float &ry, const float &fr_mm_s/*=0.0*/) {
  do_blocking_move_to(rx, ry, current_position[Z_AXIS], fr_mm_s);
}

//
// Prepare to do endstop or probe moves
// with custom feedrates.
//
//  - Save current feedrates
//  - Reset the rate multiplier
//
void bracket_probe_move(const bool before) {
  static float saved_feedrate_mm_s;
  static int16_t saved_feedrate_percentage;
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("bracket_probe_move", current_position);
  #endif
  if (before) {
    saved_feedrate_mm_s = feedrate_mm_s;
    saved_feedrate_percentage = feedrate_percentage;
    feedrate_percentage = 100;
  }
  else {
    feedrate_mm_s = saved_feedrate_mm_s;
    feedrate_percentage = saved_feedrate_percentage;
  }
}

void setup_for_endstop_or_probe_move() { bracket_probe_move(true); }
void clean_up_after_endstop_or_probe_move() { bracket_probe_move(false); }

#if HAS_SOFTWARE_ENDSTOPS

  bool soft_endstops_enabled = true;

  // Software Endstops are based on the configured limits.
  float soft_endstop_min[XYZ] = { X_MIN_BED, Y_MIN_BED, Z_MIN_POS },
        soft_endstop_max[XYZ] = { X_MAX_BED, Y_MAX_BED, Z_MAX_POS };

  #if IS_KINEMATIC
    float soft_endstop_radius, soft_endstop_radius_2;
  #endif

  /**
   * Constrain the given coordinates to the software endstops.
   *
   * For DELTA/SCARA the XY constraint is based on the smallest
   * radius within the set software endstops.
   */
  void clamp_to_software_endstops(float target[XYZ]) {
    if (!soft_endstops_enabled) return;
    #if IS_KINEMATIC
      const float dist_2 = HYPOT2(target[X_AXIS], target[Y_AXIS]);
      if (dist_2 > soft_endstop_radius_2) {
        const float ratio = soft_endstop_radius / SQRT(dist_2); // 200 / 300 = 0.66
        target[X_AXIS] *= ratio;
        target[Y_AXIS] *= ratio;
      }
    #else
      #if ENABLED(MIN_SOFTWARE_ENDSTOP_X)
        NOLESS(target[X_AXIS], soft_endstop_min[X_AXIS]);
      #endif
      #if ENABLED(MIN_SOFTWARE_ENDSTOP_Y)
        NOLESS(target[Y_AXIS], soft_endstop_min[Y_AXIS]);
      #endif
      #if ENABLED(MAX_SOFTWARE_ENDSTOP_X)
        NOMORE(target[X_AXIS], soft_endstop_max[X_AXIS]);
      #endif
      #if ENABLED(MAX_SOFTWARE_ENDSTOP_Y)
        NOMORE(target[Y_AXIS], soft_endstop_max[Y_AXIS]);
      #endif
    #endif
    #if ENABLED(MIN_SOFTWARE_ENDSTOP_Z)
      NOLESS(target[Z_AXIS], soft_endstop_min[Z_AXIS]);
    #endif
    #if ENABLED(MAX_SOFTWARE_ENDSTOP_Z)
      NOMORE(target[Z_AXIS], soft_endstop_max[Z_AXIS]);
    #endif
  }

  /**
   * Software endstops can be used to monitor the open end of
   * an axis that has a hardware endstop on the other end. Or
   * they can prevent axes from moving past endstops and grinding.
   *
   * To keep doing their job as the coordinate system changes,
   * the software endstop positions must be refreshed to remain
   * at the same positions relative to the machine.
   */
  void update_software_endstops(const AxisEnum axis) {

    #if ENABLED(DUAL_X_CARRIAGE)

      if (axis == X_AXIS) {

        // In Dual X mode hotend_offset[X] is T1's home position
        const float dual_max_x = MAX(hotend_offset[X_AXIS][1], X2_MAX_POS);

        if (active_extruder != 0) {
          // T1 can move from X2_MIN_POS to X2_MAX_POS or X2 home position (whichever is larger)
          soft_endstop_min[X_AXIS] = X2_MIN_POS;
          soft_endstop_max[X_AXIS] = dual_max_x;
        }
        else if (dxc_is_duplicating()) {
          // In Duplication Mode, T0 can move as far left as X1_MIN_POS
          // but not so far to the right that T1 would move past the end
          soft_endstop_min[X_AXIS] = X1_MIN_POS;
          soft_endstop_max[X_AXIS] = MIN(X1_MAX_POS, dual_max_x - duplicate_extruder_x_offset);
        }
        else {
          // In other modes, T0 can move from X1_MIN_POS to X1_MAX_POS
          soft_endstop_min[X_AXIS] = X1_MIN_POS;
          soft_endstop_max[X_AXIS] = X1_MAX_POS;
        }
      }

    #elif ENABLED(DELTA)

      soft_endstop_min[axis] = base_min_pos(axis);
      soft_endstop_max[axis] = (axis == Z_AXIS ? delta_height
      #if HAS_BED_PROBE
        - zprobe_zoffset
      #endif
      : base_max_pos(axis));

      switch (axis) {
        case X_AXIS:
        case Y_AXIS:
          // Get a minimum radius for clamping
          soft_endstop_radius = MIN(ABS(MAX(soft_endstop_min[X_AXIS], soft_endstop_min[Y_AXIS])), soft_endstop_max[X_AXIS], soft_endstop_max[Y_AXIS]);
          soft_endstop_radius_2 = sq(soft_endstop_radius);
          break;
        case Z_AXIS:
          delta_clip_start_height = soft_endstop_max[axis] - delta_safe_distance_from_top();
        default: break;
      }

    #else

      soft_endstop_min[axis] = base_min_pos(axis);
      soft_endstop_max[axis] = base_max_pos(axis);

    #endif

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("For ", axis_codes[axis]);
        SERIAL_ECHOPAIR(" axis:\n soft_endstop_min = ", soft_endstop_min[axis]);
        SERIAL_ECHOLNPAIR("\n soft_endstop_max = ", soft_endstop_max[axis]);
      }
    #endif
  }

#endif

#if !UBL_SEGMENTED
#if IS_KINEMATIC

  #if IS_SCARA
    /**
     * Before raising this value, use M665 S[seg_per_sec] to decrease
     * the number of segments-per-second. Default is 200. Some deltas
     * do better with 160 or lower. It would be good to know how many
     * segments-per-second are actually possible for SCARA on AVR.
     *
     * Longer segments result in less kinematic overhead
     * but may produce jagged lines. Try 0.5mm, 1.0mm, and 2.0mm
     * and compare the difference.
     */
    #define SCARA_MIN_SEGMENT_LENGTH 0.5f
  #endif

  /**
   * Prepare a linear move in a DELTA or SCARA setup.
   *
   * Called from prepare_move_to_destination as the
   * default Delta/SCARA segmenter.
   *
   * This calls planner.buffer_line several times, adding
   * small incremental moves for DELTA or SCARA.
   *
   * For Unified Bed Leveling (Delta or Segmented Cartesian)
   * the ubl.prepare_segmented_line_to method replaces this.
   *
   * For Auto Bed Leveling (Bilinear) with SEGMENT_LEVELED_MOVES
   * this is replaced by segmented_line_to_destination below.
   */
  inline bool prepare_kinematic_move_to(const float (&rtarget)[XYZE]) {

    // Get the top feedrate of the move in the XY plane
    const float _feedrate_mm_s = MMS_SCALED(feedrate_mm_s);

    const float xdiff = rtarget[X_AXIS] - current_position[X_AXIS],
                ydiff = rtarget[Y_AXIS] - current_position[Y_AXIS];

    // If the move is only in Z/E don't split up the move
    if (!xdiff && !ydiff) {
      planner.buffer_line(rtarget, _feedrate_mm_s, active_extruder);
      return false; // caller will update current_position
    }

    // Fail if attempting move outside printable radius
    if (!position_is_reachable(rtarget[X_AXIS], rtarget[Y_AXIS])) return true;

    // Remaining cartesian distances
    const float zdiff = rtarget[Z_AXIS] - current_position[Z_AXIS],
                ediff = rtarget[E_AXIS] - current_position[E_AXIS];

    // Get the linear distance in XYZ
    float cartesian_mm = SQRT(sq(xdiff) + sq(ydiff) + sq(zdiff));

    // If the move is very short, check the E move distance
    if (UNEAR_ZERO(cartesian_mm)) cartesian_mm = ABS(ediff);

    // No E move either? Game over.
    if (UNEAR_ZERO(cartesian_mm)) return true;

    // Minimum number of seconds to move the given distance
    const float seconds = cartesian_mm / _feedrate_mm_s;

    // The number of segments-per-second times the duration
    // gives the number of segments
    uint16_t segments = delta_segments_per_second * seconds;

    // For SCARA enforce a minimum segment size
    #if IS_SCARA
      NOMORE(segments, cartesian_mm * (1.0f / float(SCARA_MIN_SEGMENT_LENGTH)));
    #endif

    // At least one segment is required
    NOLESS(segments, 1U);

    // The approximate length of each segment
    const float inv_segments = 1.0f / float(segments),
                segment_distance[XYZE] = {
                  xdiff * inv_segments,
                  ydiff * inv_segments,
                  zdiff * inv_segments,
                  ediff * inv_segments
                },
                cartesian_segment_mm = cartesian_mm * inv_segments;

    #if ENABLED(SCARA_FEEDRATE_SCALING)
      const float inv_duration = _feedrate_mm_s / cartesian_segment_mm;
    #endif

    /*
    SERIAL_ECHOPAIR("mm=", cartesian_mm);
    SERIAL_ECHOPAIR(" seconds=", seconds);
    SERIAL_ECHOPAIR(" segments=", segments);
    SERIAL_ECHOPAIR(" segment_mm=", cartesian_segment_mm);
    SERIAL_EOL();
    //*/

    // Get the current position as starting point
    float raw[XYZE];
    COPY(raw, current_position);

    // Calculate and execute the segments
    while (--segments) {

      static millis_t next_idle_ms = millis() + 200UL;
      thermalManager.manage_heater();  // This returns immediately if not really needed.
      if (ELAPSED(millis(), next_idle_ms)) {
        next_idle_ms = millis() + 200UL;
        idle();
      }

      LOOP_XYZE(i) raw[i] += segment_distance[i];

      if (!planner.buffer_line(raw, _feedrate_mm_s, active_extruder, cartesian_segment_mm
        #if ENABLED(SCARA_FEEDRATE_SCALING)
          , inv_duration
        #endif
      ))
        break;
    }

    // Ensure last segment arrives at target location.
    planner.buffer_line(rtarget, _feedrate_mm_s, active_extruder, cartesian_segment_mm
      #if ENABLED(SCARA_FEEDRATE_SCALING)
        , inv_duration
      #endif
    );

    return false; // caller will update current_position
  }

#else // !IS_KINEMATIC

  #if ENABLED(SEGMENT_LEVELED_MOVES)

    /**
     * Prepare a segmented move on a CARTESIAN setup.
     *
     * This calls planner.buffer_line several times, adding
     * small incremental moves. This allows the planner to
     * apply more detailed bed leveling to the full move.
     */
    inline void segmented_line_to_destination(const float &fr_mm_s, const float segment_size=LEVELED_SEGMENT_LENGTH) {

      const float xdiff = destination[X_AXIS] - current_position[X_AXIS],
                  ydiff = destination[Y_AXIS] - current_position[Y_AXIS];

      // If the move is only in Z/E don't split up the move
      if (!xdiff && !ydiff) {
        planner.buffer_line(destination, fr_mm_s, active_extruder);
        return;
      }

      // Remaining cartesian distances
      const float zdiff = destination[Z_AXIS] - current_position[Z_AXIS],
                  ediff = destination[E_AXIS] - current_position[E_AXIS];

      // Get the linear distance in XYZ
      // If the move is very short, check the E move distance
      // No E move either? Game over.
      float cartesian_mm = SQRT(sq(xdiff) + sq(ydiff) + sq(zdiff));
      if (UNEAR_ZERO(cartesian_mm)) cartesian_mm = ABS(ediff);
      if (UNEAR_ZERO(cartesian_mm)) return;

      // The length divided by the segment size
      // At least one segment is required
      uint16_t segments = cartesian_mm / segment_size;
      NOLESS(segments, 1U);

      // The approximate length of each segment
      const float inv_segments = 1.0f / float(segments),
                  cartesian_segment_mm = cartesian_mm * inv_segments,
                  segment_distance[XYZE] = {
                    xdiff * inv_segments,
                    ydiff * inv_segments,
                    zdiff * inv_segments,
                    ediff * inv_segments
                  };

      #if ENABLED(SCARA_FEEDRATE_SCALING)
        const float inv_duration = _feedrate_mm_s / cartesian_segment_mm;
      #endif

      // SERIAL_ECHOPAIR("mm=", cartesian_mm);
      // SERIAL_ECHOLNPAIR(" segments=", segments);
      // SERIAL_ECHOLNPAIR(" segment_mm=", cartesian_segment_mm);

      // Get the raw current position as starting point
      float raw[XYZE];
      COPY(raw, current_position);

      // Calculate and execute the segments
      while (--segments) {
        static millis_t next_idle_ms = millis() + 200UL;
        thermalManager.manage_heater();  // This returns immediately if not really needed.
        if (ELAPSED(millis(), next_idle_ms)) {
          next_idle_ms = millis() + 200UL;
          idle();
        }
        LOOP_XYZE(i) raw[i] += segment_distance[i];
        if (!planner.buffer_line(raw, fr_mm_s, active_extruder, cartesian_segment_mm
          #if ENABLED(SCARA_FEEDRATE_SCALING)
            , inv_duration
          #endif
        ))
          break;
      }

      // Since segment_distance is only approximate,
      // the final move must be to the exact destination.
      planner.buffer_line(destination, fr_mm_s, active_extruder, cartesian_segment_mm
        #if ENABLED(SCARA_FEEDRATE_SCALING)
          , inv_duration
        #endif
      );
    }

  #endif // SEGMENT_LEVELED_MOVES

  /**
   * Prepare a linear move in a Cartesian setup.
   *
   * When a mesh-based leveling system is active, moves are segmented
   * according to the configuration of the leveling system.
   *
   * Returns true if current_position[] was set to destination[]
   */
  inline bool prepare_move_to_destination_cartesian() {
    #if HAS_MESH
      if (planner.leveling_active && planner.leveling_active_at_z(destination[Z_AXIS])) {
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          ubl.line_to_destination_cartesian(MMS_SCALED(feedrate_mm_s), active_extruder);  // UBL's motion routine needs to know about
          return true;                                                                    // all moves, including Z-only moves.
        #elif ENABLED(SEGMENT_LEVELED_MOVES)
          segmented_line_to_destination(MMS_SCALED(feedrate_mm_s));
          return false; // caller will update current_position
        #else
          /**
           * For MBL and ABL-BILINEAR only segment moves when X or Y are involved.
           * Otherwise fall through to do a direct single move.
           */
          if (current_position[X_AXIS] != destination[X_AXIS] || current_position[Y_AXIS] != destination[Y_AXIS]) {
            #if ENABLED(MESH_BED_LEVELING)
              mbl.line_to_destination(MMS_SCALED(feedrate_mm_s));
            #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
              bilinear_line_to_destination(MMS_SCALED(feedrate_mm_s));
            #endif
            return true;
          }
        #endif
      }
    #endif // HAS_MESH

    buffer_line_to_destination(MMS_SCALED(feedrate_mm_s));
    return false; // caller will update current_position
  }

#endif // !IS_KINEMATIC
#endif // !UBL_SEGMENTED

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  bool extruder_duplication_enabled = false,                              // Used in Dual X mode 2 & 3
       scaled_duplication_mode      = false;                              // Used in Dual X mode 2 & 3
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
      return base_home_pos(X_AXIS);
    else
      /**
       * In dual carriage mode the extruder offset provides an override of the
       * second X-carriage position when homed - otherwise X2_HOME_POS is used.
       * This allows soft recalibration of the second extruder home position
       * without firmware reflash (through the M218 command).
       */
      return hotend_offset[X_AXIS][1] > 0 ? hotend_offset[X_AXIS][1] : X2_HOME_POS;
  }

  /**
   * Prepare a linear move in a dual X axis setup
   *
   * Return true if current_position[] was set to destination[]
   */
  inline bool dual_x_carriage_unpark() {
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
              set_current_from_destination();
              NOLESS(raised_parked_position[Z_AXIS], destination[Z_AXIS]);
              delayed_move_time = millis();
              return true;
            }
          }
          // unpark extruder: 1) raise, 2) move into starting XY position, 3) lower

            #define CUR_X    current_position[X_AXIS]
            #define CUR_Y    current_position[Y_AXIS]
            #define CUR_Z    current_position[Z_AXIS]
            #define CUR_E    current_position[E_AXIS]
            #define RAISED_X raised_parked_position[X_AXIS]
            #define RAISED_Y raised_parked_position[Y_AXIS]
            #define RAISED_Z raised_parked_position[Z_AXIS]

            if (  planner.buffer_line(RAISED_X, RAISED_Y, RAISED_Z, CUR_E, planner.settings.max_feedrate_mm_s[Z_AXIS], active_extruder))
              if (planner.buffer_line(   CUR_X,    CUR_Y, RAISED_Z, CUR_E, PLANNER_XY_FEEDRATE(),             active_extruder))
                  planner.buffer_line(   CUR_X,    CUR_Y,    CUR_Z, CUR_E, planner.settings.max_feedrate_mm_s[Z_AXIS], active_extruder);
          delayed_move_time = 0;
          active_extruder_parked = false;
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Clear active_extruder_parked");
          #endif
          break;
        case DXC_SCALED_DUPLICATION_MODE:
        case DXC_DUPLICATION_MODE:
          if (active_extruder == 0) {
            #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(LEVELING)) {
                SERIAL_ECHOPAIR("Set planner X", inactive_extruder_x_pos);
                SERIAL_ECHOLNPAIR(" ... Line to X", current_position[X_AXIS] + duplicate_extruder_x_offset);
              }
            #endif
            // move duplicate extruder into correct duplication position.
            planner.set_position_mm(inactive_extruder_x_pos, current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS]);

            if (!planner.buffer_line(
                dual_x_carriage_mode == DXC_DUPLICATION_MODE ? duplicate_extruder_x_offset + current_position[X_AXIS] : inactive_extruder_x_pos,
                current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS],
                planner.settings.max_feedrate_mm_s[X_AXIS], 1
              )
            ) break;
            planner.synchronize();
            sync_plan_position();
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
    stepper.set_directions();
    return false;
  }

#endif // DUAL_X_CARRIAGE

/**
 * Prepare a single move and get ready for the next one
 *
 * This may result in several calls to planner.buffer_line to
 * do smaller moves for DELTA, SCARA, mesh moves, etc.
 *
 * Make sure current_position[E] and destination[E] are good
 * before calling or cold/lengthy extrusion may get missed.
 */
void prepare_move_to_destination() {
  clamp_to_software_endstops(destination);

  #if ENABLED(PREVENT_COLD_EXTRUSION) || ENABLED(PREVENT_LENGTHY_EXTRUDE)

    if (!DEBUGGING(DRYRUN)) {
      if (destination[E_AXIS] != current_position[E_AXIS]) {
        #if ENABLED(PREVENT_COLD_EXTRUSION)
          if (thermalManager.tooColdToExtrude(active_extruder)) {
            current_position[E_AXIS] = destination[E_AXIS]; // Behave as if the move really took place, but ignore E part
            SERIAL_ECHO_MSG(MSG_ERR_COLD_EXTRUDE_STOP);
          }
        #endif // PREVENT_COLD_EXTRUSION
        #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
          if (ABS(destination[E_AXIS] - current_position[E_AXIS]) * planner.e_factor[active_extruder] > (EXTRUDE_MAXLENGTH)) {
            current_position[E_AXIS] = destination[E_AXIS]; // Behave as if the move really took place, but ignore E part
            SERIAL_ECHO_MSG(MSG_ERR_LONG_EXTRUDE_STOP);
          }
        #endif // PREVENT_LENGTHY_EXTRUDE
      }
    }

  #endif // PREVENT_COLD_EXTRUSION || PREVENT_LENGTHY_EXTRUDE

  #if ENABLED(DUAL_X_CARRIAGE)
    if (dual_x_carriage_unpark()) return;
  #endif

  if (
    #if UBL_SEGMENTED
      //ubl.prepare_segmented_line_to(destination, MMS_SCALED(feedrate_mm_s))   // This doesn't seem to work correctly on UBL.
      #if IS_KINEMATIC                                                          // Use Kinematic / Cartesian cases as a workaround for now.
        ubl.prepare_segmented_line_to(destination, MMS_SCALED(feedrate_mm_s))
      #else
        prepare_move_to_destination_cartesian()
      #endif
    #elif IS_KINEMATIC
      prepare_kinematic_move_to(destination)
    #else
      prepare_move_to_destination_cartesian()
    #endif
  ) return;

  set_current_from_destination();
}

#if HAS_AXIS_UNHOMED_ERR

  bool axis_unhomed_error(const bool x/*=true*/, const bool y/*=true*/, const bool z/*=true*/) {
    #if ENABLED(HOME_AFTER_DEACTIVATE)
      const bool xx = x && !TEST(axis_known_position, X_AXIS),
                 yy = y && !TEST(axis_known_position, Y_AXIS),
                 zz = z && !TEST(axis_known_position, Z_AXIS);
    #else
      const bool xx = x && !TEST(axis_homed, X_AXIS),
                 yy = y && !TEST(axis_homed, Y_AXIS),
                 zz = z && !TEST(axis_homed, Z_AXIS);
    #endif
    if (xx || yy || zz) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM(MSG_HOME " ");
      if (xx) SERIAL_ECHOPGM(MSG_X);
      if (yy) SERIAL_ECHOPGM(MSG_Y);
      if (zz) SERIAL_ECHOPGM(MSG_Z);
      SERIAL_ECHOLNPGM(" " MSG_FIRST);

      #if ENABLED(ULTRA_LCD) || ENABLED(EXTENSIBLE_UI)
        ui.status_printf_P(0, PSTR(MSG_HOME " %s%s%s " MSG_FIRST), xx ? MSG_X : "", yy ? MSG_Y : "", zz ? MSG_Z : "");
      #endif
      return true;
    }
    return false;
  }

#endif // HAS_AXIS_UNHOMED_ERR

/**
 * Homing bump feedrate (mm/s)
 */
float get_homing_bump_feedrate(const AxisEnum axis) {
  #if HOMING_Z_WITH_PROBE
    if (axis == Z_AXIS) return MMM_TO_MMS(Z_PROBE_SPEED_SLOW);
  #endif
  static const uint8_t homing_bump_divisor[] PROGMEM = HOMING_BUMP_DIVISOR;
  uint8_t hbd = pgm_read_byte(&homing_bump_divisor[axis]);
  if (hbd < 1) {
    hbd = 10;
    SERIAL_ECHO_MSG("Warning: Homing Bump Divisor < 1");
  }
  return homing_feedrate(axis) / hbd;
}

#if ENABLED(SENSORLESS_HOMING)
  /**
   * Set sensorless homing if the axis has it, accounting for Core Kinematics.
   */
  sensorless_t start_sensorless_homing_per_axis(const AxisEnum axis) {
    sensorless_t stealth_states { false, false, false, false, false, false, false };

    switch (axis) {
      default: break;
      #if X_SENSORLESS
        case X_AXIS:
          stealth_states.x = tmc_enable_stallguard(stepperX);
          #if AXIS_HAS_STALLGUARD(X2)
            stealth_states.x2 = tmc_enable_stallguard(stepperX2);
          #endif
          #if CORE_IS_XY && Y_SENSORLESS
            stealth_states.y = tmc_enable_stallguard(stepperY);
          #elif CORE_IS_XZ && Z_SENSORLESS
            stealth_states.z = tmc_enable_stallguard(stepperZ);
          #endif
          break;
      #endif
      #if Y_SENSORLESS
        case Y_AXIS:
          stealth_states.y = tmc_enable_stallguard(stepperY);
          #if AXIS_HAS_STALLGUARD(Y2)
            stealth_states.y2 = tmc_enable_stallguard(stepperY2);
          #endif
          #if CORE_IS_XY && X_SENSORLESS
            stealth_states.x = tmc_enable_stallguard(stepperX);
          #elif CORE_IS_YZ && Z_SENSORLESS
            stealth_states.z = tmc_enable_stallguard(stepperZ);
          #endif
          break;
      #endif
      #if Z_SENSORLESS
        case Z_AXIS:
          stealth_states.z = tmc_enable_stallguard(stepperZ);
          #if AXIS_HAS_STALLGUARD(Z2)
            stealth_states.z2 = tmc_enable_stallguard(stepperZ2);
          #endif
          #if AXIS_HAS_STALLGUARD(Z3)
            stealth_states.z3 = tmc_enable_stallguard(stepperZ3);
          #endif
          #if CORE_IS_XZ && X_SENSORLESS
            stealth_states.x = tmc_enable_stallguard(stepperX);
          #elif CORE_IS_YZ && Y_SENSORLESS
            stealth_states.y = tmc_enable_stallguard(stepperY);
          #endif
          break;
      #endif
    }
    return stealth_states;
  }

  void end_sensorless_homing_per_axis(const AxisEnum axis, sensorless_t enable_stealth) {
    switch (axis) {
      default: break;
      #if X_SENSORLESS
        case X_AXIS:
          tmc_disable_stallguard(stepperX, enable_stealth.x);
          #if AXIS_HAS_STALLGUARD(X2)
            tmc_disable_stallguard(stepperX2, enable_stealth.x2);
          #endif
          #if CORE_IS_XY && Y_SENSORLESS
            tmc_disable_stallguard(stepperY, enable_stealth.y);
          #elif CORE_IS_XZ && Z_SENSORLESS
            tmc_disable_stallguard(stepperZ, enable_stealth.z);
          #endif
          break;
      #endif
      #if Y_SENSORLESS
        case Y_AXIS:
          tmc_disable_stallguard(stepperY, enable_stealth.y);
          #if AXIS_HAS_STALLGUARD(Y2)
            tmc_disable_stallguard(stepperY2, enable_stealth.y2);
          #endif
          #if CORE_IS_XY && X_SENSORLESS
            tmc_disable_stallguard(stepperX, enable_stealth.x);
          #elif CORE_IS_YZ && Z_SENSORLESS
            tmc_disable_stallguard(stepperZ, enable_stealth.z);
          #endif
          break;
      #endif
      #if Z_SENSORLESS
        case Z_AXIS:
          tmc_disable_stallguard(stepperZ, enable_stealth.z);
          #if AXIS_HAS_STALLGUARD(Z2)
            tmc_disable_stallguard(stepperZ2, enable_stealth.z2);
          #endif
          #if AXIS_HAS_STALLGUARD(Z3)
            tmc_disable_stallguard(stepperZ3, enable_stealth.z3);
          #endif
          #if CORE_IS_XZ && X_SENSORLESS
            tmc_disable_stallguard(stepperX, enable_stealth.x);
          #elif CORE_IS_YZ && Y_SENSORLESS
            tmc_disable_stallguard(stepperY, enable_stealth.y);
          #endif
          break;
      #endif
    }
  }

#endif // SENSORLESS_HOMING

/**
 * Home an individual linear axis
 */
void do_homing_move(const AxisEnum axis, const float distance, const float fr_mm_s=0.0) {

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> do_homing_move(", axis_codes[axis]);
      SERIAL_ECHOPAIR(", ", distance);
      SERIAL_ECHOPGM(", ");
      if (fr_mm_s)
        SERIAL_ECHO(fr_mm_s);
      else {
        SERIAL_ECHOPAIR("[", homing_feedrate(axis));
        SERIAL_CHAR(']');
      }
      SERIAL_ECHOLNPGM(")");
    }
  #endif

  #if HOMING_Z_WITH_PROBE && HAS_HEATED_BED && ENABLED(WAIT_FOR_BED_HEATER)
    // Wait for bed to heat back up between probing points
    if (axis == Z_AXIS && distance < 0 && thermalManager.isHeatingBed()) {
      serialprintPGM(msg_wait_for_bed_heating);
      LCD_MESSAGEPGM(MSG_BED_HEATING);
      thermalManager.wait_for_bed();
      ui.reset_status();
    }
  #endif

  // Only do some things when moving towards an endstop
  const int8_t axis_home_dir =
    #if ENABLED(DUAL_X_CARRIAGE)
      (axis == X_AXIS) ? x_home_dir(active_extruder) :
    #endif
    home_dir(axis);
  const bool is_home_dir = (axis_home_dir > 0) == (distance > 0);

  #if ENABLED(SENSORLESS_HOMING)
    sensorless_t stealth_states;
  #endif

  if (is_home_dir) {

    #if HOMING_Z_WITH_PROBE && QUIET_PROBING
      if (axis == Z_AXIS) probing_pause(true);
    #endif

    // Disable stealthChop if used. Enable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      stealth_states = start_sensorless_homing_per_axis(axis);
    #endif
  }

  #if IS_SCARA
    // Tell the planner the axis is at 0
    current_position[axis] = 0;
    sync_plan_position();
    current_position[axis] = distance;
    planner.buffer_line(current_position, fr_mm_s ? fr_mm_s : homing_feedrate(axis), active_extruder);
  #else
    float target[ABCE] = { planner.get_axis_position_mm(A_AXIS), planner.get_axis_position_mm(B_AXIS), planner.get_axis_position_mm(C_AXIS), planner.get_axis_position_mm(E_AXIS) };
    target[axis] = 0;
    planner.set_machine_position_mm(target);
    target[axis] = distance;

    #if IS_KINEMATIC && ENABLED(JUNCTION_DEVIATION)
      const float delta_mm_cart[XYZE] = {0, 0, 0, 0};
    #endif

    // Set delta/cartesian axes directly
    planner.buffer_segment(target
      #if IS_KINEMATIC && ENABLED(JUNCTION_DEVIATION)
        , delta_mm_cart
      #endif
      , fr_mm_s ? fr_mm_s : homing_feedrate(axis), active_extruder
    );
  #endif

  planner.synchronize();

  if (is_home_dir) {

    #if HOMING_Z_WITH_PROBE && QUIET_PROBING
      if (axis == Z_AXIS) probing_pause(false);
    #endif

    endstops.validate_homing_move();

    // Re-enable stealthChop if used. Disable diag1 pin on driver.
    #if ENABLED(SENSORLESS_HOMING)
      end_sensorless_homing_per_axis(axis, stealth_states);
    #endif
  }

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("<<< do_homing_move(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif
}

/**
 * Set an axis' current position to its home position (after homing).
 *
 * For Core and Cartesian robots this applies one-to-one when an
 * individual axis has been homed.
 *
 * DELTA should wait until all homing is done before setting the XYZ
 * current_position to home, because homing is a single operation.
 * In the case where the axis positions are already known and previously
 * homed, DELTA could home to X or Y individually by moving either one
 * to the center. However, homing Z always homes XY and Z.
 *
 * SCARA should wait until all XY homing is done before setting the XY
 * current_position to home, because neither X nor Y is at home until
 * both are at home. Z can however be homed individually.
 *
 * Callers must sync the planner position after calling this!
 */
void set_axis_is_at_home(const AxisEnum axis) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> set_axis_is_at_home(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  SBI(axis_known_position, axis);
  SBI(axis_homed, axis);

  #if HAS_POSITION_SHIFT
    position_shift[axis] = 0;
    update_workspace_offset(axis);
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    if (axis == X_AXIS && (active_extruder == 1 || dual_x_carriage_mode == DXC_DUPLICATION_MODE)) {
      current_position[X_AXIS] = x_home_pos(active_extruder);
      return;
    }
  #endif

  #if ENABLED(MORGAN_SCARA)
    scara_set_axis_is_at_home(axis);
  #elif ENABLED(DELTA)
    current_position[axis] = (axis == Z_AXIS ? delta_height
    #if HAS_BED_PROBE
      - zprobe_zoffset
    #endif
    : base_home_pos(axis));
  #else
    current_position[axis] = base_home_pos(axis);
  #endif

  /**
   * Z Probe Z Homing? Account for the probe's Z offset.
   */
  #if HAS_BED_PROBE && Z_HOME_DIR < 0
    if (axis == Z_AXIS) {
      #if HOMING_Z_WITH_PROBE

        current_position[Z_AXIS] -= zprobe_zoffset;

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOLNPGM("*** Z HOMED WITH PROBE (Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN) ***");
            SERIAL_ECHOLNPAIR("> zprobe_zoffset = ", zprobe_zoffset);
          }
        #endif

      #elif ENABLED(DEBUG_LEVELING_FEATURE)

        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("*** Z HOMED TO ENDSTOP (Z_MIN_PROBE_ENDSTOP) ***");

      #endif
    }
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      #if HAS_HOME_OFFSET
        SERIAL_ECHOPAIR("> home_offset[", axis_codes[axis]);
        SERIAL_ECHOLNPAIR("] = ", home_offset[axis]);
      #endif
      DEBUG_POS("", current_position);
      SERIAL_ECHOPAIR("<<< set_axis_is_at_home(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    I2CPEM.homed(axis);
  #endif
}

/**
 * Set an axis' to be unhomed.
 */
void set_axis_is_not_at_home(const AxisEnum axis) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> set_axis_is_not_at_home(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  CBI(axis_known_position, axis);
  CBI(axis_homed, axis);

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("<<< set_axis_is_not_at_home(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  #if ENABLED(I2C_POSITION_ENCODERS)
    I2CPEM.unhomed(axis);
  #endif
}

/**
 * Home an individual "raw axis" to its endstop.
 * This applies to XYZ on Cartesian and Core robots, and
 * to the individual ABC steppers on DELTA and SCARA.
 *
 * At the end of the procedure the axis is marked as
 * homed and the current position of that axis is updated.
 * Kinematic robots should wait till all axes are homed
 * before updating the current position.
 */

void homeaxis(const AxisEnum axis) {

  #if IS_SCARA
    // Only Z homing (with probe) is permitted
    if (axis != Z_AXIS) { BUZZ(100, 880); return; }
  #else
    #define CAN_HOME(A) \
      (axis == _AXIS(A) && ((A##_MIN_PIN > -1 && A##_HOME_DIR < 0) || (A##_MAX_PIN > -1 && A##_HOME_DIR > 0)))
    if (!CAN_HOME(X) && !CAN_HOME(Y) && !CAN_HOME(Z)) return;
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR(">>> homeaxis(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif

  const int axis_home_dir = (
    #if ENABLED(DUAL_X_CARRIAGE)
      axis == X_AXIS ? x_home_dir(active_extruder) :
    #endif
    home_dir(axis)
  );

  // Homing Z towards the bed? Deploy the Z probe or endstop.
  #if HOMING_Z_WITH_PROBE
    if (axis == Z_AXIS && DEPLOY_PROBE()) return;
  #endif

  // Set flags for X, Y, Z motor locking
  #if HAS_EXTRA_ENDSTOPS
    switch (axis) {
      #if ENABLED(X_DUAL_ENDSTOPS)
        case X_AXIS:
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        case Y_AXIS:
      #endif
      #if Z_MULTI_ENDSTOPS
        case Z_AXIS:
      #endif
      stepper.set_separate_multi_axis(true);
      default: break;
    }
  #endif

  // Fast move towards endstop until triggered
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Home 1 Fast:");
  #endif

  #if HOMING_Z_WITH_PROBE && ENABLED(BLTOUCH)
    // BLTOUCH needs to be deployed every time
    if (axis == Z_AXIS && set_bltouch_deployed(true)) return;
  #endif

  do_homing_move(axis, 1.5f * max_length(
    #if ENABLED(DELTA)
      Z_AXIS
    #else
      axis
    #endif
    ) * axis_home_dir
  );

  #if HOMING_Z_WITH_PROBE && ENABLED(BLTOUCH)
    // BLTOUCH needs to be stowed after trigger to rearm itself
    if (axis == Z_AXIS) set_bltouch_deployed(false);
  #endif

  // When homing Z with probe respect probe clearance
  const float bump = axis_home_dir * (
    #if HOMING_Z_WITH_PROBE
      (axis == Z_AXIS && (Z_HOME_BUMP_MM)) ? MAX(Z_CLEARANCE_BETWEEN_PROBES, Z_HOME_BUMP_MM) :
    #endif
    home_bump_mm(axis)
  );

  // If a second homing move is configured...
  if (bump) {
    // Move away from the endstop by the axis HOME_BUMP_MM
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Move Away:");
    #endif
    do_homing_move(axis, -bump
      #if HOMING_Z_WITH_PROBE
        , axis == Z_AXIS ? MMM_TO_MMS(Z_PROBE_SPEED_FAST) : 0.0
      #endif
    );

    // Slow move towards endstop until triggered
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("Home 2 Slow:");
    #endif

    #if HOMING_Z_WITH_PROBE && ENABLED(BLTOUCH)
      // BLTOUCH needs to be deployed every time
      if (axis == Z_AXIS && set_bltouch_deployed(true)) return;
    #endif

    do_homing_move(axis, 2 * bump, get_homing_bump_feedrate(axis));

    #if HOMING_Z_WITH_PROBE && ENABLED(BLTOUCH)
      // BLTOUCH needs to be stowed after trigger to rearm itself
      if (axis == Z_AXIS) set_bltouch_deployed(false);
    #endif
  }

  #if HAS_EXTRA_ENDSTOPS
    const bool pos_dir = axis_home_dir > 0;
    #if ENABLED(X_DUAL_ENDSTOPS)
      if (axis == X_AXIS) {
        const float adj = ABS(endstops.x2_endstop_adj);
        if (adj) {
          if (pos_dir ? (endstops.x2_endstop_adj > 0) : (endstops.x2_endstop_adj < 0)) stepper.set_x_lock(true); else stepper.set_x2_lock(true);
          do_homing_move(axis, pos_dir ? -adj : adj);
          stepper.set_x_lock(false);
          stepper.set_x2_lock(false);
        }
      }
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      if (axis == Y_AXIS) {
        const float adj = ABS(endstops.y2_endstop_adj);
        if (adj) {
          if (pos_dir ? (endstops.y2_endstop_adj > 0) : (endstops.y2_endstop_adj < 0)) stepper.set_y_lock(true); else stepper.set_y2_lock(true);
          do_homing_move(axis, pos_dir ? -adj : adj);
          stepper.set_y_lock(false);
          stepper.set_y2_lock(false);
        }
      }
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      if (axis == Z_AXIS) {
        const float adj = ABS(endstops.z2_endstop_adj);
        if (adj) {
          if (pos_dir ? (endstops.z2_endstop_adj > 0) : (endstops.z2_endstop_adj < 0)) stepper.set_z_lock(true); else stepper.set_z2_lock(true);
          do_homing_move(axis, pos_dir ? -adj : adj);
          stepper.set_z_lock(false);
          stepper.set_z2_lock(false);
        }
      }
    #endif
    #if ENABLED(Z_TRIPLE_ENDSTOPS)
      if (axis == Z_AXIS) {
        // we push the function pointers for the stepper lock function into an array
        void (*lock[3]) (bool)= {&stepper.set_z_lock, &stepper.set_z2_lock, &stepper.set_z3_lock};
        float adj[3] = {0, endstops.z2_endstop_adj, endstops.z3_endstop_adj};

        void (*tempLock) (bool);
        float tempAdj;

        // manual bubble sort by adjust value
        if (adj[1] < adj[0]) {
          tempLock = lock[0], tempAdj = adj[0];
          lock[0] = lock[1], adj[0] = adj[1];
          lock[1] = tempLock, adj[1] = tempAdj;
        }
        if (adj[2] < adj[1]) {
          tempLock = lock[1], tempAdj = adj[1];
          lock[1] = lock[2], adj[1] = adj[2];
          lock[2] = tempLock, adj[2] = tempAdj;
        }
        if (adj[1] < adj[0]) {
          tempLock = lock[0], tempAdj = adj[0];
          lock[0] = lock[1], adj[0] = adj[1];
          lock[1] = tempLock, adj[1] = tempAdj;
        }

        if (pos_dir) {
          // normalize adj to smallest value and do the first move
          (*lock[0])(true);
          do_homing_move(axis, adj[1] - adj[0]);
          // lock the second stepper for the final correction
          (*lock[1])(true);
          do_homing_move(axis, adj[2] - adj[1]);
        }
        else {
          (*lock[2])(true);
          do_homing_move(axis, adj[1] - adj[2]);
          (*lock[1])(true);
          do_homing_move(axis, adj[0] - adj[1]);
        }

        stepper.set_z_lock(false);
        stepper.set_z2_lock(false);
        stepper.set_z3_lock(false);
      }
    #endif

    // Reset flags for X, Y, Z motor locking
    switch (axis) {
      #if ENABLED(X_DUAL_ENDSTOPS)
        case X_AXIS:
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        case Y_AXIS:
      #endif
      #if Z_MULTI_ENDSTOPS
        case Z_AXIS:
      #endif
      stepper.set_separate_multi_axis(false);
      default: break;
    }
  #endif

  #if IS_SCARA

    set_axis_is_at_home(axis);
    sync_plan_position();

  #elif ENABLED(DELTA)

    // Delta has already moved all three towers up in G28
    // so here it re-homes each tower in turn.
    // Delta homing treats the axes as normal linear axes.

    // retrace by the amount specified in delta_endstop_adj + additional dist in order to have minimum steps
    if (delta_endstop_adj[axis] * Z_HOME_DIR <= 0) {
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("delta_endstop_adj:");
      #endif
      do_homing_move(axis, delta_endstop_adj[axis] - (MIN_STEPS_PER_SEGMENT + 1) * planner.steps_to_mm[axis] * Z_HOME_DIR);
    }

  #else // CARTESIAN / CORE

    set_axis_is_at_home(axis);
    sync_plan_position();

    destination[axis] = current_position[axis];

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) DEBUG_POS("> AFTER set_axis_is_at_home", current_position);
    #endif

  #endif

  // Put away the Z probe
  #if HOMING_Z_WITH_PROBE
    if (axis == Z_AXIS && STOW_PROBE()) return;
  #endif

  // Clear retracted status if homing the Z axis
  #if ENABLED(FWRETRACT)
    if (axis == Z_AXIS) fwretract.current_hop = 0.0;
  #endif

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPAIR("<<< homeaxis(", axis_codes[axis]);
      SERIAL_CHAR(')');
      SERIAL_EOL();
    }
  #endif
} // homeaxis()

#if HAS_WORKSPACE_OFFSET
  void update_workspace_offset(const AxisEnum axis) {
    workspace_offset[axis] = home_offset[axis] + position_shift[axis];
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPAIR("For ", axis_codes[axis]);
        SERIAL_ECHOPAIR(" axis:\n home_offset = ", home_offset[axis]);
        SERIAL_ECHOLNPAIR("\n position_shift = ", position_shift[axis]);
      }
    #endif
  }
#endif

#if HAS_M206_COMMAND
  /**
   * Change the home offset for an axis.
   * Also refreshes the workspace offset.
   */
  void set_home_offset(const AxisEnum axis, const float v) {
    home_offset[axis] = v;
    update_workspace_offset(axis);
  }
#endif // HAS_M206_COMMAND
