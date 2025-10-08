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

enum ftMotionShaper_t : uint8_t {
  ftMotionShaper_NONE  = 0, // No compensator
  ftMotionShaper_ZV    = 1, // Zero Vibration
  ftMotionShaper_ZVD   = 2, // Zero Vibration and Derivative
  ftMotionShaper_ZVDD  = 3, // Zero Vibration, Derivative, and Double Derivative
  ftMotionShaper_ZVDDD = 4, // Zero Vibration, Derivative, Double Derivative, and Triple Derivative
  ftMotionShaper_EI    = 5, // Extra-Intensive
  ftMotionShaper_2HEI  = 6, // 2-Hump Extra-Intensive
  ftMotionShaper_3HEI  = 7, // 3-Hump Extra-Intensive
  ftMotionShaper_MZV   = 8  // Modified Zero Vibration
};

enum dynFreqMode_t : uint8_t {
  dynFreqMode_DISABLED   = 0,
  dynFreqMode_Z_BASED    = 1,
  dynFreqMode_MASS_BASED = 2
};

#define AXIS_IS_SHAPING(A)    TERN0(FTM_SHAPER_##A, (ftMotion.cfg.shaper.A != ftMotionShaper_NONE))
#define AXIS_IS_EISHAPING(A)  TERN0(FTM_SHAPER_##A, WITHIN(ftMotion.cfg.shaper.A, ftMotionShaper_EI, ftMotionShaper_3HEI))

typedef struct XYZEarray<float, FTM_WINDOW_SIZE> xyze_trajectory_t;
typedef struct XYZEarray<float, FTM_BATCH_SIZE> xyze_trajectoryMod_t;

// TODO: Convert ft_command_t to a struct with bitfields instead of using a primitive type
enum {
  LOGICAL_AXIS_PAIRED_LIST(
    FT_BIT_DIR_E, FT_BIT_STEP_E,
    FT_BIT_DIR_X, FT_BIT_STEP_X, FT_BIT_DIR_Y, FT_BIT_STEP_Y, FT_BIT_DIR_Z, FT_BIT_STEP_Z,
    FT_BIT_DIR_I, FT_BIT_STEP_I, FT_BIT_DIR_J, FT_BIT_STEP_J, FT_BIT_DIR_K, FT_BIT_STEP_K,
    FT_BIT_DIR_U, FT_BIT_STEP_U, FT_BIT_DIR_V, FT_BIT_STEP_V, FT_BIT_DIR_W, FT_BIT_STEP_W
  ),
  FT_BIT_COUNT
};

typedef bits_t(FT_BIT_COUNT) ft_command_t;

// Emitters for code that only cares about shaped XYZE
#if HAS_FTM_SHAPING
  #define NUM_AXES_SHAPED         COUNT_ENABLED(HAS_X_AXIS, HAS_Y_AXIS, FTM_SHAPER_Z, FTM_SHAPER_E)
  #define SHAPED_AXIS_NAMES       XY_LIST(X, Y) OPTARG(FTM_SHAPER_Z, Z) OPTARG(FTM_SHAPER_E, E)
  #define SHAPED_GANG(A,B,C,D)    XY_GANG(A, B) TERN_(FTM_SHAPER_Z, C) TERN_(FTM_SHAPER_E, D)
  #define SHAPED_LIST(A,B,C,D)    XY_LIST(A, B) OPTARG(FTM_SHAPER_Z, C) OPTARG(FTM_SHAPER_E, D)
  #define SHAPED_ARRAY(A,B,C,D) { SHAPED_LIST(A, B, C, D) }
  #define SHAPED_CODE(A,B,C,D)    XY_CODE(A, B) OPTCODE(FTM_SHAPER_Z, C) OPTCODE(FTM_SHAPER_E, D)
  #define SHAPED_MAP(F)           MAP(F, SHAPED_AXIS_NAMES)
#else
  #define NUM_AXES_SHAPED 0
  #define SHAPED_AXIS_NAMES
  #define SHAPED_GANG(...)
  #define SHAPED_LIST(...)
  #define SHAPED_ARRAY(...) {}
  #define SHAPED_CODE(...)
  #define SHAPED_MAP(...)
#endif

template<typename T>
struct FTShapedAxes {
  union {
    struct { T SHAPED_AXIS_NAMES; };
    struct { T SHAPED_LIST(x, y, z, e); };
    T val[NUM_AXES_SHAPED];
  };
  T& operator[](const int axis) {
    return val[axis_to_index(axis)];
  }

private:
  static constexpr int axis_to_index(const int axis) {
    int idx = -1;
    #define _ATOI(A) idx++; if (axis == _AXIS(A)) return idx;
    SHAPED_MAP(_ATOI);
    #undef _ATOI
    return -1; // Invalid axis
  }
};

typedef FTShapedAxes<float>            ft_shaped_float_t;
typedef FTShapedAxes<ftMotionShaper_t> ft_shaped_shaper_t;
typedef FTShapedAxes<dynFreqMode_t>    ft_shaped_dfm_t;

#if ENABLED(FTM_SMOOTHING)
  typedef struct FTSmoothedAxes {
    float CARTES_AXIS_NAMES;
  } ft_smoothed_float_t;
#endif
