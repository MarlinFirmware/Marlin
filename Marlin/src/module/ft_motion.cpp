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
#include "stepper.h" // Access stepper block queue function and abort status.

FxdTiCtrl fxdTiCtrl;

#if !HAS_X_AXIS
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_ZV, "ftMotionMode_ZV requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_ZVD, "ftMotionMode_ZVD requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_EI, "ftMotionMode_EI requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_2HEI, "ftMotionMode_2HEI requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_3HEI, "ftMotionMode_3HEI requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_MZV, "ftMotionMode_MZV requires at least one linear axis.");
#endif
#if !HAS_DYNAMIC_FREQ_MM
  static_assert(FTM_DEFAULT_DYNFREQ_MODE != dynFreqMode_Z_BASED, "dynFreqMode_Z_BASED requires a Z axis.");
#endif
#if !HAS_DYNAMIC_FREQ_G
  static_assert(FTM_DEFAULT_DYNFREQ_MODE != dynFreqMode_MASS_BASED, "dynFreqMode_MASS_BASED requires an X axis and an extruder.");
#endif

//-----------------------------------------------------------------//
// Variables.
//-----------------------------------------------------------------//

// Public variables.

ft_config_t FxdTiCtrl::cfg;
ft_command_t FxdTiCtrl::stepperCmdBuff[FTM_STEPPERCMD_BUFF_SIZE] = {0U};                // Buffer of stepper commands.
hal_timer_t FxdTiCtrl::stepperCmdBuff_StepRelativeTi[FTM_STEPPERCMD_BUFF_SIZE] = {0U};  // Buffer of the stepper command timing.
uint8_t FxdTiCtrl::stepperCmdBuff_ApplyDir[FTM_STEPPERCMD_DIR_SIZE] = {0U};             // Buffer of whether DIR needs to be updated.
uint32_t FxdTiCtrl::stepperCmdBuff_produceIdx = 0,  // Index of next stepper command write to the buffer.
         FxdTiCtrl::stepperCmdBuff_consumeIdx = 0;  // Index of next stepper command read from the buffer.

bool FxdTiCtrl::sts_stepperBusy = false;          // The stepper buffer has items and is in use.

// Private variables.
// NOTE: These are sized for Ulendo FBS use.
xyze_trajectory_t FxdTiCtrl::traj;                // = {0.0f} Storage for fixed-time-based trajectory.
xyze_trajectoryMod_t FxdTiCtrl::trajMod;          // = {0.0f} Storage for modified fixed-time-based trajectory.

block_t* FxdTiCtrl::current_block_cpy = nullptr;  // Pointer to current block being processed.
bool FxdTiCtrl::blockProcRdy = false,             // Indicates a block is ready to be processed.
     FxdTiCtrl::blockProcRdy_z1 = false,          // Storage for the previous indicator.
     FxdTiCtrl::blockProcDn = false;              // Indicates current block is done being processed.
bool FxdTiCtrl::batchRdy = false;                 // Indicates a batch of the fixed time trajectory
                                                  //  has been generated, is now available in the upper -
                                                  //  half of traj.x[], y, z ... e vectors, and is ready to be
                                                  //  post processed, if applicable, then interpolated.
bool FxdTiCtrl::batchRdyForInterp = false;        // Indicates the batch is done being post processed,
                                                  //  if applicable, and is ready to be converted to step commands.
bool FxdTiCtrl::runoutEna = false;                // True if runout of the block hasn't been done and is allowed.
bool FxdTiCtrl::runout = false;                   // Indicates if runout is in progress.

// Trapezoid data variables.
xyze_pos_t   FxdTiCtrl::startPosn,                    // (mm) Start position of block
             FxdTiCtrl::endPosn_prevBlock = { 0.0f }; // (mm) End position of previous block
xyze_float_t FxdTiCtrl::ratio;                        // (ratio) Axis move ratio of block
float FxdTiCtrl::accel_P,                       // Acceleration prime of block. [mm/sec/sec]
      FxdTiCtrl::decel_P,                       // Deceleration prime of block. [mm/sec/sec]
      FxdTiCtrl::F_P,                           // Feedrate prime of block. [mm/sec]
      FxdTiCtrl::f_s,                           // Starting feedrate of block. [mm/sec]
      FxdTiCtrl::s_1e,                          // Position after acceleration phase of block.
      FxdTiCtrl::s_2e;                          // Position after acceleration and coasting phase of block.

uint32_t FxdTiCtrl::N1,                         // Number of data points in the acceleration phase.
         FxdTiCtrl::N2,                         // Number of data points in the coasting phase.
         FxdTiCtrl::N3;                         // Number of data points in the deceleration phase.

uint32_t FxdTiCtrl::max_intervals;              // Total number of data points that will be generated from block.

// Make vector variables.
uint32_t FxdTiCtrl::makeVector_idx = 0,                     // Index of fixed time trajectory generation of the overall block.
         FxdTiCtrl::makeVector_idx_z1 = 0,                  // Storage for the previously calculated index above.
         FxdTiCtrl::makeVector_batchIdx = FTM_BATCH_SIZE;   // Index of fixed time trajectory generation within the batch.

// Interpolation variables.
xyze_long_t FxdTiCtrl::steps = { 0 };                                            // Step count accumulator.
xyze_stepDir_t FxdTiCtrl::dirState = LOGICAL_AXIS_ARRAY_1(stepDirState_NOT_SET); // Memory of the currently set step direction of the axis.

