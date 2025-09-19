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
 * 7th order polynomial trajectory generator.
 * Provides smooth acceleration with continuous velocity, acceleration, jerk, and snap.
 */
class Poly7TrajectoryGenerator : public TrajectoryGenerator {
public:
  Poly7TrajectoryGenerator() = default;

  void plan(float initial_speed, float final_speed, float acceleration, float nominal_speed, float distance) override {
    initial_speed_ = initial_speed;
    final_speed_ = final_speed;

    // Calculate timing phases using the same logic as trapezoidal generator
    const float one_over_accel = 1.0f / acceleration;
    const float ldiff = distance + 0.5f * one_over_accel * (initial_speed * initial_speed + final_speed * final_speed);

    T2 = ldiff / nominal_speed - one_over_accel * nominal_speed;
    if (T2 < 0.0f) {
      T2 = 0.0f;
      nominal_speed_ = sqrtf(ldiff * acceleration);
    } else {
      nominal_speed_ = nominal_speed;
    }

    T1 = (nominal_speed_ - initial_speed_) * one_over_accel;
    T3 = (nominal_speed_ - final_speed_) * one_over_accel;

    // Acceleration phase
    if (T1 > 1e-6) {
        const float d1 = (initial_speed_ + nominal_speed_) * T1 * 0.5f;
        const float T1_2 = T1 * T1;
        const float T1_3 = T1_2 * T1;
        const float T1_4 = T1_3 * T1;
        const float T1_5 = T1_4 * T1;
        const float T1_6 = T1_5 * T1;
        const float T1_7 = T1_6 * T1;
        accel_c0 = 0.0f;
        accel_c1 = initial_speed_;
        accel_c2 = 0.0f;
        accel_c3 = 0.0f;
        accel_c4 = (35.0f * d1 - (20.0f * initial_speed_ + 15.0f * nominal_speed_) * T1) / T1_4;
        accel_c5 = (84.0f * d1 - (48.0f * initial_speed_ + 36.0f * nominal_speed_) * T1) / -T1_5;
        accel_c6 = (70.0f * d1 - (40.0f * initial_speed_ + 30.0f * nominal_speed_) * T1) / T1_6;
        accel_c7 = (20.0f * d1 - (12.0f * initial_speed_ + 8.0f * nominal_speed_) * T1) / -T1_7;
        pos_before_coast = d1;
    } else {
        accel_c0 = accel_c1 = accel_c2 = accel_c3 = accel_c4 = accel_c5 = accel_c6 = accel_c7 = 0.0f;
        pos_before_coast = 0.0f;
    }

    // Coast phase
    pos_after_coast = pos_before_coast + nominal_speed_ * T2;

    // Deceleration phase
    if (T3 > 1e-6) {
        const float d3 = (nominal_speed_ + final_speed_) * T3 * 0.5f;
        const float T3_2 = T3 * T3;
        const float T3_3 = T3_2 * T3;
        const float T3_4 = T3_3 * T3;
        const float T3_5 = T3_4 * T3;
        const float T3_6 = T3_5 * T3;
        const float T3_7 = T3_6 * T3;
        decel_c0 = 0.0f;
        decel_c1 = nominal_speed_;
        decel_c2 = 0.0f;
        decel_c3 = 0.0f;
        decel_c4 = (35.0f * d3 - (20.0f * nominal_speed_ + 15.0f * final_speed_) * T3) / T3_4;
        decel_c5 = (84.0f * d3 - (48.0f * nominal_speed_ + 36.0f * final_speed_) * T3) / -T3_5;
        decel_c6 = (70.0f * d3 - (40.0f * nominal_speed_ + 30.0f * final_speed_) * T3) / T3_6;
        decel_c7 = (20.0f * d3 - (12.0f * nominal_speed_ + 8.0f * final_speed_) * T3) / -T3_7;
    } else {
        decel_c0 = decel_c1 = decel_c2 = decel_c3 = decel_c4 = decel_c5 = decel_c6 = decel_c7 = 0.0f;
    }
  }

  void planRunout(float duration) override {
    reset();
    T2 = duration;
  }

  float getDistanceAtTime(float t) const override {
    if (t < T1) {
        // Acceleration phase
        return accel_c0 + t * (accel_c1 + t * (accel_c2 + t * (accel_c3 + t * (accel_c4 + t * (accel_c5 + t * (accel_c6 + t * accel_c7))))));
    } else if (t <= (T1 + T2)) {
        // Coasting phase
        return pos_before_coast + nominal_speed_ * (t - T1);
    } else if (t <= (T1 + T2 + T3)) {
        // Deceleration phase
        const float tau = t - (T1 + T2);
        return pos_after_coast + decel_c0 + tau * (decel_c1 + tau * (decel_c2 + tau * (decel_c3 + tau * (decel_c4 + tau * (decel_c5 + tau * (decel_c6 + tau * decel_c7))))));
    }
    return pos_after_coast + (nominal_speed_ + final_speed_) * T3 * 0.5f; // End position
  }

  float getTotalDuration() const override { return T1 + T2 + T3; }

  void reset() override {
    accel_c0 = accel_c1 = accel_c2 = accel_c3 = accel_c4 = accel_c5 = accel_c6 = accel_c7 = 0.0f;
    decel_c0 = decel_c1 = decel_c2 = decel_c3 = decel_c4 = decel_c5 = decel_c6 = decel_c7 = 0.0f;
    T1 = T2 = T3 = 0.0f;
    initial_speed_ = nominal_speed_ = final_speed_ = 0.0f;
    pos_before_coast = pos_after_coast = 0.0f;
  }

private:
  float accel_c0 = 0.0f, accel_c1 = 0.0f, accel_c2 = 0.0f, accel_c3 = 0.0f, accel_c4 = 0.0f, accel_c5 = 0.0f, accel_c6 = 0.0f, accel_c7 = 0.0f;
  float decel_c0 = 0.0f, decel_c1 = 0.0f, decel_c2 = 0.0f, decel_c3 = 0.0f, decel_c4 = 0.0f, decel_c5 = 0.0f, decel_c6 = 0.0f, decel_c7 = 0.0f;
  float T1 = 0.0f, T2 = 0.0f, T3 = 0.0f;
  float initial_speed_ = 0.0f, nominal_speed_ = 0.0f, final_speed_ = 0.0f;
  float pos_before_coast = 0.0f, pos_after_coast = 0.0f;
};
