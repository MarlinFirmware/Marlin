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
#elif ENABLED(POLAR)
  #include "polar.h"
#endif

#if ENABLED(AUTO_REPORT_POSITION)
  #include "../libs/autoreport.h"
#endif

// Error margin to work around float imprecision
constexpr float fslop = 0.0001;

#if ENABLED(REALTIME_REPORTING_COMMANDS)
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
#endif

#if USE_SENSORLESS
  struct sensorless_t;
#endif

/**
 * Homing and Trusted Axes
 */
typedef bits_t(NUM_AXES) main_axes_bits_t;
constexpr main_axes_bits_t main_axes_mask = _BV(NUM_AXES) - 1;

class Motion {
public:
  static bool relative_mode;            // Relative Mode - G90/G91

  // Flags for rotational axes
  static constexpr AxisFlags rotational{0 LOGICAL_AXIS_GANG(
      | 0, | 0, | 0, | 0,
      | (ENABLED(AXIS4_ROTATES)<<I_AXIS), | (ENABLED(AXIS5_ROTATES)<<J_AXIS), | (ENABLED(AXIS6_ROTATES)<<K_AXIS),
      | (ENABLED(AXIS7_ROTATES)<<U_AXIS), | (ENABLED(AXIS8_ROTATES)<<V_AXIS), | (ENABLED(AXIS9_ROTATES)<<W_AXIS))
  };

  #if HAS_MULTI_EXTRUDER
    static uint8_t extruder;            // Selected extruder (tool) - T<extruder>
  #else
    static constexpr uint8_t extruder = 0;
  #endif

  static xyze_pos_t position,           // High-level current tool position
                    destination;        // Destination for a move

  static feedRate_t feedrate_mm_s;      // Feedrate for G-moves, set by the most recent G-move

  // Feedrate scaling is applied to all G0/G1, G2/G3, and G5 moves
  static int16_t feedrate_percentage;
  static feedRate_t mms_scaled(const feedRate_t f=feedrate_mm_s) {
    return f * 0.01f * feedrate_percentage;
  }

  #if IS_KINEMATIC
    static abce_pos_t delta;            // Scratch space for a kinematic result
  #endif
  #if HAS_SCARA_OFFSET
    static abc_pos_t scara_home_offset; // A and B angular offsets, Z mm offset
  #endif

  #if HAS_HOTEND_OFFSET
    static xyz_pos_t hotend_offset[HOTENDS];
    static void reset_hotend_offsets();
  #elif HOTENDS
    static constexpr xyz_pos_t hotend_offset[HOTENDS] = { { TERN_(HAS_X_AXIS, 0) } };
  #else
    static constexpr xyz_pos_t hotend_offset[1] = { { TERN_(HAS_X_AXIS, 0) } };
  #endif

  #if HAS_HOTEND_OFFSET
    static xyz_pos_t& active_hotend_offset() { return hotend_offset[extruder]; }
  #else
    static const xyz_pos_t& active_hotend_offset() { return hotend_offset[extruder]; }
  #endif

  #if ENABLED(LCD_SHOW_E_TOTAL)
    static float e_move_accumulator;
  #endif

  static xyz_pos_t cartes;              // Scratch space for a cartesian result
  static void get_cartesian_from_steppers();
  static void set_current_from_steppers_for_axis(const AxisEnum axis);

  static void report_position();
  static void report_position_real();
  static void report_position_projected();

  #if ENABLED(AUTO_REPORT_POSITION)
    struct PositionReport {
      static void report() {
        TERN(AUTO_REPORT_REAL_POSITION, report_position_real(), report_position_projected());
      }
    };
    static AutoReporter<PositionReport> position_auto_reporter;
  #endif

  /**
   * Set the planner/stepper positions directly from motion.position with
   * no kinematic translation. Used for homing axes and cartesian/core syncing.
   */
  static void sync_plan_position();
  #if HAS_EXTRUDERS
    static void sync_plan_position_e();
    static void set_and_sync_e(const float epos) {
      position.e = epos;
      sync_plan_position_e();
    }
  #endif

  //
  // Homing and Trusted Axes
  //

  /**
   * Feed rates are often configured with mm/m
   * but the planner and stepper like mm/s units.
   */
  #if ENABLED(EDITABLE_HOMING_FEEDRATE)
    static xyz_feedrate_t homing_feedrate_mm_m;
  #else
    static constexpr xyz_feedrate_t homing_feedrate_mm_m = HOMING_FEEDRATE_MM_M;
  #endif

