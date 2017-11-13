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

#if IS_SCARA
  #include "../module/scara.h"
#endif

extern bool relative_mode;

extern float current_position[XYZE],  // High-level current tool position
             destination[XYZE];       // Destination for a move

// Scratch space for a cartesian result
extern float cartes[XYZ];

// Until kinematics.cpp is created, declare this here
#if IS_KINEMATIC
  extern float delta[ABC];
#endif

#if OLDSCHOOL_ABL
  extern float xy_probe_feedrate_mm_s;
  #define XY_PROBE_FEEDRATE_MM_S xy_probe_feedrate_mm_s
#elif defined(XY_PROBE_SPEED)
  #define XY_PROBE_FEEDRATE_MM_S MMM_TO_MMS(XY_PROBE_SPEED)
#else
  #define XY_PROBE_FEEDRATE_MM_S PLANNER_XY_FEEDRATE()
#endif

/**
 * Feed rates are often configured with mm/m
 * but the planner and stepper like mm/s units.
 */
extern const float homing_feedrate_mm_s[4];
FORCE_INLINE float homing_feedrate(const AxisEnum a) { return pgm_read_float(&homing_feedrate_mm_s[a]); }

extern float feedrate_mm_s;

/**
 * Feedrate scaling and conversion
 */
extern int16_t feedrate_percentage;
#define MMS_SCALED(MM_S) ((MM_S)*feedrate_percentage*0.01)

extern uint8_t active_extruder;

#if HOTENDS > 1
  extern float hotend_offset[XYZ][HOTENDS];
#endif

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

void report_current_position();

inline void set_current_from_destination() { COPY(current_position, destination); }
inline void set_destination_from_current() { COPY(destination, current_position); }

void get_cartesian_from_steppers();
void set_current_from_steppers_for_axis(const AxisEnum axis);

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
void buffer_line_to_destination(const float fr_mm_s);

#if IS_KINEMATIC
  void prepare_uninterpolated_move_to_destination(const float fr_mm_s=0.0);
#endif

void prepare_move_to_destination();

/**
 * Blocking movement and shorthand functions
 */
void do_blocking_move_to(const float &x, const float &y, const float &z, const float &fr_mm_s=0.0);
void do_blocking_move_to_x(const float &x, const float &fr_mm_s=0.0);
void do_blocking_move_to_z(const float &z, const float &fr_mm_s=0.0);
void do_blocking_move_to_xy(const float &x, const float &y, const float &fr_mm_s=0.0);

void setup_for_endstop_or_probe_move();
void clean_up_after_endstop_or_probe_move();

void bracket_probe_move(const bool before);
void setup_for_endstop_or_probe_move();
void clean_up_after_endstop_or_probe_move();

//
// Homing
//

#define HAS_AXIS_UNHOMED_ERR (                                                     \
         ENABLED(Z_PROBE_ALLEN_KEY)                                                \
      || ENABLED(Z_PROBE_SLED)                                                     \
      || HAS_PROBING_PROCEDURE                                                     \
      || HOTENDS > 1                                                               \
      || ENABLED(NOZZLE_CLEAN_FEATURE)                                             \
      || ENABLED(NOZZLE_PARK_FEATURE)                                              \
      || (ENABLED(ADVANCED_PAUSE_FEATURE) && ENABLED(HOME_BEFORE_FILAMENT_CHANGE)) \
      || HAS_M206_COMMAND                                                          \
    ) || ENABLED(NO_MOTION_BEFORE_HOMING)

#if HAS_AXIS_UNHOMED_ERR
  bool axis_unhomed_error(const bool x=true, const bool y=true, const bool z=true);
#endif

#if ENABLED(NO_MOTION_BEFORE_HOMING)
  #define MOTION_CONDITIONS (IsRunning() && !axis_unhomed_error())
#else
  #define MOTION_CONDITIONS IsRunning()
#endif

void set_axis_is_at_home(const AxisEnum axis);

void homeaxis(const AxisEnum axis);
#define HOMEAXIS(LETTER) homeaxis(LETTER##_AXIS)

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

#define NATIVE_TO_LOGICAL(POS, AXIS) ((POS) + WORKSPACE_OFFSET(AXIS))
#define LOGICAL_TO_NATIVE(POS, AXIS) ((POS) - WORKSPACE_OFFSET(AXIS))

#if HAS_POSITION_SHIFT || DISABLED(DELTA)
  #define LOGICAL_X_POSITION(POS)   NATIVE_TO_LOGICAL(POS, X_AXIS)
  #define LOGICAL_Y_POSITION(POS)   NATIVE_TO_LOGICAL(POS, Y_AXIS)
  #define RAW_X_POSITION(POS)       LOGICAL_TO_NATIVE(POS, X_AXIS)
  #define RAW_Y_POSITION(POS)       LOGICAL_TO_NATIVE(POS, Y_AXIS)
#else
  #define LOGICAL_X_POSITION(POS)   (POS)
  #define LOGICAL_Y_POSITION(POS)   (POS)
  #define RAW_X_POSITION(POS)       (POS)
  #define RAW_Y_POSITION(POS)       (POS)
#endif

#define LOGICAL_Z_POSITION(POS)     NATIVE_TO_LOGICAL(POS, Z_AXIS)
#define RAW_Z_POSITION(POS)         LOGICAL_TO_NATIVE(POS, Z_AXIS)

/**
 * position_is_reachable family of functions
 */

#if IS_KINEMATIC // (DELTA or SCARA)

  inline bool position_is_reachable(const float &rx, const float &ry) {
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

  inline bool position_is_reachable_by_probe(const float &rx, const float &ry) {

    // Both the nozzle and the probe must be able to reach the point.
    // This won't work on SCARA since the probe offset rotates with the arm.

    return position_is_reachable(rx, ry)
        && position_is_reachable(rx - (X_PROBE_OFFSET_FROM_EXTRUDER), ry - (Y_PROBE_OFFSET_FROM_EXTRUDER));
  }

#else // CARTESIAN

  inline bool position_is_reachable(const float &rx, const float &ry) {
      // Add 0.001 margin to deal with float imprecision
      return WITHIN(rx, X_MIN_POS - 0.001, X_MAX_POS + 0.001)
          && WITHIN(ry, Y_MIN_POS - 0.001, Y_MAX_POS + 0.001);
  }

  inline bool position_is_reachable_by_probe(const float &rx, const float &ry) {
      // Add 0.001 margin to deal with float imprecision
      return WITHIN(rx, MIN_PROBE_X - 0.001, MAX_PROBE_X + 0.001)
          && WITHIN(ry, MIN_PROBE_Y - 0.001, MAX_PROBE_Y + 0.001);
  }

#endif // CARTESIAN

/**
 * Dual X Carriage / Dual Nozzle
 */
#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  extern bool extruder_duplication_enabled;       // Used in Dual X mode 2
#endif

/**
 * Dual X Carriage
 */
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

#if HAS_WORKSPACE_OFFSET || ENABLED(DUAL_X_CARRIAGE)
  void update_software_endstops(const AxisEnum axis);
#endif

#if HAS_M206_COMMAND
  void set_home_offset(const AxisEnum axis, const float v);
#endif

#endif // MOTION_H
