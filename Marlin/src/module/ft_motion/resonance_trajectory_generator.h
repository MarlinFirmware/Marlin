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

#include "trajectory_generator.h"

  typedef struct FTMResonanceTestParams {
    AxisEnum axis       = X_AXIS;     // Axis to test
    float min_freq      = 5.0f;       // Minimum frequency [Hz]
    float max_freq      = 100.0f;     // Maximum frequency [Hz]
    float hz_per_sec    = 1.0f;       // Frequency change rate [Hz/sec]
    float accel_per_hz  = 60.0f;      // Acceleration per Hz [mm/sec^2/Hz] or [g/Hz]
    int16_t amplitude_correction = 5; // Amplitude correction factor
    millis_t start_time = 0;          // Start time of the test
    xyze_pos_t start_pos;             // Initial stepper position 
  } ftm_resonance_test_params_t;

class ResonanceTrajectoryGenerator : public TrajectoryGenerator {
  public:
    ResonanceTrajectoryGenerator();

    void plan(const float initial_speed, const float final_speed, const float acceleration, float nominal_speed, const float distance) override {};

    void planRunout(const float duration) override;

    float getDistanceAtTime(const float t) const override { return 0.0f; }
     
    float getTotalDuration() const override {return 0.0f; }

    void reset() override;

    float getCurrentFrequency();     // Return frequency based on timeline

    static ftm_resonance_test_params_t rt_params;        // Resonance test parameters

    void fill_stepper_plan_buffer(); // Fill stepper plan buffer with trajectory points  

    bool isActive() const { return active; }
    bool isDone() const { return done; }
    void setActive(bool state) { active = state; }
    void setDone(bool state) { done = state; }

    void abort();                                        // Abort resonance test
    static float timeline;      // Timeline Value to calculate resonance frequency

  private:
    static bool active;         // Resonance test active
    static bool done;           // Resonance test done
};