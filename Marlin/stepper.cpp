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

#include "Marlin.h"
#include "stepper.h"
#include "endstops.h"
#include "planner.h"
#include "temperature.h"
#include "ultralcd.h"
#include "language.h"
#include "cardreader.h"
#include "speed_lookuptable.h"

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

Stepper stepper; // Singleton

// public:

block_t* Stepper::current_block = NULL;  // A pointer to the block currently being traced

#if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
  bool Stepper::abort_on_endstop_hit = false;
#endif

#if ENABLED(Z_DUAL_ENDSTOPS)
  bool Stepper::performing_homing = false;
#endif

// private:

unsigned char Stepper::last_direction_bits = 0;        // The next stepping-bits to be output
unsigned int Stepper::cleaning_buffer_counter = 0;

#if ENABLED(Z_DUAL_ENDSTOPS)
  bool Stepper::locked_z_motor = false;
  bool Stepper::locked_z2_motor = false;
#endif

long Stepper::counter_X = 0,
     Stepper::counter_Y = 0,
     Stepper::counter_Z = 0,
     Stepper::counter_E = 0;

volatile uint32_t Stepper::step_events_completed = 0; // The number of step events executed in the current block

#if ENABLED(ADVANCE) || ENABLED(LIN_ADVANCE)

  unsigned char Stepper::old_OCR0A;
  volatile unsigned char Stepper::eISR_Rate = 200; // Keep the ISR at a low rate until needed

  #if ENABLED(LIN_ADVANCE)
    volatile long Stepper::e_steps[E_STEPPERS];
    int Stepper::extruder_advance_k = LIN_ADVANCE_K,
        Stepper::final_estep_rate,
        Stepper::current_estep_rate[E_STEPPERS],
        Stepper::current_adv_steps[E_STEPPERS];
  #else
    long Stepper::e_steps[E_STEPPERS],
         Stepper::final_advance = 0,
         Stepper::old_advance = 0,
         Stepper::advance_rate,
         Stepper::advance;
  #endif
#endif

long Stepper::acceleration_time, Stepper::deceleration_time;

volatile long Stepper::count_position[NUM_AXIS] = { 0 };
volatile signed char Stepper::count_direction[NUM_AXIS] = { 1, 1, 1, 1 };

#if ENABLED(MIXING_EXTRUDER)
  long Stepper::counter_m[MIXING_STEPPERS];
#endif

unsigned short Stepper::acc_step_rate; // needed for deceleration start point
uint8_t Stepper::step_loops, Stepper::step_loops_nominal;
unsigned short Stepper::OCR1A_nominal;

volatile long Stepper::endstops_trigsteps[XYZ];

#if ENABLED(X_DUAL_STEPPER_DRIVERS)
  #define X_APPLY_DIR(v,Q) do{ X_DIR_WRITE(v); X2_DIR_WRITE((v) != INVERT_X2_VS_X_DIR); }while(0)
  #define X_APPLY_STEP(v,Q) do{ X_STEP_WRITE(v); X2_STEP_WRITE(v); }while(0)
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
      if (current_block->active_extruder != 0) X2_STEP_WRITE(v); else X_STEP_WRITE(v); \
    }
#else
  #define X_APPLY_DIR(v,Q) X_DIR_WRITE(v)
  #define X_APPLY_STEP(v,Q) X_STEP_WRITE(v)
#endif

#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #define Y_APPLY_DIR(v,Q) do{ Y_DIR_WRITE(v); Y2_DIR_WRITE((v) != INVERT_Y2_VS_Y_DIR); }while(0)
  #define Y_APPLY_STEP(v,Q) do{ Y_STEP_WRITE(v); Y2_STEP_WRITE(v); }while(0)
#else
  #define Y_APPLY_DIR(v,Q) Y_DIR_WRITE(v)
  #define Y_APPLY_STEP(v,Q) Y_STEP_WRITE(v)
#endif

#if ENABLED(Z_DUAL_STEPPER_DRIVERS)
  #define Z_APPLY_DIR(v,Q) do{ Z_DIR_WRITE(v); Z2_DIR_WRITE(v); }while(0)
  #if ENABLED(Z_DUAL_ENDSTOPS)
    #define Z_APPLY_STEP(v,Q) \
    if (performing_homing) { \
      if (Z_HOME_DIR > 0) {\
        if (!(TEST(endstops.old_endstop_bits, Z_MAX) && (count_direction[Z_AXIS] > 0)) && !locked_z_motor) Z_STEP_WRITE(v); \
        if (!(TEST(endstops.old_endstop_bits, Z2_MAX) && (count_direction[Z_AXIS] > 0)) && !locked_z2_motor) Z2_STEP_WRITE(v); \
      } \
      else { \
        if (!(TEST(endstops.old_endstop_bits, Z_MIN) && (count_direction[Z_AXIS] < 0)) && !locked_z_motor) Z_STEP_WRITE(v); \
        if (!(TEST(endstops.old_endstop_bits, Z2_MIN) && (count_direction[Z_AXIS] < 0)) && !locked_z2_motor) Z2_STEP_WRITE(v); \
      } \
    } \
    else { \
      Z_STEP_WRITE(v); \
      Z2_STEP_WRITE(v); \
    }
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

