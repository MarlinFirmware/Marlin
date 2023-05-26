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
#if HAS_X_AXIS
  float FxdTiCtrl::xd[2 * (FTM_BATCH_SIZE)],  // = {0.0f} Storage for fixed-time-based trajectory.
        FxdTiCtrl::xm[FTM_BATCH_SIZE];        // = {0.0f} Storage for modified fixed-time-based trajectory.
#endif
#if HAS_Y_AXIS
  float FxdTiCtrl::yd[2 * (FTM_BATCH_SIZE)], FxdTiCtrl::ym[FTM_BATCH_SIZE];
#endif
#if HAS_Z_AXIS
  float FxdTiCtrl::zd[2 * (FTM_BATCH_SIZE)], FxdTiCtrl::zm[FTM_BATCH_SIZE];
#endif
#if HAS_EXTRUDERS
  float FxdTiCtrl::ed[2 * (FTM_BATCH_SIZE)], FxdTiCtrl::em[FTM_BATCH_SIZE];
#endif

block_t* FxdTiCtrl::current_block_cpy = nullptr; // Pointer to current block being processed.
bool FxdTiCtrl::blockProcRdy = false,           // Indicates a block is ready to be processed.
     FxdTiCtrl::blockProcRdy_z1 = false,        // Storage for the previous indicator.
     FxdTiCtrl::blockProcDn = false;            // Indicates current block is done being processed.
bool FxdTiCtrl::batchRdy = false;               // Indicates a batch of the fixed time trajectory
                                                //  has been generated, is now available in the upper -
                                                //  half of xd, yd, zd, ed vectors, and is ready to be
                                                //  post processed, if applicable, then interpolated.
bool FxdTiCtrl::batchRdyForInterp = false;      // Indicates the batch is done being post processed,
                                                //  if applicable, and is ready to be converted to step commands.
bool FxdTiCtrl::runoutEna = false;              // True if runout of the block hasn't been done and is allowed.

// Trapezoid data variables.
#if HAS_X_AXIS
  float FxdTiCtrl::x_startPosn,                 // (mm) Start position of block
        FxdTiCtrl::x_endPosn_prevBlock = 0.0f,  // (mm) Start position of block
        FxdTiCtrl::x_Ratio;                     // (ratio) Axis move ratio of block
#endif
#if HAS_Y_AXIS
  float FxdTiCtrl::y_startPosn,
        FxdTiCtrl::y_endPosn_prevBlock = 0.0f,
        FxdTiCtrl::y_Ratio;
#endif
#if HAS_Z_AXIS
  float FxdTiCtrl::z_startPosn,
        FxdTiCtrl::z_endPosn_prevBlock = 0.0f,
        FxdTiCtrl::z_Ratio;
#endif
#if HAS_EXTRUDERS
  float FxdTiCtrl::e_startPosn,
        FxdTiCtrl::e_endPosn_prevBlock = 0.0f,
        FxdTiCtrl::e_Ratio;
#endif
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
#if HAS_X_AXIS
  int32_t FxdTiCtrl::x_steps = 0;                               // Step count accumulator.
  stepDirState_t FxdTiCtrl::x_dirState = stepDirState_NOT_SET;  // Memory of the currently set step direction of the axis.
#endif
#if HAS_Y_AXIS
  int32_t FxdTiCtrl::y_steps = 0;
  stepDirState_t FxdTiCtrl::y_dirState = stepDirState_NOT_SET;
#endif
#if HAS_Z_AXIS
  int32_t FxdTiCtrl::z_steps = 0;
  stepDirState_t FxdTiCtrl::z_dirState = stepDirState_NOT_SET;
#endif
#if HAS_EXTRUDERS
  int32_t FxdTiCtrl::e_steps = 0;
  stepDirState_t FxdTiCtrl::e_dirState = stepDirState_NOT_SET;
#endif

