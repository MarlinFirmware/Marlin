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
 * planner_bezier.cpp
 *
 * Compute and buffer movement commands for bezier curves
 *
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(BEZIER_CURVE_SUPPORT)

#include "planner.h"
#include "motion.h"
#include "temperature.h"

#include "../Marlin.h"
#include "../core/language.h"
#include "../gcode/queue.h"

// See the meaning in the documentation of cubic_b_spline().
#define MIN_STEP 0.002
#define MAX_STEP 0.1
#define SIGMA 0.1

// Compute the linear interpolation between two real numbers.
inline static float interp(float a, float b, float t) { return (1.0 - t) * a + t * b; }

/**
 * Compute a Bézier curve using the De Casteljau's algorithm (see
 * https://en.wikipedia.org/wiki/De_Casteljau%27s_algorithm), which is
 * easy to code and has good numerical stability (very important,
 * since Arudino works with limited precision real numbers).
 */
inline static float eval_bezier(float a, float b, float c, float d, float t) {
  float iab = interp(a, b, t);
  float ibc = interp(b, c, t);
  float icd = interp(c, d, t);
  float iabc = interp(iab, ibc, t);
  float ibcd = interp(ibc, icd, t);
  float iabcd = interp(iabc, ibcd, t);
  return iabcd;
}

/**
 * We approximate Euclidean distance with the sum of the coordinates
 * offset (so-called "norm 1"), which is quicker to compute.
 */
inline static float dist1(float x1, float y1, float x2, float y2) { return FABS(x1 - x2) + FABS(y1 - y2); }

/**
 * The algorithm for computing the step is loosely based on the one in Kig
 * (See https://sources.debian.net/src/kig/4:15.08.3-1/misc/kigpainter.cpp/#L759)
 * However, we do not use the stack.
 *
 * The algorithm goes as it follows: the parameters t runs from 0.0 to
 * 1.0 describing the curve, which is evaluated by eval_bezier(). At
 * each iteration we have to choose a step, i.e., the increment of the
 * t variable. By default the step of the previous iteration is taken,
 * and then it is enlarged or reduced depending on how straight the
 * curve locally is. The step is always clamped between MIN_STEP/2 and
 * 2*MAX_STEP. MAX_STEP is taken at the first iteration.
 *
 * For some t, the step value is considered acceptable if the curve in
 * the interval [t, t+step] is sufficiently straight, i.e.,
 * sufficiently close to linear interpolation. In practice the
 * following test is performed: the distance between eval_bezier(...,
 * t+step/2) is evaluated and compared with 0.5*(eval_bezier(...,
 * t)+eval_bezier(..., t+step)). If it is smaller than SIGMA, then the
 * step value is considered acceptable, otherwise it is not. The code
 * seeks to find the larger step value which is considered acceptable.
 *
 * At every iteration the recorded step value is considered and then
 * iteratively halved until it becomes acceptable. If it was already
 * acceptable in the beginning (i.e., no halving were done), then
 * maybe it was necessary to enlarge it; then it is iteratively
 * doubled while it remains acceptable. The last acceptable value
 * found is taken, provided that it is between MIN_STEP and MAX_STEP
 * and does not bring t over 1.0.
 *
 * Caveat: this algorithm is not perfect, since it can happen that a
 * step is considered acceptable even when the curve is not linear at
 * all in the interval [t, t+step] (but its mid point coincides "by
 * chance" with the midpoint according to the parametrization). This
 * kind of glitches can be eliminated with proper first derivative
 * estimates; however, given the improbability of such configurations,
 * the mitigation offered by MIN_STEP and the small computational
 * power available on Arduino, I think it is not wise to implement it.
 */
