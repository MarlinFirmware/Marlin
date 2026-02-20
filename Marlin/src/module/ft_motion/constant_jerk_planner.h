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

#include "trajectory_constant_jerk.h"
#include "../planner.h"

#ifndef FTM_MERGE_BUFFER_THRESHOLD
  #define FTM_MERGE_BUFFER_THRESHOLD 0.75f
#endif

#define CJP_MERGE_AMAX_RATIO 1.1f
#define CJP_MAX_LOOKAHEAD 16

/**
 * Constant-jerk block planner with own jerk-aware reverse/forward pass.
 *
 * Marlin's planner computes entry/exit speeds using trapezoidal kinematics
 * (v² = v₀² + 2·a·d) which assume instant acceleration changes. Under jerk
 * constraints, S-curves need more distance for the same speed change, so
 * Marlin's speeds are not achievable.
 *
 * This planner:
 * - Ignores block->entry_speed and block->exit_speed entirely
 * - Uses block->max_entry_speed_sqr as junction speed ceiling (geometric, valid)
 * - Runs its own jerk-aware reverse/forward pass on all visible blocks
 * - Optionally merges compatible consecutive blocks into a single S-curve
 *
 * Merging rules (unchanged):
 * - Only when buffer >= 75% full (FTM_MERGE_BUFFER_THRESHOLD)
 * - Only up to half the queued blocks
 * - Compatible = same nominal_speed, a_max within 10% ratio
 */
class ConstantJerkBlockPlanner {
public:

  void setJerkMax(float j) { jerk_max = j; }
  float getJerkMax() const { return jerk_max; }

  void reset() {
    has_plan = false;
    orig_block_index = 0;
    orig_block_start_dist = 0;
    orig_block_end_dist = 0;
    group_block_count = 0;
    traj.reset();
  }

  /**
   * Plan trajectory for the current block (already consumed from planner buffer).
   *
   * @param current_block The block already consumed by plan_next_block()
   *
   * Looks ahead at future blocks via get_future_block(), runs a jerk-aware
   * reverse/forward pass across all visible blocks, then plans the first
   * block (or merged group) as an S-curve trajectory.
   *
   * Returns true if a trajectory is ready for execution.
   */
  bool planNext(const block_t* current_block) {
    has_plan = false;

    if (!current_block) return false;

    // --- 1. Gather block parameters into local arrays ---
    // Block 0 = current_block (already consumed from buffer)
    // Blocks 1..N-1 = future blocks still in buffer

    float mm[CJP_MAX_LOOKAHEAD];
    float nominal[CJP_MAX_LOOKAHEAD];
    float accel[CJP_MAX_LOOKAHEAD];
    float max_junction_v[CJP_MAX_LOOKAHEAD]; // max entry speed ceiling per block

    mm[0] = current_block->millimeters;
    nominal[0] = current_block->nominal_speed;
    accel[0] = current_block->acceleration;
    max_junction_v[0] = SQRT(current_block->max_entry_speed_sqr);

    uint8_t block_count = 1;

    // Look ahead at future blocks.
    // get_future_block(offset) returns block_buffer[tail + offset].
    // The current block is at tail (offset 0), so offset 1 = next block.
    for (uint8_t i = 1; i < CJP_MAX_LOOKAHEAD; i++) {
      block_t* blk = planner.get_future_block(i); // offset 1 = next block after current
      if (!blk || blk->is_sync()) break;

      mm[i] = blk->millimeters;
      nominal[i] = blk->nominal_speed;
      accel[i] = blk->acceleration;
      max_junction_v[i] = SQRT(blk->max_entry_speed_sqr);
      block_count++;
    }

    // --- 2. Jerk-aware reverse pass ---
    // Compute achievable entry/exit speeds for each block

    float entry_v[CJP_MAX_LOOKAHEAD];
    float exit_v[CJP_MAX_LOOKAHEAD];

    // Last block must be able to stop (safe exit = 0)
    exit_v[block_count - 1] = 0.0f;

    for (int8_t i = block_count - 1; i >= 0; i--) {
      // What entry speed can reach this block's exit speed over its distance?
      float v_reachable = maxReachableSpeed(exit_v[i], mm[i], nominal[i], accel[i], jerk_max);
      // Cap by junction ceiling and nominal
      entry_v[i] = _MIN(v_reachable, max_junction_v[i], nominal[i]);
      // This block's entry speed becomes the previous block's exit speed
      if (i > 0) exit_v[i - 1] = entry_v[i];
    }

    // --- 3. Jerk-aware forward pass ---
    // entry_v[0] is already set from reverse pass; propagate forward

    for (uint8_t i = 0; i < block_count; i++) {
      // What exit speed can be reached from this block's entry speed?
      float v_reachable = maxReachableSpeed(entry_v[i], mm[i], nominal[i], accel[i], jerk_max);
      // Cap by what the next block can accept
      if (i < block_count - 1) {
        exit_v[i] = _MIN(v_reachable, entry_v[i + 1], nominal[i]);
        // Propagate: next block's entry can't exceed what we can deliver
        entry_v[i + 1] = _MIN(entry_v[i + 1], exit_v[i]);
      }
      else {
        exit_v[i] = _MIN(v_reachable, exit_v[i], nominal[i]);
      }
    }

    // --- 4. Determine merge group ---
    // Merging rules:
    // - Only when buffer >= 75% full
    // - Only up to half the visible blocks (never merge ALL visible blocks)
    //   This ensures the reverse pass has blocks beyond the merge group to
    //   propagate speeds from, and those blocks can merge with future arrivals.
    // - Compatible = same nominal_speed, a_max within 10% ratio
    // - Interior junction speeds must not exceed max_junction_v

    const uint8_t queued = planner.movesplanned();
    const uint8_t threshold = (uint8_t)(BLOCK_BUFFER_SIZE * FTM_MERGE_BUFFER_THRESHOLD);

    uint8_t merge_count = 1;

    if (queued >= threshold && block_count > 2) {
      // Never merge more than half the visible blocks — always leave at least
      // half for look-ahead so the exit speed isn't forced to 0, and so the
      // remaining blocks can merge with new arrivals later.
      uint8_t max_merge = block_count / 2;
      if (max_merge < 2) max_merge = 2;

      float group_nominal = nominal[0];
      float group_a_min = accel[0];
      float group_a_max = accel[0];

      for (uint8_t i = 1; i < max_merge; i++) {
        // Check merge compatibility
        if (nominal[i] != group_nominal) break;

        float new_min = _MIN(group_a_min, accel[i]);
        float new_max = _MAX(group_a_max, accel[i]);
        if (new_max > new_min * CJP_MERGE_AMAX_RATIO) break;

        // Verify interior junction: the merged trajectory has no speed
        // constraint at this point, so check that the unconstrained speed
        // here won't exceed the junction limit.
        float dist_to_junction = sumDist(mm, i);
        float dist_after_junction = sumDist(mm, merge_count + 1) - dist_to_junction;
        float cons_a = _MIN(group_a_min, accel[i]);

        float v_from_entry = maxReachableSpeed(entry_v[0], dist_to_junction,
                                                group_nominal, cons_a, jerk_max);
        float v_from_exit = maxReachableSpeed(exit_v[merge_count], dist_after_junction,
                                               group_nominal, cons_a, jerk_max);
        float v_at_junction = _MIN(v_from_entry, v_from_exit);

        if (v_at_junction > max_junction_v[i]) break;

        group_a_min = new_min;
        group_a_max = new_max;
        merge_count++;
      }
    }

    // --- 5. Plan trajectory ---
    float plan_entry = entry_v[0];
    float plan_exit = exit_v[merge_count - 1];
    float plan_mm, plan_a, plan_nominal;

    if (merge_count > 1) {
      plan_mm = sumDist(mm, merge_count);
      plan_a = accel[0];
      plan_nominal = nominal[0];
      for (uint8_t i = 1; i < merge_count; i++)
        plan_a = _MIN(plan_a, accel[i]);
    }
    else {
      plan_mm = mm[0];
      plan_a = accel[0];
      plan_nominal = nominal[0];
    }

    traj.plan_full(plan_entry, plan_exit, plan_a, jerk_max, plan_mm, plan_nominal);

    // Set up execution tracking
    group_block_count = merge_count;
    orig_block_index = 0;
    orig_block_start_dist = 0;
    orig_block_end_dist = mm[0];

    has_plan = true;
    return true;
  }

