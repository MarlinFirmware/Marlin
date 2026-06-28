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

#include "../planner.h"

constexpr float CJP_MERGE_AMAX_RATIO = 1.1f;    // Max accel ratio between blocks to allow merging into a superblock
constexpr float CJP_MERGE_NOM_RATIO  = 1.01f;   // Max nominal speed ratio between blocks to allow merging
constexpr float CJP_TOL              = 0.01f;   // (mm or mm/s) Float-precision tolerance for velocity/distance "close enough" checks
constexpr float CJP_CONVERGE_TOL     = 0.001f;  // (mm/s) Newton/bisection early-exit threshold — stop when bracket < this
constexpr float CJP_REUSE_MARGIN     = 1.001f;  // Multiplicative margin for trajectory reuse: skip replan if junction improved < 0.1%
constexpr float CJP_INFEASIBLE_REL   = 1.01f;   // Relative tolerance for infeasible decel ramp detection (1% margin)
constexpr float CJP_INFEASIBLE_ABS   = 1.0f;    // (mm/s²) Absolute tolerance for infeasible decel ramp detection

/**
 * Constant-jolt block planner (1-block emission with non-zero a_entry)
 *
 * The standard trapezoidal planner assumes instant acceleration changes.
 * Under jolt constraints, S-curves need more distance for the same speed
 * change, so a different planner is needed.
 *
 * This planner:
 * - Ignores Marlin's trapezoidal entry/exit speeds entirely
 * - Uses block->vmax_junction as the junction speed ceiling (geometric limit)
 * - Runs a jolt-aware backward pass on all visible blocks
 * - Merges compatible consecutive blocks into a superblock trajectory,
 *   but emits only the first block, carrying exit (v, a) to the next cycle
 *
 *
 * ─── 1-block emission model ───
 *
 *   Each planNext() call:
 *   1. Plans a multi-block superblock trajectory [0..left_end) for lookahead
 *   2. Truncates the trajectory to block 0's distance (truncateToDistance)
 *   3. Stores exit (v, a) at block 0 boundary for the next call
 *   4. Consumes only 1 block from the buffer
 *
 *   The non-zero exit acceleration (a_exit) from block 0's cut point is
 *   passed as a_entry to the next cycle's plan_accel_decel(). This gives
 *   smoother transitions than forcing a=0 at every block boundary.
 *
 *   When a_entry is infeasible for plan_accel_decel (e.g., too high for the
 *   available distance), the planner falls back to plan_decel,
 *   then plan_decel_with_carry, then plan_accel_decel to v=0.
 *
 * ─── Backward pass (lazy) ───
 *
 *   safe_tail = max speed at which block i can be entered (with a=0)
 *   such that blocks [i..N) can decelerate to a stop by block N.
 *
 *   Computed right-to-left using a single accumulator (no array):
 *     safe_tail = 0  (terminal: must stop after last block)
 *     safe_tail = maxReachableSpeed(safe_tail, mm[i], ...)  for i = N-1 down to max_left_compatible
 *   Further steps computed lazily as candidates fail (see merge algorithm).
 *
 * ─── Merge algorithm (largest feasible left_end) ───
 *
 *   1. Find left-compatible extent: same nominal, accel ratio ≤ 1.1.
 *   2. Try from largest left_end downward, take first feasible:
 *      a. v_junction = min(safe_tail, nominal, vmax_junction)
 *      b. Plan left S-curve toward v_junction
 *      c. Check velocity at block 0 exit against vmax_junction[1]
 *      d. Check interior junctions against vmax_junction[k]
 *   3. For candidate=1 if exit overshoots vmax_junction[1], replan
 *      targeting v=0 (safety fallback — no smaller candidate available).
 *   4. Truncate trajectory to block 0, store (v, a) exit state.
 *
 *   Replanning each cycle from actual (v, a) compensates for the
 *   conservative backward pass (which assumes a=0 at boundaries).
 *
 * ─── Feasibility invariant ───
 *
 *   The very first call starts with v_entry=0, a_entry=0. From rest,
 *   a feasible trajectory always exists for any distance (accelerate
 *   arbitrarily little, then decelerate back to zero). Each subsequent
 *   call enters with the (v, a) from truncating a feasible trajectory
 *   at block 0's boundary — a state that was reachable within a valid
 *   plan. Since the planner can always fall back to decelerating to v=0
 *   from any such state, feasibility is maintained across all cycles.
 */

