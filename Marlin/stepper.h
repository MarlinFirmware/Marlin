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
 * stepper.h - stepper motor driver: executes motion plans of planner.c using the stepper motors
 * Part of Grbl
 *
 * Copyright (c) 2009-2011 Simen Svale Skogsrud
 *
 * Grbl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Grbl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STEPPER_H
#define STEPPER_H

#include "planner.h"
#include "speed_lookuptable.h"
#include "stepper_indirection.h"
#include "language.h"

class Stepper;
extern Stepper stepper;

// intRes = intIn1 * intIn2 >> 16
// uses:
// r26 to store 0
// r27 to store the byte 1 of the 24 bit result
#define MultiU16X8toH16(intRes, charIn1, intIn2) \
  asm volatile ( \
                 "clr r26 \n\t" \
                 "mul %A1, %B2 \n\t" \
                 "movw %A0, r0 \n\t" \
                 "mul %A1, %A2 \n\t" \
                 "add %A0, r1 \n\t" \
                 "adc %B0, r26 \n\t" \
                 "lsr r0 \n\t" \
                 "adc %A0, r26 \n\t" \
                 "adc %B0, r26 \n\t" \
                 "clr r1 \n\t" \
                 : \
                 "=&r" (intRes) \
                 : \
                 "d" (charIn1), \
                 "d" (intIn2) \
                 : \
                 "r26" \
               )

class Stepper {

  public:

    block_t* current_block = NULL;  // A pointer to the block currently being traced

    #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
      bool abort_on_endstop_hit = false;
    #endif

    #if ENABLED(Z_DUAL_ENDSTOPS)
      bool performing_homing = false;
    #endif

    #if ENABLED(ADVANCE)
      long e_steps[4];
    #endif

  private:

    unsigned char last_direction_bits = 0;        // The next stepping-bits to be output
    unsigned int cleaning_buffer_counter = 0;

    #if ENABLED(Z_DUAL_ENDSTOPS)
      bool locked_z_motor = false,
           locked_z2_motor = false;
    #endif

    // Counter variables for the Bresenham line tracer
    long counter_X = 0, counter_Y = 0, counter_Z = 0, counter_E = 0;
    volatile unsigned long step_events_completed = 0; // The number of step events executed in the current block

    #if ENABLED(ADVANCE)
      unsigned char old_OCR0A;
      long advance_rate, advance, final_advance = 0;
      long old_advance = 0;
    #endif

    long acceleration_time, deceleration_time;
    //unsigned long accelerate_until, decelerate_after, acceleration_rate, initial_rate, final_rate, nominal_rate;
    unsigned short acc_step_rate; // needed for deceleration start point
    uint8_t step_loops;
    uint8_t step_loops_nominal;
    unsigned short OCR1A_nominal;

    volatile long endstops_trigsteps[3];
    volatile long endstops_stepsTotal, endstops_stepsDone;

    #if HAS_MOTOR_CURRENT_PWM
      #ifndef PWM_MOTOR_CURRENT
        #define PWM_MOTOR_CURRENT DEFAULT_PWM_MOTOR_CURRENT
      #endif
      const int motor_current_setting[3] = PWM_MOTOR_CURRENT;
    #endif

    //
    // Positions of stepper motors, in step units
    //
    volatile long count_position[NUM_AXIS] = { 0 };

    //
    // Current direction of stepper motors (+1 or -1)
    //
    volatile signed char count_direction[NUM_AXIS] = { 1 };

  public:

    //
    // Constructor / initializer
    //
    Stepper() {};

    //
    // Initialize stepper hardware
    //
    void init();

    //
    // Interrupt Service Routines
    //

    void isr();

    #if ENABLED(ADVANCE)
      void advance_isr();
    #endif

    //
    // Block until all buffered steps are executed
    //
    void synchronize();

    //
    // Set the current position in steps
    //
    void set_position(const long& x, const long& y, const long& z, const long& e);
    void set_e_position(const long& e);

    //
    // Set direction bits for all steppers
    //
    void set_directions();

    //
    // Get the position of a stepper, in steps
    //
    long position(AxisEnum axis);

    //
    // Report the positions of the steppers, in steps
    //
    void report_positions();

    //
    // Get the position (mm) of an axis based on stepper position(s)
    //
    float get_axis_position_mm(AxisEnum axis);

    //
    // The stepper subsystem goes to sleep when it runs out of things to execute. Call this
    // to notify the subsystem that it is time to go to work.
    //
    void wake_up();

    //
    // Wait for moves to finish and disable all steppers
    //
    void finish_and_disable();

    //
    // Quickly stop all steppers and clear the blocks queue
    //
    void quick_stop();

    //
    // The direction of a single motor
    //
    FORCE_INLINE bool motor_direction(AxisEnum axis) { return TEST(last_direction_bits, axis); }

