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

#include <stdint.h>

/**
 * Base class for trajectory generators.
 * Provides a zero-cost abstraction for different trajectory generation algorithms.
    | Feature | Trapezoidal | Poly3 | Poly5 | S-Curve (Poly6) | Poly7 |
    | :--- | :--- | :--- | :--- | :--- | :--- |
    | **Position Order** | 2nd (piecewise) | 3rd | 5th | 6th | 7th |
    | **Constraints** | `p(0)=0` `p(T)=d` `v(0)=vᵢ` `v(T)=vₒ` `a(t)=const` | `p(0)=0` `p(T)=d` `v(0)=vᵢ` `v(T)=vₒ` | `p(0)=0` `p(T)=d` `v(0)=vᵢ` `v(T)=vₒ` `a(0)=0` `a(T)=0` | `p(0)=0` `p(T)=d` `v(0)=vᵢ` `v(T)=vₒ` `a(0)=0` `a(T)=0` `j(0)=0` `j(T)=0` | `p(0)=0` `p(T)=d` `v(0)=vᵢ` `v(T)=vₒ` `a(0)=0` `a(T)=0` `j(0)=0` `j(T)=0` |
    | **Velocity** | Continuous | Continuous | Continuous | Continuous | Continuous |
    | **Acceleration** | Discontinuous | Discontinuous | Continuous | Continuous | Continuous |
    | **Jerk** | Infinite | Infinite | Discontinuous | Continuous | Continuous |
    | **Snap** | Infinite | Infinite | Infinite | Discontinuous | Continuous |
    | **Peak Accel.** | **1.0x** (0%) | **2.0x** (+100%) | **1.95x** (+95%) | **1.875x** (+87.5%) | **2.1875x** (+118.75%) |
    | **"Smoothness"** | Basic | Smooth | Very Smooth | Extremely Smooth | Smoothest |

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
  virtual void plan(float initial_speed, float final_speed, float acceleration, float nominal_speed, float distance) = 0;

  /**
   * Plan a zero-motion trajectory for a specific duration.
   * @param duration The total time for the runout phase [s]
   */
  virtual void planRunout(float duration) = 0;

  /**
   * Get the distance traveled at time t.
   * @param t Time since start of trajectory [s]
   * @return Distance traveled [mm]
   */
  virtual float getDistanceAtTime(float t) const = 0;

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
enum class TrajectoryType : uint8_t {
  TRAPEZOIDAL = 0,
  POLY3 = 1,
  POLY5 = 2,
  POLY7 = 3,
  S_CURVE = 4
};
