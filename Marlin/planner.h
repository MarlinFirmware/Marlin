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

#include "types.h"
#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_FEATURE)
  #include "vector_3.h"
#endif

class Planner;
extern Planner planner;

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

  unsigned char active_extruder;            // The extruder to move (if E move)

  // Fields used by the bresenham algorithm for tracing the line
  long steps[NUM_AXIS];                     // Step count along each axis
  unsigned long step_event_count;           // The number of step events required to complete this block

  #if ENABLED(MIXING_EXTRUDER)
    unsigned long mix_event_count[MIXING_STEPPERS]; // Scaled step_event_count for the mixing steppers
  #endif

  long accelerate_until,                    // The index of the step event on which to stop acceleration
       decelerate_after,                    // The index of the step event on which to start decelerating
       acceleration_rate;                   // The acceleration rate used for acceleration calculation

  unsigned char direction_bits;             // The direction bit set for this block (refers to *_DIRECTION_BIT in config.h)

  // Advance extrusion
  #if ENABLED(LIN_ADVANCE)
    bool use_advance_lead;
    int e_speed_multiplier8; // Factorised by 2^8 to avoid float
  #elif ENABLED(ADVANCE)
    long advance_rate;
    volatile long initial_advance;
    volatile long final_advance;
    float advance;
  #endif

  // Fields used by the motion planner to manage acceleration
  float nominal_speed,                               // The nominal speed for this block in mm/sec
        entry_speed,                                 // Entry speed at previous-current junction in mm/sec
        max_entry_speed,                             // Maximum allowable junction entry speed in mm/sec
        millimeters,                                 // The total travel of this block in mm
        acceleration;                                // acceleration mm/sec^2
  unsigned char recalculate_flag,                    // Planner flag to recalculate trapezoids on entry junction
                nominal_length_flag;                 // Planner flag for nominal speed always reached

  // Settings for the trapezoid generator
  unsigned long nominal_rate,                        // The nominal step rate for this block in step_events/sec
                initial_rate,                        // The jerk-adjusted step rate at start of block
                final_rate,                          // The minimal rate at exit
                acceleration_steps_per_s2;           // acceleration steps/sec^2

  #if FAN_COUNT > 0
    unsigned long fan_speed[FAN_COUNT];
  #endif

  #if ENABLED(BARICUDA)
    unsigned long valve_pressure, e_to_p_pressure;
  #endif

  volatile char busy;

} block_t;

#define BLOCK_MOD(n) ((n)&(BLOCK_BUFFER_SIZE-1))

class Planner {

  public:

    /**
     * A ring buffer of moves described in steps
     */
    static block_t block_buffer[BLOCK_BUFFER_SIZE];
    static volatile uint8_t block_buffer_head;           // Index of the next block to be pushed
    static volatile uint8_t block_buffer_tail;

    static float max_feedrate_mm_s[NUM_AXIS]; // Max speeds in mm per second
    static float axis_steps_per_mm[NUM_AXIS];
    static float steps_to_mm[NUM_AXIS];
    static unsigned long max_acceleration_steps_per_s2[NUM_AXIS];
    static unsigned long max_acceleration_mm_per_s2[NUM_AXIS]; // Use M201 to override by software

    static millis_t min_segment_time;
    static float min_feedrate_mm_s;
    static float acceleration;         // Normal acceleration mm/s^2  DEFAULT ACCELERATION for all printing moves. M204 SXXXX
    static float retract_acceleration; // Retract acceleration mm/s^2 filament pull-back and push-forward while standing still in the other axes M204 TXXXX
    static float travel_acceleration;  // Travel acceleration mm/s^2  DEFAULT ACCELERATION for all NON printing moves. M204 MXXXX
    static float max_xy_jerk;          // The largest speed change requiring no acceleration
    static float max_z_jerk;
    static float max_e_jerk;
    static float min_travel_feedrate_mm_s;

    #if ENABLED(AUTO_BED_LEVELING_FEATURE)
      static matrix_3x3 bed_level_matrix; // Transform to compensate for bed level
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

