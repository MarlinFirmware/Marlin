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

#include "../../inc/MarlinConfig.h"

#if ENABLED(FTM_CONSTANT_JOLT)

#include "trajectory_constant_jolt.h"
#include "constant_jolt_math.h"

float ConstantJoltTrajectoryGenerator::jolt_default_ = FTM_DEFAULT_JOLT * 1000.0f;

void ConstantJoltTrajectoryGenerator::plan(
    const float, const float, const float, const float, const float) {
  planner_.planNext(*this, *jolt_ptr_);
}

void ConstantJoltTrajectoryGenerator::planRunout(const float duration) {
  reset();
  // Cruise at zero speed for the entire duration (same as trapezoidal)
  state.phase_dt[3] = duration;
  state.total_duration = duration;
  buildPhaseCache();
}

bool ConstantJoltTrajectoryGenerator::planNext(float j_max) {
  return planner_.planNext(*this, j_max);
}

// Common state setup for plan_accel_decel and plan_decel.
// Called at the start of each strategy, not by the caller.
void ConstantJoltTrajectoryGenerator::setupPlanState(
    float v_entry_in, float v_exit_in,
    float a_max_in, float j_max_in,
    float dist_total_in, float a_entry_in) {
  view_t0 = view_d0 = 0.0f;
  v_entry = v_entry_in;
  state.v_exit = v_exit_in;
  state.a_max = a_max_in;
  state.j_max = j_max_in;
  state.dist_total = dist_total_in;
  a_entry = a_entry_in;
  state.a_exit = 0.0f;
}


void ConstantJoltTrajectoryGenerator::truncateToDistance(float d) {
  const float d_abs = d + view_d0;
  if (d_abs >= state.dist_total) return;
  const float t_abs = getTimeAtDistance(d) + view_t0;
  const int ph = findPhase(t_abs);
  const float dt = t_abs - state.phase_start_time[ph];
  state.v_exit = state.phase_start_v[ph] + state.phase_start_a[ph] * dt + 0.5f * phaseJolt(ph) * dt * dt;
  state.a_exit = state.phase_start_a[ph] + phaseJolt(ph) * dt;
  state.dist_total = d_abs;
  state.total_duration = t_abs;
  // Ensure dist_total covers any float-precision overshoot at total_duration.
  // Prevents the ghost-phase check in getVelocityAtTime from clamping velocity
  // before total_duration.
  const float d_at_end = rawDistanceAtTime(t_abs);
  if (d_at_end > state.dist_total) state.dist_total = d_at_end;
}

float ConstantJoltTrajectoryGenerator::getVelocityAtDistance(const float d) const {
  if (d <= 0.0f) return v_entry;
  const float d_eff = state.dist_total - view_d0;
  if (d >= d_eff) return state.v_exit;
  const float d_abs = d + view_d0;
  const int ph = findPhaseByDist(d_abs);
  const float delta_s = d_abs - state.phase_start_pos[ph];
  const float v_ph = state.phase_start_v[ph];
  const float a_ph = state.phase_start_a[ph];
  const float jl = phaseJolt(ph);

  if (jl == 0.0f) {
    // Constant accel: delta_s = v0*t + 0.5*a*t²
    if (a_ph == 0.0f) return v_ph;  // cruise phase
    // t = (-v0 + sqrt(v0² + 2*a*delta_s)) / a
    const float disc = v_ph * v_ph + 2.0f * a_ph * delta_s;
    const float t = (-v_ph + SQRT(_MAX(0.0f, disc))) / a_ph;
    return v_ph + a_ph * t;
  }

  // Jolt phase: find t such that s(t) = delta_s, then return v(t).
  const float t = constant_jolt::solvePhaseTime(v_ph, a_ph, jl, delta_s, 0.0f, state.phase_dt[ph]);
  return v_ph + a_ph * t + 0.5f * jl * t * t;
}

float ConstantJoltTrajectoryGenerator::getTimeAtDistance(float d) const {
  if (d <= 0.0f) return 0.0f;
  const float d_eff = state.dist_total - view_d0;
  if (d >= d_eff) return state.total_duration - view_t0;
  // Work in absolute coordinates.
  const float d_abs = d + view_d0;
  const int ph = findPhaseByDist(d_abs);
  const float t0 = state.phase_start_time[ph];
  float lo = t0;
  float hi = (ph < 6) ? state.phase_start_time[ph + 1] : state.total_duration;
  float mid = lo;
  for (int i = 0; i < 32; i++) {
    mid = (lo + hi) * 0.5f;
    if (mid == lo || mid == hi) break;
    if (distanceInPhase(ph, mid - t0) < d_abs) lo = mid;
    else hi = mid;
  }
  return mid - view_t0;
}

