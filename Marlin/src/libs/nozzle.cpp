/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#if EITHER(NOZZLE_CLEAN_FEATURE, NOZZLE_PARK_FEATURE)

#include "nozzle.h"

#include "../Marlin.h"
#include "../module/motion.h"
#include "point_t.h"

#if ENABLED(NOZZLE_CLEAN_FEATURE)

  /**
   * @brief Stroke clean pattern
   * @details Wipes the nozzle back and forth in a linear movement
   *
   * @param start point_t defining the starting point
   * @param end point_t defining the ending point
   * @param strokes number of strokes to execute
   */
  void Nozzle::stroke(const point_t &start, const point_t &end, const uint8_t &strokes) {
    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      const float ix = current_position[X_AXIS], iy = current_position[Y_AXIS], iz = current_position[Z_AXIS];
    #endif

    // Move to the starting point
    do_blocking_move_to(start.x, start.y, start.z);

    // Start the stroke pattern
    for (uint8_t i = 0; i < (strokes >> 1); i++) {
      do_blocking_move_to_xy(end.x, end.y);
      do_blocking_move_to_xy(start.x, start.y);
    }

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      do_blocking_move_to(ix, iy, iz);
    #endif
  }

  /**
   * @brief Zig-zag clean pattern
   * @details Apply a zig-zag cleaning pattern
   *
   * @param start point_t defining the starting point
   * @param end point_t defining the ending point
   * @param strokes number of strokes to execute
   * @param objects number of triangles to do
   */
  void Nozzle::zigzag(const point_t &start, const point_t &end, const uint8_t &strokes, const uint8_t &objects) {
    const float diffx = end.x - start.x, diffy = end.y - start.y;
    if (!diffx || !diffy) return;

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      const float ix = current_position[X_AXIS], iy = current_position[Y_AXIS], iz = current_position[Z_AXIS];
    #endif

    do_blocking_move_to(start.x, start.y, start.z);

    const uint8_t zigs = objects << 1;
    const bool horiz = ABS(diffx) >= ABS(diffy);    // Do a horizontal wipe?
    const float P = (horiz ? diffx : diffy) / zigs;   // Period of each zig / zag
    const point_t *side;
    for (uint8_t j = 0; j < strokes; j++) {
      for (int8_t i = 0; i < zigs; i++) {
        side = (i & 1) ? &end : &start;
        if (horiz)
          do_blocking_move_to_xy(start.x + i * P, side->y);
        else
          do_blocking_move_to_xy(side->x, start.y + i * P);
      }
      for (int8_t i = zigs; i >= 0; i--) {
        side = (i & 1) ? &end : &start;
        if (horiz)
          do_blocking_move_to_xy(start.x + i * P, side->y);
        else
          do_blocking_move_to_xy(side->x, start.y + i * P);
      }
    }

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      do_blocking_move_to(ix, iy, iz);
    #endif
  }

  /**
   * @brief Circular clean pattern
   * @details Apply a circular cleaning pattern
   *
   * @param start point_t defining the middle of circle
   * @param strokes number of strokes to execute
   * @param radius radius of circle
   */
  void Nozzle::circle(const point_t &start, const point_t &middle, const uint8_t &strokes, const float &radius) {
    if (strokes == 0) return;

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      const float ix = current_position[X_AXIS], iy = current_position[Y_AXIS], iz = current_position[Z_AXIS];
    #endif

    do_blocking_move_to(start.x, start.y, start.z);

    for (uint8_t s = 0; s < strokes; s++)
      for (uint8_t i = 0; i < NOZZLE_CLEAN_CIRCLE_FN; i++)
        do_blocking_move_to_xy(
          middle.x + sin((RADIANS(360) / NOZZLE_CLEAN_CIRCLE_FN) * i) * radius,
          middle.y + cos((RADIANS(360) / NOZZLE_CLEAN_CIRCLE_FN) * i) * radius
        );

    // Let's be safe
    do_blocking_move_to_xy(start.x, start.y);

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      do_blocking_move_to(ix, iy, iz);
    #endif
  }

  /**
   * @brief Clean the nozzle
   * @details Starts the selected clean procedure pattern
   *
   * @param pattern one of the available patterns
   * @param argument depends on the cleaning pattern
   */
  void Nozzle::clean(const uint8_t &pattern, const uint8_t &strokes, const float &radius, const uint8_t &objects/*=0*/) {
    switch (pattern) {
      case 1:
        zigzag(NOZZLE_CLEAN_START_POINT, NOZZLE_CLEAN_END_POINT, strokes, objects);
        break;

      case 2:
        circle(NOZZLE_CLEAN_START_POINT, NOZZLE_CLEAN_CIRCLE_MIDDLE, strokes, radius);
        break;

      default:
        stroke(NOZZLE_CLEAN_START_POINT, NOZZLE_CLEAN_END_POINT, strokes);
    }
  }

#endif // NOZZLE_CLEAN_FEATURE

#if ENABLED(NOZZLE_PARK_FEATURE)

  void Nozzle::park(const uint8_t z_action, const point_t &park /*= NOZZLE_PARK_POINT*/) {
    const float fr_xy = NOZZLE_PARK_XY_FEEDRATE;
    const float fr_z = NOZZLE_PARK_Z_FEEDRATE;

    switch (z_action) {
      case 1: // Go to Z-park height
        do_blocking_move_to_z(park.z, fr_z);
        break;

      case 2: // Raise by Z-park height
        do_blocking_move_to_z(MIN(current_position[Z_AXIS] + park.z, Z_MAX_POS), fr_z);
        break;

      default: // Raise to at least the Z-park height
        do_blocking_move_to_z(MAX(park.z, current_position[Z_AXIS]), fr_z);
    }

    do_blocking_move_to_xy(park.x, park.y, fr_xy);

    report_current_position();
  }

#endif // NOZZLE_PARK_FEATURE

#endif // NOZZLE_CLEAN_FEATURE || NOZZLE_PARK_FEATURE
