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

#include "../../inc/MarlinConfig.h"

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

#define IS_SHAPING(S)         ((S) != ftMotionShaper_NONE)
#define IS_EISHAPING(S)       TERN0(HAS_FTM_EI_SHAPING, WITHIN(S, ftMotionShaper_EI, ftMotionShaper_3HEI))
#define AXIS_IS_SHAPING(A)    TERN0(FTM_SHAPER_##A, IS_SHAPING(ftMotion.cfg.shaper.A))
#define AXIS_IS_EISHAPING(A)  TERN0(FTM_SHAPER_##A, IS_EISHAPING(ftMotion.cfg.shaper.A))

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
  void reset() { ZERO(val); }

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

#define FTM_MAX_DAMPENING 0.25
constexpr float ftm_max_dampening = float(FTM_MAX_DAMPENING),
                ftm_min_df = SQRT(1.0f - sq(ftm_max_dampening));

constexpr uint32_t CALC_N1(const float v) { return LROUND((v / FTM_MIN_SHAPE_FREQ / ftm_min_df) * (FTM_FS)); }

// Maximum delays for shaping functions
constexpr float ftm_shaping_max_i = _MAX(0.0f
  OPTARG(FTM_SHAPER_ZV,    1 * CALC_N1(0.5f))  OPTARG(FTM_SHAPER_EI,   2 * CALC_N1(0.5f)  )
  OPTARG(FTM_SHAPER_ZVD,   2 * CALC_N1(0.5f))  OPTARG(FTM_SHAPER_2HEI, 3 * CALC_N1(0.5f)  )
  OPTARG(FTM_SHAPER_ZVDD,  3 * CALC_N1(0.5f))  OPTARG(FTM_SHAPER_3HEI, 4 * CALC_N1(0.5f)  )
  OPTARG(FTM_SHAPER_ZVDDD, 4 * CALC_N1(0.5f))  OPTARG(FTM_SHAPER_MZV,  2 * CALC_N1(0.375f))
);

// Max delays for smoothing
constexpr uint32_t ftm_smooth_max_i = uint32_t(TERN0(FTM_SMOOTHING, CEIL(FTM_FS * FTM_MAX_SMOOTHING_TIME)));

constexpr size_t ftm_zmax = ftm_shaping_max_i + ftm_smooth_max_i;

constexpr uint8_t ftm_shaping_ni_size = _MAX(1
  OPTARG(FTM_SHAPER_ZV,    2)  OPTARG(FTM_SHAPER_EI,   3)
  OPTARG(FTM_SHAPER_ZVD,   3)  OPTARG(FTM_SHAPER_2HEI, 4)
  OPTARG(FTM_SHAPER_ZVDD,  4)  OPTARG(FTM_SHAPER_3HEI, 5)
  OPTARG(FTM_SHAPER_ZVDDD, 5)  OPTARG(FTM_SHAPER_MZV,  3)
);

// Shaping data
typedef struct AxisShaping {
  bool ena = false;                         // Enabled indication
  float d_zi[ftm_zmax] = { 0.0f };          // Data point delay vector
  float Ai[ftm_shaping_ni_size];            // Shaping gain vector
  int32_t Ni[ftm_shaping_ni_size] = { 0 };  // Shaping time index vector
  uint32_t max_i;                           // Vector length for the selected shaper

  // Set the gains used by shaping functions
  void set_axis_shaping_N(const ftMotionShaper_t shaper, const float f, const float zeta);

  // Set the indices (per pulse delays) used by shaping functions
  void set_axis_shaping_A(
    const ftMotionShaper_t shaper,
    const float zeta
    OPTARG(HAS_FTM_EI_SHAPING, const float vtol)
  );

} axis_shaping_t;

typedef struct Shaping {
  uint32_t zi_idx;           // Index of storage in the data point delay vectors.
  axis_shaping_t SHAPED_AXIS_NAMES;
  uint32_t largest_delay_samples;
  // Shaping an axis makes it lag with respect to the others by certain amount, the "centroid delay"
  // Ni[0] stores how far in the past the first step would need to happen to avoid desynchronisation (it is therefore negative).
  // Of course things can't be done in the past, so when shaping is applied, the all axes are delayed by largest_delay_samples
  // minus their own centroid delay. This makes them all be equally delayed and therefore in synch.
  void refresh_largest_delay_samples() { largest_delay_samples = -_MIN(SHAPED_LIST(X.Ni[0], Y.Ni[0], Z.Ni[0], E.Ni[0])); }
  void reset() {
    #define _RESET_ZI(A) ZERO(A.d_zi);
    SHAPED_MAP(_RESET_ZI);
    zi_idx = 0;
  }
} shaping_t;
