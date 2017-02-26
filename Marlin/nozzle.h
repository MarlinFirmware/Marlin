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

#if ENABLED(NOZZLE_CLEAN_FEATURE)
  constexpr float nozzle_clean_start_point[4] = NOZZLE_CLEAN_START_POINT,
                  nozzle_clean_end_point[4] = NOZZLE_CLEAN_END_POINT,
                  nozzle_clean_length = fabs(nozzle_clean_start_point[X_AXIS] - nozzle_clean_end_point[X_AXIS]), //abs x size of wipe pad
                  nozzle_clean_height = fabs(nozzle_clean_start_point[Y_AXIS] - nozzle_clean_end_point[Y_AXIS]); //abs y size of wipe pad
  constexpr bool nozzle_clean_horizontal = nozzle_clean_length >= nozzle_clean_height; //whether to zig-zag horizontally or vertically
#endif //NOZZLE_CLEAN_FEATURE

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
    ) __attribute__((optimize ("Os"))) {
      #if ENABLED(NOZZLE_CLEAN_FEATURE)

        #if ENABLED(NOZZLE_CLEAN_GOBACK)
          // Store the current coords
          point_t const initial = {
            current_position[X_AXIS],
            current_position[Y_AXIS],
            current_position[Z_AXIS],
            current_position[E_AXIS]
          };
        #endif // NOZZLE_CLEAN_GOBACK

        // Move to the starting point
        do_blocking_move_to_xy(start.x, start.y);
        do_blocking_move_to_z(start.z);

        // Start the stroke pattern
        for (uint8_t i = 0; i < (strokes >>1); i++) {
          do_blocking_move_to_xy(end.x, end.y);
          do_blocking_move_to_xy(start.x, start.y);
        }

        #if ENABLED(NOZZLE_CLEAN_GOBACK)
          // Move the nozzle to the initial point
          do_blocking_move_to(initial.x, initial.y, initial.z);
        #endif // NOZZLE_CLEAN_GOBACK

      #endif // NOZZLE_CLEAN_FEATURE
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
    static void zigzag(
      __attribute__((unused)) point_t const &start,
      __attribute__((unused)) point_t const &end,
      __attribute__((unused)) uint8_t const &strokes,
      __attribute__((unused)) uint8_t const &objects
    ) __attribute__((optimize ("Os"))) {
      #if ENABLED(NOZZLE_CLEAN_FEATURE)
        float A = nozzle_clean_horizontal ? nozzle_clean_height : nozzle_clean_length; // [twice the] Amplitude
        float P = ( nozzle_clean_horizontal ? nozzle_clean_length : nozzle_clean_height ) / (objects << 1); // Period

        // Don't allow impossible triangles
        if (A <= 0.0f || P <= 0.0f ) return;

        #if ENABLED(NOZZLE_CLEAN_GOBACK)
          // Store the current coords
          point_t const initial = {
            current_position[X_AXIS],
            current_position[Y_AXIS],
            current_position[Z_AXIS],
            current_position[E_AXIS]
          };
        #endif // NOZZLE_CLEAN_GOBACK

        for (uint8_t j = 0; j < strokes; j++) {
          for (uint8_t i = 0; i < (objects << 1); i++) {
            float const x = start.x + ( nozzle_clean_horizontal ? i * P : (A/P) * (P - fabs(fmod((i*P), (2*P)) - P)) );
            float const y = start.y + (!nozzle_clean_horizontal ? i * P : (A/P) * (P - fabs(fmod((i*P), (2*P)) - P)) );

            do_blocking_move_to_xy(x, y);
            if (i == 0) do_blocking_move_to_z(start.z);
          }

          for (int i = (objects << 1); i > -1; i--) {
            float const x = start.x + ( nozzle_clean_horizontal ? i * P : (A/P) * (P - fabs(fmod((i*P), (2*P)) - P)) );
            float const y = start.y + (!nozzle_clean_horizontal ? i * P : (A/P) * (P - fabs(fmod((i*P), (2*P)) - P)) );

            do_blocking_move_to_xy(x, y);
          }
        }

        #if ENABLED(NOZZLE_CLEAN_GOBACK)
          // Move the nozzle to the initial point
          do_blocking_move_to_z(initial.z);
          do_blocking_move_to_xy(initial.x, initial.y);
        #endif // NOZZLE_CLEAN_GOBACK

      #endif // NOZZLE_CLEAN_FEATURE
    }

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
      __attribute__((unused)) uint8_t const &objects = 0
    ) __attribute__((optimize ("Os"))) {
      #if ENABLED(NOZZLE_CLEAN_FEATURE)
        #if ENABLED(DELTA)
          if (current_position[Z_AXIS] > delta_clip_start_height)
            do_blocking_move_to_z(delta_clip_start_height);
        #endif
        switch (pattern) {
          case 1:
            Nozzle::zigzag(
              NOZZLE_CLEAN_START_POINT,
              NOZZLE_CLEAN_END_POINT, strokes, objects);
            break;

          default:
            Nozzle::stroke(
              NOZZLE_CLEAN_START_POINT,
              NOZZLE_CLEAN_END_POINT, strokes);
        }
      #endif // NOZZLE_CLEAN_FEATURE
    }

    static void park(
      __attribute__((unused)) uint8_t const &z_action
    ) __attribute__((optimize ("Os"))) {
      #if ENABLED(NOZZLE_PARK_FEATURE)
        float const z = current_position[Z_AXIS];
        point_t const park = NOZZLE_PARK_POINT;

        switch(z_action) {
          case 1: // force Z-park height
            do_blocking_move_to_z(park.z);
            break;

          case 2: // Raise by Z-park height
            do_blocking_move_to_z(
              (z + park.z > Z_MAX_POS) ? Z_MAX_POS : z + park.z);
            break;

          default: // Raise to Z-park height if lower
            if (current_position[Z_AXIS] < park.z)
              do_blocking_move_to_z(park.z);
        }

        do_blocking_move_to_xy(park.x, park.y);

      #endif // NOZZLE_PARK_FEATURE
    }
};

#endif