  /**
   * Check if the current distance within the merged trajectory has crossed
   * into the next original block.
   */
  bool checkBlockBoundary(float dist_in_merged) {
    if (!has_plan) return false;
    if (orig_block_index >= group_block_count - 1) return false;
    return dist_in_merged >= orig_block_end_dist;
  }

  // Call after confirming a boundary crossing and obtaining the next block
  void advanceBlock(float next_block_mm) {
    orig_block_start_dist = orig_block_end_dist;
    orig_block_index++;
    orig_block_end_dist += next_block_mm;
  }

  /**
   * Get the distance within the current sub-block, given a merged distance.
   */
  float localDistance(float dist_in_merged) const {
    return dist_in_merged - orig_block_start_dist;
  }

  bool isDone() const { return !has_plan || orig_block_index >= group_block_count; }

  ConstantJerkTrajectoryGenerator& trajectory() { return traj; }
  uint8_t blockCount() const { return group_block_count; }
  uint8_t currentBlockIndex() const { return orig_block_index; }
  bool isActive() const { return has_plan; }

private:

  /**
   * Compute the maximum speed reachable from v_from over a given distance
   * using a constant-jerk ramp. Uses binary search with cj_planRamp.
   */
  float maxReachableSpeed(float v_from, float total_mm,
                          float nominal, float a_max_val, float j_max_val) {
    if (total_mm <= 0.0f) return v_from;

    // Trapezoidal upper bound
    float v_trap = SQRT(v_from * v_from + 2.0f * a_max_val * total_mm);
    float hi = _MIN(nominal, v_trap);
    float lo = v_from;

    if (hi <= lo) return lo;

    float pa, pb, pc;
    float s = cj_planRamp(v_from, hi, j_max_val, a_max_val, false, pa, pb, pc);
    if (s <= total_mm) return hi;

    for (int i = 0; i < 32; i++) {
      float mid = 0.5f * (lo + hi);
      s = cj_planRamp(v_from, mid, j_max_val, a_max_val, false, pa, pb, pc);
      if (s <= total_mm)
        lo = mid;
      else
        hi = mid;
    }
    return lo;
  }

  static float sumDist(const float* mm_arr, uint8_t count) {
    float total = 0;
    for (uint8_t i = 0; i < count; i++) total += mm_arr[i];
    return total;
  }

  float jerk_max = 30000.0f;

  ConstantJerkTrajectoryGenerator traj;
  uint8_t group_block_count = 0;
  uint8_t orig_block_index = 0;
  float orig_block_start_dist = 0;
  float orig_block_end_dist = 0;
  bool has_plan = false;
};
