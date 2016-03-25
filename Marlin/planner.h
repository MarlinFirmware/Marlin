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
  planner.h - buffers movement commands and manages the acceleration profile plan
  Part of Grbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

// This module is to be considered a sub-module of stepper.c. Please don't include
// this file from any other module.

#ifndef PLANNER_H
#define PLANNER_H

#include "Marlin.h"

// This struct is used when buffering the setup for each linear movement "nominal" values are as specified in
// the source g-code and may never actually be reached if acceleration management is active.
typedef struct {
  // Fields used by the bresenham algorithm for tracing the line
  long steps[NUM_AXIS];                     // Step count along each axis
  unsigned long step_event_count;           // The number of step events required to complete this block
  long accelerate_until;                    // The index of the step event on which to stop acceleration
  long decelerate_after;                    // The index of the step event on which to start decelerating
  long acceleration_rate;                   // The acceleration rate used for acceleration calculation
  unsigned char direction_bits;             // The direction bit set for this block (refers to *_DIRECTION_BIT in config.h)
  unsigned char active_extruder;            // Selects the active extruder
  #if ENABLED(ADVANCE)
    long advance_rate;
    volatile long initial_advance;
    volatile long final_advance;
    float advance;
  #endif

  // Fields used by the motion planner to manage acceleration
  // float speed_x, speed_y, speed_z, speed_e;          // Nominal mm/sec for each axis
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

// Initialize the motion plan subsystem
void plan_init();

void check_axes_activity();

// Get the number of buffered moves
extern volatile unsigned char block_buffer_head;
extern volatile unsigned char block_buffer_tail;
FORCE_INLINE uint8_t movesplanned() { return BLOCK_MOD(block_buffer_head - block_buffer_tail + BLOCK_BUFFER_SIZE); }

#if ENABLED(AUTO_BED_LEVELING_FEATURE) || ENABLED(MESH_BED_LEVELING)

  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    #include "vector_3.h"

    // Transform required to compensate for bed level
    extern matrix_3x3 plan_bed_level_matrix;

    /**
     * Get the position applying the bed level matrix
     */
    vector_3 plan_get_position();
  #endif  // AUTO_BED_LEVELING_FEATURE

  /**
   * Add a new linear movement to the buffer. x, y, z are the signed, absolute target position in
   * millimeters. Feed rate specifies the (target) speed of the motion.
   */
  void plan_buffer_line(float x, float y, float z, const float& e, float feed_rate, const uint8_t extruder);

  /**
   * Set the planner positions. Used for G92 instructions.
   * Multiplies by axis_steps_per_unit[] to set stepper positions.
   * Clears previous speed values.
   */
  void plan_set_position(float x, float y, float z, const float& e);

#else

  void plan_buffer_line(const float& x, const float& y, const float& z, const float& e, float feed_rate, const uint8_t extruder);
  void plan_set_position(const float& x, const float& y, const float& z, const float& e);

#endif // AUTO_BED_LEVELING_FEATURE || MESH_BED_LEVELING

void plan_set_e_position(const float& e);

//===========================================================================
//============================= public variables ============================
//===========================================================================

extern millis_t minsegmenttime;
extern float max_feedrate[NUM_AXIS]; // Max speeds in mm per minute
extern float axis_steps_per_unit[NUM_AXIS];
extern unsigned long max_acceleration_units_per_sq_second[NUM_AXIS]; // Use M201 to override by software
extern float minimumfeedrate;
extern float acceleration;         // Normal acceleration mm/s^2  DEFAULT ACCELERATION for all printing moves. M204 SXXXX
extern float retract_acceleration; // Retract acceleration mm/s^2 filament pull-back and push-forward while standing still in the other axes M204 TXXXX
extern float travel_acceleration;  // Travel acceleration mm/s^2  DEFAULT ACCELERATION for all NON printing moves. M204 MXXXX
extern float max_xy_jerk;          // The largest speed change requiring no acceleration
extern float max_z_jerk;
extern float max_e_jerk;
extern float mintravelfeedrate;
extern unsigned long axis_steps_per_sqr_second[NUM_AXIS];

#if ENABLED(AUTOTEMP)
  extern bool autotemp_enabled;
  extern float autotemp_max;
  extern float autotemp_min;
  extern float autotemp_factor;
#endif

extern block_t block_buffer[BLOCK_BUFFER_SIZE];            // A ring buffer for motion instructions
extern volatile unsigned char block_buffer_head;           // Index of the next block to be pushed
extern volatile unsigned char block_buffer_tail;

// Returns true if the buffer has a queued block, false otherwise
FORCE_INLINE bool blocks_queued() { return (block_buffer_head != block_buffer_tail); }

// Called when the current block is no longer needed. Discards
// the block and makes the memory available for new blocks.
FORCE_INLINE void plan_discard_current_block() {
  if (blocks_queued())
    block_buffer_tail = BLOCK_MOD(block_buffer_tail + 1);
}

// Gets the current block. Returns NULL if buffer empty
FORCE_INLINE block_t* plan_get_current_block() {
  if (blocks_queued()) {
    block_t* block = &block_buffer[block_buffer_tail];
    block->busy = true;
    return block;
  }
  else
    return NULL;
}

void reset_acceleration_rates();

#endif // PLANNER_H
