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

    #if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
      static bool performing_homing;
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      #ifndef PWM_MOTOR_CURRENT
        #define PWM_MOTOR_CURRENT DEFAULT_PWM_MOTOR_CURRENT
      #endif
      static uint32_t motor_current_setting[3];
    #endif

  private:

    static uint8_t last_direction_bits,     // The next stepping-bits to be output
                   last_movement_extruder,  // Last movement extruder, as computed when the last movement was fetched from planner
                   axis_did_move;           // Last Movement in the given direction is not null, as computed when the last movement was fetched from planner
    static bool abort_current_block;        // Signals to the stepper that current block should be aborted

    #if ENABLED(X_DUAL_ENDSTOPS)
      static bool locked_X_motor, locked_X2_motor;
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      static bool locked_Y_motor, locked_Y2_motor;
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      static bool locked_Z_motor, locked_Z2_motor;
    #endif

    // Counter variables for the Bresenham line tracer
    static int32_t counter_X, counter_Y, counter_Z, counter_E;
    static uint32_t step_events_completed; // The number of step events executed in the current block

    #if ENABLED(S_CURVE_ACCELERATION)
      static int32_t bezier_A,     // A coefficient in Bézier speed curve
                     bezier_B,     // B coefficient in Bézier speed curve
                     bezier_C;     // C coefficient in Bézier speed curve
      static uint32_t bezier_F,    // F coefficient in Bézier speed curve
                      bezier_AV;   // AV coefficient in Bézier speed curve
      #ifdef __AVR__
        static bool A_negative;    // If A coefficient was negative
      #endif
      static bool bezier_2nd_half; // If Bézier curve has been initialized or not
    #endif

    static uint32_t nextMainISR;   // time remaining for the next Step ISR
    static bool all_steps_done;    // all steps done

    #if ENABLED(LIN_ADVANCE)

      static uint32_t LA_decelerate_after; // Copy from current executed block. Needed because current_block is set to NULL "too early".
      static uint32_t nextAdvanceISR, eISR_Rate;
      static uint16_t current_adv_steps, final_adv_steps, max_adv_steps; // Copy from current executed block. Needed because current_block is set to NULL "too early".
      static int8_t e_steps;
      static bool use_advance_lead;
      #if E_STEPPERS > 1
        static int8_t LA_active_extruder; // Copy from current executed block. Needed because current_block is set to NULL "too early".
      #else
        static constexpr int8_t LA_active_extruder = 0;
      #endif

    #endif // LIN_ADVANCE

    static uint32_t acceleration_time, deceleration_time;
    static uint8_t step_loops, step_loops_nominal;

    static uint32_t ticks_nominal;
    #if DISABLED(S_CURVE_ACCELERATION)
      static uint32_t acc_step_rate; // needed for deceleration start point
    #endif

    static volatile int32_t endstops_trigsteps[XYZ];
    static volatile int32_t endstops_stepsTotal, endstops_stepsDone;

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
      static int32_t counter_m[MIXING_STEPPERS];
      #define MIXING_STEPPERS_LOOP(VAR) \
        for (uint8_t VAR = 0; VAR < MIXING_STEPPERS; VAR++) \
          if (current_block->mix_event_count[VAR])
    #endif

  public:

    //
    // Constructor / initializer
    //
    Stepper() { };

    // Initialize stepper hardware
    static void init();

    // Interrupt Service Routines

    // The ISR scheduler
    static void isr();

    // The stepper pulse phase ISR
    static void stepper_pulse_phase_isr();

    // The stepper block processing phase ISR
    static uint32_t stepper_block_phase_isr();

    #if ENABLED(LIN_ADVANCE)
      // The Linear advance stepper ISR
      static uint32_t advance_isr();
    #endif

    // Get the position of a stepper, in steps
    static int32_t position(const AxisEnum axis);

    // Report the positions of the steppers, in steps
    static void report_positions();

    // The stepper subsystem goes to sleep when it runs out of things to execute. Call this
    // to notify the subsystem that it is time to go to work.
    static void wake_up();

    // Quickly stop all steppers
    FORCE_INLINE static void quick_stop() { abort_current_block = true; }

    // The direction of a single motor
    FORCE_INLINE static bool motor_direction(const AxisEnum axis) { return TEST(last_direction_bits, axis); }

    // The last movement direction was not null on the specified axis. Note that motor direction is not necessarily the same.
    FORCE_INLINE static bool axis_is_moving(const AxisEnum axis) { return TEST(axis_did_move, axis); }

    // The extruder associated to the last movement
    FORCE_INLINE static uint8_t movement_extruder() { return last_movement_extruder; }

    // Handle a triggered endstop
    static void endstop_triggered(const AxisEnum axis);

    // Triggered position of an axis in steps
    static int32_t triggered_position(const AxisEnum axis);

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
      FORCE_INLINE static void set_x_lock(const bool state) { locked_X_motor = state; }
      FORCE_INLINE static void set_x2_lock(const bool state) { locked_X2_motor = state; }
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_homing_flag_y(const bool state) { performing_homing = state; }
      FORCE_INLINE static void set_y_lock(const bool state) { locked_Y_motor = state; }
      FORCE_INLINE static void set_y2_lock(const bool state) { locked_Y2_motor = state; }
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_homing_flag_z(const bool state) { performing_homing = state; }
      FORCE_INLINE static void set_z_lock(const bool state) { locked_Z_motor = state; }
      FORCE_INLINE static void set_z2_lock(const bool state) { locked_Z2_motor = state; }
    #endif

    #if ENABLED(BABYSTEPPING)
      static void babystep(const AxisEnum axis, const bool direction); // perform a short step with a single stepper motor, outside of any convention
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      static void refresh_motor_power();
    #endif

    // Set the current position in steps
    inline static void set_position(const int32_t &a, const int32_t &b, const int32_t &c, const int32_t &e) {
      planner.synchronize();

      // Disable stepper interrupts, to ensure atomic setting of all the position variables
      const bool was_enabled = STEPPER_ISR_ENABLED();
      if (was_enabled) DISABLE_STEPPER_DRIVER_INTERRUPT();

      // Set position
      _set_position(a, b, c, e);

      // Reenable Stepper ISR
      if (was_enabled) ENABLE_STEPPER_DRIVER_INTERRUPT();
    }

    inline static void set_position(const AxisEnum a, const int32_t &v) {
      planner.synchronize();

    #ifdef __AVR__
      // Protect the access to the position. Only required for AVR, as
      //  any 32bit CPU offers atomic access to 32bit variables
      const bool was_enabled = STEPPER_ISR_ENABLED();
      if (was_enabled) DISABLE_STEPPER_DRIVER_INTERRUPT();
    #endif

      count_position[a] = v;

    #ifdef __AVR__
      // Reenable Stepper ISR
      if (was_enabled) ENABLE_STEPPER_DRIVER_INTERRUPT();
    #endif
    }

  private:

    // Set the current position in steps
    static void _set_position(const int32_t &a, const int32_t &b, const int32_t &c, const int32_t &e);

    // Set direction bits for all steppers
    static void set_directions();

    // Limit the speed to 10KHz for AVR
    #ifndef STEP_DOUBLER_FREQUENCY
      #define STEP_DOUBLER_FREQUENCY 10000
    #endif

    FORCE_INLINE static uint32_t calc_timer_interval(uint32_t step_rate) {
      uint32_t timer;

      NOMORE(step_rate, uint32_t(MAX_STEP_FREQUENCY));

      #if DISABLED(DISABLE_MULTI_STEPPING)
        if (step_rate > STEP_DOUBLER_FREQUENCY * 2) { // If steprate > (STEP_DOUBLER_FREQUENCY * 2) kHz >> step 4 times
          step_rate >>= 2;
          step_loops = 4;
        }
        else if (step_rate > STEP_DOUBLER_FREQUENCY) { // If steprate > STEP_DOUBLER_FREQUENCY kHz >> step 2 times
          step_rate >>= 1;
          step_loops = 2;
        }
        else
      #endif
          step_loops = 1;

      #ifdef CPU_32_BIT
        // In case of high-performance processor, it is able to calculate in real-time
        const uint32_t min_time_per_step = (HAL_STEPPER_TIMER_RATE) / ((STEP_DOUBLER_FREQUENCY) * 2);
        timer = uint32_t(HAL_STEPPER_TIMER_RATE) / step_rate;
        NOLESS(timer, min_time_per_step); // (STEP_DOUBLER_FREQUENCY * 2 kHz - this should never happen)
      #else
        constexpr uint32_t min_step_rate = F_CPU / 500000U;
        NOLESS(step_rate, min_step_rate);
        step_rate -= min_step_rate; // Correct for minimal speed
        if (step_rate >= (8 * 256)) { // higher step rate
          const uint8_t tmp_step_rate = (step_rate & 0x00FF);
          const uint16_t table_address = (uint16_t)&speed_lookuptable_fast[(uint8_t)(step_rate >> 8)][0],
                         gain = (uint16_t)pgm_read_word_near(table_address + 2);
          timer = MultiU16X8toH16(tmp_step_rate, gain);
          timer = (uint16_t)pgm_read_word_near(table_address) - timer;
        }
        else { // lower step rates
          uint16_t table_address = (uint16_t)&speed_lookuptable_slow[0][0];
          table_address += ((step_rate) >> 1) & 0xFFFC;
          timer = (uint16_t)pgm_read_word_near(table_address)
                - (((uint16_t)pgm_read_word_near(table_address + 2) * (uint8_t)(step_rate & 0x0007)) >> 3);
        }
        if (timer < 100) { // (20kHz - this should never happen)
          timer = 100;
          SERIAL_ECHOLNPAIR(MSG_STEPPER_TOO_HIGH, step_rate);
        }
      #endif

      return timer;
    }

    #if ENABLED(S_CURVE_ACCELERATION)
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
