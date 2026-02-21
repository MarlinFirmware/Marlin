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
 * Merge algorithm:
 * - Find left-compatible group and right-compatible group
 * - Treat right group as a superblock for better exit speed on the left
 * - Check v_peak of left group against min interior junction limit
 * - Binary split on failure until valid or single block
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
      block_t* blk = planner.get_future_block(i);
      if (!blk || blk->is_sync()) break;

      mm[i] = blk->millimeters;
      nominal[i] = blk->nominal_speed;
      accel[i] = blk->acceleration;
      max_junction_v[i] = SQRT(blk->max_entry_speed_sqr);
      block_count++;
    }

    // --- 2. Jerk-aware reverse/forward pass ---
    float entry_v[CJP_MAX_LOOKAHEAD];
    float exit_v[CJP_MAX_LOOKAHEAD];

    if (block_count == 1) {
      // Single block: must decelerate to 0
      exit_v[0] = 0.0f;
      entry_v[0] = _MIN(max_junction_v[0], nominal[0],
                         maxReachableSpeed(0.0f, mm[0], nominal[0], accel[0], jerk_max));
    }
    else {
      // Reverse pass
      exit_v[block_count - 1] = 0.0f;
      for (int8_t i = block_count - 1; i >= 0; i--) {
        float v_reachable = maxReachableSpeed(exit_v[i], mm[i], nominal[i], accel[i], jerk_max);
        entry_v[i] = _MIN(v_reachable, max_junction_v[i], nominal[i]);
        if (i > 0) exit_v[i - 1] = entry_v[i];
      }

      // Forward pass
      for (uint8_t i = 0; i < block_count; i++) {
        float v_reachable = maxReachableSpeed(entry_v[i], mm[i], nominal[i], accel[i], jerk_max);
        if (i < block_count - 1) {
          exit_v[i] = _MIN(v_reachable, entry_v[i + 1], nominal[i]);
          entry_v[i + 1] = _MIN(entry_v[i + 1], exit_v[i]);
        }
        else {
          exit_v[i] = _MIN(v_reachable, exit_v[i], nominal[i]);
        }
      }
    }

    // --- 4. Determine merge group ---
    //
    // Algorithm:
    // 1. Find left-compatible range [0..L-1] and right-compatible range [L..R-1]
    // 2. Treat right as a superblock for better left exit speed
    // 3. Compute v_peak of left group; if > min interior junction, split and retry
    // 4. Same for right group
    // 5. Left group is the merge group to execute

    uint8_t merge_count = 1;

    if (block_count > 2) {
      // Find left-compatible group, tracking cumulative mm, min accel, min junction
      float group_a_min = accel[0], group_a_max = accel[0];
      float cum_mm[CJP_MAX_LOOKAHEAD];  // cum_mm[i] = sumDist(mm, i+1)
      float cum_min_a[CJP_MAX_LOOKAHEAD]; // cum_min_a[i] = minVal(accel, 0, i+1)
      float cum_min_jv[CJP_MAX_LOOKAHEAD]; // cum_min_jv[i] = minVal(max_junction_v, 1, i+1) for i>=1
      cum_mm[0] = mm[0];
      cum_min_a[0] = accel[0];
      cum_min_jv[0] = max_junction_v[0]; // unused but initialized

      uint8_t left_end = 1;
      for (uint8_t i = 1; i < block_count; i++) {
        if (nominal[i] != nominal[0]) break;
        float new_min = _MIN(group_a_min, accel[i]);
        float new_max = _MAX(group_a_max, accel[i]);
        if (new_max > new_min * CJP_MERGE_AMAX_RATIO) break;
        group_a_min = new_min;
        group_a_max = new_max;
        cum_mm[i] = cum_mm[i - 1] + mm[i];
        cum_min_a[i] = _MIN(cum_min_a[i - 1], accel[i]);
        cum_min_jv[i] = (i == 1) ? max_junction_v[1] : _MIN(cum_min_jv[i - 1], max_junction_v[i]);
        left_end = i + 1;
      }

      // Cap left group to at most half the visible blocks
      uint8_t max_left = block_count / 2;
      if (max_left < 2) max_left = 2;
      if (left_end > max_left) left_end = max_left;

      if (left_end >= 2) {
        // Find right-compatible group starting at left_end
        uint8_t right_end = left_end;
        if (left_end < block_count) {
          float r_a_min = accel[left_end], r_a_max = accel[left_end];
          right_end = left_end + 1;
          for (uint8_t i = left_end + 1; i < block_count; i++) {
            if (nominal[i] != nominal[left_end]) break;
            float new_min = _MIN(r_a_min, accel[i]);
            float new_max = _MAX(r_a_max, accel[i]);
            if (new_max > new_min * CJP_MERGE_AMAX_RATIO) break;
            r_a_min = new_min;
            r_a_max = new_max;
            right_end = i + 1;
          }
        }

        // Iteratively refine: split groups until junction constraints are met
        while (true) {
          if (left_end < 2) break;

          float left_mm = cum_mm[left_end - 1];
          float left_a = cum_min_a[left_end - 1];
          float left_nominal = nominal[0];

          // Compute the exit speed for the left group.
          // If the right side is a superblock (>1 block), it can accept higher
          // entry speed since it has more distance to decelerate.
          float right_mm = 0, right_a = 0, right_nominal = 0;
          bool has_right_super = (right_end > left_end + 1);
          if (has_right_super) {
            right_mm = sumDist(mm + left_end, right_end - left_end);
            right_a = minVal(accel, left_end, right_end);
            right_nominal = nominal[left_end];
          }

          float after_left_entry;
          if (has_right_super) {
            float tail_entry = (right_end < block_count) ? entry_v[right_end] : 0.0f;
            float v_reach = maxReachableSpeed(tail_entry, right_mm, right_nominal, right_a, jerk_max);
            after_left_entry = _MIN(v_reach, max_junction_v[left_end], right_nominal);
          }
          else if (left_end < block_count) {
            after_left_entry = entry_v[left_end];
          }
          else {
            after_left_entry = 0.0f;
          }

          // Left superblock: reverse then forward
          float left_exit = after_left_entry;
          float v_reach = maxReachableSpeed(left_exit, left_mm, left_nominal, left_a, jerk_max);
          float left_entry = _MIN(v_reach, max_junction_v[0], left_nominal);

          // Forward: cap exit by what's reachable from entry
          float v_fwd = maxReachableSpeed(left_entry, left_mm, left_nominal, left_a, jerk_max);
          left_exit = _MIN(v_fwd, left_exit, left_nominal);

          // Check v_peak against min interior junction limit
          float v_peak = peakSpeed(left_entry, left_exit, left_a, jerk_max, left_mm, left_nominal);
          float min_jv = cum_min_jv[left_end - 1];

          if (v_peak <= min_jv) {
            // Left group valid. Check right group if it's a superblock.
            if (has_right_super) {
              float right_entry = left_exit;
              float right_exit_v = (right_end < block_count) ? entry_v[right_end] : 0.0f;

              float rv_fwd = maxReachableSpeed(right_entry, right_mm, right_nominal, right_a, jerk_max);
              right_exit_v = _MIN(rv_fwd, right_exit_v, right_nominal);

              float right_vpeak = peakSpeed(right_entry, right_exit_v, right_a, jerk_max, right_mm, right_nominal);
              float right_min_jv = minVal(max_junction_v, left_end + 1, right_end);

              if (right_vpeak > right_min_jv) {
                uint8_t right_len = right_end - left_end;
                right_end = left_end + right_len / 2;
                if (right_end <= left_end) right_end = left_end + 1;
                continue;
              }
            }
            // Both groups valid
            merge_count = left_end;
            entry_v[0] = left_entry;
            exit_v[left_end - 1] = left_exit;
            break;
          }
          else {
            // Left too aggressive — split in half
            uint8_t new_left_end = left_end / 2;
            if (new_left_end < 2) new_left_end = 1;
            right_end = left_end;
            left_end = new_left_end;
          }
        }
      }
    }

    // --- 5. Plan trajectory ---
    float plan_entry = entry_v[0];
    float plan_exit = exit_v[merge_count - 1];
    float plan_mm = sumDist(mm, merge_count);
    float plan_a = minVal(accel, 0, merge_count);
    float plan_nominal = nominal[0];

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
      if (hi - lo < 0.01f) break;
    }
    return lo;
  }

  /**
   * Compute the peak velocity of an S-curve trajectory (without building phases).
   */
  static float peakSpeed(float v_entry, float v_exit, float a_max_val,
                          float j_max_val, float dist, float v_nominal) {
    const float v_small = _MIN(v_entry, v_exit);
    const float v_large = _MAX(v_entry, v_exit);
    float v_peak = _MAX(v_large, v_nominal);
    float s_ramps = cj_totalRampDist(v_peak, v_small, v_large, j_max_val, a_max_val);

    if (s_ramps > dist) {
      float v_hi = v_peak, v_lo = v_large;
      if (cj_totalRampDist(v_lo, v_small, v_large, j_max_val, a_max_val) > dist)
        return v_lo;
      for (int i = 0; i < 16; i++) {
        float mid = 0.5f * (v_lo + v_hi);
        float s = cj_totalRampDist(mid, v_small, v_large, j_max_val, a_max_val);
        if (s > dist) v_hi = mid; else v_lo = mid;
        if (v_hi - v_lo < 0.01f) break;
      }
      v_peak = v_lo;
    }
    return v_peak;
  }

  static float sumDist(const float* mm_arr, uint8_t count) {
    float total = 0;
    for (uint8_t i = 0; i < count; i++) total += mm_arr[i];
    return total;
  }

  // Minimum value in arr[from..to-1]
  static float minVal(const float* arr, uint8_t from, uint8_t to) {
    float v = arr[from];
    for (uint8_t i = from + 1; i < to; i++) v = _MIN(v, arr[i]);
    return v;
  }

  float jerk_max = 30000.0f;

  ConstantJerkTrajectoryGenerator traj;
  uint8_t group_block_count = 0;
  uint8_t orig_block_index = 0;
  float orig_block_start_dist = 0;
  float orig_block_end_dist = 0;
  bool has_plan = false;
};