    #if ENABLED(DISABLE_INACTIVE_EXTRUDER)
      /**
       * Counters to manage disabling inactive extruders
       */
      static uint8_t g_uc_extruder_last_move[EXTRUDERS];
    #endif // DISABLE_INACTIVE_EXTRUDER

    #ifdef XY_FREQUENCY_LIMIT
      // Used for the frequency limit
      #define MAX_FREQ_TIME long(1000000.0/XY_FREQUENCY_LIMIT)
      // Old direction bits. Used for speed calculations
      static unsigned char old_direction_bits;
      // Segment times (in µs). Used for speed calculations
      static long axis_segment_time[2][3];
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

    // Manage fans, paste pressure, etc.
    static void check_axes_activity();

    /**
     * Number of moves currently in the planner
     */
    static uint8_t movesplanned() { return BLOCK_MOD(block_buffer_head - block_buffer_tail + BLOCK_BUFFER_SIZE); }

    static bool is_full() { return (block_buffer_tail == BLOCK_MOD(block_buffer_head + 1)); }

    #if ENABLED(AUTO_BED_LEVELING_FEATURE) || ENABLED(MESH_BED_LEVELING)

      #if ENABLED(AUTO_BED_LEVELING_FEATURE)
        /**
         * The corrected position, applying the bed level matrix
         */
        static vector_3 adjusted_position();
      #endif

      /**
       * Add a new linear movement to the buffer.
       *
       *  x,y,z,e   - target position in mm
       *  fr_mm_s   - (target) speed of the move (mm/s)
       *  extruder  - target extruder
       */
      static void buffer_line(float x, float y, float z, const float& e, float fr_mm_s, const uint8_t extruder);

      /**
       * Set the planner.position and individual stepper positions.
       * Used by G92, G28, G29, and other procedures.
       *
       * Multiplies by axis_steps_per_mm[] and does necessary conversion
       * for COREXY / COREXZ / COREYZ to set the corresponding stepper positions.
       *
       * Clears previous speed values.
       */
      static void set_position_mm(float x, float y, float z, const float& e);

    #else

      static void buffer_line(const float& x, const float& y, const float& z, const float& e, float fr_mm_s, const uint8_t extruder);
      static void set_position_mm(const float& x, const float& y, const float& z, const float& e);

    #endif // AUTO_BED_LEVELING_FEATURE || MESH_BED_LEVELING

    /**
     * Set the E position (mm) of the planner (and the E stepper)
     */
    static void set_e_position_mm(const float& e);

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
        block->busy = true;
        return block;
      }
      else
        return NULL;
    }

    #if ENABLED(AUTOTEMP)
      static float autotemp_max;
      static float autotemp_min;
      static float autotemp_factor;
      static bool autotemp_enabled;
      static void getHighESpeed();
      static void autotemp_M109();
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
    static float estimate_acceleration_distance(float initial_rate, float target_rate, float accel) {
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
    static float intersection_distance(float initial_rate, float final_rate, float accel, float distance) {
      if (accel == 0) return 0; // accel was 0, set intersection distance to 0
      return (accel * 2 * distance - sq(initial_rate) + sq(final_rate)) / (accel * 4);
    }

    /**
     * Calculate the maximum allowable speed at this point, in order
     * to reach 'target_velocity' using 'acceleration' within a given
     * 'distance'.
     */
    static float max_allowable_speed(float accel, float target_velocity, float distance) {
      return sqrt(sq(target_velocity) - 2 * accel * distance);
    }

    static void calculate_trapezoid_for_block(block_t* block, float entry_factor, float exit_factor);

    static void reverse_pass_kernel(block_t* previous, block_t* current, block_t* next);
    static void forward_pass_kernel(block_t* previous, block_t* current, block_t* next);

    static void reverse_pass();
    static void forward_pass();

    static void recalculate_trapezoids();

    static void recalculate();

};

#endif // PLANNER_H