// intRes = longIn1 * longIn2 >> 24
// uses:
// r26 to store 0
// r27 to store bits 16-23 of the 48bit result. The top bit is used to round the two byte result.
// note that the lower two bytes and the upper byte of the 48bit result are not calculated.
// this can cause the result to be out by one as the lower bytes may cause carries into the upper ones.
// B0 A0 are bits 24-39 and are the returned value
// C1 B1 A1 is longIn1
// D2 C2 B2 A2 is longIn2
//
#define MultiU24X32toH16(intRes, longIn1, longIn2) \
  asm volatile ( \
                 "clr r26 \n\t" \
                 "mul %A1, %B2 \n\t" \
                 "mov r27, r1 \n\t" \
                 "mul %B1, %C2 \n\t" \
                 "movw %A0, r0 \n\t" \
                 "mul %C1, %C2 \n\t" \
                 "add %B0, r0 \n\t" \
                 "mul %C1, %B2 \n\t" \
                 "add %A0, r0 \n\t" \
                 "adc %B0, r1 \n\t" \
                 "mul %A1, %C2 \n\t" \
                 "add r27, r0 \n\t" \
                 "adc %A0, r1 \n\t" \
                 "adc %B0, r26 \n\t" \
                 "mul %B1, %B2 \n\t" \
                 "add r27, r0 \n\t" \
                 "adc %A0, r1 \n\t" \
                 "adc %B0, r26 \n\t" \
                 "mul %C1, %A2 \n\t" \
                 "add r27, r0 \n\t" \
                 "adc %A0, r1 \n\t" \
                 "adc %B0, r26 \n\t" \
                 "mul %B1, %A2 \n\t" \
                 "add r27, r1 \n\t" \
                 "adc %A0, r26 \n\t" \
                 "adc %B0, r26 \n\t" \
                 "lsr r27 \n\t" \
                 "adc %A0, r26 \n\t" \
                 "adc %B0, r26 \n\t" \
                 "mul %D2, %A1 \n\t" \
                 "add %A0, r0 \n\t" \
                 "adc %B0, r1 \n\t" \
                 "mul %D2, %B1 \n\t" \
                 "add %B0, r0 \n\t" \
                 "clr r1 \n\t" \
                 : \
                 "=&r" (intRes) \
                 : \
                 "d" (longIn1), \
                 "d" (longIn2) \
                 : \
                 "r26" , "r27" \
               )

// Some useful constants

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  SBI(TIMSK1, OCIE1A)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() CBI(TIMSK1, OCIE1A)

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
  //  TCNT1 = 0;
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

  if (motor_direction(E_AXIS)) {
    REV_E_DIR();
    count_direction[E_AXIS] = -1;
  }
  else {
    NORM_E_DIR();
    count_direction[E_AXIS] = 1;
  }
}

// "The Stepper Driver Interrupt" - This timer interrupt is the workhorse.
// It pops blocks from the block_buffer and executes them by pulsing the stepper pins appropriately.
ISR(TIMER1_COMPA_vect) { Stepper::isr(); }

