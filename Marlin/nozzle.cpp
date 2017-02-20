#include "nozzle.h"

#include "Marlin.h"
#include "point_t.h"

/**
  * @brief Stroke clean pattern
  * @details Wipes the nozzle back and forth in a linear movement
  *
  * @param start point_t defining the starting point
  * @param end point_t defining the ending point
  * @param strokes number of strokes to execute
  */
void Nozzle::stroke(
  __attribute__((unused)) point_t const &start,
  __attribute__((unused)) point_t const &end,
  __attribute__((unused)) uint8_t const &strokes
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
  __attribute__((unused)) point_t const &start,
  __attribute__((unused)) point_t const &end,
  __attribute__((unused)) uint8_t const &strokes,
  __attribute__((unused)) uint8_t const &objects
) {
  #if ENABLED(NOZZLE_CLEAN_FEATURE)
    float const A =  nozzle_clean_horizontal ? nozzle_clean_height : nozzle_clean_length, // [twice the] Amplitude
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
      for (uint8_t i = 0; i < objects << 1; i++) {
        float const m1 = i * P,
                    m2 = (A / P) * (P - FABS(FMOD(m1, (2 * P)) - P)),
                    x = start.x + (nozzle_clean_horizontal ? m1 : m2),
                    y = start.y + (nozzle_clean_horizontal ? m2 : m1);
        do_blocking_move_to_xy(x, y);
        if (i == 0) do_blocking_move_to_z(start.z);
      }

      for (int8_t i = objects << 1; i >= 0; i--) {
        float const m1 = i * P,
                    m2 = (A / P) * (P - FABS(FMOD(m1, (2 * P)) - P)),
                    x = start.x + (nozzle_clean_horizontal ? m1 : m2),
                    y = start.y + (nozzle_clean_horizontal ? m2 : m1);
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
  __attribute__((unused)) point_t const &start,
  __attribute__((unused)) point_t const &middle,
  __attribute__((unused)) uint8_t const &strokes,
  __attribute__((unused)) float const &radius
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
    } else {
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
      } else {
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
  __attribute__((unused)) uint8_t const &pattern,
  __attribute__((unused)) uint8_t const &strokes,
  __attribute__((unused)) float const &radius,
  __attribute__((unused)) uint8_t const &objects
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
  __attribute__((unused)) uint8_t const &z_action
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
