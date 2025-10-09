/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#if ENABLED(FT_MOTION)

#include "ft_motion.h"
#include "ft_motion/trapezoidal_trajectory_generator.h"
#include "ft_motion/poly5_trajectory_generator.h"
#include "ft_motion/poly6_trajectory_generator.h"
#include "stepper.h" // Access stepper block queue function and abort status.
#include "endstops.h"

FTMotion ftMotion;

//-----------------------------------------------------------------
// Variables.
//-----------------------------------------------------------------

// Public variables.

ft_config_t FTMotion::cfg;
bool FTMotion::busy; // = false
ft_command_t FTMotion::stepperCmdBuff[FTM_STEPPERCMD_BUFF_SIZE] = {0U}; // Stepper commands buffer.
int32_t FTMotion::stepperCmdBuff_produceIdx = 0, // Index of next stepper command write to the buffer.
        FTMotion::stepperCmdBuff_consumeIdx = 0; // Index of next stepper command read from the buffer.

bool FTMotion::stepperCmdBuffHasData = false;   // The stepper buffer has items and is in use.

XYZEval<millis_t> FTMotion::axis_move_end_ti = { 0 };
AxisBits FTMotion::axis_move_dir;

// Private variables.

// NOTE: These are sized for Ulendo FBS use.
xyze_trajectory_t    FTMotion::traj;            // = {0.0f} Storage for fixed-time-based trajectory.
xyze_trajectoryMod_t FTMotion::trajMod;         // = {0.0f} Storage for fixed time trajectory window.

bool FTMotion::blockProcRdy = false;            // Set when new block data is loaded from stepper module into FTM,
                                                //  and reset when block is completely converted to FTM trajectory.
bool FTMotion::batchRdy = false;                // Indicates a batch of the fixed time trajectory has been
                                                //  generated, is now available in the upper-batch of traj.A[], and
                                                //  is ready to be post-processed (if applicable) and interpolated.
                                                //  Reset once the data has been shifted out.
bool FTMotion::batchRdyForInterp = false;       // Indicates the batch is done being post processed
                                                //  (if applicable) and is ready to be converted to step commands.

// Block data variables.
xyze_pos_t   FTMotion::startPos,                    // (mm) Start position of block
             FTMotion::endPos_prevBlock = { 0.0f }; // (mm) End position of previous block
xyze_float_t FTMotion::ratio;                       // (ratio) Axis move ratio of block
float FTMotion::tau = 0.0f;                         // (s) Time since start of block

// Trajectory generators
TrapezoidalTrajectoryGenerator FTMotion::trapezoidalGenerator;
Poly5TrajectoryGenerator FTMotion::poly5Generator;
Poly6TrajectoryGenerator FTMotion::poly6Generator;
TrajectoryGenerator* FTMotion::currentGenerator = &FTMotion::trapezoidalGenerator;
TrajectoryType FTMotion::trajectoryType = TrajectoryType::FTM_TRAJECTORY_TYPE;

// Make vector variables.
uint32_t FTMotion::traj_idx_get = 0,            // Index of fixed time trajectory generation of the overall block.
         FTMotion::traj_idx_set = 0;            // Index of fixed time trajectory generation within the batch.

// Interpolation variables.
xyze_long_t FTMotion::steps = { 0 };            // Step count accumulator.
xyze_long_t FTMotion::step_error_q10 = { 0 };   // Fractional remainder in q10.21 format

uint32_t FTMotion::interpIdx = 0;               // Index of current data point being interpolated.

#if ENABLED(DISTINCT_E_FACTORS)
  uint8_t FTMotion::block_extruder_axis;        // Cached E Axis from last-fetched block
#elif HAS_EXTRUDERS
  constexpr uint8_t FTMotion::block_extruder_axis;
  bool FTMotion::use_advance_lead;
#endif

// Shaping variables.
#if HAS_FTM_SHAPING
  FTMotion::shaping_t FTMotion::shaping = {
    zi_idx: 0
    #if HAS_X_AXIS
      , X:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 } // ena, d_zi[], Ai[], Ni[], max_i
    #endif
    #if HAS_Y_AXIS
      , Y:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 }
    #endif
    #if ENABLED(FTM_SHAPER_Z)
      , Z:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 }
    #endif
    #if ENABLED(FTM_SHAPER_E)
      , E:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 }
    #endif
  };