void ConstantJoltTrajectoryGenerator::reset() {
  planner_.resetPlannerState();
  state = CJTrajectoryState{};
  view_t0 = view_d0 = v_entry = a_entry = 0.0f;
}

void ConstantJoltTrajectoryGenerator::savePreTruncation(
    CJTrajectorySnapshot& snap, uint8_t merge_count, float mm_consumed) const {
  if (merge_count <= 1) {
    snap.left_end = 0;  // single block or failure — no reuse possible
    return;
  }
  snap.traj = state;
  snap.left_end = merge_count - 1;
  snap.mm_consumed = mm_consumed;
}

bool ConstantJoltTrajectoryGenerator::finalizeAndTruncate(float dist_total_in) {
  if (state.dist_total > dist_total_in) {
    if (state.dist_total - dist_total_in > CJP_TOL)
      return false;
    // Save the target v_exit before truncation overwrites it.
    const float v_exit_target = state.v_exit;
    truncateToDistance(dist_total_in);
    // After truncation, verify the exit velocity still matches the target.
    // A tiny distance overshoot on a steep decel can produce large v error.
    // v_exit=0 (can't-brake) is exempt — self-corrects next cycle.
    if (v_exit_target > CJP_TOL && ABS(state.v_exit - v_exit_target) > CJP_TOL)
      return false;
  }
  return true;
}

void ConstantJoltTrajectoryGenerator::reuse(
    CJTrajectorySnapshot& snap, float mm0,
    float v_entry_in, float a_entry_in) {
  // Restore saved trajectory phases.
  state = snap.traj;
  // Set view offset to skip consumed distance.
  if (snap.mm_consumed > 0.0f) {
    view_d0 = snap.mm_consumed;
    view_t0 = snap.t_consumed;
    v_entry = v_entry_in;
    a_entry = a_entry_in;
  } else {
    view_t0 = view_d0 = 0.0f;
  }
  // Advance snapshot: consume one more block.
  float prev_t = snap.t_consumed;
  snap.left_end--;
  truncateToDistance(mm0);
  snap.mm_consumed += mm0;
  snap.t_consumed = getTotalDuration() + prev_t;
}

/**
 * ─── plan_accel_decel ─────────────────────────────────────────────────────────
 *
 * Shape: [ +j    hold  -j    crse  -j    hold  +j    ]
 *         ╰─ accel ramp ─╯        ╰─ decel ramp ─╯
 *
 * Accel ramp: accelerates from (v_entry, a_entry) up to v_peak with a=0.
 * Cruise:     holds v_peak (a=0). Duration fills leftover distance.
 * Decel ramp: decelerates from v_peak down to v_exit with a=0.
 * Any phase can have zero duration (7 phases degenerate to fewer).
 *
 * Solver: finds v_peak (single variable) via bracketed Newton.
 *
 * Limitation: the template assumes "accelerate then decelerate", so the accel
 * ramp always starts with +j (build acceleration upward). When a_entry > 0,
 * the +j ramp must ride out the existing acceleration -- building a up to a_peak
 * then back to 0 -- and velocity RISES during this (v_peak >= v_entry + a_entry^2/2j).
 * If this minimum ramp doesn't fit in the available distance, plan_accel_decel
 * fails and plan_decel takes over (it applies opposing jerk first to tame
 * a_entry before decelerating -- a different phase structure that this template
 * can't express).
 */
