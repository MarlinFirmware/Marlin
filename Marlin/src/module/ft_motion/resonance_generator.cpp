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

void ResonanceGenerator::fill_stepper_plan_buffer() {
  xyze_float_t traj_coords = {};

  while (!ftMotion.stepper_plan_is_full()) {
    // Calculate current frequency
    // Logarithmic approach with duration per octave
    const float freq = rt_params.min_freq * powf(2.0f, rt_time / rt_params.octave_duration);
    if (freq > rt_params.max_freq) {
      done = true;
      return;
    }

    // Amplitude based on a sinusoidal wave : A = accel / (4 * PI^2 * f^2)
    //const float accel_magnitude = rt_params.accel_per_hz * freq;
    //const float amplitude = rt_params.amplitude_correction * accel_magnitude / (4.0f * sq(M_PI) * sq(freq));
    const float amplitude = rt_params.amplitude_correction * rt_params.accel_per_hz * 0.25f / (sq(M_PI) * freq);

    // Phase in radians
    const float phase = 2.0f * M_PI * freq * rt_time;

    // Position Offset : between -A and +A
    const float pos_offset = amplitude * sinf(phase);

    // Set base position and apply offset to the test axis in one step for all axes
    #define _SET_TRAJ(A) traj_coords.A = rt_params.start_pos.A + (rt_params.axis == A##_AXIS ? pos_offset : 0.0f);
    LOGICAL_AXIS_MAP(_SET_TRAJ);

    stepper_plan_t plan = ftMotion.calc_stepper_plan(traj_coords);
    // Store in buffer
    ftMotion.enqueue_stepper_plan(plan);
    // Increment time for the next point
    rt_time += FTM_TS;
  }
}

#endif // FTM_RESONANCE_TEST
