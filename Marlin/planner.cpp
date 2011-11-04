/*
  planner.c - buffers movement commands and manages the acceleration profile plan
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

/* The ring buffer implementation gleaned from the wiring_serial library by David A. Mellis. */

/*  
  Reasoning behind the mathematics in this module (in the key of 'Mathematica'):
  
  s == speed, a == acceleration, t == time, d == distance

  Basic definitions:

    Speed[s_, a_, t_] := s + (a*t) 
    Travel[s_, a_, t_] := Integrate[Speed[s, a, t], t]

  Distance to reach a specific speed with a constant acceleration:

    Solve[{Speed[s, a, t] == m, Travel[s, a, t] == d}, d, t]
      d -> (m^2 - s^2)/(2 a) --> estimate_acceleration_distance()

  Speed after a given distance of travel with constant acceleration:

    Solve[{Speed[s, a, t] == m, Travel[s, a, t] == d}, m, t]
      m -> Sqrt[2 a d + s^2]    

    DestinationSpeed[s_, a_, d_] := Sqrt[2 a d + s^2]

  When to start braking (di) to reach a specified destionation speed (s2) after accelerating
  from initial speed s1 without ever stopping at a plateau:

    Solve[{DestinationSpeed[s1, a, di] == DestinationSpeed[s2, a, d - di]}, di]
      di -> (2 a d - s1^2 + s2^2)/(4 a) --> intersection_distance()

    IntersectionDistance[s1_, s2_, a_, d_] := (2 a d - s1^2 + s2^2)/(4 a)
*/
                                                                                                            

//#include <inttypes.h>
//#include <math.h>       
//#include <stdlib.h>

#include "Marlin.h"
#include "Configuration.h"
#include "pins.h"
#include "fastio.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "ultralcd.h"

unsigned long minsegmenttime;
float max_feedrate[4]; // set the max speeds
float axis_steps_per_unit[4];
long max_acceleration_units_per_sq_second[4]; // Use M201 to override by software
float minimumfeedrate;
float acceleration;         // Normal acceleration mm/s^2  THIS IS THE DEFAULT ACCELERATION for all moves. M204 SXXXX
float retract_acceleration; //  mm/s^2   filament pull-pack and push-forward  while standing still in the other axis M204 TXXXX
float max_xy_jerk; //speed than can be stopped at once, if i understand correctly.
float max_z_jerk;
float mintravelfeedrate;
unsigned long axis_steps_per_sqr_second[NUM_AXIS];
// Manage heater variables.

static block_t block_buffer[BLOCK_BUFFER_SIZE];            // A ring buffer for motion instfructions
static volatile unsigned char block_buffer_head;           // Index of the next block to be pushed
static volatile unsigned char block_buffer_tail;           // Index of the block to process now

// The current position of the tool in absolute steps
 long position[4];   

#define ONE_MINUTE_OF_MICROSECONDS 60000000.0

// Calculates the distance (not time) it takes to accelerate from initial_rate to target_rate using the 
// given acceleration:
inline float estimate_acceleration_distance(float initial_rate, float target_rate, float acceleration) {
  if (acceleration!=0) {
  return((target_rate*target_rate-initial_rate*initial_rate)/
         (2.0*acceleration));
  }
  else {
    return 0.0;  // acceleration was 0, set acceleration distance to 0
  }
}

// This function gives you the point at which you must start braking (at the rate of -acceleration) if 
// you started at speed initial_rate and accelerated until this point and want to end at the final_rate after
// a total travel of distance. This can be used to compute the intersection point between acceleration and
// deceleration in the cases where the trapezoid has no plateau (i.e. never reaches maximum speed)

inline float intersection_distance(float initial_rate, float final_rate, float acceleration, float distance) {
 if (acceleration!=0) {
  return((2.0*acceleration*distance-initial_rate*initial_rate+final_rate*final_rate)/
         (4.0*acceleration) );
  }
  else {
    return 0.0;  // acceleration was 0, set intersection distance to 0
  }
}

// Calculates trapezoid parameters so that the entry- and exit-speed is compensated by the provided factors.

