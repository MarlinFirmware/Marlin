/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "resonance_generator.h"

#if ENABLED(FT_MOTION)
  #include "../../module/ft_motion.h"
#endif

#include "../../gcode/gcode.h" // for home_all_axes

resonance_test_params_t ResonanceGenerator::rt_params;     // Resonance test parameters
float ResonanceGenerator::timeline;

bool ResonanceGenerator::active = false;                   // Resonance test active
bool ResonanceGenerator::done = false;                     // Resonance test done
int32_t ResonanceGenerator::phase_inc_fp;
int32_t ResonanceGenerator::freq_mul_inc;
int32_t ResonanceGenerator::max_freq_fp;
int32_t ResonanceGenerator::phase_fp;
int32_t ResonanceGenerator::current_freq_fp;
int32_t ResonanceGenerator::amplitude_precalc_fp;


#if HAS_STANDARD_MOTION
  block_t ResonanceGenerator::block;
#endif

ResonanceGenerator rtg;

ResonanceGenerator::ResonanceGenerator() {}

void ResonanceGenerator::start() {
  gcode.home_all_axes(); // Always home axes first
  motion.blocking_move_xy(X_CENTER, Y_CENTER, Z_CLEARANCE_FOR_HOMING);

  rt_params.start_pos = motion.position;
  active = true;
  done = false;

  // Clamp Z-axis acceleration for safety
  if (rt_params.axis == Z_AXIS)
    NOMORE(rt_params.accel_per_hz, 15.0f);

  #if HAS_STANDARD_MOTION
    if (TERN1(FT_MOTION, !ftMotion.cfg.active)) {
      block.reset();
      block.initial_rate = (rt_params.axis == Z_AXIS) ? 2000 : 8000;
    }
  #endif

  // Precompute fixed-point sine sweep parameters
  amplitude_precalc_fp = F2FP((rt_params.amplitude_correction * rt_params.accel_per_hz * 0.25f) / sq(M_PI));
  current_freq_fp = F2FP(rt_params.min_freq);
  max_freq_fp = F2FP(rt_params.max_freq);
  phase_fp = 0;

  // Sweep time base
  // After n generated points, t = n·RESONANCE_TS  and  f(t) = min_freq · exp2(t / octave_duration)
  freq_mul_inc = F2FPQ(exp2f(RESONANCE_TS / rt_params.octave_duration) - 1.0f);
  // Use Q28 for phase calculation only for consistency between freq and phase (Q16 is enough) 
  phase_inc_fp = F2FPQ((2.0f * M_PI) * RESONANCE_TS);
}

void ResonanceGenerator::abort() {
  reset();
  #if HAS_STANDARD_MOTION
    if (!TERN0(FT_MOTION, ftMotion.cfg.active))
      return;
  #endif
  TERN_(FT_MOTION, ftMotion.reset());
}

void ResonanceGenerator::reset() {
  rt_params = resonance_test_params_t();
  #if HAS_STANDARD_MOTION
    if (!TERN0(FT_MOTION, ftMotion.cfg.active))
      block.reset();
  #endif
  active = false;
  done = false;
}

float ResonanceGenerator::calc_next_pos() {
  // Phase accumulation and wrapping within [0, 2π)
  // Δφ = 2π·f·RESONANCE_TS
  phase_fp += (int32_t)(((int64_t)current_freq_fp * phase_inc_fp) >> FP_Q);
  if (phase_fp >= M_TAU_FP) phase_fp -= M_TAU_FP;
  else if (phase_fp < 0) phase_fp += M_TAU_FP;

  // -π <= r <= π
  const int32_t r_fp = (phase_fp > M_PI_FP) ? phase_fp - M_TAU_FP : phase_fp;

  // Windowing polynomial: 1.0 - r²/π²
  const int64_t r2 = (int64_t)r_fp * r_fp;
  const int32_t poly_fp = FP_ONE - (int32_t)(((int64_t)C0101321184_FP * (r2 >> FP_BITS)) >> FP_BITS);

  // Amplitude A = amplitude_precalc / f
  const int32_t amplitude_fp = (int32_t)(((int64_t)amplitude_precalc_fp * FP_ONE) / current_freq_fp);

  // Position offset = A·r·poly
  const int32_t pos_fp = (int32_t)(((((int64_t)amplitude_fp * r_fp) >> FP_BITS) * poly_fp) >> FP_BITS);

  return FP2F(pos_fp);
}

#if ENABLED(FT_MOTION)

  void ResonanceGenerator::fill_stepper_plan_buffer() {
    #if HAS_FTM_DIR_CHANGE_HOLD
      xyze_float_t traj_coords = ftMotion.get_last_target_traj();
      traj_coords[rt_params.axis] = rt_params.start_pos[rt_params.axis];
    #else
      xyze_float_t traj_coords = rt_params.start_pos;
    #endif
    // Save starting position, avoid cumulative errors
    const float start_pos = rt_params.start_pos[rt_params.axis];

    while (!ftMotion.stepping.is_full()) {
      // Calculate current frequency with exponential sweep
      current_freq_fp += (int32_t)(((int64_t)current_freq_fp * freq_mul_inc + (1L << (FP_Q - 1))) >> FP_Q);
      if (current_freq_fp > max_freq_fp) {
        done = true;
        return;
      }

      // Resonate the axis being tested
      traj_coords[rt_params.axis] = start_pos + calc_next_pos();

      TERN_(HAS_FTM_DIR_CHANGE_HOLD, traj_coords = ftMotion.ftm_hold_frames(traj_coords));

      // Store in buffer
      ftMotion.stepping_enqueue(traj_coords);
    }
  }

#endif // FT_MOTION

#if HAS_STANDARD_MOTION

  block_t* ResonanceGenerator::generate_resonance_block() {
    // Static variables to retain state between calls and avoid cumulative errors
    static float prev_pos = 0.0f, step_accumulator = 0.0f;

    const float step_mm = planner.settings.axis_steps_per_mm[rt_params.axis];
    const uint8_t axis_bit = 1 << rt_params.axis;

    // Calculate current frequency with exponential sweep
    current_freq_fp += (int32_t)(((int64_t)current_freq_fp * freq_mul_inc + (1L << (FP_Q - 1))) >> FP_Q);
    if (current_freq_fp > max_freq_fp) {
      done = true;
      return nullptr;
    }

    // Calculate position and accumulate steps
    const float current_pos = calc_next_pos();
    step_accumulator += (current_pos - prev_pos) * step_mm;
    prev_pos = current_pos;

    // Extract steps
    const int32_t delta_steps = (int32_t)floor(step_accumulator);
    step_accumulator -= delta_steps;
    const uint32_t abs_steps = abs(delta_steps);

    // Update block
    block.steps[rt_params.axis] = abs_steps;
    block.step_event_count = abs_steps;
    if (delta_steps < 0)
      block.direction_bits |= axis_bit;
    else
      block.direction_bits &= ~axis_bit;

    return &block;
  }

#endif // HAS_STANDARD_MOTION

#endif // RESONANCE_TEST