bool ConstantJoltTrajectoryGenerator::plan_accel_decel(
    float v_entry_in, float v_exit_in, float a_max_in, float j_max_in,
    float dist_total, float v_nominal, float a_entry_in) {
  setupPlanState(v_entry_in, v_exit_in, a_max_in, j_max_in, dist_total, a_entry_in);

  // Minimum v_peak: the accel ramp from (v_entry, a_entry) must complete a full
  // [+j, hold, -j] cycle back to a=0. absorbedVelocity gives the velocity after
  // this cycle — v_peak can't be lower (the ramp shape requires it).
  const float v_peak_min_for_a = constant_jolt::absorbedVelocity(v_entry, a_entry, state.j_max);
  const float v_hard_floor = (a_entry >= 0.0f) ? _MAX(v_entry, state.v_exit) : state.v_exit;
  const float v_peak_floor = _MAX(v_hard_floor, v_peak_min_for_a);
  const float dist_total_in = dist_total;

  // Even the tightest ramps (v_peak = floor, no cruise) overshoot? Give up.
  if (constant_jolt::totalRampDistCF(v_entry, state.v_exit, v_peak_floor, state.j_max, state.a_max, a_entry) > state.dist_total)
    return false;

  float v_peak = v_nominal;
  // Bracketed Newton using closed-form distance (fast). Derivative also closed-form.
  if (constant_jolt::totalRampDistCF(v_entry, state.v_exit, v_nominal, state.j_max, state.a_max, a_entry) > state.dist_total) {
    float v_lo = v_peak_floor, v_hi = v_nominal;

    // Initial guess: algebraic (trap+trap quadratic) for a_entry=0, midpoint otherwise
    if (a_entry == 0.0f) {
      float guess = constant_jolt::vpeakGuess(v_entry, state.v_exit, state.dist_total, state.j_max, state.a_max);
      v_peak = _MAX(v_lo, _MIN(guess, v_hi));
    } else {
      v_peak = 0.5f * (v_lo + v_hi);
    }

    for (int i = 0; i < 10; i++) {
      if (v_peak <= v_lo) v_peak = v_lo + CJP_CONVERGE_TOL;
      float fp;
      const float f = constant_jolt::totalRampDistAndDeriv(v_entry, state.v_exit, v_peak, state.j_max, state.a_max, a_entry, fp) - state.dist_total;
      if (f <= 0.0f) v_lo = v_peak; else v_hi = v_peak;
      if (f <= 0.0f && f > -CJP_TOL) break;
      if (v_hi - v_lo < CJP_CONVERGE_TOL) break;
      if (fp > 1e-10f) {
        v_peak -= f / fp;
        if (v_peak <= v_lo || v_peak >= v_hi) v_peak = 0.5f * (v_lo + v_hi);
      } else {
        v_peak = 0.5f * (v_lo + v_hi);
      }
    }
    v_peak = v_lo;
  }

  // constant_jolt::planRamp now uses the same closed-form as constant_jolt::totalRampDistCF for distance,
  // so dist_ramps <= state.dist_total is guaranteed (no precision gap to refine).
  float t1, t2, t3, t4 = 0, t5, t6, t7;
  float dist_accel = constant_jolt::planRamp(v_entry, v_peak, state.j_max, state.a_max, false, t1, t2, t3, a_entry);
  float dist_decel = constant_jolt::planRamp(state.v_exit, v_peak, state.j_max, state.a_max, true, t5, t6, t7);
  float dist_ramps = dist_accel + dist_decel;


  if (v_peak > 0.0f && state.dist_total > dist_ramps)
    t4 = (state.dist_total - dist_ramps) / v_peak;

  state.phase_dt[0] = t1; state.phase_dt[1] = t2; state.phase_dt[2] = t3;
  state.phase_dt[3] = t4;
  state.phase_dt[4] = t5; state.phase_dt[5] = t6; state.phase_dt[6] = t7;
  state.total_duration = t1 + t2 + t3 + t4 + t5 + t6 + t7;
  buildPhaseCache();
  return finalizeAndTruncate(dist_total_in);
}

/**
 * ─── plan_decel ──────────────────────────────────────────────────────────────
 *
 * Shape (a>0): [             -j          -j    hold  +j    ]
 *                           ╰ absorb ╯  ╰─ decel ramp ─╯
 *
 * Shape (a<0): [ +j                      -j    hold  +j    ]
 *               ╰ absorb ╯              ╰─ decel ramp ─╯
 *
 * When plan_accel_decel fails (a_entry too large for the +j accel ramp to fit),
 * this strategy applies OPPOSING jerk to tame a_entry before decelerating.
 * The absorption phase reduces |a| toward 0. The decel ramp then handles the
 * remaining state (v_mid, a_mid) via planDecelRampWithA (which supports a != 0).
 *
 * Solver: bisects on absorption time t0 to fill the available distance.
 * More absorption = velocity changes more = decel ramp distance changes.
 * The bisection finds the t0 where absorption + decel exactly fits.
 *
 * Note: absorption and decel start are BOTH the same jerk sign (-j for a_entry > 0)
 * in consecutive phases (2 and 4, with phase 3 = 0). They're mathematically one
 * continuous -j application, split only for code organization.
 */