#endif

#if ENABLED(FTM_SMOOTHING)
  FTMotion::smoothing_t FTMotion::smoothing = {
    #if HAS_X_AXIS
      X:{ { 0.0f }, 0.0f, 0 },  // smoothing_pass[], alpha, delay_samples
    #endif
    #if HAS_Y_AXIS
      Y:{ { 0.0f }, 0.0f, 0 },
    #endif
    #if HAS_Z_AXIS
      Z:{ { 0.0f }, 0.0f, 0 },
    #endif
    #if HAS_EXTRUDERS
      E:{ { 0.0f }, 0.0f, 0 }
    #endif
  };
#endif

#if HAS_EXTRUDERS
  // Linear advance variables.
  float FTMotion::prev_traj_e = 0.0f;     // (ms) Unit delay of raw extruder position.
#endif

constexpr uint32_t BATCH_SIDX_IN_WINDOW = (FTM_WINDOW_SIZE) - (FTM_BATCH_SIZE); // Batch start index in window.

//-----------------------------------------------------------------
// Function definitions.
//-----------------------------------------------------------------

// Public functions.

// Controller main, to be invoked from non-isr task.
void FTMotion::loop() {

  if (!cfg.active) return;

  /**
   * Handle block abort with the following sequence:
   * 1. Zero out commands in stepper ISR.
   * 2. Drain the motion buffer, stop processing until they are emptied.
   * 3. Reset all the states / memory.
   * 4. Signal ready for new block.
   */
  if (stepper.abort_current_block) {
    discard_planner_block_protected();
    reset();
    stepper.abort_current_block = false;  // Abort finished.
  }

  while (!blockProcRdy && (stepper.current_block = planner.get_current_block())) {
    if (stepper.current_block->is_sync()) {     // Sync block?
      if (stepper.current_block->is_sync_pos()) // Position sync? Set the position.
        stepper._set_position(stepper.current_block->position);
      discard_planner_block_protected();
      continue;
    }
    loadBlockData(stepper.current_block);
    blockProcRdy = true;

    // Some kinematics track axis motion in HX, HY, HZ
    #if ANY(CORE_IS_XY, CORE_IS_XZ, MARKFORGED_XY, MARKFORGED_YX)
      stepper.last_direction_bits.hx = stepper.current_block->direction_bits.hx;
    #endif
    #if ANY(CORE_IS_XY, CORE_IS_YZ, MARKFORGED_XY, MARKFORGED_YX)
      stepper.last_direction_bits.hy = stepper.current_block->direction_bits.hy;
    #endif
    #if ANY(CORE_IS_XZ, CORE_IS_YZ)
      stepper.last_direction_bits.hz = stepper.current_block->direction_bits.hz;
    #endif
  }

  if (blockProcRdy) {

    if (!batchRdy) generateTrajectoryPointsFromBlock(); // may clear blockProcRdy

    // Check if the block has been completely converted:
    if (!blockProcRdy) {
      discard_planner_block_protected();
      if (!batchRdy && !planner.has_blocks_queued()) {
        runoutBlock();
        generateTrajectoryPointsFromBlock(); // Additional call to guarantee batchRdy is set this loop.
      }
    }
  }

  // FBS / post processing.
  if (batchRdy && !batchRdyForInterp) {

    // Call Ulendo FBS here.

    #if ENABLED(FTM_UNIFIED_BWS)
      trajMod = traj; // Move the window to traj
    #else
      // Copy the uncompensated vectors.
      #define TCOPY(A) memcpy(trajMod.A, traj.A, sizeof(trajMod.A));
      LOGICAL_AXIS_MAP_LC(TCOPY);

      // Shift the time series back in the window
      #define TSHIFT(A) memcpy(traj.A, &traj.A[FTM_BATCH_SIZE], BATCH_SIDX_IN_WINDOW * sizeof(traj.A[0]));
      LOGICAL_AXIS_MAP_LC(TSHIFT);
    #endif

    // ... data is ready in trajMod.
    batchRdyForInterp = true;

    batchRdy = false; // Clear so generateTrajectoryPointsFromBlock() can resume generating points.
  }

  // Interpolation (generation of step commands from fixed time trajectory).
  while (batchRdyForInterp
    && (stepperCmdBuffItems() < (FTM_STEPPERCMD_BUFF_SIZE) - (FTM_STEPS_PER_UNIT_TIME))
  ) {
    generateStepsFromTrajectory(interpIdx);
    if (++interpIdx == FTM_BATCH_SIZE) {
      batchRdyForInterp = false;
      interpIdx = 0;
    }
  }

  // Set busy status for use by planner.busy()
  busy = (stepperCmdBuffHasData || blockProcRdy || batchRdy || batchRdyForInterp);

}

