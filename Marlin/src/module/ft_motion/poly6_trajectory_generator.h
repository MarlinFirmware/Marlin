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
 * 6th order S-Curve trajectory generator.
 * Provides a smooth, jerk-limited motion profile that matches the classic
 * Marlin S-Curve implementation.
 * Acceleration and jerk start and end at zero, snap, crackle and pop are such that
 * the distance and phase durations match those of a trapezoidal profile.
 */
class Poly6TrajectoryGenerator : public TrajectoryGenerator {
public:
  Poly6TrajectoryGenerator() = default;

  void plan(float initial_speed, float final_speed, float acceleration, float nominal_speed, float distance) override {
    this->initial_speed = initial_speed;
    this->nominal_speed = nominal_speed;

    // Calculate timing phases using the same logic as trapezoidal generator
    const float one_over_accel = 1.0f / acceleration;
    const float ldiff = distance + 0.5f * one_over_accel * (sq(this->initial_speed) + sq(final_speed));

    T2 = ldiff / this->nominal_speed - one_over_accel * this->nominal_speed;
    if (T2 < 0.0f) {
      T2 = 0.0f;
      this->nominal_speed = sqrtf(ldiff * acceleration);
    }

    T1 = (this->nominal_speed - this->initial_speed) * one_over_accel;
    T3 = (this->nominal_speed - final_speed) * one_over_accel;

    // Acceleration phase (5th-order velocity -> 6th-order position)
    // v(t) = (v_f - v_i) * (6t^5 - 15t^4 + 10t^3) + v_i
    // s(t) = ∫v(t)dt = (v_f - v_i) * (t^6 - 3t^5 + (10/4)t^4) + v_i*t
    const float dv1 = this->nominal_speed - this->initial_speed;
    // acc_c0 = 0.0f; // initial position is zero
    acc_c1 = this->initial_speed;
    // acc_c2 = 0.0f; // initial acceleration is zero
    // acc_c3 = 0.0f; // initial jerk is zero
    acc_c4 = dv1 * 2.5f / (T1 * T1 * T1);
    acc_c5 = dv1 * -3.0f / (T1 * T1 * T1 * T1);
    acc_c6 = dv1 * 1.0f / (T1 * T1 * T1 * T1 * T1);
    pos_before_coast = (this->initial_speed + this->nominal_speed) * T1 * 0.5f;

    // Coast phase
    pos_after_coast = pos_before_coast + this->nominal_speed * T2;

    // Deceleration phase
    const float dv2 = final_speed - this->nominal_speed;
    // dec_c0 = 0.0f; // initial position is zero
    dec_c1 = this->nominal_speed;
    // dec_c2 = 0.0f; // initial acceleration is zero
    // dec_c3 = 0.0f; // initial jerk is zero
    dec_c4 = dv2 * 2.5f / (T3 * T3 * T3);
    dec_c5 = dv2 * -3.0f / (T3 * T3 * T3 * T3);
    dec_c6 = dv2 * 1.0f / (T3 * T3 * T3 * T3 * T3);
  }

  void planRunout(float duration) override {
    reset();
    T2 = duration;
  }

  float getDistanceAtTime(float t) const override {
    if (t < T1) {
        // Acceleration phase
        return t * (acc_c1 + t * t * t * (acc_c4 + t * (acc_c5 + t * acc_c6)));
    } else if (t <= (T1 + T2)) {
        // Coasting phase
        return pos_before_coast + this->nominal_speed * (t - T1);
    }
    // Deceleration phase
    const float tau = t - (T1 + T2);
    return pos_after_coast + tau * (dec_c1 + tau * tau * tau * (dec_c4 + tau * (dec_c5 + tau * dec_c6)));
  }

  float getTotalDuration() const override { return T1 + T2 + T3; }

  void reset() override {
    acc_c1 = acc_c4 = acc_c5 = acc_c6 = 0.0f;
    dec_c1 = dec_c4 = dec_c5 = dec_c6 = 0.0f;
    T1 = T2 = T3 = 0.0f;
    initial_speed = nominal_speed = 0.0f;
    pos_before_coast = pos_after_coast = 0.0f;
  }

private:
  // c1: initial velocity, c4: snap, c5: crackle, c6: pop
  // acceleration coefficients
  float acc_c1 = 0.0f, acc_c4 = 0.0f, acc_c5 = 0.0f, acc_c6 = 0.0f;
  // deceleration coefficients
  float dec_c1 = 0.0f, dec_c4 = 0.0f, dec_c5 = 0.0f, dec_c6 = 0.0f;
  float T1 = 0.0f, T2 = 0.0f, T3 = 0.0f;
  float initial_speed = 0.0f, nominal_speed = 0.0f;
  float pos_before_coast = 0.0f, pos_after_coast = 0.0f;
};
