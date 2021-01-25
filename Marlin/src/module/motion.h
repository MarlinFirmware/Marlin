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
  extern feedRate_t xy_probe_feedrate_mm_s;
  #define XY_PROBE_FEEDRATE_MM_S xy_probe_feedrate_mm_s
#elif defined(XY_PROBE_SPEED)
  #define XY_PROBE_FEEDRATE_MM_S MMM_TO_MMS(XY_PROBE_SPEED)
#else
  #define XY_PROBE_FEEDRATE_MM_S PLANNER_XY_FEEDRATE()
#endif

constexpr feedRate_t z_probe_fast_mm_s = MMM_TO_MMS(Z_PROBE_SPEED_FAST);

/**
 * Feed rates are often configured with mm/m
 * but the planner and stepper like mm/s units.
 */
constexpr xyz_feedrate_t homing_feedrate_mm_m = HOMING_FEEDRATE_MM_M;
FORCE_INLINE feedRate_t homing_feedrate(const AxisEnum a) {
  float v;
  #if ENABLED(DELTA)
    v = homing_feedrate_mm_m.z;
  #else
    switch (a) {
      case X_AXIS: v = homing_feedrate_mm_m.x; break;
      case Y_AXIS: v = homing_feedrate_mm_m.y; break;
      case Z_AXIS:
          default: v = homing_feedrate_mm_m.z;
    }
  #endif
  return MMM_TO_MMS(v);
}

feedRate_t get_homing_bump_feedrate(const AxisEnum axis);

/**
 * The default feedrate for many moves, set by the most recent move
 */
extern feedRate_t feedrate_mm_s;

/**
 * Feedrate scaling is applied to all G0/G1, G2/G3, and G5 moves
 */
extern int16_t feedrate_percentage;
#define MMS_SCALED(V) ((V) * 0.01f * feedrate_percentage)

// The active extruder (tool). Set with T<extruder> command.
#if HAS_MULTI_EXTRUDER
  extern uint8_t active_extruder;
#else
  constexpr uint8_t active_extruder = 0;
#endif

#if ENABLED(LCD_SHOW_E_TOTAL)
  extern float e_move_accumulator;
#endif

#ifdef __IMXRT1062__
  #define DEFS_PROGMEM
#else
  #define DEFS_PROGMEM PROGMEM
#endif

inline float pgm_read_any(const float *p)   { return TERN(__IMXRT1062__, *p, pgm_read_float(p)); }
inline int8_t pgm_read_any(const int8_t *p) { return TERN(__IMXRT1062__, *p, pgm_read_byte(p)); }

