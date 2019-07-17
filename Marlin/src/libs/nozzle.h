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
#pragma once

#include "../inc/MarlinConfig.h"
#include "point_t.h"

/**
 * @brief Nozzle class
 *
 * @todo: Do not ignore the end.z value and allow XYZ movements
 */
class Nozzle {
  private:

  #if ENABLED(NOZZLE_CLEAN_FEATURE)

    /**
     * @brief Stroke clean pattern
     * @details Wipes the nozzle back and forth in a linear movement
     *
     * @param start point_t defining the starting point
     * @param end point_t defining the ending point
     * @param strokes number of strokes to execute
     */
    static void stroke(const point_t &start, const point_t &end, const uint8_t &strokes) _Os;

    /**
     * @brief Zig-zag clean pattern
     * @details Apply a zig-zag cleaning pattern
     *
     * @param start point_t defining the starting point
     * @param end point_t defining the ending point
     * @param strokes number of strokes to execute
     * @param objects number of objects to create
     */
    static void zigzag(const point_t &start, const point_t &end, const uint8_t &strokes, const uint8_t &objects) _Os;

    /**
     * @brief Circular clean pattern
     * @details Apply a circular cleaning pattern
     *
     * @param start point_t defining the middle of circle
     * @param strokes number of strokes to execute
     * @param radius radius of circle
     */
    static void circle(const point_t &start, const point_t &middle, const uint8_t &strokes, const float &radius) _Os;

  #endif // NOZZLE_CLEAN_FEATURE

  public:

  #if ENABLED(NOZZLE_CLEAN_FEATURE)

    /**
     * @brief Clean the nozzle
     * @details Starts the selected clean procedure pattern
     *
     * @param pattern one of the available patterns
     * @param argument depends on the cleaning pattern
     */
    static void clean(const uint8_t &pattern, const uint8_t &strokes, const float &radius, const uint8_t &objects, const uint8_t cleans) _Os;

  #endif // NOZZLE_CLEAN_FEATURE

  #if ENABLED(NOZZLE_PARK_FEATURE)

    static void park(const uint8_t z_action, const point_t &park=NOZZLE_PARK_POINT) _Os;

  #endif
};

extern Nozzle nozzle;
