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

bool ConstantJoltBlockPlanner::planNext(ConstantJoltTrajectoryGenerator& traj, float j_max) {
  float mm[BLOCK_BUFFER_SIZE];
  float nominal[BLOCK_BUFFER_SIZE];
  float accel[BLOCK_BUFFER_SIZE];
  float vmax_junction[BLOCK_BUFFER_SIZE + 1];  // max entry speed ceiling per block; [block_count]=0 (must stop)

  uint8_t block_count = 0;

  // Look ahead at future blocks.
  for (uint8_t i = 0; i < BLOCK_BUFFER_SIZE; i++) {
    block_t* blk = planner.get_future_block(i);
    if (!blk) break;
    if (blk->is_sync()) continue; // skip sync blocks in lookahead

    mm[block_count] = blk->millimeters;
    nominal[block_count] = blk->nominal_speed;
    accel[block_count] = blk->acceleration;
    vmax_junction[block_count] = blk->vmax_junction;
    block_count++;
  }
  if (block_count == 0) {
    traj.reset();
    return false;
  }
  vmax_junction[block_count] = 0.0f;  // must stop after last block

  // Carry both velocity and acceleration from previous block's exit.
  // The previous exit state already happened — never cap or reset it.
  float v_left_entry = traj.getExitSpeed();
  float a_left_entry = traj.getExitAccel();


  // Find max left-compatible extent: nominal spread ≤ 1%, accel ratio ≤ 1.1.
  // Nominals may differ slightly on curves (speed_factor caps per-axis rates).
  // The merged trajectory cruises at up to nom_max (per candidate). All blocks
  // in the group are within 1% of each other, so reuse is safe.
  uint8_t max_left_compatible = 1;
  {
    float a_min = accel[0], a_max_group = accel[0];
    float nom_lo = nominal[0], nom_hi = nominal[0];
    for (uint8_t i = 1; i < block_count; i++) {
      float new_lo = _MIN(nom_lo, nominal[i]);
      float new_hi = _MAX(nom_hi, nominal[i]);
      if (new_hi > new_lo * CJP_MERGE_NOM_RATIO) break;
      float new_a_min = _MIN(a_min, accel[i]);
      float new_a_max = _MAX(a_max_group, accel[i]);
      if (new_a_max > new_a_min * CJP_MERGE_AMAX_RATIO) break;
      nom_lo = new_lo;
      nom_hi = new_hi;
      a_min = new_a_min;
      a_max_group = new_a_max;
      max_left_compatible++;
    }
  }

  /**
   * ─── Backward pass with integrated reuse check ───
   *
   * Compute max feasible entry speed for [i..N) assuming per-block (unmerged) planning.
   * NOT valid for merged trajectories — merging has more distance and can safely exceed these.
   *
   * ─── Why trajectory reuse is essential (not just an optimization) ───
   *
   * When a merged trajectory is truncated at block 0, the exit state (v, a) is carried
   * to the next cycle. Ideally, replanning from that state would reconstruct the same
   * trajectory. In practice, plan_full's Newton solver converges to a slightly different
   * v_peak due to float rounding along a different convergence path (different initial
   * guess, different bracket tightening sequence). This epsilon difference in v_peak
   * produces a different trajectory shape. After truncation, the next block's entry state
   * diverges further. Over several cycles, the accumulated divergence can cause merged
   * candidates to fail (overshoot by > CJP_TOL), forcing a v=0 fallback with
   * harsh deceleration that prevents the last block from stopping.
   *
   * Reuse sidesteps this entirely: restore the saved pre-truncation trajectory and
   * advance within it. No Newton, no convergence differences, exact continuation.
   *
   * When a previous trajectory is available for reuse, we check at the reuse junction
   * whether the backward pass value has improved. If not, we reuse the previous
   * trajectory — skipping the backward pass, tryLeftEnd, and Newton entirely.
   * Fast path: all current blocks fit within the previous merge — reuse directly.
   * The junction is the terminal condition (v=0), which can't improve.
   */
  if (prev_traj.left_end == block_count) {
    traj.reuse(prev_traj, mm[0], v_left_entry, a_left_entry);
    shiftSafeTails(block_count);
    return true;
  }

  // Precompute cumulative distance, accel-min, and nominal-max for candidates.
  float cum_dist[BLOCK_BUFFER_SIZE + 1];
  float cum_accel_min[BLOCK_BUFFER_SIZE];
  float cum_nom_max[BLOCK_BUFFER_SIZE];
  cum_dist[0] = 0;
  cum_accel_min[0] = accel[0];
  cum_nom_max[0] = nominal[0];
  for (uint8_t i = 0; i < max_left_compatible; i++) {
    cum_dist[i + 1] = cum_dist[i] + mm[i];
    if (i > 0) {
      cum_accel_min[i] = _MIN(cum_accel_min[i - 1], accel[i]);
      cum_nom_max[i] = _MAX(cum_nom_max[i - 1], nominal[i]);
    }
  }

  /**
   * ─── Lazy backward pass ───
   *
   * Chained backward pass computed on demand as candidates are tried.
   * Positions behind the winning candidate are never computed — no trajectory
   * depends on them, so the _MAX clamp on stale shifted values is sufficient.
   */
  shiftSafeTails(block_count);

  float safe_exit = 0;  // terminal; then running backward pass accumulator
  uint8_t i;
  for (i = block_count; i > 0; i--) {
    if (i < block_count) {
      float new_entry = maxReachableSpeed(safe_exit, mm[i],
                                             vmax_junction[i], accel[i], j_max);
      safe_exit = prev_safe_tails[i] = _MAX(prev_safe_tails[i], new_entry);
    }
    if (i > max_left_compatible) continue;

    // Reuse check (before multi-block — multi-block shouldn't prevent reuse)
    if (i == prev_traj.left_end) {
      if (safe_exit <= prev_traj.v_exit_junction * CJP_REUSE_MARGIN + CJP_TOL) {
        traj.reuse(prev_traj, mm[0], v_left_entry, a_left_entry);
        return true;
      }
    }

    if (multi_block_tail && safe_exit < vmax_junction[i] - CJP_TOL) {
      float multi = multiBlockSafeSpeed(i, block_count, mm, nominal, vmax_junction, accel, j_max);
      if (multi > safe_exit) safe_exit = multi;
    }

    float v_target = maxReachableSpeed(v_left_entry, cum_dist[i], safe_exit,
                                        cum_accel_min[i - 1], j_max, a_left_entry);

    // Step 1: Standard 7-phase S-curve.
    bool ok = traj.plan_accel_decel(v_left_entry, v_target, cum_accel_min[i - 1], j_max,
                                  cum_dist[i], cum_nom_max[i - 1], a_left_entry);
    if (ok) {
      ok = verifyJunctions(traj, i, mm, vmax_junction);
      if (!ok) continue;
      break;
    }

    // Step 2: Partial absorption + decel (a_entry != 0).
    if (a_left_entry != 0.0f) {
      ok = traj.plan_decel(v_left_entry, v_target, cum_accel_min[i - 1], j_max,
                                         cum_dist[i], a_left_entry);
      if (ok) ok = verifyJunctions(traj, i, mm, vmax_junction);
      if (ok) break;
    }

    // Step 3: [-j, +j] decel targeting safe exit speed.
    ok = traj.plan_decel_with_carry(v_left_entry, safe_exit, cum_accel_min[i - 1], j_max,
                                          cum_dist[i], a_left_entry);
    if (ok) ok = verifyJunctions(traj, i, mm, vmax_junction);
    if (ok) break;

  }
  uint8_t best_left_end = i;  // 0 if no candidate succeeded

  // Fallbacks when all candidates failed.
  if (best_left_end == 0) {
    // Try reuse: previous merge's remaining trajectory is still valid
    // (junctions never worsen — safe_tail >= prev exit speed).
    if (prev_traj.left_end > 0) {
      traj.reuse(prev_traj, mm[0], v_left_entry, a_left_entry);
      return true;
    }

    // Try decel to full stop.
    bool stop_ok = traj.plan_accel_decel(v_left_entry, 0.0f, accel[0], j_max,
                                       cum_dist[1], cum_nom_max[0], a_left_entry);
    if (!stop_ok && a_left_entry != 0.0f)
      stop_ok = traj.plan_decel(v_left_entry, 0.0f, accel[0], j_max,
                                              cum_dist[1], a_left_entry);
    if (stop_ok) {
      best_left_end = 1;
    }
  }

  prev_traj.v_exit_junction = safe_exit;
  prev_safe_tails[best_left_end] = _MAX(prev_safe_tails[best_left_end], safe_exit);

  // Emit only 1 block: truncate trajectory to block 0's distance.
  // truncateToDistance updates v_exit and a_exit on the trajectory.
  // For normal single-block plans, dist_total == mm[0] so this is a no-op.
  // For multi-block superblocks or can't-brake decel, it cuts to block 0.

  // Save pre-truncation trajectory for potential reuse in next planNext call.
  traj.savePreTruncation(prev_traj, best_left_end, mm[0]);

  traj.truncateToDistance(mm[0]);
  prev_traj.t_consumed = traj.getTotalDuration();  // view_t0=0, so this is t_abs

  return true;
}

