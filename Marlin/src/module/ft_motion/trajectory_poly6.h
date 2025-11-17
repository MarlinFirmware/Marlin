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
 * 6th order S-Curve trajectory generator.
 * Applies a sextic (position) only to accel/decel phases, coast is flat.
 * - T1/T2/T3 exactly match trapezoid timings
 * - a(mid-phase) = overshoot * a_max (accel)  and  = -overshoot * a_max (decel)
 * - v,a start/end at 0 within each phase; joins are continuous.
 */
class Poly6TrajectoryGenerator : public TrajectoryGenerator {
public:
  Poly6TrajectoryGenerator();

  void plan(const float initial_speed, const float final_speed, const float acceleration, float nominal_speed, const float distance) override;

  void planRunout(const float duration) override;

  float getDistanceAtTime(const float t) const override;

  float getTotalDuration() const override;

  void reset() override;

private:
  // ===== Utilities (position domain) =====
  // Base quintic in position (end accel = 0): s5(u) = s0 + v0*Ts*u + c3 u^3 + c4 u^4 + c5 u^5
  static inline float s5_u(const float s0, const float v0, const float Ts,
                           const float c3, const float c4, const float c5, const float u) {
    const float u2 = sq(u), u3 = u2 * u, u4 = u3 * u, u5 = u4 * u;
    return s0 + v0 * Ts * u + c3 * u3 + c4 * u4 + c5 * u5;
  }
  static inline float s5pp_u(const float c3, const float c4, const float c5, const float u) {
    // d^2/du^2 (c3 u^3 + c4 u^4 + c5 u^5) = 6a u + 12 c4 u^2 + 20 c5 u^3
    return 6.0f * c3 * u + 12.0f * c4 * sq(u) + 20.0f * c5 * cu(u);
  }

  // Shape term K(u) = u^3(1-u)^3 added in position with scalar c
  static inline float K_u(const float /*s0*/, const float /*v0*/, const float /*Ts*/, const float u) {
    const float um1 = 1.0f - u;
    return cu(u) * cu(um1);
  }

  // Timings and kinematics
  float T1 = 0.0f, T2 = 0.0f, T3 = 0.0f;
  float initial_speed = 0.0f, nominal_speed = 0.0f;
  float pos_before_coast = 0.0f, pos_after_coast = 0.0f;

  // New phase polynomials
  float acc_c3 = 0.0f, acc_c4 = 0.0f, acc_c5 = 0.0f, acc_c6 = 0.0f;
  float dec_c3 = 0.0f, dec_c4 = 0.0f, dec_c5 = 0.0f, dec_c6 = 0.0f;
};
