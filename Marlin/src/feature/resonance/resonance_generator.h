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
#pragma once

#include "../../inc/MarlinConfigPre.h"

#include <math.h>

#if HAS_STANDARD_MOTION
  #include "../../module/planner.h"
#endif

// Fixed-point configuration
#define FP_BITS 16  // 16 bits for fractional part
#define FP_ONE (1UL << FP_BITS)  // Fixed-point 1.0

// Convert float to fixed-point
#define F2FP(x) ((int32_t)((x) * FP_ONE + 0.5f))

// Convert fixed-point to float
#define FP2F(x) ((float)(x) / FP_ONE)

// Fixed-point constants
#define M_TAU_FP F2FP(2.0f * M_PI)
#define M_PI_FP F2FP(M_PI)
#define C0101321184_FP F2FP(0.101321184f)

// For rt_time calculation, perfect match between octave duration and frequency sweep
#define RATIO (1.0f + 1.0f / 65536.0f) // 1 + 1/2^16

typedef struct ResonanceTestParams {
  AxisEnum axis         = NO_AXIS_ENUM; // Axis to test
  float min_freq        =   5.0f;       // Minimum frequency [Hz]
  float max_freq        = 100.0f;       // Maximum frequency [Hz]
  float octave_duration =  40.0f;       // Octave duration for logarithmic progression
  float accel_per_hz    =  60.0f;       // Acceleration per Hz [mm/sec/Hz] or [g/Hz]
  int16_t amplitude_correction = 5;     // Amplitude correction factor
  xyze_pos_t start_pos;                 // Initial stepper position
} resonance_test_params_t;

class ResonanceGenerator {
  public:
    static resonance_test_params_t rt_params;     // Resonance test parameters
    static float timeline;                        // Timeline Value to calculate resonance frequency

    ResonanceGenerator();

    static void reset();
    static void start();

    // Return frequency based on timeline
    static float getFrequencyFromTimeline() {
      // Logarithmic approach with duration per octave
      return rt_params.min_freq * exp2f(timeline / rt_params.octave_duration);
    }

    #if HAS_STANDARD_MOTION
      static block_t* generate_resonance_block(); // Generate planner block for standard motion
    #endif
    #if ENABLED(FT_MOTION)
      static void fill_stepper_plan_buffer();     // Fill stepper plan buffer with trajectory points
    #endif

    static void setActive(const bool state) { active = state; }
    static bool isActive() { return active; }

    static void setDone(const bool state) { done = state; }
    static bool isDone() { return done; }

    static void abort();             // Abort resonance test

  private:
    static float calc_next_pos();     // Calculate next position point based on current frequency

    // Fixed-point variables
    static int32_t amplitude_precalc_fp;  // Fixed-point amplitude precalculation
    static int32_t current_freq_fp;       // Fixed-point current frequency

    // Phase variables (in radians, stored as fixed-point)
    static int32_t phase_fp;          // Fixed-point phase accumulator
    static int32_t freq_to_phase_fp;  // Fixed-point frequency to phase conversion

    static int32_t max_freq_fp;       // Fixed-point maximum frequency
    #if HAS_STANDARD_MOTION
      static block_t block;
    #endif
    static bool active;               // Resonance test active
    static bool done;                 // Resonance test done
};

extern ResonanceGenerator rtg;
