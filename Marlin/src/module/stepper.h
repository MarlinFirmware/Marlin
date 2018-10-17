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

#include "../inc/MarlinConfig.h"

// Disable multiple steps per ISR
//#define DISABLE_MULTI_STEPPING

//
// Estimate the amount of time the Stepper ISR will take to execute
//

#ifndef MINIMUM_STEPPER_PULSE
  #define MINIMUM_STEPPER_PULSE 0UL
#endif

#ifndef MAXIMUM_STEPPER_RATE
  #if MINIMUM_STEPPER_PULSE
    #define MAXIMUM_STEPPER_RATE (1000000UL / (2UL * (unsigned long)(MINIMUM_STEPPER_PULSE)))
  #else
    #define MAXIMUM_STEPPER_RATE 500000UL
  #endif
#endif

#ifdef CPU_32_BIT

  // The base ISR takes 792 cycles
  #define ISR_BASE_CYCLES  792UL

  // Linear advance base time is 64 cycles
  #if ENABLED(LIN_ADVANCE)
    #define ISR_LA_BASE_CYCLES 64UL
  #else
    #define ISR_LA_BASE_CYCLES 0UL
  #endif

  // S curve interpolation adds 40 cycles
  #if ENABLED(S_CURVE_ACCELERATION)
    #define ISR_S_CURVE_CYCLES 40UL
  #else
    #define ISR_S_CURVE_CYCLES 0UL
  #endif

  // Stepper Loop base cycles
  #define ISR_LOOP_BASE_CYCLES 4UL

  // To start the step pulse, in the worst case takes
  #define ISR_START_STEPPER_CYCLES 13UL

  // And each stepper (start + stop pulse) takes in worst case
  #define ISR_STEPPER_CYCLES 16UL

#else

  // The base ISR takes 752 cycles
  #define ISR_BASE_CYCLES  752UL

  // Linear advance base time is 32 cycles
  #if ENABLED(LIN_ADVANCE)
    #define ISR_LA_BASE_CYCLES 32UL
  #else
    #define ISR_LA_BASE_CYCLES 0UL
  #endif

  // S curve interpolation adds 160 cycles
  #if ENABLED(S_CURVE_ACCELERATION)
    #define ISR_S_CURVE_CYCLES 160UL
  #else
    #define ISR_S_CURVE_CYCLES 0UL
  #endif

  // Stepper Loop base cycles
  #define ISR_LOOP_BASE_CYCLES 32UL

  // To start the step pulse, in the worst case takes
  #define ISR_START_STEPPER_CYCLES 57UL

  // And each stepper (start + stop pulse) takes in worst case
  #define ISR_STEPPER_CYCLES 88UL

#endif

// Add time for each stepper
#ifdef HAS_X_STEP
  #define ISR_START_X_STEPPER_CYCLES ISR_START_STEPPER_CYCLES
  #define ISR_X_STEPPER_CYCLES       ISR_STEPPER_CYCLES
#else
  #define ISR_START_X_STEPPER_CYCLES 0UL
  #define ISR_X_STEPPER_CYCLES       0UL
#endif
#ifdef HAS_Y_STEP
  #define ISR_START_Y_STEPPER_CYCLES ISR_START_STEPPER_CYCLES
  #define ISR_Y_STEPPER_CYCLES       ISR_STEPPER_CYCLES
#else
  #define ISR_START_Y_STEPPER_CYCLES 0UL
  #define ISR_Y_STEPPER_CYCLES       0UL
#endif
#ifdef HAS_Z_STEP
  #define ISR_START_Z_STEPPER_CYCLES ISR_START_STEPPER_CYCLES
  #define ISR_Z_STEPPER_CYCLES       ISR_STEPPER_CYCLES
#else
  #define ISR_START_Z_STEPPER_CYCLES 0UL
  #define ISR_Z_STEPPER_CYCLES       0UL
#endif

// E is always interpolated, even for mixing extruders
#define ISR_START_E_STEPPER_CYCLES   ISR_START_STEPPER_CYCLES
#define ISR_E_STEPPER_CYCLES         ISR_STEPPER_CYCLES

// If linear advance is disabled, then the loop also handles them
#if DISABLED(LIN_ADVANCE) && ENABLED(MIXING_EXTRUDER) // ToDo: ???
  // HELP ME: What is what?
  // Directions are set up for MIXING_STEPPERS - like before.
  // Finding the right stepper may last up to MIXING_STEPPERS loops in get_next_stepper().
  //   These loops are a bit faster than advancing a bresenham counter.
  // Always only one e-stepper is stepped.
  #define ISR_START_MIXING_STEPPER_CYCLES ((MIXING_STEPPERS) * (ISR_START_STEPPER_CYCLES))
  #define ISR_MIXING_STEPPER_CYCLES ((MIXING_STEPPERS) * (ISR_STEPPER_CYCLES))