void Stepper::isr() {
  if (cleaning_buffer_counter) {
    current_block = NULL;
    planner.discard_current_block();
    #ifdef SD_FINISHED_RELEASECOMMAND
      if ((cleaning_buffer_counter == 1) && (SD_FINISHED_STEPPERRELEASE)) enqueue_and_echo_commands_P(PSTR(SD_FINISHED_RELEASECOMMAND));
    #endif
    cleaning_buffer_counter--;
    OCR1A = 200;
    return;
  }

  // If there is no current block, attempt to pop one from the buffer
  if (!current_block) {
    // Anything in the buffer?
    current_block = planner.get_current_block();
    if (current_block) {
      current_block->busy = true;
      trapezoid_generator_reset();

      // Initialize Bresenham counters to 1/2 the ceiling
      counter_X = counter_Y = counter_Z = counter_E = -(current_block->step_event_count >> 1);

      #if ENABLED(MIXING_EXTRUDER)
        MIXING_STEPPERS_LOOP(i)
          counter_m[i] = -(current_block->mix_event_count[i] >> 1);
      #endif

      step_events_completed = 0;

      #if ENABLED(Z_LATE_ENABLE)
        if (current_block->steps[Z_AXIS] > 0) {
          enable_z();
          OCR1A = 2000; //1ms wait
          return;
        }
      #endif

      // #if ENABLED(ADVANCE)
      //   e_steps[TOOL_E_INDEX] = 0;
      // #endif
    }
    else {
      OCR1A = 2000; // 1kHz.
      return;
    }
  }

  // Update endstops state, if enabled
  if (endstops.enabled
    #if HAS_BED_PROBE
      || endstops.z_probe_enabled
    #endif
  ) endstops.update();

  // Take multiple steps per interrupt (For high speed moves)
  bool all_steps_done = false;
  for (int8_t i = 0; i < step_loops; i++) {
    #ifndef USBCON
      customizedSerial.checkRx(); // Check for serial chars.
    #endif

    #if ENABLED(LIN_ADVANCE)

      counter_E += current_block->steps[E_AXIS];
      if (counter_E > 0) {
        counter_E -= current_block->step_event_count;
        #if DISABLED(MIXING_EXTRUDER)
          // Don't step E here for mixing extruder
          count_position[E_AXIS] += count_direction[E_AXIS];
          motor_direction(E_AXIS) ? --e_steps[TOOL_E_INDEX] : ++e_steps[TOOL_E_INDEX];
        #endif
      }

      #if ENABLED(MIXING_EXTRUDER)
        // Step mixing steppers proportionally
        bool dir = motor_direction(E_AXIS);
        MIXING_STEPPERS_LOOP(j) {
          counter_m[j] += current_block->steps[E_AXIS];
          if (counter_m[j] > 0) {
            counter_m[j] -= current_block->mix_event_count[j];
            dir ? --e_steps[j] : ++e_steps[j];
          }
        }
      #endif

      if (current_block->use_advance_lead) {
        int delta_adv_steps = (((long)extruder_advance_k * current_estep_rate[TOOL_E_INDEX]) >> 9) - current_adv_steps[TOOL_E_INDEX];
        #if ENABLED(MIXING_EXTRUDER)
          // Mixing extruders apply advance lead proportionally
          MIXING_STEPPERS_LOOP(j) {
            int steps = delta_adv_steps * current_block->step_event_count / current_block->mix_event_count[j];
            e_steps[j] += steps;
            current_adv_steps[j] += steps;
          }
        #else
          // For most extruders, advance the single E stepper
          e_steps[TOOL_E_INDEX] += delta_adv_steps;
          current_adv_steps[TOOL_E_INDEX] += delta_adv_steps;
        #endif
      }

    #elif ENABLED(ADVANCE)

      // Always count the unified E axis
      counter_E += current_block->steps[E_AXIS];
      if (counter_E > 0) {
        counter_E -= current_block->step_event_count;
        #if DISABLED(MIXING_EXTRUDER)
          // Don't step E here for mixing extruder
          motor_direction(E_AXIS) ? --e_steps[TOOL_E_INDEX] : ++e_steps[TOOL_E_INDEX];
        #endif
      }

      #if ENABLED(MIXING_EXTRUDER)

        // Step mixing steppers proportionally
        bool dir = motor_direction(E_AXIS);
        MIXING_STEPPERS_LOOP(j) {
          counter_m[j] += current_block->steps[E_AXIS];
          if (counter_m[j] > 0) {
            counter_m[j] -= current_block->mix_event_count[j];
            dir ? --e_steps[j] : ++e_steps[j];
          }
        }

      #endif // MIXING_EXTRUDER

    #endif // ADVANCE or LIN_ADVANCE

    #define _COUNTER(AXIS) counter_## AXIS
    #define _APPLY_STEP(AXIS) AXIS ##_APPLY_STEP
    #define _INVERT_STEP_PIN(AXIS) INVERT_## AXIS ##_STEP_PIN

    // Advance the Bresenham counter; start a pulse if the axis needs a step
    #define PULSE_START(AXIS) \
      _COUNTER(AXIS) += current_block->steps[_AXIS(AXIS)]; \
      if (_COUNTER(AXIS) > 0) { _APPLY_STEP(AXIS)(!_INVERT_STEP_PIN(AXIS),0); }

    // Stop an active pulse, reset the Bresenham counter, update the position
    #define PULSE_STOP(AXIS) \
      if (_COUNTER(AXIS) > 0) { \
        _COUNTER(AXIS) -= current_block->step_event_count; \
        count_position[_AXIS(AXIS)] += count_direction[_AXIS(AXIS)]; \
        _APPLY_STEP(AXIS)(_INVERT_STEP_PIN(AXIS),0); \
      }

    #define CYCLES_EATEN_BY_CODE 240

    // If a minimum pulse time was specified get the CPU clock
    #if STEP_PULSE_CYCLES > CYCLES_EATEN_BY_CODE
      static uint32_t pulse_start;
      pulse_start = TCNT0;
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

    // For non-advance use linear interpolation for E also
    #if DISABLED(ADVANCE) && DISABLED(LIN_ADVANCE)
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
    #endif // !ADVANCE && !LIN_ADVANCE

    // For a minimum pulse time wait before stopping pulses
    #if STEP_PULSE_CYCLES > CYCLES_EATEN_BY_CODE
      while ((uint32_t)(TCNT0 - pulse_start) < STEP_PULSE_CYCLES - CYCLES_EATEN_BY_CODE) { /* nada */ }
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

    #if DISABLED(ADVANCE) && DISABLED(LIN_ADVANCE)
      #if ENABLED(MIXING_EXTRUDER)
        // Always step the single E axis
        if (counter_E > 0) {
          counter_E -= current_block->step_event_count;
          count_position[E_AXIS] += count_direction[E_AXIS];
        }
        MIXING_STEPPERS_LOOP(j) {
          if (counter_m[j] > 0) {
            counter_m[j] -= current_block->mix_event_count[j];
            En_STEP_WRITE(j, INVERT_E_STEP_PIN);
          }
        }
      #else // !MIXING_EXTRUDER
        PULSE_STOP(E);
      #endif
    #endif // !ADVANCE && !LIN_ADVANCE

    if (++step_events_completed >= current_block->step_event_count) {
      all_steps_done = true;
      break;
    }
  }

  #if ENABLED(ADVANCE) || ENABLED(LIN_ADVANCE)
    // If we have esteps to execute, fire the next advance_isr "now"
    if (e_steps[TOOL_E_INDEX]) OCR0A = TCNT0 + 2;
  #endif

  // Calculate new timer value
  uint16_t timer, step_rate;
  if (step_events_completed <= (uint32_t)current_block->accelerate_until) {

    MultiU24X32toH16(acc_step_rate, acceleration_time, current_block->acceleration_rate);
    acc_step_rate += current_block->initial_rate;

    // upper limit
    NOMORE(acc_step_rate, current_block->nominal_rate);

    // step_rate to timer interval
    timer = calc_timer(acc_step_rate);
    OCR1A = timer;
    acceleration_time += timer;

    #if ENABLED(LIN_ADVANCE)

      if (current_block->use_advance_lead)
        current_estep_rate[TOOL_E_INDEX] = ((uint32_t)acc_step_rate * current_block->e_speed_multiplier8) >> 8;

      if (current_block->use_advance_lead) {
        #if ENABLED(MIXING_EXTRUDER)
          MIXING_STEPPERS_LOOP(j)
            current_estep_rate[j] = ((uint32_t)acc_step_rate * current_block->e_speed_multiplier8 * current_block->step_event_count / current_block->mix_event_count[j]) >> 8;
        #else
          current_estep_rate[TOOL_E_INDEX] = ((uint32_t)acc_step_rate * current_block->e_speed_multiplier8) >> 8;
        #endif
      }

    #elif ENABLED(ADVANCE)

      advance += advance_rate * step_loops;
      //NOLESS(advance, current_block->advance);

      long advance_whole = advance >> 8,
           advance_factor = advance_whole - old_advance;

      // Do E steps + advance steps
      #if ENABLED(MIXING_EXTRUDER)
        // ...for mixing steppers proportionally
        MIXING_STEPPERS_LOOP(j)
          e_steps[j] += advance_factor * current_block->step_event_count / current_block->mix_event_count[j];
      #else
        // ...for the active extruder
        e_steps[TOOL_E_INDEX] += advance_factor;
      #endif

      old_advance = advance_whole;

    #endif // ADVANCE or LIN_ADVANCE

    #if ENABLED(ADVANCE) || ENABLED(LIN_ADVANCE)
      eISR_Rate = (timer >> 2) * step_loops / abs(e_steps[TOOL_E_INDEX]);
    #endif
  }
  else if (step_events_completed > (uint32_t)current_block->decelerate_after) {
    MultiU24X32toH16(step_rate, deceleration_time, current_block->acceleration_rate);

    if (step_rate < acc_step_rate) { // Still decelerating?
      step_rate = acc_step_rate - step_rate;
      NOLESS(step_rate, current_block->final_rate);
    }
    else
      step_rate = current_block->final_rate;

    // step_rate to timer interval
    timer = calc_timer(step_rate);
    OCR1A = timer;
    deceleration_time += timer;

    #if ENABLED(LIN_ADVANCE)

      if (current_block->use_advance_lead) {
        #if ENABLED(MIXING_EXTRUDER)
          MIXING_STEPPERS_LOOP(j)
            current_estep_rate[j] = ((uint32_t)step_rate * current_block->e_speed_multiplier8 * current_block->step_event_count / current_block->mix_event_count[j]) >> 8;
        #else
          current_estep_rate[TOOL_E_INDEX] = ((uint32_t)step_rate * current_block->e_speed_multiplier8) >> 8;
        #endif
      }

    #elif ENABLED(ADVANCE)

      advance -= advance_rate * step_loops;
      NOLESS(advance, final_advance);

      // Do E steps + advance steps
      long advance_whole = advance >> 8,
           advance_factor = advance_whole - old_advance;

      #if ENABLED(MIXING_EXTRUDER)
        MIXING_STEPPERS_LOOP(j)
          e_steps[j] += advance_factor * current_block->step_event_count / current_block->mix_event_count[j];
      #else
        e_steps[TOOL_E_INDEX] += advance_factor;
      #endif

      old_advance = advance_whole;

    #endif // ADVANCE or LIN_ADVANCE

    #if ENABLED(ADVANCE) || ENABLED(LIN_ADVANCE)
      eISR_Rate = (timer >> 2) * step_loops / abs(e_steps[TOOL_E_INDEX]);
    #endif
  }
  else {

    #if ENABLED(LIN_ADVANCE)

      if (current_block->use_advance_lead)
        current_estep_rate[TOOL_E_INDEX] = final_estep_rate;

      eISR_Rate = (OCR1A_nominal >> 2) * step_loops_nominal / abs(e_steps[TOOL_E_INDEX]);

    #endif

    OCR1A = OCR1A_nominal;
    // ensure we're running at the correct step rate, even if we just came off an acceleration
    step_loops = step_loops_nominal;
  }

  NOLESS(OCR1A, TCNT1 + 16);

  // If current block is finished, reset pointer
  if (all_steps_done) {
    current_block = NULL;
    planner.discard_current_block();
  }
}

