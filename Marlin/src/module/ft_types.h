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
  ftMotionMode_DISABLED   =  0U,
  ftMotionMode_ENABLED    =  1U,
  //ftMotionMode_ULENDO_FBS = 2U,
  ftMotionMode_ZV         = 10U,
  ftMotionMode_ZVD        = 11U,
  ftMotionMode_EI         = 12U,
  ftMotionMode_2HEI       = 13U,
  ftMotionMode_3HEI       = 14U,
  ftMotionMode_MZV        = 15U,
  //ftMotionMode_DISCTF   = 20U
} ftMotionMode_t;

enum dynFreqMode_t : uint8_t {
  dynFreqMode_DISABLED   = 0U,
  dynFreqMode_Z_BASED    = 1U,
  dynFreqMode_MASS_BASED = 2U
};

enum stepDirState_t {
  stepDirState_NOT_SET = 0U,
  stepDirState_POS     = 1U,
  stepDirState_NEG     = 2U
};

enum {
  FT_BIT_DIR_E, FT_BIT_STEP_E,
  FT_BIT_DIR_Z, FT_BIT_STEP_Z,
  FT_BIT_DIR_Y, FT_BIT_STEP_Y,
  FT_BIT_DIR_X, FT_BIT_STEP_X,
  FT_BIT_COUNT
};

typedef bits_t(FT_BIT_COUNT) ft_command_t;