#define XYZ_DEFS(T, NAME, OPT) \
  inline T NAME(const AxisEnum axis) { \
    static const XYZval<T> NAME##_P DEFS_PROGMEM = { X_##OPT, Y_##OPT, Z_##OPT }; \
    return pgm_read_any(&NAME##_P[axis]); \
  }
XYZ_DEFS(float, base_min_pos,   MIN_POS);
XYZ_DEFS(float, base_max_pos,   MAX_POS);
XYZ_DEFS(float, base_home_pos,  HOME_POS);
XYZ_DEFS(float, max_length,     MAX_LENGTH);
XYZ_DEFS(int8_t, home_dir, HOME_DIR);

inline float home_bump_mm(const AxisEnum axis) {
  static const xyz_pos_t home_bump_mm_P DEFS_PROGMEM = HOMING_BUMP_MM;
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

#if HAS_SOFTWARE_ENDSTOPS

  typedef struct {
    bool _enabled, _loose;
    bool enabled() { return _enabled && !_loose; }

    xyz_pos_t min, max;
    void get_manual_axis_limits(const AxisEnum axis, float &amin, float &amax) {
      amin = -100000; amax = 100000; // "No limits"
      #if HAS_SOFTWARE_ENDSTOPS
        if (enabled()) switch (axis) {
          case X_AXIS:
            TERN_(MIN_SOFTWARE_ENDSTOP_X, amin = min.x);
            TERN_(MAX_SOFTWARE_ENDSTOP_X, amax = max.x);
            break;
          case Y_AXIS:
            TERN_(MIN_SOFTWARE_ENDSTOP_Y, amin = min.y);
            TERN_(MAX_SOFTWARE_ENDSTOP_Y, amax = max.y);
            break;
          case Z_AXIS:
            TERN_(MIN_SOFTWARE_ENDSTOP_Z, amin = min.z);
            TERN_(MAX_SOFTWARE_ENDSTOP_Z, amax = max.z);
          default: break;
        }
      #endif
    }
  } soft_endstops_t;

  extern soft_endstops_t soft_endstop;
  void apply_motion_limits(xyz_pos_t &target);
  void update_software_endstops(const AxisEnum axis
    #if HAS_HOTEND_OFFSET
      , const uint8_t old_tool_index=0, const uint8_t new_tool_index=0
    #endif
  );
  #define SET_SOFT_ENDSTOP_LOOSE(loose) (soft_endstop._loose = loose)

#else // !HAS_SOFTWARE_ENDSTOPS

  typedef struct {
    bool enabled() { return false; }
    void get_manual_axis_limits(const AxisEnum axis, float &amin, float &amax) {
      // No limits
      amin = current_position[axis] - 1000;
      amax = current_position[axis] + 1000;
    }
  } soft_endstops_t;
  extern soft_endstops_t soft_endstop;
  #define apply_motion_limits(V)        NOOP
  #define update_software_endstops(...) NOOP
  #define SET_SOFT_ENDSTOP_LOOSE(V)     NOOP

#endif // !HAS_SOFTWARE_ENDSTOPS

void report_real_position();
void report_current_position();
void report_current_position_projected();

void get_cartesian_from_steppers();
void set_current_from_steppers_for_axis(const AxisEnum axis);

void quickstop_stepper();

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

void do_z_clearance(const float &zclear, const bool z_trusted=true, const bool raise_on_untrusted=true, const bool lower_allowed=false);

/**
 * Homing and Trusted Axes
 */
constexpr uint8_t xyz_bits = _BV(X_AXIS) | _BV(Y_AXIS) | _BV(Z_AXIS);
extern uint8_t axis_homed, axis_trusted;

void homeaxis(const AxisEnum axis);
void set_axis_is_at_home(const AxisEnum axis);
void set_axis_never_homed(const AxisEnum axis);
uint8_t axes_should_home(uint8_t axis_bits=0x07);
bool homing_needed_error(uint8_t axis_bits=0x07);

FORCE_INLINE bool axis_was_homed(const AxisEnum axis)     { return TEST(axis_homed, axis); }
FORCE_INLINE bool axis_is_trusted(const AxisEnum axis)    { return TEST(axis_trusted, axis); }
FORCE_INLINE bool axis_should_home(const AxisEnum axis)   { return (axes_should_home() & _BV(axis)) != 0; }
FORCE_INLINE bool no_axes_homed()                         { return !axis_homed; }
FORCE_INLINE bool all_axes_homed()                        { return xyz_bits == (axis_homed & xyz_bits); }
FORCE_INLINE bool homing_needed()                         { return !all_axes_homed(); }
FORCE_INLINE bool all_axes_trusted()                      { return xyz_bits == (axis_trusted & xyz_bits); }
FORCE_INLINE void set_axis_homed(const AxisEnum axis)     { SBI(axis_homed, axis); }
FORCE_INLINE void set_axis_unhomed(const AxisEnum axis)   { CBI(axis_homed, axis); }
FORCE_INLINE void set_axis_trusted(const AxisEnum axis)   { SBI(axis_trusted, axis); }
FORCE_INLINE void set_axis_untrusted(const AxisEnum axis) { CBI(axis_trusted, axis); }
FORCE_INLINE void set_all_homed()                         { axis_homed = axis_trusted = xyz_bits; }
FORCE_INLINE void set_all_unhomed()                       { axis_homed = axis_trusted = 0; }

#if ENABLED(NO_MOTION_BEFORE_HOMING)
  #define MOTION_CONDITIONS (IsRunning() && !homing_needed_error())
#else
  #define MOTION_CONDITIONS IsRunning()
#endif

#define BABYSTEP_ALLOWED() ((ENABLED(BABYSTEP_WITHOUT_HOMING) || all_axes_trusted()) && (ENABLED(BABYSTEP_ALWAYS_AVAILABLE) || printer_busy()))

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
  extern bool extruder_duplication_enabled;       // Used in Dual X mode 2
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
  extern float inactive_extruder_x,               // Used in mode 0 & 1
               duplicate_extruder_x_offset;       // Used in mode 2 & 3
  extern xyz_pos_t raised_parked_position;        // Used in mode 1
  extern bool active_extruder_parked;             // Used in mode 1, 2 & 3
  extern millis_t delayed_move_time;              // Used in mode 1
  extern int16_t duplicate_extruder_temp_offset;  // Used in mode 2 & 3
  extern bool idex_mirrored_mode;                 // Used in mode 3

  FORCE_INLINE bool idex_is_duplicating() { return dual_x_carriage_mode >= DXC_DUPLICATION_MODE; }

  float x_home_pos(const uint8_t extruder);

  FORCE_INLINE int x_home_dir(const uint8_t extruder) { return extruder ? X2_HOME_DIR : X_HOME_DIR; }

  void set_duplication_enabled(const bool dupe, const int8_t tool_index=-1);
  void idex_set_mirrored_mode(const bool mirr);
  void idex_set_parked(const bool park=true);

#else

  #if ENABLED(MULTI_NOZZLE_DUPLICATION)
    extern uint8_t duplication_e_mask;
    enum DualXMode : char { DXC_DUPLICATION_MODE = 2 };
    FORCE_INLINE void set_duplication_enabled(const bool dupe) { extruder_duplication_enabled = dupe; }
  #endif

  FORCE_INLINE int x_home_dir(const uint8_t) { return home_dir(X_AXIS); }

#endif

#if HAS_M206_COMMAND
  void set_home_offset(const AxisEnum axis, const float v);
#endif

#if USE_SENSORLESS
  struct sensorless_t;
  sensorless_t start_sensorless_homing_per_axis(const AxisEnum axis);
  void end_sensorless_homing_per_axis(const AxisEnum axis, sensorless_t enable_stealth);
#endif