uint32_t FxdTiCtrl::interpIdx = 0,                    // Index of current data point being interpolated.
         FxdTiCtrl::interpIdx_z1 = 0;                 // Storage for the previously calculated index above.
hal_timer_t FxdTiCtrl::nextStepTicks = FTM_MIN_TICKS; // Accumulator for the next step time (in ticks).

// Shaping variables.
#if HAS_X_AXIS
  FxdTiCtrl::shaping_t FxdTiCtrl::shaping = {
    0, 0,
    x:{ { 0.0f }, { 0.0f }, { 0 } },                  // d_zi, Ai, Ni
    #if HAS_Y_AXIS
      y:{ { 0.0f }, { 0.0f }, { 0 } }                 // d_zi, Ai, Ni
    #endif
  };
#endif

#if HAS_EXTRUDERS
  // Linear advance variables.
  float FxdTiCtrl::e_raw_z1 = 0.0f;             // (ms) Unit delay of raw extruder position.
  float FxdTiCtrl::e_advanced_z1 = 0.0f;        // (ms) Unit delay of advanced extruder position.
#endif

constexpr uint32_t last_batchIdx = (FTM_WINDOW_SIZE) - (FTM_BATCH_SIZE);

//-----------------------------------------------------------------//
// Function definitions.
//-----------------------------------------------------------------//

// Public functions.

// Sets controller states to begin processing a block.
void FxdTiCtrl::startBlockProc(block_t * const current_block) {
  current_block_cpy = current_block;
  blockProcRdy = true;
  blockProcDn = false;
  runoutEna = true;
}

// Moves any free data points to the stepper buffer even if a full batch isn't ready.
void FxdTiCtrl::runoutBlock() {

  if (runoutEna && !batchRdy) {   // If the window is full already (block intervals was a multiple of
                                  // the batch size), or runout is not enabled, no runout is needed.
    // Fill out the trajectory window with the last position calculated.
    if (makeVector_batchIdx > last_batchIdx)
      for (uint32_t i = makeVector_batchIdx; i < (FTM_WINDOW_SIZE); i++) {
        LOGICAL_AXIS_CODE(
          traj.e[i] = traj.e[makeVector_batchIdx - 1],
          traj.x[i] = traj.x[makeVector_batchIdx - 1],
          traj.y[i] = traj.y[makeVector_batchIdx - 1],
          traj.z[i] = traj.z[makeVector_batchIdx - 1],
          traj.i[i] = traj.i[makeVector_batchIdx - 1],
          traj.j[i] = traj.j[makeVector_batchIdx - 1],
          traj.k[i] = traj.k[makeVector_batchIdx - 1],
          traj.u[i] = traj.u[makeVector_batchIdx - 1],
          traj.v[i] = traj.v[makeVector_batchIdx - 1],
          traj.w[i] = traj.w[makeVector_batchIdx - 1]
        );
      }

    makeVector_batchIdx = last_batchIdx;
    batchRdy = true;
    runout = true;
  }
  runoutEna = false;
}

