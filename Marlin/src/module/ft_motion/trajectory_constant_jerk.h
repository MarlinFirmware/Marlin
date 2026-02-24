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

#include "trajectory_generator.h"
#include <math.h>

/**
 * Constant-jerk (7-phase S-curve) trajectory generator.
 *
 * Phases: [+jerk, cruise_accel, -jerk, cruise_velocity, -jerk, cruise_decel, +jerk]
 * All junctions have a=0 (no non-zero boundary accelerations).
 * Uses binary search to find feasible peak velocity.
 */

// Simulate one phase of motion with constant jerk
static inline void cj_simulatePhase(float jerk, float dt, float &v, float &a, float &s) {
  if (dt <= 0.0f) return;
  s += v * dt + 0.5f * a * dt * dt + (1.0f / 6.0f) * jerk * dt * dt * dt;
  v += a * dt + 0.5f * jerk * dt * dt;
  a += jerk * dt;
}

// Plan a 3-phase ramp between v_start and v_peak.
// Returns the distance consumed.
static inline float cj_planRamp(float v_start, float v_peak, float j, float a_max,
                                bool decel, float &pa, float &pb, float &pc) {
  float dv = v_peak - v_start;
  float a_peak_sq = j * dv;
  if (a_peak_sq < 0) {
    pa = pb = pc = 0;
    return 0;
  }
  float a_peak = SQRT(a_peak_sq);

  if (a_peak <= a_max) {
    pa = a_peak / j;
    pb = 0;
    pc = a_peak / j;
  }
  else {
    pa = a_max / j;
    pc = a_max / j;
    float dv_no_hold = (a_max * a_max) / j;
    pb = _MAX(0.0f, (dv - dv_no_hold) / a_max);
  }

  float jk = decel ? -j : j;
  float v = decel ? v_peak : v_start;
  float a_v = 0, s = 0;
  cj_simulatePhase(jk, pa, v, a_v, s);
  cj_simulatePhase(0, pb, v, a_v, s);
  cj_simulatePhase(-jk, pc, v, a_v, s);
  return s;
}

// Symmetric total ramp distance
static inline float cj_totalRampDist(float vp, float v_small, float v_large,
                                     float j, float a_max) {
  float a, b, c;
  float s1 = cj_planRamp(v_small, vp, j, a_max, false, a, b, c);
  float s2 = cj_planRamp(v_large, vp, j, a_max, true, a, b, c);
  return s1 + s2;
}

class ConstantJerkTrajectoryGenerator : public TrajectoryGenerator {
public:
  ConstantJerkTrajectoryGenerator() = default;

  // plan() override is unused — the CJ block planner calls plan_full() directly.
  // Kept to satisfy the pure virtual interface.
  void plan(const float, const float, const float, const float, const float) override {}

  // Plan with explicit jerk and a_max (used by the block merging planner).
  // Jerk comes from cfg.jerk_max, passed through by the caller.
  void plan_full(float initial_speed_in, float final_speed_in,
                 float accel_max_in, float jerk_in,
                 float distance_in, float v_nominal_in) {
    reset();

    v0 = initial_speed_in;
    v1 = final_speed_in;
    a_max = accel_max_in;
    j = jerk_in;
    distance = distance_in;

    const float v_nominal = v_nominal_in;
    const float v_small = _MIN(v0, v1);
    const float v_large = _MAX(v0, v1);

    float v_lo = v_large;
    float v_peak = _MAX(v_large, v_nominal);
    float s_ramps = cj_totalRampDist(v_peak, v_small, v_large, j, a_max);

    if (s_ramps > distance) {
      float v_hi = v_peak;
      if (cj_totalRampDist(v_lo, v_small, v_large, j, a_max) > distance)
        return; // Infeasible
      for (int i = 0; i < 48; i++) {
        float mid = 0.5f * (v_lo + v_hi);
        float s_mid = cj_totalRampDist(mid, v_small, v_large, j, a_max);
        if (s_mid > distance)
          v_hi = mid;
        else
          v_lo = mid;
        if (distance - s_mid >= 0 && distance - s_mid < 0.1f) break;
      }
      v_peak = v_lo;
    }

    float t1, t2, t3, t4 = 0, t5, t6, t7;
    float s_accel = cj_planRamp(v0, v_peak, j, a_max, false, t1, t2, t3);
    float s_decel = cj_planRamp(v1, v_peak, j, a_max, true, t5, t6, t7);

    s_ramps = s_accel + s_decel;
    if (v_peak > 0.0f && distance > s_ramps)
      t4 = (distance - s_ramps) / v_peak;

    phase_dt[0] = t1; phase_dt[1] = t2; phase_dt[2] = t3;
    phase_dt[3] = t4;
    phase_dt[4] = t5; phase_dt[5] = t6; phase_dt[6] = t7;

    total_duration = t1 + t2 + t3 + t4 + t5 + t6 + t7;
    buildPhaseCache();
  }

