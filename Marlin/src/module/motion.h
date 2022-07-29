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
  extern uint8_t saved_slots[(SAVED_POSITIONS + 7) >> 3]; // TODO: Add support for HAS_I_AXIS
  extern xyze_pos_t stored_position[SAVED_POSITIONS];
#endif

// Scratch space for a cartesian result
extern xyz_pos_t cartes;

// Until kinematics.cpp is created, declare this here
#if IS_KINEMATIC
  extern abce_pos_t delta;
#endif

#if HAS_ABL_NOT_UBL
  extern feedRate_t xy_probe_feedrate_mm_s;
  #define XY_PROBE_FEEDRATE_MM_S xy_probe_feedrate_mm_s
#elif defined(XY_PROBE_FEEDRATE)
  #define XY_PROBE_FEEDRATE_MM_S MMM_TO_MMS(XY_PROBE_FEEDRATE)
#else
  #define XY_PROBE_FEEDRATE_MM_S PLANNER_XY_FEEDRATE()
#endif

#if HAS_BED_PROBE
  constexpr feedRate_t z_probe_fast_mm_s = MMM_TO_MMS(Z_PROBE_FEEDRATE_FAST);
#endif

/**
 * Feed rates are often configured with mm/m
 * but the planner and stepper like mm/s units.
 */
