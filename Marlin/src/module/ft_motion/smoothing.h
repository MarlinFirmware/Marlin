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

#include "../../inc/MarlinConfig.h"
typedef struct FTSmoothedAxes {
  float CARTES_AXIS_NAMES;
} ft_smoothed_float_t;

// Smoothing data for each axis
typedef struct AxisSmoothing {
  float smoothing_pass[FTM_SMOOTHING_ORDER] = { 0.0f }; // Last value of each of the exponential smoothing passes
  float alpha = 0.0f;               // Pre-calculated alpha for smoothing.
  uint32_t delay_samples = 0;       // Pre-calculated delay in samples for smoothing.
  void set_smoothing_time(const float s_time); // Set smoothing time, recalculate alpha and delay.
} axis_smoothing_t;

typedef struct Smoothing {
  axis_smoothing_t CARTES_AXIS_NAMES;
  int32_t larges_delay_samples;
  // Shaping an axis makes it lag with respect to the others by certain amount, the "centroid delay"
  // Ni[0] stores how far in the past the first step would need to happen to avoid desynchronisation (it is therefore negative).
  // Of course things can't be done in the past, so when shaping is applied, the all axes are delayed by largest_centroid_delay
  // minus their own centroid delay. This makes them all be equally delayed and therefore in synch.
  void refresh_larges_delay_samples() { larges_delay_samples = _MAX(CARTES_LIST(X.delay_samples, Y.delay_samples, Z.delay_samples, E.delay_samples)); }
} smoothing_t;