bool ConstantJoltTrajectoryGenerator::plan_decel(
    float v_entry_in, float v_exit_in, float a_max_in, float j_max_in,
    float dist_total, float a_entry_in) {
  setupPlanState(v_entry_in, v_exit_in, a_max_in, j_max_in, dist_total, a_entry_in);
  const float dist_total_in = dist_total;
  const float j_absorb = (a_entry > 0.0f) ? -state.j_max : state.j_max;  // opposes a_entry
  const float t0_max = ABS(a_entry) / state.j_max;


  // Bisect t0 so d_absorption + d_decel = dist_total.
  // Save results when t0_lo is updated so we can reuse them after the loop.
  float t0_lo = 0.0f, t0_hi = t0_max;
  // Initialize with t0_lo=0 values (no absorption, immediate decel)
  float v_mid = v_entry, d_abs = 0;
  float dt_d1, dt_d2, dt_d3;
  float d_dec = constant_jolt::planDecelRampWithA(v_entry, state.v_exit, state.j_max, state.a_max, dt_d1, dt_d2, dt_d3, a_entry);

  // Check immediate decel (t0=0): if it already overshoots and absorption
  // only adds distance (a_entry > 0), fail immediately.
  if (d_dec > state.dist_total + CJP_TOL && a_entry > 0.0f)
    return false;
  for (int iter = 0; iter < 20; iter++) {
    float t0_mid = 0.5f * (t0_lo + t0_hi);
    float a_m = a_entry + j_absorb * t0_mid;
    float v_m = v_entry + a_entry * t0_mid + 0.5f * j_absorb * t0_mid * t0_mid;
    float d_a = constant_jolt::phaseDist(v_entry, a_entry, j_absorb, t0_mid);
    float d1, d2, d3;
    float d_d = constant_jolt::planDecelRampWithA(v_m, state.v_exit, state.j_max, state.a_max, d1, d2, d3, a_m);
    // d_d < 0: decel ramp infeasible (|a_m| exceeds triangular peak) → need more absorption
    if (d_d < 0.0f || d_a + d_d <= state.dist_total) {
      t0_lo = t0_mid;
      v_mid = v_m; d_abs = d_a; d_dec = d_d;
      dt_d1 = d1; dt_d2 = d2; dt_d3 = d3;
      if (d_d >= 0.0f && state.dist_total - (d_a + d_d) < CJP_TOL) break;
    } else {
      t0_hi = t0_mid;
    }
  }

  float t0 = t0_lo;
  if (d_dec < 0.0f) { d_dec = dt_d1 = dt_d2 = dt_d3 = 0.0f; }

  if (v_mid < state.v_exit - CJP_TOL && d_dec < CJP_TOL) return false;

  // Reject if the absorption + decel can't cover the target distance.
  // This happens when the trajectory's total distance is invariant to
  // absorption time (e.g., accel then decel back to 0 covers the same
  // distance regardless of the peak velocity reached during absorption).
  if (d_abs + d_dec < state.dist_total - CJP_TOL && d_dec >= 0.0f) return false;

  // Phase mapping: absorption goes in the phase whose jolt matches j_absorb
  if (a_entry > 0.0f) {
    // j_absorb = -j_max → matches phase 2 jolt
    state.phase_dt[0] = state.phase_dt[1] = 0.0f;
    state.phase_dt[2] = t0;
  } else {
    // j_absorb = +j_max → matches phase 0 jolt
    state.phase_dt[0] = t0;
    state.phase_dt[1] = state.phase_dt[2] = 0.0f;
  }
  state.phase_dt[3] = 0.0f;
  state.phase_dt[4] = dt_d1; state.phase_dt[5] = dt_d2; state.phase_dt[6] = dt_d3;

  state.total_duration = t0 + dt_d1 + dt_d2 + dt_d3;
  // _MAX because bisection uses conservative t0_lo, so d_covered may
  // be slightly less than dist_total_in.
  state.dist_total = _MAX(dist_total_in, d_abs + d_dec);
  buildPhaseCache();
  return finalizeAndTruncate(dist_total_in);
}

