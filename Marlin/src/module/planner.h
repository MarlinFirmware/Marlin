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
 * planner.h
 *
 * Buffer movement commands and manage the acceleration profile plan
 *
 * Derived from Grbl
 * Copyright (c) 2009-2011 Simen Svale Skogsrud
 */

#include "../MarlinCore.h"

#if ENABLED(JD_HANDLE_SMALL_SEGMENTS)
  // Enable this option for perfect accuracy but maximum
  // computation. Should be fine on ARM processors.
  //#define JD_USE_MATH_ACOS

  // Disable this option to save 120 bytes of PROGMEM,
  // but incur increased computation and a reduction
  // in accuracy.
  #define JD_USE_LOOKUP_TABLE
#endif

#include "motion.h"
#include "../gcode/queue.h"

#if ENABLED(DELTA)
  #include "delta.h"
#elif ENABLED(POLARGRAPH)
  #include "polargraph.h"
#endif

#if ABL_PLANAR
  #include "../libs/vector_3.h" // for matrix_3x3
#endif

#if ENABLED(FWRETRACT)
  #include "../feature/fwretract.h"
#endif

#if ENABLED(MIXING_EXTRUDER)
  #include "../feature/mixing.h"
#endif

#if HAS_CUTTER
  #include "../feature/spindle_laser_types.h"
#endif

#if ENABLED(DIRECT_STEPPING)
  #include "../feature/direct_stepping.h"
#endif

#if ENABLED(EXTERNAL_CLOSED_LOOP_CONTROLLER)
  #include "../feature/closedloop.h"
#endif

// Feedrate for manual moves
#ifdef MANUAL_FEEDRATE
  constexpr xyze_feedrate_t _mf = MANUAL_FEEDRATE,
           manual_feedrate_mm_s = LOGICAL_AXIS_ARRAY(_mf.e / 60.0f,
                                                     _mf.x / 60.0f, _mf.y / 60.0f, _mf.z / 60.0f,
                                                     _mf.i / 60.0f, _mf.j / 60.0f, _mf.k / 60.0f,
                                                     _mf.u / 60.0f, _mf.v / 60.0f, _mf.w / 60.0f);
#endif

#if IS_KINEMATIC && HAS_JUNCTION_DEVIATION
  #define HAS_DIST_MM_ARG 1
#endif

/**
 * Planner block flags as boolean bit fields
 */
enum BlockFlagBit {
  // Recalculate trapezoids on entry junction. For optimization.
  BLOCK_BIT_RECALCULATE,

  // Nominal speed always reached.
  // i.e., The segment is long enough, so the nominal speed is reachable if accelerating
  // from a safe speed (in consideration of jerking from zero speed).
  BLOCK_BIT_NOMINAL_LENGTH,

  // The block is segment 2+ of a longer move
  BLOCK_BIT_CONTINUED,

  // Sync the stepper counts from the block
  BLOCK_BIT_SYNC_POSITION

  // Direct stepping page
  OPTARG(DIRECT_STEPPING, BLOCK_BIT_PAGE)


  // Sync the fan speeds from the block
  OPTARG(LASER_SYNCHRONOUS_M106_M107, BLOCK_BIT_SYNC_FANS)

  // Sync laser power from a queued block
  OPTARG(LASER_POWER_SYNC, BLOCK_BIT_LASER_PWR)
};

/**
 * Planner block flags as boolean bit fields
 */
typedef struct {
  union {
    uint8_t bits;

    struct {
      bool recalculate:1;

      bool nominal_length:1;

      bool continued:1;

      bool sync_position:1;

      #if ENABLED(DIRECT_STEPPING)
        bool page:1;
      #endif

      #if ENABLED(LASER_SYNCHRONOUS_M106_M107)
        bool sync_fans:1;
      #endif

      #if ENABLED(LASER_POWER_SYNC)
        bool sync_laser_pwr:1;
      #endif
    };
  };

  void clear() volatile { bits = 0; }
  void apply(const uint8_t f) volatile { bits |= f; }
  void apply(const BlockFlagBit b) volatile { SBI(bits, b); }
  void reset(const BlockFlagBit b) volatile { bits = _BV(b); }
  void set_nominal(const bool n) volatile { recalculate = true; if (n) nominal_length = true; }

} block_flags_t;

#if ENABLED(LASER_FEATURE)

  typedef struct {
    bool isEnabled:1;                                 // Set to engage the inline laser power output.
    bool dir:1;
    bool isPowered:1;                                 // Set on any parsed G1, G2, G3, or G5 powered move, cleared on G0 and G28.
    bool isSyncPower:1;                               // Set on a M3 sync based set laser power, used to determine active trap power
    bool Reserved:4;
  } power_status_t;

  typedef struct {
    power_status_t status;                            // See planner settings for meaning
    uint8_t power;                                    // Ditto; When in trapezoid mode this is nominal power

    #if ENABLED(LASER_POWER_TRAP)
      float trap_ramp_active_pwr;                     // Laser power level during active trapezoid smoothing
      float trap_ramp_entry_incr;                     // Acceleration per step laser power increment (trap entry)
      float trap_ramp_exit_decr;                      // Deceleration per step laser power decrement (trap exit)
    #endif
  } block_laser_t;

#endif

