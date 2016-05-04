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

#include "Marlin.h"

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

  long accelerate_until;                    // The index of the step event on which to stop acceleration
  long decelerate_after;                    // The index of the step event on which to start decelerating
  long acceleration_rate;                   // The acceleration rate used for acceleration calculation

  unsigned char direction_bits;             // The direction bit set for this block (refers to *_DIRECTION_BIT in config.h)

  #if ENABLED(ADVANCE)
    long advance_rate;
    volatile long initial_advance;
    volatile long final_advance;
    float advance;
  #endif

  // Fields used by the motion planner to manage acceleration
  float nominal_speed;                               // The nominal speed for this block in mm/sec
  float entry_speed;                                 // Entry speed at previous-current junction in mm/sec
  float max_entry_speed;                             // Maximum allowable junction entry speed in mm/sec
  float millimeters;                                 // The total travel of this block in mm
  float acceleration;                                // acceleration mm/sec^2
  unsigned char recalculate_flag;                    // Planner flag to recalculate trapezoids on entry junction
  unsigned char nominal_length_flag;                 // Planner flag for nominal speed always reached

  // Settings for the trapezoid generator
  unsigned long nominal_rate;                        // The nominal step rate for this block in step_events/sec
  unsigned long initial_rate;                        // The jerk-adjusted step rate at start of block
  unsigned long final_rate;                          // The minimal rate at exit
  unsigned long acceleration_st;                     // acceleration steps/sec^2

  #if FAN_COUNT > 0
    unsigned long fan_speed[FAN_COUNT];
  #endif

  #if ENABLED(BARICUDA)
    unsigned long valve_pressure;
    unsigned long e_to_p_pressure;
  #endif

  volatile char busy;

} block_t;

#define BLOCK_MOD(n) ((n)&(BLOCK_BUFFER_SIZE-1))

class Planner {

  public:

    /**
     * A ring buffer of moves described in steps
     */
    block_t block_buffer[BLOCK_BUFFER_SIZE];
    volatile uint8_t block_buffer_head = 0;           // Index of the next block to be pushed
    volatile uint8_t block_buffer_tail = 0;

    float max_feedrate[NUM_AXIS]; // Max speeds in mm per minute
    float axis_steps_per_unit[NUM_AXIS];
    unsigned long axis_steps_per_sqr_second[NUM_AXIS];
    unsigned long max_acceleration_units_per_sq_second[NUM_AXIS]; // Use M201 to override by software

    millis_t min_segment_time;
    float min_feedrate;
    float acceleration;         // Normal acceleration mm/s^2  DEFAULT ACCELERATION for all printing moves. M204 SXXXX
    float retract_acceleration; // Retract acceleration mm/s^2 filament pull-back and push-forward while standing still in the other axes M204 TXXXX
    float travel_acceleration;  // Travel acceleration mm/s^2  DEFAULT ACCELERATION for all NON printing moves. M204 MXXXX
    float max_xy_jerk;          // The largest speed change requiring no acceleration
    float max_z_jerk;
    float max_e_jerk;
    float min_travel_feedrate;

    #if ENABLED(AUTO_BED_LEVELING_FEATURE)
      matrix_3x3 bed_level_matrix; // Transform to compensate for bed level
    #endif

  private:

    /**
     * The current position of the tool in absolute steps
     * Reclculated if any axis_steps_per_unit are changed by gcode
     */
    long position[NUM_AXIS] = { 0 };

    /**
     * Speed of previous path line segment
     */
    float previous_speed[NUM_AXIS];

    /**
     * Nominal speed of previous path line segment
     */
    float previous_nominal_speed;

    #if ENABLED(DISABLE_INACTIVE_EXTRUDER)
      /**
       * Counters to manage disabling inactive extruders
       */
      uint8_t g_uc_extruder_last_move[EXTRUDERS] = { 0 };
    #endif // DISABLE_INACTIVE_EXTRUDER

    #ifdef XY_FREQUENCY_LIMIT
      // Used for the frequency limit
      #define MAX_FREQ_TIME (1000000.0/XY_FREQUENCY_LIMIT)
      // Old direction bits. Used for speed calculations
      static unsigned char old_direction_bits = 0;
      // Segment times (in µs). Used for speed calculations
      static long axis_segment_time[2][3] = { {MAX_FREQ_TIME + 1, 0, 0}, {MAX_FREQ_TIME + 1, 0, 0} };
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      extern bool extruder_duplication_enabled;
    #endif

  public:

    Planner();

    void init();

    void reset_acceleration_rates();

    // Manage fans, paste pressure, etc.
    void check_axes_activity();

