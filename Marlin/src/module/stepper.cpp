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
 * stepper.cpp - A singleton object to execute motion plans using stepper motors
 * Marlin Firmware
 *
 * Derived from Grbl
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

/* The timer calculations of this module informed by the 'RepRap cartesian firmware' by Zack Smith
   and Philipp Tiefenbacher. */

#include "stepper.h"

#ifdef __AVR__
  #include "speed_lookuptable.h"
#endif

#include "endstops.h"
#include "planner.h"
#include "motion.h"

#include "../module/temperature.h"
#include "../lcd/ultralcd.h"
#include "../core/language.h"
#include "../gcode/queue.h"
#include "../sd/cardreader.h"
#include "../Marlin.h"

#if MB(ALLIGATOR)
  #include "../feature/dac/dac_dac084s085.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

Stepper stepper; // Singleton

// public:

block_t* Stepper::current_block = NULL;  // A pointer to the block currently being traced

#if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
  bool Stepper::abort_on_endstop_hit = false;
#endif

#if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
  bool Stepper::performing_homing = false;
#endif

#if HAS_MOTOR_CURRENT_PWM
  uint32_t Stepper::motor_current_setting[3]; // Initialized by settings.load()
#endif

// private:

uint8_t Stepper::last_direction_bits = 0;        // The next stepping-bits to be output
int16_t Stepper::cleaning_buffer_counter = 0;

#if ENABLED(X_DUAL_ENDSTOPS)
  bool Stepper::locked_x_motor = false, Stepper::locked_x2_motor = false;
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  bool Stepper::locked_y_motor = false, Stepper::locked_y2_motor = false;
#endif
#if ENABLED(Z_DUAL_ENDSTOPS)
  bool Stepper::locked_z_motor = false, Stepper::locked_z2_motor = false;
#endif

long Stepper::counter_X = 0,
     Stepper::counter_Y = 0,
     Stepper::counter_Z = 0,
     Stepper::counter_E = 0;

volatile uint32_t Stepper::step_events_completed = 0; // The number of step events executed in the current block

#if ENABLED(LIN_ADVANCE)

  uint32_t Stepper::LA_decelerate_after;

  constexpr hal_timer_t ADV_NEVER = HAL_TIMER_TYPE_MAX;

  hal_timer_t Stepper::nextMainISR = 0,
              Stepper::nextAdvanceISR = ADV_NEVER,
              Stepper::eISR_Rate = ADV_NEVER;
  uint16_t Stepper::current_adv_steps = 0,
           Stepper::final_adv_steps,
           Stepper::max_adv_steps;

  int8_t Stepper::e_steps = 0;

  #if E_STEPPERS > 1
    int8_t Stepper::LA_active_extruder; // Copy from current executed block. Needed because current_block is set to NULL "too early".
  #else
    constexpr int8_t Stepper::LA_active_extruder;
  #endif

  bool Stepper::use_advance_lead;

#endif // LIN_ADVANCE

long Stepper::acceleration_time, Stepper::deceleration_time;

volatile long Stepper::count_position[NUM_AXIS] = { 0 };
volatile signed char Stepper::count_direction[NUM_AXIS] = { 1, 1, 1, 1 };

#if ENABLED(MIXING_EXTRUDER)
  long Stepper::counter_m[MIXING_STEPPERS];
#endif

uint8_t Stepper::step_loops, Stepper::step_loops_nominal;

hal_timer_t Stepper::OCR1A_nominal,
            Stepper::acc_step_rate; // needed for deceleration start point

volatile long Stepper::endstops_trigsteps[XYZ];