/**
 * struct block_t
 *
 * A single entry in the planner buffer.
 * Tracks linear movement over multiple axes.
 *
 * The "nominal" values are as-specified by G-code, and
 * may never actually be reached due to acceleration limits.
 */
typedef struct block_t {

  volatile block_flags_t flag;              // Block flags

  volatile bool is_fan_sync() { return TERN0(LASER_SYNCHRONOUS_M106_M107, flag.sync_fans); }
  volatile bool is_pwr_sync() { return TERN0(LASER_POWER_SYNC, flag.sync_laser_pwr); }
  volatile bool is_sync() { return flag.sync_position || is_fan_sync() || is_pwr_sync(); }
  volatile bool is_page() { return TERN0(DIRECT_STEPPING, flag.page); }
  volatile bool is_move() { return !(is_sync() || is_page()); }

  // Fields used by the motion planner to manage acceleration
  float nominal_speed_sqr,                  // The nominal speed for this block in (mm/sec)^2
        entry_speed_sqr,                    // Entry speed at previous-current junction in (mm/sec)^2
        max_entry_speed_sqr,                // Maximum allowable junction entry speed in (mm/sec)^2
        millimeters,                        // The total travel of this block in mm
        acceleration;                       // acceleration mm/sec^2

  union {
    abce_ulong_t steps;                     // Step count along each axis
    abce_long_t position;                   // New position to force when this sync block is executed
  };
  uint32_t step_event_count;                // The number of step events required to complete this block

  #if HAS_MULTI_EXTRUDER
    uint8_t extruder;                       // The extruder to move (if E move)
  #else
    static constexpr uint8_t extruder = 0;
  #endif

  #if ENABLED(MIXING_EXTRUDER)
    mixer_comp_t b_color[MIXING_STEPPERS];  // Normalized color for the mixing steppers
  #endif

  // Settings for the trapezoid generator
  uint32_t accelerate_until,                // The index of the step event on which to stop acceleration
           decelerate_after;                // The index of the step event on which to start decelerating

  #if ENABLED(S_CURVE_ACCELERATION)
    uint32_t cruise_rate,                   // The actual cruise rate to use, between end of the acceleration phase and start of deceleration phase
             acceleration_time,             // Acceleration time and deceleration time in STEP timer counts
             deceleration_time,
             acceleration_time_inverse,     // Inverse of acceleration and deceleration periods, expressed as integer. Scale depends on CPU being used
             deceleration_time_inverse;
  #else
    uint32_t acceleration_rate;             // The acceleration rate used for acceleration calculation
  #endif

  axis_bits_t direction_bits;               // The direction bit set for this block (refers to *_DIRECTION_BIT in config.h)

  // Advance extrusion
  #if ENABLED(LIN_ADVANCE)
    bool use_advance_lead;
    uint16_t advance_speed,                 // STEP timer value for extruder speed offset ISR
             max_adv_steps,                 // max. advance steps to get cruising speed pressure (not always nominal_speed!)
             final_adv_steps;               // advance steps due to exit speed
    float e_D_ratio;
  #endif

  uint32_t nominal_rate,                    // The nominal step rate for this block in step_events/sec
           initial_rate,                    // The jerk-adjusted step rate at start of block
           final_rate,                      // The minimal rate at exit
           acceleration_steps_per_s2;       // acceleration steps/sec^2

  #if ENABLED(DIRECT_STEPPING)
    page_idx_t page_idx;                    // Page index used for direct stepping
  #endif

  #if HAS_CUTTER
    cutter_power_t cutter_power;            // Power level for Spindle, Laser, etc.
  #endif

  #if HAS_FAN
    uint8_t fan_speed[FAN_COUNT];
  #endif

  #if ENABLED(BARICUDA)
    uint8_t valve_pressure, e_to_p_pressure;
  #endif

  #if HAS_WIRED_LCD
    uint32_t segment_time_us;
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    uint32_t sdpos;
    xyze_pos_t start_position;
  #endif

  #if ENABLED(LASER_FEATURE)
    block_laser_t laser;
  #endif

  void reset() { memset((char*)this, 0, sizeof(*this)); }

} block_t;

#if ANY(LIN_ADVANCE, SCARA_FEEDRATE_SCALING, GRADIENT_MIX, LCD_SHOW_E_TOTAL, POWER_LOSS_RECOVERY)
  #define HAS_POSITION_FLOAT 1
#endif

#define BLOCK_MOD(n) ((n)&(BLOCK_BUFFER_SIZE-1))

#if ENABLED(LASER_FEATURE)
  typedef struct {
    /**
     * Laser status flags
     */
    power_status_t status;
    /**
     * Laser power: 0 or 255 in case of PWM-less laser,
     * or the OCR (oscillator count register) value;
     * Using OCR instead of raw power, because it avoids
     * floating point operations during the move loop.
     */
    volatile uint8_t power;
  } laser_state_t;
#endif

