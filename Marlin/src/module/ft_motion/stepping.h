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

  void reset();
  // Updates error and bresenham_iterations_pending, sets step_bits and returns interval until the next step (or end of frame).
  uint32_t advance_until_step();
  /**
   * If bresenham_iterations_pending, advance to next actual step.
   * Else, consume stepper data point
   * Then return interval until that next step
   */
  uint32_t plan();
  #define INTERVAL_PER_ITERATION (STEPPER_TIMER_RATE / FTM_STEPPER_FS)
  #define INTERVAL_PER_TRAJ_POINT (STEPPER_TIMER_RATE / FTM_FS)
  #define ITERATIONS_PER_TRAJ (FTM_STEPPER_FS * FTM_TS)
} stepping_t;
