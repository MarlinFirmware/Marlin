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

#include "../planner.h"
#include "trajectory_constant_jerk.h"

#define CJP_MERGE_AMAX_RATIO 1.1f

/**
 * Constant-jerk block planner
 *
 * The standard trapeozidal planner assumes instant acceleration changes.
 * Under jerk constraints, S-curves need more distance for the same speed
 * change, so a diferent planner is needed.
 *
 * This planner:
 * - Ignores block->entry_speed and block->exit_speed entirely
 * - Uses block->max_entry_speed_sqr as junction speed ceiling (geometric, valid)
 * - Runs its own jerk-aware reverse/forward pass on all visible blocks enforcing
 * zero acceleration at junctions
 * - Optionally merges compatible consecutive blocks into a single S-curve to remove
 * the relax the zero acceleration at junctions limitation
 *
 *
 * Merge algorithm:
 * - Find left-compatible group and right-compatible group (same nominal speed, close max axel)
 * - Treat right group as a superblock for better exit speed on the left
 * - Check v_peak of left group against min interior junction limit
 * - Binary split on failure until valid or single block (new left and right are halves of previous left group)
 */

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
      if (s > dist)
        v_hi = mid;
      else
        v_lo = mid;
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

class ConstantJerkBlockPlanner {
 public:
  void reset() {
    has_plan = false;
    orig_block_index = 0;
    orig_block_start_dist = 0;
    orig_block_end_dist = 0;
    group_block_count = 0;
    blocks_until_end = -1;
    stored_boundary_entry_v = 0.0f;
    prev_group_block_count = 0;
    prev_right_group_size = 0;
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
  bool planNext(const block_t* current_block, const float jerk_max) {
    has_plan = false;

    if (!current_block) return false;

    // --- 1. Gather block parameters into local arrays ---
    // Block 0 = current_block (already consumed from buffer)
    // Blocks 1..N-1 = future blocks still in buffer

    float mm[BLOCK_BUFFER_SIZE];
    float nominal[BLOCK_BUFFER_SIZE];
    float accel[BLOCK_BUFFER_SIZE];
    float max_junction_v[BLOCK_BUFFER_SIZE];  // max entry speed ceiling per block

    mm[0] = current_block->millimeters;
    nominal[0] = current_block->nominal_speed;
    accel[0] = current_block->acceleration;
    max_junction_v[0] = SQRT(current_block->max_entry_speed_sqr);

    uint8_t block_count = 1;

    // Look ahead at future blocks.
    // get_future_block(offset) returns block_buffer[tail + offset].
    // The current block is at tail (offset 0), so offset 1 = next block.
    for (uint8_t i = 1; i < BLOCK_BUFFER_SIZE; i++) {
      block_t* blk = planner.get_future_block(i);
      if (!blk || blk->is_sync()) break;

      mm[i] = blk->millimeters;
      nominal[i] = blk->nominal_speed;
      accel[i] = blk->acceleration;
      max_junction_v[i] = SQRT(blk->max_entry_speed_sqr);
      block_count++;
    }

    // --- 2. Jerk-aware reverse/forward pass ---
    // INVARIANT: entry_v[0] MUST equal stored_boundary_entry_v at boundaries
    // to ensure velocity continuity between merged groups.
    float entry_v[BLOCK_BUFFER_SIZE];
    float exit_v[BLOCK_BUFFER_SIZE];

    // Decrement blocks_until_end as blocks are consumed
    if (blocks_until_end >= 0) {
      blocks_until_end -= prev_group_block_count;
      if (blocks_until_end < 0) blocks_until_end = -1;
    }

    // Initialize entry_v[0] based on boundary state
    if (blocks_until_end == 0) {
      // At boundary: MUST match previous exit velocity
      entry_v[0] = stored_boundary_entry_v;
    } else if (blocks_until_end < 0) {
      // Fresh start: begin from rest
      entry_v[0] = 0.0f;
    }

    if (block_count == 1) {
      // Single block: just validate constraints
      exit_v[0] = 0.0f;
      entry_v[0] = _MIN(entry_v[0], max_junction_v[0], nominal[0]);
    } else {
      // Multi-block: standard reverse/forward pass
      // INVARIANT: entry_v[0] is already set (boundary or fresh start)

      // Reverse pass: start from end, work backwards to entry_v[0]
      exit_v[block_count - 1] = 0.0f;
      for (int8_t i = block_count - 1; i > 0; i--) {
        float v_reachable = maxReachableSpeed(exit_v[i], mm[i], nominal[i], accel[i], jerk_max);
        entry_v[i] = _MIN(v_reachable, max_junction_v[i], nominal[i]);
        exit_v[i - 1] = entry_v[i];
      }

      // Now entry_v[0] is set by reverse pass - apply boundary constraint if needed
      if (blocks_until_end == 0) {
        entry_v[0] = stored_boundary_entry_v;
      } else if (blocks_until_end < 0) {
        entry_v[0] = 0.0f;
      }
      // Re-propagate from entry_v[0] to ensure consistency
      for (uint8_t i = 0; i < block_count - 1; i++) {
        float v_reachable = maxReachableSpeed(entry_v[i], mm[i], nominal[i], accel[i], jerk_max);
        exit_v[i] = _MIN(v_reachable, entry_v[i + 1], nominal[i]);
        entry_v[i + 1] = _MIN(entry_v[i + 1], exit_v[i]);
      }
      // Last block exits to 0
      float v_reachable = maxReachableSpeed(entry_v[block_count - 1], mm[block_count - 1],
                                            nominal[block_count - 1], accel[block_count - 1], jerk_max);
      exit_v[block_count - 1] = _MIN(v_reachable, 0.0f, nominal[block_count - 1]);
    }

    // --- 4. Determine merge group ---
    //
    // Algorithm:
    // 1. Find left-compatible range [0..L-1] and right-compatible range [L..R-1]
    // 2. Treat right as a superblock for better left exit speed
    // 3. Compute v_peak of left group; if > min interior junction, split and retry
    // 4. Same for right group
    // 5. Left group is the merge group to execute

    uint8_t left_end = 1;
    uint8_t right_end = 1;
    if (block_count > 2) {
      // Find left-compatible group, tracking cumulative mm, min accel, min junction
      float group_a_min = accel[0], group_a_max = accel[0];
      float cum_mm[BLOCK_BUFFER_SIZE];      // cum_mm[i] = sumDist(mm, i+1)
      float cum_min_a[BLOCK_BUFFER_SIZE];   // cum_min_a[i] = minVal(accel, 0, i+1)
      float cum_min_jv[BLOCK_BUFFER_SIZE];  // cum_min_jv[i] = minVal(max_junction_v, 1, i+1) for i>=1
      cum_mm[0] = mm[0];
      cum_min_a[0] = accel[0];
      cum_min_jv[0] = max_junction_v[0];  // unused but initialized

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

      // Cap left group to at most half the buffer size
      if (left_end > BLOCK_BUFFER_SIZE / 2) left_end = BLOCK_BUFFER_SIZE / 2;

      if (left_end >= 2) {
        // Find right-compatible group starting at left_end
        right_end = left_end;
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
        // Floor for left group size: when continuing from a boundary, the first
        // prev_right_group_size blocks were previously validated as a right group.
        // Don't shrink left below this or we'd reclassify validated right blocks
        // as unvalidated left blocks with stricter junction constraints.

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
          } else if (left_end < block_count) {
            after_left_entry = entry_v[left_end];
          } else {
            after_left_entry = 0.0f;
          }

          // Left superblock: reverse then forward
          // INVARIANT: Use entry_v[0] which was set by boundary/fresh-start logic
          float left_exit = after_left_entry;
          float v_reach = maxReachableSpeed(left_exit, left_mm, left_nominal, left_a, jerk_max);
          float left_entry = _MIN(v_reach, entry_v[0], left_nominal);

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
            entry_v[0] = left_entry;
            exit_v[left_end - 1] = left_exit;
            break;
          } else {
            // Left too aggressive — need to split
            uint8_t new_left_end = left_end / 2;
            if (new_left_end < 2) new_left_end = 1;

            // Don't shrink below floor - the blocks we're shrinking into were
            // previously validated as a right group (when continuing from boundary)
            if (new_left_end < prev_right_group_size) {
              // Try shrinking right group instead
              uint8_t right_len = right_end - left_end;
              if (right_len > 1) {
                right_end = left_end + right_len / 2;
                continue;
              }
              // TODO(@dbuezas): We should never get here but we do
              // I think here lies the last discontinuity bug
            }

            right_end = left_end;
            left_end = new_left_end;
          }
        }
      }
    }

    // --- 5. Plan trajectory ---
    float plan_entry = entry_v[0];
    float plan_exit = exit_v[left_end - 1];
    if (left_end < block_count)
      plan_exit = _MIN(plan_exit, entry_v[left_end]);
    float plan_mm = sumDist(mm, left_end);
    float plan_a = minVal(accel, 0, left_end);
    float plan_nominal = nominal[0];

    // Apply boundary constraint after merge algorithm
    if (blocks_until_end == 0) {
      plan_entry = _MIN(plan_entry, stored_boundary_entry_v);
    } else if (blocks_until_end < 0) {
      plan_entry = 0.0f;
    }

    traj.plan_full(plan_entry, plan_exit, plan_a, jerk_max, plan_mm, plan_nominal);

    // === STORE BOUNDARY INFO FOR NEXT PASS ===
    // INVARIANT: stored_boundary_entry_v MUST equal the actual trajectory exit velocity.
    // This is required for velocity continuity at group boundaries.
    // NOTE: Do NOT use exit_v[left_end-1] here - it may differ from trajectory
    // exit due to S-curve constraints (next block's entry limit).
    if (left_end < block_count) {
      blocks_until_end = left_end;  // blocks until we hit right group boundary
      // INVARIANT: Store trajectory exit, not exit_v array value
      stored_boundary_entry_v = traj.getVelocityAtTime(traj.getTotalDuration());
    } else {
      blocks_until_end = -1;
      stored_boundary_entry_v = 0.0f;
    }

    // Set up execution tracking
    prev_group_block_count = left_end;
    prev_right_group_size = right_end - left_end;
    group_block_count = left_end;
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

  ConstantJerkTrajectoryGenerator traj;
  uint8_t group_block_count = 0;
  uint8_t prev_group_block_count = 0;
  uint8_t prev_right_group_size = 0;  // Floor for left group size on next planNext
  uint8_t orig_block_index = 0;
  float orig_block_start_dist = 0;
  float orig_block_end_dist = 0;
  bool has_plan = false;

  // === BOUNDARY VELOCITY TRACKING INVARIANTS ===
  //
  // When blocks are consumed and a new plan is created:
  // 1. stored_boundary_entry_v MUST equal the actual trajectory exit velocity
  //    of the previously consumed blocks (for velocity continuity)
  // 2. blocks_until_end counts down from merge_count to 0 as blocks are consumed
  // 3. When blocks_until_end == 0, the current block is the boundary between
  //    the consumed left group and the remaining right group
  // 4. At boundaries, entry_v[0] MUST be set to stored_boundary_entry_v
  //    (forced, not just capped) to maintain continuity
  //
  // The trajectory generator may produce different exit velocities than
  // exit_v[] due to S-curve constraints. Always use trajectory.getVelocityAtTime()
  // for storing boundary velocities, never exit_v[].
  //
  int blocks_until_end = -1;
  float stored_boundary_entry_v = 0.0f;
};
