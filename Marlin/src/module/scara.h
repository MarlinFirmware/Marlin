/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * scara.h - SCARA-specific functions
 */

#pragma once

#include "../core/macros.h"

extern float delta_segments_per_second;

// Float constants for SCARA calculations
float constexpr L1 = SCARA_LINKAGE_1, L2 = SCARA_LINKAGE_2,
                L1_2 = sq(float(L1)), L1_2_2 = 2.0 * L1_2,
                L2_2 = sq(float(L2));

void scara_set_axis_is_at_home(const AxisEnum axis);

void inverse_kinematics(const float (&raw)[XYZ]);
FORCE_INLINE void inverse_kinematics(const float (&raw)[XYZE]) {
  const float raw_xyz[XYZ] = { raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS] };
  inverse_kinematics(raw_xyz);
}
void forward_kinematics_SCARA(const float &a, const float &b);

void scara_report_positions();