void calculate_trapezoid_for_block(block_t *block, float entry_speed, float exit_speed) {
  if(block->busy == true) return; // If block is busy then bail out.
  float entry_factor = entry_speed / block->nominal_speed;
  float exit_factor = exit_speed / block->nominal_speed;
  long initial_rate = ceil(block->nominal_rate*entry_factor);
  long final_rate = ceil(block->nominal_rate*exit_factor);
  
#ifdef ADVANCE
  long initial_advance = block->advance*entry_factor*entry_factor;
  long final_advance = block->advance*exit_factor*exit_factor;
#endif // ADVANCE

  // Limit minimal step rate (Otherwise the timer will overflow.)
  if(initial_rate <120) initial_rate=120;
  if(final_rate < 120) final_rate=120;
  
  // Calculate the acceleration steps
  long acceleration = block->acceleration_st;
  long accelerate_steps = estimate_acceleration_distance(initial_rate, block->nominal_rate, acceleration);
  long decelerate_steps = estimate_acceleration_distance(final_rate, block->nominal_rate, acceleration);
  // Calculate the size of Plateau of Nominal Rate. 
  long plateau_steps = block->step_event_count-accelerate_steps-decelerate_steps;

  // Is the Plateau of Nominal Rate smaller than nothing? That means no cruising, and we will
  // have to use intersection_distance() to calculate when to abort acceleration and start braking 
  // in order to reach the final_rate exactly at the end of this block.
  if (plateau_steps < 0) {  
    accelerate_steps = intersection_distance(initial_rate, final_rate, acceleration, block->step_event_count);
    plateau_steps = 0;
  }  

  long decelerate_after = accelerate_steps+plateau_steps;

  CRITICAL_SECTION_START;  // Fill variables used by the stepper in a critical section
  if(block->busy == false) { // Don't update variables if block is busy.
    block->accelerate_until = accelerate_steps;
    block->decelerate_after = decelerate_after;
    block->initial_rate = initial_rate;
    block->final_rate = final_rate;
#ifdef ADVANCE
    block->initial_advance = initial_advance;
    block->final_advance = final_advance;
#endif //ADVANCE
  }
  CRITICAL_SECTION_END;
}                    

// Calculates the maximum allowable speed at this point when you must be able to reach target_velocity using the 
// acceleration within the allotted distance.
inline float max_allowable_speed(float acceleration, float target_velocity, float distance) {
  return(
  sqrt(target_velocity*target_velocity-2*acceleration*60*60*distance)
    );
}

// "Junction jerk" in this context is the immediate change in speed at the junction of two blocks.
// This method will calculate the junction jerk as the euclidean distance between the nominal 
// velocities of the respective blocks.
inline float junction_jerk(block_t *before, block_t *after) {
  return(sqrt(
    pow((before->speed_x-after->speed_x), 2)+
    pow((before->speed_y-after->speed_y), 2)));
}

// Return the safe speed which is max_jerk/2, e.g. the 
// speed under which you cannot exceed max_jerk no matter what you do.
float safe_speed(block_t *block) {
  float safe_speed;
  safe_speed = max_xy_jerk/2;  
  if(abs(block->speed_z) > max_z_jerk/2) safe_speed = max_z_jerk/2;
  if (safe_speed > block->nominal_speed) safe_speed = block->nominal_speed;
  return safe_speed;  
}

// The kernel called by planner_recalculate() when scanning the plan from last to first entry.
void planner_reverse_pass_kernel(block_t *previous, block_t *current, block_t *next) {
  if(!current) { 
    return; 
  }

  float entry_speed = current->nominal_speed;
  float exit_factor;
  float exit_speed;
  if (next) {
    exit_speed = next->entry_speed;
  } 
  else {
    exit_speed = safe_speed(current);
  }

  // Calculate the entry_factor for the current block. 
  if (previous) {
    // Reduce speed so that junction_jerk is within the maximum allowed
    float jerk = junction_jerk(previous, current);
    if((previous->steps_x == 0) && (previous->steps_y == 0)) {
      entry_speed = safe_speed(current);
    }
    else if (jerk > max_xy_jerk) {
      entry_speed = (max_xy_jerk/jerk) * entry_speed;
    } 
    if(abs(previous->speed_z - current->speed_z) > max_z_jerk) {
      entry_speed = (max_z_jerk/abs(previous->speed_z - current->speed_z)) * entry_speed;
    } 
    // If the required deceleration across the block is too rapid, reduce the entry_factor accordingly.
    if (entry_speed > exit_speed) {
      float max_entry_speed = max_allowable_speed(-current->acceleration,exit_speed, current->millimeters);
      if (max_entry_speed < entry_speed) {
        entry_speed = max_entry_speed;
      }
    }    
  } 
  else {
    entry_speed = safe_speed(current);
  }
  // Store result
  current->entry_speed = entry_speed;
}