// Controller main, to be invoked from non-isr task.
void FxdTiCtrl::loop() {

  if (!cfg.mode) return;

  // Handle block abort with the following sequence:
  // 1. Zero out commands in stepper ISR.
  // 2. Drain the motion buffer, stop processing until they are emptied.
  // 3. Reset all the states / memory.
  // 4. Signal ready for new block.
  if (stepper.abort_current_block) {
    if (sts_stepperBusy) return;          // Wait until motion buffers are emptied
    reset();
    blockProcDn = true;                   // Set queueing to look for next block.
    runoutEna = false;                    // Disabling running out this block, since we want to halt the motion.
    stepper.abort_current_block = false;  // Abort finished.
  }

  // Planner processing and block conversion.
  if (!blockProcRdy && !runout) stepper.fxdTiCtrl_BlockQueueUpdate();

  if (blockProcRdy) {
    if (!blockProcRdy_z1) loadBlockData(current_block_cpy); // One-shot.
    while (!blockProcDn && !batchRdy && (makeVector_idx - makeVector_idx_z1 < (FTM_POINTS_PER_LOOP)))
      makeVector();
  }

  if (runout && !batchRdy) { // The lower half of the window has been runout.
    // Runout the upper half of the window: the upper half has been shifted into the lower
    // half. Fill out the upper half so another batch can be processed.
    for (uint32_t i = last_batchIdx; i < (FTM_WINDOW_SIZE) - 1; i++) {
      LOGICAL_AXIS_CODE(
        traj.e[i] = traj.e[(FTM_WINDOW_SIZE) - 1],
        traj.x[i] = traj.x[(FTM_WINDOW_SIZE) - 1],
        traj.y[i] = traj.y[(FTM_WINDOW_SIZE) - 1],
        traj.z[i] = traj.z[(FTM_WINDOW_SIZE) - 1],
        traj.i[i] = traj.i[(FTM_WINDOW_SIZE) - 1],
        traj.j[i] = traj.j[(FTM_WINDOW_SIZE) - 1],
        traj.k[i] = traj.k[(FTM_WINDOW_SIZE) - 1],
        traj.u[i] = traj.u[(FTM_WINDOW_SIZE) - 1],
        traj.v[i] = traj.v[(FTM_WINDOW_SIZE) - 1],
        traj.w[i] = traj.w[(FTM_WINDOW_SIZE) - 1]
      );
    }
    batchRdy = true;
    runout = false;
  }

  // FBS / post processing.
  if (batchRdy && !batchRdyForInterp) {

    // Call Ulendo FBS here.

    // Copy the uncompensated vectors. (XY done, other axes uncompensated)
    LOGICAL_AXIS_CODE(
      memcpy(trajMod.e, &traj.e[FTM_BATCH_SIZE], sizeof(trajMod.e)),
      memcpy(trajMod.x, &traj.x[FTM_BATCH_SIZE], sizeof(trajMod.x)),
      memcpy(trajMod.y, &traj.y[FTM_BATCH_SIZE], sizeof(trajMod.y)),
      memcpy(trajMod.z, &traj.z[FTM_BATCH_SIZE], sizeof(trajMod.z)),
      memcpy(trajMod.i, &traj.i[FTM_BATCH_SIZE], sizeof(trajMod.i)),
      memcpy(trajMod.j, &traj.j[FTM_BATCH_SIZE], sizeof(trajMod.j)),
      memcpy(trajMod.k, &traj.k[FTM_BATCH_SIZE], sizeof(trajMod.k)),
      memcpy(trajMod.u, &traj.u[FTM_BATCH_SIZE], sizeof(trajMod.u)),
      memcpy(trajMod.v, &traj.v[FTM_BATCH_SIZE], sizeof(trajMod.v)),
      memcpy(trajMod.w, &traj.w[FTM_BATCH_SIZE], sizeof(trajMod.w))
    );

    // Shift the time series back in the window for (shaped) X and Y
    TERN_(HAS_X_AXIS, memcpy(traj.x, &traj.x[FTM_BATCH_SIZE], sizeof(traj.x) / 2));
    TERN_(HAS_Y_AXIS, memcpy(traj.y, &traj.y[FTM_BATCH_SIZE], sizeof(traj.y) / 2));

    // Z...W and E Disabled! Uncompensated so the lower half is not used.
    //TERN_(HAS_Z_AXIS, memcpy(&traj.z[0], &traj.z[FTM_BATCH_SIZE], sizeof(traj.z) / 2));

    // ... data is ready in trajMod.
    batchRdyForInterp = true;

    batchRdy = false; // Clear so that makeVector() may resume generating points.

  } // if (batchRdy && !batchRdyForInterp)

  // Interpolation.
  while ( batchRdyForInterp
          && ( stepperCmdBuffItems() < ((FTM_STEPPERCMD_BUFF_SIZE) - (FTM_STEPS_PER_UNIT_TIME)) )
          && ( (interpIdx - interpIdx_z1) < (FTM_STEPS_PER_LOOP) )
  ) {
    convertToSteps(interpIdx);

    if (++interpIdx == FTM_BATCH_SIZE) {
      batchRdyForInterp = false;
      interpIdx = 0;
    }
  }

  // Report busy status to planner.
  planner.fxdTiCtrl_busy = (sts_stepperBusy || ((!blockProcDn && blockProcRdy) || batchRdy || batchRdyForInterp || runoutEna));

  blockProcRdy_z1 = blockProcRdy;
  makeVector_idx_z1 = makeVector_idx;
  interpIdx_z1 = interpIdx;
}

