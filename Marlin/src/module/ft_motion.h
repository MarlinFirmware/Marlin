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
#pragma once

#include "../inc/MarlinConfigPre.h" // Access the top level configurations.
#include "../module/planner.h"      // Access block type from planner.

#include "ft_types.h"

#define FTM_STEPPERCMD_DIR_SIZE ((FTM_STEPPERCMD_BUFF_SIZE + 7) / 8)

class FxdTiCtrl {

  public:

    // Public variables
    static ftMotionMode_t cfg_mode;                         // Mode / active compensation mode configuration.
    static bool cfg_linearAdvEna;                           // Linear advance enable configuration.
    static float cfg_linearAdvK;                            // Linear advance gain.
    static dynFreqMode_t cfg_dynFreqMode;                   // Dynamic frequency mode configuration.

    #if HAS_X_AXIS
      static float cfg_baseFreq[1 + ENABLED(HAS_Y_AXIS)];   // Base frequency. [Hz]
      static float cfg_dynFreqK[1 + ENABLED(HAS_Y_AXIS)];   // Scaling / gain for dynamic frequency. [Hz/mm] or [Hz/g]
    #endif

    static ft_command_t stepperCmdBuff[FTM_STEPPERCMD_BUFF_SIZE];               // Buffer of stepper commands.
    static hal_timer_t stepperCmdBuff_StepRelativeTi[FTM_STEPPERCMD_BUFF_SIZE]; // Buffer of the stepper command timing.
    static uint8_t stepperCmdBuff_ApplyDir[FTM_STEPPERCMD_DIR_SIZE];            // Buffer of whether DIR needs to be updated.
    static uint32_t stepperCmdBuff_produceIdx,              // Index of next stepper command write to the buffer.
                    stepperCmdBuff_consumeIdx;              // Index of next stepper command read from the buffer.

    static bool sts_stepperBusy;                            // The stepper buffer has items and is in use.


    // Public methods
    static void startBlockProc(block_t * const current_block); // Set controller states to begin processing a block.
    static bool getBlockProcDn() { return blockProcDn; }    // Return true if the controller no longer needs the current block.
    static void runoutBlock();                              // Move any free data points to the stepper buffer even if a full batch isn't ready.
    static void loop();                                     // Controller main, to be invoked from non-isr task.


    #if HAS_X_AXIS
      // Refresh the gains used by shaping functions.
      // To be called on init or mode or zeta change.
      static void updateShapingA(const_float_t zeta=FTM_SHAPING_ZETA, const_float_t vtol=FTM_SHAPING_V_TOL);

      // Refresh the indices used by shaping functions.
      // To be called when frequencies change.
      static void updateShapingN(const_float_t xf OPTARG(HAS_Y_AXIS, const_float_t yf), const_float_t zeta=FTM_SHAPING_ZETA);
    #endif

    static void reset();                                    // Resets all states of the fixed time conversion to defaults.

  private:

    #if HAS_X_AXIS
      static float xd[2 * (FTM_BATCH_SIZE)], xm[FTM_BATCH_SIZE];
    #endif
    #if HAS_Y_AXIS
      static float yd[2 * (FTM_BATCH_SIZE)], ym[FTM_BATCH_SIZE];
    #endif
    #if HAS_Z_AXIS
      static float zd[2 * (FTM_BATCH_SIZE)], zm[FTM_BATCH_SIZE];
    #endif
    #if HAS_EXTRUDERS
      static float ed[2 * (FTM_BATCH_SIZE)], em[FTM_BATCH_SIZE];
    #endif

    static block_t *current_block_cpy;
    static bool blockProcRdy, blockProcRdy_z1, blockProcDn;
    static bool batchRdy, batchRdyForInterp;
    static bool runoutEna;

    // Trapezoid data variables.
    #if HAS_X_AXIS
      static float x_startPosn, x_endPosn_prevBlock, x_Ratio;
    #endif
    #if HAS_Y_AXIS
      static float y_startPosn, y_endPosn_prevBlock, y_Ratio;
    #endif
    #if HAS_Z_AXIS
      static float z_startPosn, z_endPosn_prevBlock, z_Ratio;
    #endif
    #if HAS_EXTRUDERS
      static float e_startPosn, e_endPosn_prevBlock, e_Ratio;
    #endif
    static float accel_P, decel_P,
                 F_P,
                 f_s,
                 s_1e,
                 s_2e;

    static uint32_t N1, N2, N3;
    static uint32_t max_intervals;

    // Make vector variables.
    static uint32_t makeVector_idx,
                    makeVector_idx_z1,
                    makeVector_batchIdx;

    // Interpolation variables.
    static uint32_t interpIdx,
                    interpIdx_z1;
    #if HAS_X_AXIS
      static int32_t x_steps;
      static stepDirState_t x_dirState;
    #endif
    #if HAS_Y_AXIS
      static int32_t y_steps;
      static stepDirState_t y_dirState;
    #endif
    #if HAS_Z_AXIS
      static int32_t z_steps;
      static stepDirState_t z_dirState;
    #endif
    #if HAS_EXTRUDERS
      static int32_t e_steps;
      static stepDirState_t e_dirState;
    #endif

    static hal_timer_t nextStepTicks;

    // Shaping variables.
    #if HAS_X_AXIS
      static uint32_t xy_zi_idx, xy_max_i;
      static float xd_zi[FTM_ZMAX];
      static float x_Ai[5];
      static uint32_t x_Ni[5];
    #endif
    #if HAS_Y_AXIS
      static float yd_zi[FTM_ZMAX];
      static float y_Ai[5];
      static uint32_t y_Ni[5];
    #endif

    // Linear advance variables.
    #if HAS_EXTRUDERS
      static float e_raw_z1, e_advanced_z1;
    #endif

    // Private methods
    static uint32_t stepperCmdBuffItems();
    static void init();
    static void loadBlockData(block_t * const current_block);
    static void makeVector();
    static void convertToSteps(const uint32_t idx);

}; // class fxdTiCtrl

extern FxdTiCtrl fxdTiCtrl;
