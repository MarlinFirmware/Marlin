/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../inc/MarlinConfig.h"

//
// uint64-free equivalent of: ((uint64_t)a * b) >> 16
//
FORCE_INLINE constexpr uint32_t a_times_b_shift_16(const uint32_t a, const uint32_t b) {
  const uint32_t hi = a >> 16, lo = a & 0x0000FFFF;
  return (hi * b) + ((lo * b) >> 16);
}
#define FTM_NEVER uint32_t(UINT16_MAX)                               // Reserved number to indicate "no ticks in this frame" (FRAME_TICKS_FP+1 would work too)
constexpr uint32_t FRAME_TICKS = STEPPER_TIMER_RATE / FTM_FS;        // Timer ticks in a frame
static_assert(FRAME_TICKS < FTM_NEVER, "(STEPPER_TIMER_RATE / FTM_FS) must be < 2^16 (otherwise fixed-point numbers exceed uint16 vars).");
constexpr uint32_t FTM_Q_INT = 32u - __builtin_clz(FRAME_TICKS + 1); // Bits to represent the max value (duration of a frame, +1 one for FTM_NEVER).
                                                                     // "clz" counts leading zeroes.
constexpr uint32_t FTM_Q = 16 - FTM_Q_INT;                           // uint16 interval fractional bits.
                                                                     // Intervals buffer has fixed point numbers with the point on this position

// The _FP and _fp suffixes mean the number is in fixed point format with the point at the FTM_Q position.
// See: https://en.wikipedia.org/wiki/Fixed-point_arithmetic
// E.g number_fp = number << FTM_Q
//     number == (number_fp >> FTM_Q)
constexpr uint32_t ONE_FP = 1 << FTM_Q;                   // Number 1 in fixed point format
constexpr uint32_t FP_FLOOR_MASK = ~(ONE_FP - 1);         // Bit mask to do FLOOR in fixed point
constexpr uint32_t FRAME_TICKS_FP = FRAME_TICKS << FTM_Q; // Ticks in a frame in fixed point

typedef struct stepper_plan {
  AxisBits dir_bits;
  xyze_uint_t first_interval_fp;
  xyze_uint_t interval_fp;
} stepper_plan_t;