#if ENABLED(ADVANCE) || ENABLED(LIN_ADVANCE)

  // Timer interrupt for E. e_steps is set in the main routine;
  // Timer 0 is shared with millies
  ISR(TIMER0_COMPA_vect) { Stepper::advance_isr(); }

  void Stepper::advance_isr() {

    old_OCR0A += eISR_Rate;
    OCR0A = old_OCR0A;

    #define START_E_PULSE(INDEX) \
      if (e_steps[INDEX]) E## INDEX ##_STEP_WRITE(!INVERT_E_STEP_PIN)

    #define STOP_E_PULSE(INDEX) \
      if (e_steps[INDEX]) { \
        e_steps[INDEX] <= 0 ? ++e_steps[INDEX] : --e_steps[INDEX]; \
        E## INDEX ##_STEP_WRITE(INVERT_E_STEP_PIN); \
      }

    #define CYCLES_EATEN_BY_E 60

    // Step all E steppers that have steps
    for (uint8_t i = 0; i < step_loops; i++) {

      #if STEP_PULSE_CYCLES > CYCLES_EATEN_BY_E
        static uint32_t pulse_start;
        pulse_start = TCNT0;
      #endif

      START_E_PULSE(0);
      #if E_STEPPERS > 1
        START_E_PULSE(1);
        #if E_STEPPERS > 2
          START_E_PULSE(2);
          #if E_STEPPERS > 3
            START_E_PULSE(3);
          #endif
        #endif
      #endif

      // For a minimum pulse time wait before stopping pulses
      #if STEP_PULSE_CYCLES > CYCLES_EATEN_BY_E
        while ((uint32_t)(TCNT0 - pulse_start) < STEP_PULSE_CYCLES - CYCLES_EATEN_BY_E) { /* nada */ }
      #endif

      STOP_E_PULSE(0);
      #if E_STEPPERS > 1
        STOP_E_PULSE(1);
        #if E_STEPPERS > 2
          STOP_E_PULSE(2);
          #if E_STEPPERS > 3
            STOP_E_PULSE(3);
          #endif
        #endif
      #endif
    }

  }

