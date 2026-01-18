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

#if ENABLED(RESONANCE_TEST)

#include "../../module/ft_motion.h"
#include "resonance_generator.h"
#include "../../gcode/gcode.h" // for home_all_axes

#include <math.h>

resonance_test_params_t ResonanceGenerator::rt_params;     // Resonance test parameters

bool ResonanceGenerator::active = false;                       // Resonance test active
bool ResonanceGenerator::done = false;                         // Resonance test done
float ResonanceGenerator::rt_time = FTM_TS;                    // Resonance test timer
float ResonanceGenerator::timeline = 0.0f;
float ResonanceGenerator::amplitude_precalc;
float ResonanceGenerator::freq_mul;
xyze_float_t ResonanceGenerator:: traj_coords;

ResonanceGenerator rtg;

ResonanceGenerator::ResonanceGenerator() {}

void ResonanceGenerator::start() {
  gcode.home_all_axes; // For safety and ensure known axes

  // Safe Acceleration per Hz for Z axis
  if (rt_params.axis == Z_AXIS && rt_params.accel_per_hz > 15.0f)
    rt_params.accel_per_hz = 15.0f;

  // Always move to the center of the bed
  do_blocking_move_to_xy(X_CENTER, Y_CENTER, Z_CLEARANCE_FOR_HOMING);
      
  rt_params.start_pos = current_position;
  traj_coords = rt_params.start_pos;
  rt_time = FTM_TS;
  active = true;
  done = false;
  // Precompute sine sweep const
  amplitude_precalc = (rt_params.amplitude_correction * rt_params.accel_per_hz * 0.25f) / sq(M_PI);
  current_freq = rt_params.min_freq;
  const float inv_octave_duration = 1.0f / rt_params.octave_duration;
  freq_mul = exp2f(FTM_TS * inv_octave_duration);
}

void ResonanceGenerator::abort() {
  reset();
  ftMotion.reset();
}

void ResonanceGenerator::reset() {
  rt_params = resonance_test_params_t();
  rt_time = FTM_TS;
  active = false;
  done = false;
}

// Fast sine approximation
float ResonanceGenerator::fast_sin(float x) {

  // Reduce the angle to [-π, π]
  const float y = x * (1.0f / M_TAU); // Multiples of 2π
  int k = static_cast<int>(y);        // Truncates toward zero

  // Negative? The truncation is one too high.
  if (y < 0.0f) --k;                // Correct for negatives

  float r = x - k * M_TAU;    // -π <= r <= π
  if (r > M_PI)
    r -= M_TAU;
  else if (r < -M_PI)
    r += M_TAU;

  // Optimized polynomial approximation
  // Using sin(x) ≈ x(1 - (x²/π²) * 0.785398163) where 0.785398163 ≈ 1/π
  const float r2 = r * r;

  return r * (1.0f - 0.101321184f * r2);
}

void ResonanceGenerator::fill_stepper_plan_buffer() {
  //xyze_float_t traj_coords = rt_params.start_pos;

  while (!ftMotion.stepping.is_full()) {
    // Calculate current frequency
    current_freq *= freq_mul;
    if (current_freq > rt_params.max_freq) {
      done = true;
      return;
    }

    // Resonate the axis being tested
    traj_coords[rt_params.axis] = calc_next_pos();

    // Increment for the next point (before calling out)
    rt_time += FTM_TS;

    // Store in buffer
    ftMotion.stepping_enqueue(traj_coords);
  }
}

#endif // RESONANCE_TEST