class ConstantJoltTrajectoryGenerator;  // Forward declaration

// Trajectory state: phases + cache + parameters.
// Inherited by the generator (direct field access), embedded in snapshot (struct copy).
struct CJTrajectoryState {
  float phase_dt[7] = {};
  float phase_start_time[7] = {};
  float phase_start_pos[7] = {};
  float phase_start_v[7] = {};
  float phase_start_a[7] = {};
  float v_exit = 0, a_exit = 0;
  float a_max = 0, j_max = 0;
  float dist_total = 0;
  float total_duration = 0;
};

// Pre-truncation trajectory state for reuse optimization.
struct CJTrajectorySnapshot {
  CJTrajectoryState traj;
  float mm_consumed = 0;   // Cumulative distance consumed from the original trajectory
  float t_consumed = 0;    // Absolute time at mm_consumed in the original trajectory
  float v_exit_junction = 0;  // Backward pass value at the merge boundary (for reuse check)
  uint8_t left_end = 0;    // Remaining blocks after consuming block 0 (0 = invalid)
};

class ConstantJoltBlockPlanner {
 public:
  bool multi_block_tail = true;  // Use continuous decel across mergeable tail blocks

  // Reset all planner state (called by the generator's planRunout/reset).
  void resetPlannerState() {
    prev_traj.left_end = 0;
    prev_traj.v_exit_junction = 0;
    for (int i = 0; i < BLOCK_BUFFER_SIZE; i++) prev_safe_tails[i] = 0;
  }

  /**
   * Plan trajectory for the current block (already consumed from planner buffer).
   *
   * Looks ahead at future blocks via get_future_block(), runs a jolt-aware
   * reverse/forward pass across all visible blocks, then plans the first
   * block (or merged group) as an S-curve trajectory.
   *
   * Returns true if a trajectory is ready for execution.
   */
  bool planNext(ConstantJoltTrajectoryGenerator& traj, float j_max);


 private:
  /**
   * Max speed reachable from v_from over dist_total via jolt-limited ramp,
   * capped by v_max. Bracketed Newton on closed-form rampDist.
   * When v_from > v_max, returns v_max (hard ceiling).
   * Monotone in dist_total: more distance → higher or equal result.
   *
   * a_entry: initial acceleration (default 0). When non-zero, the ramp must
   * first absorb a_entry (bring a to 0), which uses distance and reduces
   * the max reachable speed. Uses rampDistWithA for distance.
   */
  float maxReachableSpeed(float v_from, float dist_total,
                          float v_max, float a_max, float j_max,
                          float a_entry = 0.0f);

  // Verify all junctions (interior + exit) of a merged trajectory.
  // Returns true if velocity at every block boundary respects vmax_junction.
  bool verifyJunctions(const ConstantJoltTrajectoryGenerator& traj,
                       uint8_t merge_count, const float mm[],
                       const float vmax_junction[]);

  float multiBlockSafeSpeed(uint8_t i, uint8_t block_count,
                            const float* mm, const float* nominal,
                            const float* vmax_junction, const float* accel,
                            float j_max);

  void shiftSafeTails(uint8_t block_count) {
    for (int i = 0; i < block_count - 1; i++)
      prev_safe_tails[i] = prev_safe_tails[i + 1];
    for (int i = block_count - 1; i < BLOCK_BUFFER_SIZE; i++)
      prev_safe_tails[i] = 0;
  }

  // Previous planNext state for trajectory reuse.
  CJTrajectorySnapshot prev_traj;

  float prev_safe_tails[BLOCK_BUFFER_SIZE] = {};

};