// planner_recalculate() needs to go over the current plan twice. Once in reverse and once forward. This 
// implements the reverse pass.
void planner_reverse_pass() {
  char block_index = block_buffer_head;
  if(((block_buffer_head-block_buffer_tail + BLOCK_BUFFER_SIZE) & (BLOCK_BUFFER_SIZE - 1)) > 3) {
    block_index = (block_buffer_head - 3) & (BLOCK_BUFFER_SIZE - 1);
    block_t *block[5] = {
      NULL, NULL, NULL, NULL, NULL  };
    while(block_index != block_buffer_tail) { 
      block_index = (block_index-1) & (BLOCK_BUFFER_SIZE -1); 
      block[2]= block[1];
      block[1]= block[0];
      block[0] = &block_buffer[block_index];
      planner_reverse_pass_kernel(block[0], block[1], block[2]);
    }
    planner_reverse_pass_kernel(NULL, block[0], block[1]);
  }
}

// The kernel called by planner_recalculate() when scanning the plan from first to last entry.
void planner_forward_pass_kernel(block_t *previous, block_t *current, block_t *next) {
  if(!current) { 
    return; 
  }
  if(previous) {
    // If the previous block is an acceleration block, but it is not long enough to 
    // complete the full speed change within the block, we need to adjust out entry
    // speed accordingly. Remember current->entry_factor equals the exit factor of 
    // the previous block.
    if(previous->entry_speed < current->entry_speed) {
      float max_entry_speed = max_allowable_speed(-previous->acceleration, previous->entry_speed, previous->millimeters);
      if (max_entry_speed < current->entry_speed) {
        current->entry_speed = max_entry_speed;
      }
    }
  }
}

// planner_recalculate() needs to go over the current plan twice. Once in reverse and once forward. This 
// implements the forward pass.
void planner_forward_pass() {
  char block_index = block_buffer_tail;
  block_t *block[3] = {
    NULL, NULL, NULL  };

  while(block_index != block_buffer_head) {
    block[0] = block[1];
    block[1] = block[2];
    block[2] = &block_buffer[block_index];
    planner_forward_pass_kernel(block[0],block[1],block[2]);
    block_index = (block_index+1) & (BLOCK_BUFFER_SIZE - 1);
  }
  planner_forward_pass_kernel(block[1], block[2], NULL);
}

// Recalculates the trapezoid speed profiles for all blocks in the plan according to the 
// entry_factor for each junction. Must be called by planner_recalculate() after 
// updating the blocks.
void planner_recalculate_trapezoids() {
  char block_index = block_buffer_tail;
  block_t *current;
  block_t *next = NULL;
  while(block_index != block_buffer_head) {
    current = next;
    next = &block_buffer[block_index];
    if (current) {
      calculate_trapezoid_for_block(current, current->entry_speed, next->entry_speed);      
    }
    block_index = (block_index+1) & (BLOCK_BUFFER_SIZE - 1);
  }
  calculate_trapezoid_for_block(next, next->entry_speed, safe_speed(next));
}

// Recalculates the motion plan according to the following algorithm:
//
//   1. Go over every block in reverse order and calculate a junction speed reduction (i.e. block_t.entry_factor) 
//      so that:
//     a. The junction jerk is within the set limit
//     b. No speed reduction within one block requires faster deceleration than the one, true constant 
//        acceleration.
//   2. Go over every block in chronological order and dial down junction speed reduction values if 
//     a. The speed increase within one block would require faster accelleration than the one, true 
//        constant acceleration.
//
// When these stages are complete all blocks have an entry_factor that will allow all speed changes to 
// be performed using only the one, true constant acceleration, and where no junction jerk is jerkier than 
// the set limit. Finally it will:
//
//   3. Recalculate trapezoids for all blocks.

