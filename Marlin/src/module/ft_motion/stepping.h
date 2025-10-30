/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  xyze_ulong_t advance_dividend_q0_32{0};
  void reset() { advance_dividend_q0_32.reset(); }
} stepper_plan_t;


// Stepping plan handles steps for a while frame (trajectory point delta)
typedef struct Stepping {
  stepper_plan_t stepper_plan;
  xyze_ulong_t advance_dividend_reciprocal{0}; // Note this 32 bit reciprocal underestimates quotients by at most one.
  xyze_ulong_t delta_error_q32{ LOGICAL_AXIS_LIST_1(_BV32(31)) };
  AxisBits step_bits;
  uint32_t bresenham_iterations_pending;

  void reset() {
    stepper_plan.reset();
    delta_error_q32.set(LOGICAL_AXIS_ARRAY_1(_BV32(31))); // start as 0.5 in q32 so steps are rounded
    step_bits = 0;
    bresenham_iterations_pending = 0;
  }

  #define INTERVAL_PER_ITERATION (STEPPER_TIMER_RATE / FTM_STEPPER_FS)
  #define INTERVAL_PER_TRAJ_POINT (STEPPER_TIMER_RATE / FTM_FS)
  #define ITERATIONS_PER_TRAJ (FTM_STEPPER_FS * FTM_TS)

  // Updates error and bresenham_iterations_pending, sets step_bits and returns interval until the next step (or end of frame).
  uint32_t advance_until_step() {
    xyze_ulong_t space_q32 = -delta_error_q32 + UINT32_MAX; // How much accumulation until a step in any axis is ALMOST due
                                                        // scalar in the right hand because types.h is missing scalar on left cases

    xyze_ulong_t advance_q32 = stepper_plan.advance_dividend_q0_32;
    uint32_t iterations = bresenham_iterations_pending;
    // Per-axis lower-bound approx of floor(space_q32/adv), min across axes (lower bound because this fast division underestimates result by up to 1)
    // #define RUN_AXIS(A) if(advance_q32.A > 0) NOMORE(iterations, space_q32.A/advance_q32.A);
    #define RUN_AXIS(A) if(advance_q32.A > 0) NOMORE(iterations, uint32_t((uint64_t(space_q32.A) * advance_dividend_reciprocal.A) >> 32));
    LOGICAL_AXIS_MAP(RUN_AXIS);
    #undef RUN_AXIS

    #define RUN_AXIS(A) delta_error_q32.A += advance_q32.A * iterations;
    LOGICAL_AXIS_MAP(RUN_AXIS);
    #undef RUN_AXIS

    bresenham_iterations_pending -= iterations;
    step_bits = 0;
    // iterations may be underestimated by 1 by the cheap division, therefore we may have to do 2 iterations here
    while (bresenham_iterations_pending && !(bool)step_bits) {
      iterations++;
      bresenham_iterations_pending--;
      #define RUN_AXIS(A) do{                                                      \
          delta_error_q32.A += stepper_plan.advance_dividend_q0_32.A;              \
          step_bits.A = delta_error_q32.A < stepper_plan.advance_dividend_q0_32.A; \
        }while(0);
      LOGICAL_AXIS_MAP(RUN_AXIS);
      #undef RUN_AXIS
    }

    return iterations * INTERVAL_PER_ITERATION;
  }

  /**
   * If bresenham_iterations_pending, advance to next actual step.
   * Else, consume stepper data point
   * Then return interval until that next step
   */
  uint32_t plan() {
    uint32_t intervals = 0;
    if (bresenham_iterations_pending > 0) {
      intervals = advance_until_step();
      if (bool(step_bits)) return intervals; // steps to make => return the wait time so it gets done in due time
      // Else all bresenham iterations were advanced without steps => this is just the frame end, so plan the next one directly and accumulate the wait
    }

    if (ftMotion.stepper_plan_is_empty()) {
      bresenham_iterations_pending = 0;
      step_bits = 0;
      return INTERVAL_PER_TRAJ_POINT;
    }

    AxisBits old_dir_bits = stepper_plan.dir_bits;
    stepper_plan = ftMotion.dequeue_stepper_plan();
    const AxisBits dir_flip_mask = old_dir_bits ^ stepper_plan.dir_bits;  // axes that must toggle now
    if (dir_flip_mask) {
      #define _HANDLE_DIR_CHANGES(A) if (dir_flip_mask.A) delta_error_q32.A *= -1;
      LOGICAL_AXIS_MAP(_HANDLE_DIR_CHANGES);
      #undef _HANDLE_DIR_CHANGES
    }

    if (!(bool)stepper_plan.advance_dividend_q0_32) {
      // don't waste time in zero motion traj points
      bresenham_iterations_pending = 0;
      step_bits = 0;
      return INTERVAL_PER_TRAJ_POINT;
    }

    advance_dividend_reciprocal.set(LOGICAL_AXIS_ARRAY_1(UINT32_MAX));
    advance_dividend_reciprocal /= stepper_plan.advance_dividend_q0_32; // this vector division is unavoidable, but it saves a division per step during bresenham
    /* 2^32 / (dividend*2^32)*/
    /* 1 / (dividend)*/
    bresenham_iterations_pending = ITERATIONS_PER_TRAJ;
    return intervals + advance_until_step();
  }
} stepping_t;