    /**
     * Number of moves currently in the planner
     */
    FORCE_INLINE uint8_t movesplanned() { return BLOCK_MOD(block_buffer_head - block_buffer_tail + BLOCK_BUFFER_SIZE); }

    #if ENABLED(AUTO_BED_LEVELING_FEATURE) || ENABLED(MESH_BED_LEVELING)

      #if ENABLED(AUTO_BED_LEVELING_FEATURE)
        /**
         * The corrected position, applying the bed level matrix
         */
        vector_3 adjusted_position();
      #endif

      /**
       * Add a new linear movement to the buffer.
       *
       *  x,y,z,e   - target position in mm
       *  feed_rate - (target) speed of the move
       *  extruder  - target extruder
       */
      void buffer_line(float x, float y, float z, const float& e, float feed_rate, const uint8_t extruder);

      /**
       * Set the planner.position and individual stepper positions.
       * Used by G92, G28, G29, and other procedures.
       *
       * Multiplies by axis_steps_per_unit[] and does necessary conversion
       * for COREXY / COREXZ to set the corresponding stepper positions.
       *
       * Clears previous speed values.
       */
      void set_position(float x, float y, float z, const float& e);

    #else

      void buffer_line(const float& x, const float& y, const float& z, const float& e, float feed_rate, const uint8_t extruder);
      void set_position(const float& x, const float& y, const float& z, const float& e);

    #endif // AUTO_BED_LEVELING_FEATURE || MESH_BED_LEVELING

    /**
     * Set the E position (mm) of the planner (and the E stepper)
     */
    void set_e_position(const float& e);

    /**
     * Does the buffer have any blocks queued?
     */
    FORCE_INLINE bool blocks_queued() { return (block_buffer_head != block_buffer_tail); }

    /**
     * "Discards" the block and "releases" the memory.
     * Called when the current block is no longer needed.
     */
    FORCE_INLINE void discard_current_block() {
      if (blocks_queued())
        block_buffer_tail = BLOCK_MOD(block_buffer_tail + 1);
    }

    /**
     * The current block. NULL if the buffer is empty.
     * This also marks the block as busy.
     */
    FORCE_INLINE block_t* get_current_block() {
      if (blocks_queued()) {
        block_t* block = &block_buffer[block_buffer_tail];
        block->busy = true;
        return block;
      }
      else
        return NULL;
    }

    #if ENABLED(AUTOTEMP)
      float autotemp_max = 250;
      float autotemp_min = 210;
      float autotemp_factor = 0.1;
      bool autotemp_enabled = false;
      void getHighESpeed();
      void autotemp_M109();
    #endif

  private:

    /**
     * Get the index of the next / previous block in the ring buffer
     */
    FORCE_INLINE int8_t next_block_index(int8_t block_index) { return BLOCK_MOD(block_index + 1); }
    FORCE_INLINE int8_t prev_block_index(int8_t block_index) { return BLOCK_MOD(block_index - 1); }

    /**
     * Calculate the distance (not time) it takes to accelerate
     * from initial_rate to target_rate using the given acceleration:
     */
    FORCE_INLINE float estimate_acceleration_distance(float initial_rate, float target_rate, float acceleration) {
      if (acceleration == 0) return 0; // acceleration was 0, set acceleration distance to 0
      return (target_rate * target_rate - initial_rate * initial_rate) / (acceleration * 2);
    }

    /**
     * Return the point at which you must start braking (at the rate of -'acceleration') if
     * you start at 'initial_rate', accelerate (until reaching the point), and want to end at
     * 'final_rate' after traveling 'distance'.
     *
     * This is used to compute the intersection point between acceleration and deceleration
     * in cases where the "trapezoid" has no plateau (i.e., never reaches maximum speed)
     */
    FORCE_INLINE float intersection_distance(float initial_rate, float final_rate, float acceleration, float distance) {
      if (acceleration == 0) return 0; // acceleration was 0, set intersection distance to 0
      return (acceleration * 2 * distance - initial_rate * initial_rate + final_rate * final_rate) / (acceleration * 4);
    }

    /**
     * Calculate the maximum allowable speed at this point, in order
     * to reach 'target_velocity' using 'acceleration' within a given
     * 'distance'.
     */
    FORCE_INLINE float max_allowable_speed(float acceleration, float target_velocity, float distance) {
      return sqrt(target_velocity * target_velocity - 2 * acceleration * distance);
    }

    void calculate_trapezoid_for_block(block_t* block, float entry_factor, float exit_factor);

    void reverse_pass_kernel(block_t* previous, block_t* current, block_t* next);
    void forward_pass_kernel(block_t* previous, block_t* current, block_t* next);

    void reverse_pass();
    void forward_pass();

    void recalculate_trapezoids();

    void recalculate();

};

#endif // PLANNER_H
