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

#ifndef __NOZZLE_H__
#define __NOZZLE_H__

#include "Marlin.h"
#include "point_t.h"

#if ENABLED(NOZZLE_CLEAN_FEATURE)
  constexpr float nozzle_clean_start_point[4] = NOZZLE_CLEAN_START_POINT,
                  nozzle_clean_end_point[4] = NOZZLE_CLEAN_END_POINT,
                  nozzle_clean_length = fabs(nozzle_clean_start_point[X_AXIS] - nozzle_clean_end_point[X_AXIS]), //abs x size of wipe pad
                  nozzle_clean_height = fabs(nozzle_clean_start_point[Y_AXIS] - nozzle_clean_end_point[Y_AXIS]); //abs y size of wipe pad
  constexpr bool nozzle_clean_horizontal = nozzle_clean_length >= nozzle_clean_height; //whether to zig-zag horizontally or vertically
#endif // NOZZLE_CLEAN_FEATURE

/**
 * @brief Nozzle class
 *
 * @todo: Do not ignore the end.z value and allow XYZ movements
 */
class Nozzle {
  private:
    /**
     * @brief Stroke clean pattern
     * @details Wipes the nozzle back and forth in a linear movement
     *
     * @param start point_t defining the starting point
     * @param end point_t defining the ending point
     * @param strokes number of strokes to execute
     */
    static void stroke(
      __attribute__((unused)) point_t const &start,
      __attribute__((unused)) point_t const &end,
      __attribute__((unused)) uint8_t const &strokes
    ) __attribute__((optimize ("Os")));

    /**
     * @brief Zig-zag clean pattern
     * @details Apply a zig-zag cleaning pattern
     *
     * @param start point_t defining the starting point
     * @param end point_t defining the ending point
     * @param strokes number of strokes to execute
     * @param objects number of objects to create
     */
    static void zigzag(
      __attribute__((unused)) point_t const &start,
      __attribute__((unused)) point_t const &end,
      __attribute__((unused)) uint8_t const &strokes,
      __attribute__((unused)) uint8_t const &objects
    ) __attribute__((optimize ("Os")));

    /**
     * @brief Circular clean pattern
     * @details Apply a circular cleaning pattern
     *
     * @param start point_t defining the middle of circle
     * @param strokes number of strokes to execute
     * @param radius radius of circle
     */
    static void circle(
      __attribute__((unused)) point_t const &start,
      __attribute__((unused)) point_t const &middle,
      __attribute__((unused)) uint8_t const &strokes,
      __attribute__((unused)) float const &radius
    ) __attribute__((optimize ("Os")));

  public:
    /**
     * @brief Clean the nozzle
     * @details Starts the selected clean procedure pattern
     *
     * @param pattern one of the available patterns
     * @param argument depends on the cleaning pattern
     */
    static void clean(
      __attribute__((unused)) uint8_t const &pattern,
      __attribute__((unused)) uint8_t const &strokes,
      __attribute__((unused)) float const &radius,
      __attribute__((unused)) uint8_t const &objects = 0
    ) __attribute__((optimize ("Os")));

    static void park(
      __attribute__((unused)) uint8_t const &z_action
    ) __attribute__((optimize ("Os")));
};

#endif