    #if HAS_DIGIPOTSS
      void digitalPotWrite(int address, int value);
    #endif
    void microstep_ms(uint8_t driver, int8_t ms1, int8_t ms2);
    void digipot_current(uint8_t driver, int current);
    void microstep_mode(uint8_t driver, uint8_t stepping);
    void microstep_readings();

    #if ENABLED(Z_DUAL_ENDSTOPS)
      FORCE_INLINE void set_homing_flag(bool state) { performing_homing = state; }
      FORCE_INLINE void set_z_lock(bool state) { locked_z_motor = state; }
      FORCE_INLINE void set_z2_lock(bool state) { locked_z2_motor = state; }
    #endif

    #if ENABLED(BABYSTEPPING)
      void babystep(const uint8_t axis, const bool direction); // perform a short step with a single stepper motor, outside of any convention
    #endif

    inline void kill_current_block() {
      step_events_completed = current_block->step_event_count;
    }

    //
    // Handle a triggered endstop
    //
    void endstop_triggered(AxisEnum axis);

    //
    // Triggered position of an axis in mm (not core-savvy)
    //
    FORCE_INLINE float triggered_position_mm(AxisEnum axis) {
      return endstops_trigsteps[axis] / planner.axis_steps_per_unit[axis];
    }

  private:

    FORCE_INLINE unsigned short calc_timer(unsigned short step_rate) {
      unsigned short timer;

      NOMORE(step_rate, MAX_STEP_FREQUENCY);

      if (step_rate > 20000) { // If steprate > 20kHz >> step 4 times
        step_rate = (step_rate >> 2) & 0x3fff;
        step_loops = 4;
      }
      else if (step_rate > 10000) { // If steprate > 10kHz >> step 2 times
        step_rate = (step_rate >> 1) & 0x7fff;
        step_loops = 2;
      }
      else {
        step_loops = 1;
      }

      NOLESS(step_rate, F_CPU / 500000);
      step_rate -= F_CPU / 500000; // Correct for minimal speed
      if (step_rate >= (8 * 256)) { // higher step rate
        unsigned short table_address = (unsigned short)&speed_lookuptable_fast[(unsigned char)(step_rate >> 8)][0];
        unsigned char tmp_step_rate = (step_rate & 0x00ff);
        unsigned short gain = (unsigned short)pgm_read_word_near(table_address + 2);
        MultiU16X8toH16(timer, tmp_step_rate, gain);
        timer = (unsigned short)pgm_read_word_near(table_address) - timer;
      }
      else { // lower step rates
        unsigned short table_address = (unsigned short)&speed_lookuptable_slow[0][0];
        table_address += ((step_rate) >> 1) & 0xfffc;
        timer = (unsigned short)pgm_read_word_near(table_address);
        timer -= (((unsigned short)pgm_read_word_near(table_address + 2) * (unsigned char)(step_rate & 0x0007)) >> 3);
      }
      if (timer < 100) { timer = 100; MYSERIAL.print(MSG_STEPPER_TOO_HIGH); MYSERIAL.println(step_rate); }//(20kHz this should never happen)
      return timer;
    }

    // Initializes the trapezoid generator from the current block. Called whenever a new
    // block begins.
    FORCE_INLINE void trapezoid_generator_reset() {

      static int8_t last_extruder = -1;

      if (current_block->direction_bits != last_direction_bits || current_block->active_extruder != last_extruder) {
        last_direction_bits = current_block->direction_bits;
        last_extruder = current_block->active_extruder;
        set_directions();
      }

      #if ENABLED(ADVANCE)
        advance = current_block->initial_advance;
        final_advance = current_block->final_advance;
        // Do E steps + advance steps
        e_steps[current_block->active_extruder] += ((advance >>8) - old_advance);
        old_advance = advance >>8;
      #endif
      deceleration_time = 0;
      // step_rate to timer interval
      OCR1A_nominal = calc_timer(current_block->nominal_rate);
      // make a note of the number of step loops required at nominal speed
      step_loops_nominal = step_loops;
      acc_step_rate = current_block->initial_rate;
      acceleration_time = calc_timer(acc_step_rate);
      OCR1A = acceleration_time;

      // SERIAL_ECHO_START;
      // SERIAL_ECHOPGM("advance :");
      // SERIAL_ECHO(current_block->advance/256.0);
      // SERIAL_ECHOPGM("advance rate :");
      // SERIAL_ECHO(current_block->advance_rate/256.0);
      // SERIAL_ECHOPGM("initial advance :");
      // SERIAL_ECHO(current_block->initial_advance/256.0);
      // SERIAL_ECHOPGM("final advance :");
      // SERIAL_ECHOLN(current_block->final_advance/256.0);
    }

    void digipot_init();
    void microstep_init();

};

#endif // STEPPER_H