uint32_t FxdTiCtrl::interpIdx = 0,                    // Index of current data point being interpolated.
         FxdTiCtrl::interpIdx_z1 = 0;                 // Storage for the previously calculated index above.
hal_timer_t FxdTiCtrl::nextStepTicks = FTM_MIN_TICKS; // Accumulator for the next step time (in ticks).

// Shaping variables.
#if HAS_X_AXIS
  uint32_t FxdTiCtrl::xy_zi_idx = 0,                  // Index of storage in the data point delay vectors.
           FxdTiCtrl::xy_max_i = 0;                   // Vector length for the selected shaper.
  float FxdTiCtrl::xd_zi[FTM_ZMAX] = { 0.0f };        // Data point delay vector.
  float FxdTiCtrl::x_Ai[5];                           // Shaping gain vector.
  uint32_t FxdTiCtrl::x_Ni[5];                        // Shaping time index vector.
#endif
#if HAS_Y_AXIS
  float FxdTiCtrl::yd_zi[FTM_ZMAX] = { 0.0f };
  float FxdTiCtrl::y_Ai[5];
  uint32_t FxdTiCtrl::y_Ni[5];
#endif

#if HAS_EXTRUDERS
  // Linear advance variables.
  float FxdTiCtrl::e_raw_z1 = 0.0f;             // (ms) Unit delay of raw extruder position.
  float FxdTiCtrl::e_advanced_z1 = 0.0f;        // (ms) Unit delay of advanced extruder position.