#if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
  #define LOCKED_X_MOTOR  locked_x_motor
  #define LOCKED_Y_MOTOR  locked_y_motor
  #define LOCKED_Z_MOTOR  locked_z_motor
  #define LOCKED_X2_MOTOR locked_x2_motor
  #define LOCKED_Y2_MOTOR locked_y2_motor
  #define LOCKED_Z2_MOTOR locked_z2_motor
  #define DUAL_ENDSTOP_APPLY_STEP(AXIS,v)                                                                                                           \
    if (performing_homing) {                                                                                                                        \
      if (AXIS##_HOME_DIR < 0) {                                                                                                                    \
        if (!(TEST(endstops.old_endstop_bits, AXIS##_MIN) && count_direction[AXIS##_AXIS] < 0) && !LOCKED_##AXIS##_MOTOR) AXIS##_STEP_WRITE(v);     \
        if (!(TEST(endstops.old_endstop_bits, AXIS##2_MIN) && count_direction[AXIS##_AXIS] < 0) && !LOCKED_##AXIS##2_MOTOR) AXIS##2_STEP_WRITE(v);  \
      }                                                                                                                                             \
      else {                                                                                                                                        \
        if (!(TEST(endstops.old_endstop_bits, AXIS##_MAX) && count_direction[AXIS##_AXIS] > 0) && !LOCKED_##AXIS##_MOTOR) AXIS##_STEP_WRITE(v);     \
        if (!(TEST(endstops.old_endstop_bits, AXIS##2_MAX) && count_direction[AXIS##_AXIS] > 0) && !LOCKED_##AXIS##2_MOTOR) AXIS##2_STEP_WRITE(v);  \
      }                                                                                                                                             \
    }                                                                                                                                               \
    else {                                                                                                                                          \
      AXIS##_STEP_WRITE(v);                                                                                                                         \
      AXIS##2_STEP_WRITE(v);                                                                                                                        \
    }
#endif

#if ENABLED(X_DUAL_STEPPER_DRIVERS)
  #define X_APPLY_DIR(v,Q) do{ X_DIR_WRITE(v); X2_DIR_WRITE((v) != INVERT_X2_VS_X_DIR); }while(0)
  #if ENABLED(X_DUAL_ENDSTOPS)
    #define X_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(X,v)
  #else
    #define X_APPLY_STEP(v,Q) do{ X_STEP_WRITE(v); X2_STEP_WRITE(v); }while(0)
  #endif
#elif ENABLED(DUAL_X_CARRIAGE)
  #define X_APPLY_DIR(v,ALWAYS) \
    if (extruder_duplication_enabled || ALWAYS) { \
      X_DIR_WRITE(v); \
      X2_DIR_WRITE(v); \
    } \
    else { \
      if (current_block->active_extruder) X2_DIR_WRITE(v); else X_DIR_WRITE(v); \
    }
  #define X_APPLY_STEP(v,ALWAYS) \
    if (extruder_duplication_enabled || ALWAYS) { \
      X_STEP_WRITE(v); \
      X2_STEP_WRITE(v); \
    } \
    else { \
      if (current_block->active_extruder) X2_STEP_WRITE(v); else X_STEP_WRITE(v); \
    }
#else
  #define X_APPLY_DIR(v,Q) X_DIR_WRITE(v)
  #define X_APPLY_STEP(v,Q) X_STEP_WRITE(v)
#endif

#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #define Y_APPLY_DIR(v,Q) do{ Y_DIR_WRITE(v); Y2_DIR_WRITE((v) != INVERT_Y2_VS_Y_DIR); }while(0)
  #if ENABLED(Y_DUAL_ENDSTOPS)
    #define Y_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(Y,v)
  #else
    #define Y_APPLY_STEP(v,Q) do{ Y_STEP_WRITE(v); Y2_STEP_WRITE(v); }while(0)
  #endif
#else
  #define Y_APPLY_DIR(v,Q) Y_DIR_WRITE(v)
  #define Y_APPLY_STEP(v,Q) Y_STEP_WRITE(v)
#endif

#if ENABLED(Z_DUAL_STEPPER_DRIVERS)
  #define Z_APPLY_DIR(v,Q) do{ Z_DIR_WRITE(v); Z2_DIR_WRITE(v); }while(0)
  #if ENABLED(Z_DUAL_ENDSTOPS)
    #define Z_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(Z,v)
  #else
    #define Z_APPLY_STEP(v,Q) do{ Z_STEP_WRITE(v); Z2_STEP_WRITE(v); }while(0)
  #endif
#else
  #define Z_APPLY_DIR(v,Q) Z_DIR_WRITE(v)
  #define Z_APPLY_STEP(v,Q) Z_STEP_WRITE(v)
#endif

#if DISABLED(MIXING_EXTRUDER)
  #define E_APPLY_STEP(v,Q) E_STEP_WRITE(v)
#endif

/**
 *         __________________________
 *        /|                        |\     _________________         ^
 *       / |                        | \   /|               |\        |
 *      /  |                        |  \ / |               | \       s
 *     /   |                        |   |  |               |  \      p
 *    /    |                        |   |  |               |   \     e
 *   +-----+------------------------+---+--+---------------+----+    e
 *   |               BLOCK 1            |      BLOCK 2          |    d
 *
 *                           time ----->
 *
 *  The trapezoid is the shape the speed curve over time. It starts at block->initial_rate, accelerates
 *  first block->accelerate_until step_events_completed, then keeps going at constant speed until
 *  step_events_completed reaches block->decelerate_after after which it decelerates until the trapezoid generator is reset.
 *  The slope of acceleration is calculated using v = u + at where t is the accumulated timer values of the steps so far.
 */
void Stepper::wake_up() {
  // TCNT1 = 0;
  ENABLE_STEPPER_DRIVER_INTERRUPT();
}

/**
 * Set the stepper direction of each axis
 *
 *   COREXY: X_AXIS=A_AXIS and Y_AXIS=B_AXIS
 *   COREXZ: X_AXIS=A_AXIS and Z_AXIS=C_AXIS
 *   COREYZ: Y_AXIS=B_AXIS and Z_AXIS=C_AXIS
 */
void Stepper::set_directions() {

  #define SET_STEP_DIR(AXIS) \
    if (motor_direction(AXIS ##_AXIS)) { \
      AXIS ##_APPLY_DIR(INVERT_## AXIS ##_DIR, false); \
      count_direction[AXIS ##_AXIS] = -1; \
    } \
    else { \
      AXIS ##_APPLY_DIR(!INVERT_## AXIS ##_DIR, false); \
      count_direction[AXIS ##_AXIS] = 1; \
    }

  #if HAS_X_DIR
    SET_STEP_DIR(X); // A
  #endif
  #if HAS_Y_DIR
    SET_STEP_DIR(Y); // B
  #endif
  #if HAS_Z_DIR
    SET_STEP_DIR(Z); // C
  #endif

  #if DISABLED(LIN_ADVANCE)
    if (motor_direction(E_AXIS)) {
      REV_E_DIR();
      count_direction[E_AXIS] = -1;
    }
    else {
      NORM_E_DIR();
      count_direction[E_AXIS] = 1;
    }
  #endif // !LIN_ADVANCE
}

#if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  extern volatile uint8_t e_hit;
#endif

/**
 * Stepper Driver Interrupt
 *
 * Directly pulses the stepper motors at high frequency.
 *
 * AVR :
 * Timer 1 runs at a base frequency of 2MHz, with this ISR using OCR1A compare mode.
 *
 * OCR1A   Frequency
 *     1     2 MHz
 *    50    40 KHz
 *   100    20 KHz - capped max rate
 *   200    10 KHz - nominal max rate
 *  2000     1 KHz - sleep rate
 *  4000   500  Hz - init rate
 */

HAL_STEP_TIMER_ISR {
  HAL_timer_isr_prologue(STEP_TIMER_NUM);
  #if ENABLED(LIN_ADVANCE)
    Stepper::advance_isr_scheduler();
  #else
    Stepper::isr();
  #endif
}

void Stepper::isr() {

  #define ENDSTOP_NOMINAL_OCR_VAL 1500 * HAL_TICKS_PER_US // Check endstops every 1.5ms to guarantee two stepper ISRs within 5ms for BLTouch
  #define OCR_VAL_TOLERANCE        500 * HAL_TICKS_PER_US // First max delay is 2.0ms, last min delay is 0.5ms, all others 1.5ms

  #if DISABLED(LIN_ADVANCE)
    // Disable Timer0 ISRs and enable global ISR again to capture UART events (incoming chars)
    DISABLE_TEMPERATURE_INTERRUPT(); // Temperature ISR
    DISABLE_STEPPER_DRIVER_INTERRUPT();
    #ifndef CPU_32_BIT
      sei();
    #endif
  #endif

  hal_timer_t ocr_val;
  static uint32_t step_remaining = 0;  // SPLIT function always runs.  This allows 16 bit timers to be
                                       // used to generate the stepper ISR.
  #define SPLIT(L) do { \
    if (L > ENDSTOP_NOMINAL_OCR_VAL) { \
      const uint32_t remainder = (uint32_t)L % (ENDSTOP_NOMINAL_OCR_VAL); \
      ocr_val = (remainder < OCR_VAL_TOLERANCE) ? ENDSTOP_NOMINAL_OCR_VAL + remainder : ENDSTOP_NOMINAL_OCR_VAL; \
      step_remaining = (uint32_t)L - ocr_val; \
    } \
    else \
      ocr_val = L;\
  }while(0)

  // Time remaining before the next step?
  if (step_remaining) {

    // Make sure endstops are updated
    if (ENDSTOPS_ENABLED) endstops.update();

    // Next ISR either for endstops or stepping
    ocr_val = step_remaining <= ENDSTOP_NOMINAL_OCR_VAL ? step_remaining : ENDSTOP_NOMINAL_OCR_VAL;
    step_remaining -= ocr_val;
    _NEXT_ISR(ocr_val);

    #if DISABLED(LIN_ADVANCE)
      HAL_timer_restrain(STEP_TIMER_NUM, STEP_TIMER_MIN_INTERVAL * HAL_TICKS_PER_US);
      HAL_ENABLE_ISRs();
    #endif

    return;
  }

  //
  // When cleaning, discard the current block and run fast
  //
  if (cleaning_buffer_counter) {
    if (cleaning_buffer_counter < 0) {          // Count up for endstop hit
      if (current_block) planner.discard_current_block(); // Discard the active block that led to the trigger
      if (!planner.discard_continued_block())   // Discard next CONTINUED block
        cleaning_buffer_counter = 0;            // Keep discarding until non-CONTINUED
    }
    else {
      planner.discard_current_block();
      --cleaning_buffer_counter;                // Count down for abort print
      #if ENABLED(SD_FINISHED_STEPPERRELEASE) && defined(SD_FINISHED_RELEASECOMMAND)
        if (!cleaning_buffer_counter) enqueue_and_echo_commands_P(PSTR(SD_FINISHED_RELEASECOMMAND));
      #endif
    }
    current_block = NULL;                       // Prep to get a new block after cleaning
    _NEXT_ISR(HAL_STEPPER_TIMER_RATE / 10000);  // Run at max speed - 10 KHz
    HAL_ENABLE_ISRs();
    return;
  }

  // If there is no current block, attempt to pop one from the buffer
  if (!current_block) {
    // Anything in the buffer?
    if ((current_block = planner.get_current_block())) {
      trapezoid_generator_reset();

      // Initialize Bresenham counters to 1/2 the ceiling
      counter_X = counter_Y = counter_Z = counter_E = -(current_block->step_event_count >> 1);

      #if ENABLED(MIXING_EXTRUDER)
        MIXING_STEPPERS_LOOP(i)
          counter_m[i] = -(current_block->mix_event_count[i] >> 1);
      #endif

      step_events_completed = 0;

      #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
        e_hit = 2; // Needed for the case an endstop is already triggered before the new move begins.
                   // No 'change' can be detected.
      #endif

      #if ENABLED(Z_LATE_ENABLE)
        if (current_block->steps[Z_AXIS] > 0) {
          enable_Z();
          _NEXT_ISR(HAL_STEPPER_TIMER_RATE / 1000); // Run at slow speed - 1 KHz
          HAL_ENABLE_ISRs();
          return;
        }
      #endif
    }
    else {
      _NEXT_ISR(HAL_STEPPER_TIMER_RATE / 1000); // Run at slow speed - 1 KHz
      HAL_ENABLE_ISRs();
      return;
    }
  }

  // Update endstops state, if enabled
  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    if (e_hit && ENDSTOPS_ENABLED) {
      endstops.update();
      e_hit--;
    }
  #else
    if (ENDSTOPS_ENABLED) endstops.update();
  #endif

  // Take multiple steps per interrupt (For high speed moves)
  bool all_steps_done = false;
  for (uint8_t i = step_loops; i--;) {

    #define _COUNTER(AXIS) counter_## AXIS
    #define _APPLY_STEP(AXIS) AXIS ##_APPLY_STEP
    #define _INVERT_STEP_PIN(AXIS) INVERT_## AXIS ##_STEP_PIN

    // Advance the Bresenham counter; start a pulse if the axis needs a step
    #define PULSE_START(AXIS) do{ \
      _COUNTER(AXIS) += current_block->steps[_AXIS(AXIS)]; \
      if (_COUNTER(AXIS) > 0) { _APPLY_STEP(AXIS)(!_INVERT_STEP_PIN(AXIS), 0); } \
    }while(0)

    // Advance the Bresenham counter; start a pulse if the axis needs a step
    #define STEP_TICK(AXIS) do { \
      if (_COUNTER(AXIS) > 0) { \
        _COUNTER(AXIS) -= current_block->step_event_count; \
        count_position[_AXIS(AXIS)] += count_direction[_AXIS(AXIS)]; \
      } \
    }while(0)

    // Stop an active pulse, if any
    #define PULSE_STOP(AXIS) _APPLY_STEP(AXIS)(_INVERT_STEP_PIN(AXIS), 0)

    /**
     * Estimate the number of cycles that the stepper logic already takes
     * up between the start and stop of the X stepper pulse.
     *
     * Currently this uses very modest estimates of around 5 cycles.
     * True values may be derived by careful testing.
     *
     * Once any delay is added, the cost of the delay code itself
     * may be subtracted from this value to get a more accurate delay.
     * Delays under 20 cycles (1.25µs) will be very accurate, using NOPs.
     * Longer delays use a loop. The resolution is 8 cycles.
     */
    #if HAS_X_STEP
      #define _CYCLE_APPROX_1 5
    #else
      #define _CYCLE_APPROX_1 0
    #endif
    #if ENABLED(X_DUAL_STEPPER_DRIVERS)
      #define _CYCLE_APPROX_2 _CYCLE_APPROX_1 + 4
    #else
      #define _CYCLE_APPROX_2 _CYCLE_APPROX_1
    #endif
    #if HAS_Y_STEP
      #define _CYCLE_APPROX_3 _CYCLE_APPROX_2 + 5
    #else
      #define _CYCLE_APPROX_3 _CYCLE_APPROX_2
    #endif
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
      #define _CYCLE_APPROX_4 _CYCLE_APPROX_3 + 4
    #else
      #define _CYCLE_APPROX_4 _CYCLE_APPROX_3
    #endif
    #if HAS_Z_STEP
      #define _CYCLE_APPROX_5 _CYCLE_APPROX_4 + 5
    #else
      #define _CYCLE_APPROX_5 _CYCLE_APPROX_4
    #endif
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS)
      #define _CYCLE_APPROX_6 _CYCLE_APPROX_5 + 4
    #else
      #define _CYCLE_APPROX_6 _CYCLE_APPROX_5
    #endif
    #if DISABLED(LIN_ADVANCE)
      #if ENABLED(MIXING_EXTRUDER)
        #define _CYCLE_APPROX_7 _CYCLE_APPROX_6 + (MIXING_STEPPERS) * 6
      #else
        #define _CYCLE_APPROX_7 _CYCLE_APPROX_6 + 5
      #endif
    #else
      #define _CYCLE_APPROX_7 _CYCLE_APPROX_6
    #endif

    #define CYCLES_EATEN_XYZE _CYCLE_APPROX_7
    #define EXTRA_CYCLES_XYZE (STEP_PULSE_CYCLES - (CYCLES_EATEN_XYZE))

    /**
     * If a minimum pulse time was specified get the timer 0 value.
     *
     * On AVR the TCNT0 timer has an 8x prescaler, so it increments every 8 cycles.
     * That's every 0.5µs on 16MHz and every 0.4µs on 20MHz.
     * 20 counts of TCNT0 -by itself- is a good pulse delay.
     * 10µs = 160 or 200 cycles.
     */
    #if EXTRA_CYCLES_XYZE > 20
      hal_timer_t pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
    #endif

    #if HAS_X_STEP
      PULSE_START(X);
    #endif
    #if HAS_Y_STEP
      PULSE_START(Y);
    #endif
    #if HAS_Z_STEP
      PULSE_START(Z);
    #endif

    #if ENABLED(LIN_ADVANCE)

      counter_E += current_block->steps[E_AXIS];
      if (counter_E > 0) {
        #if DISABLED(MIXING_EXTRUDER)
          // Don't step E here for mixing extruder
          motor_direction(E_AXIS) ? --e_steps : ++e_steps;
        #endif
      }

      #if ENABLED(MIXING_EXTRUDER)
        // Step mixing steppers proportionally
        const bool dir = motor_direction(E_AXIS);
        MIXING_STEPPERS_LOOP(j) {
          counter_m[j] += current_block->steps[E_AXIS];
          if (counter_m[j] > 0) {
            counter_m[j] -= current_block->mix_event_count[j];
            dir ? --e_steps[j] : ++e_steps[j];
          }
        }
      #endif

    #else // !LIN_ADVANCE - use linear interpolation for E also

      #if ENABLED(MIXING_EXTRUDER)
        // Keep updating the single E axis
        counter_E += current_block->steps[E_AXIS];
        // Tick the counters used for this mix
        MIXING_STEPPERS_LOOP(j) {
          // Step mixing steppers (proportionally)
          counter_m[j] += current_block->steps[E_AXIS];
          // Step when the counter goes over zero
          if (counter_m[j] > 0) En_STEP_WRITE(j, !INVERT_E_STEP_PIN);
        }
      #else // !MIXING_EXTRUDER
        PULSE_START(E);
      #endif
    #endif // !LIN_ADVANCE

    #if HAS_X_STEP
      STEP_TICK(X);
    #endif
    #if HAS_Y_STEP
      STEP_TICK(Y);
    #endif
    #if HAS_Z_STEP
      STEP_TICK(Z);
    #endif

    STEP_TICK(E); // Always tick the single E axis

    // For minimum pulse time wait before stopping pulses
    #if EXTRA_CYCLES_XYZE > 20
      while (EXTRA_CYCLES_XYZE > (uint32_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
      pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
    #elif EXTRA_CYCLES_XYZE > 0
      DELAY_NOPS(EXTRA_CYCLES_XYZE);
    #endif

    #if HAS_X_STEP
      PULSE_STOP(X);
    #endif
    #if HAS_Y_STEP
      PULSE_STOP(Y);
    #endif
    #if HAS_Z_STEP
      PULSE_STOP(Z);
    #endif

    #if DISABLED(LIN_ADVANCE)
      #if ENABLED(MIXING_EXTRUDER)
        MIXING_STEPPERS_LOOP(j) {
          if (counter_m[j] > 0) {
            counter_m[j] -= current_block->mix_event_count[j];
            En_STEP_WRITE(j, INVERT_E_STEP_PIN);
          }
        }
      #else // !MIXING_EXTRUDER
        PULSE_STOP(E);
      #endif
    #endif // !LIN_ADVANCE

    if (++step_events_completed >= current_block->step_event_count) {
      all_steps_done = true;
      break;
    }

    // For minimum pulse time wait after stopping pulses also
    #if EXTRA_CYCLES_XYZE > 20
      if (i) while (EXTRA_CYCLES_XYZE > (uint32_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
    #elif EXTRA_CYCLES_XYZE > 0
      if (i) DELAY_NOPS(EXTRA_CYCLES_XYZE);
    #endif

  } // steps_loop

  // Calculate new timer value
  if (step_events_completed <= (uint32_t)current_block->accelerate_until) {

    #ifdef CPU_32_BIT
      MultiU32X24toH32(acc_step_rate, acceleration_time, current_block->acceleration_rate);
    #else
      MultiU24X32toH16(acc_step_rate, acceleration_time, current_block->acceleration_rate);
    #endif
    acc_step_rate += current_block->initial_rate;

    // upper limit
    NOMORE(acc_step_rate, current_block->nominal_rate);

    // step_rate to timer interval
    const hal_timer_t interval = calc_timer_interval(acc_step_rate);

    SPLIT(interval);  // split step into multiple ISRs if larger than ENDSTOP_NOMINAL_OCR_VAL
    _NEXT_ISR(ocr_val);

    acceleration_time += interval;

    #if ENABLED(LIN_ADVANCE)

      if (current_block->use_advance_lead) {
        if (step_events_completed == step_loops || (e_steps && eISR_Rate != current_block->advance_speed)) {
          nextAdvanceISR = 0; // Wake up eISR on first acceleration loop and fire ISR if final adv_rate is reached
          eISR_Rate = current_block->advance_speed;
        }
      }
      else {
        eISR_Rate = ADV_NEVER;
        if (e_steps) nextAdvanceISR = 0;
      }

    #endif // LIN_ADVANCE
  }
  else if (step_events_completed > (uint32_t)current_block->decelerate_after) {
    hal_timer_t step_rate;
    #ifdef CPU_32_BIT
      MultiU32X24toH32(step_rate, deceleration_time, current_block->acceleration_rate);
    #else
      MultiU24X32toH16(step_rate, deceleration_time, current_block->acceleration_rate);
    #endif

    if (step_rate < acc_step_rate) { // Still decelerating?
      step_rate = acc_step_rate - step_rate;
      NOLESS(step_rate, current_block->final_rate);
    }
    else
      step_rate = current_block->final_rate;

    // step_rate to timer interval
    const hal_timer_t interval = calc_timer_interval(step_rate);

    SPLIT(interval);  // split step into multiple ISRs if larger than ENDSTOP_NOMINAL_OCR_VAL
    _NEXT_ISR(ocr_val);

    deceleration_time += interval;

    #if ENABLED(LIN_ADVANCE)

      if (current_block->use_advance_lead) {
        if (step_events_completed <= (uint32_t)current_block->decelerate_after + step_loops || (e_steps && eISR_Rate != current_block->advance_speed)) {
          nextAdvanceISR = 0; // Wake up eISR on first deceleration loop
          eISR_Rate = current_block->advance_speed;
        }
      }
      else {
        eISR_Rate = ADV_NEVER;
        if (e_steps) nextAdvanceISR = 0;
      }

    #endif // LIN_ADVANCE
  }
  else {

    #if ENABLED(LIN_ADVANCE)

      // If we have esteps to execute, fire the next advance_isr "now"
      if (e_steps && eISR_Rate != current_block->advance_speed) nextAdvanceISR = 0;

    #endif

    SPLIT(OCR1A_nominal);  // split step into multiple ISRs if larger than ENDSTOP_NOMINAL_OCR_VAL
    _NEXT_ISR(ocr_val);

    // ensure we're running at the correct step rate, even if we just came off an acceleration
    step_loops = step_loops_nominal;
  }

  #if DISABLED(LIN_ADVANCE)
    // Make sure stepper ISR doesn't monopolize the CPU
    HAL_timer_restrain(STEP_TIMER_NUM, STEP_TIMER_MIN_INTERVAL * HAL_TICKS_PER_US);
  #endif

  // If current block is finished, reset pointer
  if (all_steps_done) {
    current_block = NULL;
    planner.discard_current_block();
  }
  #if DISABLED(LIN_ADVANCE)
    HAL_ENABLE_ISRs();
  #endif
}

#if ENABLED(LIN_ADVANCE)

  #define CYCLES_EATEN_E (E_STEPPERS * 5)
  #define EXTRA_CYCLES_E (STEP_PULSE_CYCLES - (CYCLES_EATEN_E))

  // Timer interrupt for E. e_steps is set in the main routine;

  void Stepper::advance_isr() {

    #if ENABLED(MK2_MULTIPLEXER) // For SNMM even-numbered steppers are reversed
      #define SET_E_STEP_DIR(INDEX) do{ if (e_steps) E0_DIR_WRITE(e_steps < 0 ? !INVERT_E## INDEX ##_DIR ^ TEST(INDEX, 0) : INVERT_E## INDEX ##_DIR ^ TEST(INDEX, 0)); }while(0)
    #elif ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
      #define SET_E_STEP_DIR(INDEX) do{ if (e_steps) { if (e_steps < 0) REV_E_DIR(); else NORM_E_DIR(); } }while(0)
    #else
      #define SET_E_STEP_DIR(INDEX) do{ if (e_steps) E## INDEX ##_DIR_WRITE(e_steps < 0 ? INVERT_E## INDEX ##_DIR : !INVERT_E## INDEX ##_DIR); }while(0)
    #endif

    #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
      #define START_E_PULSE(INDEX) do{ if (e_steps) E_STEP_WRITE(!INVERT_E_STEP_PIN); }while(0)
      #define STOP_E_PULSE(INDEX) do{ if (e_steps) { E_STEP_WRITE(INVERT_E_STEP_PIN); e_steps < 0 ? ++e_steps : --e_steps; } }while(0)
    #else
      #define START_E_PULSE(INDEX) do{ if (e_steps) E## INDEX ##_STEP_WRITE(!INVERT_E_STEP_PIN); }while(0)
      #define STOP_E_PULSE(INDEX) do { if (e_steps) { e_steps < 0 ? ++e_steps : --e_steps; E## INDEX ##_STEP_WRITE(INVERT_E_STEP_PIN); } }while(0)
    #endif

    if (use_advance_lead) {
      if (step_events_completed > LA_decelerate_after && current_adv_steps > final_adv_steps) {
        e_steps--;
        current_adv_steps--;
        nextAdvanceISR = eISR_Rate;
      }
      else if (step_events_completed < LA_decelerate_after && current_adv_steps < max_adv_steps) {
             //step_events_completed <= (uint32_t)current_block->accelerate_until) {
        e_steps++;
        current_adv_steps++;
        nextAdvanceISR = eISR_Rate;
      }
      else {
        nextAdvanceISR = ADV_NEVER;
        eISR_Rate = ADV_NEVER;
      }
    }
    else
      nextAdvanceISR = ADV_NEVER;

    switch (LA_active_extruder) {
      case 0: SET_E_STEP_DIR(0); break;
      #if EXTRUDERS > 1
        case 1: SET_E_STEP_DIR(1); break;
        #if EXTRUDERS > 2
          case 2: SET_E_STEP_DIR(2); break;
          #if EXTRUDERS > 3
            case 3: SET_E_STEP_DIR(3); break;
            #if EXTRUDERS > 4
              case 4: SET_E_STEP_DIR(4); break;
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1
    }

    // Step E stepper if we have steps
    while (e_steps) {

      #if EXTRA_CYCLES_E > 20
        hal_timer_t pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
      #endif

      switch (LA_active_extruder) {
        case 0: START_E_PULSE(0); break;
        #if EXTRUDERS > 1
          case 1: START_E_PULSE(1); break;
          #if EXTRUDERS > 2
            case 2: START_E_PULSE(2); break;
            #if EXTRUDERS > 3
              case 3: START_E_PULSE(3); break;
              #if EXTRUDERS > 4
                case 4: START_E_PULSE(4); break;
              #endif // EXTRUDERS > 4
            #endif // EXTRUDERS > 3
          #endif // EXTRUDERS > 2
        #endif // EXTRUDERS > 1
      }

      // For minimum pulse time wait before stopping pulses
      #if EXTRA_CYCLES_E > 20
        while (EXTRA_CYCLES_E > (hal_timer_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
        pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
      #elif EXTRA_CYCLES_E > 0
        DELAY_NOPS(EXTRA_CYCLES_E);
      #endif

      switch (LA_active_extruder) {
        case 0: STOP_E_PULSE(0); break;
        #if EXTRUDERS > 1
          case 1: STOP_E_PULSE(1); break;
          #if EXTRUDERS > 2
            case 2: STOP_E_PULSE(2); break;
            #if EXTRUDERS > 3
              case 3: STOP_E_PULSE(3); break;
              #if EXTRUDERS > 4
                case 4: STOP_E_PULSE(4); break;
              #endif // EXTRUDERS > 4
            #endif // EXTRUDERS > 3
          #endif // EXTRUDERS > 2
        #endif // EXTRUDERS > 1
      }

      // For minimum pulse time wait before looping
      #if EXTRA_CYCLES_E > 20
        if (e_steps) while (EXTRA_CYCLES_E > (hal_timer_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
      #elif EXTRA_CYCLES_E > 0
        if (e_steps) DELAY_NOPS(EXTRA_CYCLES_E);
      #endif

    } // e_steps
  }

  void Stepper::advance_isr_scheduler() {
    // Disable Timer0 ISRs and enable global ISR again to capture UART events (incoming chars)
    DISABLE_TEMPERATURE_INTERRUPT(); // Temperature ISR
    DISABLE_STEPPER_DRIVER_INTERRUPT();
    sei();

    // Run main stepping ISR if flagged
    if (!nextMainISR) isr();

    // Run Advance stepping ISR if flagged
    if (!nextAdvanceISR) advance_isr();

    // Is the next advance ISR scheduled before the next main ISR?
    if (nextAdvanceISR <= nextMainISR) {
      // Set up the next interrupt
      HAL_timer_set_compare(STEP_TIMER_NUM, nextAdvanceISR);
      // New interval for the next main ISR
      if (nextMainISR) nextMainISR -= nextAdvanceISR;
      // Will call Stepper::advance_isr on the next interrupt
      nextAdvanceISR = 0;
    }
    else {
      // The next main ISR comes first
      HAL_timer_set_compare(STEP_TIMER_NUM, nextMainISR);
      // New interval for the next advance ISR, if any
      if (nextAdvanceISR && nextAdvanceISR != ADV_NEVER)
        nextAdvanceISR -= nextMainISR;
      // Will call Stepper::isr on the next interrupt
      nextMainISR = 0;
    }

    // Make sure stepper ISR doesn't monopolize the CPU
    HAL_timer_restrain(STEP_TIMER_NUM, STEP_TIMER_MIN_INTERVAL * HAL_TICKS_PER_US);

    // Restore original ISR settings
    HAL_ENABLE_ISRs();
  }

#endif // LIN_ADVANCE

void Stepper::init() {

  // Init Digipot Motor Current
  #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM
    digipot_init();
  #endif

  #if MB(ALLIGATOR)
    const float motor_current[] = MOTOR_CURRENT;
    unsigned int digipot_motor = 0;
    for (uint8_t i = 0; i < 3 + EXTRUDERS; i++) {
      digipot_motor = 255 * (motor_current[i] / 2.5);
      dac084s085::setValue(i, digipot_motor);
    }
  #endif//MB(ALLIGATOR)

  // Init Microstepping Pins
  #if HAS_MICROSTEPS
    microstep_init();
  #endif

  // Init TMC Steppers
  #if ENABLED(HAVE_TMC26X)
    tmc26x_init();
  #endif

  // Init TMC2130 Steppers
  #if ENABLED(HAVE_TMC2130)
    tmc2130_init();
  #endif

  // Init TMC2208 Steppers
  #if ENABLED(HAVE_TMC2208)
    tmc2208_init();
  #endif

  // TRAMS, TMC2130 and TMC2208 advanced settings
  #if HAS_TRINAMIC
    TMC_ADV()
  #endif

  // Init L6470 Steppers
  #if ENABLED(HAVE_L6470DRIVER)
    L6470_init();
  #endif

  // Init Dir Pins
  #if HAS_X_DIR
    X_DIR_INIT;
  #endif
  #if HAS_X2_DIR
    X2_DIR_INIT;
  #endif
  #if HAS_Y_DIR
    Y_DIR_INIT;
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS) && HAS_Y2_DIR
      Y2_DIR_INIT;
    #endif
  #endif
  #if HAS_Z_DIR
    Z_DIR_INIT;
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS) && HAS_Z2_DIR
      Z2_DIR_INIT;
    #endif
  #endif
  #if HAS_E0_DIR
    E0_DIR_INIT;
  #endif
  #if HAS_E1_DIR
    E1_DIR_INIT;
  #endif
  #if HAS_E2_DIR
    E2_DIR_INIT;
  #endif
  #if HAS_E3_DIR
    E3_DIR_INIT;
  #endif
  #if HAS_E4_DIR
    E4_DIR_INIT;
  #endif

  // Init Enable Pins - steppers default to disabled.
  #if HAS_X_ENABLE
    X_ENABLE_INIT;
    if (!X_ENABLE_ON) X_ENABLE_WRITE(HIGH);
    #if (ENABLED(DUAL_X_CARRIAGE) || ENABLED(X_DUAL_STEPPER_DRIVERS)) && HAS_X2_ENABLE
      X2_ENABLE_INIT;
      if (!X_ENABLE_ON) X2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_Y_ENABLE
    Y_ENABLE_INIT;
    if (!Y_ENABLE_ON) Y_ENABLE_WRITE(HIGH);
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS) && HAS_Y2_ENABLE
      Y2_ENABLE_INIT;
      if (!Y_ENABLE_ON) Y2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_Z_ENABLE
    Z_ENABLE_INIT;
    if (!Z_ENABLE_ON) Z_ENABLE_WRITE(HIGH);
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS) && HAS_Z2_ENABLE
      Z2_ENABLE_INIT;
      if (!Z_ENABLE_ON) Z2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_E0_ENABLE
    E0_ENABLE_INIT;
    if (!E_ENABLE_ON) E0_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E1_ENABLE
    E1_ENABLE_INIT;
    if (!E_ENABLE_ON) E1_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E2_ENABLE
    E2_ENABLE_INIT;
    if (!E_ENABLE_ON) E2_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E3_ENABLE
    E3_ENABLE_INIT;
    if (!E_ENABLE_ON) E3_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E4_ENABLE
    E4_ENABLE_INIT;
    if (!E_ENABLE_ON) E4_ENABLE_WRITE(HIGH);
  #endif

  // Init endstops and pullups
  endstops.init();

  #define _STEP_INIT(AXIS) AXIS ##_STEP_INIT
  #define _WRITE_STEP(AXIS, HIGHLOW) AXIS ##_STEP_WRITE(HIGHLOW)
  #define _DISABLE(AXIS) disable_## AXIS()

  #define AXIS_INIT(AXIS, PIN) \
    _STEP_INIT(AXIS); \
    _WRITE_STEP(AXIS, _INVERT_STEP_PIN(PIN)); \
    _DISABLE(AXIS)

  #define E_AXIS_INIT(NUM) AXIS_INIT(E## NUM, E)

  // Init Step Pins
  #if HAS_X_STEP
    #if ENABLED(X_DUAL_STEPPER_DRIVERS) || ENABLED(DUAL_X_CARRIAGE)
      X2_STEP_INIT;
      X2_STEP_WRITE(INVERT_X_STEP_PIN);
    #endif
    AXIS_INIT(X, X);
  #endif

  #if HAS_Y_STEP
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
      Y2_STEP_INIT;
      Y2_STEP_WRITE(INVERT_Y_STEP_PIN);
    #endif
    AXIS_INIT(Y, Y);
  #endif

  #if HAS_Z_STEP
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS)
      Z2_STEP_INIT;
      Z2_STEP_WRITE(INVERT_Z_STEP_PIN);
    #endif
    AXIS_INIT(Z, Z);
  #endif

  #if HAS_E0_STEP
    E_AXIS_INIT(0);
  #endif
  #if HAS_E1_STEP
    E_AXIS_INIT(1);
  #endif
  #if HAS_E2_STEP
    E_AXIS_INIT(2);
  #endif
  #if HAS_E3_STEP
    E_AXIS_INIT(3);
  #endif
  #if HAS_E4_STEP
    E_AXIS_INIT(4);
  #endif

  #ifdef __AVR__
    // waveform generation = 0100 = CTC
    SET_WGM(1, CTC_OCRnA);

    // output mode = 00 (disconnected)
    SET_COMA(1, NORMAL);

    // Set the timer pre-scaler
    // Generally we use a divider of 8, resulting in a 2MHz timer
    // frequency on a 16MHz MCU. If you are going to change this, be
    // sure to regenerate speed_lookuptable.h with
    // create_speed_lookuptable.py
    SET_CS(1, PRESCALER_8);  //  CS 2 = 1/8 prescaler

    // Init Stepper ISR to 122 Hz for quick starting
    OCR1A = 0x4000;
    TCNT1 = 0;
  #else
    // Init Stepper ISR to 122 Hz for quick starting
    HAL_timer_start(STEP_TIMER_NUM, 122);
  #endif

  ENABLE_STEPPER_DRIVER_INTERRUPT();

  endstops.enable(true); // Start with endstops active. After homing they can be disabled
  sei();

  set_directions(); // Init directions to last_direction_bits = 0
}


/**
 * Block until all buffered steps are executed / cleaned
 */
void Stepper::synchronize() { while (planner.blocks_queued() || cleaning_buffer_counter) idle(); }

/**
 * Set the stepper positions directly in steps
 *
 * The input is based on the typical per-axis XYZ steps.
 * For CORE machines XYZ needs to be translated to ABC.
 *
 * This allows get_axis_position_mm to correctly
 * derive the current XYZ position later on.
 */
void Stepper::set_position(const long &a, const long &b, const long &c, const long &e) {

  synchronize(); // Bad to set stepper counts in the middle of a move

  CRITICAL_SECTION_START;

  #if CORE_IS_XY
    // corexy positioning
    // these equations follow the form of the dA and dB equations on http://www.corexy.com/theory.html
    count_position[A_AXIS] = a + b;
    count_position[B_AXIS] = CORESIGN(a - b);
    count_position[Z_AXIS] = c;
  #elif CORE_IS_XZ
    // corexz planning
    count_position[A_AXIS] = a + c;
    count_position[Y_AXIS] = b;
    count_position[C_AXIS] = CORESIGN(a - c);
  #elif CORE_IS_YZ
    // coreyz planning
    count_position[X_AXIS] = a;
    count_position[B_AXIS] = b + c;
    count_position[C_AXIS] = CORESIGN(b - c);
  #else
    // default non-h-bot planning
    count_position[X_AXIS] = a;
    count_position[Y_AXIS] = b;
    count_position[Z_AXIS] = c;
  #endif

  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

void Stepper::set_position(const AxisEnum &axis, const long &v) {
  CRITICAL_SECTION_START;
  count_position[axis] = v;
  CRITICAL_SECTION_END;
}

void Stepper::set_e_position(const long &e) {
  CRITICAL_SECTION_START;
  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

/**
 * Get a stepper's position in steps.
 */
long Stepper::position(const AxisEnum axis) {
  CRITICAL_SECTION_START;
  const long count_pos = count_position[axis];
  CRITICAL_SECTION_END;
  return count_pos;
}

/**
 * Get an axis position according to stepper position(s)
 * For CORE machines apply translation from ABC to XYZ.
 */
float Stepper::get_axis_position_mm(const AxisEnum axis) {
  float axis_steps;
  #if IS_CORE
    // Requesting one of the "core" axes?
    if (axis == CORE_AXIS_1 || axis == CORE_AXIS_2) {
      CRITICAL_SECTION_START;
      // ((a1+a2)+(a1-a2))/2 -> (a1+a2+a1-a2)/2 -> (a1+a1)/2 -> a1
      // ((a1+a2)-(a1-a2))/2 -> (a1+a2-a1+a2)/2 -> (a2+a2)/2 -> a2
      axis_steps = 0.5f * (
        axis == CORE_AXIS_2 ? CORESIGN(count_position[CORE_AXIS_1] - count_position[CORE_AXIS_2])
                            : count_position[CORE_AXIS_1] + count_position[CORE_AXIS_2]
      );
      CRITICAL_SECTION_END;
    }
    else
      axis_steps = position(axis);
  #else
    axis_steps = position(axis);
  #endif
  return axis_steps * planner.steps_to_mm[axis];
}

void Stepper::finish_and_disable() {
  synchronize();
  disable_all_steppers();
}

void Stepper::quick_stop() {
  cleaning_buffer_counter = 5000;
  DISABLE_STEPPER_DRIVER_INTERRUPT();
  while (planner.blocks_queued()) planner.discard_current_block();
  current_block = NULL;
  ENABLE_STEPPER_DRIVER_INTERRUPT();
  #if ENABLED(ULTRA_LCD)
    planner.clear_block_buffer_runtime();
  #endif
}

void Stepper::endstop_triggered(const AxisEnum axis) {

  #if IS_CORE

    endstops_trigsteps[axis] = 0.5f * (
      axis == CORE_AXIS_2 ? CORESIGN(count_position[CORE_AXIS_1] - count_position[CORE_AXIS_2])
                          : count_position[CORE_AXIS_1] + count_position[CORE_AXIS_2]
    );

  #else // !COREXY && !COREXZ && !COREYZ

    endstops_trigsteps[axis] = count_position[axis];

  #endif // !COREXY && !COREXZ && !COREYZ

  kill_current_block();
  cleaning_buffer_counter = -1; // Discard the rest of the move
}

void Stepper::report_positions() {
  CRITICAL_SECTION_START;
  const long xpos = count_position[X_AXIS],
             ypos = count_position[Y_AXIS],
             zpos = count_position[Z_AXIS];
  CRITICAL_SECTION_END;

  #if CORE_IS_XY || CORE_IS_XZ || IS_SCARA
    SERIAL_PROTOCOLPGM(MSG_COUNT_A);
  #else
    SERIAL_PROTOCOLPGM(MSG_COUNT_X);
  #endif
  SERIAL_PROTOCOL(xpos);

  #if CORE_IS_XY || CORE_IS_YZ || IS_SCARA
    SERIAL_PROTOCOLPGM(" B:");
  #else
    SERIAL_PROTOCOLPGM(" Y:");
  #endif
  SERIAL_PROTOCOL(ypos);

  #if CORE_IS_XZ || CORE_IS_YZ
    SERIAL_PROTOCOLPGM(" C:");
  #else
    SERIAL_PROTOCOLPGM(" Z:");
  #endif
  SERIAL_PROTOCOL(zpos);

  SERIAL_EOL();
}

#if ENABLED(BABYSTEPPING)

  #if ENABLED(DELTA)
    #define CYCLES_EATEN_BABYSTEP (2 * 15)
  #else
    #define CYCLES_EATEN_BABYSTEP 0
  #endif
  #define EXTRA_CYCLES_BABYSTEP (STEP_PULSE_CYCLES - (CYCLES_EATEN_BABYSTEP))

  #define _ENABLE(AXIS) enable_## AXIS()
  #define _READ_DIR(AXIS) AXIS ##_DIR_READ
  #define _INVERT_DIR(AXIS) INVERT_## AXIS ##_DIR
  #define _APPLY_DIR(AXIS, INVERT) AXIS ##_APPLY_DIR(INVERT, true)

  #if EXTRA_CYCLES_BABYSTEP > 20
    #define _SAVE_START const hal_timer_t pulse_start = HAL_timer_get_count(STEP_TIMER_NUM)
    #define _PULSE_WAIT while (EXTRA_CYCLES_BABYSTEP > (uint32_t)(HAL_timer_get_count(STEP_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
  #else
    #define _SAVE_START NOOP
    #if EXTRA_CYCLES_BABYSTEP > 0
      #define _PULSE_WAIT DELAY_NOPS(EXTRA_CYCLES_BABYSTEP)
    #elif STEP_PULSE_CYCLES > 0
      #define _PULSE_WAIT NOOP
    #elif ENABLED(DELTA)
      #define _PULSE_WAIT delayMicroseconds(2);
    #else
      #define _PULSE_WAIT delayMicroseconds(4);
    #endif
  #endif

  #define BABYSTEP_AXIS(AXIS, INVERT, DIR) {            \
      const uint8_t old_dir = _READ_DIR(AXIS);          \
      _ENABLE(AXIS);                                    \
      _SAVE_START;                                      \
      _APPLY_DIR(AXIS, _INVERT_DIR(AXIS)^DIR^INVERT);   \
      _PULSE_WAIT;                                      \
      _APPLY_STEP(AXIS)(!_INVERT_STEP_PIN(AXIS), true); \
      _PULSE_WAIT;                                      \
      _APPLY_STEP(AXIS)(_INVERT_STEP_PIN(AXIS), true);  \
      _APPLY_DIR(AXIS, old_dir);                        \
    }

  // MUST ONLY BE CALLED BY AN ISR,
  // No other ISR should ever interrupt this!
  void Stepper::babystep(const AxisEnum axis, const bool direction) {
    cli();

    switch (axis) {

      #if ENABLED(BABYSTEP_XY)

        case X_AXIS:
          #if CORE_IS_XY
            BABYSTEP_AXIS(X, false, direction);
            BABYSTEP_AXIS(Y, false, direction);
          #elif CORE_IS_XZ
            BABYSTEP_AXIS(X, false, direction);
            BABYSTEP_AXIS(Z, false, direction);
          #else
            BABYSTEP_AXIS(X, false, direction);
          #endif
          break;

        case Y_AXIS:
          #if CORE_IS_XY
            BABYSTEP_AXIS(X, false, direction);
            BABYSTEP_AXIS(Y, false, direction^(CORESIGN(1)<0));
          #elif CORE_IS_YZ
            BABYSTEP_AXIS(Y, false, direction);
            BABYSTEP_AXIS(Z, false, direction^(CORESIGN(1)<0));
          #else
            BABYSTEP_AXIS(Y, false, direction);
          #endif
          break;

      #endif

      case Z_AXIS: {

        #if CORE_IS_XZ
          BABYSTEP_AXIS(X, BABYSTEP_INVERT_Z, direction);
          BABYSTEP_AXIS(Z, BABYSTEP_INVERT_Z, direction^(CORESIGN(1)<0));

        #elif CORE_IS_YZ
          BABYSTEP_AXIS(Y, BABYSTEP_INVERT_Z, direction);
          BABYSTEP_AXIS(Z, BABYSTEP_INVERT_Z, direction^(CORESIGN(1)<0));

        #elif DISABLED(DELTA)
          BABYSTEP_AXIS(Z, BABYSTEP_INVERT_Z, direction);

        #else // DELTA

          const bool z_direction = direction ^ BABYSTEP_INVERT_Z;

          enable_X();
          enable_Y();
          enable_Z();

          const uint8_t old_x_dir_pin = X_DIR_READ,
                        old_y_dir_pin = Y_DIR_READ,
                        old_z_dir_pin = Z_DIR_READ;

          X_DIR_WRITE(INVERT_X_DIR ^ z_direction);
          Y_DIR_WRITE(INVERT_Y_DIR ^ z_direction);
          Z_DIR_WRITE(INVERT_Z_DIR ^ z_direction);

          _SAVE_START;

          X_STEP_WRITE(!INVERT_X_STEP_PIN);
          Y_STEP_WRITE(!INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(!INVERT_Z_STEP_PIN);

          _PULSE_WAIT;

          X_STEP_WRITE(INVERT_X_STEP_PIN);
          Y_STEP_WRITE(INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(INVERT_Z_STEP_PIN);

          // Restore direction bits
          X_DIR_WRITE(old_x_dir_pin);
          Y_DIR_WRITE(old_y_dir_pin);
          Z_DIR_WRITE(old_z_dir_pin);

        #endif

      } break;

      default: break;
    }
    sei();
  }

#endif // BABYSTEPPING

/**
 * Software-controlled Stepper Motor Current
 */

#if HAS_DIGIPOTSS

  // From Arduino DigitalPotControl example
  void Stepper::digitalPotWrite(const int16_t address, const int16_t value) {
    WRITE(DIGIPOTSS_PIN, LOW);  // Take the SS pin low to select the chip
    SPI.transfer(address);      // Send the address and value via SPI
    SPI.transfer(value);
    WRITE(DIGIPOTSS_PIN, HIGH); // Take the SS pin high to de-select the chip
    //delay(10);
  }

#endif // HAS_DIGIPOTSS

#if HAS_MOTOR_CURRENT_PWM

  void Stepper::refresh_motor_power() {
    for (uint8_t i = 0; i < COUNT(motor_current_setting); ++i) {
      switch (i) {
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
          case 0:
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
          case 1:
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
          case 2:
        #endif
            digipot_current(i, motor_current_setting[i]);
        default: break;
      }
    }
  }

#endif // HAS_MOTOR_CURRENT_PWM

#if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM

  void Stepper::digipot_current(const uint8_t driver, const int current) {

    #if HAS_DIGIPOTSS

      const uint8_t digipot_ch[] = DIGIPOT_CHANNELS;
      digitalPotWrite(digipot_ch[driver], current);

    #elif HAS_MOTOR_CURRENT_PWM

      if (WITHIN(driver, 0, 2))
        motor_current_setting[driver] = current; // update motor_current_setting

      #define _WRITE_CURRENT_PWM(P) analogWrite(MOTOR_CURRENT_PWM_## P ##_PIN, 255L * current / (MOTOR_CURRENT_PWM_RANGE))
      switch (driver) {
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
          case 0: _WRITE_CURRENT_PWM(XY); break;
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
          case 1: _WRITE_CURRENT_PWM(Z); break;
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
          case 2: _WRITE_CURRENT_PWM(E); break;
        #endif
      }
    #endif
  }

  void Stepper::digipot_init() {

    #if HAS_DIGIPOTSS

      static const uint8_t digipot_motor_current[] = DIGIPOT_MOTOR_CURRENT;

      SPI.begin();
      SET_OUTPUT(DIGIPOTSS_PIN);

      for (uint8_t i = 0; i < COUNT(digipot_motor_current); i++) {
        //digitalPotWrite(digipot_ch[i], digipot_motor_current[i]);
        digipot_current(i, digipot_motor_current[i]);
      }

    #elif HAS_MOTOR_CURRENT_PWM

      #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
        SET_OUTPUT(MOTOR_CURRENT_PWM_XY_PIN);
      #endif
      #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
        SET_OUTPUT(MOTOR_CURRENT_PWM_Z_PIN);
      #endif
      #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
        SET_OUTPUT(MOTOR_CURRENT_PWM_E_PIN);
      #endif

      refresh_motor_power();

      // Set Timer5 to 31khz so the PWM of the motor power is as constant as possible. (removes a buzzing noise)
      SET_CS5(PRESCALER_1);

    #endif
  }

#endif

#if HAS_MICROSTEPS

  /**
   * Software-controlled Microstepping
   */

  void Stepper::microstep_init() {
    SET_OUTPUT(X_MS1_PIN);
    SET_OUTPUT(X_MS2_PIN);
    #if HAS_Y_MICROSTEPS
      SET_OUTPUT(Y_MS1_PIN);
      SET_OUTPUT(Y_MS2_PIN);
    #endif
    #if HAS_Z_MICROSTEPS
      SET_OUTPUT(Z_MS1_PIN);
      SET_OUTPUT(Z_MS2_PIN);
    #endif
    #if HAS_E0_MICROSTEPS
      SET_OUTPUT(E0_MS1_PIN);
      SET_OUTPUT(E0_MS2_PIN);
    #endif
    #if HAS_E1_MICROSTEPS
      SET_OUTPUT(E1_MS1_PIN);
      SET_OUTPUT(E1_MS2_PIN);
    #endif
    #if HAS_E2_MICROSTEPS
      SET_OUTPUT(E2_MS1_PIN);
      SET_OUTPUT(E2_MS2_PIN);
    #endif
    #if HAS_E3_MICROSTEPS
      SET_OUTPUT(E3_MS1_PIN);
      SET_OUTPUT(E3_MS2_PIN);
    #endif
    #if HAS_E4_MICROSTEPS
      SET_OUTPUT(E4_MS1_PIN);
      SET_OUTPUT(E4_MS2_PIN);
    #endif
    static const uint8_t microstep_modes[] = MICROSTEP_MODES;
    for (uint16_t i = 0; i < COUNT(microstep_modes); i++)
      microstep_mode(i, microstep_modes[i]);
  }

  void Stepper::microstep_ms(const uint8_t driver, const int8_t ms1, const int8_t ms2) {
    if (ms1 >= 0) switch (driver) {
      case 0: WRITE(X_MS1_PIN, ms1); break;
      #if HAS_Y_MICROSTEPS
        case 1: WRITE(Y_MS1_PIN, ms1); break;
      #endif
      #if HAS_Z_MICROSTEPS
        case 2: WRITE(Z_MS1_PIN, ms1); break;
      #endif
      #if HAS_E0_MICROSTEPS
        case 3: WRITE(E0_MS1_PIN, ms1); break;
      #endif
      #if HAS_E1_MICROSTEPS
        case 4: WRITE(E1_MS1_PIN, ms1); break;
      #endif
      #if HAS_E2_MICROSTEPS
        case 5: WRITE(E2_MS1_PIN, ms1); break;
      #endif
      #if HAS_E3_MICROSTEPS
        case 6: WRITE(E3_MS1_PIN, ms1); break;
      #endif
      #if HAS_E4_MICROSTEPS
        case 7: WRITE(E4_MS1_PIN, ms1); break;
      #endif
    }
    if (ms2 >= 0) switch (driver) {
      case 0: WRITE(X_MS2_PIN, ms2); break;
      #if HAS_Y_MICROSTEPS
        case 1: WRITE(Y_MS2_PIN, ms2); break;
      #endif
      #if HAS_Z_MICROSTEPS
        case 2: WRITE(Z_MS2_PIN, ms2); break;
      #endif
      #if HAS_E0_MICROSTEPS
        case 3: WRITE(E0_MS2_PIN, ms2); break;
      #endif
      #if HAS_E1_MICROSTEPS
        case 4: WRITE(E1_MS2_PIN, ms2); break;
      #endif
      #if HAS_E2_MICROSTEPS
        case 5: WRITE(E2_MS2_PIN, ms2); break;
      #endif
      #if HAS_E3_MICROSTEPS
        case 6: WRITE(E3_MS2_PIN, ms2); break;
      #endif
      #if HAS_E4_MICROSTEPS
        case 7: WRITE(E4_MS2_PIN, ms2); break;
      #endif
    }
  }

  void Stepper::microstep_mode(const uint8_t driver, const uint8_t stepping_mode) {
    switch (stepping_mode) {
      case 1: microstep_ms(driver, MICROSTEP1); break;
      #if ENABLED(HEROIC_STEPPER_DRIVERS)
        case 128: microstep_ms(driver, MICROSTEP128); break;
      #else
        case 2: microstep_ms(driver, MICROSTEP2); break;
        case 4: microstep_ms(driver, MICROSTEP4); break;
      #endif
      case 8: microstep_ms(driver, MICROSTEP8); break;
      case 16: microstep_ms(driver, MICROSTEP16); break;
      #if MB(ALLIGATOR)
        case 32: microstep_ms(driver, MICROSTEP32); break;
      #endif
      default: SERIAL_ERROR_START(); SERIAL_ERRORLNPGM("Microsteps unavailable"); break;
    }
  }

  void Stepper::microstep_readings() {
    SERIAL_PROTOCOLLNPGM("MS1,MS2 Pins");
    SERIAL_PROTOCOLPGM("X: ");
    SERIAL_PROTOCOL(READ(X_MS1_PIN));
    SERIAL_PROTOCOLLN(READ(X_MS2_PIN));
    #if HAS_Y_MICROSTEPS
      SERIAL_PROTOCOLPGM("Y: ");
      SERIAL_PROTOCOL(READ(Y_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(Y_MS2_PIN));
    #endif
    #if HAS_Z_MICROSTEPS
      SERIAL_PROTOCOLPGM("Z: ");
      SERIAL_PROTOCOL(READ(Z_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(Z_MS2_PIN));
    #endif
    #if HAS_E0_MICROSTEPS
      SERIAL_PROTOCOLPGM("E0: ");
      SERIAL_PROTOCOL(READ(E0_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E0_MS2_PIN));
    #endif
    #if HAS_E1_MICROSTEPS
      SERIAL_PROTOCOLPGM("E1: ");
      SERIAL_PROTOCOL(READ(E1_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E1_MS2_PIN));
    #endif
    #if HAS_E2_MICROSTEPS
      SERIAL_PROTOCOLPGM("E2: ");
      SERIAL_PROTOCOL(READ(E2_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E2_MS2_PIN));
    #endif
    #if HAS_E3_MICROSTEPS
      SERIAL_PROTOCOLPGM("E3: ");
      SERIAL_PROTOCOL(READ(E3_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E3_MS2_PIN));
    #endif
    #if HAS_E4_MICROSTEPS
      SERIAL_PROTOCOLPGM("E4: ");
      SERIAL_PROTOCOL(READ(E4_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E4_MS2_PIN));
    #endif
  }

#endif // HAS_MICROSTEPS
