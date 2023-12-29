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

#if HAS_X_AXIS && (HAS_Z_AXIS || HAS_EXTRUDERS)
  #define HAS_DYNAMIC_FREQ 1
  #if HAS_Z_AXIS
    #define HAS_DYNAMIC_FREQ_MM 1
  #endif
  #if HAS_EXTRUDERS
    #define HAS_DYNAMIC_FREQ_G 1
  #endif
#endif

typedef struct FTConfig {
  ftMotionMode_t mode = FTM_DEFAULT_MODE;                 // Mode / active compensation mode configuration.

  bool modeHasShaper() { return WITHIN(mode, 10U, 19U); }

  #if HAS_X_AXIS
    float baseFreq[1 + ENABLED(HAS_Y_AXIS)] =             // Base frequency. [Hz]
      { FTM_SHAPING_DEFAULT_X_FREQ OPTARG(HAS_Y_AXIS, FTM_SHAPING_DEFAULT_Y_FREQ) };
    float zeta[1 + ENABLED(HAS_Y_AXIS)] =                 // Damping factor
        { FTM_SHAPING_ZETA_X OPTARG(HAS_Y_AXIS, FTM_SHAPING_ZETA_Y) };
    float vtol[1 + ENABLED(HAS_Y_AXIS)] =                 // Vibration Level
        { FTM_SHAPING_V_TOL_X OPTARG(HAS_Y_AXIS, FTM_SHAPING_V_TOL_Y) };
  #endif

#if HAS_DYNAMIC_FREQ
    dynFreqMode_t dynFreqMode = FTM_DEFAULT_DYNFREQ_MODE; // Dynamic frequency mode configuration.
    float dynFreqK[1 + ENABLED(HAS_Y_AXIS)] = { 0.0f };   // Scaling / gain for dynamic frequency. [Hz/mm] or [Hz/g]
  #else
    static constexpr dynFreqMode_t dynFreqMode = dynFreqMode_DISABLED;
  #endif

  #if HAS_EXTRUDERS
    bool linearAdvEna = FTM_LINEAR_ADV_DEFAULT_ENA;       // Linear advance enable configuration.
    float linearAdvK = FTM_LINEAR_ADV_DEFAULT_K;          // Linear advance gain.
  #endif
} ft_config_t;

class FTMotion {

  public:

    // Public variables
    static ft_config_t cfg;
    static bool busy;

    static void set_defaults() {
      cfg.mode = FTM_DEFAULT_MODE;

      TERN_(HAS_X_AXIS, cfg.baseFreq[X_AXIS] = FTM_SHAPING_DEFAULT_X_FREQ);
      TERN_(HAS_Y_AXIS, cfg.baseFreq[Y_AXIS] = FTM_SHAPING_DEFAULT_Y_FREQ);

      TERN_(HAS_X_AXIS, cfg.zeta[X_AXIS] = FTM_SHAPING_ZETA_X);
      TERN_(HAS_Y_AXIS, cfg.zeta[Y_AXIS] = FTM_SHAPING_ZETA_Y);

      TERN_(HAS_X_AXIS, cfg.vtol[X_AXIS] = FTM_SHAPING_V_TOL_X);
      TERN_(HAS_Y_AXIS, cfg.vtol[Y_AXIS] = FTM_SHAPING_V_TOL_Y);

      #if HAS_DYNAMIC_FREQ
        cfg.dynFreqMode = FTM_DEFAULT_DYNFREQ_MODE;
        cfg.dynFreqK[X_AXIS] = TERN_(HAS_Y_AXIS, cfg.dynFreqK[Y_AXIS]) = 0.0f;
      #endif

      #if HAS_EXTRUDERS
        cfg.linearAdvEna = FTM_LINEAR_ADV_DEFAULT_ENA;
        cfg.linearAdvK = FTM_LINEAR_ADV_DEFAULT_K;
      #endif

      #if HAS_X_AXIS
        refreshShapingN();
        updateShapingA();
      #endif

      reset();
    }

