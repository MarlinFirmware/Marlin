/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Internal math functions for the constant-jolt trajectory planner.
 * Only included by constant_jolt_planner.cpp and trajectory_constant_jolt.cpp.
 * NOT included via ft_motion.h — avoids bloating all 19+ TUs with unused code.
 */

#include "trajectory_constant_jolt.h"  // for constant_jolt::simulatePhase, CJP_* defines, _MIN, _MAX, SQRT
#include <math.h>

namespace constant_jolt {

// Solve phaseDist(v, a, j, t) = d for t ∈ [t_lo, t_hi].
// Bracketed Newton with quadratic initial guess. Returns t (upper bracket).
// Used by getVelocityAtDistance and plan_decel_with_carry inner solve.
static inline float solvePhaseTime(
  float v, float a, float j, float d,
  float t_lo, float t_hi
) {
  // Quadratic initial guess: v*t + 0.5*a*t² = d (ignores cubic jerk term).
  float t;
  if (ABS(a) > 1e-6f) {
    const float disc = v * v + 2.0f * a * d;
    t = (disc > 0.0f) ? (-v + SQRT(disc)) / a : d / _MAX(v, 1e-6f);
  }
  else {
    t = (v > 1e-6f) ? d / v : 0.5f * (t_lo + t_hi);
  }
  t = _MAX(t_lo, _MIN(t, t_hi));
  for (int i = 0; i < 16; i++) {
    const float f = phaseDist(v, a, j, t) - d;
    if (f < 0.0f) t_lo = t; else t_hi = t;
    if (t_hi - t_lo < 1e-7f) break;
    const float fp = v + a * t + 0.5f * j * t * t;  // f' = velocity at t
    if (fp > 1e-6f) {
      t -= f / fp;
      if (t < t_lo || t > t_hi) t = 0.5f * (t_lo + t_hi);
    }
    else {
      t = 0.5f * (t_lo + t_hi);
    }
  }
  return t_hi;
}

// Distance of a 3-phase ramp: [j1 for t1, hold for t2, j3 for t3].
// Horner-form polynomial per phase, no sequential state updates.
static FORCE_INLINE float rampDist3Phase(
  float v, float a,
  float j1, float t1,
  float t2,
  float j3, float t3
) {
  const float a1 = a + j1 * t1;
  const float v1 = v + (a + j1 * 0.5f * t1) * t1;
  const float v2 = v1 + a1 * t2;
  return (v + (a * 0.5f + (j1 / 6.0f) * t1) * t1) * t1
       + (v1 + a1 * 0.5f * t2) * t2
       + (v2 + (a1 * 0.5f + (j3 / 6.0f) * t3) * t3) * t3;
}

// Velocity after fully absorbing initial acceleration a (bringing a to 0).
// v + a*|a|/(2j) — the minimum v_peak for an accel ramp starting at (v, a).
static FORCE_INLINE float absorbedVelocity(float v, float a, float j) {
  return v + a * ABS(a) / (2.0f * j);
}

// Phase durations for a 3-phase accel ramp [+j to a_peak, hold at a_peak, -j to 0].
// Three regimes: triangular (a_peak² ≤ a_max²), a_start above a_max, trapezoidal.
// Takes a_peak_sq to defer sqrt — only computed in the triangular branch.
// Returns a_peak when computed (triangular), 0 otherwise.
static FORCE_INLINE float accelRampTimings(
  float a_peak_sq, float a_start,
  float a_max, float j_max, float dv,
  float &dt1, float &dt2, float &dt3
) {
  if (a_peak_sq <= a_max * a_max) {
    const float a_peak = SQRT(a_peak_sq);
    dt1 = (a_peak - a_start) / j_max;
    dt2 = 0.0f;
    dt3 = a_peak / j_max;
    return a_peak;
  }
  if (a_start > a_max) {
    dt1 = 0.0f;
    dt3 = a_start / j_max;
    dt2 = _MAX(0.0f, (dv - a_start * a_start / (2.0f * j_max)) / a_start);
  }
  else {
    dt1 = (a_max - a_start) / j_max;
    dt3 = a_max / j_max;
    dt2 = _MAX(0.0f, (dv - (2.0f * a_max * a_max - a_start * a_start) / (2.0f * j_max)) / a_max);
  }
  return 0.0f;
}

// Plan a 3-phase ramp between v_start and v_peak.
// Returns the distance consumed.
// a_start: initial acceleration (default 0). Only used for accel ramps (decel=false).
//   For accel: phases are [+j from a_start to a_peak, hold at a_peak, -j from a_peak to 0]
//   For decel: a_start is ignored (decel always starts from a=0 at v_peak)
static inline float planRamp(
  float v_start, float v_peak, float j_max, float a_max,
  bool decel, float &dt_jolt1, float &dt_hold, float &dt_jolt2,
  float a_start = 0.0f
) {
  float dv = v_peak - v_start;
  float a_peak_sq = j_max * dv + 0.5f * a_start * a_start;
  if (a_peak_sq < 0) {
    if (a_start < 0.0f) {
      a_peak_sq = 0;
    }
    else {
      dt_jolt1 = dt_hold = dt_jolt2 = 0;
      return 0;
    }
  }
  if (!decel) {
    accelRampTimings(a_peak_sq, a_start, a_max, j_max, dv, dt_jolt1, dt_hold, dt_jolt2);
  }
  else {
    if (a_peak_sq <= a_max * a_max) {
      const float a_peak = SQRT(a_peak_sq);
      dt_jolt1 = (a_peak - a_start) / j_max;
      dt_hold = 0;
      dt_jolt2 = a_peak / j_max;
    }
    else {
      dt_jolt1 = (a_max - a_start) / j_max;
      dt_jolt2 = a_max / j_max;
      float dv_no_hold = (2.0f * a_max * a_max - a_start * a_start) / (2.0f * j_max);
      dt_hold = _MAX(0.0f, (dv - dv_no_hold) / a_max);
    }
  }

  // Distance: closed-form matching rampDist / rampDistWithA exactly,
  // so planRamp and totalRampDistCF agree (no float precision gap).
  // The a_start != 0 path uses rampDist3Phase (phaseDist internally),
  // consistent with buildPhaseCache and rawDistanceAtTime.
  if (decel || a_start == 0.0f) {
    if (dv <= 0.0f) return 0.0f;
    if (j_max * dv <= a_max * a_max)
      return (v_start + v_peak) * dt_jolt2;  // dt_jolt2 = a_peak/j = sqrt(dv/j)
    else
      return (v_start + v_peak) * 0.5f * (a_max / j_max + dv / a_max);
  }
  else {
    return rampDist3Phase(v_start, a_start, j_max, dt_jolt1, dt_hold, -j_max, dt_jolt2);
  }
}

// Plan a 3-phase decel ramp from (v_start, a_start) to (v_end, 0).
// Phases: [-j, 0, +j]. a_start can be positive, zero, or negative.
// Returns distance consumed, or -1 if infeasible (|a_start| exceeds the
// triangular peak — the -j phase would push acceleration further from zero).
static inline float planDecelRampWithA(
  float v_start, float v_end, float j_max, float a_max,
  float &dt_jolt1, float &dt_hold, float &dt_jolt2,
  float a_start = 0.0f
) {
  const float dv = v_start - v_end;
  a_max = _MAX(a_max, ABS(a_start));
  float a_peak_sq = (a_start * a_start + 2.0f * j_max * dv) / 2.0f;
  if (a_peak_sq < 0.0f) a_peak_sq = 0.0f;
  float a_peak = SQRT(a_peak_sq);

  if (a_start < 0.0f && -a_start > a_peak * CJP_INFEASIBLE_REL + CJP_INFEASIBLE_ABS) {
    dt_jolt1 = dt_hold = dt_jolt2 = 0.0f;
    return -1.0f;
  }

  if (a_peak <= a_max) {
    dt_jolt1 = (a_start + a_peak) / j_max;
    dt_hold = 0.0f;
    dt_jolt2 = a_peak / j_max;
  }
  else {
    a_peak = a_max;
    dt_jolt1 = (a_start + a_max) / j_max;
    dt_jolt2 = a_max / j_max;
    float dv_no_hold = (a_start * a_start - 2.0f * a_max * a_max) / (2.0f * j_max);
    float dv_hold = -dv - dv_no_hold;
    dt_hold = _MAX(0.0f, -dv_hold / a_max);
  }

  return rampDist3Phase(v_start, a_start, -j_max, dt_jolt1, dt_hold, j_max, dt_jolt2);
}

// Accel ramp distance given a_peak² (avoids all sqrt in trapezoidal regime).
// Used by maxReachableSpeed bisection on a_peak² directly.
static inline float rampDistFromApeakSq(
  float v_start, float a_start,
  float a_peak_sq, float j_max, float a_max
) {
  const float dv = (a_peak_sq - 0.5f * a_start * a_start) / j_max;
  float dt1, dt2, dt3;
  accelRampTimings(a_peak_sq, a_start, a_max, j_max, dv, dt1, dt2, dt3);
  return rampDist3Phase(v_start, a_start, j_max, dt1, dt2, -j_max, dt3);
}

// Closed-form ramp distance (O(1), no simulation).
static FORCE_INLINE float rampDist(float v_start, float v_peak, float j_max, float a_max) {
  const float dv = v_peak - v_start;
  if (dv <= 0.0f) return 0.0f;
  if (j_max * dv <= a_max * a_max)
    return (v_start + v_peak) * SQRT(dv / j_max);
  else
    return (v_start + v_peak) * 0.5f * (a_max / j_max + dv / a_max);
}

// Closed-form ramp distance with initial acceleration.
// Delegates to planRamp (accel direction); discards phase durations.
static FORCE_INLINE float rampDistWithA(
  float v_start, float v_peak, float j_max, float a_max,
  float a_start
) {
  if (a_start == 0.0f) return rampDist(v_start, v_peak, j_max, a_max);
  float t1, t2, t3;
  return planRamp(v_start, v_peak, j_max, a_max, false, t1, t2, t3, a_start);
}

// Total accel+decel ramp distance using closed-form.
static inline float totalRampDistCF(float v_entry, float v_exit, float v_peak,
                                        float j_max, float a_max, float a_entry) {
  return rampDistWithA(v_entry, v_peak, j_max, a_max, a_entry)
       + rampDist(v_exit, v_peak, j_max, a_max);
}

// Combined distance + derivative for total ramp. Shares SQRT and branch
// decisions between the two computations. Used by the Newton loop in plan_accel_decel.
static inline float totalRampDistAndDeriv(
    float v_entry, float v_exit, float v_peak,
    float j_max, float a_max, float a_entry,
    float &deriv
) {
  float accel_dist, accel_deriv;

  // === Accel ramp ===
  {
    const float dv = v_peak - v_entry;
    if (a_entry == 0.0f) {
      if (dv <= 0.0f) {
        accel_dist = accel_deriv = 0.0f;
      }
      else if (j_max * dv <= a_max * a_max) {
        const float s = SQRT(dv / j_max);
        accel_dist = (v_entry + v_peak) * s;
        accel_deriv = (3.0f * v_peak - v_entry) * 0.5f / (j_max * s);
      }
      else {
        accel_dist = (v_entry + v_peak) * 0.5f * (a_max / j_max + dv / a_max);
        accel_deriv = 0.5f * (a_max / j_max + 2.0f * v_peak / a_max);
      }
    }
    else {
      float a_peak_sq = j_max * dv + 0.5f * a_entry * a_entry;
      bool feasible = true;
      if (a_peak_sq < 0.0f) {
        if (a_entry < 0.0f) a_peak_sq = 0.0f;
        else feasible = false;
      }
      if (!feasible) {
        accel_dist = accel_deriv = 0.0f;
      }
      else {
        float dt1, dt2, dt3;
        const float a_peak = accelRampTimings(a_peak_sq, a_entry, a_max, j_max, dv, dt1, dt2, dt3);
        accel_dist = rampDist3Phase(v_entry, a_entry, j_max, dt1, dt2, -j_max, dt3);

        // Derivative (reuses a_peak, branch, and phase durations)
        if (a_peak > 0.0f) {  // triangular (a_peak was computed)
          if (a_peak < 1e-10f) { accel_deriv = 0.0f; }
          else {
            const float da = j_max / (2.0f * a_peak);
            const float ddt = da / j_max;
            const float v1 = v_entry + a_entry * dt1 + 0.5f * j_max * dt1 * dt1;
            const float dd1 = v1 * ddt;
            const float dv1 = a_peak * ddt;
            accel_deriv = dd1 + dv1 * dt3 + v1 * ddt
                        + 0.5f * (da * dt3 * dt3 + a_peak * 2.0f * dt3 * ddt)
                        - (j_max / 2.0f) * dt3 * dt3 * ddt;
          }
        }
        else if (a_entry > a_max) {
          accel_deriv = (dt2 <= 0.0f) ? 0.0f : (v_entry + a_entry * dt2) / a_entry + dt3;
        }
        else {
          if (dt2 <= 0.0f) { accel_deriv = 0.0f; }
          else {
            const float v1 = v_entry + a_entry * dt1 + 0.5f * j_max * dt1 * dt1;
            accel_deriv = (v1 + a_max * dt2) / a_max + dt3;
          }
        }
      }
    }
  }

  // === Decel ramp (always a_entry=0) ===
  float decel_dist, decel_deriv;
  {
    const float dv = v_peak - v_exit;
    if (dv <= 0.0f) {
      decel_dist = decel_deriv = 0.0f;
    }
    else if (j_max * dv <= a_max * a_max) {
      const float s = SQRT(dv / j_max);
      decel_dist = (v_exit + v_peak) * s;
      decel_deriv = (3.0f * v_peak - v_exit) * 0.5f / (j_max * s);
    }
    else {
      decel_dist = (v_exit + v_peak) * 0.5f * (a_max / j_max + dv / a_max);
      decel_deriv = 0.5f * (a_max / j_max + 2.0f * v_peak / a_max);
    }
  }

  deriv = accel_deriv + decel_deriv;
  return accel_dist + decel_dist;
}

// Algebraic initial guess for v_peak assuming both ramps are trapezoidal.
static inline float vpeakGuess(float v_entry, float v_exit,
                                    float dist_total, float j_max, float a_max) {
  const float am2j = a_max * a_max / j_max;
  const float B = am2j;
  const float C = 0.5f * (v_entry * (am2j - v_entry) + v_exit * (am2j - v_exit)) - a_max * dist_total;
  const float disc = B * B - 4.0f * C;
  if (disc < 0.0f) return 0.5f * (v_entry + v_exit);
  return (-B + SQRT(disc)) * 0.5f;
}

} // namespace constant_jolt