typedef struct {
   uint32_t max_acceleration_mm_per_s2[DISTINCT_AXES], // (mm/s^2) M201 XYZE
            min_segment_time_us;                // (µs) M205 B
      float axis_steps_per_mm[DISTINCT_AXES];   // (steps) M92 XYZE - Steps per millimeter
 feedRate_t max_feedrate_mm_s[DISTINCT_AXES];   // (mm/s) M203 XYZE - Max speeds
      float acceleration,                       // (mm/s^2) M204 S - Normal acceleration. DEFAULT ACCELERATION for all printing moves.
            retract_acceleration,               // (mm/s^2) M204 R - Retract acceleration. Filament pull-back and push-forward while standing still in the other axes
            travel_acceleration;                // (mm/s^2) M204 T - Travel acceleration. DEFAULT ACCELERATION for all NON printing moves.
 feedRate_t min_feedrate_mm_s,                  // (mm/s) M205 S - Minimum linear feedrate
            min_travel_feedrate_mm_s;           // (mm/s) M205 T - Minimum travel feedrate
} planner_settings_t;

#if ENABLED(IMPROVE_HOMING_RELIABILITY)
  struct motion_state_t {
    TERN(DELTA, xyz_ulong_t, xy_ulong_t) acceleration;
    #if HAS_CLASSIC_JERK
      TERN(DELTA, xyz_float_t, xy_float_t) jerk_state;
    #endif
  };
#endif

#if DISABLED(SKEW_CORRECTION)
  #define XY_SKEW_FACTOR 0
  #define XZ_SKEW_FACTOR 0
  #define YZ_SKEW_FACTOR 0
#endif

typedef struct {
  #if ENABLED(SKEW_CORRECTION_GCODE)
    float xy;
    #if ENABLED(SKEW_CORRECTION_FOR_Z)
      float xz, yz;
    #else
      const float xz = XZ_SKEW_FACTOR, yz = YZ_SKEW_FACTOR;
    #endif
  #else
    const float xy = XY_SKEW_FACTOR,
                xz = XZ_SKEW_FACTOR, yz = YZ_SKEW_FACTOR;
  #endif
} skew_factor_t;

#if ENABLED(DISABLE_INACTIVE_EXTRUDER)
  typedef IF<(BLOCK_BUFFER_SIZE > 64), uint16_t, uint8_t>::type last_move_t;
#endif

#if ENABLED(ARC_SUPPORT)
  #define HINTS_CURVE_RADIUS
  #define HINTS_SAFE_EXIT_SPEED
#endif

struct PlannerHints {
  float millimeters = 0.0;            // Move Length, if known, else 0.
  #if ENABLED(SCARA_FEEDRATE_SCALING)
    float inv_duration = 0.0;         // Reciprocal of the move duration, if known
  #endif
  #if ENABLED(HINTS_CURVE_RADIUS)
    float curve_radius = 0.0;         // Radius of curvature of the motion path - to calculate cornering speed
  #else
    static constexpr float curve_radius = 0.0;
  #endif
  #if ENABLED(HINTS_SAFE_EXIT_SPEED)
    float safe_exit_speed_sqr = 0.0;  // Square of the speed considered "safe" at the end of the segment
                                      // i.e., at or below the exit speed of the segment that the planner
                                      // would calculate if it knew the as-yet-unbuffered path
  #endif

  PlannerHints(const_float_t mm=0.0f) : millimeters(mm) {}
};

class Planner {
  public:

    /**
     * The move buffer, calculated in stepper steps
     *
     * block_buffer is a ring buffer...
     *
     *             head,tail : indexes for write,read
     *            head==tail : the buffer is empty
     *            head!=tail : blocks are in the buffer
     *   head==(tail-1)%size : the buffer is full
     *
     *  Writer of head is Planner::buffer_segment().
     *  Reader of tail is Stepper::isr(). Always consider tail busy / read-only
     */
    static block_t block_buffer[BLOCK_BUFFER_SIZE];
    static volatile uint8_t block_buffer_head,      // Index of the next block to be pushed
                            block_buffer_nonbusy,   // Index of the first non busy block
                            block_buffer_planned,   // Index of the optimally planned block
                            block_buffer_tail;      // Index of the busy block, if any
    static uint16_t cleaning_buffer_counter;        // A counter to disable queuing of blocks
    static uint8_t delay_before_delivering;         // This counter delays delivery of blocks when queue becomes empty to allow the opportunity of merging blocks


    #if ENABLED(DISTINCT_E_FACTORS)
      static uint8_t last_extruder;                 // Respond to extruder change
    #endif

    #if ENABLED(DIRECT_STEPPING)
      static uint32_t last_page_step_rate;          // Last page step rate given
      static xyze_bool_t last_page_dir;             // Last page direction given
    #endif

    #if HAS_EXTRUDERS
      static int16_t flow_percentage[EXTRUDERS];    // Extrusion factor for each extruder
      static float e_factor[EXTRUDERS];             // The flow percentage and volumetric multiplier combine to scale E movement
    #endif

    #if DISABLED(NO_VOLUMETRICS)
      static float filament_size[EXTRUDERS],          // diameter of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder
                   volumetric_area_nominal,           // Nominal cross-sectional area
                   volumetric_multiplier[EXTRUDERS];  // Reciprocal of cross-sectional area of filament (in mm^2). Pre-calculated to reduce computation in the planner
                                                      // May be auto-adjusted by a filament width sensor
    #endif

    #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
      static float volumetric_extruder_limit[EXTRUDERS],          // Maximum mm^3/sec the extruder can handle
                   volumetric_extruder_feedrate_limit[EXTRUDERS]; // Feedrate limit (mm/s) calculated from volume limit
    #endif

