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

#include "../inc/MarlinConfigPre.h"

#if ENABLED(BACKLASH_COMPENSATION)

#include "backlash.h"

#include "../module/motion.h"
#include "../module/planner.h"

#ifdef BACKLASH_DISTANCE_MM
  #if ENABLED(BACKLASH_GCODE)
    float Backlash::distance_mm[XYZ] = BACKLASH_DISTANCE_MM;
  #else
    const float Backlash::distance_mm[XYZ] = BACKLASH_DISTANCE_MM;
  #endif
#endif

#if ENABLED(BACKLASH_GCODE)
  uint8_t Backlash::correction = (BACKLASH_CORRECTION) * 0xFF;
  #ifdef BACKLASH_SMOOTHING_MM
    float Backlash::smoothing_mm = BACKLASH_SMOOTHING_MM;
  #endif
#endif

#if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
  float Backlash::measured_mm[XYZ] = { 0 };
  uint8_t Backlash::measured_count[XYZ] = { 0 };
#endif

Backlash backlash;

/**
 * To minimize seams in the printed part, backlash correction only adds
 * steps to the current segment (instead of creating a new segment, which
 * causes discontinuities and print artifacts).
 *
 * With a non-zero BACKLASH_SMOOTHING_MM value the backlash correction is
 * spread over multiple segments, smoothing out artifacts even more.
 */

void Backlash::add_correction_steps(const int32_t &da, const int32_t &db, const int32_t &dc, const uint8_t dm, block_t * const block) {
  static uint8_t last_direction_bits;
  uint8_t changed_dir = last_direction_bits ^ dm;
  // Ignore direction change if no steps are taken in that direction
  if (da == 0) CBI(changed_dir, X_AXIS);
  if (db == 0) CBI(changed_dir, Y_AXIS);
  if (dc == 0) CBI(changed_dir, Z_AXIS);
  last_direction_bits ^= changed_dir;

  if (correction == 0) return;

  #ifdef BACKLASH_SMOOTHING_MM
    // The segment proportion is a value greater than 0.0 indicating how much residual_error
    // is corrected for in this segment. The contribution is based on segment length and the
    // smoothing distance. Since the computation of this proportion involves a floating point
    // division, defer computation until needed.
    float segment_proportion = 0;

    // Residual error carried forward across multiple segments, so correction can be applied
    // to segments where there is no direction change.
    static int32_t residual_error[XYZ] = { 0 };
  #else
    // No direction change, no correction.
    if (!changed_dir) return;
    // No leftover residual error from segment to segment
    int32_t residual_error[XYZ] = { 0 };
  #endif

  const float f_corr = float(correction) / 255.0f;

  LOOP_XYZ(axis) {
    if (distance_mm[axis]) {
      const bool reversing = TEST(dm,axis);

      // When an axis changes direction, add axis backlash to the residual error
      if (TEST(changed_dir, axis))
        residual_error[axis] += (reversing ? -f_corr : f_corr) * distance_mm[axis] * planner.settings.axis_steps_per_mm[axis];

      // Decide how much of the residual error to correct in this segment
      int32_t error_correction = residual_error[axis];
      #ifdef BACKLASH_SMOOTHING_MM
        if (error_correction && smoothing_mm != 0) {
          // Take up a portion of the residual_error in this segment, but only when
          // the current segment travels in the same direction as the correction
          if (reversing == (error_correction < 0)) {
            if (segment_proportion == 0)
              segment_proportion = MIN(1.0f, block->millimeters / smoothing_mm);
            error_correction = CEIL(segment_proportion * error_correction);
          }
          else
            error_correction = 0; // Don't take up any backlash in this segment, as it would subtract steps
        }
      #endif
      // Making a correction reduces the residual error and modifies delta_mm
      if (error_correction) {
        block->steps[axis] += ABS(error_correction);
        residual_error[axis] -= error_correction;
      }
    }
  }
}

#if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
  #if USES_Z_MIN_PROBE_ENDSTOP
    #define TEST_PROBE_PIN (READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING)
  #else
    #define TEST_PROBE_PIN (READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING)
  #endif

  // Measure Z backlash by raising nozzle in increments until probe deactivates
  void Backlash::measure_with_probe() {
    if (measured_count[Z_AXIS] == 255) return;

    float start_height = current_position[Z_AXIS];
    while (current_position[Z_AXIS] < (start_height + BACKLASH_MEASUREMENT_LIMIT) && TEST_PROBE_PIN)
      do_blocking_move_to_z(current_position[Z_AXIS] + BACKLASH_MEASUREMENT_RESOLUTION, MMM_TO_MMS(BACKLASH_MEASUREMENT_FEEDRATE));

    // The backlash from all probe points is averaged, so count the number of measurements
    measured_mm[Z_AXIS] += current_position[Z_AXIS] - start_height;
    measured_count[Z_AXIS]++;
  }
#endif

#endif // BACKLASH_COMPENSATION