#else
  #define ISR_START_MIXING_STEPPER_CYCLES 0UL
  #define ISR_MIXING_STEPPER_CYCLES  0UL
#endif

// Calculate the minimum time to start all stepper pulses in the ISR loop
#define MIN_ISR_START_LOOP_CYCLES (ISR_START_X_STEPPER_CYCLES + ISR_START_Y_STEPPER_CYCLES + ISR_START_Z_STEPPER_CYCLES + ISR_START_E_STEPPER_CYCLES + ISR_START_MIXING_STEPPER_CYCLES)

// And the total minimum loop time, not including the base
#define MIN_ISR_LOOP_CYCLES (ISR_X_STEPPER_CYCLES + ISR_Y_STEPPER_CYCLES + ISR_Z_STEPPER_CYCLES + ISR_E_STEPPER_CYCLES + ISR_MIXING_STEPPER_CYCLES)

// Calculate the minimum MPU cycles needed per pulse to enforce, limited to the max stepper rate
#define _MIN_STEPPER_PULSE_CYCLES(N) MAX((unsigned long)((F_CPU) / (MAXIMUM_STEPPER_RATE)), ((F_CPU) / 500000UL) * (N))
#if MINIMUM_STEPPER_PULSE
  #define MIN_STEPPER_PULSE_CYCLES _MIN_STEPPER_PULSE_CYCLES((unsigned long)(MINIMUM_STEPPER_PULSE))
#else
  #define MIN_STEPPER_PULSE_CYCLES _MIN_STEPPER_PULSE_CYCLES(1UL)
#endif

// Calculate the minimum ticks of the PULSE timer that must elapse with the step pulse enabled
// adding the "start stepper pulse" code section execution cycles to account for that not all
// pulses start at the beginning of the loop, so an extra time must be added to compensate so
// the last generated pulse (usually the extruder stepper) has the right length
#define MIN_PULSE_TICKS (((PULSE_TIMER_TICKS_PER_US) * (unsigned long)(MINIMUM_STEPPER_PULSE)) + ((MIN_ISR_START_LOOP_CYCLES) / (unsigned long)(PULSE_TIMER_PRESCALE)))

// Calculate the extra ticks of the PULSE timer between step pulses
#define ADDED_STEP_TICKS (((MIN_STEPPER_PULSE_CYCLES) / (PULSE_TIMER_PRESCALE)) - (MIN_PULSE_TICKS))

// But the user could be enforcing a minimum time, so the loop time is
#define ISR_LOOP_CYCLES (ISR_LOOP_BASE_CYCLES + MAX(MIN_STEPPER_PULSE_CYCLES, MIN_ISR_LOOP_CYCLES))

// If linear advance is enabled, then it is handled separately
#if ENABLED(LIN_ADVANCE)

  // Estimate the minimum LA loop time
  #if ENABLED(MIXING_EXTRUDER) // ToDo: ???
    // HELP ME: What is what?
    // Directions are set up for MIXING_STEPPERS - like before.
    // Finding the right stepper may last up to MIXING_STEPPERS loops in get_next_stepper().
    //   These loops are a bit faster than advancing a bresenham counter.
    // Always only one e-stepper is stepped.
    #define MIN_ISR_LA_LOOP_CYCLES ((MIXING_STEPPERS) * (ISR_STEPPER_CYCLES))
  #else
    #define MIN_ISR_LA_LOOP_CYCLES ISR_STEPPER_CYCLES
  #endif

  // And the real loop time
  #define ISR_LA_LOOP_CYCLES MAX(MIN_STEPPER_PULSE_CYCLES, MIN_ISR_LA_LOOP_CYCLES)

#else
  #define ISR_LA_LOOP_CYCLES 0UL
#endif

// Now estimate the total ISR execution time in cycles given a step per ISR multiplier
#define ISR_EXECUTION_CYCLES(R) (((ISR_BASE_CYCLES + ISR_S_CURVE_CYCLES + (ISR_LOOP_CYCLES) * (R) + ISR_LA_BASE_CYCLES + ISR_LA_LOOP_CYCLES)) / (R))