void planner_recalculate() {   
  planner_reverse_pass();
  planner_forward_pass();
  planner_recalculate_trapezoids();
}

void plan_init() {
  block_buffer_head = 0;
  block_buffer_tail = 0;
  memset(position, 0, sizeof(position)); // clear position
}


void plan_discard_current_block() {
  if (block_buffer_head != block_buffer_tail) {
    block_buffer_tail = (block_buffer_tail + 1) & (BLOCK_BUFFER_SIZE - 1);  
  }
}

block_t *plan_get_current_block() {
  if (block_buffer_head == block_buffer_tail) { 
    return(NULL); 
  }
  block_t *block = &block_buffer[block_buffer_tail];
  block->busy = true;
  return(block);
}

void check_axes_activity() {
  unsigned char x_active = 0;
  unsigned char y_active = 0;  
  unsigned char z_active = 0;
  unsigned char e_active = 0;
  block_t *block;

  if(block_buffer_tail != block_buffer_head) {
    char block_index = block_buffer_tail;
    while(block_index != block_buffer_head) {
      block = &block_buffer[block_index];
      if(block->steps_x != 0) x_active++;
      if(block->steps_y != 0) y_active++;
      if(block->steps_z != 0) z_active++;
      if(block->steps_e != 0) e_active++;
      block_index = (block_index+1) & (BLOCK_BUFFER_SIZE - 1);
    }
  }
  if((DISABLE_X) && (x_active == 0)) disable_x();
  if((DISABLE_Y) && (y_active == 0)) disable_y();
  if((DISABLE_Z) && (z_active == 0)) disable_z();
  if((DISABLE_E) && (e_active == 0)) disable_e();
}

