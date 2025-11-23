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

typedef struct stepper_plan {
  AxisBits dir_bits;
  xyze_uint_t first_interval_q11_5;
  xyze_uint_t interval_q11_5;
} stepper_plan_t;

// Stepping plan handles steps for a whole frame (trajectory point delta)
typedef struct Stepping {

  //
  // ISR part
  //

  AxisBits dir_bits;
  AxisBits step_bits;

  // The wait and interval vars could be uin16_t, but 32 bit MCUs handle 32 bit vars faster (no unnecessary masking)
  xyze_ulong_t axis_interval_q5{ LOGICAL_AXIS_LIST_1(FTM_NEVER) };
  xyze_ulong_t ticks_left_per_axis_q5{ LOGICAL_AXIS_LIST_1(FTM_NEVER) };
  uint32_t ticks_left_in_frame_q5 = FTM_NEVER;

  FORCE_INLINE uint32_t advance_until_step();

  FORCE_INLINE void reset() {
    step_bits = 0;
    axis_interval_q5 = FTM_NEVER;
    ticks_left_per_axis_q5 = FTM_NEVER;
    ticks_left_in_frame_q5 = FTM_NEVER;

    stepper_plan_tail = stepper_plan_head = 0;
    curr_steps_q48_16.reset();
  }

  //
  // Buffering part
  //

  stepper_plan_t stepper_plan_buff[FTM_BUFFER_SIZE];
  uint32_t stepper_plan_tail, stepper_plan_head;
  XYZEval<int64_t> curr_steps_q48_16;

  FORCE_INLINE void enqueue(XYZEval<int64_t> next_steps_q48_16);

  FORCE_INLINE stepper_plan_t& dequeue();

  FORCE_INLINE bool is_busy() {
    return !(is_empty() && ticks_left_in_frame_q5 == 0);
  }
  FORCE_INLINE bool is_empty() {
    return stepper_plan_head == stepper_plan_tail;
  }
  FORCE_INLINE bool is_full() {
    return ((stepper_plan_head + 1) & FTM_BUFFER_MASK) == stepper_plan_tail;
  }
} stepping_t;

//
// uint64-free equivalent of: ((uint64_t)a * b) >> 16
//
FORCE_INLINE uint32_t a_times_b_shift_16(uint32_t a, uint32_t b) {
  uint32_t hi = a >> 16;
  uint32_t lo = a & 0xFFFFu;
  uint32_t hi_prod = hi * (uint32_t)b;
  uint32_t lo_prod = lo * (uint32_t)b;
  uint32_t r = hi_prod + (lo_prod >> 16);
  return r;
}

constexpr uint32_t ONE_Q5 = 1 << 5;
constexpr uint32_t Q5_INTEGER_MASK = ~(ONE_Q5 - 1);
constexpr uint32_t FRAME_TICKS_Q5 = TIMER_TICKS_PER_FRAME << 5;

FORCE_INLINE uint32_t Stepping::advance_until_step() {
  step_bits.reset();
  uint32_t ticks_to_wait_q5 = 0;
  while (1) {
    // find next step
    uint32_t ticks_to_next_step_q5 = FTM_NEVER;
    #define _RUN_LOOP(A) NOMORE(ticks_to_next_step_q5, ticks_left_per_axis_q5.A);
    LOGICAL_AXIS_MAP(_RUN_LOOP)
    #undef _RUN_LOOP

    if (ticks_to_next_step_q5 > ticks_left_in_frame_q5) {
      // frame ends before next step
      if (is_empty()) {
        ticks_left_in_frame_q5 = 0;
        ticks_left_per_axis_q5 = FTM_NEVER;
        return FTM_NEVER;
      }

      const uint32_t frame_wait_floor_q5 = ticks_left_in_frame_q5 & Q5_INTEGER_MASK;
      ticks_to_wait_q5 += frame_wait_floor_q5;
      ticks_left_in_frame_q5 -= frame_wait_floor_q5;

      const stepper_plan_t next = dequeue();
      dir_bits         = next.dir_bits;
      axis_interval_q5 = next.interval_q11_5.asUInt32();
      // Note the frame actually ends a fraction of a tick later, so ticks_left_in_frame_q5 still has that fraction.
      // Instead of discarding that time, we delay both the end of the next frame, and al first steps by that amount.
      ticks_left_per_axis_q5  = next.first_interval_q11_5.asUInt32();
      ticks_left_per_axis_q5 += ticks_left_in_frame_q5;
      ticks_left_in_frame_q5  += FRAME_TICKS_Q5;
    } else {
      // step happens before frame end
      // advance to it
      const uint32_t wait_floor_q5 = ticks_to_next_step_q5 & Q5_INTEGER_MASK;
      ticks_to_wait_q5 += wait_floor_q5;
      ticks_left_in_frame_q5 -= wait_floor_q5;
      #define _RUN_LOOP(A) ticks_left_per_axis_q5.A -= wait_floor_q5;
      LOGICAL_AXIS_MAP(_RUN_LOOP)
      #undef _RUN_LOOP
      // build step_bits
      #define _RUN_LOOP(A)                                \
        if (ticks_left_per_axis_q5.A < ONE_Q5) {          \
          step_bits.A = 1;                                \
          ticks_left_per_axis_q5.A += axis_interval_q5.A; \
        }
      LOGICAL_AXIS_MAP(_RUN_LOOP)
      #undef _RUN_LOOP
      return ticks_to_wait_q5 >> 5;
    }
  }
}

