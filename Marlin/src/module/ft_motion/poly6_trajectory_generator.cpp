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

#include "../../inc/MarlinConfig.h"

#if ENABLED(FT_MOTION)

#include "poly6_trajectory_generator.h"
#include <math.h>
#include "../ft_motion.h"

Poly6TrajectoryGenerator::Poly6TrajectoryGenerator() {}

void Poly6TrajectoryGenerator::plan(const float initial_speed, const float final_speed, const float acceleration, float nominal_speed, const float distance) {
  this->initial_speed = initial_speed;
  this->nominal_speed = nominal_speed;

  // --- Trapezoid timings (unchanged) ---
  const float invA = 1.0f / acceleration;
  const float ldiff = distance + 0.5f * invA * (sq(this->initial_speed) + sq(final_speed));

  T2 = ldiff / this->nominal_speed - invA * this->nominal_speed;
  if (T2 < 0.0f) {
    T2 = 0.0f;
    this->nominal_speed = sqrtf(ldiff * acceleration);
  }

  T1 = (this->nominal_speed - this->initial_speed) * invA;
  T3 = (this->nominal_speed - final_speed) * invA;

  // Distances at phase boundaries (trapezoid areas)
  pos_before_coast = 0.5f * (this->initial_speed + this->nominal_speed) * T1;
  pos_after_coast  = pos_before_coast + this->nominal_speed * T2;

  // --- Build sextic (in position) for each phase ---
  // We start from a quintic-in-position s5(u) that meets endpoints with a(0)=a(1)=0,
  // then add c*K(u) where K(u) = u^3(1-u)^3 to hit a_mid target exactly.
  // Parameterization: u=t/Tphase  in [0,1].

  // Common mid values for K''(u)
  constexpr float Kpp_mid = 6 * 0.5f - 36 * 0.25f + 60 * 0.125f - 30 * 0.0625f; // = 1.875f

  // ---- Accel phase ----
  {
    const float Ts = T1;
    const float s0 = 0.0f;
    const float v0 = this->initial_speed;
    const float s1 = pos_before_coast;
    const float v1 = this->nominal_speed;

    const float delta_p = s1 - s0 - v0 * Ts;
    const float delta_v = (v1 - v0) * Ts;

    // s5(u) = s0 + v0*Ts*u + c3 u^3 + c4 u^4 + c5 u^5
    acc_c3 = 10.0f * delta_p - 4.0f * delta_v;
    acc_c4 = -15.0f * delta_p + 7.0f * delta_v;
    acc_c5 =  6.0f * delta_p - 3.0f * delta_v;

    // a5_mid = s5''(0.5)/Ts^2
    const float a5_mid = s5pp_u(acc_c3, acc_c4, acc_c5, 0.5f) / (Ts*Ts);
    const float a_mid_target = ftMotion.cfg.poly6_acceleration_overshoot * acceleration;
    // c chosen so that (s5''(0.5)+c5*K''(0.5))/Ts^2 == a_mid_target, meaning at half the acc/decc phase
    acc_c6 = (Ts*Ts) * (a_mid_target - a5_mid) / Kpp_mid;
  }

  // ---- Decel phase ----
  {
    const float Ts = T3;
    const float s0 = pos_after_coast;
    const float v0 = this->nominal_speed;
    const float s1 = pos_after_coast + 0.5f * (this->nominal_speed + final_speed) * T3;
    const float v1 = final_speed;

    const float delta_p = s1 - s0 - v0 * Ts;
    const float delta_v = (v1 - v0) * Ts;

    dec_c3 = 10.0f * delta_p - 4.0f * delta_v;
    dec_c4  = -15.0f * delta_p + 7.0f * delta_v;
    dec_c5 =  6.0f * delta_p - 3.0f * delta_v;

    const float a5_mid = s5pp_u(dec_c3, dec_c4, dec_c5, 0.5f) / sq(Ts);
    const float a_mid_target = -ftMotion.cfg.poly6_acceleration_overshoot * acceleration;
    dec_c6 = sq(Ts) * (a_mid_target - a5_mid) / Kpp_mid;
  }

}

void Poly6TrajectoryGenerator::planRunout(const float duration) {
  reset();
  T2 = duration;
}

float Poly6TrajectoryGenerator::getDistanceAtTime(const float t) const {
  if (t < T1) {
    // Accel phase: u=t/T1
    const float u = t / T1;
    return s5_u(0.0f, initial_speed, T1, acc_c3, acc_c4, acc_c5, u)
         + acc_c6 * K_u(0.0f, initial_speed, T1, u); // K added as pure shape (position domain)
  }
  else if (t <= (T1 + T2)) {
    // Coast
    return pos_before_coast + this->nominal_speed * (t - T1);
  }
  // Decel phase
  const float tau = t - (T1 + T2),
              u = tau / T3;
  return s5_u(pos_after_coast, this->nominal_speed, T3, dec_c3, dec_c4, dec_c5, u)
       + dec_c6 * K_u(pos_after_coast, this->nominal_speed, T3, u);
}

float Poly6TrajectoryGenerator::getTotalDuration() const { return T1 + T2 + T3; }

void Poly6TrajectoryGenerator::reset() {
  T1 = T2 = T3 = 0.0f;
  initial_speed = nominal_speed = 0.0f;
  pos_before_coast = pos_after_coast = 0.0f;

  acc_c3 = acc_c4 = acc_c5 = 0.0f;
  dec_c3 = dec_c4 = dec_c5 = 0.0f;
  acc_c6 = dec_c6 = 0.0f;
}

#endif // FT_MOTION