/**
 * ─── verifyJunctions ─────────────────────────────────────────────────────────
 * Check that the planned trajectory respects all interior junction speed limits
 * and the exit junction. Returns true if all junctions are within tolerance.
 */
bool ConstantJoltBlockPlanner::verifyJunctions(
  const ConstantJoltTrajectoryGenerator& traj,
  uint8_t merge_count, const float mm[],
  const float vmax_junction[]
) {
  float dist_cum = 0;
  for (uint8_t k = 0; k < merge_count; k++) {
    dist_cum += mm[k];
    const float limit = vmax_junction[k + 1] + CJP_TOL;

    /**
     * Fast check: find which phase this junction falls in and compute the
     * min/max velocity in that phase without solving for exact time.
     * v(t) = v0 + a0*t + 0.5*j*t^2 is quadratic in jolt phases.
     *   +j phases: parabola opens up   → max at endpoints, min at vertex
     *   -j phases: parabola opens down → min at endpoints, max at vertex
     *    0 phases: linear              → min/max at endpoints
     */
    const int ph = traj.findPhaseByDist(dist_cum);
    const float v0 = traj.state.phase_start_v[ph];
    const float v1 = (ph < 6) ? traj.state.phase_start_v[ph + 1] : traj.state.v_exit;
    const float jl = traj.phaseJolt(ph);
    const float a0 = traj.state.phase_start_a[ph];
    const float dt = traj.state.phase_dt[ph];

    float v_max_phase = _MAX(v0, v1);
    float v_min_phase = _MIN(v0, v1);

    // Vertex of the quadratic: t_v = -a0/jl (only meaningful if jl != 0)
    if (jl != 0.0f) {
      const float t_v = -a0 / jl;
      if (t_v > 0.0f && t_v < dt) {
        const float v_v = v0 + a0 * t_v + 0.5f * jl * t_v * t_v;
        if (jl < 0.0f) v_max_phase = _MAX(v_max_phase, v_v);  // opens down: vertex is max
        else            v_min_phase = _MIN(v_min_phase, v_v);  // opens up: vertex is min
      }
    }

    if (v_max_phase <= limit) continue;    // entire phase below limit → safe
    if (v_min_phase > limit) return false; // entire phase above limit → reject

    // Peak exceeds limit — need exact velocity at this distance
    const float v_at_junction = traj.getVelocityAtDistance(dist_cum);
    if (v_at_junction > limit) return false;
  }
  return true;
}

