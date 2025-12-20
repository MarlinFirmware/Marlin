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

typedef struct FTSmoothedAxes {
  union {
    struct { float CARTES_AXIS_NAMES;    };
    struct { float CARTES_AXIS_NAMES_LC; };
    float data[CARTES_COUNT];
  };
        float&  operator[](const int n)       { return data[n]; }
  const float&  operator[](const int n) const { return data[n]; }
} ft_smoothed_float_t;

// Smoothing data for each axis
// For the smoothing algorithm use an approximation of moving window averaging
// with Gaussian weights, based on chained exponential smoothers.
typedef struct AxisSmoothing {
  float smoothing_pass[FTM_SMOOTHING_ORDER] = { 0.0f }; // Last value of each of the exponential smoothing passes
  float alpha = 0.0f;                                   // Pre-calculated alpha for smoothing
  uint32_t delay_samples = 0;                           // Pre-calculated delay in samples for smoothing
  void set_time(const float s_time);                    // Set smoothing time, recalculate alpha and delay
} axis_smoothing_t;

typedef struct Smoothing {
  axis_smoothing_t CARTES_AXIS_NAMES;
  int32_t largest_delay_samples;
  // Smoothing causes a phase delay equal to smoothing_time. This delay is compensated-for during axis synchronization,
  // which is done by delaying all axes to match the laggiest one (i.e., largest_delay_samples).
  void refresh_largest_delay_samples() { largest_delay_samples = _MAX(CARTES_LIST(X.delay_samples, Y.delay_samples, Z.delay_samples, E.delay_samples)); }
  // Note: The delay equals smoothing_time only if the input signal frequency is under 1/smoothing_time; which, luckily, holds in this case.
  void reset() {
    #define _CLEAR(A) ZERO(A.smoothing_pass);
    LOGICAL_AXIS_MAP(_CLEAR);
    #undef _CLEAR
  }
} smoothing_t;
