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
 * motion.h
 *
 * High-level motion commands to feed the planner
 * Some of these methods may migrate to the planner class.
 */

#ifndef MOTION_H
#define MOTION_H

#include "../inc/MarlinConfig.h"

//#include "../HAL/HAL.h"

// #if ENABLED(DELTA)
//   #include "../module/delta.h"
// #endif

extern bool relative_mode;

extern float current_position[XYZE], destination[XYZE];

extern float feedrate_mm_s;

extern uint8_t active_extruder;

extern float soft_endstop_min[XYZ], soft_endstop_max[XYZ];

FORCE_INLINE float pgm_read_any(const float *p) { return pgm_read_float_near(p); }
FORCE_INLINE signed char pgm_read_any(const signed char *p) { return pgm_read_byte_near(p); }

#define XYZ_DEFS(type, array, CONFIG) \
  extern const type array##_P[XYZ]; \
  FORCE_INLINE type array(AxisEnum axis) { return pgm_read_any(&array##_P[axis]); } \
  typedef void __void_##CONFIG##__

XYZ_DEFS(float, base_min_pos,   MIN_POS);
XYZ_DEFS(float, base_max_pos,   MAX_POS);
XYZ_DEFS(float, base_home_pos,  HOME_POS);
XYZ_DEFS(float, max_length,     MAX_LENGTH);
XYZ_DEFS(float, home_bump_mm,   HOME_BUMP_MM);
XYZ_DEFS(signed char, home_dir, HOME_DIR);

#if HAS_SOFTWARE_ENDSTOPS
  extern bool soft_endstops_enabled;
  void clamp_to_software_endstops(float target[XYZ]);
#else
  #define soft_endstops_enabled false
  #define clamp_to_software_endstops(x) NOOP
#endif

inline void set_current_to_destination() { COPY(current_position, destination); }
inline void set_destination_to_current() { COPY(destination, current_position); }

/**
 * sync_plan_position
 *
 * Set the planner/stepper positions directly from current_position with
 * no kinematic translation. Used for homing axes and cartesian/core syncing.
 */
void sync_plan_position();
void sync_plan_position_e();

#if IS_KINEMATIC
  void sync_plan_position_kinematic();
  #define SYNC_PLAN_POSITION_KINEMATIC() sync_plan_position_kinematic()
#else
  #define SYNC_PLAN_POSITION_KINEMATIC() sync_plan_position()
#endif

/**
 * Move the planner to the current position from wherever it last moved
 * (or from wherever it has been told it is located).
 */
void line_to_current_position();

/**
 * Move the planner to the position stored in the destination array, which is
 * used by G0/G1/G2/G3/G5 and many other functions to set a destination.
 */
void line_to_destination(const float fr_mm_s);

inline void line_to_destination() { line_to_destination(feedrate_mm_s); }

#if IS_KINEMATIC
  void prepare_uninterpolated_move_to_destination(const float fr_mm_s=0.0);
#endif

void prepare_move_to_destination();

void clamp_to_software_endstops(float target[XYZ]);

//
// Macros
//

// Workspace offsets
#if HAS_WORKSPACE_OFFSET
  #if HAS_HOME_OFFSET
    extern float home_offset[XYZ];
  #endif
  #if HAS_POSITION_SHIFT
    extern float position_shift[XYZ];
  #endif
#endif

#if HAS_HOME_OFFSET && HAS_POSITION_SHIFT
  extern float workspace_offset[XYZ];
  #define WORKSPACE_OFFSET(AXIS) workspace_offset[AXIS]
#elif HAS_HOME_OFFSET
  #define WORKSPACE_OFFSET(AXIS) home_offset[AXIS]
#elif HAS_POSITION_SHIFT
  #define WORKSPACE_OFFSET(AXIS) position_shift[AXIS]
#else
  #define WORKSPACE_OFFSET(AXIS) 0
#endif

#define LOGICAL_POSITION(POS, AXIS) ((POS) + WORKSPACE_OFFSET(AXIS))
#define RAW_POSITION(POS, AXIS)     ((POS) - WORKSPACE_OFFSET(AXIS))

#if HAS_POSITION_SHIFT || DISABLED(DELTA)
  #define LOGICAL_X_POSITION(POS)   LOGICAL_POSITION(POS, X_AXIS)
  #define LOGICAL_Y_POSITION(POS)   LOGICAL_POSITION(POS, Y_AXIS)
  #define RAW_X_POSITION(POS)       RAW_POSITION(POS, X_AXIS)
  #define RAW_Y_POSITION(POS)       RAW_POSITION(POS, Y_AXIS)
#else
  #define LOGICAL_X_POSITION(POS)   (POS)
  #define LOGICAL_Y_POSITION(POS)   (POS)
  #define RAW_X_POSITION(POS)       (POS)
  #define RAW_Y_POSITION(POS)       (POS)
#endif

#define LOGICAL_Z_POSITION(POS)     LOGICAL_POSITION(POS, Z_AXIS)
#define RAW_Z_POSITION(POS)         RAW_POSITION(POS, Z_AXIS)
#define RAW_CURRENT_POSITION(A)     RAW_##A##_POSITION(current_position[A##_AXIS])

/**
 * position_is_reachable family of functions
 */

#if IS_KINEMATIC // (DELTA or SCARA)

  #if IS_SCARA
    extern const float L1, L2;
  #endif

  inline bool position_is_reachable_raw_xy(const float &rx, const float &ry) {
    #if ENABLED(DELTA)
      return HYPOT2(rx, ry) <= sq(DELTA_PRINTABLE_RADIUS);
    #elif IS_SCARA
      #if MIDDLE_DEAD_ZONE_R > 0
        const float R2 = HYPOT2(rx - SCARA_OFFSET_X, ry - SCARA_OFFSET_Y);
        return R2 >= sq(float(MIDDLE_DEAD_ZONE_R)) && R2 <= sq(L1 + L2);
      #else
        return HYPOT2(rx - SCARA_OFFSET_X, ry - SCARA_OFFSET_Y) <= sq(L1 + L2);
      #endif
    #else // CARTESIAN
      // To be migrated from MakerArm branch in future
    #endif
  }

  inline bool position_is_reachable_by_probe_raw_xy(const float &rx, const float &ry) {

    // Both the nozzle and the probe must be able to reach the point.
    // This won't work on SCARA since the probe offset rotates with the arm.

    return position_is_reachable_raw_xy(rx, ry)
        && position_is_reachable_raw_xy(rx - X_PROBE_OFFSET_FROM_EXTRUDER, ry - Y_PROBE_OFFSET_FROM_EXTRUDER);
  }

#else // CARTESIAN

  inline bool position_is_reachable_raw_xy(const float &rx, const float &ry) {
      // Add 0.001 margin to deal with float imprecision
      return WITHIN(rx, X_MIN_POS - 0.001, X_MAX_POS + 0.001)
          && WITHIN(ry, Y_MIN_POS - 0.001, Y_MAX_POS + 0.001);
  }

  inline bool position_is_reachable_by_probe_raw_xy(const float &rx, const float &ry) {
      // Add 0.001 margin to deal with float imprecision
      return WITHIN(rx, MIN_PROBE_X - 0.001, MAX_PROBE_X + 0.001)
          && WITHIN(ry, MIN_PROBE_Y - 0.001, MAX_PROBE_Y + 0.001);
  }

#endif // CARTESIAN

FORCE_INLINE bool position_is_reachable_by_probe_xy(const float &lx, const float &ly) {
  return position_is_reachable_by_probe_raw_xy(RAW_X_POSITION(lx), RAW_Y_POSITION(ly));
}

FORCE_INLINE bool position_is_reachable_xy(const float &lx, const float &ly) {
  return position_is_reachable_raw_xy(RAW_X_POSITION(lx), RAW_Y_POSITION(ly));
}

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  extern bool extruder_duplication_enabled;       // Used in Dual X mode 2
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  extern DualXMode dual_x_carriage_mode;
  extern float inactive_extruder_x_pos,           // used in mode 0 & 1
               raised_parked_position[XYZE],      // used in mode 1
               duplicate_extruder_x_offset;       // used in mode 2
  extern bool active_extruder_parked;             // used in mode 1 & 2
  extern millis_t delayed_move_time;              // used in mode 1
  extern int16_t duplicate_extruder_temp_offset;  // used in mode 2

  float x_home_pos(const int extruder);

  FORCE_INLINE int x_home_dir(const uint8_t extruder) { return extruder ? X2_HOME_DIR : X_HOME_DIR; }

#endif // DUAL_X_CARRIAGE

#endif // MOTION_H
