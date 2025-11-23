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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(FT_MOTION)

#include "stepping.h"
#include "../ft_motion.h"

// uint64-free equivalent of: ((uint64_t)a * b) >> 16
uint32_t a_times_b_shift_16(uint32_t a, uint32_t b){
    uint32_t hi = a >> 16;
    uint32_t lo = a & 0xFFFFu;
    uint32_t hi_prod = hi * (uint32_t)b;
    uint32_t lo_prod = lo * (uint32_t)b;
    uint32_t r = hi_prod + (lo_prod >> 16);
    return r;
}

uint32_t Stepping::advance_until_step() {
  step_bits.reset();
  // find next event
  uint32_t next_event_q5 = frame_wait_q5;
  #define _RUN_LOOP(A) NOMORE(next_event_q5, axis_wait_q5.A);
  LOGICAL_AXIS_MAP(_RUN_LOOP)
  #undef _RUN_LOOP

  // advance until it
  constexpr uint32_t ONE_Q5 = 1 << 5;
  constexpr uint32_t Q5_INTEGER_MASK = ~(ONE_Q5 - 1);
  constexpr uint32_t FRAME_TICKS_Q5 = TIMER_TICKS_PER_FRAME << 5;

  const uint32_t next_event_round_q5 = next_event_q5 & Q5_INTEGER_MASK;
  frame_wait_q5 -= next_event_round_q5;
  #define _RUN_LOOP(A) axis_wait_q5.A -= next_event_round_q5;
  LOGICAL_AXIS_MAP(_RUN_LOOP)
  #undef _RUN_LOOP
  // if frame ended, load next
  if (frame_wait_q5 < ONE_Q5) {
    if (is_empty()) {
      frame_wait_q5 = FTM_NEVER;
      axis_wait_q5 = FTM_NEVER;
      return FTM_NEVER;
    }

    const stepper_plan_t next = dequeue();
    dir_bits         = next.dir_bits;
    axis_interval_q5 = next.interval_q11_5;
    // Note the frame actually ends a fraction of a tick later, so frame_wait_q5 still has that fraction.
    // Instead of discarding that time, we delay both the end of the next frame, and al first steps by that amount.
    axis_wait_q5     = next.first_interval_q11_5;
    axis_wait_q5    += frame_wait_q5; // avoid suming XYZEval + scalar in the line above b/c it creates a new struct
    frame_wait_q5   += FRAME_TICKS_Q5;
  }

  // build step_bits
  #define _RUN_LOOP(A)                          \
    if (axis_wait_q5.A < ONE_Q5) {              \
        step_bits.A = 1;                        \
        axis_wait_q5.A += axis_interval_q5.A;   \
    }
  LOGICAL_AXIS_MAP(_RUN_LOOP)
  #undef _RUN_LOOP

  return next_event_round_q5 >> 5;
}