// The maximum allowable stepping frequency when doing x128-x1 stepping (in Hz)
#define MAX_STEP_ISR_FREQUENCY_128X ((F_CPU) / ISR_EXECUTION_CYCLES(128))
#define MAX_STEP_ISR_FREQUENCY_64X  ((F_CPU) / ISR_EXECUTION_CYCLES(64))
#define MAX_STEP_ISR_FREQUENCY_32X  ((F_CPU) / ISR_EXECUTION_CYCLES(32))
#define MAX_STEP_ISR_FREQUENCY_16X  ((F_CPU) / ISR_EXECUTION_CYCLES(16))
#define MAX_STEP_ISR_FREQUENCY_8X   ((F_CPU) / ISR_EXECUTION_CYCLES(8))
#define MAX_STEP_ISR_FREQUENCY_4X   ((F_CPU) / ISR_EXECUTION_CYCLES(4))
#define MAX_STEP_ISR_FREQUENCY_2X   ((F_CPU) / ISR_EXECUTION_CYCLES(2))
#define MAX_STEP_ISR_FREQUENCY_1X   ((F_CPU) / ISR_EXECUTION_CYCLES(1))

// The minimum allowable frequency for step smoothing will be 1/10 of the maximum nominal frequency (in Hz)
#define MIN_STEP_ISR_FREQUENCY MAX_STEP_ISR_FREQUENCY_1X

//
// Stepper class definition
//

#include "stepper_indirection.h"

#ifdef __AVR__
  #include "speed_lookuptable.h"
#endif

#include "../module/planner.h"
#include "../core/language.h"

class Stepper {

  public:

    #if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS
      static bool separate_multi_axis;
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      #ifndef PWM_MOTOR_CURRENT
        #define PWM_MOTOR_CURRENT DEFAULT_PWM_MOTOR_CURRENT
      #endif
      static uint32_t motor_current_setting[3];
    #endif

  private:

    static block_t* current_block;          // A pointer to the block currently being traced

    static uint8_t last_direction_bits,     // The next stepping-bits to be output
                   axis_did_move;           // Last Movement in the given direction is not null, as computed when the last movement was fetched from planner

    static bool abort_current_block;        // Signals to the stepper that current block should be aborted

    // Last-moved extruder, as set when the last movement was fetched from planner
    #if EXTRUDERS < 2
      static constexpr uint8_t last_moved_extruder = 0;
    #elif DISABLED(MIXING_EXTRUDER)
      static uint8_t last_moved_extruder;
    #endif

    #if ENABLED(X_DUAL_ENDSTOPS)
      static bool locked_X_motor, locked_X2_motor;
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      static bool locked_Y_motor, locked_Y2_motor;
    #endif
    #if Z_MULTI_ENDSTOPS
      static bool locked_Z_motor, locked_Z2_motor;
    #endif
    #if ENABLED(Z_TRIPLE_ENDSTOPS)
      static bool locked_Z3_motor;
    #endif

    static uint32_t acceleration_time, deceleration_time; // time measured in Stepper Timer ticks
    static uint8_t steps_per_isr;         // Count of steps to perform per Stepper ISR call

    #if ENABLED(ADAPTIVE_STEP_SMOOTHING)
      static uint8_t oversampling_factor; // Oversampling factor (log2(multiplier)) to increase temporal resolution of axis
    #else
      static constexpr uint8_t oversampling_factor = 0;
    #endif

    // Delta error variables for the Bresenham line tracer
    static int32_t delta_error[XYZE];
    static uint32_t advance_dividend[XYZE],
                    advance_divisor,
                    step_events_completed,  // The number of step events executed in the current block
                    accelerate_until,       // The point from where we need to stop acceleration
                    decelerate_after,       // The point from where we need to start decelerating
                    step_event_count;       // The total event count for the current block

    #if EXTRUDERS > 1 || ENABLED(MIXING_EXTRUDER)
      static uint8_t stepper_extruder;
    #else
      static constexpr uint8_t stepper_extruder = 0;
    #endif

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
    #if ENABLED(LIN_ADVANCE)
      static uint32_t nextAdvanceISR, LA_isr_rate;
      static uint16_t LA_current_adv_steps, LA_final_adv_steps, LA_max_adv_steps; // Copy from current executed block. Needed because current_block is set to NULL "too early".
      static int8_t LA_steps;
      static bool LA_use_advance_lead;
    #endif // LIN_ADVANCE

    static int32_t ticks_nominal;
    #if DISABLED(S_CURVE_ACCELERATION)
      static uint32_t acc_step_rate; // needed for deceleration start point
    #endif

    static volatile int32_t endstops_trigsteps[XYZ];

    //
    // Positions of stepper motors, in step units
    //
    static volatile int32_t count_position[NUM_AXIS];

    //
    // Current direction of stepper motors (+1 or -1)
    //
    static int8_t count_direction[NUM_AXIS];

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

