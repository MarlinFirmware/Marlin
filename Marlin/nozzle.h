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

#ifndef __NOZZLE_H__
#define __NOZZLE_H__

#include "Marlin.h"
#include "point_t.h"

/**
 * @brief Nozzle class
 *
 * @todo: Do not ignore the end.z value and allow XYZ movements
 * @todo: Currently this feature needs HAS_BED_PROBE to be active
 *  due to the do_blocking_move_to*() functions.
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
    static void stroke(point_t const &start, point_t const &end, uint8_t const &strokes)
    __attribute__ ((optimize ("Os"))) {

      #if ENABLED(NOZZLE_CLEAN_PARK)
        // Store the current coords
        point_t const initial = {
          current_position[X_AXIS],
          current_position[Y_AXIS],
          current_position[Z_AXIS],
          current_position[E_AXIS]
        };
      #endif

      // Move to the starting point
      do_blocking_move_to_xy(start.x, start.y);
      do_blocking_move_to_z(start.z);

      // Start the stroke pattern
      for (uint8_t i = 0; i < (strokes >>1); i++) {
        do_blocking_move_to_xy(end.x, end.y);
        do_blocking_move_to_xy(start.x, start.y);
      }

      #if ENABLED(NOZZLE_CLEAN_PARK)
        // Move the nozzle to the initial point
        do_blocking_move_to_z(initial.z);
        do_blocking_move_to_xy(initial.x, initial.y);
      #endif
    }

    /**
     * @brief Zig-zag clean pattern
     * @details Apply a zig-zag cleanning pattern
     *
     * @param start point_t defining the starting point
     * @param end point_t defining the ending point
     * @param strokes number of strokes to execute
     * @param objects number of objects to create
     */
    static void zigzag(point_t const &start,
      point_t const &end, uint8_t const &strokes, uint8_t const &objects)
    __attribute__ ((optimize ("Os"))) {
      float A = fabs(end.y - start.y); // [twice the] Amplitude
      float P = fabs(end.x - start.x) / (objects << 1); // Period

      // Don't allow impossible triangles
      if (A <= 0.0f || P <= 0.0f ) return;

      #if ENABLED(NOZZLE_CLEAN_PARK)
        // Store the current coords
        point_t const initial = {
          current_position[X_AXIS],
          current_position[Y_AXIS],
          current_position[Z_AXIS],
          current_position[E_AXIS]
        };
      #endif

      for (uint8_t j = 0; j < strokes; j++) {
        for (uint8_t i = 0; i < (objects << 1); i++) {
          float const x = start.x + i * P;
          float const y = start.y + (A/P) * (P - fabs(fmod((i*P), (2*P)) - P));

          do_blocking_move_to_xy(x, y);
          if (i == 0) do_blocking_move_to_z(start.z);
        }

        for (int i = (objects << 1); i > -1; i--) {
          float const x = start.x + i * P;
          float const y = start.y + (A/P) * (P - fabs(fmod((i*P), (2*P)) - P));

          do_blocking_move_to_xy(x, y);
        }
      }

      #if ENABLED(NOZZLE_CLEAN_PARK)
        // Move the nozzle to the initial point
        do_blocking_move_to_z(initial.z);
        do_blocking_move_to_xy(initial.x, initial.y);
      #endif
    }

  public:
    /**
     * @brief Clean the nozzle
     * @details Starts the selected clean procedure pattern
     *
     * @param pattern one of the available patterns
     * @param argument depends on the cleaning pattern
     */
    static void clean(uint8_t const &pattern,
      uint8_t const &strokes, uint8_t const &objects = 0)
    __attribute__ ((optimize ("Os"))) {
      switch (pattern) {
        case 1:
          Nozzle::zigzag(
            NOZZLE_CLEAN_START_PT,
            NOZZLE_CLEAN_END_PT, strokes, objects);
          break;

        default:
          Nozzle::stroke(
            NOZZLE_CLEAN_START_PT,
            NOZZLE_CLEAN_END_PT, strokes);
      }
    }
};

#endif
