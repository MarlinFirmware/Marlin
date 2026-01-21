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

#include "../../inc/MarlinConfigPre.h"

#include <math.h>

#ifndef M_TAU
  #define M_TAU (2.0f * M_PI)
#endif

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
    static resonance_test_params_t rt_params; // Resonance test parameters
    static float timeline;                        // Timeline Value to calculate resonance frequency

    ResonanceGenerator();

    void reset();

    void start();

    // Return frequency based on timeline
    float getFrequencyFromTimeline() {
      // Logarithmic approach with duration per octave
      return rt_params.min_freq * exp2f(timeline / rt_params.octave_duration);
    }

    void fill_stepper_plan_buffer();                // Fill stepper plan buffer with trajectory points

    void setActive(const bool state) { active = state; }
    bool isActive() const { return active; }

    void setDone(const bool state) { done = state; }
    bool isDone() const { return done; }

    void abort();             // Abort resonance test

  private:
    float calc_next_pos();            // Calculate next position
    static float rt_time;             // Test timer
    static float freq_mul;            // Frequency multiplier for sine sweeping
    static float amplitude_precalc;   // Precalculated part of amplitude formula
    float current_freq;               // Current frequency being generated in sinusoidal motion
    static float phase;               // Current phase in radians
    static bool active;               // Resonance test active
    static bool done;                 // Resonance test done
};

extern ResonanceGenerator rtg;