void Stepping::enqueue(xyze_float_t traj_coords) {
  // Convert trajectory to step delta
  #define _TOSTEPS_q16(A, B) int64_t(traj_coords.A * planner.settings.axis_steps_per_mm[B] * (1ULL << 16))
  XYZEval<int64_t> next_steps_q48_16 = LOGICAL_AXIS_ARRAY(
    _TOSTEPS_q16(e, ftMotion.block_extruder_axis),
    _TOSTEPS_q16(x, X_AXIS), _TOSTEPS_q16(y, Y_AXIS), _TOSTEPS_q16(z, Z_AXIS),
    _TOSTEPS_q16(i, I_AXIS), _TOSTEPS_q16(j, J_AXIS), _TOSTEPS_q16(k, K_AXIS),
    _TOSTEPS_q16(u, U_AXIS), _TOSTEPS_q16(v, V_AXIS), _TOSTEPS_q16(w, W_AXIS)
  );
  #undef _TOSTEPS_q32

  constexpr uint32_t HALF_PHASE_OFFSET = (1 << 15); // to make steps at .5 crossings instead of integers to center the error
  stepper_plan_t stepper_plan;
  #define _RUN_AXIS(A) do {                                                           \
    int64_t offset_curr_q48_16 = curr_steps_q48_16.A + HALF_PHASE_OFFSET;             \
    int64_t offset_next_q48_16 = next_steps_q48_16.A + HALF_PHASE_OFFSET;             \
    curr_steps_q48_16.A = next_steps_q48_16.A;                                        \
    bool new_dir = offset_next_q48_16 >= offset_curr_q48_16;                          \
    stepper_plan.dir_bits.A = new_dir;                                                \
    uint32_t delta_q16_16 = abs(offset_next_q48_16 - offset_curr_q48_16);             \
    uint32_t curr_phase_q1_16 = offset_curr_q48_16 & 0xFFFF;                          \
    uint32_t next_phase_q1_16 = offset_next_q48_16 & 0xFFFF;                          \
    if (!new_dir) {                                                                   \
      /* when going backwards, the phase is 1-phase */                                \
      curr_phase_q1_16 = (1<<16) - curr_phase_q1_16;                                  \
      next_phase_q1_16 = (1<<16) - next_phase_q1_16;                                  \
    }                                                                                 \
    /* When going e.g from 0.6 to 1.0, the delta is not a whole step, */              \
    /* but the phase overflow indicates a step.  */                                   \
    uint32_t carry = curr_phase_q1_16 > next_phase_q1_16;                             \
    /* steps_to_make = integer steps + potential fraction crossing an integer */      \
    uint16_t steps_to_make = (delta_q16_16 >> 16) + carry;                            \
    if (steps_to_make == 0) {                                                         \
      stepper_plan.first_interval_q11_5.A = FTM_NEVER;                                \
      stepper_plan.interval_q11_5.A       = FTM_NEVER;                                \
      break;                                                                          \
    }                                                                                 \
    uint32_t interval_q27_5 = ((uint32_t)TIMER_TICKS_PER_FRAME << 21) / delta_q16_16; \
    uint32_t current_frame_phase_q27_5 =                                              \
      a_times_b_shift_16(interval_q27_5, curr_phase_q1_16);                           \
    uint16_t first_interval_q11_5 = interval_q27_5 - current_frame_phase_q27_5;       \
    /* The calculation of interval_q27_5 may undershoot its value by a fraction */    \
    /* due to integer (floor) division. This small fractional error can */            \
    /* ocasionally make a spurious step fit inside this frame. */                     \
    /* To avoid that corner case, the first interval is incremented just enough */    \
    /* for it to not fit.   */                                                        \
    int32_t tick_of_spurious_step_q27_5 = first_interval_q11_5 +                      \
                                          interval_q27_5 * steps_to_make;             \
    if (tick_of_spurious_step_q27_5 <= (TIMER_TICKS_PER_FRAME << 5)) {                \
      first_interval_q11_5 += (TIMER_TICKS_PER_FRAME << 5) -                          \
                              tick_of_spurious_step_q27_5 + 1;                        \
    }                                                                                 \
    stepper_plan.first_interval_q11_5.A = first_interval_q11_5;                       \
    stepper_plan.interval_q11_5.A       = MIN(interval_q27_5, FTM_NEVER);             \
  } while(0);                                                                         \

  LOGICAL_AXIS_MAP(_RUN_AXIS);
  #undef _RUN_AXIS

  stepper_plan_buff[stepper_plan_head] = stepper_plan;
  stepper_plan_head = (stepper_plan_head + 1u) & FTM_BUFFER_MASK;
}

// Dequeue a plan.
// Zero-copy consume; caller must use it before next dequeue if they keep a ref.
// Done like this to avoid double copy.
// e.g do: stepper_plan_t data = dequeue(); this is ok
stepper_plan_t& Stepping::dequeue() {
  const uint32_t i = stepper_plan_tail;
  stepper_plan_tail = (i + 1u) & FTM_BUFFER_MASK;
  return stepper_plan_buff[i];
}
#endif // FT_MOTION
