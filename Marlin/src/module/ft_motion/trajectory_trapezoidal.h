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

#include "trajectory_generator.h"
#include <math.h>

/**
 * Trapezoidal trajectory generator.
 * Provides continuous velocity, but acceleration is discontinuous.
 * Implements a trapezoidal velocity profile with acceleration, cruise, and deceleration phases.
 */
class TrapezoidalTrajectoryGenerator : public TrajectoryGenerator {
public:
  TrapezoidalTrajectoryGenerator() = default;

  void plan(const float initial_speed, const float final_speed, const float acceleration, const float nominal_speed, const float distance) override {
    this->initial_speed = initial_speed;
    this->nominal_speed = nominal_speed;
    this->acceleration = acceleration;

    const float one_over_accel = 1.0f / acceleration;
    const float ldiff = distance + 0.5f * one_over_accel * (sq(initial_speed) + sq(final_speed));

    T2 = ldiff / nominal_speed - one_over_accel * nominal_speed;
    if (T2 < 0.0f) {
      T2 = 0.0f;
      this->nominal_speed = sqrtf(ldiff * acceleration);
    }

    T1 = (this->nominal_speed - initial_speed) * one_over_accel;
    T3 = (this->nominal_speed - final_speed) * one_over_accel;

    // Calculate the distance traveled during the accel phase
    pos_before_coast = initial_speed * T1 + 0.5f * acceleration * sq(T1);

    // Calculate the distance traveled during the coast phase
    pos_after_coast = pos_before_coast + this->nominal_speed * T2;
  }

  float getDistanceAtTime(const float t) const override {
    if (t < T1) {
      // Acceleration phase
      return (this->initial_speed * t) + (0.5f * this->acceleration * sq(t));
    } else if (t <= (T1 + T2)) {
      // Coasting phase
      return pos_before_coast + this->nominal_speed * (t - T1);
    }
    // Deceleration phase
    const float tau_decel = t - (T1 + T2);
    return pos_after_coast + this->nominal_speed * tau_decel - 0.5f * this->acceleration * sq(tau_decel);
  }

  float getTotalDuration() const override {
    return T1 + T2 + T3;
  }

  void planRunout(const float duration) override {
    reset();
    T2 = duration; // Coast at zero speed for the entire duration
  }

  void reset() override {
    T1 = T2 = T3 = 0.0f;
    this->initial_speed = this->nominal_speed = this->acceleration = 0.0f;
    pos_before_coast = pos_after_coast = 0.0f;
  }

private:
  // Internal trajectory parameters - kept private
  float T1 = 0.0f;               // Duration of acceleration phase [s]
  float T2 = 0.0f;               // Duration of coasting phase [s]
  float T3 = 0.0f;               // Duration of deceleration phase [s]
  float initial_speed = 0.0f;    // Starting feedrate [mm/s]
  float nominal_speed = 0.0f;    // Peak feedrate [mm/s]
  float acceleration = 0.0f;     // Acceleration [mm/s²]
  float pos_before_coast = 0.0f; // Position after acceleration phase [mm]
  float pos_after_coast = 0.0f;  // Position after acceleration and coasting phase [mm]
};
