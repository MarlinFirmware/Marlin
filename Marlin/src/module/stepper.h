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
 * Derived from Grbl
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

#include "stepper_indirection.h"

#ifdef __AVR__
  #include "speed_lookuptable.h"
#endif

#include "../inc/MarlinConfig.h"
#include "../module/planner.h"
#include "../core/language.h"

class Stepper;
extern Stepper stepper;

class Stepper {

  public:

    static block_t* current_block;  // A pointer to the block currently being traced

    #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
      static bool abort_on_endstop_hit;
    #endif

    #if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
      static bool performing_homing;
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      #ifndef PWM_MOTOR_CURRENT
        #define PWM_MOTOR_CURRENT DEFAULT_PWM_MOTOR_CURRENT
      #endif
      static uint32_t motor_current_setting[3];
    #endif

    static int16_t cleaning_buffer_counter;

  private:

    static uint8_t last_direction_bits;        // The next stepping-bits to be output

    #if ENABLED(X_DUAL_ENDSTOPS)
      static bool locked_x_motor, locked_x2_motor;
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      static bool locked_y_motor, locked_y2_motor;
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      static bool locked_z_motor, locked_z2_motor;
    #endif

    // Counter variables for the Bresenham line tracer
    static long counter_X, counter_Y, counter_Z, counter_E;
    static volatile uint32_t step_events_completed; // The number of step events executed in the current block

    #if ENABLED(BEZIER_JERK_CONTROL)
      static int32_t bezier_A,     // A coefficient in Bézier speed curve
                     bezier_B,     // B coefficient in Bézier speed curve
                     bezier_C;     // C coefficient in Bézier speed curve
      static uint32_t bezier_F;    // F coefficient in Bézier speed curve
      static uint32_t bezier_AV;   // AV coefficient in Bézier speed curve
      #ifdef __AVR__
        static bool A_negative;    // If A coefficient was negative
      #endif
      static bool bezier_2nd_half; // If Bézier curve has been initialized or not
    #endif

    #if ENABLED(LIN_ADVANCE)

      static uint32_t LA_decelerate_after; // Copy from current executed block. Needed because current_block is set to NULL "too early".
      static hal_timer_t nextMainISR, nextAdvanceISR, eISR_Rate;
      static uint16_t current_adv_steps, final_adv_steps, max_adv_steps; // Copy from current executed block. Needed because current_block is set to NULL "too early".
      #define _NEXT_ISR(T) nextMainISR = T
      static int8_t e_steps;
      static bool use_advance_lead;
      #if E_STEPPERS > 1
        static int8_t LA_active_extruder; // Copy from current executed block. Needed because current_block is set to NULL "too early".
      #else
        static constexpr int8_t LA_active_extruder = 0;
      #endif

    #else // !LIN_ADVANCE

      #define _NEXT_ISR(T) HAL_timer_set_compare(STEP_TIMER_NUM, T);

    #endif // !LIN_ADVANCE

    static int32_t acceleration_time, deceleration_time;
    static uint8_t step_loops, step_loops_nominal;

    static hal_timer_t OCR1A_nominal;
    #if DISABLED(BEZIER_JERK_CONTROL)
      static hal_timer_t acc_step_rate; // needed for deceleration start point
    #endif

    static volatile long endstops_trigsteps[XYZ];
    static volatile long endstops_stepsTotal, endstops_stepsDone;

    //
    // Positions of stepper motors, in step units
    //
    static volatile int32_t count_position[NUM_AXIS];

    //
    // Current direction of stepper motors (+1 or -1)
    //
    static volatile signed char count_direction[NUM_AXIS];

    //
    // Mixing extruder mix counters
    //
    #if ENABLED(MIXING_EXTRUDER)
      static long counter_m[MIXING_STEPPERS];
      #define MIXING_STEPPERS_LOOP(VAR) \
        for (uint8_t VAR = 0; VAR < MIXING_STEPPERS; VAR++) \
          if (current_block->mix_event_count[VAR])
    #endif

  public:

    //
    // Constructor / initializer
    //
    Stepper() { };

    //
    // Initialize stepper hardware
    //
    static void init();

    //
    // Interrupt Service Routines
    //

    static void isr();

    #if ENABLED(LIN_ADVANCE)
      static void advance_isr();
      static void advance_isr_scheduler();
    #endif

    //
    // Block until all buffered steps are executed
    //
    static void synchronize();

    //
    // Set the current position in steps
    //
    static void set_position(const long &a, const long &b, const long &c, const long &e);
    static void set_position(const AxisEnum &a, const long &v);
    static void set_e_position(const long &e);

    //
    // Set direction bits for all steppers
    //
    static void set_directions();

    //
    // Get the position of a stepper, in steps
    //
    static long position(const AxisEnum axis);

    //
    // Report the positions of the steppers, in steps
    //
    static void report_positions();

    //
    // Get the position (mm) of an axis based on stepper position(s)
    //
    static float get_axis_position_mm(const AxisEnum axis);