// Add a new linear movement to the buffer. steps_x, _y and _z is the absolute position in 
// mm. Microseconds specify how many microseconds the move should take to perform. To aid acceleration
// calculation the caller must also provide the physical length of the line in millimeters.
void plan_buffer_line(float x, float y, float z, float e, float feed_rate) {

  // The target position of the tool in absolute steps
  // Calculate target position in absolute steps
  long target[4];
  target[X_AXIS] = lround(x*axis_steps_per_unit[X_AXIS]);
  target[Y_AXIS] = lround(y*axis_steps_per_unit[Y_AXIS]);
  target[Z_AXIS] = lround(z*axis_steps_per_unit[Z_AXIS]);     
  target[E_AXIS] = lround(e*axis_steps_per_unit[E_AXIS]);     

  // Calculate the buffer head after we push this byte
  int next_buffer_head = (block_buffer_head + 1) & (BLOCK_BUFFER_SIZE - 1);

  // If the buffer is full: good! That means we are well ahead of the robot. 
  // Rest here until there is room in the buffer.
  while(block_buffer_tail == next_buffer_head) { 
    manage_heater(); 
    manage_inactivity(1); 
    LCD_STATUS;
  }

  // Prepare to set up new block
  block_t *block = &block_buffer[block_buffer_head];
  
  // Mark block as not busy (Not executed by the stepper interrupt)
  block->busy = false;

  // Number of steps for each axis
  block->steps_x = labs(target[X_AXIS]-position[X_AXIS]);
  block->steps_y = labs(target[Y_AXIS]-position[Y_AXIS]);
  block->steps_z = labs(target[Z_AXIS]-position[Z_AXIS]);
  block->steps_e = labs(target[E_AXIS]-position[E_AXIS]);
  block->step_event_count = max(block->steps_x, max(block->steps_y, max(block->steps_z, block->steps_e)));

  // Bail if this is a zero-length block
  if (block->step_event_count <=dropsegments) { 
    return; 
  };

  //enable active axes
  if(block->steps_x != 0) enable_x();
  if(block->steps_y != 0) enable_y();
  if(block->steps_z != 0) enable_z();
  if(block->steps_e != 0) enable_e();

  float delta_x_mm = (target[X_AXIS]-position[X_AXIS])/axis_steps_per_unit[X_AXIS];
  float delta_y_mm = (target[Y_AXIS]-position[Y_AXIS])/axis_steps_per_unit[Y_AXIS];
  float delta_z_mm = (target[Z_AXIS]-position[Z_AXIS])/axis_steps_per_unit[Z_AXIS];
  float delta_e_mm = (target[E_AXIS]-position[E_AXIS])/axis_steps_per_unit[E_AXIS];
  block->millimeters = sqrt(square(delta_x_mm) + square(delta_y_mm) + square(delta_z_mm) + square(delta_e_mm));

  unsigned long microseconds;

  if (block->steps_e == 0) {
	if(feed_rate<mintravelfeedrate) feed_rate=mintravelfeedrate;
  }
  else {
    	if(feed_rate<minimumfeedrate) feed_rate=minimumfeedrate;
  } 

  microseconds = lround((block->millimeters/feed_rate)*1000000);

  // slow down when de buffer starts to empty, rather than wait at the corner for a buffer refill
  // reduces/removes corner blobs as the machine won't come to a full stop.
  int blockcount=(block_buffer_head-block_buffer_tail + BLOCK_BUFFER_SIZE) & (BLOCK_BUFFER_SIZE - 1);
  
  if ((blockcount>0) && (blockcount < (BLOCK_BUFFER_SIZE - 4))) {
    if (microseconds<minsegmenttime)  { // buffer is draining, add extra time.  The amount of time added increases if the buffer is still emptied more.
        microseconds=microseconds+lround(2*(minsegmenttime-microseconds)/blockcount);
    }
  }
  else {
    if (microseconds<minsegmenttime) microseconds=minsegmenttime;
  }
  //  END OF SLOW DOWN SECTION  
  
  
  // Calculate speed in mm/minute for each axis
  float multiplier = 60.0*1000000.0/microseconds;
  block->speed_z = delta_z_mm * multiplier; 
  block->speed_x = delta_x_mm * multiplier;
  block->speed_y = delta_y_mm * multiplier;
  block->speed_e = delta_e_mm * multiplier; 


  // Limit speed per axis
  float speed_factor = 1; //factor <=1 do decrease speed
  if(abs(block->speed_x) > max_feedrate[X_AXIS]) {
    //// [ErikDeBruijn] IS THIS THE BUG WE'RE LOOING FOR????
    //// [bernhard] No its not, according to Zalm.
		//// the if would always be true, since tmp_speedfactor <=0 due the inial if, so its safe to set. the next lines actually compare.
    speed_factor = max_feedrate[X_AXIS] / abs(block->speed_x);
    //if(speed_factor > tmp_speed_factor) speed_factor = tmp_speed_factor;
  }
  if(abs(block->speed_y) > max_feedrate[Y_AXIS]){
    float tmp_speed_factor = max_feedrate[Y_AXIS] / abs(block->speed_y);
    if(speed_factor > tmp_speed_factor) speed_factor = tmp_speed_factor;
  }
  if(abs(block->speed_z) > max_feedrate[Z_AXIS]){
    float tmp_speed_factor = max_feedrate[Z_AXIS] / abs(block->speed_z);
    if(speed_factor > tmp_speed_factor) speed_factor = tmp_speed_factor;
  }
  if(abs(block->speed_e) > max_feedrate[E_AXIS]){
    float tmp_speed_factor = max_feedrate[E_AXIS] / abs(block->speed_e);
    if(speed_factor > tmp_speed_factor) speed_factor = tmp_speed_factor;
  }
  multiplier = multiplier * speed_factor;
  block->speed_z = delta_z_mm * multiplier; 
  block->speed_x = delta_x_mm * multiplier;
  block->speed_y = delta_y_mm * multiplier;
  block->speed_e = delta_e_mm * multiplier; 
  block->nominal_speed = block->millimeters * multiplier;
  block->nominal_rate = ceil(block->step_event_count * multiplier / 60);  

  if(block->nominal_rate < 120) block->nominal_rate = 120;
  block->entry_speed = safe_speed(block);

  // Compute the acceleration rate for the trapezoid generator. 
  float travel_per_step = block->millimeters/block->step_event_count;
  if(block->steps_x == 0 && block->steps_y == 0 && block->steps_z == 0) {
    block->acceleration_st = ceil( (retract_acceleration)/travel_per_step); // convert to: acceleration steps/sec^2
  }
  else {
    block->acceleration_st = ceil( (acceleration)/travel_per_step);      // convert to: acceleration steps/sec^2
    float tmp_acceleration = (float)block->acceleration_st / (float)block->step_event_count;
    // Limit acceleration per axis
    if((tmp_acceleration * block->steps_x) > axis_steps_per_sqr_second[X_AXIS]) {
      block->acceleration_st = axis_steps_per_sqr_second[X_AXIS];
      tmp_acceleration = (float)block->acceleration_st / (float)block->step_event_count;
    }
    if((tmp_acceleration * block->steps_y) > axis_steps_per_sqr_second[Y_AXIS]) {
      block->acceleration_st = axis_steps_per_sqr_second[Y_AXIS];
      tmp_acceleration = (float)block->acceleration_st / (float)block->step_event_count;
    }
    if((tmp_acceleration * block->steps_e) > axis_steps_per_sqr_second[E_AXIS]) {
      block->acceleration_st = axis_steps_per_sqr_second[E_AXIS];
      tmp_acceleration = (float)block->acceleration_st / (float)block->step_event_count;
    }
    if((tmp_acceleration * block->steps_z) > axis_steps_per_sqr_second[Z_AXIS]) {
      block->acceleration_st = axis_steps_per_sqr_second[Z_AXIS];
      tmp_acceleration = (float)block->acceleration_st / (float)block->step_event_count;
    }
  }
  block->acceleration = block->acceleration_st * travel_per_step;
  block->acceleration_rate = (long)((float)block->acceleration_st * 8.388608);

#ifdef ADVANCE
  // Calculate advance rate
  if((block->steps_e == 0) || (block->steps_x == 0 && block->steps_y == 0 && block->steps_z == 0)) {
    block->advance_rate = 0;
    block->advance = 0;
  }
  else {
    long acc_dist = estimate_acceleration_distance(0, block->nominal_rate, block->acceleration_st);
    float advance = (STEPS_PER_CUBIC_MM_E * EXTRUDER_ADVANCE_K) * 
      (block->speed_e * block->speed_e * EXTRUTION_AREA * EXTRUTION_AREA / 3600.0)*65536;
    block->advance = advance;
    if(acc_dist == 0) {
      block->advance_rate = 0;
    } 
    else {
      block->advance_rate = advance / (float)acc_dist;
    }
  }
#endif // ADVANCE

  // compute a preliminary conservative acceleration trapezoid
  float safespeed = safe_speed(block);
  calculate_trapezoid_for_block(block, safespeed, safespeed); 

  // Compute direction bits for this block
  block->direction_bits = 0;
  if (target[X_AXIS] < position[X_AXIS]) { 
    block->direction_bits |= (1<<X_AXIS); 
  }
  if (target[Y_AXIS] < position[Y_AXIS]) { 
    block->direction_bits |= (1<<Y_AXIS); 
  }
  if (target[Z_AXIS] < position[Z_AXIS]) { 
    block->direction_bits |= (1<<Z_AXIS); 
  }
  if (target[E_AXIS] < position[E_AXIS]) { 
    block->direction_bits |= (1<<E_AXIS); 
  }

  // Move buffer head
  block_buffer_head = next_buffer_head;     

  // Update position 
  memcpy(position, target, sizeof(target)); // position[] = target[]

  planner_recalculate();
  st_wake_up();
}

void plan_set_position(float x, float y, float z, float e)
{
  position[X_AXIS] = lround(x*axis_steps_per_unit[X_AXIS]);
  position[Y_AXIS] = lround(y*axis_steps_per_unit[Y_AXIS]);
  position[Z_AXIS] = lround(z*axis_steps_per_unit[Z_AXIS]);     
  position[E_AXIS] = lround(e*axis_steps_per_unit[E_AXIS]);     
}