#endif // ADVANCE or LIN_ADVANCE

void Stepper::init() {

  // Init Digipot Motor Current
  #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM
    digipot_init();
  #endif

  // Init Microstepping Pins
  #if HAS_MICROSTEPS
    microstep_init();
  #endif

  // Init TMC Steppers
  #if ENABLED(HAVE_TMCDRIVER)
    tmc_init();
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

  // Init Enable Pins - steppers default to disabled.
  #if HAS_X_ENABLE
    X_ENABLE_INIT;
    if (!X_ENABLE_ON) X_ENABLE_WRITE(HIGH);
    #if ENABLED(DUAL_X_CARRIAGE) && HAS_X2_ENABLE
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

  // Init endstops and pullups
  endstops.init();

  #define _STEP_INIT(AXIS) AXIS ##_STEP_INIT
  #define _WRITE_STEP(AXIS, HIGHLOW) AXIS ##_STEP_WRITE(HIGHLOW)
  #define _DISABLE(axis) disable_## axis()

  #define AXIS_INIT(axis, AXIS, PIN) \
    _STEP_INIT(AXIS); \
    _WRITE_STEP(AXIS, _INVERT_STEP_PIN(PIN)); \
    _DISABLE(axis)

  #define E_AXIS_INIT(NUM) AXIS_INIT(e## NUM, E## NUM, E)

  // Init Step Pins
  #if HAS_X_STEP
    #if ENABLED(X_DUAL_STEPPER_DRIVERS) || ENABLED(DUAL_X_CARRIAGE)
      X2_STEP_INIT;
      X2_STEP_WRITE(INVERT_X_STEP_PIN);
    #endif
    AXIS_INIT(x, X, X);
  #endif

  #if HAS_Y_STEP
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
      Y2_STEP_INIT;
      Y2_STEP_WRITE(INVERT_Y_STEP_PIN);
    #endif
    AXIS_INIT(y, Y, Y);
  #endif

  #if HAS_Z_STEP
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS)
      Z2_STEP_INIT;
      Z2_STEP_WRITE(INVERT_Z_STEP_PIN);
    #endif
    AXIS_INIT(z, Z, Z);
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

  // waveform generation = 0100 = CTC
  CBI(TCCR1B, WGM13);
  SBI(TCCR1B, WGM12);
  CBI(TCCR1A, WGM11);
  CBI(TCCR1A, WGM10);

  // output mode = 00 (disconnected)
  TCCR1A &= ~(3 << COM1A0);
  TCCR1A &= ~(3 << COM1B0);
  // Set the timer pre-scaler
  // Generally we use a divider of 8, resulting in a 2MHz timer
  // frequency on a 16MHz MCU. If you are going to change this, be
  // sure to regenerate speed_lookuptable.h with
  // create_speed_lookuptable.py
  TCCR1B = (TCCR1B & ~(0x07 << CS10)) | (2 << CS10);

  OCR1A = 0x4000;
  TCNT1 = 0;
  ENABLE_STEPPER_DRIVER_INTERRUPT();

  #if ENABLED(ADVANCE) || ENABLED(LIN_ADVANCE)

    for (int i = 0; i < E_STEPPERS; i++) {
      e_steps[i] = 0;
      #if ENABLED(LIN_ADVANCE)
        current_adv_steps[i] = 0;
      #endif
    }

    #if defined(TCCR0A) && defined(WGM01)
      CBI(TCCR0A, WGM01);
      CBI(TCCR0A, WGM00);
    #endif
    SBI(TIMSK0, OCIE0A);

  #endif // ADVANCE or LIN_ADVANCE

  endstops.enable(true); // Start with endstops active. After homing they can be disabled
  sei();

  set_directions(); // Init directions to last_direction_bits = 0
}