// Stepping plan handles steps for a whole frame (trajectory point delta)
typedef struct Stepping {

  //
  // ISR part
  //

  AxisBits dir_bits;
  AxisBits step_bits;

  xyze_ulong_t axis_interval_fp{ LOGICAL_AXIS_LIST_1(FTM_NEVER) };
  xyze_ulong_t ticks_left_per_axis_fp{ LOGICAL_AXIS_LIST_1(FTM_NEVER) };
  uint32_t ticks_left_in_frame_fp = 0;

  // Return how many full ticks we must wait before
  // generating the next step pulse. The call is inexpensive:
  //  - no heap, no locks – pure arithmetic on pre-computed data
  FORCE_INLINE uint32_t advance_until_step() {
    step_bits = 0;
    uint32_t ticks_to_wait_fp = 0;

    for (;;) {
      // Smallest remaining tick count among all axes – next step time
      const uint32_t ticks_to_next_step_fp = ticks_left_per_axis_fp.small();

      // Does the frame finish before this next step occurs?
      if (ticks_to_next_step_fp > ticks_left_in_frame_fp) {
        // Frame ends before next step
        if (is_empty()) {
          ticks_left_in_frame_fp = 0;
          ticks_left_per_axis_fp = FTM_NEVER;
          return FTM_NEVER;
        }

        // Consume the rest of this frame's time
        const uint32_t wait_floor_fp = ticks_left_in_frame_fp & FP_FLOOR_MASK;
        ticks_to_wait_fp += wait_floor_fp;
        ticks_left_in_frame_fp -= wait_floor_fp;

        //
        // Pull the next plan – it already contains:
        //  - direction bits
        //  - first_interval_fp (time to the first step)
        //  - interval_fp       (repeating step period)
        //
        const stepper_plan_t next = dequeue();
        dir_bits         = next.dir_bits;
        axis_interval_fp = next.interval_fp.asUInt32();

        // Note the frame will actually end a fraction of a tick in the future, and ticks_left_in_frame_fp still has that fraction.
        // Instead of discarding that time, we delay both the end of the next frame, and all first steps by that amount.
        ticks_left_per_axis_fp  = next.first_interval_fp.asUInt32();
        ticks_left_per_axis_fp += ticks_left_in_frame_fp;
        ticks_left_in_frame_fp += FRAME_TICKS_FP;   // Start a fresh frame
      }
      else {
        // Step happens before frame end
        // Advance to it
        const uint32_t wait_floor_fp = ticks_to_next_step_fp & FP_FLOOR_MASK;
        ticks_to_wait_fp += wait_floor_fp;
        ticks_left_in_frame_fp -= wait_floor_fp;
        ticks_left_per_axis_fp -= wait_floor_fp;

        // Build step_bits: any axis whose counter < ONE_FP should step before the next tick, so we tick now
        // unless the frame ends earlier.
        uint32_t limit_fp = _MIN(ONE_FP - 1, ticks_left_in_frame_fp);
        auto _set_step_bit = [&](const AxisEnum A) __attribute__((always_inline)) {
          if (ticks_left_per_axis_fp[A] <= limit_fp) {
            step_bits[A] = 1;
            ticks_left_per_axis_fp[A] += axis_interval_fp[A];
          }
        };
        LOGICAL_AXIS_CALL(_set_step_bit);

        return ticks_to_wait_fp >> FTM_Q;   // Convert fixed point back to whole ticks
      }
    } // loop forever
  }

  FORCE_INLINE void reset() {
    step_bits = 0;
    axis_interval_fp = FTM_NEVER;
    ticks_left_per_axis_fp = FTM_NEVER;
    ticks_left_in_frame_fp = 0;

    stepper_plan_tail = stepper_plan_head = 0;
    curr_steps_q48_16.reset();
  }

  //
  // Buffering part
  //

  stepper_plan_t stepper_plan_buff[FTM_BUFFER_SIZE];
  uint32_t stepper_plan_tail = 0, stepper_plan_head = 0;
  XYZEval<int64_t> curr_steps_q48_16{0};

  FORCE_INLINE void enqueue(XYZEval<int64_t> next_steps_q48_16) {

    stepper_plan_t stepper_plan;
    constexpr uint32_t HALF_PHASE_OFFSET = (1UL << 15); // to make steps at .5 crossings instead of integers to center the error

    auto _run_axis = [&](const AxisEnum A) __attribute__((always_inline)) {
      // Add half-phase offset to keep step error centred
      const int64_t offset_curr_q48_16 = curr_steps_q48_16[A] + HALF_PHASE_OFFSET,
                    offset_next_q48_16 = next_steps_q48_16[A] + HALF_PHASE_OFFSET;
      curr_steps_q48_16[A] = next_steps_q48_16[A];

      // Determine direction change
      const bool new_dir = offset_next_q48_16 >= offset_curr_q48_16;
      stepper_plan.dir_bits[A] = new_dir;

      // Δsteps in Q16.16 format – magnitude only
      const uint32_t delta_q16_16 = abs(offset_next_q48_16 - offset_curr_q48_16);

      // Current / next phase (fractional part of the position)
      uint32_t curr_phase_q1_16 = offset_curr_q48_16 & 0xFFFF,
               next_phase_q1_16 = offset_next_q48_16 & 0xFFFF;
      if (!new_dir) {
        // When going backwards, the phase is 1-phase
        curr_phase_q1_16 = (1UL<<16) - curr_phase_q1_16;
        next_phase_q1_16 = (1UL<<16) - next_phase_q1_16;
      }
      // When going, e.g., from 0.6 to 1.0, the delta is not a whole step,
      // but the phase overflow indicates a step.
      const uint32_t carry = curr_phase_q1_16 > next_phase_q1_16;

      // steps_to_make = integer steps + potential fraction crossing an integer
      const uint16_t steps_to_make = (delta_q16_16 >> 16) + carry;

      if (steps_to_make == 0) {                // No steps on this axis
        stepper_plan.first_interval_fp[A] = FTM_NEVER;
        stepper_plan.interval_fp[A]       = FTM_NEVER;
        return;
      }

      // Compute the exact time between steps.
      //   interval = ticks_per_frame / delta
      //   current_frame_phase_fp = interval * curr_phase
      const uint32_t interval_fp = (FRAME_TICKS_FP << 16) / delta_q16_16,
                     current_frame_phase_fp = a_times_b_shift_16(interval_fp, curr_phase_q1_16);
      uint32_t first_interval_fp = interval_fp - current_frame_phase_fp;

      // The calculation of interval_fp may undershoot its value by a fraction
      // due to integer (floor) division. This small fractional error can
      // occasionally make a spurious step fit inside this frame.
      // To avoid that corner case, the first interval is incremented just enough
      // for it to not fit.
      const uint32_t tick_of_spurious_step_fp = first_interval_fp + interval_fp * steps_to_make;
      if (tick_of_spurious_step_fp <= FRAME_TICKS_FP) {
        first_interval_fp += FRAME_TICKS_FP - tick_of_spurious_step_fp + 1;
      }

      stepper_plan.first_interval_fp[A] = _MIN(first_interval_fp, FTM_NEVER);
      stepper_plan.interval_fp[A]       = _MIN(interval_fp, FTM_NEVER);
    };

    LOGICAL_AXIS_CALL(_run_axis);

    // Store the plan into the circular buffer
    stepper_plan_buff[stepper_plan_head] = stepper_plan;
    stepper_plan_head = (stepper_plan_head + 1U) & FTM_BUFFER_MASK;
  }

  // Dequeue a plan.
  // Zero-copy consume; caller must use it before next dequeue if they keep a ref.
  // Done like this to avoid double copy.
  // e.g do: stepper_plan_t data = dequeue(); this is ok
  FORCE_INLINE stepper_plan_t& dequeue() {
    const uint32_t i = stepper_plan_tail;
    stepper_plan_tail = (i + 1U) & FTM_BUFFER_MASK;
    return stepper_plan_buff[i];
  }

  //
  // Simple helper predicates
  //

  FORCE_INLINE bool is_busy() {
    return !(is_empty() && ticks_left_in_frame_fp == 0);
  }
  FORCE_INLINE bool is_empty() {
    return stepper_plan_head == stepper_plan_tail;
  }
  FORCE_INLINE bool is_full() {
    return ((stepper_plan_head + 1) & FTM_BUFFER_MASK) == stepper_plan_tail;
  }

} stepping_t;
