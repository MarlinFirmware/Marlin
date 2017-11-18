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
 * planner.h
 *
 * Buffer movement commands and manage the acceleration profile plan
 *
 * Derived from Grbl
 * Copyright (c) 2009-2011 Simen Svale Skogsrud
 */

#ifndef PLANNER_H
#define PLANNER_H

#include "../Marlin.h"

#include "motion.h"

#if ENABLED(DELTA)
  #include "delta.h"
#endif

#if HAS_ABL
  #include "../libs/vector_3.h"
#endif

enum BlockFlagBit {
  // Recalculate trapezoids on entry junction. For optimization.
  BLOCK_BIT_RECALCULATE,

  // Nominal speed always reached.
  // i.e., The segment is long enough, so the nominal speed is reachable if accelerating
  // from a safe speed (in consideration of jerking from zero speed).
  BLOCK_BIT_NOMINAL_LENGTH,

  // Start from a halt at the start of this block, respecting the maximum allowed jerk.
  BLOCK_BIT_START_FROM_FULL_HALT,

  // The block is busy
  BLOCK_BIT_BUSY
};

enum BlockFlag {
  BLOCK_FLAG_RECALCULATE          = _BV(BLOCK_BIT_RECALCULATE),
  BLOCK_FLAG_NOMINAL_LENGTH       = _BV(BLOCK_BIT_NOMINAL_LENGTH),
  BLOCK_FLAG_START_FROM_FULL_HALT = _BV(BLOCK_BIT_START_FROM_FULL_HALT),
  BLOCK_FLAG_BUSY                 = _BV(BLOCK_BIT_BUSY)
};

/**
 * struct block_t
 *
 * A single entry in the planner buffer.
 * Tracks linear movement over multiple axes.
 *
 * The "nominal" values are as-specified by gcode, and
 * may never actually be reached due to acceleration limits.
 */
typedef struct {

  uint8_t flag;                             // Block flags (See BlockFlag enum above)

  unsigned char active_extruder;            // The extruder to move (if E move)

  // Fields used by the Bresenham algorithm for tracing the line
  int32_t steps[NUM_AXIS];                  // Step count along each axis
  uint32_t step_event_count;                // The number of step events required to complete this block

  #if ENABLED(MIXING_EXTRUDER)
    uint32_t mix_event_count[MIXING_STEPPERS]; // Scaled step_event_count for the mixing steppers
  #endif

  int32_t accelerate_until,                 // The index of the step event on which to stop acceleration
          decelerate_after,                 // The index of the step event on which to start decelerating
          acceleration_rate;                // The acceleration rate used for acceleration calculation

  uint8_t direction_bits;                   // The direction bit set for this block (refers to *_DIRECTION_BIT in config.h)

  // Advance extrusion
  #if ENABLED(LIN_ADVANCE)
    bool use_advance_lead;
    uint32_t abs_adv_steps_multiplier8; // Factorised by 2^8 to avoid float
  #endif

  // Fields used by the motion planner to manage acceleration
  float nominal_speed,                      // The nominal speed for this block in mm/sec
        entry_speed,                        // Entry speed at previous-current junction in mm/sec
        max_entry_speed,                    // Maximum allowable junction entry speed in mm/sec
        millimeters,                        // The total travel of this block in mm
        acceleration;                       // acceleration mm/sec^2

  // Settings for the trapezoid generator
  uint32_t nominal_rate,                    // The nominal step rate for this block in step_events/sec
           initial_rate,                    // The jerk-adjusted step rate at start of block
           final_rate,                      // The minimal rate at exit
           acceleration_steps_per_s2;       // acceleration steps/sec^2

  #if FAN_COUNT > 0
    uint16_t fan_speed[FAN_COUNT];
  #endif

  #if ENABLED(BARICUDA)
    uint8_t valve_pressure, e_to_p_pressure;
  #endif

  uint32_t segment_time_us;

} block_t;

#define BLOCK_MOD(n) ((n)&(BLOCK_BUFFER_SIZE-1))

class Planner {

  public:

    /**
     * A ring buffer of moves described in steps
     */
    static block_t block_buffer[BLOCK_BUFFER_SIZE];
    static volatile uint8_t block_buffer_head,  // Index of the next block to be pushed
                            block_buffer_tail;

    #if ENABLED(DISTINCT_E_FACTORS)
      static uint8_t last_extruder;             // Respond to extruder change
    #endif

    static int16_t flow_percentage[EXTRUDERS];  // Extrusion factor for each extruder

