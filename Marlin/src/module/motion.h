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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * motion.h
 *
 * High-level motion commands to feed the planner
 * Some of these methods may migrate to the planner class.
 */

#include "../inc/MarlinConfig.h"

#if IS_SCARA
  #include "scara.h"
#endif

// Axis homed and known-position states
extern uint8_t axis_homed, axis_known_position;
constexpr uint8_t xyz_bits = _BV(X_AXIS) | _BV(Y_AXIS) | _BV(Z_AXIS);
FORCE_INLINE bool no_axes_homed() { return !axis_homed; }
FORCE_INLINE bool all_axes_homed() { return (axis_homed & xyz_bits) == xyz_bits; }
FORCE_INLINE bool all_axes_known() { return (axis_known_position & xyz_bits) == xyz_bits; }
FORCE_INLINE void set_all_unhomed() { axis_homed = 0; }
FORCE_INLINE void set_all_unknown() { axis_known_position = 0; }

FORCE_INLINE bool homing_needed() {
  return !TERN(HOME_AFTER_DEACTIVATE, all_axes_known, all_axes_homed)();
}

// Error margin to work around float imprecision
constexpr float fslop = 0.0001;

extern bool relative_mode;

extern xyze_pos_t current_position,  // High-level current tool position
                  destination;       // Destination for a move

// G60/G61 Position Save and Return
#if SAVED_POSITIONS
  extern uint8_t saved_slots[(SAVED_POSITIONS + 7) >> 3];
  extern xyz_pos_t stored_position[SAVED_POSITIONS];
#endif

// Scratch space for a cartesian result
extern xyz_pos_t cartes;

// Until kinematics.cpp is created, declare this here
#if IS_KINEMATIC
  extern abc_pos_t delta;
#endif

#if HAS_ABL_NOT_UBL
  extern float xy_probe_feedrate_mm_s;
  #define XY_PROBE_FEEDRATE_MM_S xy_probe_feedrate_mm_s
#elif defined(XY_PROBE_SPEED)
  #define XY_PROBE_FEEDRATE_MM_S MMM_TO_MMS(XY_PROBE_SPEED)
#else
  #define XY_PROBE_FEEDRATE_MM_S PLANNER_XY_FEEDRATE()
#endif

#if ENABLED(Z_SAFE_HOMING)
  constexpr xy_float_t safe_homing_xy = { Z_SAFE_HOMING_X_POINT, Z_SAFE_HOMING_Y_POINT };
#endif

/**
 * Feed rates are often configured with mm/m
 * but the planner and stepper like mm/s units.
 */
extern const feedRate_t homing_feedrate_mm_s[XYZ];
FORCE_INLINE feedRate_t homing_feedrate(const AxisEnum a) { return pgm_read_float(&homing_feedrate_mm_s[a]); }
feedRate_t get_homing_bump_feedrate(const AxisEnum axis);

extern feedRate_t feedrate_mm_s;

/**
 * Feedrate scaling
 */
extern int16_t feedrate_percentage;

// The active extruder (tool). Set with T<extruder> command.
#if EXTRUDERS > 1
  extern uint8_t active_extruder;
#else
  constexpr uint8_t active_extruder = 0;
#endif

#if ENABLED(LCD_SHOW_E_TOTAL)
  extern float e_move_accumulator;
#endif

inline float pgm_read_any(const float *p) { return pgm_read_float(p); }
inline signed char pgm_read_any(const signed char *p) { return pgm_read_byte(p); }

