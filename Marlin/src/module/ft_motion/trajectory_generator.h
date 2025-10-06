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

#include "../../inc/MarlinConfig.h"

#include <stdint.h>

/**
 * Base class for trajectory generators.
 * Provides a zero-cost abstraction for different trajectory generation algorithms.
 */
class TrajectoryGenerator {
public:
  /**
   * Plan a trajectory with the given parameters.
   * @param initial_speed Starting feedrate [mm/s]
   * @param final_speed Ending feedrate [mm/s]
   * @param acceleration Acceleration [mm/s²]
   * @param nominal_speed Peak feedrate [mm/s]
   * @param distance Total distance to travel [mm]
   */
  virtual void plan(const float initial_speed, const float final_speed, const float acceleration, const float nominal_speed, const float distance) = 0;

  /**
   * Plan a zero-motion trajectory for a specific duration.
   * @param duration The total time for the runout phase [s]
   */
  virtual void planRunout(const float duration) = 0;

  /**
   * Get the distance traveled at time t.
   * @param t Time since start of trajectory [s]
   * @return Distance traveled [mm]
   */
  virtual float getDistanceAtTime(const float t) const = 0;

  /**
   * Get the total duration of the trajectory.
   * @return Total time [s]
   */
  virtual float getTotalDuration() const = 0;

  /**
   * Reset the trajectory generator to initial state.
   */
  virtual void reset() = 0;

protected:
  // Protected constructor to prevent direct instantiation
  TrajectoryGenerator() = default;
  virtual ~TrajectoryGenerator() = default;
};

/**
 * Trajectory generator types for runtime selection
 */
enum class TrajectoryType : uint8_t { TRAPEZOIDAL, POLY5, POLY6 };