void cubic_b_spline(const float position[NUM_AXIS], const float target[NUM_AXIS], const float offset[4], float fr_mm_s, uint8_t extruder) {
  // Absolute first and second control points are recovered.
  const float first0 = position[X_AXIS] + offset[0],
              first1 = position[Y_AXIS] + offset[1],
              second0 = target[X_AXIS] + offset[2],
              second1 = target[Y_AXIS] + offset[3];
  float t = 0.0;

  float bez_target[4];
  bez_target[X_AXIS] = position[X_AXIS];
  bez_target[Y_AXIS] = position[Y_AXIS];
  float step = MAX_STEP;

  millis_t next_idle_ms = millis() + 200UL;

  while (t < 1.0) {

    thermalManager.manage_heater();
    millis_t now = millis();
    if (ELAPSED(now, next_idle_ms)) {
      next_idle_ms = now + 200UL;
      idle();
    }

    // First try to reduce the step in order to make it sufficiently
    // close to a linear interpolation.
    bool did_reduce = false;
    float new_t = t + step;
    NOMORE(new_t, 1.0);
    float new_pos0 = eval_bezier(position[X_AXIS], first0, second0, target[X_AXIS], new_t),
          new_pos1 = eval_bezier(position[Y_AXIS], first1, second1, target[Y_AXIS], new_t);
    for (;;) {
      if (new_t - t < (MIN_STEP)) break;
      const float candidate_t = 0.5 * (t + new_t),
                  candidate_pos0 = eval_bezier(position[X_AXIS], first0, second0, target[X_AXIS], candidate_t),
                  candidate_pos1 = eval_bezier(position[Y_AXIS], first1, second1, target[Y_AXIS], candidate_t),
                  interp_pos0 = 0.5 * (bez_target[X_AXIS] + new_pos0),
                  interp_pos1 = 0.5 * (bez_target[Y_AXIS] + new_pos1);
      if (dist1(candidate_pos0, candidate_pos1, interp_pos0, interp_pos1) <= (SIGMA)) break;
      new_t = candidate_t;
      new_pos0 = candidate_pos0;
      new_pos1 = candidate_pos1;
      did_reduce = true;
    }

    // If we did not reduce the step, maybe we should enlarge it.
    if (!did_reduce) for (;;) {
      if (new_t - t > MAX_STEP) break;
      const float candidate_t = t + 2.0 * (new_t - t);
      if (candidate_t >= 1.0) break;
      const float candidate_pos0 = eval_bezier(position[X_AXIS], first0, second0, target[X_AXIS], candidate_t),
                  candidate_pos1 = eval_bezier(position[Y_AXIS], first1, second1, target[Y_AXIS], candidate_t),
                  interp_pos0 = 0.5 * (bez_target[X_AXIS] + candidate_pos0),
                  interp_pos1 = 0.5 * (bez_target[Y_AXIS] + candidate_pos1);
      if (dist1(new_pos0, new_pos1, interp_pos0, interp_pos1) > (SIGMA)) break;
      new_t = candidate_t;
      new_pos0 = candidate_pos0;
      new_pos1 = candidate_pos1;
    }

    // Check some postcondition; they are disabled in the actual
    // Marlin build, but if you test the same code on a computer you
    // may want to check they are respect.
    /*
      assert(new_t <= 1.0);
      if (new_t < 1.0) {
        assert(new_t - t >= (MIN_STEP) / 2.0);
        assert(new_t - t <= (MAX_STEP) * 2.0);
      }
    */

    step = new_t - t;
    t = new_t;

    // Compute and send new position
    bez_target[X_AXIS] = new_pos0;
    bez_target[Y_AXIS] = new_pos1;
    // FIXME. The following two are wrong, since the parameter t is
    // not linear in the distance.
    bez_target[Z_AXIS] = interp(position[Z_AXIS], target[Z_AXIS], t);
    bez_target[E_AXIS] = interp(position[E_AXIS], target[E_AXIS], t);
    clamp_to_software_endstops(bez_target);

    #if HAS_UBL_AND_CURVES
      float pos[XYZ] = { bez_target[X_AXIS], bez_target[Y_AXIS], bez_target[Z_AXIS] };
      planner.apply_leveling(pos);
      planner.buffer_segment(pos[X_AXIS], pos[Y_AXIS], pos[Z_AXIS], bez_target[E_AXIS], fr_mm_s, active_extruder);
    #else
      planner.buffer_line_kinematic(bez_target, fr_mm_s, extruder);
    #endif
  }
}

#endif // BEZIER_CURVE_SUPPORT
