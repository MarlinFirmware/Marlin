/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * polargraph.cpp
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(IS_POLARGRAPH)

#include "polargraph.h"
#include "motion.h"

// For homing:
#include "planner.h"
#include "endstops.h"
#include "../lcd/marlinui.h"
#include "../MarlinCore.h"


float segments_per_second;

void inverse_kinematics(const xyz_pos_t &raw) {
  float y = raw.y-Y_MAX_POS;
    
  float x = raw.x-X_MIN_POS;  // x-xmin
  float a = HYPOT(x,y);

  x = X_MAX_POS-raw.x;  // x-xmax
  float b = HYPOT(x,y);

  delta.set(a,b,raw.z);
}
#endif