FORCE_INLINE void Stepping::enqueue(XYZEval<int64_t> next_steps_q48_16) {
  stepper_plan_t stepper_plan;
  auto _run_axis = [&](const AxisEnum A) __attribute__((always_inline)) {
    const int64_t offset_curr_q48_16 = curr_steps_q48_16[A];
    const int64_t offset_next_q48_16 = next_steps_q48_16[A];
    curr_steps_q48_16[A] = next_steps_q48_16[A];
    const bool new_dir = offset_next_q48_16 >= offset_curr_q48_16;
    stepper_plan.dir_bits[A] = new_dir;
    const uint32_t delta_q16_16 = abs(offset_next_q48_16 - offset_curr_q48_16);
    uint32_t curr_phase_q1_16 = offset_curr_q48_16 & 0xFFFF;
    uint32_t next_phase_q1_16 = offset_next_q48_16 & 0xFFFF;
    if (!new_dir) {
      // When going backwards, the phase is 1-phase
      curr_phase_q1_16 = (1<<16) - curr_phase_q1_16;
      next_phase_q1_16 = (1<<16) - next_phase_q1_16;
    }
    // When going, e.g., from 0.6 to 1.0, the delta is not a whole step,
    // but the phase overflow indicates a step.
    const uint32_t carry = curr_phase_q1_16 > next_phase_q1_16;
    // steps_to_make = integer steps + potential fraction crossing an integer
    const uint16_t steps_to_make = (delta_q16_16 >> 16) + carry;
    if (steps_to_make == 0) {
      stepper_plan.first_interval_q11_5[A] = FTM_NEVER;
      stepper_plan.interval_q11_5[A]       = FTM_NEVER;
      return;
    }
    const uint32_t interval_q27_5 = ((uint32_t)TIMER_TICKS_PER_FRAME << 21) / delta_q16_16;
    const uint32_t current_frame_phase_q27_5 = a_times_b_shift_16(interval_q27_5, curr_phase_q1_16);
    uint16_t first_interval_q11_5 = interval_q27_5 - current_frame_phase_q27_5;
    // The calculation of interval_q27_5 may undershoot its value by a fraction
    // due to integer (floor) division. This small fractional error can
    // occasionally make a spurious step fit inside this frame.
    // To avoid that corner case, the first interval is incremented just enough
    // for it to not fit.
    const int32_t tick_of_spurious_step_q27_5 = first_interval_q11_5 + interval_q27_5 * steps_to_make;
    if (tick_of_spurious_step_q27_5 <= (TIMER_TICKS_PER_FRAME << 5)) {
      first_interval_q11_5 += (TIMER_TICKS_PER_FRAME << 5) - tick_of_spurious_step_q27_5 + 1;
    }
    stepper_plan.first_interval_q11_5[A] = first_interval_q11_5;
    stepper_plan.interval_q11_5[A]       = _MIN(interval_q27_5, FTM_NEVER);
  };

  #define _RUN_AXIS(A) _run_axis(_AXIS(A));
  LOGICAL_AXIS_MAP(_RUN_AXIS);
  #undef _RUN_AXIS

  stepper_plan_buff[stepper_plan_head] = stepper_plan;
  stepper_plan_head = (stepper_plan_head + 1U) & FTM_BUFFER_MASK;
}

// Dequeue a plan.
// Zero-copy consume; caller must use it before next dequeue if they keep a ref.
// Done like this to avoid double copy.
// e.g do: stepper_plan_t data = dequeue(); this is ok
FORCE_INLINE stepper_plan_t& Stepping::dequeue() {
  const uint32_t i = stepper_plan_tail;
  stepper_plan_tail = (i + 1u) & FTM_BUFFER_MASK;
  return stepper_plan_buff[i];
}