#endif

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
    if (makeVector_batchIdx > FTM_BATCH_SIZE) {
      for (uint32_t i = makeVector_batchIdx; i < 2 * (FTM_BATCH_SIZE); i++) {
                             xd[i] = xd[makeVector_batchIdx - 1];
        TERN_(HAS_Y_AXIS,    yd[i] = yd[makeVector_batchIdx - 1]);
        TERN_(HAS_Y_AXIS,    zd[i] = zd[makeVector_batchIdx - 1]);
        TERN_(HAS_EXTRUDERS, ed[i] = ed[makeVector_batchIdx - 1]);
      }
    }
    makeVector_batchIdx = FTM_BATCH_SIZE;
    batchRdy = true;
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
  if (!blockProcRdy) stepper.fxdTiCtrl_BlockQueueUpdate();

  if (blockProcRdy) {
    if (!blockProcRdy_z1) loadBlockData(current_block_cpy); // One-shot.
    while (!blockProcDn && !batchRdy && (makeVector_idx - makeVector_idx_z1 < (FTM_POINTS_PER_LOOP)))
      makeVector();
  }

  // FBS / post processing.
  if (batchRdy && !batchRdyForInterp) {

    // Call Ulendo FBS here.

    memcpy(xm, &xd[FTM_BATCH_SIZE], sizeof(xm));
    TERN_(HAS_Y_AXIS, memcpy(ym, &yd[FTM_BATCH_SIZE], sizeof(ym)));

    // Done compensating ...

    // Copy the uncompensated vectors.
    TERN_(HAS_Z_AXIS,    memcpy(zm, &zd[FTM_BATCH_SIZE], sizeof(zm)));
    TERN_(HAS_EXTRUDERS, memcpy(em, &ed[FTM_BATCH_SIZE], sizeof(em)));

    // Shift the time series back in the window.
    memcpy(xd, &xd[FTM_BATCH_SIZE], sizeof(xd) / 2);
    TERN_(HAS_Y_AXIS, memcpy(yd, &yd[FTM_BATCH_SIZE], sizeof(yd) / 2));
    // Disabled by comment as these are uncompensated, the lower half is not used.
    //TERN_(HAS_Z_AXIS,    memcpy(zd, &zd[FTM_BATCH_SIZE], (sizeof(zd) / 2)));
    //TERN_(HAS_EXTRUDERS, memcpy(ed, &ed[FTM_BATCH_SIZE], (sizeof(ed) / 2)));

    // ... data is ready in xm, ym, zm, em.
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
  void FxdTiCtrl::updateShapingA(const_float_t zeta/*=FTM_SHAPING_ZETA*/, const_float_t vtol/*=FTM_SHAPING_V_TOL*/) {

    const float K = exp( -zeta * M_PI / sqrt(1.0f - sq(zeta)) ),
                K2 = sq(K);

    switch (cfg.mode) {

      case ftMotionMode_ZV:
        xy_max_i = 1U;
        x_Ai[0] = 1.0f / (1.0f + K);
        x_Ai[1] = x_Ai[0] * K;
        break;

      case ftMotionMode_ZVD:
        xy_max_i = 2U;
        x_Ai[0] = 1.0f / ( 1.0f + 2.0f * K + K2 );
        x_Ai[1] = x_Ai[0] * 2.0f * K;
        x_Ai[2] = x_Ai[0] * K2;
        break;

      case ftMotionMode_EI: {
        xy_max_i = 2U;
        x_Ai[0] = 0.25f * (1.0f + vtol);
        x_Ai[1] = 0.50f * (1.0f - vtol) * K;
        x_Ai[2] = x_Ai[0] * K2;
        const float A_adj = 1.0f / (x_Ai[0] + x_Ai[1] + x_Ai[2]);
        for (uint32_t i = 0U; i < 3U; i++) { x_Ai[i] *= A_adj; }
      } break;

      case ftMotionMode_2HEI: {
        xy_max_i = 3U;
        const float vtol2 = sq(vtol);
        const float X = pow(vtol2 * (sqrt(1.0f - vtol2) + 1.0f), 1.0f / 3.0f);
        x_Ai[0] = ( 3.0f * sq(X) + 2.0f * X + 3.0f * vtol2 ) / (16.0f * X);
        x_Ai[1] = ( 0.5f - x_Ai[0] ) * K;
        x_Ai[2] = x_Ai[1] * K;
        x_Ai[3] = x_Ai[0] * cu(K);
        const float A_adj = 1.0f / (x_Ai[0] + x_Ai[1] + x_Ai[2] + x_Ai[3]);
        for (uint32_t i = 0U; i < 4U; i++) { x_Ai[i] *= A_adj; }
      } break;

      case ftMotionMode_3HEI: {
        xy_max_i = 4U;
        x_Ai[0] = 0.0625f * ( 1.0f + 3.0f * vtol + 2.0f * sqrt( 2.0f * ( vtol + 1.0f ) * vtol ) );
        x_Ai[1] = 0.25f * ( 1.0f - vtol ) * K;
        x_Ai[2] = ( 0.5f * ( 1.0f + vtol ) - 2.0f * x_Ai[0] ) * K2;
        x_Ai[3] = x_Ai[1] * K2;
        x_Ai[4] = x_Ai[0] * sq(K2);
        const float A_adj = 1.0f / (x_Ai[0] + x_Ai[1] + x_Ai[2] + x_Ai[3] + x_Ai[4]);
        for (uint32_t i = 0U; i < 5U; i++) { x_Ai[i] *= A_adj; }
      } break;

      case ftMotionMode_MZV: {
        xy_max_i = 2U;
        const float B = 1.4142135623730950488016887242097f * K;
        x_Ai[0] = 1.0f / (1.0f + B + K2);
        x_Ai[1] = x_Ai[0] * B;
        x_Ai[2] = x_Ai[0] * K2;
      } break;

      default:
        for (uint32_t i = 0U; i < 5U; i++) x_Ai[i] = 0.0f;
        xy_max_i = 0;
    }
    #if HAS_Y_AXIS
      memcpy(y_Ai, x_Ai, sizeof(x_Ai)); // For now, zeta and vtol are shared across x and y.
    #endif
  }

  // Refresh the indices used by shaping functions.
  // To be called when frequencies change.
  void FxdTiCtrl::updateShapingN(const_float_t xf OPTARG(HAS_Y_AXIS, const_float_t yf), const_float_t zeta/*=FTM_SHAPING_ZETA*/) {

    // Protections omitted for DBZ and for index exceeding array length.

    const float df = sqrt(1.0f - sq(zeta));

    switch (cfg.mode) {
      case ftMotionMode_ZV:
        x_Ni[1] = round((0.5f / xf / df) * (FTM_FS));
        #if HAS_Y_AXIS
          y_Ni[1] = round((0.5f / yf / df) * (FTM_FS));
        #endif
        break;
      case ftMotionMode_ZVD:
      case ftMotionMode_EI:
        x_Ni[1] = round((0.5f / xf / df) * (FTM_FS));
        x_Ni[2] = 2 * x_Ni[1];
        #if HAS_Y_AXIS
          y_Ni[1] = round((0.5f / yf / df) * (FTM_FS));
          y_Ni[2] = 2 * y_Ni[1];
        #endif
        break;
      case ftMotionMode_2HEI:
        x_Ni[1] = round((0.5f / xf / df) * (FTM_FS));
        x_Ni[2] = 2 * x_Ni[1];
        x_Ni[3] = 3 * x_Ni[1];
        #if HAS_Y_AXIS
          y_Ni[1] = round((0.5f / yf / df) * (FTM_FS));
          y_Ni[2] = 2 * y_Ni[1];
          y_Ni[3] = 3 * y_Ni[1];
        #endif
        break;
      case ftMotionMode_3HEI:
        x_Ni[1] = round((0.5f / xf / df) * (FTM_FS));
        x_Ni[2] = 2 * x_Ni[1];
        x_Ni[3] = 3 * x_Ni[1];
        x_Ni[4] = 4 * x_Ni[1];
        #if HAS_Y_AXIS
          y_Ni[1] = round((0.5f / yf / df) * (FTM_FS));
          y_Ni[2] = 2 * y_Ni[1];
          y_Ni[3] = 3 * y_Ni[1];
          y_Ni[4] = 4 * y_Ni[1];
        #endif
        break;
      case ftMotionMode_MZV:
        x_Ni[1] = round((0.375f / xf / df) * (FTM_FS));
        x_Ni[2] = 2 * x_Ni[1];
        #if HAS_Y_AXIS
          y_Ni[1] = round((0.375f / yf / df) * (FTM_FS));
          y_Ni[2] = 2 * y_Ni[1];
        #endif
        break;
      default:
        for (uint32_t i = 0U; i < 5U; i++) { x_Ni[i] = 0; TERN_(HAS_Y_AXIS, y_Ni[i] = 0); }
    }
  }

#endif // HAS_X_AXIS

// Reset all trajectory processing variables.
void FxdTiCtrl::reset() {

  stepperCmdBuff_produceIdx = stepperCmdBuff_consumeIdx = 0;

  for (uint32_t i = 0U; i < (FTM_BATCH_SIZE); i++) { // Reset trajectory history
    TERN_(HAS_X_AXIS,    xd[i] = 0.0f);
    TERN_(HAS_Y_AXIS,    yd[i] = 0.0f);
    TERN_(HAS_Z_AXIS,    zd[i] = 0.0f);
    TERN_(HAS_EXTRUDERS, ed[i] = 0.0f);
  }

  blockProcRdy = blockProcRdy_z1 = blockProcDn = false;
  batchRdy = batchRdyForInterp = false;
  runoutEna = false;

  TERN_(HAS_X_AXIS,    x_endPosn_prevBlock = 0.0f);
  TERN_(HAS_Y_AXIS,    y_endPosn_prevBlock = 0.0f);
  TERN_(HAS_Z_AXIS,    z_endPosn_prevBlock = 0.0f);
  TERN_(HAS_EXTRUDERS, e_endPosn_prevBlock = 0.0f);

  makeVector_idx = makeVector_idx_z1 = 0;
  makeVector_batchIdx = FTM_BATCH_SIZE;

  TERN_(HAS_X_AXIS,    x_steps = 0);
  TERN_(HAS_Y_AXIS,    y_steps = 0);
  TERN_(HAS_Z_AXIS,    z_steps = 0);
  TERN_(HAS_EXTRUDERS, e_steps = 0);

  interpIdx = interpIdx_z1 = 0;

  TERN_(HAS_X_AXIS,    x_dirState = stepDirState_NOT_SET);
  TERN_(HAS_Y_AXIS,    y_dirState = stepDirState_NOT_SET);
  TERN_(HAS_Z_AXIS,    z_dirState = stepDirState_NOT_SET);
  TERN_(HAS_EXTRUDERS, e_dirState = stepDirState_NOT_SET);

  nextStepTicks = FTM_MIN_TICKS;

  #if HAS_X_AXIS
    for (uint32_t i = 0U; i < (FTM_ZMAX); i++) { xd_zi[i] = 0.0f; TERN_(HAS_Y_AXIS, yd_zi[i] = 0.0f); }
    xy_zi_idx = 0;
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

  #if HAS_X_AXIS
    x_startPosn = x_endPosn_prevBlock;
    float x_moveDist = current_block->steps.a / planner.settings.axis_steps_per_mm[X_AXIS];
    if (!direction.x) x_moveDist *= -1.0f;
    x_Ratio = x_moveDist * oneOverLength;
  #endif

  #if HAS_Y_AXIS
    y_startPosn = y_endPosn_prevBlock;
    float y_moveDist = current_block->steps.b / planner.settings.axis_steps_per_mm[Y_AXIS];
    if (!direction.y) y_moveDist *= -1.0f;
    y_Ratio = y_moveDist * oneOverLength;
  #endif

  #if HAS_Z_AXIS
    z_startPosn = z_endPosn_prevBlock;
    float z_moveDist = current_block->steps.c / planner.settings.axis_steps_per_mm[Z_AXIS];
    if (!direction.z) z_moveDist *= -1.0f;
    z_Ratio = z_moveDist * oneOverLength;
  #endif

  #if HAS_EXTRUDERS
    e_startPosn = e_endPosn_prevBlock;
    float extrusion = current_block->steps.e / planner.settings.axis_steps_per_mm[E_AXIS_N(current_block->extruder)];
    if (!direction.e) extrusion *= -1.0f;
    e_Ratio = extrusion * oneOverLength;
  #endif

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
  if (T2 < 0.0f)  {
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

  TERN_(HAS_X_AXIS,    x_endPosn_prevBlock += x_moveDist);
  TERN_(HAS_Y_AXIS,    y_endPosn_prevBlock += y_moveDist);
  TERN_(HAS_Z_AXIS,    z_endPosn_prevBlock += z_moveDist);
  TERN_(HAS_EXTRUDERS, e_endPosn_prevBlock += extrusion);
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

  TERN_(HAS_X_AXIS, xd[makeVector_batchIdx] = x_startPosn + x_Ratio * dist);  // (mm) X position for this datapoint
  TERN_(HAS_Y_AXIS, yd[makeVector_batchIdx] = y_startPosn + y_Ratio * dist);  // (mm) Y
  TERN_(HAS_Z_AXIS, zd[makeVector_batchIdx] = z_startPosn + z_Ratio * dist);  // (mm) Z

  #if HAS_EXTRUDERS
    const float new_raw_z1 = e_startPosn + e_Ratio * dist;
    if (cfg.linearAdvEna) {
      float dedt_adj = (new_raw_z1 - e_raw_z1) * (FTM_FS);
      if (e_Ratio > 0.0f) dedt_adj += accel_k * cfg.linearAdvK;

      e_advanced_z1 += dedt_adj * (FTM_TS);
      ed[makeVector_batchIdx] = e_advanced_z1;

      e_raw_z1 = new_raw_z1;
    }
    else {
      ed[makeVector_batchIdx] = new_raw_z1;
      // Alternatively: ed[makeVector_batchIdx] = e_startPosn + (e_Ratio * dist) / (N1 + N2 + N3);
    }
  #endif

  // Update shaping parameters if needed.
  #if HAS_DYNAMIC_FREQ_MM
    static float zd_z1 = 0.0f;
  #endif
  switch (cfg.dynFreqMode) {

    #if HAS_DYNAMIC_FREQ_MM
      case dynFreqMode_Z_BASED:
        if (zd[makeVector_batchIdx] != zd_z1) { // Only update if Z changed.
          const float xf = cfg.baseFreq[X_AXIS] + cfg.dynFreqK[X_AXIS] * zd[makeVector_batchIdx],
                      yf = cfg.baseFreq[Y_AXIS] + cfg.dynFreqK[Y_AXIS] * zd[makeVector_batchIdx];
          updateShapingN(_MAX(xf, FTM_MIN_SHAPE_FREQ), _MAX(yf, FTM_MIN_SHAPE_FREQ));
          zd_z1 = zd[makeVector_batchIdx];
        }
        break;
    #endif

    #if HAS_DYNAMIC_FREQ_G
      case dynFreqMode_MASS_BASED:
        // Update constantly. The optimization done for Z value makes
        // less sense for E, as E is expected to constantly change.
        updateShapingN(      cfg.baseFreq[X_AXIS] + cfg.dynFreqK[X_AXIS] * ed[makeVector_batchIdx]
          OPTARG(HAS_Y_AXIS, cfg.baseFreq[Y_AXIS] + cfg.dynFreqK[Y_AXIS] * ed[makeVector_batchIdx]) );
        break;
    #endif

    default: break;
  }

  // Apply shaping if in mode.
  #if HAS_X_AXIS
    if (WITHIN(cfg.mode, 10U, 19U)) {
      xd_zi[xy_zi_idx] = xd[makeVector_batchIdx];
      xd[makeVector_batchIdx] *= x_Ai[0];
      #if HAS_Y_AXIS
        yd_zi[xy_zi_idx] = yd[makeVector_batchIdx];
        yd[makeVector_batchIdx] *= y_Ai[0];
      #endif
      for (uint32_t i = 1U; i <= xy_max_i; i++) {
        const uint32_t udiffx = xy_zi_idx - x_Ni[i];
        xd[makeVector_batchIdx] += x_Ai[i] * xd_zi[x_Ni[i] > xy_zi_idx ? (FTM_ZMAX) + udiffx : udiffx];
        #if HAS_Y_AXIS
          const uint32_t udiffy = xy_zi_idx - y_Ni[i];
          yd[makeVector_batchIdx] += y_Ai[i] * yd_zi[y_Ni[i] > xy_zi_idx ? (FTM_ZMAX) + udiffy : udiffy];
        #endif
      }
      if (++xy_zi_idx == (FTM_ZMAX)) xy_zi_idx = 0;
    }
  #endif

  // Filled up the queue with regular and shaped steps
  if (++makeVector_batchIdx == 2 * (FTM_BATCH_SIZE)) {
    makeVector_batchIdx = FTM_BATCH_SIZE;
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
  #if HAS_X_AXIS
    int32_t x_err_P = 0;
  #endif
  #if HAS_Y_AXIS
    int32_t y_err_P = 0;
  #endif
  #if HAS_Z_AXIS
    int32_t z_err_P = 0;
  #endif
  #if HAS_EXTRUDERS
    int32_t e_err_P = 0;
  #endif

  //#define STEPS_ROUNDING
  #if ENABLED(STEPS_ROUNDING)
    #if HAS_X_AXIS
      const float x_steps_tar = xm[idx] * planner.settings.axis_steps_per_mm[X_AXIS] + (xm[idx] < 0.0f ? -0.5f : 0.5f); // May be eliminated if guaranteed positive.
      const int32_t x_delta = int32_t(x_steps_tar) - x_steps;
    #endif
    #if HAS_Y_AXIS
      const float y_steps_tar = ym[idx] * planner.settings.axis_steps_per_mm[Y_AXIS] + (ym[idx] < 0.0f ? -0.5f : 0.5f);
      const int32_t y_delta = int32_t(y_steps_tar) - y_steps;
    #endif
    #if HAS_Z_AXIS
      const float z_steps_tar = zm[idx] * planner.settings.axis_steps_per_mm[Z_AXIS] + (zm[idx] < 0.0f ? -0.5f : 0.5f);
      const int32_t z_delta = int32_t(z_steps_tar) - z_steps;
    #endif
    #if HAS_EXTRUDERS
      const float e_steps_tar = em[idx] * planner.settings.axis_steps_per_mm[E_AXIS] + (em[idx] < 0.0f ? -0.5f : 0.5f);
      const int32_t e_delta = int32_t(e_steps_tar) - e_steps;
    #endif
  #else
    #if HAS_X_AXIS
      const int32_t x_delta = int32_t(xm[idx] * planner.settings.axis_steps_per_mm[X_AXIS]) - x_steps;
    #endif
    #if HAS_Y_AXIS
      const int32_t y_delta = int32_t(ym[idx] * planner.settings.axis_steps_per_mm[Y_AXIS]) - y_steps;
    #endif
    #if HAS_Z_AXIS
      const int32_t z_delta = int32_t(zm[idx] * planner.settings.axis_steps_per_mm[Z_AXIS]) - z_steps;
    #endif
    #if HAS_EXTRUDERS
      const int32_t e_delta = int32_t(em[idx] * planner.settings.axis_steps_per_mm[E_AXIS]) - e_steps;
    #endif
  #endif

  bool any_dirChange = (false
    || TERN0(HAS_X_AXIS,    (x_delta > 0 && x_dirState != stepDirState_POS) || (x_delta < 0 && x_dirState != stepDirState_NEG))
    || TERN0(HAS_Y_AXIS,    (y_delta > 0 && y_dirState != stepDirState_POS) || (y_delta < 0 && y_dirState != stepDirState_NEG))
    || TERN0(HAS_Z_AXIS,    (z_delta > 0 && z_dirState != stepDirState_POS) || (z_delta < 0 && z_dirState != stepDirState_NEG))
    || TERN0(HAS_EXTRUDERS, (e_delta > 0 && e_dirState != stepDirState_POS) || (e_delta < 0 && e_dirState != stepDirState_NEG))
  );

  for (uint32_t i = 0U; i < (FTM_STEPS_PER_UNIT_TIME); i++) {

    // TODO: (?) Since the *delta variables will not change,
    // the comparison may be done once before iterating at
    // expense of storage and lines of code.

    bool anyStep = false;

    stepperCmdBuff[stepperCmdBuff_produceIdx] = 0;

    // Commands are written in the format:
    // |X_step|X_direction|Y_step|Y_direction|Z_step|Z_direction|E_step|E_direction|
    #if HAS_X_AXIS
      if (x_delta >= 0) {
        if ((x_err_P + x_delta) < (FTM_CTS_COMPARE_VAL)) {
          x_err_P += x_delta;
        }
        else {
          x_steps++;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_X) | _BV(FT_BIT_STEP_X);
          x_err_P += x_delta - (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
      else {
        if ((x_err_P + x_delta) > -(FTM_CTS_COMPARE_VAL)) {
          x_err_P += x_delta;
        }
        else {
          x_steps--;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_STEP_X);
          x_err_P += x_delta + (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
    #endif // HAS_X_AXIS

    #if HAS_Y_AXIS
      if (y_delta >= 0) {
        if ((y_err_P + y_delta) < (FTM_CTS_COMPARE_VAL)) {
          y_err_P += y_delta;
        }
        else {
          y_steps++;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_Y) | _BV(FT_BIT_STEP_Y);
          y_err_P += y_delta - (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
      else {
        if ((y_err_P + y_delta) > -(FTM_CTS_COMPARE_VAL)) {
          y_err_P += y_delta;
        }
        else {
          y_steps--;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_STEP_Y);
          y_err_P += y_delta + (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
    #endif // HAS_Y_AXIS

    #if HAS_Z_AXIS
      if (z_delta >= 0) {
        if ((z_err_P + z_delta) < (FTM_CTS_COMPARE_VAL)) {
          z_err_P += z_delta;
        }
        else {
          z_steps++;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_Z) | _BV(FT_BIT_STEP_Z);
          z_err_P += z_delta - (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
      else {
        if ((z_err_P + z_delta) > -(FTM_CTS_COMPARE_VAL)) {
          z_err_P += z_delta;
        }
        else {
          z_steps--;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_STEP_Z);
          z_err_P += z_delta + (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
    #endif // HAS_Z_AXIS

    #if HAS_EXTRUDERS
      if (e_delta >= 0) {
        if ((e_err_P + e_delta) < (FTM_CTS_COMPARE_VAL)) {
          e_err_P += e_delta;
        }
        else {
          e_steps++;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_E) | _BV(FT_BIT_STEP_E);
          e_err_P += e_delta - (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
      else {
        if ((e_err_P + e_delta) > -(FTM_CTS_COMPARE_VAL)) {
          e_err_P += e_delta;
        }
        else {
          e_steps--;
          stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_STEP_E);
          e_err_P += e_delta + (FTM_STEPS_PER_UNIT_TIME);
          anyStep = true;
        }
      }
    #endif // HAS_EXTRUDERS

    if (!anyStep) {
      nextStepTicks += (FTM_MIN_TICKS);
    }
    else {
      stepperCmdBuff_StepRelativeTi[stepperCmdBuff_produceIdx] = nextStepTicks;

      const uint8_t dir_index = stepperCmdBuff_produceIdx >> 3,
                    dir_bit = stepperCmdBuff_produceIdx & 0x7;
      if (any_dirChange) {
        SBI(stepperCmdBuff_ApplyDir[dir_index], dir_bit);
        #if HAS_X_AXIS
          if (x_delta > 0) {
            stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_X);
            x_dirState = stepDirState_POS;
          }
          else {
            x_dirState = stepDirState_NEG;
          }
        #endif

        #if HAS_Y_AXIS
          if (y_delta > 0) {
            stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_Y);
            y_dirState = stepDirState_POS;
          }
          else {
            y_dirState = stepDirState_NEG;
          }
        #endif

        #if HAS_Z_AXIS
          if (z_delta > 0) {
            stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_Z);
            z_dirState = stepDirState_POS;
          }
          else {
            z_dirState = stepDirState_NEG;
          }
        #endif

        #if HAS_EXTRUDERS
          if (e_delta > 0) {
            stepperCmdBuff[stepperCmdBuff_produceIdx] |= _BV(FT_BIT_DIR_E);
            e_dirState = stepDirState_POS;
          }
          else {
            e_dirState = stepDirState_NEG;
          }
        #endif

        any_dirChange = false;
      }
      else { // ...no direction change.
        CBI(stepperCmdBuff_ApplyDir[dir_index], dir_bit);
      }

      if (stepperCmdBuff_produceIdx == (FTM_STEPPERCMD_BUFF_SIZE) - 1) {
        stepperCmdBuff_produceIdx = 0;
      }
      else {
        stepperCmdBuff_produceIdx++;
      }

      nextStepTicks = FTM_MIN_TICKS;
    }
  } // FTM_STEPS_PER_UNIT_TIME loop
}

#endif // FT_MOTION