/**
 * ─── plan_decel_with_carry ────────────────────────────────────────────────────
 *
 * Shape: [                         -j          +j    ]
 *                               ╰ brake ╯ ╰ recover ╯
 *
 * Last resort when plan_accel_decel and plan_decel both fail.
 * Pure braking: -j builds negative acceleration, then +j partially recovers
 * toward a=0. Exits with (v <= safe_exit, a <= 0) -- the "carry" exit that
 * hands off mid-deceleration to the next block. The next cycle enters with
 * a < 0 and handles it via plan_accel_decel or plan_decel.
 *
 * Unlike the other strategies, this does NOT target a specific v_exit with a=0.
 * Instead, it finds the MINIMUM braking that keeps v_exit <= safe_exit -- the
 * gentlest decel that's still feasible for downstream blocks.
 *
 * Solver: bisects on t4 (-j duration). For each t4, an inner Newton solve
 * finds t6 (+j duration) to fill the remaining distance, capped so a <= 0.
 * More t4 = harder braking = lower v_exit.
 */
bool ConstantJoltTrajectoryGenerator::plan_decel_with_carry(
    float v_entry_in, float v_max_safe_exit,
    float a_max_in, float j_max_in,
    float dist_total, float a_entry_in) {
  const float j = j_max_in;
  const float dist = dist_total;
  const float v0 = v_entry_in;
  const float a0 = a_entry_in;

  // For a given t4 (-j duration), compute exit state after [-j, +j] covering
  // exactly dist, with +j capped so a ≤ 0. Returns:
  //   -1: need less -j (t4 too large — v_mid depleted or -j overshoots dist)
  //   +1: need more -j (v_exit > v_safe, or a_exit > 0, or can't fill dist)
  //    0: solution found (v_exit ≤ v_safe, a_exit ≤ 0, dist matched)
  auto eval = [&](float t4, float &t6_out, float &v_exit_out, float &a_exit_out) -> int {
    const float d4 = constant_jolt::phaseDist(v0, a0, -j, t4);
    if (d4 > dist) return -1;
    const float v_mid = v0 + a0 * t4 - 0.5f * j * t4 * t4;
    if (v_mid < 0.0f) return -1;
    const float a_mid = a0 - j * t4;
    const float d_rem = dist - d4;

    const float t6_max = (a_mid < 0.0f) ? -a_mid / j : 0.0f;
    if (constant_jolt::phaseDist(v_mid, a_mid, j, t6_max) < d_rem) return -1;

    t6_out = constant_jolt::solvePhaseTime(v_mid, a_mid, j, d_rem, 0.0f, t6_max);
    a_exit_out = a_mid + j * t6_out;
    v_exit_out = v_mid + a_mid * t6_out + 0.5f * j * t6_out * t6_out;

    if (a_exit_out > 0.0f) return +1;
    if (v_exit_out > v_max_safe_exit) return +1;
    return 0;
  };

  const float disc_v = a0 * a0 + 2.0f * j * v0;
  const float t4_max = (disc_v > 0.0f) ? (a0 + SQRT(disc_v)) / j : 0.0f;
  if (t4_max <= 0.0f) return false;

  const float t4_min = (a0 > 0.0f) ? a0 / j : 0.0f;
  float lo = t4_min, hi = t4_max;
  float t4_best = 0.0f, t6_best = 0.0f, v_best = 0.0f, a_best = 0.0f;
  bool found = false;
  for (int i = 0; i < 32; i++) {
    float mid = 0.5f * (lo + hi);
    if (mid == lo || mid == hi) break;
    float t6_f, v_f, a_f;
    const int dir = eval(mid, t6_f, v_f, a_f);
    if (dir == 0) {
      hi = mid;
      t4_best = mid; t6_best = t6_f; v_best = v_f; a_best = a_f;
      found = true;
    } else if (dir < 0) {
      hi = mid;
    } else {
      lo = mid;
    }
  }

  if (!found) return false;

  view_t0 = view_d0 = 0.0f;
  v_entry = v_entry_in;
  a_entry = a_entry_in;
  state.a_max = a_max_in;
  state.j_max = j_max_in;
  for (int i = 0; i < 7; i++) state.phase_dt[i] = 0.0f;
  state.phase_dt[4] = t4_best;
  state.phase_dt[6] = t6_best;
  state.total_duration = t4_best + t6_best;
  state.v_exit = v_best;
  state.a_exit = a_best;
  state.dist_total = dist;
  buildPhaseCache();

  if (state.v_exit > v_max_safe_exit) return false;
  if (state.a_exit > 0.0f) return false;
  if (state.v_exit - state.a_exit * state.a_exit / (2.0f * j) < 0.0f) return false;

  return true;
}

#endif // FTM_CONSTANT_JOLT
