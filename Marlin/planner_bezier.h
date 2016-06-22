/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * planner_bezier.h
 *
 * Compute and buffer movement commands for bezier curves
 *
 */

#ifndef PLANNER_BEZIER_H
#define PLANNER_BEZIER_H

#include "Marlin.h"

void cubic_b_spline(
              const float position[NUM_AXIS], // current position
              const float target[NUM_AXIS],   // target position
              const float offset[4],          // a pair of offsets
              float feed_rate,
              uint8_t extruder
            );

#endif // PLANNER_BEZIER_H