    //
    // SCARA AB axes are in degrees, not mm
    //
    #if IS_SCARA
      FORCE_INLINE static float get_axis_position_degrees(const AxisEnum axis) { return get_axis_position_mm(axis); }
    #endif

    //
    // The stepper subsystem goes to sleep when it runs out of things to execute. Call this
    // to notify the subsystem that it is time to go to work.
    //
    static void wake_up();

    //
    // Wait for moves to finish and disable all steppers
    //
    static void finish_and_disable();

    //
    // Quickly stop all steppers and clear the blocks queue
    //
    static void quick_stop();

    //
    // The direction of a single motor
    //
    FORCE_INLINE static bool motor_direction(const AxisEnum axis) { return TEST(last_direction_bits, axis); }

    #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM
      static void digitalPotWrite(const int16_t address, const int16_t value);
      static void digipot_current(const uint8_t driver, const int16_t current);
    #endif

    #if HAS_MICROSTEPS
      static void microstep_ms(const uint8_t driver, const int8_t ms1, const int8_t ms2);
      static void microstep_mode(const uint8_t driver, const uint8_t stepping);
      static void microstep_readings();
    #endif

    #if ENABLED(X_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_homing_flag_x(const bool state) { performing_homing = state; }
      FORCE_INLINE static void set_x_lock(const bool state) { locked_x_motor = state; }
      FORCE_INLINE static void set_x2_lock(const bool state) { locked_x2_motor = state; }
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_homing_flag_y(const bool state) { performing_homing = state; }
      FORCE_INLINE static void set_y_lock(const bool state) { locked_y_motor = state; }
      FORCE_INLINE static void set_y2_lock(const bool state) { locked_y2_motor = state; }
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_homing_flag_z(const bool state) { performing_homing = state; }
      FORCE_INLINE static void set_z_lock(const bool state) { locked_z_motor = state; }
      FORCE_INLINE static void set_z2_lock(const bool state) { locked_z2_motor = state; }
    #endif

    #if ENABLED(BABYSTEPPING)
      static void babystep(const AxisEnum axis, const bool direction); // perform a short step with a single stepper motor, outside of any convention
    #endif

    static inline void kill_current_block() {
      step_events_completed = current_block->step_event_count;
    }

    //
    // Handle a triggered endstop
    //
    static void endstop_triggered(const AxisEnum axis);

    //
    // Triggered position of an axis in mm (not core-savvy)
    //
    FORCE_INLINE static float triggered_position_mm(const AxisEnum axis) {
      return endstops_trigsteps[axis] * planner.steps_to_mm[axis];
    }

    #if HAS_MOTOR_CURRENT_PWM
      static void refresh_motor_power();
    #endif

  private:

    FORCE_INLINE static hal_timer_t calc_timer_interval(hal_timer_t step_rate) {
      hal_timer_t timer;

      NOMORE(step_rate, MAX_STEP_FREQUENCY);

      // TODO: HAL: tidy this up, use condtionals_post.h
      #ifdef CPU_32_BIT
        #if ENABLED(DISABLE_MULTI_STEPPING)
          step_loops = 1;
        #else
          if (step_rate > STEP_DOUBLER_FREQUENCY * 2) { // If steprate > (STEP_DOUBLER_FREQUENCY * 2) kHz >> step 4 times
            step_rate >>= 2;
            step_loops = 4;
          }
          else if (step_rate > STEP_DOUBLER_FREQUENCY) { // If steprate > STEP_DOUBLER_FREQUENCY kHz >> step 2 times
            step_rate >>= 1;
            step_loops = 2;
          }
          else {
            step_loops = 1;
          }
        #endif
      #else
        if (step_rate > 20000) { // If steprate > 20kHz >> step 4 times
          step_rate >>= 2;
          step_loops = 4;
        }
        else if (step_rate > 10000) { // If steprate > 10kHz >> step 2 times
          step_rate >>= 1;
          step_loops = 2;
        }
        else {
          step_loops = 1;
        }
      #endif

      #ifdef CPU_32_BIT
        // In case of high-performance processor, it is able to calculate in real-time
        const uint32_t MIN_TIME_PER_STEP = (HAL_STEPPER_TIMER_RATE) / ((STEP_DOUBLER_FREQUENCY) * 2);
        timer = uint32_t(HAL_STEPPER_TIMER_RATE) / step_rate;
        NOLESS(timer, MIN_TIME_PER_STEP); // (STEP_DOUBLER_FREQUENCY * 2 kHz - this should never happen)
      #else
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
        if (timer < 100) { // (20kHz - this should never happen)
          timer = 100;
          SERIAL_ECHOPGM(MSG_STEPPER_TOO_HIGH);
          SERIAL_ECHOLN(step_rate);
        }
      #endif

      return timer;
    }

    #if ENABLED(BEZIER_JERK_CONTROL)
      static void _calc_bezier_curve_coeffs(const int32_t v0, const int32_t v1, const uint32_t av);
      static int32_t _eval_bezier_curve(const uint32_t curr_step);
    #endif

    #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM
      static void digipot_init();
    #endif

    #if HAS_MICROSTEPS
      static void microstep_init();
    #endif

};

#endif // STEPPER_H