    static float e_factor[EXTRUDERS],               // The flow percentage and volumetric multiplier combine to scale E movement
                 filament_size[EXTRUDERS],          // diameter of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder
                 volumetric_area_nominal,           // Nominal cross-sectional area
                 volumetric_multiplier[EXTRUDERS];  // Reciprocal of cross-sectional area of filament (in mm^2). Pre-calculated to reduce computation in the planner
                                                    // May be auto-adjusted by a filament width sensor

    static float max_feedrate_mm_s[XYZE_N],     // Max speeds in mm per second
                 axis_steps_per_mm[XYZE_N],
                 steps_to_mm[XYZE_N];
    static uint32_t max_acceleration_steps_per_s2[XYZE_N],
                    max_acceleration_mm_per_s2[XYZE_N]; // Use M201 to override

    static uint32_t min_segment_time_us; // Use 'M205 B<µs>' to override
    static float min_feedrate_mm_s,
                 acceleration,         // Normal acceleration mm/s^2  DEFAULT ACCELERATION for all printing moves. M204 SXXXX
                 retract_acceleration, // Retract acceleration mm/s^2 filament pull-back and push-forward while standing still in the other axes M204 TXXXX
                 travel_acceleration,  // Travel acceleration mm/s^2  DEFAULT ACCELERATION for all NON printing moves. M204 MXXXX
                 max_jerk[XYZE],       // The largest speed change requiring no acceleration
                 min_travel_feedrate_mm_s;

    #if HAS_LEVELING
      static bool leveling_active;              // Flag that bed leveling is enabled
      #if ABL_PLANAR
        static matrix_3x3 bed_level_matrix; // Transform to compensate for bed level
      #endif
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        static float z_fade_height, inverse_z_fade_height;
      #endif
    #endif

    #if ENABLED(LIN_ADVANCE)
      static float extruder_advance_k, advance_ed_ratio;
    #endif

  private:

    /**
     * The current position of the tool in absolute steps
     * Recalculated if any axis_steps_per_mm are changed by gcode
     */
    static long position[NUM_AXIS];

    /**
     * Speed of previous path line segment
     */
    static float previous_speed[NUM_AXIS];

    /**
     * Nominal speed of previous path line segment
     */
    static float previous_nominal_speed;

    /**
     * Limit where 64bit math is necessary for acceleration calculation
     */
    static uint32_t cutoff_long;

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      static float last_fade_z;
    #endif

    #if ENABLED(DISABLE_INACTIVE_EXTRUDER)
      /**
       * Counters to manage disabling inactive extruders
       */
      static uint8_t g_uc_extruder_last_move[EXTRUDERS];
    #endif // DISABLE_INACTIVE_EXTRUDER

    #ifdef XY_FREQUENCY_LIMIT
      // Used for the frequency limit
      #define MAX_FREQ_TIME_US (uint32_t)(1000000.0 / XY_FREQUENCY_LIMIT)
      // Old direction bits. Used for speed calculations
      static unsigned char old_direction_bits;
      // Segment times (in µs). Used for speed calculations
      static long axis_segment_time_us[2][3];
    #endif

    #if ENABLED(LIN_ADVANCE)
      static float position_float[NUM_AXIS];
    #endif

    #if ENABLED(ULTRA_LCD)
      volatile static uint32_t block_buffer_runtime_us; //Theoretical block buffer runtime in µs
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

    static void reset_acceleration_rates();
    static void refresh_positioning();

    FORCE_INLINE static void refresh_e_factor(const uint8_t e) {
      e_factor[e] = volumetric_multiplier[e] * flow_percentage[e] * 0.01;
    }

    // Manage fans, paste pressure, etc.
    static void check_axes_activity();

    /**
     * Number of moves currently in the planner
     */
    static uint8_t movesplanned() { return BLOCK_MOD(block_buffer_head - block_buffer_tail + BLOCK_BUFFER_SIZE); }

    static bool is_full() { return (block_buffer_tail == BLOCK_MOD(block_buffer_head + 1)); }

    // Update multipliers based on new diameter measurements
    static void calculate_volumetric_multipliers();

    FORCE_INLINE static void set_filament_size(const uint8_t e, const float &v) {
      filament_size[e] = v;
      // make sure all extruders have some sane value for the filament size
      for (uint8_t i = 0; i < COUNT(filament_size); i++)
        if (!filament_size[i]) filament_size[i] = DEFAULT_NOMINAL_FILAMENT_DIA;
    }

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

      /**
       * Get the Z leveling fade factor based on the given Z height,
       * re-calculating only when needed.
       *
       *  Returns 1.0 if planner.z_fade_height is 0.0.
       *  Returns 0.0 if Z is past the specified 'Fade Height'.
       */
      inline static float fade_scaling_factor_for_z(const float &rz) {
        static float z_fade_factor = 1.0;
        if (z_fade_height) {
          if (rz >= z_fade_height) return 0.0;
          if (last_fade_z != rz) {
            last_fade_z = rz;
            z_fade_factor = 1.0 - rz * inverse_z_fade_height;
          }
          return z_fade_factor;
        }
        return 1.0;
      }