#if HAS_X_AXIS

  // Refresh the gains used by shaping functions.
  // To be called on init or mode or zeta change.

  void FxdTiCtrl::Shaping::updateShapingA(const_float_t zeta/*=FTM_SHAPING_ZETA*/, const_float_t vtol/*=FTM_SHAPING_V_TOL*/) {

    const float K = exp(-zeta * M_PI / sqrt(1.0f - sq(zeta))),
                K2 = sq(K);

    switch (cfg.mode) {

      case ftMotionMode_ZV:
        max_i = 1U;
        x.Ai[0] = 1.0f / (1.0f + K);
        x.Ai[1] = x.Ai[0] * K;
        break;

      case ftMotionMode_ZVD:
        max_i = 2U;
        x.Ai[0] = 1.0f / ( 1.0f + 2.0f * K + K2 );
        x.Ai[1] = x.Ai[0] * 2.0f * K;
        x.Ai[2] = x.Ai[0] * K2;
        break;

      case ftMotionMode_EI: {
        max_i = 2U;
        x.Ai[0] = 0.25f * (1.0f + vtol);
        x.Ai[1] = 0.50f * (1.0f - vtol) * K;
        x.Ai[2] = x.Ai[0] * K2;
        const float A_adj = 1.0f / (x.Ai[0] + x.Ai[1] + x.Ai[2]);
        for (uint32_t i = 0U; i < 3U; i++) { x.Ai[i] *= A_adj; }
      } break;

      case ftMotionMode_2HEI: {
        max_i = 3U;
        const float vtol2 = sq(vtol);
        const float X = pow(vtol2 * (sqrt(1.0f - vtol2) + 1.0f), 1.0f / 3.0f);
        x.Ai[0] = ( 3.0f * sq(X) + 2.0f * X + 3.0f * vtol2 ) / (16.0f * X);
        x.Ai[1] = ( 0.5f - x.Ai[0] ) * K;
        x.Ai[2] = x.Ai[1] * K;
        x.Ai[3] = x.Ai[0] * cu(K);
        const float A_adj = 1.0f / (x.Ai[0] + x.Ai[1] + x.Ai[2] + x.Ai[3]);
        for (uint32_t i = 0U; i < 4U; i++) { x.Ai[i] *= A_adj; }
      } break;

      case ftMotionMode_3HEI: {
        max_i = 4U;
        x.Ai[0] = 0.0625f * ( 1.0f + 3.0f * vtol + 2.0f * sqrt( 2.0f * ( vtol + 1.0f ) * vtol ) );
        x.Ai[1] = 0.25f * ( 1.0f - vtol ) * K;
        x.Ai[2] = ( 0.5f * ( 1.0f + vtol ) - 2.0f * x.Ai[0] ) * K2;
        x.Ai[3] = x.Ai[1] * K2;
        x.Ai[4] = x.Ai[0] * sq(K2);
        const float A_adj = 1.0f / (x.Ai[0] + x.Ai[1] + x.Ai[2] + x.Ai[3] + x.Ai[4]);
        for (uint32_t i = 0U; i < 5U; i++) { x.Ai[i] *= A_adj; }
      } break;

      case ftMotionMode_MZV: {
        max_i = 2U;
        const float B = 1.4142135623730950488016887242097f * K;
        x.Ai[0] = 1.0f / (1.0f + B + K2);
        x.Ai[1] = x.Ai[0] * B;
        x.Ai[2] = x.Ai[0] * K2;
      } break;

      default:
        for (uint32_t i = 0U; i < 5U; i++) x.Ai[i] = 0.0f;
        max_i = 0;
    }
    #if HAS_Y_AXIS
      memcpy(y.Ai, x.Ai, sizeof(x.Ai)); // For now, zeta and vtol are shared across x and y.
    #endif
  }

  void FxdTiCtrl::updateShapingA(const_float_t zeta/*=FTM_SHAPING_ZETA*/, const_float_t vtol/*=FTM_SHAPING_V_TOL*/) {
    shaping.updateShapingA(zeta, vtol);
  }

  // Refresh the indices used by shaping functions.
  // To be called when frequencies change.

  void FxdTiCtrl::AxisShaping::updateShapingN(const_float_t f, const_float_t df) {
    // Protections omitted for DBZ and for index exceeding array length.
    switch (cfg.mode) {
      case ftMotionMode_ZV:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        break;
      case ftMotionMode_ZVD:
      case ftMotionMode_EI:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        break;
      case ftMotionMode_2HEI:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        Ni[3] = Ni[2] + Ni[1];
        break;
      case ftMotionMode_3HEI:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        Ni[3] = Ni[2] + Ni[1];
        Ni[4] = Ni[3] + Ni[1];
        break;
      case ftMotionMode_MZV:
        Ni[1] = round((0.375f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        break;
      default: ZERO(Ni);
    }
  }

  void FxdTiCtrl::updateShapingN(const_float_t xf OPTARG(HAS_Y_AXIS, const_float_t yf), const_float_t zeta/*=FTM_SHAPING_ZETA*/) {
    const float df = sqrt(1.0f - sq(zeta));
    shaping.x.updateShapingN(xf, df);
    TERN_(HAS_Y_AXIS, shaping.y.updateShapingN(yf, df));
  }

#endif // HAS_X_AXIS

// Reset all trajectory processing variables.
void FxdTiCtrl::reset() {

  stepperCmdBuff_produceIdx = stepperCmdBuff_consumeIdx = 0;

  traj.reset(); // Reset trajectory history
  trajMod.reset(); // Reset modified trajectory history

  blockProcRdy = blockProcRdy_z1 = blockProcDn = false;
  batchRdy = batchRdyForInterp = false;
  runoutEna = false;
  runout = false;

  endPosn_prevBlock.reset();

  makeVector_idx = makeVector_idx_z1 = 0;
  makeVector_batchIdx = FTM_BATCH_SIZE;

  steps.reset();
  interpIdx = interpIdx_z1 = 0;
  dirState = LOGICAL_AXIS_ARRAY_1(stepDirState_NOT_SET);
  nextStepTicks = FTM_MIN_TICKS;

  #if HAS_X_AXIS
    for (uint32_t i = 0U; i < (FTM_ZMAX); i++)
      shaping.x.d_zi[i] = TERN_(HAS_Y_AXIS, shaping.y.d_zi[i] =) 0.0f;
    shaping.zi_idx = 0;
  #endif

  TERN_(HAS_EXTRUDERS, e_raw_z1 = e_advanced_z1 = 0.0f);
}

// Private functions.
// Auxiliary function to get number of step commands in the buffer.
uint32_t FxdTiCtrl::stepperCmdBuffItems() {
  const uint32_t udiff = stepperCmdBuff_produceIdx - stepperCmdBuff_consumeIdx;
  return stepperCmdBuff_produceIdx < stepperCmdBuff_consumeIdx ? (FTM_STEPPERCMD_BUFF_SIZE) + udiff : udiff;
}

// Initializes storage variables before startup.
void FxdTiCtrl::init() {
  #if HAS_X_AXIS
    refreshShapingN();
    updateShapingA();
  #endif
  reset(); // Precautionary.
}

// Loads / converts block data from planner to fixed-time control variables.
void FxdTiCtrl::loadBlockData(block_t * const current_block) {

  const float totalLength = current_block->millimeters,
              oneOverLength = 1.0f / totalLength;

  const AxisBits direction = current_block->direction_bits;

  startPosn = endPosn_prevBlock;
  xyze_pos_t moveDist = LOGICAL_AXIS_ARRAY(
    current_block->steps.e / planner.settings.axis_steps_per_mm[E_AXIS_N(current_block->extruder)],
    current_block->steps.x / planner.settings.axis_steps_per_mm[X_AXIS],
    current_block->steps.y / planner.settings.axis_steps_per_mm[Y_AXIS],
    current_block->steps.z / planner.settings.axis_steps_per_mm[Z_AXIS],
    current_block->steps.i / planner.settings.axis_steps_per_mm[I_AXIS],
    current_block->steps.j / planner.settings.axis_steps_per_mm[J_AXIS],
    current_block->steps.k / planner.settings.axis_steps_per_mm[K_AXIS],
    current_block->steps.u / planner.settings.axis_steps_per_mm[U_AXIS],
    current_block->steps.v / planner.settings.axis_steps_per_mm[V_AXIS],
    current_block->steps.w / planner.settings.axis_steps_per_mm[W_AXIS]
  );

  LOGICAL_AXIS_CODE(
    if (!direction.e) moveDist.e *= -1.0f,
    if (!direction.x) moveDist.x *= -1.0f,
    if (!direction.y) moveDist.y *= -1.0f,
    if (!direction.z) moveDist.z *= -1.0f,
    if (!direction.i) moveDist.i *= -1.0f,
    if (!direction.j) moveDist.j *= -1.0f,
    if (!direction.k) moveDist.k *= -1.0f,
    if (!direction.u) moveDist.u *= -1.0f,
    if (!direction.v) moveDist.v *= -1.0f,
    if (!direction.w) moveDist.w *= -1.0f
  );

  ratio = moveDist * oneOverLength;

  const float spm = totalLength / current_block->step_event_count;  // (steps/mm) Distance for each step
              f_s = spm * current_block->initial_rate;  // (steps/s) Start feedrate
  const float f_e = spm * current_block->final_rate;    // (steps/s) End feedrate

  const float a = current_block->acceleration,          // (mm/s^2) Same magnitude for acceleration or deceleration
              oneby2a = 1.0f / (2.0f * a),              // (s/mm) Time to accelerate or decelerate one mm (i.e., oneby2a * 2
              oneby2d = -oneby2a;                       // (s/mm) Time to accelerate or decelerate one mm (i.e., oneby2a * 2
  const float fsSqByTwoA = sq(f_s) * oneby2a,           // (mm) Distance to accelerate from start speed to nominal speed
              feSqByTwoD = sq(f_e) * oneby2d;           // (mm) Distance to decelerate from nominal speed to end speed

  float F_n = current_block->nominal_speed;             // (mm/s) Speed we hope to achieve, if possible
  const float fdiff = feSqByTwoD - fsSqByTwoA,          // (mm) Coasting distance if nominal speed is reached
              odiff = oneby2a - oneby2d,                // (i.e., oneby2a * 2) (mm/s) Change in speed for one second of acceleration
              ldiff = totalLength - fdiff;              // (mm) Distance to travel if nominal speed is reached
  float T2 = (1.0f / F_n) * (ldiff - odiff * sq(F_n));  // (s) Coasting duration after nominal speed reached
  if (T2 < 0.0f) {
    T2 = 0.0f;
    F_n = SQRT(ldiff / odiff);                          // Clip by intersection if nominal speed can't be reached.
  }

  const float T1 = (F_n - f_s) / a,                     // (s) Accel Time = difference in feedrate over acceleration
              T3 = (F_n - f_e) / a;                     // (s) Decel Time = difference in feedrate over acceleration

  N1 = ceil(T1 * (FTM_FS));                       // Accel datapoints based on Hz frequency
  N2 = ceil(T2 * (FTM_FS));                       // Coast
  N3 = ceil(T3 * (FTM_FS));                       // Decel

  const float T1_P = N1 * (FTM_TS),               // (s) Accel datapoints x timestep resolution
              T2_P = N2 * (FTM_TS),               // (s) Coast
              T3_P = N3 * (FTM_TS);               // (s) Decel

  // Calculate the reachable feedrate at the end of the accel phase
  // totalLength is the total distance to travel in mm
  // f_s is the starting feedrate in mm/s
  // f_e is the ending feedrate in mm/s
  // T1_P is the time spent accelerating in seconds
  // T2_P is the time spent coasting in seconds
  // T3_P is the time spent decelerating in seconds
  // f_s * T1_P is the distance traveled during the accel phase
  // f_e * T3_P is the distance traveled during the decel phase
  //
  F_P = (2.0f * totalLength - f_s * T1_P - f_e * T3_P) / (T1_P + 2.0f * T2_P + T3_P); // (mm/s) Feedrate at the end of the accel phase

  // Calculate the acceleration and deceleration rates
  accel_P = N1 ? ((F_P - f_s) / T1_P) : 0.0f;

  decel_P = (f_e - F_P) / T3_P;

  // Calculate the distance traveled during the accel phase
  s_1e = f_s * T1_P + 0.5f * accel_P * sq(T1_P);

  // Calculate the distance traveled during the decel phase
  s_2e = s_1e + F_P * T2_P;

  // One less than (Accel + Coasting + Decel) datapoints
  max_intervals = N1 + N2 + N3 - 1U;

  endPosn_prevBlock += moveDist;
}

// Generate data points of the trajectory.
void FxdTiCtrl::makeVector() {
  float accel_k = 0.0f;                                   // (mm/s^2) Acceleration K factor
  float tau = (makeVector_idx + 1) * (FTM_TS);            // (s) Time since start of block
  float dist = 0.0f;                                      // (mm) Distance traveled

  if (makeVector_idx < N1) {
    // Acceleration phase
    dist = (f_s * tau) + (0.5f * accel_P * sq(tau));      // (mm) Distance traveled for acceleration phase
    accel_k = accel_P;                                    // (mm/s^2) Acceleration K factor from Accel phase
  }
  else if (makeVector_idx >= N1 && makeVector_idx < (N1 + N2)) {
    // Coasting phase
    dist = s_1e + F_P * (tau - N1 * (FTM_TS));            // (mm) Distance traveled for coasting phase
    //accel_k = 0.0f;
  }
  else {
    // Deceleration phase
    const float tau_ = tau - (N1 + N2) * (FTM_TS);        // (s) Time since start of decel phase
    dist = s_2e + F_P * tau_ + 0.5f * decel_P * sq(tau_); // (mm) Distance traveled for deceleration phase
    accel_k = decel_P;                                    // (mm/s^2) Acceleration K factor from Decel phase
  }

  NUM_AXIS_CODE(
    traj.x[makeVector_batchIdx] = startPosn.x + ratio.x * dist,
    traj.y[makeVector_batchIdx] = startPosn.y + ratio.y * dist,
    traj.z[makeVector_batchIdx] = startPosn.z + ratio.z * dist,
    traj.i[makeVector_batchIdx] = startPosn.i + ratio.i * dist,
    traj.j[makeVector_batchIdx] = startPosn.j + ratio.j * dist,
    traj.k[makeVector_batchIdx] = startPosn.k + ratio.k * dist,
    traj.u[makeVector_batchIdx] = startPosn.u + ratio.u * dist,
    traj.v[makeVector_batchIdx] = startPosn.v + ratio.v * dist,
    traj.w[makeVector_batchIdx] = startPosn.w + ratio.w * dist
  );

  #if HAS_EXTRUDERS
    const float new_raw_z1 = startPosn.e + ratio.e * dist;
    if (cfg.linearAdvEna) {
      float dedt_adj = (new_raw_z1 - e_raw_z1) * (FTM_FS);
      if (ratio.e > 0.0f) dedt_adj += accel_k * cfg.linearAdvK;

      e_advanced_z1 += dedt_adj * (FTM_TS);
      traj.e[makeVector_batchIdx] = e_advanced_z1;

      e_raw_z1 = new_raw_z1;
    }
    else {
      traj.e[makeVector_batchIdx] = new_raw_z1;
      // Alternatively: ed[makeVector_batchIdx] = startPosn.e + (ratio.e * dist) / (N1 + N2 + N3);
    }
  #endif

  // Update shaping parameters if needed.
  #if HAS_DYNAMIC_FREQ_MM
    static float zd_z1 = 0.0f;
  #endif
  switch (cfg.dynFreqMode) {

    #if HAS_DYNAMIC_FREQ_MM
      case dynFreqMode_Z_BASED:
        if (traj.z[makeVector_batchIdx] != zd_z1) { // Only update if Z changed.
          const float xf = cfg.baseFreq[X_AXIS] + cfg.dynFreqK[X_AXIS] * traj.z[makeVector_batchIdx],
                      yf = cfg.baseFreq[Y_AXIS] + cfg.dynFreqK[Y_AXIS] * traj.z[makeVector_batchIdx];
          updateShapingN(_MAX(xf, FTM_MIN_SHAPE_FREQ), _MAX(yf, FTM_MIN_SHAPE_FREQ));
          zd_z1 = traj.z[makeVector_batchIdx];
        }
        break;
    #endif

    #if HAS_DYNAMIC_FREQ_G
      case dynFreqMode_MASS_BASED:
        // Update constantly. The optimization done for Z value makes
        // less sense for E, as E is expected to constantly change.
        updateShapingN(      cfg.baseFreq[X_AXIS] + cfg.dynFreqK[X_AXIS] * traj.e[makeVector_batchIdx]
          OPTARG(HAS_Y_AXIS, cfg.baseFreq[Y_AXIS] + cfg.dynFreqK[Y_AXIS] * traj.e[makeVector_batchIdx]) );
        break;
    #endif

    default: break;
  }

  // Apply shaping if in mode.
  #if HAS_X_AXIS
    if (cfg.modeHasShaper()) {
      shaping.x.d_zi[shaping.zi_idx] = traj.x[makeVector_batchIdx];
      traj.x[makeVector_batchIdx] *= shaping.x.Ai[0];
      #if HAS_Y_AXIS
        shaping.y.d_zi[shaping.zi_idx] = traj.y[makeVector_batchIdx];
        traj.y[makeVector_batchIdx] *= shaping.y.Ai[0];
      #endif
      for (uint32_t i = 1U; i <= shaping.max_i; i++) {
        const uint32_t udiffx = shaping.zi_idx - shaping.x.Ni[i];
        traj.x[makeVector_batchIdx] += shaping.x.Ai[i] * shaping.x.d_zi[shaping.x.Ni[i] > shaping.zi_idx ? (FTM_ZMAX) + udiffx : udiffx];
        #if HAS_Y_AXIS
          const uint32_t udiffy = shaping.zi_idx - shaping.y.Ni[i];
          traj.y[makeVector_batchIdx] += shaping.y.Ai[i] * shaping.y.d_zi[shaping.y.Ni[i] > shaping.zi_idx ? (FTM_ZMAX) + udiffy : udiffy];
        #endif
      }
      if (++shaping.zi_idx == (FTM_ZMAX)) shaping.zi_idx = 0;
    }
  #endif

  // Filled up the queue with regular and shaped steps
  if (++makeVector_batchIdx == (FTM_WINDOW_SIZE)) {
    makeVector_batchIdx = last_batchIdx;
    batchRdy = true;
  }

  if (makeVector_idx == max_intervals) {
    blockProcDn = true;
    blockProcRdy = false;
    makeVector_idx = 0;
  }
  else
    makeVector_idx++;
}

// Interpolates single data point to stepper commands.
void FxdTiCtrl::convertToSteps(const uint32_t idx) {
  xyze_long_t err_P = { 0 };

  //#define STEPS_ROUNDING
  #if ENABLED(STEPS_ROUNDING)
    const xyze_float_t steps_tar = LOGICAL_AXIS_ARRAY(
      trajMod.e[idx] * planner.settings.axis_steps_per_mm[E_AXIS_N(current_block->extruder)] + (trajMod.e[idx] < 0.0f ? -0.5f : 0.5f), // May be eliminated if guaranteed positive.
      trajMod.x[idx] * planner.settings.axis_steps_per_mm[X_AXIS] + (trajMod.x[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.y[idx] * planner.settings.axis_steps_per_mm[Y_AXIS] + (trajMod.y[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.z[idx] * planner.settings.axis_steps_per_mm[Z_AXIS] + (trajMod.z[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.i[idx] * planner.settings.axis_steps_per_mm[I_AXIS] + (trajMod.i[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.j[idx] * planner.settings.axis_steps_per_mm[J_AXIS] + (trajMod.j[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.k[idx] * planner.settings.axis_steps_per_mm[K_AXIS] + (trajMod.k[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.u[idx] * planner.settings.axis_steps_per_mm[U_AXIS] + (trajMod.u[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.v[idx] * planner.settings.axis_steps_per_mm[V_AXIS] + (trajMod.v[idx] < 0.0f ? -0.5f : 0.5f),
      trajMod.w[idx] * planner.settings.axis_steps_per_mm[W_AXIS] + (trajMod.w[idx] < 0.0f ? -0.5f : 0.5f),
    );
    xyze_long_t delta = xyze_long_t(steps_tar) - steps;
    //const xyze_long_t delta = LOGICAL_AXIS_ARRAY(
    //  int32_t(steps_tar.e) - steps.e,
    //  int32_t(steps_tar.x) - steps.x,
    //  int32_t(steps_tar.y) - steps.y,
    //  int32_t(steps_tar.z) - steps.z,
    //  int32_t(steps_tar.i) - steps.i,
    //  int32_t(steps_tar.j) - steps.j,
    //  int32_t(steps_tar.k) - steps.k,
    //  int32_t(steps_tar.u) - steps.u,
    //  int32_t(steps_tar.v) - steps.v,
    //  int32_t(steps_tar.w) - steps.w
    //);
  #else
    xyze_long_t delta = LOGICAL_AXIS_ARRAY(
      int32_t(trajMod.e[idx] * planner.settings.axis_steps_per_mm[E_AXIS_N(current_block->extruder)]) - steps.e,
      int32_t(trajMod.x[idx] * planner.settings.axis_steps_per_mm[X_AXIS]) - steps.x,
      int32_t(trajMod.y[idx] * planner.settings.axis_steps_per_mm[Y_AXIS]) - steps.y,
      int32_t(trajMod.z[idx] * planner.settings.axis_steps_per_mm[Z_AXIS]) - steps.z,
      int32_t(trajMod.i[idx] * planner.settings.axis_steps_per_mm[I_AXIS]) - steps.i,
      int32_t(trajMod.j[idx] * planner.settings.axis_steps_per_mm[J_AXIS]) - steps.j,
      int32_t(trajMod.k[idx] * planner.settings.axis_steps_per_mm[K_AXIS]) - steps.k,
      int32_t(trajMod.u[idx] * planner.settings.axis_steps_per_mm[U_AXIS]) - steps.u,
      int32_t(trajMod.v[idx] * planner.settings.axis_steps_per_mm[V_AXIS]) - steps.v,
      int32_t(trajMod.w[idx] * planner.settings.axis_steps_per_mm[W_AXIS]) - steps.w
    );
  #endif

  bool any_dirChange = (false
    LOGICAL_AXIS_GANG(
      || (delta.e > 0 && dirState.e != stepDirState_POS) || (delta.e < 0 && dirState.e != stepDirState_NEG),
      || (delta.x > 0 && dirState.x != stepDirState_POS) || (delta.x < 0 && dirState.x != stepDirState_NEG),
      || (delta.y > 0 && dirState.y != stepDirState_POS) || (delta.y < 0 && dirState.y != stepDirState_NEG),
      || (delta.z > 0 && dirState.z != stepDirState_POS) || (delta.z < 0 && dirState.z != stepDirState_NEG),
      || (delta.i > 0 && dirState.i != stepDirState_POS) || (delta.i < 0 && dirState.i != stepDirState_NEG),
      || (delta.j > 0 && dirState.j != stepDirState_POS) || (delta.j < 0 && dirState.j != stepDirState_NEG),
      || (delta.k > 0 && dirState.k != stepDirState_POS) || (delta.k < 0 && dirState.k != stepDirState_NEG),
      || (delta.u > 0 && dirState.u != stepDirState_POS) || (delta.u < 0 && dirState.u != stepDirState_NEG),
      || (delta.v > 0 && dirState.v != stepDirState_POS) || (delta.v < 0 && dirState.v != stepDirState_NEG),
      || (delta.w > 0 && dirState.w != stepDirState_POS) || (delta.w < 0 && dirState.w != stepDirState_NEG)
    )
  );

  for (uint32_t i = 0U; i < (FTM_STEPS_PER_UNIT_TIME); i++) {

    // TODO: (?) Since the *delta variables will not change,
    // the comparison may be done once before iterating at
    // expense of storage and lines of code.

    bool anyStep = false;

    // Commands are written in a bitmask with step and dir as single bits
    auto COMMAND_SET = [&](auto &d, auto &e, auto &s, auto &b, auto bd, auto bs) {
      if (d >= 0) {
        if (e + d < (FTM_CTS_COMPARE_VAL)) {
          e += d;
        }
        else {
          s++;
          b |= bd | bs;
          e += d - (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
      else {
        if ((e + d) > -(FTM_CTS_COMPARE_VAL)) {
          e += d;
        }
        else {
          s--;
          b |= bs;
          e += d + (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
    };

    // Init all step/dir bits to 0 (defaulting to reverse/negative motion)
    stepperCmdBuff[stepperCmdBuff_produceIdx] = 0;

    // Set up step/dir bits for all axes
    LOGICAL_AXIS_CODE(
      COMMAND_SET(delta.e, err_P.e, steps.e, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_E), _BV(FT_BIT_STEP_E)),
      COMMAND_SET(delta.x, err_P.x, steps.x, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_X), _BV(FT_BIT_STEP_X)),
      COMMAND_SET(delta.y, err_P.y, steps.y, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_Y), _BV(FT_BIT_STEP_Y)),
      COMMAND_SET(delta.z, err_P.z, steps.z, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_Z), _BV(FT_BIT_STEP_Z)),
      COMMAND_SET(delta.i, err_P.i, steps.i, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_I), _BV(FT_BIT_STEP_I)),
      COMMAND_SET(delta.j, err_P.j, steps.j, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_J), _BV(FT_BIT_STEP_J)),
      COMMAND_SET(delta.k, err_P.k, steps.k, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_K), _BV(FT_BIT_STEP_K)),
      COMMAND_SET(delta.u, err_P.u, steps.u, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_U), _BV(FT_BIT_STEP_U)),
      COMMAND_SET(delta.v, err_P.v, steps.v, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_V), _BV(FT_BIT_STEP_V)),
      COMMAND_SET(delta.w, err_P.w, steps.w, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_W), _BV(FT_BIT_STEP_W)),
    );

    if (!anyStep) {
      nextStepTicks += (FTM_MIN_TICKS);
    }
    else {
      stepperCmdBuff_StepRelativeTi[stepperCmdBuff_produceIdx] = nextStepTicks;

      const uint8_t dir_index = stepperCmdBuff_produceIdx >> 3,
                    dir_bit = stepperCmdBuff_produceIdx & 0x7;
      if (any_dirChange) {

        SBI(stepperCmdBuff_ApplyDir[dir_index], dir_bit);

        auto DIR_SET = [&](auto &d, auto &c, auto &b, auto bd) {
          if (d > 0) { b |= bd; c = stepDirState_POS; } else { c = stepDirState_NEG; }
        };

        LOGICAL_AXIS_CODE(
          DIR_SET(delta.e, dirState.e, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_E)),
          DIR_SET(delta.x, dirState.x, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_X)),
          DIR_SET(delta.y, dirState.y, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_Y)),
          DIR_SET(delta.z, dirState.z, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_Z)),
          DIR_SET(delta.i, dirState.i, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_I)),
          DIR_SET(delta.j, dirState.j, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_J)),
          DIR_SET(delta.k, dirState.k, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_K)),
          DIR_SET(delta.u, dirState.u, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_U)),
          DIR_SET(delta.v, dirState.v, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_V)),
          DIR_SET(delta.w, dirState.w, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_W)),
        );

        any_dirChange = false;
      }
      else { // ...no direction change.
        CBI(stepperCmdBuff_ApplyDir[dir_index], dir_bit);
      }

      if (stepperCmdBuff_produceIdx == (FTM_STEPPERCMD_BUFF_SIZE) - 1)
        stepperCmdBuff_produceIdx = 0;
      else
        stepperCmdBuff_produceIdx++;

      nextStepTicks = FTM_MIN_TICKS;
    }
  } // FTM_STEPS_PER_UNIT_TIME loop
}

#endif // FT_MOTION
