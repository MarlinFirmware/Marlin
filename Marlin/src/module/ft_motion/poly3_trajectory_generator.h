/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * 3rd order polynomial trajectory generator.
 * Provides smooth acceleration with continuous velocity and acceleration.
 */
class Poly3TrajectoryGenerator : public TrajectoryGenerator {
public:
  Poly3TrajectoryGenerator() = default;

  void plan(float initial_speed, float final_speed, float acceleration, float nominal_speed, float distance) override {
    initial_speed_ = initial_speed;
    final_speed_ = final_speed;
    nominal_speed_ = nominal_speed;

    // Calculate timing based on a trapezoidal profile
    const float one_over_accel = 1.0f / acceleration;
    T1 = (nominal_speed_ - initial_speed_) * one_over_accel;
    T3 = (nominal_speed_ - final_speed_) * one_over_accel;

    // Calculate distance covered in accel/decel phases with a real cubic S-curve (linear acceleration)
    // This profile has a(0)=0 but a(T1) != 0 to satisfy the other constraints.
    const float d1 = (2.0f * initial_speed_ + nominal_speed_) * T1 / 3.0f;
    const float d3 = (2.0f * nominal_speed_ + final_speed_) * T3 / 3.0f;

    // Calculate cruise distance and time, adjusting for the different ramp distances
    const float d2 = distance - d1 - d3;
    if (d2 < 0) {
      // No cruise phase, recalculate T1 and T3
      const float ldiff = distance + 0.5f * one_over_accel * (initial_speed * initial_speed + final_speed * final_speed);
      nominal_speed_ = sqrtf(ldiff * acceleration);
      T1 = (nominal_speed_ - initial_speed_) * one_over_accel;
      T3 = (nominal_speed_ - final_speed_) * one_over_accel;
      T2 = 0;
    } else {
      T2 = d2 / nominal_speed_;
    }

    // Acceleration phase
    if (T1 > 1e-6) {
        const float v_diff = nominal_speed_ - initial_speed_;
        accel_c0 = 0.0f;
        accel_c1 = initial_speed_;
        accel_c2 = 0.0f; // a(0) = 0
        accel_c3 = v_diff / (3.0f * T1 * T1);
        pos_before_coast = (2.0f * initial_speed_ + nominal_speed_) * T1 / 3.0f;
    } else {
        accel_c0 = accel_c1 = accel_c2 = accel_c3 = 0.0f;
        pos_before_coast = 0.0f;
    }

    // Coast phase
    pos_after_coast = pos_before_coast + nominal_speed_ * T2;

    // Deceleration phase
    if (T3 > 1e-6) {
        const float v_diff = final_speed_ - nominal_speed_;
        decel_c0 = 0.0f; // Relative position
        decel_c1 = nominal_speed_;
        decel_c2 = v_diff / T3;
        decel_c3 = -v_diff / (3.0f * T3 * T3);
    } else {
        decel_c0 = decel_c1 = decel_c2 = decel_c3 = 0.0f;
    }
  }

  void planRunout(float duration) override {
    reset();
    T2 = duration;
  }

  float getDistanceAtTime(float t) const override {
    if (t < T1) {
        // Acceleration phase
        return accel_c0 + t * (accel_c1 + t * (accel_c2 + t * accel_c3));
    } else if (t <= (T1 + T2)) {
        // Coasting phase
        return pos_before_coast + nominal_speed_ * (t - T1);
    } else if (t <= (T1 + T2 + T3)) {
        // Deceleration phase
        const float tau = t - (T1 + T2);
        return pos_after_coast + decel_c0 + tau * (decel_c1 + tau * (decel_c2 + tau * decel_c3));
    }
    return pos_after_coast + (nominal_speed_ + final_speed_) * T3 * 0.5f; // End position
  }

  float getTotalDuration() const override { return T1 + T2 + T3; }

  void reset() override {
    accel_c0 = accel_c1 = accel_c2 = accel_c3 = 0.0f;
    decel_c0 = decel_c1 = decel_c2 = decel_c3 = 0.0f;
    T1 = T2 = T3 = 0.0f;
    initial_speed_ = nominal_speed_ = final_speed_ = 0.0f;
    pos_before_coast = pos_after_coast = 0.0f;
  }

private:
  float accel_c0 = 0.0f, accel_c1 = 0.0f, accel_c2 = 0.0f, accel_c3 = 0.0f;
  float decel_c0 = 0.0f, decel_c1 = 0.0f, decel_c2 = 0.0f, decel_c3 = 0.0f;
  float T1 = 0.0f, T2 = 0.0f, T3 = 0.0f;
  float initial_speed_ = 0.0f, nominal_speed_ = 0.0f, final_speed_ = 0.0f;
  float pos_before_coast = 0.0f, pos_after_coast = 0.0f;
};