/**
 * Block until all buffered steps are executed
 */
void Stepper::synchronize() { while (planner.blocks_queued()) idle(); }

/**
 * Set the stepper positions directly in steps
 *
 * The input is based on the typical per-axis XYZ steps.
 * For CORE machines XYZ needs to be translated to ABC.
 *
 * This allows get_axis_position_mm to correctly
 * derive the current XYZ position later on.
 */
void Stepper::set_position(const long& x, const long& y, const long& z, const long& e) {

  synchronize(); // Bad to set stepper counts in the middle of a move

  CRITICAL_SECTION_START;

  #if ENABLED(COREXY)
    // corexy positioning
    // these equations follow the form of the dA and dB equations on http://www.corexy.com/theory.html
    count_position[A_AXIS] = x + y;
    count_position[B_AXIS] = x - y;
    count_position[Z_AXIS] = z;
  #elif ENABLED(COREXZ)
    // corexz planning
    count_position[A_AXIS] = x + z;
    count_position[Y_AXIS] = y;
    count_position[C_AXIS] = x - z;
  #elif ENABLED(COREYZ)
    // coreyz planning
    count_position[X_AXIS] = x;
    count_position[B_AXIS] = y + z;
    count_position[C_AXIS] = y - z;
  #else
    // default non-h-bot planning
    count_position[X_AXIS] = x;
    count_position[Y_AXIS] = y;
    count_position[Z_AXIS] = z;
  #endif

  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

void Stepper::set_position(const AxisEnum &axis, const long& v) {
  CRITICAL_SECTION_START;
  count_position[axis] = v;
  CRITICAL_SECTION_END;
}

void Stepper::set_e_position(const long& e) {
  CRITICAL_SECTION_START;
  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

/**
 * Get a stepper's position in steps.
 */
long Stepper::position(AxisEnum axis) {
  CRITICAL_SECTION_START;
  long count_pos = count_position[axis];
  CRITICAL_SECTION_END;
  return count_pos;
}

/**
 * Get an axis position according to stepper position(s)
 * For CORE machines apply translation from ABC to XYZ.
 */
float Stepper::get_axis_position_mm(AxisEnum axis) {
  float axis_steps;
  #if ENABLED(COREXY) || ENABLED(COREXZ) || ENABLED(COREYZ)
    // Requesting one of the "core" axes?
    if (axis == CORE_AXIS_1 || axis == CORE_AXIS_2) {
      CRITICAL_SECTION_START;
      long pos1 = count_position[CORE_AXIS_1],
           pos2 = count_position[CORE_AXIS_2];
      CRITICAL_SECTION_END;
      // ((a1+a2)+(a1-a2))/2 -> (a1+a2+a1-a2)/2 -> (a1+a1)/2 -> a1
      // ((a1+a2)-(a1-a2))/2 -> (a1+a2-a1+a2)/2 -> (a2+a2)/2 -> a2
      axis_steps = (pos1 + ((axis == CORE_AXIS_1) ? pos2 : -pos2)) * 0.5f;
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
}

void Stepper::endstop_triggered(AxisEnum axis) {

  #if ENABLED(COREXY) || ENABLED(COREXZ) || ENABLED(COREYZ)

    float axis_pos = count_position[axis];
    if (axis == CORE_AXIS_1)
      axis_pos = (axis_pos + count_position[CORE_AXIS_2]) * 0.5;
    else if (axis == CORE_AXIS_2)
      axis_pos = (count_position[CORE_AXIS_1] - axis_pos) * 0.5;
    endstops_trigsteps[axis] = axis_pos;

  #else // !COREXY && !COREXZ && !COREYZ

    endstops_trigsteps[axis] = count_position[axis];

  #endif // !COREXY && !COREXZ && !COREYZ

  kill_current_block();
}

void Stepper::report_positions() {
  CRITICAL_SECTION_START;
  long xpos = count_position[X_AXIS],
       ypos = count_position[Y_AXIS],
       zpos = count_position[Z_AXIS];
  CRITICAL_SECTION_END;

  #if ENABLED(COREXY) || ENABLED(COREXZ) || IS_SCARA
    SERIAL_PROTOCOLPGM(MSG_COUNT_A);
  #else
    SERIAL_PROTOCOLPGM(MSG_COUNT_X);
  #endif
  SERIAL_PROTOCOL(xpos);

  #if ENABLED(COREXY) || ENABLED(COREYZ) || IS_SCARA
    SERIAL_PROTOCOLPGM(" B:");
  #else
    SERIAL_PROTOCOLPGM(" Y:");
  #endif
  SERIAL_PROTOCOL(ypos);

  #if ENABLED(COREXZ) || ENABLED(COREYZ)
    SERIAL_PROTOCOLPGM(" C:");
  #else
    SERIAL_PROTOCOLPGM(" Z:");
  #endif
  SERIAL_PROTOCOL(zpos);

  SERIAL_EOL;
}

#if ENABLED(BABYSTEPPING)

  // MUST ONLY BE CALLED BY AN ISR,
  // No other ISR should ever interrupt this!
  void Stepper::babystep(const uint8_t axis, const bool direction) {

    #define _ENABLE(axis) enable_## axis()
    #define _READ_DIR(AXIS) AXIS ##_DIR_READ
    #define _INVERT_DIR(AXIS) INVERT_## AXIS ##_DIR
    #define _APPLY_DIR(AXIS, INVERT) AXIS ##_APPLY_DIR(INVERT, true)

    #define BABYSTEP_AXIS(axis, AXIS, INVERT) { \
        _ENABLE(axis); \
        uint8_t old_pin = _READ_DIR(AXIS); \
        _APPLY_DIR(AXIS, _INVERT_DIR(AXIS)^direction^INVERT); \
        _APPLY_STEP(AXIS)(!_INVERT_STEP_PIN(AXIS), true); \
        delayMicroseconds(2); \
        _APPLY_STEP(AXIS)(_INVERT_STEP_PIN(AXIS), true); \
        _APPLY_DIR(AXIS, old_pin); \
      }

    switch (axis) {

      case X_AXIS:
        BABYSTEP_AXIS(x, X, false);
        break;

      case Y_AXIS:
        BABYSTEP_AXIS(y, Y, false);
        break;

      case Z_AXIS: {

        #if DISABLED(DELTA)

          BABYSTEP_AXIS(z, Z, BABYSTEP_INVERT_Z);

        #else // DELTA

          bool z_direction = direction ^ BABYSTEP_INVERT_Z;

          enable_x();
          enable_y();
          enable_z();
          uint8_t old_x_dir_pin = X_DIR_READ,
                  old_y_dir_pin = Y_DIR_READ,
                  old_z_dir_pin = Z_DIR_READ;
          //setup new step
          X_DIR_WRITE(INVERT_X_DIR ^ z_direction);
          Y_DIR_WRITE(INVERT_Y_DIR ^ z_direction);
          Z_DIR_WRITE(INVERT_Z_DIR ^ z_direction);
          //perform step
          X_STEP_WRITE(!INVERT_X_STEP_PIN);
          Y_STEP_WRITE(!INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(!INVERT_Z_STEP_PIN);
          delayMicroseconds(2);
          X_STEP_WRITE(INVERT_X_STEP_PIN);
          Y_STEP_WRITE(INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(INVERT_Z_STEP_PIN);
          //get old pin state back.
          X_DIR_WRITE(old_x_dir_pin);
          Y_DIR_WRITE(old_y_dir_pin);
          Z_DIR_WRITE(old_z_dir_pin);

        #endif

      } break;

      default: break;
    }
  }

#endif //BABYSTEPPING

/**
 * Software-controlled Stepper Motor Current
 */

#if HAS_DIGIPOTSS

  // From Arduino DigitalPotControl example
  void Stepper::digitalPotWrite(int address, int value) {
    WRITE(DIGIPOTSS_PIN, LOW); // take the SS pin low to select the chip
    SPI.transfer(address); //  send in the address and value via SPI:
    SPI.transfer(value);
    WRITE(DIGIPOTSS_PIN, HIGH); // take the SS pin high to de-select the chip:
    //delay(10);
  }

#endif //HAS_DIGIPOTSS

#if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM

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
        digipot_current(0, motor_current_setting[0]);
      #endif
      #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
        SET_OUTPUT(MOTOR_CURRENT_PWM_Z_PIN);
        digipot_current(1, motor_current_setting[1]);
      #endif
      #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
        SET_OUTPUT(MOTOR_CURRENT_PWM_E_PIN);
        digipot_current(2, motor_current_setting[2]);
      #endif
      //Set timer5 to 31khz so the PWM of the motor power is as constant as possible. (removes a buzzing noise)
      TCCR5B = (TCCR5B & ~(_BV(CS50) | _BV(CS51) | _BV(CS52))) | _BV(CS50);
    #endif
  }

  void Stepper::digipot_current(uint8_t driver, int current) {
    #if HAS_DIGIPOTSS
      const uint8_t digipot_ch[] = DIGIPOT_CHANNELS;
      digitalPotWrite(digipot_ch[driver], current);
    #elif HAS_MOTOR_CURRENT_PWM
      #define _WRITE_CURRENT_PWM(P) analogWrite(P, 255L * current / (MOTOR_CURRENT_PWM_RANGE))
      switch (driver) {
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
          case 0: _WRITE_CURRENT_PWM(MOTOR_CURRENT_PWM_XY_PIN); break;
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
          case 1: _WRITE_CURRENT_PWM(MOTOR_CURRENT_PWM_Z_PIN); break;
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
          case 2: _WRITE_CURRENT_PWM(MOTOR_CURRENT_PWM_E_PIN); break;
        #endif
      }
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
    #if HAS_MICROSTEPS_Y
      SET_OUTPUT(Y_MS1_PIN);
      SET_OUTPUT(Y_MS2_PIN);
    #endif
    #if HAS_MICROSTEPS_Z
      SET_OUTPUT(Z_MS1_PIN);
      SET_OUTPUT(Z_MS2_PIN);
    #endif
    #if HAS_MICROSTEPS_E0
      SET_OUTPUT(E0_MS1_PIN);
      SET_OUTPUT(E0_MS2_PIN);
    #endif
    #if HAS_MICROSTEPS_E1
      SET_OUTPUT(E1_MS1_PIN);
      SET_OUTPUT(E1_MS2_PIN);
    #endif
    static const uint8_t microstep_modes[] = MICROSTEP_MODES;
    for (uint16_t i = 0; i < COUNT(microstep_modes); i++)
      microstep_mode(i, microstep_modes[i]);
  }

  void Stepper::microstep_ms(uint8_t driver, int8_t ms1, int8_t ms2) {
    if (ms1 >= 0) switch (driver) {
      case 0: digitalWrite(X_MS1_PIN, ms1); break;
      #if HAS_MICROSTEPS_Y
        case 1: digitalWrite(Y_MS1_PIN, ms1); break;
      #endif
      #if HAS_MICROSTEPS_Z
        case 2: digitalWrite(Z_MS1_PIN, ms1); break;
      #endif
      #if HAS_MICROSTEPS_E0
        case 3: digitalWrite(E0_MS1_PIN, ms1); break;
      #endif
      #if HAS_MICROSTEPS_E1
        case 4: digitalWrite(E1_MS1_PIN, ms1); break;
      #endif
    }
    if (ms2 >= 0) switch (driver) {
      case 0: digitalWrite(X_MS2_PIN, ms2); break;
      #if HAS_MICROSTEPS_Y
        case 1: digitalWrite(Y_MS2_PIN, ms2); break;
      #endif
      #if HAS_MICROSTEPS_Z
        case 2: digitalWrite(Z_MS2_PIN, ms2); break;
      #endif
      #if HAS_MICROSTEPS_E0
        case 3: digitalWrite(E0_MS2_PIN, ms2); break;
      #endif
      #if HAS_MICROSTEPS_E1
        case 4: digitalWrite(E1_MS2_PIN, ms2); break;
      #endif
    }
  }

  void Stepper::microstep_mode(uint8_t driver, uint8_t stepping_mode) {
    switch (stepping_mode) {
      case 1: microstep_ms(driver, MICROSTEP1); break;
      case 2: microstep_ms(driver, MICROSTEP2); break;
      case 4: microstep_ms(driver, MICROSTEP4); break;
      case 8: microstep_ms(driver, MICROSTEP8); break;
      case 16: microstep_ms(driver, MICROSTEP16); break;
    }
  }

  void Stepper::microstep_readings() {
    SERIAL_PROTOCOLLNPGM("MS1,MS2 Pins");
    SERIAL_PROTOCOLPGM("X: ");
    SERIAL_PROTOCOL(READ(X_MS1_PIN));
    SERIAL_PROTOCOLLN(READ(X_MS2_PIN));
    #if HAS_MICROSTEPS_Y
      SERIAL_PROTOCOLPGM("Y: ");
      SERIAL_PROTOCOL(READ(Y_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(Y_MS2_PIN));
    #endif
    #if HAS_MICROSTEPS_Z
      SERIAL_PROTOCOLPGM("Z: ");
      SERIAL_PROTOCOL(READ(Z_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(Z_MS2_PIN));
    #endif
    #if HAS_MICROSTEPS_E0
      SERIAL_PROTOCOLPGM("E0: ");
      SERIAL_PROTOCOL(READ(E0_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E0_MS2_PIN));
    #endif
    #if HAS_MICROSTEPS_E1
      SERIAL_PROTOCOLPGM("E1: ");
      SERIAL_PROTOCOL(READ(E1_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E1_MS2_PIN));
    #endif
  }

#endif // HAS_MICROSTEPS

#if ENABLED(LIN_ADVANCE)

  void Stepper::advance_M905(const float &k) {
    if (k >= 0) extruder_advance_k = k;
    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("Advance factor: ", extruder_advance_k);
    SERIAL_EOL;
  }

#endif // LIN_ADVANCE