    static planner_settings_t settings;

    #if ENABLED(LASER_FEATURE)
      static laser_state_t laser_inline;
    #endif

    static uint32_t max_acceleration_steps_per_s2[DISTINCT_AXES]; // (steps/s^2) Derived from mm_per_s2
    static float mm_per_step[DISTINCT_AXES];          // Millimeters per step

    #if HAS_JUNCTION_DEVIATION
      static float junction_deviation_mm;             // (mm) M205 J
      #if HAS_LINEAR_E_JERK
        static float max_e_jerk[DISTINCT_E];          // Calculated from junction_deviation_mm
      #endif
    #endif

    #if HAS_CLASSIC_JERK
      // (mm/s^2) M205 XYZ(E) - The largest speed change requiring no acceleration.
      static TERN(HAS_LINEAR_E_JERK, xyz_pos_t, xyze_pos_t) max_jerk;
    #endif

    #if HAS_LEVELING
      static bool leveling_active;          // Flag that bed leveling is enabled
      #if ABL_PLANAR
        static matrix_3x3 bed_level_matrix; // Transform to compensate for bed level
      #endif
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        static float z_fade_height, inverse_z_fade_height;
      #endif
    #else
      static constexpr bool leveling_active = false;
    #endif

    #if ENABLED(LIN_ADVANCE)
      static float extruder_advance_K[EXTRUDERS];
    #endif

    /**
     * The current position of the tool in absolute steps
     * Recalculated if any axis_steps_per_mm are changed by G-code
     */
    static xyze_long_t position;

    #if HAS_POSITION_FLOAT
      static xyze_pos_t position_float;
    #endif

    #if IS_KINEMATIC
      static xyze_pos_t position_cart;
    #endif

    static skew_factor_t skew_factor;

    #if ENABLED(SD_ABORT_ON_ENDSTOP_HIT)
      static bool abort_on_endstop_hit;
    #endif
    #ifdef XY_FREQUENCY_LIMIT
      static int8_t xy_freq_limit_hz;         // Minimum XY frequency setting
      static float xy_freq_min_speed_factor;  // Minimum speed factor setting
      static int32_t xy_freq_min_interval_us; // Minimum segment time based on xy_freq_limit_hz
      static void refresh_frequency_limit() {
        //xy_freq_min_interval_us = xy_freq_limit_hz ?: LROUND(1000000.0f / xy_freq_limit_hz);
        if (xy_freq_limit_hz)
          xy_freq_min_interval_us = LROUND(1000000.0f / xy_freq_limit_hz);
      }
      static void set_min_speed_factor_u8(const uint8_t v255) {
        xy_freq_min_speed_factor = float(ui8_to_percent(v255)) / 100;
      }
      static void set_frequency_limit(const uint8_t hz) {
        xy_freq_limit_hz = constrain(hz, 0, 100);
        refresh_frequency_limit();
      }
    #endif

  private:

    /**
     * Speed of previous path line segment
     */
    static xyze_float_t previous_speed;

    /**
     * Nominal speed of previous path line segment (mm/s)^2
     */
    static float previous_nominal_speed_sqr;

    /**
     * Limit where 64bit math is necessary for acceleration calculation
     */
    static uint32_t acceleration_long_cutoff;

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      static float last_fade_z;
    #endif

    #if ENABLED(DISABLE_INACTIVE_EXTRUDER)
      // Counters to manage disabling inactive extruder steppers
      static last_move_t g_uc_extruder_last_move[E_STEPPERS];
    #endif

    #if HAS_WIRED_LCD
      volatile static uint32_t block_buffer_runtime_us; // Theoretical block buffer runtime in µs
    #endif

  public:

    /**
     * Instance Methods
     */

    Planner();

    void init();

    /**
     * Static (class) Methods
     */

    // Recalculate steps/s^2 accelerations based on mm/s^2 settings
    static void refresh_acceleration_rates();

    /**
     * Recalculate 'position' and 'mm_per_step'.
     * Must be called whenever settings.axis_steps_per_mm changes!
     */
    static void refresh_positioning();

    // For an axis set the Maximum Acceleration in mm/s^2
    static void set_max_acceleration(const AxisEnum axis, float inMaxAccelMMS2);

    // For an axis set the Maximum Feedrate in mm/s
    static void set_max_feedrate(const AxisEnum axis, float inMaxFeedrateMMS);

    // For an axis set the Maximum Jerk (instant change) in mm/s
    #if HAS_CLASSIC_JERK
      static void set_max_jerk(const AxisEnum axis, float inMaxJerkMMS);
    #else
      static void set_max_jerk(const AxisEnum, const_float_t) {}
    #endif

    #if HAS_EXTRUDERS
      FORCE_INLINE static void refresh_e_factor(const uint8_t e) {
        e_factor[e] = flow_percentage[e] * 0.01f * TERN(NO_VOLUMETRICS, 1.0f, volumetric_multiplier[e]);
      }

      static void set_flow(const uint8_t e, const int16_t flow) {
        flow_percentage[e] = flow;
        refresh_e_factor(e);
      }

    #endif

    // Manage fans, paste pressure, etc.
    static void check_axes_activity();