  FORCE_INLINE static feedRate_t homing_feedrate(const AxisEnum a) {
    float v = TERN0(HAS_Z_AXIS, homing_feedrate_mm_m.z);
    #if DISABLED(DELTA)
      NUM_AXIS_CODE(
             if (a == X_AXIS) v = homing_feedrate_mm_m.x,
        else if (a == Y_AXIS) v = homing_feedrate_mm_m.y,
        else if (a == Z_AXIS) v = homing_feedrate_mm_m.z,
        else if (a == I_AXIS) v = homing_feedrate_mm_m.i,
        else if (a == J_AXIS) v = homing_feedrate_mm_m.j,
        else if (a == K_AXIS) v = homing_feedrate_mm_m.k,
        else if (a == U_AXIS) v = homing_feedrate_mm_m.u,
        else if (a == V_AXIS) v = homing_feedrate_mm_m.v,
        else if (a == W_AXIS) v = homing_feedrate_mm_m.w
      );
    #endif
    return MMM_TO_MMS(v);
  }

  static feedRate_t get_homing_bump_feedrate(const AxisEnum axis);

  #if HAS_HOME_OFFSET
    static xyz_pos_t home_offset;
    static void set_home_offset(const AxisEnum axis, const float v) { home_offset[axis] = v; }
  #endif

  #if HAS_DUPLICATION_MODE
    static bool extruder_duplication;   // Used in Dual X mode 2
    static void _set_duplication_enabled(const bool dupe) { extruder_duplication = dupe; }
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    static DualXMode idex_mode;
    static bool idex_is_duplicating() { return idex_mode >= DXC_DUPLICATION_MODE; }

    static float inactive_extruder_x,                 // Used in mode 0 & 1
                 duplicate_extruder_x_offset;         // Used in mode 2 & 3

    static bool active_extruder_parked;               // Used in mode 1, 2 & 3
    static millis_t delayed_move_time;                // Used in mode 1
    static celsius_t duplicate_extruder_temp_offset;  // Used in mode 2 & 3

    static bool idex_mirrored_mode;                   // Used in mode 3
    static void idex_set_mirrored_mode(const bool mirr);

    static float x_home_pos(const uint8_t tool) {
      if (tool == 0) return X_HOME_POS;

      /**
       * In dual carriage mode the extruder offset provides an override of the
       * second X-carriage position when homed - otherwise X2_HOME_POS is used.
       * This allows soft recalibration of the second extruder home position
       * (with M218 T1 Xn) without firmware reflash.
       */
      return hotend_offset[1].x > 0 ? hotend_offset[1].x : X2_HOME_POS;
    }
    static void idex_set_parked(const bool park=true);
    static bool unpark_before_move();
    static void set_extruder_duplication(const bool dupe, const int8_t tool_index=-1);
    static void idex_home_x();
  #else
    static bool unpark_before_move() { return false; }
    #if ENABLED(MULTI_NOZZLE_DUPLICATION)
      static uint8_t duplication_e_mask;
      static void set_extruder_duplication(const bool dupe) { _set_duplication_enabled(dupe); }
    #endif
  #endif

  //
  // Probing
  //
  #if HAS_VARIABLE_XY_PROBE_FEEDRATE
    static feedRate_t xy_probe_feedrate_mm_s;   // Set with 'G29 S' for ABL LINEAR/BILINEAR. TODO: Store to EEPROM.
  #endif
  #if ENABLED(DWIN_LCD_PROUI)
    static uint16_t z_probe_slow_mm_s;
  #elif defined(Z_PROBE_FEEDRATE_SLOW)
    static constexpr feedRate_t z_probe_slow_mm_s = MMM_TO_MMS(Z_PROBE_FEEDRATE_SLOW);
  #endif
  #ifdef Z_PROBE_FEEDRATE_FAST
    static constexpr feedRate_t z_probe_fast_mm_s = MMM_TO_MMS(Z_PROBE_FEEDRATE_FAST);
  #endif

  #ifdef __IMXRT1062__
    #define DEFS_PROGMEM
  #else
    #define DEFS_PROGMEM PROGMEM
  #endif

