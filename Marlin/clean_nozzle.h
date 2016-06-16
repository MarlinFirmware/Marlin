/*
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

#ifndef __CLEAN_NOZZLE_H__
#define __CLEAN_NOZZLE_H__

#include "Marlin.h"
#include "point_t.h"

/**
 * @brief CleanNozzle class
 *
 * @todo: Do not ignore the end.z value and allow XYZ movements
 * @todo: Currently this feature needs AUTO_BED_LEVELING_FEATURE to be active
 *  due to the do_blocking_move_to*() functions.
 */
class CleanNozzle {
  private:
    /**
     * @brief Stroke clean pattern
     * @details Wipes the nozzle back and forth in a linear movement
     *
     * @param start point_t defining the starting point
     * @param end point_t defining the ending point
     * @param strokes number of strokes to execute
     */
    static void stroke(point_t const &start, point_t const &end, uint8_t const &strokes)
    __attribute__ ((optimize ("Os"))) {
      // Move to the starting point
      do_blocking_move_to_xy(start.x, start.y);
      do_blocking_move_to_z(start.z);

      // Start the stroke pattern
      for (uint8_t i = 0; i < (strokes >>1); i++) {
        do_blocking_move_to_xy(end.x, end.y);
        do_blocking_move_to_xy(start.x, start.y);
      }
    }

    /**
     * @brief Zig-zag clean pattern
     * @details Apply a zig-zag cleanning pattern
     *
     * @param start point_t defining the starting point
     * @param end point_t defining the ending point
     * @param triangles number of triangles to execute
     */
    static void zigzag(point_t const &start, point_t const &end, uint8_t const &triangles)
    __attribute__ ((optimize ("Os"))) {
      // Move to the starting point
      do_blocking_move_to_xy(start.x, start.y);
      do_blocking_move_to_z(start.z);

      // Calculate the triangle side
      float const a = fabs(end.x - start.x) / triangles;

      // Don't allow the sides (a, b) to be smaller than 5mm
      if (a < 5 || fabs(end.y - start.y) < 5) return;

      // Start the zig-zag pattern
      for (uint8_t i = 0; i < triangles; i++) {
        float const x = start.x + (a * (i + 1));
        do_blocking_move_to_xy(x, end.y);
        do_blocking_move_to_y(start.y);
      }
    }

  public:
    /**
     * @brief Clean the nozzle
     * @details Starts the selected clean procedure pattern
     *
     * @param pattern one of the available patterns
     * @param argument depends on the cleaning pattern
     */
    static void start(uint8_t const &pattern, uint8_t const &argument)
    __attribute__ ((optimize ("Os"))) {
      switch (pattern) {
        case 1:
          CleanNozzle::zigzag(
            CLEAN_NOZZLE_START_PT,
            CLEAN_NOZZLE_END_PT, argument);
          break;

        default:
          CleanNozzle::stroke(
            CLEAN_NOZZLE_START_PT,
            CLEAN_NOZZLE_END_PT, argument);
      }
    }
};

#endif
