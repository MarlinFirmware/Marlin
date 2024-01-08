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

FTMotion ftMotion;

#if !HAS_X_AXIS
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_ZV, "ftMotionMode_ZV requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_ZVD, "ftMotionMode_ZVD requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_ZVDD, "ftMotionMode_ZVD requires at least one linear axis.");
  static_assert(FTM_DEFAULT_MODE == ftMotionMode_ZVDDD, "ftMotionMode_ZVD requires at least one linear axis.");
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

//-----------------------------------------------------------------
// Variables.
//-----------------------------------------------------------------

// Public variables.

ft_config_t FTMotion::cfg;
bool FTMotion::busy; // = false
ft_command_t FTMotion::stepperCmdBuff[FTM_STEPPERCMD_BUFF_SIZE] = {0U}; // Stepper commands buffer.
int32_t FTMotion::stepperCmdBuff_produceIdx = 0, // Index of next stepper command write to the buffer.
        FTMotion::stepperCmdBuff_consumeIdx = 0; // Index of next stepper command read from the buffer.

bool FTMotion::sts_stepperBusy = false;         // The stepper buffer has items and is in use.

// Private variables.

// NOTE: These are sized for Ulendo FBS use.
xyze_trajectory_t    FTMotion::traj;            // = {0.0f} Storage for fixed-time-based trajectory.
xyze_trajectoryMod_t FTMotion::trajMod;         // = {0.0f} Storage for fixed time trajectory window.

bool FTMotion::blockProcRdy = false,            // Indicates a block is ready to be processed.
     FTMotion::blockProcRdy_z1 = false,         // Storage for the previous indicator.
     FTMotion::blockProcDn = false;             // Indicates current block is done being processed.
bool FTMotion::batchRdy = false;                // Indicates a batch of the fixed time trajectory
                                                //  has been generated, is now available in the upper -
                                                //  half of traj.x[], y, z ... e vectors, and is ready to be
                                                //  post processed, if applicable, then interpolated.
bool FTMotion::batchRdyForInterp = false;       // Indicates the batch is done being post processed,
                                                //  if applicable, and is ready to be converted to step commands.
bool FTMotion::runoutEna = false;               // True if runout of the block hasn't been done and is allowed.
bool FTMotion::blockDataIsRunout = false;       // Indicates the last loaded block variables are for a runout.

// Trapezoid data variables.
xyze_pos_t   FTMotion::startPosn,                     // (mm) Start position of block
             FTMotion::endPosn_prevBlock = { 0.0f };  // (mm) End position of previous block
xyze_float_t FTMotion::ratio;                         // (ratio) Axis move ratio of block
float FTMotion::accel_P,                        // Acceleration prime of block. [mm/sec/sec]
      FTMotion::decel_P,                        // Deceleration prime of block. [mm/sec/sec]
      FTMotion::F_P,                            // Feedrate prime of block. [mm/sec]
      FTMotion::f_s,                            // Starting feedrate of block. [mm/sec]
      FTMotion::s_1e,                           // Position after acceleration phase of block.
      FTMotion::s_2e;                           // Position after acceleration and coasting phase of block.

uint32_t FTMotion::N1,                          // Number of data points in the acceleration phase.
         FTMotion::N2,                          // Number of data points in the coasting phase.
         FTMotion::N3;                          // Number of data points in the deceleration phase.

uint32_t FTMotion::max_intervals;               // Total number of data points that will be generated from block.

// Make vector variables.
uint32_t FTMotion::makeVector_idx = 0,          // Index of fixed time trajectory generation of the overall block.
         FTMotion::makeVector_idx_z1 = 0,       // Storage for the previously calculated index above.
         FTMotion::makeVector_batchIdx = 0;     // Index of fixed time trajectory generation within the batch.

// Interpolation variables.
xyze_long_t FTMotion::steps = { 0 };            // Step count accumulator.

uint32_t FTMotion::interpIdx = 0,               // Index of current data point being interpolated.
         FTMotion::interpIdx_z1 = 0;            // Storage for the previously calculated index above.