      FORCE_INLINE static void force_fade_recalc() { last_fade_z = -999.999; }

      FORCE_INLINE static void set_z_fade_height(const float &zfh) {
        z_fade_height = zfh > 0 ? zfh : 0;
        inverse_z_fade_height = RECIPROCAL(z_fade_height);
        force_fade_recalc();
      }

      FORCE_INLINE static bool leveling_active_at_z(const float &rz) {
        return !z_fade_height || rz < z_fade_height;
      }

    #else

      FORCE_INLINE static float fade_scaling_factor_for_z(const float &rz) {
        UNUSED(rz);
        return 1.0;
      }

      FORCE_INLINE static bool leveling_active_at_z(const float &rz) { UNUSED(rz); return true; }

    #endif

    #if PLANNER_LEVELING

      #define ARG_X float rx
      #define ARG_Y float ry
      #define ARG_Z float rz

      /**
       * Apply leveling to transform a cartesian position
       * as it will be given to the planner and steppers.
       */
      static void apply_leveling(float &rx, float &ry, float &rz);
      static void apply_leveling(float raw[XYZ]) { apply_leveling(raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS]); }
      static void unapply_leveling(float raw[XYZ]);

    #else

      #define ARG_X const float &rx
      #define ARG_Y const float &ry
      #define ARG_Z const float &rz

    #endif

    /**
     * Planner::_buffer_line
     *
     * Add a new direct linear movement to the buffer.
     *
     * Leveling and kinematics should be applied ahead of this.
     *
     *  a,b,c,e   - target position in mm or degrees
     *  fr_mm_s   - (target) speed of the move (mm/s)
     *  extruder  - target extruder
     */
    static void _buffer_line(const float &a, const float &b, const float &c, const float &e, float fr_mm_s, const uint8_t extruder);

    static void _set_position_mm(const float &a, const float &b, const float &c, const float &e);

    /**
     * Add a new linear movement to the buffer.
     * The target is NOT translated to delta/scara
     *
     * Leveling will be applied to input on cartesians.
     * Kinematic machines should call buffer_line_kinematic (for leveled moves).
     * (Cartesians may also call buffer_line_kinematic.)
     *
     *  rx,ry,rz,e   - target position in mm or degrees
     *  fr_mm_s      - (target) speed of the move (mm/s)
     *  extruder     - target extruder
     */
    static FORCE_INLINE void buffer_line(ARG_X, ARG_Y, ARG_Z, const float &e, const float &fr_mm_s, const uint8_t extruder) {
      #if PLANNER_LEVELING && IS_CARTESIAN
        apply_leveling(rx, ry, rz);
      #endif
      _buffer_line(rx, ry, rz, e, fr_mm_s, extruder);
    }

    /**
     * Add a new linear movement to the buffer.
     * The target is cartesian, it's translated to delta/scara if
     * needed.
     *
     *  cart     - x,y,z,e CARTESIAN target in mm
     *  fr_mm_s  - (target) speed of the move (mm/s)
     *  extruder - target extruder
     */
    static FORCE_INLINE void buffer_line_kinematic(const float cart[XYZE], const float &fr_mm_s, const uint8_t extruder) {
      #if PLANNER_LEVELING
        float raw[XYZ] = { cart[X_AXIS], cart[Y_AXIS], cart[Z_AXIS] };
        apply_leveling(raw);
      #else
        const float * const raw = cart;
      #endif
      #if IS_KINEMATIC
        inverse_kinematics(raw);
        _buffer_line(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], cart[E_AXIS], fr_mm_s, extruder);
      #else
        _buffer_line(raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS], cart[E_AXIS], fr_mm_s, extruder);
      #endif
    }

    /**
     * Set the planner.position and individual stepper positions.
     * Used by G92, G28, G29, and other procedures.
     *
     * Multiplies by axis_steps_per_mm[] and does necessary conversion
     * for COREXY / COREXZ / COREYZ to set the corresponding stepper positions.
     *
     * Clears previous speed values.
     */
    static FORCE_INLINE void set_position_mm(ARG_X, ARG_Y, ARG_Z, const float &e) {
      #if PLANNER_LEVELING && IS_CARTESIAN
        apply_leveling(rx, ry, rz);
      #endif
      _set_position_mm(rx, ry, rz, e);
    }
    static void set_position_mm_kinematic(const float position[NUM_AXIS]);
    static void set_position_mm(const AxisEnum axis, const float &v);
    static FORCE_INLINE void set_z_position_mm(const float &z) { set_position_mm(Z_AXIS, z); }
    static FORCE_INLINE void set_e_position_mm(const float &e) { set_position_mm(AxisEnum(E_AXIS), e); }

    /**
     * Sync from the stepper positions. (e.g., after an interrupted move)
     */
    static void sync_from_steppers();

    /**
     * Does the buffer have any blocks queued?
     */
    static bool blocks_queued() { return (block_buffer_head != block_buffer_tail); }

    /**
     * "Discards" the block and "releases" the memory.
     * Called when the current block is no longer needed.
     */
    static void discard_current_block() {
      if (blocks_queued())
        block_buffer_tail = BLOCK_MOD(block_buffer_tail + 1);
    }

    /**
     * The current block. NULL if the buffer is empty.
     * This also marks the block as busy.
     */
    static block_t* get_current_block() {
      if (blocks_queued()) {
        block_t* block = &block_buffer[block_buffer_tail];
        #if ENABLED(ULTRA_LCD)
          block_buffer_runtime_us -= block->segment_time_us; //We can't be sure how long an active block will take, so don't count it.
        #endif
        SBI(block->flag, BLOCK_BIT_BUSY);
        return block;
      }
      else {
        #if ENABLED(ULTRA_LCD)
          clear_block_buffer_runtime(); // paranoia. Buffer is empty now - so reset accumulated time to zero.
        #endif
        return NULL;
      }
    }

    #if ENABLED(ULTRA_LCD)

      static uint16_t block_buffer_runtime() {
        CRITICAL_SECTION_START
          millis_t bbru = block_buffer_runtime_us;
        CRITICAL_SECTION_END
        // To translate µs to ms a division by 1000 would be required.
        // We introduce 2.4% error here by dividing by 1024.
        // Doesn't matter because block_buffer_runtime_us is already too small an estimation.
        bbru >>= 10;
        // limit to about a minute.
        NOMORE(bbru, 0xFFFFul);
        return bbru;
      }

      static void clear_block_buffer_runtime(){
        CRITICAL_SECTION_START
          block_buffer_runtime_us = 0;
        CRITICAL_SECTION_END
      }

    #endif

    #if ENABLED(AUTOTEMP)
      static float autotemp_min, autotemp_max, autotemp_factor;
      static bool autotemp_enabled;
      static void getHighESpeed();
      static void autotemp_M104_M109();
    #endif

  private:

    /**
     * Get the index of the next / previous block in the ring buffer
     */
    static int8_t next_block_index(int8_t block_index) { return BLOCK_MOD(block_index + 1); }
    static int8_t prev_block_index(int8_t block_index) { return BLOCK_MOD(block_index - 1); }

    /**
     * Calculate the distance (not time) it takes to accelerate
     * from initial_rate to target_rate using the given acceleration:
     */
    static float estimate_acceleration_distance(const float &initial_rate, const float &target_rate, const float &accel) {
      if (accel == 0) return 0; // accel was 0, set acceleration distance to 0
      return (sq(target_rate) - sq(initial_rate)) / (accel * 2);
    }

    /**
     * Return the point at which you must start braking (at the rate of -'acceleration') if
     * you start at 'initial_rate', accelerate (until reaching the point), and want to end at
     * 'final_rate' after traveling 'distance'.
     *
     * This is used to compute the intersection point between acceleration and deceleration
     * in cases where the "trapezoid" has no plateau (i.e., never reaches maximum speed)
     */
    static float intersection_distance(const float &initial_rate, const float &final_rate, const float &accel, const float &distance) {
      if (accel == 0) return 0; // accel was 0, set intersection distance to 0
      return (accel * 2 * distance - sq(initial_rate) + sq(final_rate)) / (accel * 4);
    }

    /**
     * Calculate the maximum allowable speed at this point, in order
     * to reach 'target_velocity' using 'acceleration' within a given
     * 'distance'.
     */
    static float max_allowable_speed(const float &accel, const float &target_velocity, const float &distance) {
      return SQRT(sq(target_velocity) - 2 * accel * distance);
    }

    static void calculate_trapezoid_for_block(block_t* const block, const float &entry_factor, const float &exit_factor);

    static void reverse_pass_kernel(block_t* const current, const block_t *next);
    static void forward_pass_kernel(const block_t *previous, block_t* const current);

    static void reverse_pass();
    static void forward_pass();

    static void recalculate_trapezoids();

    static void recalculate();

};

#define PLANNER_XY_FEEDRATE() (min(planner.max_feedrate_mm_s[X_AXIS], planner.max_feedrate_mm_s[Y_AXIS]))

extern Planner planner;

#endif // PLANNER_H