    // Apply fan speeds
    #if HAS_FAN
      static void sync_fan_speeds(uint8_t (&fan_speed)[FAN_COUNT]);
      #if FAN_KICKSTART_TIME
        static void kickstart_fan(uint8_t (&fan_speed)[FAN_COUNT], const millis_t &ms, const uint8_t f);
      #else
        FORCE_INLINE static void kickstart_fan(uint8_t (&)[FAN_COUNT], const millis_t &, const uint8_t) {}
      #endif
    #endif

    #if ENABLED(FILAMENT_WIDTH_SENSOR)
      void apply_filament_width_sensor(const int8_t encoded_ratio);

      static float volumetric_percent(const bool vol) {
        return 100.0f * (vol
            ? volumetric_area_nominal / volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
            : volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
        );
      }
    #endif

    #if ENABLED(IMPROVE_HOMING_RELIABILITY)
      void enable_stall_prevention(const bool onoff);
    #endif

    #if DISABLED(NO_VOLUMETRICS)

      // Update multipliers based on new diameter measurements
      static void calculate_volumetric_multipliers();

      #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
        // Update pre calculated extruder feedrate limits based on volumetric values
        static void calculate_volumetric_extruder_limit(const uint8_t e);
        static void calculate_volumetric_extruder_limits();
      #endif

      FORCE_INLINE static void set_filament_size(const uint8_t e, const_float_t v) {
        filament_size[e] = v;
        if (v > 0) volumetric_area_nominal = CIRCLE_AREA(v * 0.5); //TODO: should it be per extruder
        // make sure all extruders have some sane value for the filament size
        LOOP_L_N(i, COUNT(filament_size))
          if (!filament_size[i]) filament_size[i] = DEFAULT_NOMINAL_FILAMENT_DIA;
      }

    #endif

    #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
      FORCE_INLINE static void set_volumetric_extruder_limit(const uint8_t e, const_float_t v) {
        volumetric_extruder_limit[e] = v;
        calculate_volumetric_extruder_limit(e);
      }
    #endif

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

      /**
       * Get the Z leveling fade factor based on the given Z height,
       * re-calculating only when needed.
       *
       *  Returns 1.0 if planner.z_fade_height is 0.0.
       *  Returns 0.0 if Z is past the specified 'Fade Height'.
       */
      static float fade_scaling_factor_for_z(const_float_t rz) {
        static float z_fade_factor = 1;
        if (!z_fade_height || rz <= 0) return 1;
        if (rz >= z_fade_height) return 0;
        if (last_fade_z != rz) {
          last_fade_z = rz;
          z_fade_factor = 1 - rz * inverse_z_fade_height;
        }
        return z_fade_factor;
      }

      FORCE_INLINE static void force_fade_recalc() { last_fade_z = -999.999f; }

      FORCE_INLINE static void set_z_fade_height(const_float_t zfh) {
        z_fade_height = zfh > 0 ? zfh : 0;
        inverse_z_fade_height = RECIPROCAL(z_fade_height);
        force_fade_recalc();
      }

      FORCE_INLINE static bool leveling_active_at_z(const_float_t rz) {
        return !z_fade_height || rz < z_fade_height;
      }

    #else

      FORCE_INLINE static float fade_scaling_factor_for_z(const_float_t) { return 1; }

      FORCE_INLINE static bool leveling_active_at_z(const_float_t) { return true; }

    #endif

    #if ENABLED(SKEW_CORRECTION)

      FORCE_INLINE static void skew(float &cx, float &cy, const_float_t cz) {
        if (COORDINATE_OKAY(cx, X_MIN_POS + 1, X_MAX_POS) && COORDINATE_OKAY(cy, Y_MIN_POS + 1, Y_MAX_POS)) {
          const float sx = cx - cy * skew_factor.xy - cz * (skew_factor.xz - (skew_factor.xy * skew_factor.yz)),
                      sy = cy - cz * skew_factor.yz;
          if (COORDINATE_OKAY(sx, X_MIN_POS, X_MAX_POS) && COORDINATE_OKAY(sy, Y_MIN_POS, Y_MAX_POS)) {
            cx = sx; cy = sy;
          }
        }
      }
      FORCE_INLINE static void skew(xyz_pos_t &raw) { skew(raw.x, raw.y, raw.z); }

      FORCE_INLINE static void unskew(float &cx, float &cy, const_float_t cz) {
        if (COORDINATE_OKAY(cx, X_MIN_POS, X_MAX_POS) && COORDINATE_OKAY(cy, Y_MIN_POS, Y_MAX_POS)) {
          const float sx = cx + cy * skew_factor.xy + cz * skew_factor.xz,
                      sy = cy + cz * skew_factor.yz;
          if (COORDINATE_OKAY(sx, X_MIN_POS, X_MAX_POS) && COORDINATE_OKAY(sy, Y_MIN_POS, Y_MAX_POS)) {
            cx = sx; cy = sy;
          }
        }
      }
      FORCE_INLINE static void unskew(xyz_pos_t &raw) { unskew(raw.x, raw.y, raw.z); }

    #endif // SKEW_CORRECTION

