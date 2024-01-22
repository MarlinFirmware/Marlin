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

#include "../core/types.h"

typedef enum FXDTICtrlMode : uint8_t {
  ftMotionMode_DISABLED   =  0, // Standard Motion
  ftMotionMode_ENABLED    =  1, // Time-Based Motion
  ftMotionMode_ZV         = 10, // Zero Vibration
  ftMotionMode_ZVD        = 11, // Zero Vibration and Derivative
  ftMotionMode_ZVDD       = 12, // Zero Vibration, Derivative, and Double Derivative
  ftMotionMode_ZVDDD      = 13, // Zero Vibration, Derivative, Double Derivative, and Triple Derivative
  ftMotionMode_EI         = 14, // Extra-Intensive
  ftMotionMode_2HEI       = 15, // 2-Hump Extra-Intensive
  ftMotionMode_3HEI       = 16, // 3-Hump Extra-Intensive
  ftMotionMode_MZV        = 17  // Mass-based Zero Vibration
} ftMotionMode_t;

enum dynFreqMode_t : uint8_t {
  dynFreqMode_DISABLED   = 0,
  dynFreqMode_Z_BASED    = 1,
  dynFreqMode_MASS_BASED = 2
};

#define IS_EI_MODE(N) WITHIN(N, ftMotionMode_EI, ftMotionMode_3HEI)

typedef struct XYZEarray<float, FTM_WINDOW_SIZE> xyze_trajectory_t;
typedef struct XYZEarray<float, FTM_BATCH_SIZE> xyze_trajectoryMod_t;

enum {
  LIST_N(DOUBLE(LOGICAL_AXES),
    FT_BIT_DIR_E, FT_BIT_STEP_E,
    FT_BIT_DIR_X, FT_BIT_STEP_X, FT_BIT_DIR_Y, FT_BIT_STEP_Y, FT_BIT_DIR_Z, FT_BIT_STEP_Z,
    FT_BIT_DIR_I, FT_BIT_STEP_I, FT_BIT_DIR_J, FT_BIT_STEP_J, FT_BIT_DIR_K, FT_BIT_STEP_K,
    FT_BIT_DIR_U, FT_BIT_STEP_U, FT_BIT_DIR_V, FT_BIT_STEP_V, FT_BIT_DIR_W, FT_BIT_STEP_W
  ),
  FT_BIT_COUNT
};

typedef bits_t(FT_BIT_COUNT) ft_command_t;