// Shaping variables.
#if HAS_X_AXIS
  FTMotion::shaping_t FTMotion::shaping = {
    0, 0,
    x:{ { 0.0f }, { 0.0f }, { 0 } },            // d_zi, Ai, Ni
    #if HAS_Y_AXIS
      y:{ { 0.0f }, { 0.0f }, { 0 } }           // d_zi, Ai, Ni
    #endif
  };
#endif

#if HAS_EXTRUDERS
  // Linear advance variables.
  float FTMotion::e_raw_z1 = 0.0f;        // (ms) Unit delay of raw extruder position.
  float FTMotion::e_advanced_z1 = 0.0f;   // (ms) Unit delay of advanced extruder position.
#endif

constexpr uint32_t last_batchIdx = (FTM_WINDOW_SIZE) - (FTM_BATCH_SIZE);

//-----------------------------------------------------------------
// Function definitions.
//-----------------------------------------------------------------

// Public functions.

// Sets controller states to begin processing a block.
void FTMotion::startBlockProc() {
  blockProcRdy = true;
  blockProcDn = false;
  runoutEna = true;
}

// Move any free data points to the stepper buffer even if a full batch isn't ready.
void FTMotion::runoutBlock() {

  if (!runoutEna) return;

  startPosn = endPosn_prevBlock;
  ratio.reset();

  max_intervals = cfg.modeHasShaper() ? shaper_intervals : 0;
  if (max_intervals <= TERN(FTM_UNIFIED_BWS, FTM_BATCH_SIZE, min_max_intervals - (FTM_BATCH_SIZE)))
    max_intervals = min_max_intervals;

  max_intervals += (
    #if ENABLED(FTM_UNIFIED_BWS)
      FTM_WINDOW_SIZE - makeVector_batchIdx
    #else
      FTM_WINDOW_SIZE - ((last_batchIdx < (FTM_BATCH_SIZE)) ? 0 : makeVector_batchIdx)
    #endif
  );
  blockProcRdy = blockDataIsRunout = true;
  runoutEna = blockProcDn = false;
}

// Controller main, to be invoked from non-isr task.
void FTMotion::loop() {

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
    stepper.abort_current_block = false;  // Abort finished.
  }

  // Planner processing and block conversion.
  if (!blockProcRdy) stepper.ftMotion_blockQueueUpdate();

  if (blockProcRdy) {
    if (!blockProcRdy_z1) { // One-shot.
      if (!blockDataIsRunout) loadBlockData(stepper.current_block);
      else blockDataIsRunout = false;
    }
    while (!blockProcDn && !batchRdy && (makeVector_idx - makeVector_idx_z1 < (FTM_POINTS_PER_LOOP)))
      makeVector();
  }

  // FBS / post processing.
  if (batchRdy && !batchRdyForInterp) {

    // Call Ulendo FBS here.

    #if ENABLED(FTM_UNIFIED_BWS)
      trajMod = traj; // Move the window to traj
    #else
      // Copy the uncompensated vectors.
      #define TCOPY(A) memcpy(trajMod.A, traj.A, sizeof(trajMod.A))
      LOGICAL_AXIS_CODE(
        TCOPY(e),
        TCOPY(x), TCOPY(y), TCOPY(z),
        TCOPY(i), TCOPY(j), TCOPY(k),
        TCOPY(u), TCOPY(v), TCOPY(w)
      );

      // Shift the time series back in the window
      #define TSHIFT(A) memcpy(traj.A, &traj.A[FTM_BATCH_SIZE], last_batchIdx * sizeof(traj.A[0]))
      LOGICAL_AXIS_CODE(
        TSHIFT(e),
        TSHIFT(x), TSHIFT(y), TSHIFT(z),
        TSHIFT(i), TSHIFT(j), TSHIFT(k),
        TSHIFT(u), TSHIFT(v), TSHIFT(w)
      );
    #endif

    // ... data is ready in trajMod.
    batchRdyForInterp = true;

    batchRdy = false; // Clear so makeVector() can resume generating points.
  }

  // Interpolation.
  while (batchRdyForInterp
    && (stepperCmdBuffItems() < (FTM_STEPPERCMD_BUFF_SIZE) - (FTM_STEPS_PER_UNIT_TIME))
    && (interpIdx - interpIdx_z1 < (FTM_STEPS_PER_LOOP))
  ) {
    convertToSteps(interpIdx);
    if (++interpIdx == FTM_BATCH_SIZE) {
      batchRdyForInterp = false;
      interpIdx = 0;
    }
  }

  // Report busy status to planner.
  busy = (sts_stepperBusy || ((!blockProcDn && blockProcRdy) || batchRdy || batchRdyForInterp || runoutEna));

  blockProcRdy_z1 = blockProcRdy;
  makeVector_idx_z1 = makeVector_idx;
  interpIdx_z1 = interpIdx;
}