    #if HAS_LEVELING
      /**
       * Apply leveling to transform a cartesian position
       * as it will be given to the planner and steppers.
       */
      static void apply_leveling(xyz_pos_t &raw);
      static void unapply_leveling(xyz_pos_t &raw);
      FORCE_INLINE static void force_unapply_leveling(xyz_pos_t &raw) {
        leveling_active = true;
        unapply_leveling(raw);
        leveling_active = false;
      }
    #else
      FORCE_INLINE static void apply_leveling(xyz_pos_t&) {}
      FORCE_INLINE static void unapply_leveling(xyz_pos_t&) {}
    #endif

    #if ENABLED(FWRETRACT)
      static void apply_retract(float &rz, float &e);
      FORCE_INLINE static void apply_retract(xyze_pos_t &raw) { apply_retract(raw.z, raw.e); }
      static void unapply_retract(float &rz, float &e);
      FORCE_INLINE static void unapply_retract(xyze_pos_t &raw) { unapply_retract(raw.z, raw.e); }
    #endif

    #if HAS_POSITION_MODIFIERS
      FORCE_INLINE static void apply_modifiers(xyze_pos_t &pos, bool leveling=ENABLED(PLANNER_LEVELING)) {
        TERN_(SKEW_CORRECTION, skew(pos));
        if (leveling) apply_leveling(pos);
        TERN_(FWRETRACT, apply_retract(pos));
      }

      FORCE_INLINE static void unapply_modifiers(xyze_pos_t &pos, bool leveling=ENABLED(PLANNER_LEVELING)) {
        TERN_(FWRETRACT, unapply_retract(pos));
        if (leveling) unapply_leveling(pos);
        TERN_(SKEW_CORRECTION, unskew(pos));
      }
    #endif // HAS_POSITION_MODIFIERS

    // Number of moves currently in the planner including the busy block, if any
    FORCE_INLINE static uint8_t movesplanned() { return BLOCK_MOD(block_buffer_head - block_buffer_tail); }

    // Number of nonbusy moves currently in the planner
    FORCE_INLINE static uint8_t nonbusy_movesplanned() { return BLOCK_MOD(block_buffer_head - block_buffer_nonbusy); }

    // Remove all blocks from the buffer
    FORCE_INLINE static void clear_block_buffer() { block_buffer_nonbusy = block_buffer_planned = block_buffer_head = block_buffer_tail = 0; }

    // Check if movement queue is full
    FORCE_INLINE static bool is_full() { return block_buffer_tail == next_block_index(block_buffer_head); }

    // Get count of movement slots free
    FORCE_INLINE static uint8_t moves_free() { return BLOCK_BUFFER_SIZE - 1 - movesplanned(); }

    /**
     * Planner::get_next_free_block
     *
     * - Get the next head indices (passed by reference)
     * - Wait for the number of spaces to open up in the planner
     * - Return the first head block
     */
    FORCE_INLINE static block_t* get_next_free_block(uint8_t &next_buffer_head, const uint8_t count=1) {

      // Wait until there are enough slots free
      while (moves_free() < count) { idle(); }

      // Return the first available block
      next_buffer_head = next_block_index(block_buffer_head);
      return &block_buffer[block_buffer_head];
    }

    /**
     * Planner::_buffer_steps
     *
     * Add a new linear movement to the buffer (in terms of steps).
     *
     *  target      - target position in steps units
     *  fr_mm_s     - (target) speed of the move
     *  extruder    - target extruder
     *  hints       - parameters to aid planner calculations
     *
     * Returns true if movement was buffered, false otherwise
     */
    static bool _buffer_steps(const xyze_long_t &target
      OPTARG(HAS_POSITION_FLOAT, const xyze_pos_t &target_float)
      OPTARG(HAS_DIST_MM_ARG, const xyze_float_t &cart_dist_mm)
      , feedRate_t fr_mm_s, const uint8_t extruder, const PlannerHints &hints
    );

    /**
     * @brief Populate a block in preparation for insertion
     * @details Populate the fields of a new linear movement block
     *          that will be added to the queue and processed soon
     *          by the Stepper ISR.
     *
     * @param block         A block to populate
     * @param target        Target position in steps units
     * @param target_float  Target position in native mm
     * @param cart_dist_mm  The pre-calculated move lengths for all axes, in mm
     * @param fr_mm_s       (target) speed of the move
     * @param extruder      target extruder
     * @param hints         parameters to aid planner calculations
     *
     * @return  true if movement is acceptable, false otherwise
     */
    static bool _populate_block(block_t * const block, const xyze_long_t &target
      OPTARG(HAS_POSITION_FLOAT, const xyze_pos_t &target_float)
      OPTARG(HAS_DIST_MM_ARG, const xyze_float_t &cart_dist_mm)
      , feedRate_t fr_mm_s, const uint8_t extruder, const PlannerHints &hints
    );

    /**
     * Planner::buffer_sync_block
     * Add a block to the buffer that just updates the position
     * @param sync_flag sets a condition bit to process additional items
     * such as sync fan pwm or sync M3/M4 laser power into a queued block
     */
      static void buffer_sync_block(const BlockFlagBit flag=BLOCK_BIT_SYNC_POSITION);

  #if IS_KINEMATIC
    private:

      // Allow do_homing_move to access internal functions, such as buffer_segment.
      friend void do_homing_move(const AxisEnum, const float, const feedRate_t, const bool);
  #endif