  static float pgm_read_any(const float *p)   { return TERN(__IMXRT1062__, *p, pgm_read_float(p)); }
  static int8_t pgm_read_any(const int8_t *p) { return TERN(__IMXRT1062__, *p, pgm_read_byte(p)); }

  #define XYZ_DEFS(T, NAME, OPT) \
    static T NAME(const AxisEnum axis) { \
      static constexpr XYZval<T> NAME##_P DEFS_PROGMEM = NUM_AXIS_ARRAY(X_##OPT, Y_##OPT, Z_##OPT, I_##OPT, J_##OPT, K_##OPT, U_##OPT, V_##OPT, W_##OPT); \
      return pgm_read_any(&NAME##_P[axis]); \
    }
  XYZ_DEFS(float,  base_min_pos,  MIN_POS);     // base_min_pos(axis)
  XYZ_DEFS(float,  base_max_pos,  MAX_POS);     // base_max_pos(axis)
  XYZ_DEFS(float,  base_home_pos, HOME_POS);    // base_home_pos(axis)
  XYZ_DEFS(float,  max_axis_length, MAX_LENGTH); // max_axis_length(axis)
  XYZ_DEFS(int8_t, home_dir,      HOME_DIR);    // home_dir(axis)

  static float home_bump_mm(const AxisEnum axis) {
    static const xyz_pos_t home_bump_mm_P DEFS_PROGMEM = HOMING_BUMP_MM;
    return pgm_read_any(&home_bump_mm_P[axis]);
  }

  #if HAS_X_AXIS
    static int8_t tool_x_home_dir(const uint8_t tool=extruder) {
      UNUSED(tool);
      return TERN(DUAL_X_CARRIAGE, tool ? 1 : -1, X_HOME_DIR);
    }
  #endif

  //
  // Workspace offsets
  //

  #if HAS_WORKSPACE_OFFSET
    static xyz_pos_t workspace_offset;
    static float native_to_logical(const float f, const AxisEnum a) { return f + workspace_offset[a]; }
    static float logical_to_native(const float f, const AxisEnum a) { return f - workspace_offset[a]; }
    FORCE_INLINE static void toLogical(xy_pos_t &raw)   { raw += workspace_offset; }
    FORCE_INLINE static void toLogical(xyz_pos_t &raw)  { raw += workspace_offset; }
    FORCE_INLINE static void toLogical(xyze_pos_t &raw) { raw += workspace_offset; }
    FORCE_INLINE static void toNative(xy_pos_t &raw)    { raw -= workspace_offset; }
    FORCE_INLINE static void toNative(xyz_pos_t &raw)   { raw -= workspace_offset; }
    FORCE_INLINE static void toNative(xyze_pos_t &raw)  { raw -= workspace_offset; }
  #else
    static float native_to_logical(const float f, const AxisEnum) { return f; }
    static float logical_to_native(const float f, const AxisEnum) { return f; }
    FORCE_INLINE static void toLogical(xy_pos_t&)   {}
    FORCE_INLINE static void toLogical(xyz_pos_t&)  {}
    FORCE_INLINE static void toLogical(xyze_pos_t&) {}
    FORCE_INLINE static void toNative(xy_pos_t&)    {}
    FORCE_INLINE static void toNative(xyz_pos_t&)   {}
    FORCE_INLINE static void toNative(xyze_pos_t&)  {}
  #endif
  #if HAS_X_AXIS
    static float logical_x(const float f) { return native_to_logical(f, X_AXIS); }
    static float raw_x(const float f)     { return logical_to_native(f, X_AXIS); }
  #endif
  #if HAS_Y_AXIS
    static float logical_y(const float f) { return native_to_logical(f, Y_AXIS); }
    static float raw_y(const float f)     { return logical_to_native(f, Y_AXIS); }
  #endif
  #if HAS_Z_AXIS
    static float logical_z(const float f) { return native_to_logical(f, Z_AXIS); }
    static float raw_z(const float f)     { return logical_to_native(f, Z_AXIS); }
  #endif
  #if HAS_I_AXIS
    static float logical_i(const float f) { return native_to_logical(f, I_AXIS); }
    static float raw_i(const float f)     { return logical_to_native(f, I_AXIS); }
  #endif
  #if HAS_J_AXIS
    static float logical_j(const float f) { return native_to_logical(f, J_AXIS); }
    static float raw_j(const float f)     { return logical_to_native(f, J_AXIS); }
  #endif
  #if HAS_K_AXIS
    static float logical_k(const float f) { return native_to_logical(f, K_AXIS); }
    static float raw_k(const float f)     { return logical_to_native(f, K_AXIS); }
  #endif
  #if HAS_U_AXIS
    static float logical_u(const float f) { return native_to_logical(f, U_AXIS); }
    static float raw_u(const float f)     { return logical_to_native(f, U_AXIS); }
  #endif
  #if HAS_V_AXIS
    static float logical_v(const float f) { return native_to_logical(f, V_AXIS); }
    static float raw_v(const float f)     { return logical_to_native(f, V_AXIS); }
  #endif
  #if HAS_W_AXIS
    static float logical_w(const float f) { return native_to_logical(f, W_AXIS); }
    static float raw_w(const float f)     { return logical_to_native(f, W_AXIS); }
  #endif

  #if HAS_Z_AXIS
    static bool z_min_trusted; // If Z has been powered on trust that the real Z is >= motion.position.z
  #endif
  static void set_axis_is_at_home(const AxisEnum axis);

  /**
   * axes_homed
   *   Flags that each linear axis was homed.
   *   XYZ on cartesian, ABC on delta, ABZ on SCARA.
   *
   * axes_trusted
   *   Flags that the position is trusted in each linear axis. Set when homed.
   *   Cleared whenever a stepper powers off, potentially losing its position.
   */
  #if HAS_ENDSTOPS
    #ifdef TMC_HOME_PHASE
      static void backout_to_tmc_homing_phase(const AxisEnum axis);
    #endif
    static main_axes_bits_t axes_homed, axes_trusted;
    static void homeaxis(const AxisEnum axis);
    static void set_axis_never_homed(const AxisEnum axis);
    static main_axes_bits_t axes_should_home(main_axes_bits_t axes_mask=main_axes_mask);
    static bool homing_needed_error(main_axes_bits_t axes_mask=main_axes_mask);
  #else
    static constexpr main_axes_bits_t axes_homed = main_axes_mask, axes_trusted = main_axes_mask; // Zero-endstop machines are always homed and trusted
    static void homeaxis(const AxisEnum axis) {}
    static void set_axis_never_homed(const AxisEnum)    {}
    static main_axes_bits_t axes_should_home(main_axes_bits_t=main_axes_mask) { return 0; }
    static bool homing_needed_error(main_axes_bits_t=main_axes_mask) { return false; }
  #endif

  static void set_axis_unhomed(const AxisEnum axis)   { TERN_(HAS_ENDSTOPS, CBI(axes_homed, axis)); }
  static void set_axis_untrusted(const AxisEnum axis) { TERN_(HAS_ENDSTOPS, CBI(axes_trusted, axis)); }
  static void set_all_unhomed()                       { TERN_(HAS_ENDSTOPS, axes_homed = axes_trusted = 0); }
  static void set_axis_homed(const AxisEnum axis)     { TERN_(HAS_ENDSTOPS, SBI(axes_homed, axis)); }
  static void set_axis_trusted(const AxisEnum axis)   { TERN_(HAS_ENDSTOPS, SBI(axes_trusted, axis)); }
  static void set_all_homed()                         { TERN_(HAS_ENDSTOPS, axes_homed = axes_trusted = main_axes_mask); }

  static bool axis_was_homed(const AxisEnum axis)     { return TEST(axes_homed, axis); }
  static bool axis_is_trusted(const AxisEnum axis)    { return TEST(axes_trusted, axis); }
  static bool axis_should_home(const AxisEnum axis)   { return axes_should_home(_BV(axis)) != 0; }
  static bool no_axes_homed()                         { return !axes_homed; }
  static bool all_axes_homed()                        { return main_axes_mask == (axes_homed & main_axes_mask); }
  static bool homing_needed()                         { return !all_axes_homed(); }
  static bool all_axes_trusted()                      { return main_axes_mask == (axes_trusted & main_axes_mask); }

  static void home_if_needed(const bool keeplev=false);

  static bool gcode_motion_ignored();

  //
  // Software Endstops
  //

  #if HAS_SOFTWARE_ENDSTOPS

    typedef struct {
      bool _enabled, _loose;
      bool enabled() { return _enabled && !_loose; }

      xyz_pos_t min, max;
      void get_manual_axis_limits(const AxisEnum axis, float &amin, float &amax) {
        amin = -100000; amax = 100000; // "No limits"
        #if HAS_SOFTWARE_ENDSTOPS
          if (enabled()) switch (axis) {
            #if HAS_X_AXIS
              case X_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_X, amin = min.x); TERN_(MAX_SOFTWARE_ENDSTOP_X, amax = max.x); break;
            #endif
            #if HAS_Y_AXIS
              case Y_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_Y, amin = min.y); TERN_(MAX_SOFTWARE_ENDSTOP_Y, amax = max.y); break;
            #endif
            #if HAS_Z_AXIS
              case Z_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_Z, amin = min.z); TERN_(MAX_SOFTWARE_ENDSTOP_Z, amax = max.z); break;
            #endif
            #if HAS_I_AXIS
              case I_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_I, amin = min.i); TERN_(MIN_SOFTWARE_ENDSTOP_I, amax = max.i); break;
            #endif
            #if HAS_J_AXIS
              case J_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_J, amin = min.j); TERN_(MIN_SOFTWARE_ENDSTOP_J, amax = max.j); break;
            #endif
            #if HAS_K_AXIS
              case K_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_K, amin = min.k); TERN_(MIN_SOFTWARE_ENDSTOP_K, amax = max.k); break;
            #endif
            #if HAS_U_AXIS
              case U_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_U, amin = min.u); TERN_(MIN_SOFTWARE_ENDSTOP_U, amax = max.u); break;
            #endif
            #if HAS_V_AXIS
              case V_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_V, amin = min.v); TERN_(MIN_SOFTWARE_ENDSTOP_V, amax = max.v); break;
            #endif
            #if HAS_W_AXIS
              case W_AXIS: TERN_(MIN_SOFTWARE_ENDSTOP_W, amin = min.w); TERN_(MIN_SOFTWARE_ENDSTOP_W, amax = max.w); break;
            #endif
            default: break;
          }
        #endif
      }
    } soft_endstops_t;

  #else // !HAS_SOFTWARE_ENDSTOPS

    typedef struct {
      bool enabled() { return false; }
      void get_manual_axis_limits(const AxisEnum axis, float &amin, float &amax) {
        // No limits
        amin = position[axis] - 1000;
        amax = position[axis] + 1000;
      }
    } soft_endstops_t;

  #endif // !HAS_SOFTWARE_ENDSTOPS

  static soft_endstops_t soft_endstop;

  #if HAS_SOFTWARE_ENDSTOPS
    static void apply_limits(xyz_pos_t &target);
    static void update_software_endstops(const AxisEnum axis
      OPTARG(HAS_HOTEND_OFFSET, const uint8_t old_tool_index=0, const uint8_t new_tool_index=0)
    );
    static void set_soft_endstop_loose(const bool loose) { soft_endstop._loose = loose; }
  #else
    static void apply_limits(xyz_pos_t&) {}
    static void update_software_endstops(...) {}
    static void set_soft_endstop_loose(...) {}
  #endif

  //
  // Reachability Tests
  //
  #if IS_KINEMATIC

    // Return true if the given point is within the printable area
    static bool can_reach(const float rx, const float ry, const float inset=0);

    static bool can_reach(const xy_pos_t &pos, const float inset=0) {
      return can_reach(pos.x, pos.y, inset);
    }

  #else

    // Return true if the given position is within the machine bounds.
    static bool can_reach(XY_LIST(const float rx, const float ry));
    static bool can_reach(const xy_pos_t &pos) {
      return can_reach(XY_LIST(pos.x, pos.y));
    }

  #endif

  // Hard Stop with potential step loss, used in rare situations
  static void quickstop_stepper();

  // Get the linear distance over multiple axes
  static float get_move_distance(const xyze_pos_t &diff OPTARG(HAS_ROTATIONAL_AXES, bool &is_cartesian_move));

  // Internal and procedural moves should run without feedrate scaling
  static void remember_feedrate_scaling_off();
  static void restore_feedrate_and_scaling();

  /**
   * Move the planner to the current position from wherever it last moved
   * (or from wherever it has been told it is located).
   */
  static void goto_current_position(const feedRate_t fr_mm_s=feedrate_mm_s);

  #if HAS_EXTRUDERS
    static void unscaled_e_move(const float length, const feedRate_t fr_mm_s);
  #endif

  static void prepare_line_to_destination();

  static void prepare_internal_move_to_destination(const feedRate_t fr_mm_s=0.0f) {
    _goto_destination_internal(fr_mm_s);
  }

  #if IS_KINEMATIC
    static void prepare_fast_move_to_destination(const feedRate_t scaled_fr_mm_s=mms_scaled());

    static void prepare_internal_fast_move_to_destination(const feedRate_t fr_mm_s=0.0f) {
      _goto_destination_internal(fr_mm_s, true);
    }
  #endif

  //
  // Blocking Move for internal procedures
  //

  static void blocking_move(NUM_AXIS_ARGS_(const float) const feedRate_t fr_mm_s=0.0f);
  static void blocking_move(const xy_pos_t &raw, const feedRate_t fr_mm_s=0.0f);
  static void blocking_move(const xyz_pos_t &raw, const feedRate_t fr_mm_s=0.0f)  { blocking_move(NUM_AXIS_ELEM_(raw) fr_mm_s); }
  static void blocking_move(const xyze_pos_t &raw, const feedRate_t fr_mm_s=0.0f) { blocking_move(NUM_AXIS_ELEM_(raw) fr_mm_s); }

  #if HAS_X_AXIS
    static void blocking_move_x(const float rx, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_Y_AXIS
    static void blocking_move_y(const float ry, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_Z_AXIS
    static void blocking_move_z(const float rz, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_I_AXIS
    static void blocking_move_i(const float ri, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xyz_i(const xyze_pos_t &raw, const float i, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_J_AXIS
    static void blocking_move_j(const float rj, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xyzi_j(const xyze_pos_t &raw, const float j, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_K_AXIS
    static void blocking_move_k(const float rk, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xyzij_k(const xyze_pos_t &raw, const float k, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_U_AXIS
    static void blocking_move_u(const float ru, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xyzijk_u(const xyze_pos_t &raw, const float u, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_V_AXIS
    static void blocking_move_v(const float rv, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xyzijku_v(const xyze_pos_t &raw, const float v, const feedRate_t fr_mm_s=0.0f);
  #endif
  #if HAS_W_AXIS
    static void blocking_move_w(const float rw, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xyzijkuv_w(const xyze_pos_t &raw, const float w, const feedRate_t fr_mm_s=0.0f);
  #endif

  #if HAS_Y_AXIS
    static void blocking_move_xy(const float rx, const float ry, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xy(const xy_pos_t &raw, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xy(const xyz_pos_t &raw, const feedRate_t fr_mm_s=0.0f)  { blocking_move_xy(xy_pos_t(raw), fr_mm_s); }
    static void blocking_move_xy(const xyze_pos_t &raw, const feedRate_t fr_mm_s=0.0f) { blocking_move_xy(xy_pos_t(raw), fr_mm_s); }
  #endif

  #if HAS_Z_AXIS
    static void blocking_move_xy_z(const xy_pos_t &raw, const float z, const feedRate_t fr_mm_s=0.0f);
    static void blocking_move_xy_z(const xyz_pos_t &raw, const float z, const feedRate_t fr_mm_s=0.0f)  { blocking_move_xy_z(xy_pos_t(raw), z, fr_mm_s); }
    static void blocking_move_xy_z(const xyze_pos_t &raw, const float z, const feedRate_t fr_mm_s=0.0f) { blocking_move_xy_z(xy_pos_t(raw), z, fr_mm_s); }
  #endif

  static void do_homing_move(const AxisEnum axis, const float distance, const feedRate_t fr_mm_s=0.0, const bool final_approach=true);

  #if HAS_Z_AXIS
    static void do_z_clearance(const float zclear, const bool with_probe=true, const bool lower_allowed=false);
    static void do_z_clearance_by(const float zclear);
    static void do_z_post_clearance();
    static void do_move_after_z_homing();
  #else
    static void do_z_clearance(float, bool=true, bool=false) {}
    static void do_z_clearance_by(float) {}
  #endif

  //
  // Realtime Reporting
  //

  #if ENABLED(REALTIME_REPORTING_COMMANDS)
    static M_StateEnum M_State_grbl;
    M_StateEnum grbl_state_for_marlin_state();
    static void report_current_grblstate_moving();
    static void report_position_moving();

    #if ENABLED(FULL_REPORT_TO_HOST_FEATURE)
      static void set_and_report_grblstate(const M_StateEnum state, const bool force=true) {
        if (force || M_State_grbl != state) {
          M_State_grbl = state;
          report_current_grblstate_moving();
        }
      }
    #endif

    static void quickpause_stepper();
    static void quickresume_stepper();
  #endif // REALTIME_REPORTING_COMMANDS

  //
  // Trinamic Stepper Drivers
  //
  #if USE_SENSORLESS
    static sensorless_t sensorless_axis_homing_start(const AxisEnum axis);
    static void sensorless_axis_homing_end(const AxisEnum axis, sensorless_t enable_stealth);
  #endif

  #if HAS_HOMING_CURRENT
    static void set_homing_current(const AxisEnum axis);
    static void restore_homing_current(const AxisEnum axis);
  #endif

private:
  #if ENABLED(DUAL_X_CARRIAGE)
    static xyz_pos_t raised_parked_position;    // Used in mode 1
  #endif
  #if SECONDARY_AXES
    static void secondary_axis_moves(SECONDARY_AXIS_ARGS_LC(const float), const feedRate_t fr_mm_s);
  #endif
  #if IS_KINEMATIC
    static bool goto_destination_kinematic();
  #else
    static bool goto_destination_cartesian();
    #if ENABLED(SEGMENT_LEVELED_MOVES) && DISABLED(AUTO_BED_LEVELING_UBL)
      static void goto_destination_segmented(const feedRate_t fr_mm_s, const float segment_size=LEVELED_SEGMENT_LENGTH);
    #endif
  #endif
  static void _goto_destination_internal(const feedRate_t fr_mm_s=0.0f OPTARG(IS_KINEMATIC, const bool is_fast=false));

}; // class Motion

// Specify read-only XY_PROBE_FEEDRATE_MM_S, feed rate between Probe Points.
#if HAS_VARIABLE_XY_PROBE_FEEDRATE
  // ABL LINEAR and BILINEAR use 'G29 S' value, or MMM_TO_MMS(XY_PROBE_FEEDRATE)
  #define XY_PROBE_FEEDRATE_MM_S motion.xy_probe_feedrate_mm_s
#elif defined(XY_PROBE_FEEDRATE)
  // Probe feedrate can be hard-coded by configuration
  #define XY_PROBE_FEEDRATE_MM_S MMM_TO_MMS(XY_PROBE_FEEDRATE)
#else
  // Defer to Planner XY max feedrate, or 60 mm/s
  #define XY_PROBE_FEEDRATE_MM_S PLANNER_XY_FEEDRATE_MM_S
#endif

#define BABYSTEP_ALLOWED() ((ENABLED(BABYSTEP_WITHOUT_HOMING) || motion.all_axes_trusted()) && (ENABLED(BABYSTEP_ALWAYS_AVAILABLE) || marlin.printer_busy()))

extern Motion motion;

#if HAS_PROUI_MESH_EDIT
  #define MESH_X_DIST ((mesh_max.x - mesh_min.x) / (GRID_MAX_CELLS_X))
  #define MESH_Y_DIST ((mesh_max.y - mesh_min.y) / (GRID_MAX_CELLS_Y))
  extern xy_pos_t mesh_min, mesh_max;
#elif HAS_MESH
  #define MESH_X_DIST (float((MESH_MAX_X) - (MESH_MIN_X)) / (GRID_MAX_CELLS_X))
  #define MESH_Y_DIST (float((MESH_MAX_Y) - (MESH_MIN_Y)) / (GRID_MAX_CELLS_Y))
  constexpr xy_pos_t mesh_min{ MESH_MIN_X, MESH_MIN_Y },
                     mesh_max{ MESH_MAX_X, MESH_MAX_Y };
#endif

// External conversion methods (motion.h)
inline void toLogical(xy_pos_t &raw)   { motion.toLogical(raw); }
inline void toLogical(xyz_pos_t &raw)  { motion.toLogical(raw); }
inline void toLogical(xyze_pos_t &raw) { motion.toLogical(raw); }
inline void toNative(xy_pos_t &lpos)   { motion.toNative(lpos); }
inline void toNative(xyz_pos_t &lpos)  { motion.toNative(lpos); }
inline void toNative(xyze_pos_t &lpos) { motion.toNative(lpos); }
