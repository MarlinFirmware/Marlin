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

void Stepping::reset() {
  stepper_plan.reset();
  delta_error_q32.set(LOGICAL_AXIS_LIST_1(1UL << 31)); // Start as 0.5 in q32 so steps are rounded
  step_bits = 0;
  bresenham_iterations_pending = 0;
}

uint32_t Stepping::advance_until_step() {
  xyze_ulong_t space_q32 = -delta_error_q32 + UINT32_MAX; // How much accumulation until a step in any axis is ALMOST due
                                                          // TODO: Add operators to types.h for scalar destination.

  xyze_ulong_t advance_q32 = stepper_plan.advance_dividend_q0_32;
  uint32_t iterations = bresenham_iterations_pending;
  // Per-axis lower-bound approx of floor(space_q32/adv), min across axes (lower bound because this fast division underestimates result by up to 1)
  //#define RUN_AXIS(A) if (advance_q32.A > 0) NOMORE(iterations, space_q32.A / advance_q32.A);
  #define RUN_AXIS(A) NOMORE(iterations, uint32_t((uint64_t(space_q32.A) * advance_dividend_reciprocal.A) >> 32));
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

uint32_t Stepping::plan() {
  uint32_t intervals = 0;
  if (bresenham_iterations_pending > 0) {
    intervals = advance_until_step();
    if (bool(step_bits)) return intervals; // steps to make => return the wait time so it gets done in due time
    // Else all Bresenham iterations were advanced without steps => this is just the frame end, so plan the next one directly and accumulate the wait
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

  if (stepper_plan.advance_dividend_q0_32 == 0) {
    // Don't waste time in zero motion traj points
    bresenham_iterations_pending = 0;
    step_bits = 0;
    return INTERVAL_PER_TRAJ_POINT;
  }

  // This vector division is unavoidable, but it saves a division per step during Bresenham
  // The reciprocal is actually 2^32/dividend, but that requires dividing a uint64_t, which quite expensive
  // Since even the real reciprocal may underestimate the quotient by 1 anyway already, this optimisation doesn't
  // make things worse. This underestimation is compensated for in advance_until_step.
  #define _DIVIDEND_RECIP(A) do{ \
    const uint32_t d = stepper_plan.advance_dividend_q0_32.A; \
    advance_dividend_reciprocal.A = d ? UINT32_MAX / d : UINT32_MAX; \
  }while(0);
  LOGICAL_AXIS_MAP(_DIVIDEND_RECIP);
  #undef _DIVIDEND_RECIP

  bresenham_iterations_pending = ITERATIONS_PER_TRAJ;
  return intervals + advance_until_step();
}

#endif // FT_MOTION