  void planRunout(const float duration) override {
    reset();
    // Cruise at zero speed for the entire duration (same as trapezoidal)
    phase_dt[3] = duration;
    total_duration = duration;
    buildPhaseCache();
  }

  float getDistanceAtTime(const float t) const override {
    if (t <= 0.0f) return 0.0f;
    if (t >= total_duration) return distance;
    const int ph = findPhase(t);
    const float dt = t - phase_start_time[ph];
    const float v = phase_start_v[ph];
    const float a = phase_start_a[ph];
    const float jk = phaseJerk(ph);
    return phase_start_pos[ph] + v * dt + 0.5f * a * dt * dt + (1.0f / 6.0f) * jk * dt * dt * dt;
  }

  float getTotalDuration() const override { return total_duration; }

  float getVelocityAtTime(const float t) const {
    if (t <= 0.0f) return v0;
    if (t >= total_duration) return v1;
    const int ph = findPhase(t);
    const float dt = t - phase_start_time[ph];
    return phase_start_v[ph] + phase_start_a[ph] * dt + 0.5f * phaseJerk(ph) * dt * dt;
  }

  float getAccelerationAtTime(const float t) const {
    if (t <= 0.0f || t >= total_duration) return 0.0f;
    const int ph = findPhase(t);
    const float dt = t - phase_start_time[ph];
    return phase_start_a[ph] + phaseJerk(ph) * dt;
  }

  float getJerkAtTime(const float t) const {
    if (t <= 0.0f || t >= total_duration) return 0.0f;
    const int ph = findPhase(t);
    return phaseJerk(ph);
  }

  void reset() override {
    v0 = v1 = 0.0f;
    a_max = j = distance = 0.0f;
    for (int i = 0; i < 7; ++i) {
      phase_dt[i] = 0.0f;
      phase_start_time[i] = 0.0f;
      phase_start_pos[i] = 0.0f;
      phase_start_v[i] = 0.0f;
      phase_start_a[i] = 0.0f;
    }
    total_duration = 0.0f;
  }

private:
  void buildPhaseCache() {
    float v = v0, a = 0.0f, s = 0.0f, t = 0.0f;
    for (int i = 0; i < 7; ++i) {
      phase_start_time[i] = t;
      phase_start_pos[i] = s;
      phase_start_v[i] = v;
      phase_start_a[i] = a;
      cj_simulatePhase(phaseJerk(i), phase_dt[i], v, a, s);
      t += phase_dt[i];
    }
  }

  int findPhase(float t) const {
    for (int i = 0; i < 7; ++i)
      if (t < phase_start_time[i] + phase_dt[i]) return i;
    return 6;
  }

  float phaseJerk(int phase) const {
    switch (phase) {
      case 0: return j;
      case 2: return -j;
      case 4: return -j;
      case 6: return j;
      default: return 0.0f;
    }
  }

  float v0 = 0, v1 = 0;
  float a_max = 0, j = 0, distance = 0;
  float total_duration = 0;
  float phase_dt[7] = {};
  float phase_start_time[7] = {};
  float phase_start_pos[7] = {};
  float phase_start_v[7] = {};
  float phase_start_a[7] = {};
};