    /**
     * Planner::buffer_segment
     *
     * Add a new linear movement to the buffer in axis units.
     *
     * Leveling and kinematics should be applied ahead of calling this.
     *
     *  a,b,c,e     - target positions in mm and/or degrees
     *  fr_mm_s     - (target) speed of the move
     *  extruder    - optional target extruder (otherwise active_extruder)
     *  hints       - optional parameters to aid planner calculations
     */
    static bool buffer_segment(const abce_pos_t &abce
      OPTARG(HAS_DIST_MM_ARG, const xyze_float_t &cart_dist_mm)
      , const_feedRate_t fr_mm_s
      , const uint8_t extruder=active_extruder
      , const PlannerHints &hints=PlannerHints()
    );

  public:

    /**
     * Add a new linear movement to the buffer.
     * The target is cartesian. It's translated to
     * delta/scara if needed.
     *
     *  cart         - target position in mm or degrees
     *  fr_mm_s      - (target) speed of the move (mm/s)
     *  extruder     - optional target extruder (otherwise active_extruder)
     *  hints        - optional parameters to aid planner calculations
     */
    static bool buffer_line(const xyze_pos_t &cart, const_feedRate_t fr_mm_s
      , const uint8_t extruder=active_extruder
      , const PlannerHints &hints=PlannerHints()
    );

    #if ENABLED(DIRECT_STEPPING)
      static void buffer_page(const page_idx_t page_idx, const uint8_t extruder, const uint16_t num_steps);
    #endif

    /**
     * Set the planner.position and individual stepper positions.
     * Used by G92, G28, G29, and other procedures.
     *
     * The supplied position is in the cartesian coordinate space and is
     * translated in to machine space as needed. Modifiers such as leveling
     * and skew are also applied.
     *
     * Multiplies by axis_steps_per_mm[] and does necessary conversion
     * for COREXY / COREXZ / COREYZ to set the corresponding stepper positions.
     *
     * Clears previous speed values.
     */
    static void set_position_mm(const xyze_pos_t &xyze);

    #if HAS_EXTRUDERS
      static void set_e_position_mm(const_float_t e);
    #endif

    /**
     * Set the planner.position and individual stepper positions.
     *
     * The supplied position is in machine space, and no additional
     * conversions are applied.
     */
    static void set_machine_position_mm(const abce_pos_t &abce);

    /**
     * Get an axis position according to stepper position(s)
     * For CORE machines apply translation from ABC to XYZ.
     */
    static float get_axis_position_mm(const AxisEnum axis);

    static abce_pos_t get_axis_positions_mm() {
      const abce_pos_t out = LOGICAL_AXIS_ARRAY(
        get_axis_position_mm(E_AXIS),
        get_axis_position_mm(A_AXIS), get_axis_position_mm(B_AXIS), get_axis_position_mm(C_AXIS),
        get_axis_position_mm(I_AXIS), get_axis_position_mm(J_AXIS), get_axis_position_mm(K_AXIS),
        get_axis_position_mm(U_AXIS), get_axis_position_mm(V_AXIS), get_axis_position_mm(W_AXIS)
      );
      return out;
    }

    // SCARA AB axes are in degrees, not mm
    #if IS_SCARA
      FORCE_INLINE static float get_axis_position_degrees(const AxisEnum axis) { return get_axis_position_mm(axis); }
    #endif

    // Called to force a quick stop of the machine (for example, when
    // a Full Shutdown is required, or when endstops are hit)
    static void quick_stop();

    #if ENABLED(REALTIME_REPORTING_COMMANDS)
      // Force a quick pause of the machine (e.g., when a pause is required in the middle of move).
      // NOTE: Hard-stops will lose steps so encoders are highly recommended if using these!
      static void quick_pause();
      static void quick_resume();
    #endif

    // Called when an endstop is triggered. Causes the machine to stop immediately
    static void endstop_triggered(const AxisEnum axis);

    // Triggered position of an axis in mm (not core-savvy)
    static float triggered_position_mm(const AxisEnum axis);

    // Blocks are queued, or we're running out moves, or the closed loop controller is waiting
    static bool busy() {
      return (has_blocks_queued() || cleaning_buffer_counter
          || TERN0(EXTERNAL_CLOSED_LOOP_CONTROLLER, CLOSED_LOOP_WAITING())
      );
    }

    // Block until all buffered steps are executed / cleaned
    static void synchronize();

    // Wait for moves to finish and disable all steppers
    static void finish_and_disable();

    // Periodic handler to manage the cleaning buffer counter
    // Called from the Temperature ISR at ~1kHz
    static void isr() { if (cleaning_buffer_counter) --cleaning_buffer_counter; }

    /**
     * Does the buffer have any blocks queued?
     */
    FORCE_INLINE static bool has_blocks_queued() { return (block_buffer_head != block_buffer_tail); }

    /**
     * Get the current block for processing
     * and mark the block as busy.
     * Return nullptr if the buffer is empty
     * or if there is a first-block delay.
     *
     * WARNING: Called from Stepper ISR context!
     */
    static block_t* get_current_block();