    static ft_command_t stepperCmdBuff[FTM_STEPPERCMD_BUFF_SIZE]; // Buffer of stepper commands.
    static uint32_t stepperCmdBuff_produceIdx,            // Index of next stepper command write to the buffer.
                    stepperCmdBuff_consumeIdx;            // Index of next stepper command read from the buffer.

    static bool sts_stepperBusy;                          // The stepper buffer has items and is in use.


    // Public methods
    static void init();
    static void startBlockProc();                         // Set controller states to begin processing a block.
    static bool getBlockProcDn() { return blockProcDn; }  // Return true if the controller no longer needs the current block.
    static void runoutBlock();                            // Move any free data points to the stepper buffer even if a full batch isn't ready.
    static void loop();                                   // Controller main, to be invoked from non-isr task.

    #if HAS_X_AXIS
      // Refresh the gains used by shaping functions.
      // To be called on init or mode or zeta change.
      static void updateShapingA(float zeta[]=cfg.zeta, float vtol[]=cfg.vtol);

      // Refresh the indices used by shaping functions.
      // To be called when frequencies change.
      static void updateShapingN(const_float_t xf OPTARG(HAS_Y_AXIS, const_float_t yf), float zeta[]=cfg.zeta);

      static void refreshShapingN() { updateShapingN(cfg.baseFreq[X_AXIS] OPTARG(HAS_Y_AXIS, cfg.baseFreq[Y_AXIS])); }

    #endif

    static void reset();                                  // Reset all states of the fixed time conversion to defaults.

  private:

    static xyze_trajectory_t traj;
    static xyze_trajectoryMod_t trajMod;
    static xyze_trajectoryWin_t trajWin;

    static bool blockProcRdy, blockProcRdy_z1, blockProcDn;
    static bool batchRdy, batchRdyForInterp;
    static bool runoutEna;
    static bool blockDataIsRunout;

    // Trapezoid data variables.
    static xyze_pos_t   startPosn,          // (mm) Start position of block
                        endPosn_prevBlock;  // (mm) End position of previous block
    static xyze_float_t ratio;              // (ratio) Axis move ratio of block
    static float accel_P, decel_P,
                 F_P,
                 f_s,
                 s_1e,
                 s_2e;

    static uint32_t N1, N2, N3;
    static uint32_t max_intervals;

    static constexpr uint32_t _ftm_size = TERN(FTM_UNIFIED_BWS, FTM_BW_SIZE, FTM_BATCH_SIZE),
                              _ftm_wind = TERN(FTM_UNIFIED_BWS, 2, ceil((FTM_WINDOW_SIZE) / _ftm_size)),
                              shaper_intervals = _ftm_size * ceil((FTM_ZMAX) / _ftm_size),
                              min_max_intervals = _ftm_size * _ftm_wind;

    // Make vector variables.
    static uint32_t makeVector_idx,
                    makeVector_idx_z1,
                    makeVector_batchIdx;

    // Interpolation variables.
    static uint32_t interpIdx,
                    interpIdx_z1;

    static xyze_long_t steps;

    // Shaping variables.
    #if HAS_X_AXIS

      typedef struct AxisShaping {
        float d_zi[FTM_ZMAX] = { 0.0f };  // Data point delay vector.
        float Ai[5];                      // Shaping gain vector.
        uint32_t Ni[5];                   // Shaping time index vector.

        void updateShapingN(const_float_t f, const_float_t df);

      } axis_shaping_t;

      typedef struct Shaping {
        uint32_t zi_idx,           // Index of storage in the data point delay vectors.
                 max_i;            // Vector length for the selected shaper.
        axis_shaping_t x;
        #if HAS_Y_AXIS
          axis_shaping_t y;
        #endif

        void updateShapingA(float zeta[]=cfg.zeta, float vtol[]=cfg.vtol);

      } shaping_t;

      static shaping_t shaping; // Shaping data

    #endif // HAS_X_AXIS

    // Linear advance variables.
    #if HAS_EXTRUDERS
      static float e_raw_z1, e_advanced_z1;
    #endif

    // Private methods
    static uint32_t stepperCmdBuffItems();
    static void loadBlockData(block_t *const current_block);
    static void makeVector();
    static void convertToSteps(const uint32_t idx);

}; // class FTMotion

extern FTMotion ftMotion;