/**
 * ─── multiBlockSafeSpeed ─────────────────────────────────────────────────────
 * Max exit speed at boundary i considering continuous decel across mergeable
 * tail blocks (no a=0 at interior boundaries). Returns 0 if no improvement.
 */
float ConstantJoltBlockPlanner::multiBlockSafeSpeed(
  uint8_t i, uint8_t block_count,
  const float* mm, const float* nominal, const float* vmax_junction,
  const float* accel, float j_max
) {
  uint8_t merge_ext = 1;
  float nom_lo = nominal[i], nom_hi = nominal[i];
  float a_lo = accel[i], a_hi = accel[i];
  for (uint8_t k = i + 1; k < block_count; k++) {
    float nlo = _MIN(nom_lo, nominal[k]), nhi = _MAX(nom_hi, nominal[k]);
    if (nhi > nlo * CJP_MERGE_NOM_RATIO) break;
    float alo = _MIN(a_lo, accel[k]), ahi = _MAX(a_hi, accel[k]);
    if (ahi > alo * CJP_MERGE_AMAX_RATIO) break;
    nom_lo = nlo; nom_hi = nhi; a_lo = alo; a_hi = ahi;
    merge_ext++;
  }
  if (merge_ext < 2) return 0;

  float v_exit_merge = 0;
  if (i + merge_ext < block_count) v_exit_merge = prev_safe_tails[i + merge_ext];
  float group_dist = 0, a_min = 1e30f;
  for (uint8_t k = i; k < i + merge_ext; k++) {
    group_dist += mm[k];
    a_min = _MIN(a_min, accel[k]);
  }

  // Terminal: max V from which we can decel to a stop within group_dist.
  // Always valid as a feasibility proof (forward pass can decel to 0 if needed).
  // When v_exit_merge is above the non-monotone crossover (~62% of V),
  // MRS from v_exit_merge gives a higher result (monotone region), so use that.
  float safe_v = maxReachableSpeed(0, group_dist, vmax_junction[i], a_min, j_max);
  if (v_exit_merge > safe_v * 0.62f)
    safe_v = maxReachableSpeed(v_exit_merge, group_dist, vmax_junction[i], a_min, j_max);

  // Each interior junction: max V that can decel to vj[k+1] over distance d_k.
  // Uses independent constraints (no chaining), avoids non-monotone MRS.
  // Stop once the ramp reaches v=0 before the end — further junctions are at v=0.
  float cum_d = 0;
  a_min = 1e30f;
  for (uint8_t k = i; k < i + merge_ext - 1; k++) {
    cum_d += mm[k];
    a_min = _MIN(a_min, accel[k]);
    if (constant_jolt::rampDist(0, safe_v, j_max, a_min) <= cum_d) break;
    float constraint = maxReachableSpeed(vmax_junction[k + 1], cum_d, safe_v, a_min, j_max);
    if (constraint < safe_v) safe_v = constraint;
  }
  return safe_v;
}