#if HAS_FTM_SHAPING

  // Refresh the gains used by shaping functions.
  void FTMotion::AxisShaping::set_axis_shaping_A(const ftMotionShaper_t shaper, const float zeta, const float vtol) {

    const float K = exp(-zeta * M_PI / sqrt(1.f - sq(zeta))),
                K2 = sq(K),
                K3 = K2 * K,
                K4 = K3 * K;

    switch (shaper) {

      case ftMotionShaper_ZV:
        max_i = 1U;
        Ai[0] = 1.0f / (1.0f + K);
        Ai[1] = Ai[0] * K;
        break;

      case ftMotionShaper_ZVD:
        max_i = 2U;
        Ai[0] = 1.0f / (1.0f + 2.0f * K + K2);
        Ai[1] = Ai[0] * 2.0f * K;
        Ai[2] = Ai[0] * K2;
        break;

      case ftMotionShaper_ZVDD:
        max_i = 3U;
        Ai[0] = 1.0f / (1.0f + 3.0f * K + 3.0f * K2 + K3);
        Ai[1] = Ai[0] * 3.0f * K;
        Ai[2] = Ai[0] * 3.0f * K2;
        Ai[3] = Ai[0] * K3;
        break;

      case ftMotionShaper_ZVDDD:
        max_i = 4U;
        Ai[0] = 1.0f / (1.0f + 4.0f * K + 6.0f * K2 + 4.0f * K3 + K4);
        Ai[1] = Ai[0] * 4.0f * K;
        Ai[2] = Ai[0] * 6.0f * K2;
        Ai[3] = Ai[0] * 4.0f * K3;
        Ai[4] = Ai[0] * K4;
        break;

      case ftMotionShaper_EI: {
        max_i = 2U;
        Ai[0] = 0.25f * (1.0f + vtol);
        Ai[1] = 0.50f * (1.0f - vtol) * K;
        Ai[2] = Ai[0] * K2;

        const float adj = 1.0f / (Ai[0] + Ai[1] + Ai[2]);
        for (uint32_t i = 0; i < 3U; i++) Ai[i] *= adj;
      } break;

      case ftMotionShaper_2HEI: {
        max_i = 3U;
        const float vtolx2 = sq(vtol);
        const float X = pow(vtolx2 * (sqrt(1.0f - vtolx2) + 1.0f), 1.0f / 3.0f);
        Ai[0] = (3.0f * sq(X) + 2.0f * X + 3.0f * vtolx2) / (16.0f * X);
        Ai[1] = (0.5f - Ai[0]) * K;
        Ai[2] = Ai[1] * K;
        Ai[3] = Ai[0] * K3;

        const float adj = 1.0f / (Ai[0] + Ai[1] + Ai[2] + Ai[3]);
        for (uint32_t i = 0; i < 4U; i++) Ai[i] *= adj;
      } break;

      case ftMotionShaper_3HEI: {
        max_i = 4U;
        Ai[0] = 0.0625f * ( 1.0f + 3.0f * vtol + 2.0f * sqrt( 2.0f * ( vtol + 1.0f ) * vtol ) );
        Ai[1] = 0.25f * ( 1.0f - vtol ) * K;
        Ai[2] = ( 0.5f * ( 1.0f + vtol ) - 2.0f * Ai[0] ) * K2;
        Ai[3] = Ai[1] * K2;
        Ai[4] = Ai[0] * K4;

        const float adj = 1.0f / (Ai[0] + Ai[1] + Ai[2] + Ai[3] + Ai[4]);
        for (uint32_t i = 0; i < 5U; i++) Ai[i] *= adj;
      } break;

      case ftMotionShaper_MZV: {
        max_i = 2U;
        const float Bx = 1.4142135623730950488016887242097f * K;
        Ai[0] = 1.0f / (1.0f + Bx + K2);
        Ai[1] = Ai[0] * Bx;
        Ai[2] = Ai[0] * K2;
      }
      break;

      case ftMotionShaper_NONE:
        max_i = 0;
        Ai[0] = 1.0f; // No echoes so the whole impulse is applied in the first tap
        break;
    }

  }

  // Refresh the indices used by shaping functions.
  // Ai[] must be precomputed (if zeta or vtol change, call set_axis_shaping_A first)
  void FTMotion::AxisShaping::set_axis_shaping_N(const ftMotionShaper_t shaper, const float f, const float zeta) {
    // Note that protections are omitted for DBZ and for index exceeding array length.
    const float df = sqrt ( 1.f - sq(zeta) );
    switch (shaper) {
      case ftMotionShaper_ZV:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        break;
      case ftMotionShaper_ZVD:
      case ftMotionShaper_EI:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        break;
      case ftMotionShaper_ZVDD:
      case ftMotionShaper_2HEI:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        Ni[3] = Ni[2] + Ni[1];
        break;
      case ftMotionShaper_ZVDDD:
      case ftMotionShaper_3HEI:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        Ni[3] = Ni[2] + Ni[1];
        Ni[4] = Ni[3] + Ni[1];
        break;
      case ftMotionShaper_MZV:
        Ni[1] = round((0.375f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        break;
      case ftMotionShaper_NONE:
        // No echoes.
        // max_i is set to 0 by set_axis_shaping_A, so delay centroid (Ni[0]) will also correctly be 0
        break;
    }

    // Group delay in samples (i.e., Axis delay caused by shaping): sum(Ai * Ni[i]).
    // Skipping i=0 since the uncompensated delay of the first impulse is always zero, so Ai[0] * Ni[0] == 0
    float centroid = 0.0f;
    for (uint8_t i = 1; i <= max_i; ++i) centroid -= Ai[i] * Ni[i];

    Ni[0] = round(centroid);

    // The resulting echo index can be negative, this is ok because it will be offset
    // by the max delay of all axes before it is used.
    for (uint8_t i = 1; i <= max_i; ++i) Ni[i] += Ni[0];
  }

  #if ENABLED(FTM_SMOOTHING)
    // Set smoothing time and recalculate alpha and delay.
    void FTMotion::AxisSmoothing::set_smoothing_time(const float s_time) {
      if (s_time > 0.001f) {
        alpha = 1.0f - expf(-(FTM_TS) * (FTM_SMOOTHING_ORDER) / s_time );
        delay_samples = s_time * FTM_FS;
      }
      else {
        alpha = 0.0f;
        delay_samples = 0;
      }
    }
  #endif

  void FTMotion::update_shaping_params() {
    #define UPDATE_SHAPER(A) \
      shaping.A.ena = ftMotion.cfg.shaper.A != ftMotionShaper_NONE; \
      shaping.A.set_axis_shaping_A(cfg.shaper.A, cfg.zeta.A, cfg.vtol.A); \
      shaping.A.set_axis_shaping_N(cfg.shaper.A, cfg.baseFreq.A, cfg.zeta.A);

    SHAPED_MAP(UPDATE_SHAPER);
  }

#endif // HAS_FTM_SHAPING

#if ENABLED(FTM_SMOOTHING)

  void FTMotion::update_smoothing_params() {
    #define _SMOOTH_PARAM(A) smoothing.A.set_smoothing_time(cfg.smoothingTime.A);
    CARTES_MAP(_SMOOTH_PARAM);
  }

  void FTMotion::set_smoothing_time(uint8_t axis, const float s_time) {
    #define _SMOOTH_CASE(A) case _AXIS(A): cfg.smoothingTime.A = s_time; break;
    switch (axis) {
      default:
      CARTES_MAP(_SMOOTH_CASE);
    }
    update_smoothing_params();
  }

#endif // FTM_SMOOTHING

// Reset all trajectory processing variables.
void FTMotion::reset() {
  const bool did_suspend = stepper.suspend();

  stepperCmdBuff_produceIdx = stepperCmdBuff_consumeIdx = 0;

  traj.reset();

  blockProcRdy = batchRdy = batchRdyForInterp = false;

  endPos_prevBlock.reset();
  tau = 0;
  traj_idx_get = 0;
  traj_idx_set = TERN(FTM_UNIFIED_BWS, 0, _MIN(BATCH_SIDX_IN_WINDOW, FTM_BATCH_SIZE));

  steps.reset();
  step_error_q10.reset();
  interpIdx = 0;

  #if HAS_FTM_SHAPING
    #define _RESET_ZI(A) ZERO(shaping.A.d_zi);
    SHAPED_MAP(_RESET_ZI);
    shaping.zi_idx = 0;
  #endif

  TERN_(HAS_EXTRUDERS, prev_traj_e = 0.0f);  // Reset linear advance variables.
  TERN_(DISTINCT_E_FACTORS, block_extruder_axis = E_AXIS);

  axis_move_end_ti.reset();

  if (did_suspend) stepper.wake_up();
}

// Private functions.

void FTMotion::discard_planner_block_protected() {
  if (stepper.current_block) {  // Safeguard in case current_block must not be null (it will
                                // be null when the "block" is a runout or generated) in order
                                // to use planner.release_current_block().
    stepper.current_block = nullptr;
    planner.release_current_block();  // FTM uses release_current_block() instead of discard_current_block(),
                                      // as in block_phase_isr(). This change is to avoid invoking axis_did_move.reset().
                                      // current_block = nullptr is added to replicate discard without axis_did_move reset.
                                      // Note invoking axis_did_move.reset() causes no issue since FTM's stepper refreshes
                                      // its values every ISR.
  }
}

/**
 * Set up a pseudo block to allow motion to settle and buffers to empty.
 * Called when the planner has one block left. The buffers will be filled
 * with the last commanded position by setting the startPos block variable to
 * the last position of the previous block and all ratios to zero such that no
 * axes' positions are incremented.
 */
void FTMotion::runoutBlock() {

  startPos = endPos_prevBlock;

  const int32_t n_to_fill_batch = (FTM_WINDOW_SIZE) - traj_idx_set;

  // This line or function is to be modified for FBS use; do not optimize out.
  const int32_t n_to_settle_shaper = num_samples_shaper_settle();

  const int32_t n_diff = n_to_settle_shaper - n_to_fill_batch,
  n_to_fill_batch_after_settling = n_diff > 0 ? (FTM_BATCH_SIZE) - (n_diff % (FTM_BATCH_SIZE)) : -n_diff;

  ratio.reset();
  uint32_t max_intervals = PROP_BATCHES * (FTM_BATCH_SIZE) + n_to_settle_shaper + n_to_fill_batch_after_settling;
  const float reminder_from_last_block = -tau;
  const float total_duration = max_intervals * FTM_TS + reminder_from_last_block;

  // Plan a zero-motion trajectory for runout
  currentGenerator->planRunout(total_duration);

  blockProcRdy = true; // since ratio is 0, the trajectory positions won't advance in any axis
}

// Auxiliary function to get number of step commands in the buffer.
int32_t FTMotion::stepperCmdBuffItems() {
  const int32_t udiff = stepperCmdBuff_produceIdx - stepperCmdBuff_consumeIdx;
  return (udiff < 0) ? udiff + (FTM_STEPPERCMD_BUFF_SIZE) : udiff;
}

// Initializes storage variables before startup.
void FTMotion::init() {
  update_shaping_params();
  TERN_(FTM_SMOOTHING, update_smoothing_params());
  setTrajectoryType(cfg.trajectory_type);
  reset(); // Precautionary.
}

// Set trajectory generator type
void FTMotion::setTrajectoryType(const TrajectoryType type) {
  cfg.trajectory_type = trajectoryType = type;
  switch (type) {
    default: cfg.trajectory_type = trajectoryType = TrajectoryType::FTM_TRAJECTORY_TYPE;
    case TrajectoryType::TRAPEZOIDAL: currentGenerator = &trapezoidalGenerator; break;
    case TrajectoryType::POLY5:       currentGenerator = &poly5Generator; break;
    case TrajectoryType::POLY6:       currentGenerator = &poly6Generator; break;
  }
  currentGenerator->reset(); // Reset the selected generator
}

// Load / convert block data from planner to fixed-time control variables.
// Called from FTMotion::loop() at the fetch of the next planner block.
void FTMotion::loadBlockData(block_t * const current_block) {
  // Cache the extruder index for this block
  TERN_(DISTINCT_E_FACTORS, block_extruder_axis = E_AXIS_N(current_block->extruder));

  const float totalLength = current_block->millimeters,
              oneOverLength = 1.0f / totalLength;

  startPos = endPos_prevBlock;
  const xyze_pos_t& moveDist = current_block->dist_mm;
  ratio = moveDist * oneOverLength;

  const float mmps = totalLength / current_block->step_event_count; // (mm/step) Distance for each step
  const float initial_speed = mmps * current_block->initial_rate;   // (mm/s) Start feedrate
  const float final_speed = mmps * current_block->final_rate;       // (mm/s) End feedrate

  // Plan the trajectory using the trajectory generator
  currentGenerator->plan(initial_speed, final_speed, current_block->acceleration, current_block->nominal_speed, totalLength);

  // Accel + Coasting + Decel + datapoints
  const float reminder_from_last_block = - tau;

  endPos_prevBlock += moveDist;

  TERN_(FTM_HAS_LIN_ADVANCE, use_advance_lead = current_block->use_advance_lead);

  // Watch endstops until the move ends
  const float total_duration = currentGenerator->getTotalDuration();
  uint32_t max_intervals = ceil((total_duration + reminder_from_last_block) * FTM_FS);
  const millis_t move_end_ti = millis() + SEC_TO_MS((FTM_TS) * float(max_intervals + num_samples_shaper_settle() + ((PROP_BATCHES) + 1) * (FTM_BATCH_SIZE)) + (float(FTM_STEPPERCMD_BUFF_SIZE) / float(FTM_STEPPER_FS)));

  #define _SET_MOVE_END(A) do{ \
    if (moveDist.A) { \
      axis_move_end_ti.A = move_end_ti; \
      axis_move_dir.A = moveDist.A > 0; \
    } \
  }while(0);

  LOGICAL_AXIS_MAP(_SET_MOVE_END);
}

// Generate data points of the trajectory.
// Called from FTMotion::loop() at the fetch of a new planner block, after loadBlockData.
void FTMotion::generateTrajectoryPointsFromBlock() {
  const float total_duration = currentGenerator->getTotalDuration();
  if (tau + FTM_TS > total_duration) {
    // TODO: refactor code so this thing is not twice.
    // the reason of it being in the beginning, is that a block can be so short that it has
    // zero trajectories.
    // the next iteration will fall beyond this block
    blockProcRdy = false;
    traj_idx_get = 0;
    tau -= total_duration;
    return;
  }
  do {
    tau += FTM_TS;                // (s) Time since start of block
                                  // If the end of the last block doesn't exactly land on a trajectory index,
                                  // tau can start negative, but it always holds that `tau > -FTM_TS`

    // Get distance from trajectory generator
    const float dist = currentGenerator->getDistanceAtTime(tau);

    #define _SET_TRAJ(q) traj.q[traj_idx_set] = startPos.q + ratio.q * dist;
    LOGICAL_AXIS_MAP_LC(_SET_TRAJ);

    #if FTM_HAS_LIN_ADVANCE
      if (cfg.linearAdvEna) {
        float traj_e = traj.e[traj_idx_set];
        if (use_advance_lead) {
          // Don't apply LA to retract/unretract blocks
          float e_rate = (traj_e - prev_traj_e) * (FTM_FS);
          traj.e[traj_idx_set] += e_rate * cfg.linearAdvK;
        }
        prev_traj_e = traj_e;
      }
    #endif

    // Update shaping parameters if needed.

    switch (cfg.dynFreqMode) {

      #if HAS_DYNAMIC_FREQ_MM
        case dynFreqMode_Z_BASED: {
          static float oldz = 0.0f;
          const float z = traj.z[traj_idx_set];
          if (z != oldz) { // Only update if Z changed.
            oldz = z;
            #if HAS_X_AXIS
              const float xf = cfg.baseFreq.x + cfg.dynFreqK.x * z;
              shaping.X.set_axis_shaping_N(cfg.shaper.x, _MAX(xf, FTM_MIN_SHAPE_FREQ), cfg.zeta.x);
            #endif
            #if HAS_Y_AXIS
              const float yf = cfg.baseFreq.y + cfg.dynFreqK.y * z;
              shaping.Y.set_axis_shaping_N(cfg.shaper.y, _MAX(yf, FTM_MIN_SHAPE_FREQ), cfg.zeta.y);
            #endif
          }
        } break;
      #endif

      #if HAS_DYNAMIC_FREQ_G
        case dynFreqMode_MASS_BASED:
          // Update constantly. The optimization done for Z value makes
          // less sense for E, as E is expected to constantly change.
          #if HAS_X_AXIS
            shaping.X.set_axis_shaping_N(cfg.shaper.x, cfg.baseFreq.x + cfg.dynFreqK.x * traj.e[traj_idx_set], cfg.zeta.x);
          #endif
          #if HAS_Y_AXIS
            shaping.Y.set_axis_shaping_N(cfg.shaper.y, cfg.baseFreq.y + cfg.dynFreqK.y * traj.e[traj_idx_set], cfg.zeta.y);
          #endif
          break;
      #endif

      default: break;
    }
    uint32_t max_total_delay = 0;

    #if ENABLED(FTM_SMOOTHING)
      #define _SMOOTHEN(A) /* Approximate gaussian smoothing via chained EMAs */ \
        if (smoothing.A.alpha > 0.0f) { \
          float smooth_val = traj.A[traj_idx_set]; \
          for (uint8_t _i = 0; _i < FTM_SMOOTHING_ORDER; ++_i) { \
            smoothing.A.smoothing_pass[_i] += (smooth_val - smoothing.A.smoothing_pass[_i]) * smoothing.A.alpha; \
            smooth_val = smoothing.A.smoothing_pass[_i]; \
          } \
          traj.A[traj_idx_set] = smooth_val; \
        }

      CARTES_MAP(_SMOOTHEN);
      max_total_delay += _MAX(CARTES_LIST(
        smoothing.X.delay_samples, smoothing.Y.delay_samples,
        smoothing.Z.delay_samples, smoothing.E.delay_samples
      ));

    #endif // FTM_SMOOTHING

    #if HAS_FTM_SHAPING

      if (ftMotion.cfg.axis_sync_enabled) {
        max_total_delay -= _MIN(SHAPED_LIST(
          shaping.X.Ni[0], shaping.Y.Ni[0],
          shaping.Z.Ni[0], shaping.E.Ni[0]
        ));
      }

      // Apply shaping if active on each axis
      #define _SHAPE(A) \
        do { \
          const uint32_t group_delay = ftMotion.cfg.axis_sync_enabled \
              ? max_total_delay - TERN0(FTM_SMOOTHING, smoothing.A.delay_samples) \
              : -shaping.A.Ni[0]; \
          /* α=1−exp(−(dt / (τ / order))) */ \
          shaping.A.d_zi[shaping.zi_idx] = traj.A[traj_idx_set]; \
          traj.A[traj_idx_set] = 0; \
          for (uint32_t i = 0; i <= shaping.A.max_i; i++) { \
            /* echo_delay is always positive since Ni[i] = echo_relative_delay - group_delay + max_total_delay */ \
            /* where echo_relative_delay > 0 and group_delay ≤ max_total_delay */ \
            const uint32_t echo_delay = group_delay + shaping.A.Ni[i]; \
            int32_t udiff = shaping.zi_idx - echo_delay; \
            if (udiff < 0) udiff += FTM_ZMAX; \
            traj.A[traj_idx_set] += shaping.A.Ai[i] * shaping.A.d_zi[udiff]; \
          } \
        } while (0);

      SHAPED_MAP(_SHAPE);

      if (++shaping.zi_idx == (FTM_ZMAX)) shaping.zi_idx = 0;

    #endif // HAS_FTM_SHAPING

    // Filled up the queue with regular and shaped steps
    if (++traj_idx_set == FTM_WINDOW_SIZE) {
      traj_idx_set = BATCH_SIDX_IN_WINDOW;
      batchRdy = true;
    }
    traj_idx_get++;
    if (tau + FTM_TS > total_duration) {
      // the next iteration will fall beyond this block
      blockProcRdy = false;
      traj_idx_get = 0;
      tau -= total_duration;
    }
  } while (blockProcRdy && !batchRdy);
} // generateTrajectoryPointsFromBlock

/**
 * @brief Interpolate a single trajectory data point into stepper commands.
 * @param idx The index of the trajectory point to convert.
 *
 * Calculate the required stepper movements for each axis based on the
 * difference between the current and previous trajectory points.
 * Add up to one stepper command to the buffer with STEP/DIR bits for all axes.
 */
void FTMotion::generateStepsFromTrajectory(const uint32_t idx) {
  constexpr float INV_FTM_STEPS_PER_UNIT_TIME = 1.0f / (FTM_STEPS_PER_UNIT_TIME);

  // q10 per-stepper-slot increment toward this sample’s target step count.
  // (traj * steps_per_mm - steps) = steps still due at the start of this UNIT_TIME.
  // Convert to q10 (×2^10), then subtract the current accumulator error: step_error_q10 / FTM_STEPS_PER_UNIT_TIME.
  // Over FTM_STEPS_PER_UNIT_TIME stepper-slots this sums to the exact target (no drift).
  // Any fraction of a step that may remain will be accounted for by the next UNIT_TIME
  #define TOSTEPS_q10(A, B) int32_t( \
    (trajMod.A[idx] * planner.settings.axis_steps_per_mm[B] - steps.A) * _BV(10) \
     - step_error_q10.A * INV_FTM_STEPS_PER_UNIT_TIME )

  xyze_long_t delta_q10 = LOGICAL_AXIS_ARRAY(
    TOSTEPS_q10(e, block_extruder_axis),
    TOSTEPS_q10(x, X_AXIS), TOSTEPS_q10(y, Y_AXIS), TOSTEPS_q10(z, Z_AXIS),
    TOSTEPS_q10(i, I_AXIS), TOSTEPS_q10(j, J_AXIS), TOSTEPS_q10(k, K_AXIS),
    TOSTEPS_q10(u, U_AXIS), TOSTEPS_q10(v, V_AXIS), TOSTEPS_q10(w, W_AXIS)
  );

  // Fixed-point denominator for step accumulation
  constexpr int32_t denom_q10 = (FTM_STEPS_PER_UNIT_TIME) << 10;

  // 1. Subtract one whole step from the accumulated distance
  // 2. Accumulate one positive or negative step
  // 3. Set the step and direction bits for the stepper command
  #define RUN_AXIS(A)                                       \
    do {                                                    \
      if (step_error_q10.A >= denom_q10) {                  \
        step_error_q10.A -= denom_q10;                      \
        steps.A++;                                          \
        cmd |= _BV(FT_BIT_DIR_##A) | _BV(FT_BIT_STEP_##A);  \
      }                                                     \
      if (step_error_q10.A <= -denom_q10) {                 \
        step_error_q10.A += denom_q10;                      \
        steps.A--;                                          \
        cmd |= _BV(FT_BIT_STEP_##A); /* neg dir implicit */ \
      }                                                     \
    } while (0);

  for (uint32_t i = 0; i < uint32_t(FTM_STEPS_PER_UNIT_TIME); i++) {
    // Reference the next stepper command in the circular buffer
    ft_command_t& cmd = stepperCmdBuff[stepperCmdBuff_produceIdx];

    // Init the command to no STEP (Reverse DIR)
    cmd = 0;

    // Accumulate the "error" for all axes according the fixed-point distance
    step_error_q10 += delta_q10;

    // Where the error has accumulated whole axis steps, add them to the command
    LOGICAL_AXIS_MAP(RUN_AXIS);

    // Next circular buffer index
    if (++stepperCmdBuff_produceIdx == (FTM_STEPPERCMD_BUFF_SIZE))
      stepperCmdBuff_produceIdx = 0;
  }
}

#endif // FT_MOTION
