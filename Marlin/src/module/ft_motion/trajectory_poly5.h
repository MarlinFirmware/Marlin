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

#include "trajectory_trapezoidal.h"
#include <math.h>

/**
 * 5th order polynomial trajectory generator.
 * Provides continuous jerk, resulting in smooth acceleration.
 * Acceleration starts and ends at zero. The jerk, snap, and crackle are such that
 * the distance and phase durations match those of a trapezoidal profile.
 */
class Poly5TrajectoryGenerator : public TrapezoidalTrajectoryGenerator {
public:
  Poly5TrajectoryGenerator() = default;

  void plan(const float initial_speed_in, const float final_speed_in, const float acceleration_in, const float nominal_speed_in, const float distance_in) override {
    // Use base class to calculate T1, T2, T3, actual nominal (top) speed and basic positions
    TrapezoidalTrajectoryGenerator::plan(initial_speed_in, final_speed_in, acceleration_in, nominal_speed_in, distance_in);

    const float final_speed = final_speed_in; // just for consistency with the other parameters that otherwise shadow the member variables

    const float d1 = (initial_speed + nominal_speed) * T1 * 0.5f;
    const float T1_2 = sq(T1);
    const float T1_3 = T1_2 * T1;
    const float T1_4 = T1_3 * T1;
    const float T1_5 = T1_4 * T1;
    // acc_c0 = 0.0f; // initial position is zero
    acc_c1 = initial_speed;
    // acc_c2 = 0.0f; // initial acceleration is zero
    acc_c3 = (10.0f * d1 - (6.0f * initial_speed + 4.0f * nominal_speed) * T1) / T1_3;
    acc_c4 = (15.0f * d1 - (8.0f * initial_speed + 7.0f * nominal_speed) * T1) / -T1_4;
    acc_c5 = (6.0f * d1 - 3.0f * (initial_speed + nominal_speed) * T1) / T1_5;
    pos_before_coast = d1;

    // Coast phase - already calculated by base class

    // Deceration phase
    const float d3 = (nominal_speed + final_speed) * T3 * 0.5f;
    const float T3_2 = sq(T3);
    const float T3_3 = T3_2 * T3;
    const float T3_4 = T3_3 * T3;
    const float T3_5 = T3_4 * T3;
    // dec_c0 = 0.0f; // initial position is zero
    dec_c1 = nominal_speed;
    // dec_c2 = 0.0f; // initial acceleration is zero
    dec_c3 = (10.0f * d3 - (6.0f * nominal_speed + 4.0f * final_speed) * T3) / T3_3;
    dec_c4 = (15.0f * d3 - (8.0f * nominal_speed + 7.0f * final_speed) * T3) / -T3_4;
    dec_c5 = (6.0f * d3 - 3.0f * (nominal_speed + final_speed) * T3) / T3_5;
  }

  float getDistanceAtTime(const float t) const override {
    if (t < T1) {
      // Acceration phase
      return t * (acc_c1 + sq(t) * (acc_c3 + t * (acc_c4 + t * acc_c5)));
    }
    else if (t <= T1_plus_T2) {
      // Coasting phase
      return pos_before_coast + nominal_speed * (t - T1);
    }
    // Deceration phase
    const float tau = t - T1_plus_T2;
    return pos_after_coast + tau * (dec_c1 + sq(tau) * (dec_c3 + tau * (dec_c4 + tau * dec_c5)));
  }

  void reset() override {
    acc_c1 = acc_c3 = acc_c4 = acc_c5 = 0.0f;
    dec_c1 = dec_c3 = dec_c4 = dec_c5 = 0.0f;
    TrapezoidalTrajectoryGenerator::reset();
  }

private:
  // c1: initial velocity, c3: jerk, c4: snap, c5: crackle
  // acceleration coefficients
  float acc_c1 = 0.0f, acc_c3 = 0.0f, acc_c4 = 0.0f, acc_c5 = 0.0f;
  // deceleration coefficients
  float dec_c1 = 0.0f, dec_c3 = 0.0f, dec_c4 = 0.0f, dec_c5 = 0.0f;
};