#define XYZ_DEFS(T, NAME, OPT) \
  inline T NAME(const AxisEnum axis) { \
    static const XYZval<T> NAME##_P PROGMEM = { X_##OPT, Y_##OPT, Z_##OPT }; \
    return pgm_read_any(&NAME##_P[axis]); \
  }
XYZ_DEFS(float, base_min_pos,   MIN_POS);
XYZ_DEFS(float, base_max_pos,   MAX_POS);
XYZ_DEFS(float, base_home_pos,  HOME_POS);
XYZ_DEFS(float, max_length,     MAX_LENGTH);
XYZ_DEFS(signed char, home_dir, HOME_DIR);

inline float home_bump_mm(const AxisEnum axis) {
  static const xyz_pos_t home_bump_mm_P PROGMEM = HOMING_BUMP_MM;
  return pgm_read_any(&home_bump_mm_P[axis]);
}

#if HAS_WORKSPACE_OFFSET
  void update_workspace_offset(const AxisEnum axis);
#else
  inline void update_workspace_offset(const AxisEnum) {}
#endif

#if HAS_HOTEND_OFFSET
  extern xyz_pos_t hotend_offset[HOTENDS];
  void reset_hotend_offsets();
#elif HOTENDS
  constexpr xyz_pos_t hotend_offset[HOTENDS] = { { 0 } };
#else
  constexpr xyz_pos_t hotend_offset[1] = { { 0 } };
#endif

typedef struct { xyz_pos_t min, max; } axis_limits_t;
#if HAS_SOFTWARE_ENDSTOPS
  extern bool soft_endstops_enabled;
  extern axis_limits_t soft_endstop;
  void apply_motion_limits(xyz_pos_t &target);
  void update_software_endstops(const AxisEnum axis
    #if HAS_HOTEND_OFFSET
      , const uint8_t old_tool_index=0, const uint8_t new_tool_index=0
    #endif
  );
  #define TEMPORARY_SOFT_ENDSTOP_STATE(enable) REMEMBER(tes, soft_endstops_enabled, enable);
#else
  constexpr bool soft_endstops_enabled = false;
  //constexpr axis_limits_t soft_endstop = {
  //  { X_MIN_POS, Y_MIN_POS, Z_MIN_POS },
  //  { X_MAX_POS, Y_MAX_POS, Z_MAX_POS } };
  #define apply_motion_limits(V)    NOOP
  #define update_software_endstops(...) NOOP
  #define TEMPORARY_SOFT_ENDSTOP_STATE(...) NOOP
#endif

void report_real_position();
void report_current_position();
void report_current_position_projected();

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

/**
 * Move the planner to the current position from wherever it last moved
 * (or from wherever it has been told it is located).
 */
void line_to_current_position(const feedRate_t &fr_mm_s=feedrate_mm_s);

#if EXTRUDERS
  void unscaled_e_move(const float &length, const feedRate_t &fr_mm_s);
#endif

void prepare_line_to_destination();

void _internal_move_to_destination(const feedRate_t &fr_mm_s=0.0f
  #if IS_KINEMATIC
    , const bool is_fast=false
  #endif
);

inline void prepare_internal_move_to_destination(const feedRate_t &fr_mm_s=0.0f) {
  _internal_move_to_destination(fr_mm_s);
}

#if IS_KINEMATIC
  void prepare_fast_move_to_destination(const feedRate_t &scaled_fr_mm_s=MMS_SCALED(feedrate_mm_s));

  inline void prepare_internal_fast_move_to_destination(const feedRate_t &fr_mm_s=0.0f) {
    _internal_move_to_destination(fr_mm_s, true);
  }
#endif

/**
 * Blocking movement and shorthand functions
 */
void do_blocking_move_to(const float rx, const float ry, const float rz, const feedRate_t &fr_mm_s=0.0f);
void do_blocking_move_to(const xy_pos_t &raw, const feedRate_t &fr_mm_s=0.0f);
void do_blocking_move_to(const xyz_pos_t &raw, const feedRate_t &fr_mm_s=0.0f);
void do_blocking_move_to(const xyze_pos_t &raw, const feedRate_t &fr_mm_s=0.0f);

void do_blocking_move_to_x(const float &rx, const feedRate_t &fr_mm_s=0.0f);
void do_blocking_move_to_y(const float &ry, const feedRate_t &fr_mm_s=0.0f);
void do_blocking_move_to_z(const float &rz, const feedRate_t &fr_mm_s=0.0f);

void do_blocking_move_to_xy(const float &rx, const float &ry, const feedRate_t &fr_mm_s=0.0f);
void do_blocking_move_to_xy(const xy_pos_t &raw, const feedRate_t &fr_mm_s=0.0f);
FORCE_INLINE void do_blocking_move_to_xy(const xyz_pos_t &raw, const feedRate_t &fr_mm_s=0.0f)  { do_blocking_move_to_xy(xy_pos_t(raw), fr_mm_s); }
FORCE_INLINE void do_blocking_move_to_xy(const xyze_pos_t &raw, const feedRate_t &fr_mm_s=0.0f) { do_blocking_move_to_xy(xy_pos_t(raw), fr_mm_s); }

void do_blocking_move_to_xy_z(const xy_pos_t &raw, const float &z, const feedRate_t &fr_mm_s=0.0f);
FORCE_INLINE void do_blocking_move_to_xy_z(const xyz_pos_t &raw, const float &z, const feedRate_t &fr_mm_s=0.0f)  { do_blocking_move_to_xy_z(xy_pos_t(raw), z, fr_mm_s); }
FORCE_INLINE void do_blocking_move_to_xy_z(const xyze_pos_t &raw, const float &z, const feedRate_t &fr_mm_s=0.0f) { do_blocking_move_to_xy_z(xy_pos_t(raw), z, fr_mm_s); }

void remember_feedrate_and_scaling();
void remember_feedrate_scaling_off();
void restore_feedrate_and_scaling();

//
// Homing
//

uint8_t axes_need_homing(uint8_t axis_bits=0x07);
bool axis_unhomed_error(uint8_t axis_bits=0x07);

#if ENABLED(NO_MOTION_BEFORE_HOMING)
  #define MOTION_CONDITIONS (IsRunning() && !axis_unhomed_error())
#else
  #define MOTION_CONDITIONS IsRunning()
#endif

void set_axis_is_at_home(const AxisEnum axis);

void set_axis_not_trusted(const AxisEnum axis);

void homeaxis(const AxisEnum axis);

/**
 * Workspace offsets
 */
#if HAS_HOME_OFFSET || HAS_POSITION_SHIFT
  #if HAS_HOME_OFFSET
    extern xyz_pos_t home_offset;
  #endif
  #if HAS_POSITION_SHIFT
    extern xyz_pos_t position_shift;
  #endif
  #if HAS_HOME_OFFSET && HAS_POSITION_SHIFT
    extern xyz_pos_t workspace_offset;
    #define _WS workspace_offset
  #elif HAS_HOME_OFFSET
    #define _WS home_offset
  #else
    #define _WS position_shift
  #endif
  #define NATIVE_TO_LOGICAL(POS, AXIS) ((POS) + _WS[AXIS])
  #define LOGICAL_TO_NATIVE(POS, AXIS) ((POS) - _WS[AXIS])
  FORCE_INLINE void toLogical(xy_pos_t &raw)   { raw += _WS; }
  FORCE_INLINE void toLogical(xyz_pos_t &raw)  { raw += _WS; }
  FORCE_INLINE void toLogical(xyze_pos_t &raw) { raw += _WS; }
  FORCE_INLINE void toNative(xy_pos_t &raw)    { raw -= _WS; }
  FORCE_INLINE void toNative(xyz_pos_t &raw)   { raw -= _WS; }
  FORCE_INLINE void toNative(xyze_pos_t &raw)  { raw -= _WS; }
#else
  #define NATIVE_TO_LOGICAL(POS, AXIS) (POS)
  #define LOGICAL_TO_NATIVE(POS, AXIS) (POS)
  FORCE_INLINE void toLogical(xy_pos_t&)   {}
  FORCE_INLINE void toLogical(xyz_pos_t&)  {}
  FORCE_INLINE void toLogical(xyze_pos_t&) {}
  FORCE_INLINE void toNative(xy_pos_t&)    {}
  FORCE_INLINE void toNative(xyz_pos_t&)   {}
  FORCE_INLINE void toNative(xyze_pos_t&)  {}
#endif
#define LOGICAL_X_POSITION(POS) NATIVE_TO_LOGICAL(POS, X_AXIS)
#define LOGICAL_Y_POSITION(POS) NATIVE_TO_LOGICAL(POS, Y_AXIS)
#define LOGICAL_Z_POSITION(POS) NATIVE_TO_LOGICAL(POS, Z_AXIS)
#define RAW_X_POSITION(POS)     LOGICAL_TO_NATIVE(POS, X_AXIS)
#define RAW_Y_POSITION(POS)     LOGICAL_TO_NATIVE(POS, Y_AXIS)
#define RAW_Z_POSITION(POS)     LOGICAL_TO_NATIVE(POS, Z_AXIS)

/**
 * position_is_reachable family of functions
 */

#if IS_KINEMATIC // (DELTA or SCARA)

  #if HAS_SCARA_OFFSET
    extern abc_pos_t scara_home_offset; // A and B angular offsets, Z mm offset
  #endif

  // Return true if the given point is within the printable area
  inline bool position_is_reachable(const float &rx, const float &ry, const float inset=0) {
    #if ENABLED(DELTA)
      return HYPOT2(rx, ry) <= sq(DELTA_PRINTABLE_RADIUS - inset + fslop);
    #elif IS_SCARA
      const float R2 = HYPOT2(rx - SCARA_OFFSET_X, ry - SCARA_OFFSET_Y);
      return (
        R2 <= sq(L1 + L2) - inset
        #if MIDDLE_DEAD_ZONE_R > 0
          && R2 >= sq(float(MIDDLE_DEAD_ZONE_R))
        #endif
      );
    #endif
  }

  inline bool position_is_reachable(const xy_pos_t &pos, const float inset=0) {
    return position_is_reachable(pos.x, pos.y, inset);
  }

#else // CARTESIAN

  // Return true if the given position is within the machine bounds.
  inline bool position_is_reachable(const float &rx, const float &ry) {
    if (!WITHIN(ry, Y_MIN_POS - fslop, Y_MAX_POS + fslop)) return false;
    #if ENABLED(DUAL_X_CARRIAGE)
      if (active_extruder)
        return WITHIN(rx, X2_MIN_POS - fslop, X2_MAX_POS + fslop);
      else
        return WITHIN(rx, X1_MIN_POS - fslop, X1_MAX_POS + fslop);
    #else
      return WITHIN(rx, X_MIN_POS - fslop, X_MAX_POS + fslop);
    #endif
  }
  inline bool position_is_reachable(const xy_pos_t &pos) { return position_is_reachable(pos.x, pos.y); }

#endif // CARTESIAN

/**
 * Duplication mode
 */
#if HAS_DUPLICATION_MODE
  extern bool extruder_duplication_enabled,       // Used in Dual X mode 2
              mirrored_duplication_mode;          // Used in Dual X mode 3
  #if ENABLED(MULTI_NOZZLE_DUPLICATION)
    extern uint8_t duplication_e_mask;
  #endif
#endif

/**
 * Dual X Carriage
 */
#if ENABLED(DUAL_X_CARRIAGE)

  enum DualXMode : char {
    DXC_FULL_CONTROL_MODE,
    DXC_AUTO_PARK_MODE,
    DXC_DUPLICATION_MODE,
    DXC_MIRRORED_MODE
  };

  extern DualXMode dual_x_carriage_mode;
  extern float inactive_extruder_x_pos,           // Used in mode 0 & 1
               duplicate_extruder_x_offset;       // Used in mode 2 & 3
  extern xyz_pos_t raised_parked_position;        // Used in mode 1
  extern bool active_extruder_parked;             // Used in mode 1, 2 & 3
  extern millis_t delayed_move_time;              // Used in mode 1
  extern int16_t duplicate_extruder_temp_offset;  // Used in mode 2 & 3

  FORCE_INLINE bool dxc_is_duplicating() { return dual_x_carriage_mode >= DXC_DUPLICATION_MODE; }

  float x_home_pos(const int extruder);

  FORCE_INLINE int x_home_dir(const uint8_t extruder) { return extruder ? X2_HOME_DIR : X_HOME_DIR; }

#else

  #if ENABLED(MULTI_NOZZLE_DUPLICATION)
    enum DualXMode : char { DXC_DUPLICATION_MODE = 2 };
  #endif

  FORCE_INLINE int x_home_dir(const uint8_t) { return home_dir(X_AXIS); }

#endif

#if HAS_M206_COMMAND
  void set_home_offset(const AxisEnum axis, const float v);
#endif
