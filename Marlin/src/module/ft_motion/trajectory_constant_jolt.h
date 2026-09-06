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


#include "constant_jolt_planner.h"
#include "trajectory_generator.h"
#include <math.h>

/**
 * Constant-jolt (7-phase S-curve) trajectory generator.
 *
 * Phases: [+jolt, cruise_accel, -jolt, cruise_velocity, -jolt, cruise_decel, +jolt]
 * Supports non-zero boundary accelerations (a_entry carried across blocks).
 * Uses bracketed Newton to find feasible peak velocity.
 */

namespace constant_jolt {

// Distance traveled during one phase with constant jolt.
// Uses j/6 (not (1/6)*j) for float consistency with the closed-form
// ramp distance functions (rampDist, rampDistWithA).
static FORCE_INLINE float phaseDist(float v, float a, float j, float dt) {
  return v * dt + 0.5f * a * dt * dt + (j / 6.0f) * dt * dt * dt;
}

// Advance one phase: accumulate distance, update velocity and acceleration.
static inline void simulatePhase(float j_phase, float dt, float &v, float &a, float &dist) {
  if (dt <= 0.0f) return;
  dist += phaseDist(v, a, j_phase, dt);
  v += a * dt + 0.5f * j_phase * dt * dt;
  a += j_phase * dt;
}

} // namespace constant_jolt

class ConstantJoltTrajectoryGenerator : public TrajectoryGenerator {
public:
  ConstantJoltTrajectoryGenerator() = default;

  // plan() delegates to the block planner. Defined in trajectory_constant_jolt.cpp.
  void plan(const float, const float, const float, const float, const float) override;

  // planRunout() resets planner sub-block state, then plans a zero-speed cruise.
  // Defined in trajectory_constant_jolt.cpp.
  void planRunout(const float duration) override;

  void setJoltPtr(float* ptr) { jolt_ptr_ = ptr; }

  // Convenience: delegates to planner_.planNext(*this, j_max).
  // Defined in trajectory_constant_jolt.cpp.
  bool planNext(float j_max);

  // Strategy 1: Standard 7-phase [+j, 0, -j, cruise, -j, 0, +j].
  bool plan_accel_decel(float v_entry_in, float v_exit_in, float a_max_in, float j_max_in,
                     float dist_total, float v_nominal, float a_entry_in = 0.0f);

  // Strategy 2: Partial absorption of a_entry, then decel ramp.
  bool plan_decel(float v_entry_in, float v_exit_in, float a_max_in, float j_max_in,
                               float dist_total, float a_entry_in);

  // Strategy 3: [-j, +j] decel, exits with a ≤ 0.
  bool plan_decel_with_carry(float v_entry_in, float v_max_safe_exit,
                                  float a_max_in, float j_max_in,
                                  float dist_total, float a_entry_in);

  // Truncate the trajectory to cover only the first `d` mm of distance.
  // After truncation, getTotalDuration() returns the time to reach `d`,
  // and v_exit/a_exit reflect the state at that point.
  void truncateToDistance(float d);

  FORCE_INLINE float getDistanceAtTime(const float t) const override {
    if (t <= 0.0f) return 0.0f;
    const float view_dur = state.total_duration - view_t0;
    if (t >= view_dur) return state.dist_total - view_d0;
    const float d_raw = rawDistanceAtTime(t + view_t0);
    return _MIN(d_raw, state.dist_total) - view_d0;
  }

  FORCE_INLINE float getTotalDuration() const override { return state.total_duration - view_t0; }

  FORCE_INLINE float getVelocityAtTime(const float t) const {
    if (t <= 0.0f) return v_entry;
    const float t_abs = t + view_t0;
    if (t_abs >= state.total_duration) return state.v_exit;
    const int ph = findPhase(t_abs);
    const float dt = t_abs - state.phase_start_time[ph];
    // Guard against ghost phases: if distance at this time already exceeds
    // dist_total (residual phase past truncation point), return exit velocity.
    if (distanceInPhase(ph, dt) >= state.dist_total) return state.v_exit;
    return state.phase_start_v[ph] + state.phase_start_a[ph] * dt + 0.5f * phaseJolt(ph) * dt * dt;
  }

  FORCE_INLINE float getAccelerationAtTime(const float t) const {
    if (t <= 0.0f) return a_entry;
    const float t_abs = t + view_t0;
    if (t_abs >= state.total_duration) return state.a_exit;
    const int ph = findPhase(t_abs);
    const float dt = t_abs - state.phase_start_time[ph];
    return state.phase_start_a[ph] + phaseJolt(ph) * dt;
  }

