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

typedef struct FTMResonanceTestParams {
  AxisEnum axis       = NO_AXIS_ENUM; // Axis to test
  float min_freq      = 5.0f;         // Minimum frequency [Hz]
  float max_freq      = 100.0f;       // Maximum frequency [Hz]
  float octave_duration = 40.0f;      // Octave duration for logarithmic progression
  float accel_per_hz  = 60.0f;        // Acceleration per Hz [mm/sec/Hz] or [g/Hz]
  int16_t amplitude_correction = 5;   // Amplitude correction factor
  xyze_pos_t start_pos;               // Initial stepper position
} ftm_resonance_test_params_t;

class ResonanceGenerator {
  public:
    static ftm_resonance_test_params_t rt_params; // Resonance test parameters
    static float timeline;                        // Timeline Value to calculate resonance frequency

    ResonanceGenerator();

    void planRunout(const float duration);

    void reset();

    void start(const xyze_pos_t &spos, const float t) {
      rt_params.start_pos = spos;
      rt_time = t;
      active = true;
      done = false;
    }

    float getFrequencyFromTimeline() {
      return (rt_params.min_freq * powf(2.0f, timeline / rt_params.octave_duration)); // Return frequency based on timeline
    }

    void fill_stepper_plan_buffer();                // Fill stepper plan buffer with trajectory points

    bool isActive() const { return active; }
    bool isDone() const { return done; }
    void setActive(bool state) { active = state; }
    void setDone(bool state) { done = state; }

    void abort();               // Abort resonance test

  private:
    static float rt_time;    // Test timer
    static bool active;         // Resonance test active
    static bool done;           // Resonance test done
};
