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
  xyze_uint_t first_interval_q11_5{0};
  xyze_uint_t interval_q11_5{0};
} stepper_plan_t;

// Stepping plan handles steps for a whole frame (trajectory point delta)
typedef struct Stepping {
  AxisBits dir_bits;
  AxisBits step_bits;
  // the wait and interval vars could be uin16_t, but 32 bit mcus handle 32 bit vars faster (no unnecessary masking)
  xyze_ulong_t axis_interval_q5;
  xyze_ulong_t axis_wait_q5{FTM_NEVER};
  uint32_t frame_wait_q5{FTM_NEVER};

  FORCE_INLINE uint32_t advance_until_step();

  FORCE_INLINE void reset(){
    step_bits = 0;
    axis_interval_q5 = FTM_NEVER;
    axis_wait_q5 = FTM_NEVER;
    frame_wait_q5 = FTM_NEVER;

    stepper_plan_tail = stepper_plan_head = 0;
    curr_steps_q48_16.reset();
  }

  FORCE_INLINE void enqueue_stepper_plan(XYZEval<int64_t> next_steps_q48_16);

  stepper_plan_t stepper_plan_buff[FTM_BUFFER_SIZE];
  uint32_t stepper_plan_tail, stepper_plan_head;
  XYZEval<int64_t> curr_steps_q48_16;

  // Dequeue a plan.
  // Zero-copy consume; caller must use it before next dequeue if they keep a ref.
  // Done like this to avoid double copy.
  // e.g do: stepper_plan_t data = dequeue_stepper_plan(); this is ok
  FORCE_INLINE stepper_plan_t& dequeue_stepper_plan() {
    const uint32_t i = stepper_plan_tail;
    stepper_plan_tail = (i + 1u) & FTM_BUFFER_MASK;
    return stepper_plan_buff[i];
  }
  FORCE_INLINE bool stepper_is_busy() {
    return !(stepper_plan_is_empty() && frame_wait_q5 == FTM_NEVER);
  }
  FORCE_INLINE bool stepper_plan_is_empty() {
    return stepper_plan_head == stepper_plan_tail;
  }
  FORCE_INLINE bool stepper_plan_is_full() {
    return ((stepper_plan_head + 1) & FTM_BUFFER_MASK) == stepper_plan_tail;
  }
} stepping_t;