    /**
     * "Release" the current block so its slot can be reused.
     * Called when the current block is no longer needed.
     */
    FORCE_INLINE static void release_current_block() {
      if (has_blocks_queued())
        block_buffer_tail = next_block_index(block_buffer_tail);
    }

    #if HAS_WIRED_LCD
      static uint16_t block_buffer_runtime();
      static void clear_block_buffer_runtime();
    #endif

    #if ENABLED(AUTOTEMP)
      static celsius_t autotemp_min, autotemp_max;
      static float autotemp_factor;
      static bool autotemp_enabled;
      static void autotemp_update();
      static void autotemp_M104_M109();
      static void autotemp_task();
    #endif

    #if HAS_LINEAR_E_JERK
      FORCE_INLINE static void recalculate_max_e_jerk() {
        const float prop = junction_deviation_mm * SQRT(0.5) / (1.0f - SQRT(0.5));
        EXTRUDER_LOOP()
          max_e_jerk[E_INDEX_N(e)] = SQRT(prop * settings.max_acceleration_mm_per_s2[E_INDEX_N(e)]);
      }
    #endif

  private:

    #if ENABLED(AUTOTEMP)
      #if ENABLED(AUTOTEMP_PROPORTIONAL)
        static void _autotemp_update_from_hotend();
      #else
        static void _autotemp_update_from_hotend() {}
      #endif
    #endif

    /**
     * Get the index of the next / previous block in the ring buffer
     */
    static constexpr uint8_t next_block_index(const uint8_t block_index) { return BLOCK_MOD(block_index + 1); }
    static constexpr uint8_t prev_block_index(const uint8_t block_index) { return BLOCK_MOD(block_index - 1); }

    /**
     * Calculate the distance (not time) it takes to accelerate
     * from initial_rate to target_rate using the given acceleration:
     */
    static float estimate_acceleration_distance(const_float_t initial_rate, const_float_t target_rate, const_float_t accel) {
      if (accel == 0) return 0; // accel was 0, set acceleration distance to 0
      return (sq(target_rate) - sq(initial_rate)) / (accel * 2);
    }

    /**
     * Return the point at which you must start braking (at the rate of -'accel') if
     * you start at 'initial_rate', accelerate (until reaching the point), and want to end at
     * 'final_rate' after traveling 'distance'.
     *
     * This is used to compute the intersection point between acceleration and deceleration
     * in cases where the "trapezoid" has no plateau (i.e., never reaches maximum speed)
     */
    static float intersection_distance(const_float_t initial_rate, const_float_t final_rate, const_float_t accel, const_float_t distance) {
      if (accel == 0) return 0; // accel was 0, set intersection distance to 0
      return (accel * 2 * distance - sq(initial_rate) + sq(final_rate)) / (accel * 4);
    }

    /**
     * Calculate the maximum allowable speed squared at this point, in order
     * to reach 'target_velocity_sqr' using 'acceleration' within a given
     * 'distance'.
     */
    static float max_allowable_speed_sqr(const_float_t accel, const_float_t target_velocity_sqr, const_float_t distance) {
      return target_velocity_sqr - 2 * accel * distance;
    }

    #if ENABLED(S_CURVE_ACCELERATION)
      /**
       * Calculate the speed reached given initial speed, acceleration and distance
       */
      static float final_speed(const_float_t initial_velocity, const_float_t accel, const_float_t distance) {
        return SQRT(sq(initial_velocity) + 2 * accel * distance);
      }
    #endif

    static void calculate_trapezoid_for_block(block_t * const block, const_float_t entry_factor, const_float_t exit_factor);

    static void reverse_pass_kernel(block_t * const current, const block_t * const next OPTARG(ARC_SUPPORT, const_float_t safe_exit_speed_sqr));
    static void forward_pass_kernel(const block_t * const previous, block_t * const current, uint8_t block_index);

    static void reverse_pass(TERN_(ARC_SUPPORT, const_float_t safe_exit_speed_sqr));
    static void forward_pass();

    static void recalculate_trapezoids(TERN_(ARC_SUPPORT, const_float_t safe_exit_speed_sqr));

    static void recalculate(TERN_(ARC_SUPPORT, const_float_t safe_exit_speed_sqr));

    #if HAS_JUNCTION_DEVIATION

      FORCE_INLINE static void normalize_junction_vector(xyze_float_t &vector) {
        float magnitude_sq = 0;
        LOOP_LOGICAL_AXES(idx) if (vector[idx]) magnitude_sq += sq(vector[idx]);
        vector *= RSQRT(magnitude_sq);
      }

      FORCE_INLINE static float limit_value_by_axis_maximum(const_float_t max_value, xyze_float_t &unit_vec) {
        float limit_value = max_value;
        LOOP_LOGICAL_AXES(idx) {
          if (unit_vec[idx]) {
            if (limit_value * ABS(unit_vec[idx]) > settings.max_acceleration_mm_per_s2[idx])
              limit_value = ABS(settings.max_acceleration_mm_per_s2[idx] / unit_vec[idx]);
          }
        }
        return limit_value;
      }

    #endif // HAS_JUNCTION_DEVIATION
};

#define PLANNER_XY_FEEDRATE() _MIN(planner.settings.max_feedrate_mm_s[X_AXIS], planner.settings.max_feedrate_mm_s[Y_AXIS])

extern Planner planner;