#if HAS_X_AXIS

  // Refresh the gains used by shaping functions.
  // To be called on init or mode or zeta change.

  void FTMotion::Shaping::updateShapingA(float zeta[]/*=cfg.zeta*/, float vtol[]/*=cfg.vtol*/) {

    const float Kx = exp(-zeta[0] * M_PI / sqrt(1.0f - sq(zeta[0]))),
                Ky = exp(-zeta[1] * M_PI / sqrt(1.0f - sq(zeta[1]))),
                Kx2 = sq(Kx),
                Ky2 = sq(Ky);

    switch (cfg.mode) {

      case ftMotionMode_ZV:
        max_i = 1U;
        x.Ai[0] = 1.0f / (1.0f + Kx);
        x.Ai[1] = x.Ai[0] * Kx;

        y.Ai[0] = 1.0f / (1.0f + Ky);
        y.Ai[1] = y.Ai[0] * Ky;
        break;

      case ftMotionMode_ZVD:
        max_i = 2U;
        x.Ai[0] = 1.0f / (1.0f + 2.0f * Kx + Kx2);
        x.Ai[1] = x.Ai[0] * 2.0f * Kx;
        x.Ai[2] = x.Ai[0] * Kx2;

        y.Ai[0] = 1.0f / (1.0f + 2.0f * Ky + Ky2);
        y.Ai[1] = y.Ai[0] * 2.0f * Ky;
        y.Ai[2] = y.Ai[0] * Ky2;
        break;

      case ftMotionMode_ZVDD:
        max_i = 3U;
        x.Ai[0] = 1.0f / (1.0f + 3.0f * Kx + 3.0f * Kx2 + cu(Kx));
        x.Ai[1] = x.Ai[0] * 3.0f * Kx;
        x.Ai[2] = x.Ai[0] * 3.0f * Kx2;
        x.Ai[3] = x.Ai[0] * cu(Kx);

        y.Ai[0] = 1.0f / (1.0f + 3.0f * Ky + 3.0f * Ky2 + cu(Ky));
        y.Ai[1] = y.Ai[0] * 3.0f * Ky;
        y.Ai[2] = y.Ai[0] * 3.0f * Ky2;
        y.Ai[3] = y.Ai[0] * cu(Ky);
        break;

      case ftMotionMode_ZVDDD:
        max_i = 4U;
        x.Ai[0] = 1.0f / (1.0f + 4.0f * Kx + 6.0f * Kx2 + 4.0f * cu(Kx) + sq(Kx2));
        x.Ai[1] = x.Ai[0] * 4.0f * Kx;
        x.Ai[2] = x.Ai[0] * 6.0f * Kx2;
        x.Ai[3] = x.Ai[0] * 4.0f * cu(Kx);
        x.Ai[4] = x.Ai[0] * sq(Kx2);

        y.Ai[0] = 1.0f / (1.0f + 4.0f * Ky + 6.0f * Ky2 + 4.0f * cu(Ky) + sq(Ky2));
        y.Ai[1] = y.Ai[0] * 4.0f * Ky;
        y.Ai[2] = y.Ai[0] * 6.0f * Ky2;
        y.Ai[3] = y.Ai[0] * 4.0f * cu(Ky);
        y.Ai[4] = y.Ai[0] * sq(Ky2);
        break;

      case ftMotionMode_EI: {
        max_i = 2U;
        x.Ai[0] = 0.25f * (1.0f + vtol[0]);
        x.Ai[1] = 0.50f * (1.0f - vtol[0]) * Kx;
        x.Ai[2] = x.Ai[0] * Kx2;

        y.Ai[0] = 0.25f * (1.0f + vtol[1]);
        y.Ai[1] = 0.50f * (1.0f - vtol[1]) * Ky;
        y.Ai[2] = y.Ai[0] * Ky2;

        const float X_adj = 1.0f / (x.Ai[0] + x.Ai[1] + x.Ai[2]);
        const float Y_adj = 1.0f / (y.Ai[0] + y.Ai[1] + y.Ai[2]);
        for (uint32_t i = 0U; i < 3U; i++) {
          x.Ai[i] *= X_adj;
          y.Ai[i] *= Y_adj;
        }
      }
      break;

      case ftMotionMode_2HEI: {
        max_i = 3U;
        const float vtolx2 = sq(vtol[0]);
        const float X = pow(vtolx2 * (sqrt(1.0f - vtolx2) + 1.0f), 1.0f / 3.0f);
        x.Ai[0] = (3.0f * sq(X) + 2.0f * X + 3.0f * vtolx2) / (16.0f * X);
        x.Ai[1] = (0.5f - x.Ai[0]) * Kx;
        x.Ai[2] = x.Ai[1] * Kx;
        x.Ai[3] = x.Ai[0] * cu(Kx);

        const float vtoly2 = sq(vtol[1]);
        const float Y = pow(vtoly2 * (sqrt(1.0f - vtoly2) + 1.0f), 1.0f / 3.0f);
        y.Ai[0] = (3.0f * sq(Y) + 2.0f * Y + 3.0f * vtoly2) / (16.0f * Y);
        y.Ai[1] = (0.5f - y.Ai[0]) * Ky;
        y.Ai[2] = y.Ai[1] * Ky;
        y.Ai[3] = y.Ai[0] * cu(Ky);

        const float X_adj = 1.0f / (x.Ai[0] + x.Ai[1] + x.Ai[2] + x.Ai[3]);
        const float Y_adj = 1.0f / (y.Ai[0] + y.Ai[1] + y.Ai[2] + y.Ai[3]);
        for (uint32_t i = 0U; i < 4U; i++) {
          x.Ai[i] *= X_adj;
          y.Ai[i] *= Y_adj;
        }
      }
      break;

      case ftMotionMode_3HEI: {
        max_i = 4U;
        x.Ai[0] = 0.0625f * ( 1.0f + 3.0f * vtol[0] + 2.0f * sqrt( 2.0f * ( vtol[0] + 1.0f ) * vtol[0] ) );
        x.Ai[1] = 0.25f * ( 1.0f - vtol[0] ) * Kx;
        x.Ai[2] = ( 0.5f * ( 1.0f + vtol[0] ) - 2.0f * x.Ai[0] ) * Kx2;
        x.Ai[3] = x.Ai[1] * Kx2;
        x.Ai[4] = x.Ai[0] * sq(Kx2);

        y.Ai[0] = 0.0625f * (1.0f + 3.0f * vtol[1] + 2.0f * sqrt(2.0f * (vtol[1] + 1.0f) * vtol[1]));
        y.Ai[1] = 0.25f * (1.0f - vtol[1]) * Ky;
        y.Ai[2] = (0.5f * (1.0f + vtol[1]) - 2.0f * y.Ai[0]) * Ky2;
        y.Ai[3] = y.Ai[1] * Ky2;
        y.Ai[4] = y.Ai[0] * sq(Ky2);

        const float X_adj = 1.0f / (x.Ai[0] + x.Ai[1] + x.Ai[2] + x.Ai[3] + x.Ai[4]);
        const float Y_adj = 1.0f / (y.Ai[0] + y.Ai[1] + y.Ai[2] + y.Ai[3] + y.Ai[4]);
        for (uint32_t i = 0U; i < 5U; i++) {
          x.Ai[i] *= X_adj;
          y.Ai[i] *= Y_adj;
        }
      }
      break;

      case ftMotionMode_MZV: {
        max_i = 2U;
        const float Bx = 1.4142135623730950488016887242097f * Kx;
        x.Ai[0] = 1.0f / (1.0f + Bx + Kx2);
        x.Ai[1] = x.Ai[0] * Bx;
        x.Ai[2] = x.Ai[0] * Kx2;

        const float By = 1.4142135623730950488016887242097f * Ky;
        y.Ai[0] = 1.0f / (1.0f + By + Ky2);
        y.Ai[1] = y.Ai[0] * By;
        y.Ai[2] = y.Ai[0] * Ky2;
      }
      break;

      default:
        ZERO(x.Ai);
        ZERO(y.Ai);
        max_i = 0;
    }

  }

  void FTMotion::updateShapingA(float zeta[]/*=cfg.zeta*/, float vtol[]/*=cfg.vtol*/) {
    shaping.updateShapingA(zeta, vtol);
  }

  // Refresh the indices used by shaping functions.
  // To be called when frequencies change.

  void FTMotion::AxisShaping::updateShapingN(const_float_t f, const_float_t df) {
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
      case ftMotionMode_ZVDD:
      case ftMotionMode_2HEI:
        Ni[1] = round((0.5f / f / df) * (FTM_FS));
        Ni[2] = Ni[1] + Ni[1];
        Ni[3] = Ni[2] + Ni[1];
        break;
      case ftMotionMode_ZVDDD:
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

  void FTMotion::updateShapingN(const_float_t xf OPTARG(HAS_Y_AXIS, const_float_t yf), float zeta[]/*=cfg.zeta*/) {
    const float xdf = sqrt(1.0f - sq(zeta[0]));
    shaping.x.updateShapingN(xf, xdf);

    #if HAS_Y_AXIS
      const float ydf = sqrt(1.0f - sq(zeta[1]));
      shaping.y.updateShapingN(yf, ydf);
    #endif
  }

#endif // HAS_X_AXIS

// Reset all trajectory processing variables.
void FTMotion::reset() {

  stepperCmdBuff_produceIdx = stepperCmdBuff_consumeIdx = 0;

  traj.reset();

  blockProcRdy = blockProcRdy_z1 = blockProcDn = false;
  batchRdy = batchRdyForInterp = false;
  runoutEna = false;

  endPosn_prevBlock.reset();

  makeVector_idx = makeVector_idx_z1 = 0;
  makeVector_batchIdx = TERN(FTM_UNIFIED_BWS, 0, _MAX(last_batchIdx, FTM_BATCH_SIZE));

  steps.reset();
  interpIdx = interpIdx_z1 = 0;

  #if HAS_X_AXIS
    ZERO(shaping.x.d_zi);
    TERN_(HAS_Y_AXIS, ZERO(shaping.y.d_zi));
    shaping.zi_idx = 0;
  #endif

  TERN_(HAS_EXTRUDERS, e_raw_z1 = e_advanced_z1 = 0.0f);
}

// Private functions.

// Auxiliary function to get number of step commands in the buffer.
int32_t FTMotion::stepperCmdBuffItems() {
  const int32_t udiff = stepperCmdBuff_produceIdx - stepperCmdBuff_consumeIdx;
  return (udiff < 0) ? udiff + (FTM_STEPPERCMD_BUFF_SIZE) : udiff;
}

// Initializes storage variables before startup.
void FTMotion::init() {
  #if HAS_X_AXIS
    refreshShapingN();
    updateShapingA();
  #endif
  reset(); // Precautionary.
}

// Loads / converts block data from planner to fixed-time control variables.
void FTMotion::loadBlockData(block_t * const current_block) {

  const float totalLength = current_block->millimeters,
              oneOverLength = 1.0f / totalLength;

  startPosn = endPosn_prevBlock;
  xyze_pos_t moveDist = LOGICAL_AXIS_ARRAY(
    current_block->steps.e * planner.mm_per_step[E_AXIS_N(current_block->extruder)] * (current_block->direction_bits.e ? 1 : -1),
    current_block->steps.x * planner.mm_per_step[X_AXIS] * (current_block->direction_bits.x ? 1 : -1),
    current_block->steps.y * planner.mm_per_step[Y_AXIS] * (current_block->direction_bits.y ? 1 : -1),
    current_block->steps.z * planner.mm_per_step[Z_AXIS] * (current_block->direction_bits.z ? 1 : -1),
    current_block->steps.i * planner.mm_per_step[I_AXIS] * (current_block->direction_bits.i ? 1 : -1),
    current_block->steps.j * planner.mm_per_step[J_AXIS] * (current_block->direction_bits.j ? 1 : -1),
    current_block->steps.k * planner.mm_per_step[K_AXIS] * (current_block->direction_bits.k ? 1 : -1),
    current_block->steps.u * planner.mm_per_step[U_AXIS] * (current_block->direction_bits.u ? 1 : -1),
    current_block->steps.v * planner.mm_per_step[V_AXIS] * (current_block->direction_bits.v ? 1 : -1),
    current_block->steps.w * planner.mm_per_step[W_AXIS] * (current_block->direction_bits.w ? 1 : -1)
  );

  ratio = moveDist * oneOverLength;

  const float spm = totalLength / current_block->step_event_count;  // (steps/mm) Distance for each step

  f_s = spm * current_block->initial_rate;              // (steps/s) Start feedrate

  const float f_e = spm * current_block->final_rate;    // (steps/s) End feedrate

  /* Keep for comprehension
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
  */

  const float accel = current_block->acceleration,
              oneOverAccel = 1.0f / accel;

  float F_n = current_block->nominal_speed;
  const float ldiff = totalLength + 0.5f * oneOverAccel * (sq(f_s) + sq(f_e));

  float T2 = ldiff / F_n - oneOverAccel * F_n;
  if (T2 < 0.0f) {
    T2 = 0.0f;
    F_n = SQRT(ldiff * accel);
  }

  const float T1 = (F_n - f_s) * oneOverAccel,
              T3 = (F_n - f_e) * oneOverAccel;

  N1 = CEIL(T1 * (FTM_FS));         // Accel datapoints based on Hz frequency
  N2 = CEIL(T2 * (FTM_FS));         // Coast
  N3 = CEIL(T3 * (FTM_FS));         // Decel

  const float T1_P = N1 * (FTM_TS), // (s) Accel datapoints x timestep resolution
              T2_P = N2 * (FTM_TS), // (s) Coast
              T3_P = N3 * (FTM_TS); // (s) Decel

  /**
   * Calculate the reachable feedrate at the end of the accel phase.
   *  totalLength is the total distance to travel in mm
   *  f_s        : (mm/s) Starting feedrate
   *  f_e        : (mm/s) Ending feedrate
   *  T1_P       : (sec) Time spent accelerating
   *  T2_P       : (sec) Time spent coasting
   *  T3_P       : (sec) Time spent decelerating
   *  f_s * T1_P : (mm) Distance traveled during the accel phase
   *  f_e * T3_P : (mm) Distance traveled during the decel phase
   */
  F_P = (2.0f * totalLength - f_s * T1_P - f_e * T3_P) / (T1_P + 2.0f * T2_P + T3_P); // (mm/s) Feedrate at the end of the accel phase

  // Calculate the acceleration and deceleration rates
  accel_P = N1 ? ((F_P - f_s) / T1_P) : 0.0f;

  decel_P = (f_e - F_P) / T3_P;

  // Calculate the distance traveled during the accel phase
  s_1e = f_s * T1_P + 0.5f * accel_P * sq(T1_P);

  // Calculate the distance traveled during the decel phase
  s_2e = s_1e + F_P * T2_P;

  // Accel + Coasting + Decel datapoints
  max_intervals = N1 + N2 + N3;

  endPosn_prevBlock += moveDist;
}

// Generate data points of the trajectory.
void FTMotion::makeVector() {
  float accel_k = 0.0f;                                 // (mm/s^2) Acceleration K factor
  float tau = (makeVector_idx + 1) * (FTM_TS);          // (s) Time since start of block
  float dist = 0.0f;                                    // (mm) Distance traveled

  if (makeVector_idx < N1) {
    // Acceleration phase
    dist = (f_s * tau) + (0.5f * accel_P * sq(tau));    // (mm) Distance traveled for acceleration phase since start of block
    accel_k = accel_P;                                  // (mm/s^2) Acceleration K factor from Accel phase
  }
  else if (makeVector_idx < (N1 + N2)) {
    // Coasting phase
    dist = s_1e + F_P * (tau - N1 * (FTM_TS));          // (mm) Distance traveled for coasting phase since start of block
    //accel_k = 0.0f;
  }
  else {
    // Deceleration phase
    tau -= (N1 + N2) * (FTM_TS);                        // (s) Time since start of decel phase
    dist = s_2e + F_P * tau + 0.5f * decel_P * sq(tau); // (mm) Distance traveled for deceleration phase since start of block
    accel_k = decel_P;                                  // (mm/s^2) Acceleration K factor from Decel phase
  }

  LOGICAL_AXIS_CODE(
    traj.e[makeVector_batchIdx] = startPosn.e + ratio.e * dist,
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
    if (cfg.linearAdvEna) {
      float dedt_adj = (traj.e[makeVector_batchIdx] - e_raw_z1) * (FTM_FS);
      if (ratio.e > 0.0f) dedt_adj += accel_k * cfg.linearAdvK;

      e_raw_z1 = traj.e[makeVector_batchIdx];
      e_advanced_z1 += dedt_adj * (FTM_TS);
      traj.e[makeVector_batchIdx] = e_advanced_z1;
    }
  #endif

  // Update shaping parameters if needed.

  switch (cfg.dynFreqMode) {

    #if HAS_DYNAMIC_FREQ_MM
      case dynFreqMode_Z_BASED:
        if (traj.z[makeVector_batchIdx] != 0.0f) { // Only update if Z changed.
                 const float xf = cfg.baseFreq[X_AXIS] + cfg.dynFreqK[X_AXIS] * traj.z[makeVector_batchIdx]
          OPTARG(HAS_Y_AXIS, yf = cfg.baseFreq[Y_AXIS] + cfg.dynFreqK[Y_AXIS] * traj.z[makeVector_batchIdx]);
          updateShapingN(_MAX(xf, FTM_MIN_SHAPE_FREQ) OPTARG(HAS_Y_AXIS, _MAX(yf, FTM_MIN_SHAPE_FREQ)));
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
  if (++makeVector_batchIdx == FTM_WINDOW_SIZE) {
    makeVector_batchIdx = last_batchIdx;
    batchRdy = true;
  }

  if (++makeVector_idx == max_intervals) {
    blockProcDn = true;
    blockProcRdy = false;
    makeVector_idx = 0;
  }
}

/**
 * Convert to steps
 * - Commands are written in a bitmask with step and dir as single bits.
 * - Tests for delta are moved outside the loop.
 * - Two functions are used for command computation with an array of function pointers.
 */
static void (*command_set[NUM_AXES TERN0(HAS_EXTRUDERS, +1)])(int32_t&, int32_t&, ft_command_t&, int32_t, int32_t);

static void command_set_pos(int32_t &e, int32_t &s, ft_command_t &b, int32_t bd, int32_t bs) {
  if (e < FTM_CTS_COMPARE_VAL) return;
  s++;
  b |= bd | bs;
  e -= FTM_STEPS_PER_UNIT_TIME;
}

static void command_set_neg(int32_t &e, int32_t &s, ft_command_t &b, int32_t bd, int32_t bs) {
  if (e > -(FTM_CTS_COMPARE_VAL)) return;
  s--;
  b |= bs;
  e += FTM_STEPS_PER_UNIT_TIME;
}

// Interpolates single data point to stepper commands.
void FTMotion::convertToSteps(const uint32_t idx) {
  xyze_long_t err_P = { 0 };

  //#define STEPS_ROUNDING
  #if ENABLED(STEPS_ROUNDING)
    #define TOSTEPS(A,B) int32_t(trajMod.A[idx] * planner.settings.axis_steps_per_mm[B] + (trajMod.A[idx] < 0.0f ? -0.5f : 0.5f))
    const xyze_long_t steps_tar = LOGICAL_AXIS_ARRAY(
      TOSTEPS(e, E_AXIS_N(current_block->extruder)), // May be eliminated if guaranteed positive.
      TOSTEPS(x, X_AXIS), TOSTEPS(y, Y_AXIS), TOSTEPS(z, Z_AXIS),
      TOSTEPS(i, I_AXIS), TOSTEPS(j, J_AXIS), TOSTEPS(k, K_AXIS),
      TOSTEPS(u, U_AXIS), TOSTEPS(v, V_AXIS), TOSTEPS(w, W_AXIS)
    );
    xyze_long_t delta = steps_tar - steps;
  #else
    #define TOSTEPS(A,B) int32_t(trajMod.A[idx] * planner.settings.axis_steps_per_mm[B]) - steps.A
    xyze_long_t delta = LOGICAL_AXIS_ARRAY(
      TOSTEPS(e, E_AXIS_N(current_block->extruder)),
      TOSTEPS(x, X_AXIS), TOSTEPS(y, Y_AXIS), TOSTEPS(z, Z_AXIS),
      TOSTEPS(i, I_AXIS), TOSTEPS(j, J_AXIS), TOSTEPS(k, K_AXIS),
      TOSTEPS(u, U_AXIS), TOSTEPS(v, V_AXIS), TOSTEPS(w, W_AXIS)
    );
  #endif

  LOGICAL_AXIS_CODE(
    command_set[E_AXIS_N(current_block->extruder)] = delta.e >= 0 ?  command_set_pos : command_set_neg,
    command_set[X_AXIS] = delta.x >= 0 ?  command_set_pos : command_set_neg,
    command_set[Y_AXIS] = delta.y >= 0 ?  command_set_pos : command_set_neg,
    command_set[Z_AXIS] = delta.z >= 0 ?  command_set_pos : command_set_neg,
    command_set[I_AXIS] = delta.i >= 0 ?  command_set_pos : command_set_neg,
    command_set[J_AXIS] = delta.j >= 0 ?  command_set_pos : command_set_neg,
    command_set[K_AXIS] = delta.k >= 0 ?  command_set_pos : command_set_neg,
    command_set[U_AXIS] = delta.u >= 0 ?  command_set_pos : command_set_neg,
    command_set[V_AXIS] = delta.v >= 0 ?  command_set_pos : command_set_neg,
    command_set[W_AXIS] = delta.w >= 0 ?  command_set_pos : command_set_neg
  );

  for (uint32_t i = 0U; i < (FTM_STEPS_PER_UNIT_TIME); i++) {

    // Init all step/dir bits to 0 (defaulting to reverse/negative motion)
    stepperCmdBuff[stepperCmdBuff_produceIdx] = 0;

    err_P += delta;

    // Set up step/dir bits for all axes
    LOGICAL_AXIS_CODE(
      command_set[E_AXIS_N(current_block->extruder)](err_P.e, steps.e, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_E), _BV(FT_BIT_STEP_E)),
      command_set[X_AXIS](err_P.x, steps.x, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_X), _BV(FT_BIT_STEP_X)),
      command_set[Y_AXIS](err_P.y, steps.y, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_Y), _BV(FT_BIT_STEP_Y)),
      command_set[Z_AXIS](err_P.z, steps.z, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_Z), _BV(FT_BIT_STEP_Z)),
      command_set[I_AXIS](err_P.i, steps.i, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_I), _BV(FT_BIT_STEP_I)),
      command_set[J_AXIS](err_P.j, steps.j, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_J), _BV(FT_BIT_STEP_J)),
      command_set[K_AXIS](err_P.k, steps.k, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_K), _BV(FT_BIT_STEP_K)),
      command_set[U_AXIS](err_P.u, steps.u, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_U), _BV(FT_BIT_STEP_U)),
      command_set[V_AXIS](err_P.v, steps.v, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_V), _BV(FT_BIT_STEP_V)),
      command_set[W_AXIS](err_P.w, steps.w, stepperCmdBuff[stepperCmdBuff_produceIdx], _BV(FT_BIT_DIR_W), _BV(FT_BIT_STEP_W)),
    );

    if (++stepperCmdBuff_produceIdx == (FTM_STEPPERCMD_BUFF_SIZE))
      stepperCmdBuff_produceIdx = 0;

  } // FTM_STEPS_PER_UNIT_TIME loop
}

#endif // FT_MOTION