constexpr xyz_feedrate_t homing_feedrate_mm_m = HOMING_FEEDRATE_MM_M;
FORCE_INLINE feedRate_t homing_feedrate(const AxisEnum a) {
  float v = TERN0(HAS_Z_AXIS, homing_feedrate_mm_m.z);
  #if DISABLED(DELTA)
    NUM_AXIS_CODE(
           if (a == X_AXIS) v = homing_feedrate_mm_m.x,
      else if (a == Y_AXIS) v = homing_feedrate_mm_m.y,
      else if (a == Z_AXIS) v = homing_feedrate_mm_m.z,
      else if (a == I_AXIS) v = homing_feedrate_mm_m.i,
      else if (a == J_AXIS) v = homing_feedrate_mm_m.j,
      else if (a == K_AXIS) v = homing_feedrate_mm_m.k
    );
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
    static const XYZval<T> NAME##_P DEFS_PROGMEM = NUM_AXIS_ARRAY(X_##OPT, Y_##OPT, Z_##OPT, I_##OPT, J_##OPT, K_##OPT); \
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
          #if HAS_Y_AXIS
            case Y_AXIS:
              TERN_(MIN_SOFTWARE_ENDSTOP_Y, amin = min.y);
              TERN_(MAX_SOFTWARE_ENDSTOP_Y, amax = max.y);
              break;
          #endif
          #if HAS_Z_AXIS
            case Z_AXIS:
              TERN_(MIN_SOFTWARE_ENDSTOP_Z, amin = min.z);
              TERN_(MAX_SOFTWARE_ENDSTOP_Z, amax = max.z);
              break;
          #endif
          #if HAS_I_AXIS
            case I_AXIS:
              TERN_(MIN_SOFTWARE_ENDSTOP_I, amin = min.i);
              TERN_(MIN_SOFTWARE_ENDSTOP_I, amax = max.i);
              break;
          #endif
          #if HAS_J_AXIS
            case J_AXIS:
              TERN_(MIN_SOFTWARE_ENDSTOP_J, amin = min.j);
              TERN_(MIN_SOFTWARE_ENDSTOP_J, amax = max.j);
              break;
          #endif
          #if HAS_K_AXIS
            case K_AXIS:
              TERN_(MIN_SOFTWARE_ENDSTOP_K, amin = min.k);
              TERN_(MIN_SOFTWARE_ENDSTOP_K, amax = max.k);
              break;
          #endif
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

#if ENABLED(AUTO_REPORT_POSITION)
  #include "../libs/autoreport.h"
  struct PositionReport { static void report() { report_current_position_projected(); } };
  extern AutoReporter<PositionReport> position_auto_reporter;
#endif

#if EITHER(FULL_REPORT_TO_HOST_FEATURE, REALTIME_REPORTING_COMMANDS)
  #define HAS_GRBL_STATE 1
  /**
   * Machine states for GRBL or TinyG
   */
  enum M_StateEnum : uint8_t {
    M_INIT = 0, //  0 machine is initializing
    M_RESET,    //  1 machine is ready for use
    M_ALARM,    //  2 machine is in alarm state (soft shut down)
    M_IDLE,     //  3 program stop or no more blocks (M0, M1, M60)
    M_END,      //  4 program end via M2, M30
    M_RUNNING,  //  5 motion is running
    M_HOLD,     //  6 motion is holding
    M_PROBE,    //  7 probe cycle active
    M_CYCLING,  //  8 machine is running (cycling)
    M_HOMING,   //  9 machine is homing
    M_JOGGING,  // 10 machine is jogging
    M_ERROR     // 11 machine is in hard alarm state (shut down)
  };
  extern M_StateEnum M_State_grbl;
  M_StateEnum grbl_state_for_marlin_state();
  void report_current_grblstate_moving();
  void report_current_position_moving();

  #if ENABLED(FULL_REPORT_TO_HOST_FEATURE)
    inline void set_and_report_grblstate(const M_StateEnum state, const bool force=true) {
      if (force || M_State_grbl != state) {
        M_State_grbl = state;
        report_current_grblstate_moving();
      }
    }
  #endif

  #if ENABLED(REALTIME_REPORTING_COMMANDS)
    void quickpause_stepper();
    void quickresume_stepper();
  #endif
#endif

void get_cartesian_from_steppers();
void set_current_from_steppers_for_axis(const AxisEnum axis);

void quickstop_stepper();

/**
 * Set the planner/stepper positions directly from current_position with
 * no kinematic translation. Used for homing axes and cartesian/core syncing.
 */
void sync_plan_position();

#if HAS_EXTRUDERS
  void sync_plan_position_e();
#endif

/**
 * Move the planner to the current position from wherever it last moved
 * (or from wherever it has been told it is located).
 */
void line_to_current_position(const_feedRate_t fr_mm_s=feedrate_mm_s);

#if HAS_EXTRUDERS
  void unscaled_e_move(const_float_t length, const_feedRate_t fr_mm_s);
#endif

void prepare_line_to_destination();

void _internal_move_to_destination(const_feedRate_t fr_mm_s=0.0f OPTARG(IS_KINEMATIC, const bool is_fast=false));

inline void prepare_internal_move_to_destination(const_feedRate_t fr_mm_s=0.0f) {
  _internal_move_to_destination(fr_mm_s);
}

#if IS_KINEMATIC
  void prepare_fast_move_to_destination(const_feedRate_t scaled_fr_mm_s=MMS_SCALED(feedrate_mm_s));

  inline void prepare_internal_fast_move_to_destination(const_feedRate_t fr_mm_s=0.0f) {
    _internal_move_to_destination(fr_mm_s, true);
  }
#endif

/**
 * Blocking movement and shorthand functions
 */
void do_blocking_move_to(NUM_AXIS_ARGS(const float), const_feedRate_t fr_mm_s=0.0f);
void do_blocking_move_to(const xy_pos_t &raw, const_feedRate_t fr_mm_s=0.0f);
void do_blocking_move_to(const xyz_pos_t &raw, const_feedRate_t fr_mm_s=0.0f);
void do_blocking_move_to(const xyze_pos_t &raw, const_feedRate_t fr_mm_s=0.0f);

void do_blocking_move_to_x(const_float_t rx, const_feedRate_t fr_mm_s=0.0f);
#if HAS_Y_AXIS
  void do_blocking_move_to_y(const_float_t ry, const_feedRate_t fr_mm_s=0.0f);
#endif
#if HAS_Z_AXIS
  void do_blocking_move_to_z(const_float_t rz, const_feedRate_t fr_mm_s=0.0f);
#endif
#if HAS_I_AXIS
  void do_blocking_move_to_i(const_float_t ri, const_feedRate_t fr_mm_s=0.0f);
  void do_blocking_move_to_xyz_i(const xyze_pos_t &raw, const_float_t i, const_feedRate_t fr_mm_s=0.0f);
#endif
#if HAS_J_AXIS
  void do_blocking_move_to_j(const_float_t rj, const_feedRate_t fr_mm_s=0.0f);
  void do_blocking_move_to_xyzi_j(const xyze_pos_t &raw, const_float_t j, const_feedRate_t fr_mm_s=0.0f);
#endif
#if HAS_K_AXIS
  void do_blocking_move_to_k(const_float_t rk, const_feedRate_t fr_mm_s=0.0f);
  void do_blocking_move_to_xyzij_k(const xyze_pos_t &raw, const_float_t k, const_feedRate_t fr_mm_s=0.0f);
#endif

#if HAS_Y_AXIS
  void do_blocking_move_to_xy(const_float_t rx, const_float_t ry, const_feedRate_t fr_mm_s=0.0f);
  void do_blocking_move_to_xy(const xy_pos_t &raw, const_feedRate_t fr_mm_s=0.0f);
  FORCE_INLINE void do_blocking_move_to_xy(const xyz_pos_t &raw, const_feedRate_t fr_mm_s=0.0f)  { do_blocking_move_to_xy(xy_pos_t(raw), fr_mm_s); }
  FORCE_INLINE void do_blocking_move_to_xy(const xyze_pos_t &raw, const_feedRate_t fr_mm_s=0.0f) { do_blocking_move_to_xy(xy_pos_t(raw), fr_mm_s); }
#endif

#if HAS_Z_AXIS
  void do_blocking_move_to_xy_z(const xy_pos_t &raw, const_float_t z, const_feedRate_t fr_mm_s=0.0f);
  FORCE_INLINE void do_blocking_move_to_xy_z(const xyz_pos_t &raw, const_float_t z, const_feedRate_t fr_mm_s=0.0f)  { do_blocking_move_to_xy_z(xy_pos_t(raw), z, fr_mm_s); }
  FORCE_INLINE void do_blocking_move_to_xy_z(const xyze_pos_t &raw, const_float_t z, const_feedRate_t fr_mm_s=0.0f) { do_blocking_move_to_xy_z(xy_pos_t(raw), z, fr_mm_s); }
#endif

void remember_feedrate_and_scaling();
void remember_feedrate_scaling_off();
void restore_feedrate_and_scaling();

#if HAS_Z_AXIS
  void do_z_clearance(const_float_t zclear, const bool lower_allowed=false);
#else
  inline void do_z_clearance(float, bool=false) {}
#endif

/**
 * Homing and Trusted Axes
 */
typedef IF<(NUM_AXES > 8), uint16_t, uint8_t>::type main_axes_bits_t;
constexpr main_axes_bits_t main_axes_mask = _BV(NUM_AXES) - 1;

typedef IF<(NUM_AXES + EXTRUDERS > 8), uint16_t, uint8_t>::type e_axis_bits_t;
constexpr e_axis_bits_t e_axis_mask = (_BV(EXTRUDERS) - 1) << NUM_AXES;

void set_axis_is_at_home(const AxisEnum axis);

#if HAS_ENDSTOPS
  /**
   * axes_homed
   *   Flags that each linear axis was homed.
   *   XYZ on cartesian, ABC on delta, ABZ on SCARA.
   *
   * axes_trusted
   *   Flags that the position is trusted in each linear axis. Set when homed.
   *   Cleared whenever a stepper powers off, potentially losing its position.
   */
  extern main_axes_bits_t axes_homed, axes_trusted;
  void homeaxis(const AxisEnum axis);
  void set_axis_never_homed(const AxisEnum axis);
  main_axes_bits_t axes_should_home(main_axes_bits_t axes_mask=main_axes_mask);
  bool homing_needed_error(main_axes_bits_t axes_mask=main_axes_mask);
  inline void set_axis_unhomed(const AxisEnum axis)   { CBI(axes_homed, axis); }
  inline void set_axis_untrusted(const AxisEnum axis) { CBI(axes_trusted, axis); }
  inline void set_all_unhomed()                       { axes_homed = axes_trusted = 0; }
  inline void set_axis_homed(const AxisEnum axis)     { SBI(axes_homed, axis); }
  inline void set_axis_trusted(const AxisEnum axis)   { SBI(axes_trusted, axis); }
  inline void set_all_homed()                         { axes_homed = axes_trusted = main_axes_mask; }
#else
  constexpr main_axes_bits_t axes_homed = main_axes_mask, axes_trusted = main_axes_mask; // Zero-endstop machines are always homed and trusted
  inline void homeaxis(const AxisEnum axis)           {}
  inline void set_axis_never_homed(const AxisEnum)    {}
  inline main_axes_bits_t axes_should_home(main_axes_bits_t=main_axes_mask) { return 0; }
  inline bool homing_needed_error(main_axes_bits_t=main_axes_mask) { return false; }
  inline void set_axis_unhomed(const AxisEnum axis)   {}
  inline void set_axis_untrusted(const AxisEnum axis) {}
  inline void set_all_unhomed()                       {}
  inline void set_axis_homed(const AxisEnum axis)     {}
  inline void set_axis_trusted(const AxisEnum axis)   {}
  inline void set_all_homed()                         {}
#endif

inline bool axis_was_homed(const AxisEnum axis)       { return TEST(axes_homed, axis); }
inline bool axis_is_trusted(const AxisEnum axis)      { return TEST(axes_trusted, axis); }
inline bool axis_should_home(const AxisEnum axis)     { return (axes_should_home() & _BV(axis)) != 0; }
inline bool no_axes_homed()                           { return !axes_homed; }
inline bool all_axes_homed()                          { return main_axes_mask == (axes_homed & main_axes_mask); }
inline bool homing_needed()                           { return !all_axes_homed(); }
inline bool all_axes_trusted()                        { return main_axes_mask == (axes_trusted & main_axes_mask); }

void home_if_needed(const bool keeplev=false);

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
#define RAW_X_POSITION(POS)     LOGICAL_TO_NATIVE(POS, X_AXIS)
#if HAS_Y_AXIS
  #define LOGICAL_Y_POSITION(POS) NATIVE_TO_LOGICAL(POS, Y_AXIS)
  #define RAW_Y_POSITION(POS)     LOGICAL_TO_NATIVE(POS, Y_AXIS)
#endif
#if HAS_Z_AXIS
  #define LOGICAL_Z_POSITION(POS) NATIVE_TO_LOGICAL(POS, Z_AXIS)
  #define RAW_Z_POSITION(POS)     LOGICAL_TO_NATIVE(POS, Z_AXIS)
#endif
#if HAS_I_AXIS
  #define LOGICAL_I_POSITION(POS) NATIVE_TO_LOGICAL(POS, I_AXIS)
  #define RAW_I_POSITION(POS)     LOGICAL_TO_NATIVE(POS, I_AXIS)
#endif
#if HAS_J_AXIS
  #define LOGICAL_J_POSITION(POS) NATIVE_TO_LOGICAL(POS, J_AXIS)
  #define RAW_J_POSITION(POS)     LOGICAL_TO_NATIVE(POS, J_AXIS)
#endif
#if HAS_K_AXIS
  #define LOGICAL_K_POSITION(POS) NATIVE_TO_LOGICAL(POS, K_AXIS)
  #define RAW_K_POSITION(POS)     LOGICAL_TO_NATIVE(POS, K_AXIS)
#endif

/**
 * position_is_reachable family of functions
 */
#if IS_KINEMATIC // (DELTA or SCARA)

  #if HAS_SCARA_OFFSET
    extern abc_pos_t scara_home_offset; // A and B angular offsets, Z mm offset
  #endif

  // Return true if the given point is within the printable area
  bool position_is_reachable(const_float_t rx, const_float_t ry, const float inset=0);

  inline bool position_is_reachable(const xy_pos_t &pos, const float inset=0) {
    return position_is_reachable(pos.x, pos.y, inset);
  }

#else

  // Return true if the given position is within the machine bounds.
  bool position_is_reachable(const_float_t rx, const_float_t ry);
  inline bool position_is_reachable(const xy_pos_t &pos) {
    return position_is_reachable(pos.x, pos.y);
  }

#endif

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
  extern float inactive_extruder_x,                 // Used in mode 0 & 1
               duplicate_extruder_x_offset;         // Used in mode 2 & 3
  extern xyz_pos_t raised_parked_position;          // Used in mode 1
  extern bool active_extruder_parked;               // Used in mode 1, 2 & 3
  extern millis_t delayed_move_time;                // Used in mode 1
  extern celsius_t duplicate_extruder_temp_offset;  // Used in mode 2 & 3
  extern bool idex_mirrored_mode;                   // Used in mode 3

  FORCE_INLINE bool idex_is_duplicating() { return dual_x_carriage_mode >= DXC_DUPLICATION_MODE; }

  float x_home_pos(const uint8_t extruder);

  #define TOOL_X_HOME_DIR(T) ((T) ? X2_HOME_DIR : X_HOME_DIR)

  void set_duplication_enabled(const bool dupe, const int8_t tool_index=-1);
  void idex_set_mirrored_mode(const bool mirr);
  void idex_set_parked(const bool park=true);

#else

  #if ENABLED(MULTI_NOZZLE_DUPLICATION)
    extern uint8_t duplication_e_mask;
    enum DualXMode : char { DXC_DUPLICATION_MODE = 2 };
    FORCE_INLINE void set_duplication_enabled(const bool dupe) { extruder_duplication_enabled = dupe; }
  #endif

  #define TOOL_X_HOME_DIR(T) X_HOME_DIR

#endif

#if HAS_M206_COMMAND
  void set_home_offset(const AxisEnum axis, const float v);
#endif

#if USE_SENSORLESS
  struct sensorless_t;
  sensorless_t start_sensorless_homing_per_axis(const AxisEnum axis);
  void end_sensorless_homing_per_axis(const AxisEnum axis, sensorless_t enable_stealth);
#endif
