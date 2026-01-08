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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(FTM_RESONANCE_TEST)

#include "../ft_motion.h"
#include "resonance_generator.h"

#include <math.h>

ftm_resonance_test_params_t ResonanceGenerator::rt_params;     // Resonance test parameters

bool ResonanceGenerator::active = false;                       // Resonance test active
bool ResonanceGenerator::done = false;                         // Resonance test done
float ResonanceGenerator::rt_time = FTM_TS;                    // Resonance test timer
float ResonanceGenerator::timeline = 0.0f;

ResonanceGenerator::ResonanceGenerator() {}

void ResonanceGenerator::abort() {
  reset();
  ftMotion.reset();
}

void ResonanceGenerator::reset() {
  rt_params = ftm_resonance_test_params_t();
  rt_time = FTM_TS;
  active = false;
  done = false;
}

// Fast sine approximation
float ResonanceGenerator::fast_sin(float x) {
  static constexpr float INV_TAU = (1.0f / M_TAU);

  // Reduce the angle to [-π, π]
  const float y = x * INV_TAU;      // Multiples of 2π
  int k = static_cast<int>(y);      // Truncates toward zero

  // Negative? The truncation is one too high.
  if (y < 0.0f) --k;                // Correct for negatives

  const float r = x - k * M_TAU;    // -π <= r <= π

  // Cheap polynomial approximation of sin(r)
  return r * (1.27323954f - 0.405284735f * ABS(r));
}

void ResonanceGenerator::fill_stepper_plan_buffer() {
  xyze_float_t traj_coords = rt_params.start_pos;

  const float amplitude_precalc = (rt_params.amplitude_correction * rt_params.accel_per_hz * 0.25f) / sq(M_PI);

  float rt_factor = rt_time * M_TAU;

  while (!ftMotion.stepping.is_full()) {
    // Calculate current frequency
    const float freq = getFrequencyFromTimeline();
    if (freq > rt_params.max_freq) {
      done = true;
      return;
    }

    // Amplitude based on a sinusoidal wave : A = accel / (4 * PI^2 * f^2)
    //const float accel_magnitude = rt_params.accel_per_hz * freq;
    //const float amplitude = rt_params.amplitude_correction * accel_magnitude / (4.0f * sq(M_PI) * sq(freq));
    const float amplitude = amplitude_precalc / freq;

    // Phase in radians
    const float phase = freq * rt_factor;

    // Position Offset : between -A and +A
    const float pos_offset = amplitude * fast_sin(phase);

    // Resonate the axis being tested
    traj_coords[rt_params.axis] = rt_params.start_pos[rt_params.axis] + pos_offset;

    // Increment for the next point (before calling out)
    rt_time += FTM_TS;
    rt_factor += FTM_TS * M_TAU;

    // Store in buffer
    ftMotion.stepping_enqueue(traj_coords);
  }
}

#endif // FTM_RESONANCE_TEST