    // Check if the given block is busy or not - Must not be called from ISR contexts
    static bool is_block_busy(const block_t* const block);

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
    FORCE_INLINE static uint8_t movement_extruder() {
      return
        #if ENABLED(MIXING_EXTRUDER) || EXTRUDERS < 2
          0
        #else
          last_moved_extruder
        #endif
      ;
    }

    // Handle a triggered endstop
    static void endstop_triggered(const AxisEnum axis);

    // Triggered position of an axis in steps
    static int32_t triggered_position(const AxisEnum axis);

    #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM
      static void digitalPotWrite(const int16_t address, const int16_t value);
      static void digipot_current(const uint8_t driver, const int16_t current);
    #endif

    #if HAS_MICROSTEPS
      static void microstep_ms(const uint8_t driver, const int8_t ms1, const int8_t ms2, const int8_t ms3);
      static void microstep_mode(const uint8_t driver, const uint8_t stepping);
      static void microstep_readings();
    #endif

    #if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS
      FORCE_INLINE static void set_separate_multi_axis(const bool state) { separate_multi_axis = state; }
    #endif
    #if ENABLED(X_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_x_lock(const bool state) { locked_X_motor = state; }
      FORCE_INLINE static void set_x2_lock(const bool state) { locked_X2_motor = state; }
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_y_lock(const bool state) { locked_Y_motor = state; }
      FORCE_INLINE static void set_y2_lock(const bool state) { locked_Y2_motor = state; }
    #endif
    #if Z_MULTI_ENDSTOPS
      FORCE_INLINE static void set_z_lock(const bool state) { locked_Z_motor = state; }
      FORCE_INLINE static void set_z2_lock(const bool state) { locked_Z2_motor = state; }
    #endif
    #if ENABLED(Z_TRIPLE_ENDSTOPS)
      FORCE_INLINE static void set_z3_lock(const bool state) { locked_Z3_motor = state; }
    #endif

    #if ENABLED(BABYSTEPPING)
      static void babystep(const AxisEnum axis, const bool direction); // perform a short step with a single stepper motor, outside of any convention
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      static void refresh_motor_power();
    #endif

    // Set the current position in steps
    static inline void set_position(const int32_t &a, const int32_t &b, const int32_t &c, const int32_t &e) {
      planner.synchronize();
      const bool was_enabled = STEPPER_ISR_ENABLED();
      if (was_enabled) DISABLE_STEPPER_DRIVER_INTERRUPT();
      _set_position(a, b, c, e);
      if (was_enabled) ENABLE_STEPPER_DRIVER_INTERRUPT();
    }

    static inline void set_position(const AxisEnum a, const int32_t &v) {
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

    // Set direction bits for all steppers
    static void set_directions();

  private:

    // Set the current position in steps
    static void _set_position(const int32_t &a, const int32_t &b, const int32_t &c, const int32_t &e);

    FORCE_INLINE static uint32_t calc_timer_interval(uint32_t step_rate, uint8_t scale, uint8_t* loops) {
      uint32_t timer;

      // Scale the frequency, as requested by the caller
      step_rate <<= scale;

      uint8_t multistep = 1;
      #if DISABLED(DISABLE_MULTI_STEPPING)

        // The stepping frequency limits for each multistepping rate
        static const uint32_t limit[] PROGMEM = {
          (  MAX_STEP_ISR_FREQUENCY_1X     ),
          (  MAX_STEP_ISR_FREQUENCY_2X >> 1),
          (  MAX_STEP_ISR_FREQUENCY_4X >> 2),
          (  MAX_STEP_ISR_FREQUENCY_8X >> 3),
          ( MAX_STEP_ISR_FREQUENCY_16X >> 4),
          ( MAX_STEP_ISR_FREQUENCY_32X >> 5),
          ( MAX_STEP_ISR_FREQUENCY_64X >> 6),
          (MAX_STEP_ISR_FREQUENCY_128X >> 7)
        };

        // Select the proper multistepping
        uint8_t idx = 0;
        while (idx < 7 && step_rate > (uint32_t)pgm_read_dword(&limit[idx])) {
          step_rate >>= 1;
          multistep <<= 1;
          ++idx;
        };
      #else
        NOMORE(step_rate, uint32_t(MAX_STEP_ISR_FREQUENCY_1X));
      #endif
      *loops = multistep;

      #ifdef CPU_32_BIT
        // In case of high-performance processor, it is able to calculate in real-time
        timer = uint32_t(STEPPER_TIMER_RATE) / step_rate;
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
        // (there is no need to limit the timer value here. All limits have been
        // applied above, and AVR is able to keep up at 30khz Stepping ISR rate)
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

extern Stepper stepper;

#endif // STEPPER_H