  FORCE_INLINE float getJoltAtTime(const float t) const {
    const float t_abs = t + view_t0;
    const float tc = (t <= 0.0f) ? view_t0 : (t_abs >= state.total_duration) ? _MAX(0.0f, state.total_duration - 1e-10f) : t_abs;
    return phaseJolt(findPhase(tc));
  }

  // Get velocity at a given distance along the trajectory.
  float getVelocityAtDistance(const float d) const;

  float getExitSpeed() const { return state.v_exit; }
  float getExitAccel() const { return state.a_exit; }

  // Find time at which the trajectory reaches distance d.
  float getTimeAtDistance(float d) const;

  void reset() override;

  // ─── Trajectory reuse: save/restore pre-truncation state ───

  void savePreTruncation(CJTrajectorySnapshot& snap, uint8_t merge_count, float mm_consumed) const;

  // Reuse a previously saved trajectory: restore phases, advance the view
  // window by one block (mm0), and update the snapshot for the next cycle.
  void reuse(CJTrajectorySnapshot& snap, float mm0,
             float v_entry_in, float a_entry_in);

  // Delegate accessors to the planner
  ConstantJoltBlockPlanner& planner() { return planner_; }


  friend class ConstantJoltBlockPlanner;  // verifyJunctions accesses phase internals

private:
  // Set member state from params (called at start of each strategy).
  void setupPlanState(float v_entry_in, float v_exit_in,
                      float a_max_in, float j_max_in,
                      float dist_total_in, float a_entry_in);

  // Check overshoot tolerance and truncate if within CJP_TOL.
  bool finalizeAndTruncate(float dist_total_in);

  // Raw distance-at-time without view offsets (used internally and by getTimeAtDistance)
  FORCE_INLINE float rawDistanceAtTime(const float t) const {
    if (t <= 0.0f) return 0.0f;
    if (t >= state.total_duration) return state.dist_total;
    const int ph = findPhase(t);
    const float dt = t - state.phase_start_time[ph];
    const float v = state.phase_start_v[ph];
    const float a = state.phase_start_a[ph];
    const float jl = phaseJolt(ph);
    return state.phase_start_pos[ph] + constant_jolt::phaseDist(v, a, jl, dt);
  }

  FORCE_INLINE void buildPhaseCache() {
    float v = v_entry, a = a_entry, dist = 0.0f, t = 0.0f;
    for (int i = 0; i < 7; ++i) {
      state.phase_start_time[i] = t;
      state.phase_start_pos[i] = dist;
      state.phase_start_v[i] = v;
      state.phase_start_a[i] = a;
      constant_jolt::simulatePhase(phaseJolt(i), state.phase_dt[i], v, a, dist);
      t += state.phase_dt[i];
    }
  }

  FORCE_INLINE int findPhase(float t) const {
    for (int i = 0; i < 7; ++i)
      if (t < state.phase_start_time[i] + state.phase_dt[i]) return i;
    return 6;
  }

  // Absolute distance at time offset dt within a known phase.
  FORCE_INLINE float distanceInPhase(int ph, float dt) const {
    return state.phase_start_pos[ph]
         + constant_jolt::phaseDist(state.phase_start_v[ph], state.phase_start_a[ph], phaseJolt(ph), dt);
  }

  FORCE_INLINE int findPhaseByDist(float d) const {
    for (int i = 0; i < 6; ++i)
      if (d < state.phase_start_pos[i + 1]) return i;
    return 6;
  }

  FORCE_INLINE float phaseJolt(int phase) const {
    switch (phase) {
      case 0: case 6: return state.j_max;
      case 2: case 4: return -state.j_max;
      default: return 0.0f;
    }
  }

  CJTrajectoryState state;              // Trajectory state (saved/restored as a unit)
  ConstantJoltBlockPlanner planner_;   // Owned by value
  static float jolt_default_;     // Safe fallback if setJoltPtr not called yet
  float* jolt_ptr_ = &jolt_default_;

  float view_t0 = 0;     // time offset: view origin in the original trajectory
  float view_d0 = 0;     // distance offset: view origin in the original trajectory

  float v_entry = 0;     // entry velocity (set by strategy or reuse)
  float a_entry = 0;     // entry acceleration (non-zero when carried from previous block)

};
