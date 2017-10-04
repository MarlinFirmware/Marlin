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

#include "nozzle.h"

#include "../Marlin.h"
#include "../module/motion.h"
#include "point_t.h"

#if ENABLED(DELTA)
  #include "../module/delta.h"
#endif

/**
  * @brief Stroke clean pattern
  * @details Wipes the nozzle back and forth in a linear movement
  *
  * @param start point_t defining the starting point
  * @param end point_t defining the ending point
  * @param strokes number of strokes to execute
  */
void Nozzle::stroke(
  _UNUSED point_t const &start,
  _UNUSED point_t const &end,
  _UNUSED uint8_t const &strokes
) {
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
void Nozzle::zigzag(
  _UNUSED point_t const &start,
  _UNUSED point_t const &end,
  _UNUSED uint8_t const &strokes,
  _UNUSED uint8_t const &objects
) {
  #if ENABLED(NOZZLE_CLEAN_FEATURE)
    const float A = nozzle_clean_horizontal ? nozzle_clean_height : nozzle_clean_length, // [twice the] Amplitude
                P = (nozzle_clean_horizontal ? nozzle_clean_length : nozzle_clean_height) / (objects << 1); // Period

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
        float const x = start.x + ( nozzle_clean_horizontal ? i * P : (A/P) * (P - FABS(FMOD((i*P), (2*P)) - P)) );
        float const y = start.y + (!nozzle_clean_horizontal ? i * P : (A/P) * (P - FABS(FMOD((i*P), (2*P)) - P)) );

        do_blocking_move_to_xy(x, y);
        if (i == 0) do_blocking_move_to_z(start.z);
      }

      for (int i = (objects << 1); i > -1; i--) {
        float const x = start.x + ( nozzle_clean_horizontal ? i * P : (A/P) * (P - FABS(FMOD((i*P), (2*P)) - P)) );
        float const y = start.y + (!nozzle_clean_horizontal ? i * P : (A/P) * (P - FABS(FMOD((i*P), (2*P)) - P)) );

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


/**
  * @brief Circular clean pattern
  * @details Apply a circular cleaning pattern
  *
  * @param start point_t defining the middle of circle
  * @param strokes number of strokes to execute
  * @param radius radius of circle
  */
void Nozzle::circle(
  _UNUSED point_t const &start,
  _UNUSED point_t const &middle,
  _UNUSED uint8_t const &strokes,
  _UNUSED float const &radius
) {
  #if ENABLED(NOZZLE_CLEAN_FEATURE)
    if (strokes == 0) return;

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      // Store the current coords
      point_t const initial = {
        current_position[X_AXIS],
        current_position[Y_AXIS],
        current_position[Z_AXIS],
        current_position[E_AXIS]
      };
    #endif // NOZZLE_CLEAN_GOBACK

    if (start.z <= current_position[Z_AXIS]) {
      // Order of movement is pretty darn important here
      do_blocking_move_to_xy(start.x, start.y);
      do_blocking_move_to_z(start.z);
    }
    else {
      do_blocking_move_to_z(start.z);
      do_blocking_move_to_xy(start.x, start.y);
    }

    float x, y;
    for (uint8_t s = 0; s < strokes; s++) {
      for (uint8_t i = 0; i < NOZZLE_CLEAN_CIRCLE_FN; i++) {
        x = middle.x + sin((M_2_PI / NOZZLE_CLEAN_CIRCLE_FN) * i) * radius;
        y = middle.y + cos((M_2_PI / NOZZLE_CLEAN_CIRCLE_FN) * i) * radius;

        do_blocking_move_to_xy(x, y);
      }
    }

    // Let's be safe
    do_blocking_move_to_xy(start.x, start.y);

    #if ENABLED(NOZZLE_CLEAN_GOBACK)
      // Move the nozzle to the initial point
      if (start.z <= initial.z) {
        // As above order is important
        do_blocking_move_to_z(initial.z);
        do_blocking_move_to_xy(initial.x, initial.y);
      }
      else {
        do_blocking_move_to_xy(initial.x, initial.y);
        do_blocking_move_to_z(initial.z);
      }
    #endif // NOZZLE_CLEAN_GOBACK

  #endif // NOZZLE_CLEAN_FEATURE
}

/**
  * @brief Clean the nozzle
  * @details Starts the selected clean procedure pattern
  *
  * @param pattern one of the available patterns
  * @param argument depends on the cleaning pattern
  */
void Nozzle::clean(
  _UNUSED uint8_t const &pattern,
  _UNUSED uint8_t const &strokes,
  _UNUSED float const &radius,
  _UNUSED uint8_t const &objects
) {
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

      case 2:
        Nozzle::circle(
          NOZZLE_CLEAN_START_POINT,
          NOZZLE_CLEAN_CIRCLE_MIDDLE, strokes, radius);
        break;

      default:
        Nozzle::stroke(
          NOZZLE_CLEAN_START_POINT,
          NOZZLE_CLEAN_END_POINT, strokes);
    }
  #endif // NOZZLE_CLEAN_FEATURE
}

void Nozzle::park(
  _UNUSED uint8_t const &z_action
) {
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
