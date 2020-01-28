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

#include "../inc/MarlinConfig.h"

#if EITHER(NOZZLE_CLEAN_FEATURE, NOZZLE_PARK_FEATURE)

#include "nozzle.h"

Nozzle nozzle;

#include "../MarlinCore.h"
#include "../module/motion.h"

#if ENABLED(NOZZLE_CLEAN_FEATURE)

  /**
   * @brief Stroke clean pattern
   * @details Wipes the nozzle back and forth in a linear movement
   *
   * @param start xyz_pos_t defining the starting point
   * @param end xyz_pos_t defining the ending point
   * @param strokes number of strokes to execute
   */
  void Nozzle::stroke(const xyz_pos_t &start, const xyz_pos_t &end, const uint8_t &strokes) {
    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      const xyz_pos_t oldpos = current_position;
    #endif

    // Move to the starting point
    #if ENABLED(NOZZLE_CLEAN_NO_Z)
      do_blocking_move_to_xy(start);
    #else
      do_blocking_move_to(start);
    #endif

    // Start the stroke pattern
    for (uint8_t i = 0; i < (strokes >> 1); i++) {
      do_blocking_move_to_xy(end);
      do_blocking_move_to_xy(start);
    }

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      do_blocking_move_to(oldpos);
    #endif
  }

  /**
   * @brief Zig-zag clean pattern
   * @details Apply a zig-zag cleaning pattern
   *
   * @param start xyz_pos_t defining the starting point
   * @param end xyz_pos_t defining the ending point
   * @param strokes number of strokes to execute
   * @param objects number of triangles to do
   */
  void Nozzle::zigzag(const xyz_pos_t &start, const xyz_pos_t &end, const uint8_t &strokes, const uint8_t &objects) {
    const xy_pos_t diff = end - start;
    if (!diff.x || !diff.y) return;

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      const xyz_pos_t back = current_position;
    #endif

    #if ENABLED(NOZZLE_CLEAN_NO_Z)
      do_blocking_move_to_xy(start);
    #else
      do_blocking_move_to(start);
    #endif

    const uint8_t zigs = objects << 1;
    const bool horiz = ABS(diff.x) >= ABS(diff.y);    // Do a horizontal wipe?
    const float P = (horiz ? diff.x : diff.y) / zigs; // Period of each zig / zag
    const xyz_pos_t *side;
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
      do_blocking_move_to(back);
    #endif
  }

  /**
   * @brief Circular clean pattern
   * @details Apply a circular cleaning pattern
   *
   * @param start xyz_pos_t defining the middle of circle
   * @param strokes number of strokes to execute
   * @param radius radius of circle
   */
  void Nozzle::circle(const xyz_pos_t &start, const xyz_pos_t &middle, const uint8_t &strokes, const float &radius) {
    if (strokes == 0) return;

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      const xyz_pos_t back = current_position;
    #endif

    #if ENABLED(NOZZLE_CLEAN_NO_Z)
      do_blocking_move_to_xy(start);
    #else
      do_blocking_move_to(start);
    #endif

    for (uint8_t s = 0; s < strokes; s++)
      for (uint8_t i = 0; i < NOZZLE_CLEAN_CIRCLE_FN; i++)
        do_blocking_move_to_xy(
          middle.x + sin((RADIANS(360) / NOZZLE_CLEAN_CIRCLE_FN) * i) * radius,
          middle.y + cos((RADIANS(360) / NOZZLE_CLEAN_CIRCLE_FN) * i) * radius
        );

    // Let's be safe
    do_blocking_move_to_xy(start);

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      do_blocking_move_to(back);
    #endif
  }

  /**
   * @brief Clean the nozzle
   * @details Starts the selected clean procedure pattern
   *
   * @param pattern one of the available patterns
   * @param argument depends on the cleaning pattern
   */
  void Nozzle::clean(const uint8_t &pattern, const uint8_t &strokes, const float &radius, const uint8_t &objects, const uint8_t cleans) {
    xyz_pos_t start[HOTENDS] = NOZZLE_CLEAN_START_POINT, end[HOTENDS] = NOZZLE_CLEAN_END_POINT, middle[HOTENDS] = NOZZLE_CLEAN_CIRCLE_MIDDLE;

    if (pattern == 2) {
      if (!(cleans & (_BV(X_AXIS) | _BV(Y_AXIS)))) {
        SERIAL_ECHOLNPGM("Warning : Clean Circle requires XY");
        return;
      }
    }
    else {
      if (!TEST(cleans, X_AXIS)) start[active_extruder].x = end[active_extruder].x = current_position.x;
      if (!TEST(cleans, Y_AXIS)) start[active_extruder].y = end[active_extruder].y = current_position.y;
    }
    if (!TEST(cleans, Z_AXIS)) start[active_extruder].z = end[active_extruder].z = current_position.z;

    switch (pattern) {
       case 1: zigzag(start[active_extruder], end[active_extruder], strokes, objects); break;
       case 2: circle(start[active_extruder], middle[active_extruder], strokes, radius);  break;
      default: stroke(start[active_extruder], end[active_extruder], strokes);
    }
  }

#endif // NOZZLE_CLEAN_FEATURE

#if ENABLED(NOZZLE_PARK_FEATURE)

  void Nozzle::park(const uint8_t z_action, const xyz_pos_t &park/*=NOZZLE_PARK_POINT*/) {
    constexpr feedRate_t fr_xy = NOZZLE_PARK_XY_FEEDRATE, fr_z = NOZZLE_PARK_Z_FEEDRATE;

    switch (z_action) {
      case 1: // Go to Z-park height
        do_blocking_move_to_z(park.z, fr_z);
        break;

      case 2: // Raise by Z-park height
        do_blocking_move_to_z(_MIN(current_position.z + park.z, Z_MAX_POS), fr_z);
        break;

      default: // Raise to at least the Z-park height
        do_blocking_move_to_z(_MAX(park.z, current_position.z), fr_z);
    }

    do_blocking_move_to_xy(park, fr_xy);

    report_current_position();
  }

#endif // NOZZLE_PARK_FEATURE

#endif // NOZZLE_CLEAN_FEATURE || NOZZLE_PARK_FEATURE
