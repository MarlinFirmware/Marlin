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
float ResonanceGenerator::phase = 0.0f;
float ResonanceGenerator::freq_mul;
xyze_float_t ResonanceGenerator:: traj_coords;

ResonanceGenerator rtg;

ResonanceGenerator::ResonanceGenerator() {}

void ResonanceGenerator::start() {
  gcode.home_all_axes(); // For safety and ensure known axes

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

float ResonanceGenerator::calc_next_pos() {
  // Amplitude based on a sinusoidal wave : A = accel / (4 * PI^2 * f^2)
  const float amplitude = amplitude_precalc / current_freq;

  // Phase accumulation in radians
  phase += current_freq * M_TAU * rt_time;
  if (phase >= M_TAU) phase -= M_TAU;

  const float r = (phase > M_PI) ? (phase - M_TAU) : phase; 
  const float r2 = r * r;
  
  // New postion
  return rt_params.start_pos[rt_params.axis] + amplitude * r * (1.0f - 0.101321184f * r2);
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

    // Store in buffer
    ftMotion.stepping_enqueue(traj_coords);
  }
}

#endif // RESONANCE_TEST
