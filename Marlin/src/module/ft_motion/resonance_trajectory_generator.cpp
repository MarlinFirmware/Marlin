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

 #include "../ft_motion.h"
 #include "resonance_trajectory_generator.h"

 #include <math.h>

  ResonanceTrajectoryGenerator::ResonanceTrajectoryGenerator() {}

  void ResonanceTrajectoryGenerator::planRunout(const float duration) {
    reset();
    ftMotion.reset();
    ftMotion.setTrajectoryType(ftMotion.previous_trajectoryType);    
  }
  void ResonanceTrajectoryGenerator::abort() {
    planRunout(0.0f);
  }

  void ResonanceTrajectoryGenerator::reset() {
    rt_params = ftm_resonance_test_params_t();
    active = false;
    done = false;
  }

  void ResonanceTrajectoryGenerator::fill_stepper_plan_buffer() {
    
    xyze_float_t traj_coords = {};
    millis_t current_ms = millis();
    
    while (!ftMotion.stepper_plan_is_full()) {
       // Calculate time for the CURRENT point
      const millis_t elapsed_ms = current_ms - rt_params.start_time;
      const float t = float(elapsed_ms) * FTM_TS;
      // Calculate current frequency
      const float freq = rt_params.min_freq + t * rt_params.hz_per_sec;
      if (freq > rt_params.max_freq) {
        done = true;
        return;
      }

      // Amplitude based on a sinusoidal wave : A = accel / (4 * PI^2 * f^2)
      //const float accel_magnitude = rt_params.accel_per_hz * freq;
      //const float amplitude = rt_params.amplitude_correction * accel_magnitude / (4.0f * PI * PI * sq(freq));
      const float amplitude = rt_params.amplitude_correction * rt_params.accel_per_hz * 0.25f / (M_PI * M_PI * freq);

      // Phase in radian
      const float phase = 2.0f * M_PI * freq * t;

      // Position Offset : between -A and +A
      const float pos_offset = amplitude * sinf(phase);

      // Set base position and apply offset to the test axis in one step for all axes
      #define _SET_TRAJ(A) traj_coords.A = rt_params.start_pos.A + (rt_params.axis == A##_AXIS ? pos_offset : 0.0f);
      LOGICAL_AXIS_MAP(_SET_TRAJ);

      stepper_plan_t plan = ftMotion.calc_stepper_plan(traj_coords);
      // Store in buffer
      ftMotion.enqueue_stepper_plan(plan);
      // Increment our local time approximation for the next point
      current_ms += FTM_TS;
    }
  }

  float ResonanceTrajectoryGenerator::getCurrentFrequency(millis_t timeline) {
    const millis_t elapsed_ms = timeline - rt_params.start_time;
    const float t = float(elapsed_ms) * FTM_TS;
    return (rt_params.min_freq + t * rt_params.hz_per_sec);
  }

  ftm_resonance_test_params_t ResonanceTrajectoryGenerator::rt_params;     // Resonance test parameters

  bool ResonanceTrajectoryGenerator::active = false;                       // Resonance test active
  bool ResonanceTrajectoryGenerator::done = false;                         // Resonance test done