/**
 * ─── maxReachableSpeed ──────────────────────────────────────────────────────
 * Find max V such that an S-curve ramp from v_from to V fits within dist_total.
 *
 * Forward direction: distance from velocity is simple (closed-form):
 *   Triangular (j·dv ≤ a²):  d = (v_from + V) · √(dv / j)
 *   Trapezoidal (j·dv > a²): d = (v_from + V) · (a/j + dv/a) / 2
 *
 * Inverse (velocity from distance) is harder — no closed-form inversion:
 *   Triangular: d = (v+V)·√((V-v)/j) → cubic in √(V-v), solved via Cardano
 *   Trapezoidal: d = (v+V)·(a/j + (V-v)/a)/2 → quadratic in V, one √
 *
 * Note: d(v_from, V) is non-monotone in v_from for fixed V (peak at v_from = V/3
 * in the triangular regime). Starting faster covers more ground during the jerk
 * ramp (a builds from 0 at higher base speed). This is real physics, not a bug.
 * The cross-cycle _MAX clamp on prev_safe_tails prevents regression from this.
 */
float ConstantJoltBlockPlanner::maxReachableSpeed(float v_from, float dist_total,
                                                   float v_max, float a_max, float j_max,
                                                   float a_entry) {
  // Trapezoidal upper bound: v²=v₀²+2ad always overestimates reachable
  // speed (no jolt phases), so v_trap >= the true S-curve answer.
  // Skip sqrtf when the bound exceeds v_max (common for non-tiny blocks).
  const float v_trap_sq = v_from * v_from + 2.0f * a_max * dist_total;
  float hi = (v_trap_sq >= v_max * v_max) ? v_max : SQRT(v_trap_sq);

  // For a_entry=0: V ≤ v_from is instantly reachable (no ramp needed).
  // For a_entry≠0: absorption takes distance, so we must check even if V ≤ v_from.
  if (a_entry == 0.0f && hi <= v_from) return hi;

  if (constant_jolt::rampDistWithA(v_from, hi, j_max, a_max, a_entry) <= dist_total) return hi;

  float v_lo;
  if (a_entry == 0.0f) {
    /**
     * Closed-form solutions for a_entry=0:
     * Trapezoidal regime (j*dv > amax²): quadratic in dv, one sqrtf.
     * Triangular regime (j*dv ≤ amax²): depressed cubic via Cardano, one cbrtf.
     * Trapezoidal regime: quadratic in dv, one sqrtf. Exact (up to float precision).
     * Triangular regime would need Cardano (cbrtf, no MCU hardware) — falls through
     * to bisection instead.
     */
    const float am2j = a_max * a_max / j_max;
    const float B = 2.0f * v_from + am2j;
    const float C = 2.0f * v_from * am2j - 2.0f * dist_total * a_max;
    const float disc = B * B - 4.0f * C;
    if (disc >= 0.0f) {
      const float dv = (-B + SQRT(disc)) * 0.5f;
      if (dv > 0.0f && j_max * dv > a_max * a_max) {
        float V = _MIN(v_from + dv, hi);
        if (constant_jolt::rampDist(v_from, V, j_max, a_max) > dist_total) V -= CJP_CONVERGE_TOL;
        return _MAX(V, v_from);
      }
    }

    // Triangular regime (a_entry=0): bisect on tau = sqrt(dv/j) instead of V.
    // Distance becomes a polynomial: d(tau) = (2*v_from + j*tau^2) * tau
    // No sqrtf per iteration — just multiplies and adds.
    {
      const float two_v = 2.0f * v_from;
      float tau_lo = 0.0f;
      float tau_hi = SQRT((hi - v_from) / j_max);
      for (int i = 0; i < 32; i++) {
        const float tau = 0.5f * (tau_lo + tau_hi);
        const float f = (two_v + j_max * tau * tau) * tau - dist_total;
        if (f <= 0.0f) tau_lo = tau; else tau_hi = tau;
        if (f <= 0.0f && f > -CJP_CONVERGE_TOL) break;
        if (tau_hi - tau_lo < 1e-7f) break;
      }
      float V = v_from + j_max * tau_lo * tau_lo;
      return _MIN(V, v_max);
    }
  }
  else {
    // Bisect on a_peak² directly (a_entry ≠ 0). No sqrt at all in setup/recovery.
    // a_peak² = j·(V − v_from) + ½·a_entry², so V = v_from + (a_peak² − ½a²) / j.
    // accelRampTimings defers sqrt to the triangular branch only.
    const float half_a_entry_sq = 0.5f * a_entry * a_entry;
    float ap_sq_lo = (a_entry > 0.0f) ? a_entry * a_entry : 0.0f;
    float ap_sq_hi = _MAX(0.0f, j_max * (hi - v_from) + half_a_entry_sq);

    for (int i = 0; i < 32; i++) {
      const float ap_sq = 0.5f * (ap_sq_lo + ap_sq_hi);
      const float f = constant_jolt::rampDistFromApeakSq(v_from, a_entry, ap_sq, j_max, a_max) - dist_total;
      if (f <= 0.0f) ap_sq_lo = ap_sq; else ap_sq_hi = ap_sq;
      if (f <= 0.0f && f > -CJP_CONVERGE_TOL) break;
      if (ap_sq_hi - ap_sq_lo < 1e-6f) break;
    }
    float V = v_from + (ap_sq_lo - half_a_entry_sq) / j_max;
    return _MIN(_MAX(V, 0.0f), v_max);
  }
}

#endif // FTM_CONSTANT_JOLT
