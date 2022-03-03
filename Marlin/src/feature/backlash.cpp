/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(BACKLASH_COMPENSATION)

#include "backlash.h"

#include "../module/motion.h"
#include "../module/planner.h"

axis_bits_t Backlash::last_direction_bits;
xyz_long_t Backlash::residual_error{0};

#ifdef BACKLASH_DISTANCE_MM
  #if ENABLED(BACKLASH_GCODE)
    xyz_float_t Backlash::distance_mm = BACKLASH_DISTANCE_MM;
  #else
    const xyz_float_t Backlash::distance_mm = BACKLASH_DISTANCE_MM;
  #endif
#endif

#if ENABLED(BACKLASH_GCODE)
  uint8_t Backlash::correction = (BACKLASH_CORRECTION) * all_on;
  #ifdef BACKLASH_SMOOTHING_MM
    float Backlash::smoothing_mm = BACKLASH_SMOOTHING_MM;
  #endif
#endif

#if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
  xyz_float_t Backlash::measured_mm{0};
  xyz_uint8_t Backlash::measured_count{0};
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

void Backlash::add_correction_steps(const int32_t &da, const int32_t &db, const int32_t &dc, const axis_bits_t dm, block_t * const block) {
  axis_bits_t changed_dir = last_direction_bits ^ dm;
  // Ignore direction change unless steps are taken in that direction
  #if DISABLED(CORE_BACKLASH) || EITHER(MARKFORGED_XY, MARKFORGED_YX)
    if (!da) CBI(changed_dir, X_AXIS);
    if (!db) CBI(changed_dir, Y_AXIS);
    if (!dc) CBI(changed_dir, Z_AXIS);
  #elif CORE_IS_XY
    if (!(da + db)) CBI(changed_dir, X_AXIS);
    if (!(da - db)) CBI(changed_dir, Y_AXIS);
    if (!dc)        CBI(changed_dir, Z_AXIS);
  #elif CORE_IS_XZ
    if (!(da + dc)) CBI(changed_dir, X_AXIS);
    if (!(da - dc)) CBI(changed_dir, Z_AXIS);
    if (!db)        CBI(changed_dir, Y_AXIS);
  #elif CORE_IS_YZ
    if (!(db + dc)) CBI(changed_dir, Y_AXIS);
    if (!(db - dc)) CBI(changed_dir, Z_AXIS);
    if (!da)        CBI(changed_dir, X_AXIS);
  #endif
  last_direction_bits ^= changed_dir;

  if (!correction && !residual_error) return;

  #ifdef BACKLASH_SMOOTHING_MM
    // The segment proportion is a value greater than 0.0 indicating how much residual_error
    // is corrected for in this segment. The contribution is based on segment length and the
    // smoothing distance. Since the computation of this proportion involves a floating point
    // division, defer computation until needed.
    float segment_proportion = 0;
  #endif

  const float f_corr = float(correction) / all_on;

  LOOP_LINEAR_AXES(axis) {
    if (distance_mm[axis]) {
      const bool reverse = TEST(dm, axis);

      // When an axis changes direction, add axis backlash to the residual error
      if (TEST(changed_dir, axis))
        residual_error[axis] += (reverse ? -f_corr : f_corr) * distance_mm[axis] * planner.settings.axis_steps_per_mm[axis];

      // Decide how much of the residual error to correct in this segment
      int32_t error_correction = residual_error[axis];
      if (reverse != (error_correction < 0))
        error_correction = 0; // Don't take up any backlash in this segment, as it would subtract steps

      #ifdef BACKLASH_SMOOTHING_MM
        if (error_correction && smoothing_mm != 0) {
          // Take up a portion of the residual_error in this segment
          if (segment_proportion == 0) segment_proportion = _MIN(1.0f, block->millimeters / smoothing_mm);
          error_correction = CEIL(segment_proportion * error_correction);
        }
      #endif

      // This correction reduces the residual error and adds block steps
      if (error_correction) {
        block->steps[axis] += ABS(error_correction);
        #if ENABLED(CORE_BACKLASH)
          switch (axis) {
            case CORE_AXIS_1:
              //block->steps[CORE_AXIS_2] += influence_distance_mm[axis] * planner.settings.axis_steps_per_mm[CORE_AXIS_2];
              //SERIAL_ECHOLNPGM("CORE_AXIS_1 dir change. distance=", distance_mm[axis], " r.err=", residual_error[axis],
              //  " da=", da, " db=", db, " block->steps[axis]=", block->steps[axis], " err_corr=", error_correction);
              break;
            case CORE_AXIS_2:
              //block->steps[CORE_AXIS_1] += influence_distance_mm[axis] * planner.settings.axis_steps_per_mm[CORE_AXIS_1];;
              //SERIAL_ECHOLNPGM("CORE_AXIS_2 dir change. distance=", distance_mm[axis], " r.err=", residual_error[axis],
              //  " da=", da, " db=", db, " block->steps[axis]=", block->steps[axis], " err_corr=", error_correction);
              break;
            case NORMAL_AXIS: break;
          }
          residual_error[axis] = 0; // No residual_error needed for next CORE block, I think...
        #else
          residual_error[axis] -= error_correction;
        #endif
      }
    }
  }
}

int32_t Backlash::get_applied_steps(const AxisEnum axis) {
  if (axis >= LINEAR_AXES) return 0;

  const bool reverse = TEST(last_direction_bits, axis);

  const int32_t residual_error_axis = residual_error[axis];

  // At startup it is assumed the last move was forwards. So the applied
  // steps will always be a non-positive number.

  if (!reverse) return -residual_error_axis;

  const float f_corr = float(correction) / all_on;
  const int32_t full_error_axis = -f_corr * distance_mm[axis] * planner.settings.axis_steps_per_mm[axis];
  return full_error_axis - residual_error_axis;
}

class Backlash::StepAdjuster {
  xyz_long_t applied_steps;
public:
  StepAdjuster() {
    LOOP_LINEAR_AXES(axis) applied_steps[axis] = backlash.get_applied_steps((AxisEnum)axis);
  }
  ~StepAdjuster() {
    // after backlash compensation parameter changes, ensure applied step count does not change
    LOOP_LINEAR_AXES(axis) residual_error[axis] += backlash.get_applied_steps((AxisEnum)axis) - applied_steps[axis];
  }
};

void Backlash::set_correction_uint8(const uint8_t v) {
  StepAdjuster adjuster;
  correction = v;
}

void Backlash::set_distance_mm(const AxisEnum axis, const float v) {
  StepAdjuster adjuster;
  distance_mm[axis] = v;
}

#ifdef BACKLASH_SMOOTHING_MM
  void Backlash::set_smoothing_mm(const float v) {
    StepAdjuster adjuster;
    smoothing_mm = v;
  }
#endif

#if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)

  #include "../module/probe.h"

  // Measure Z backlash by raising nozzle in increments until probe deactivates
  void Backlash::measure_with_probe() {
    if (measured_count.z == 255) return;

    const float start_height = current_position.z;
    while (current_position.z < (start_height + BACKLASH_MEASUREMENT_LIMIT) && PROBE_TRIGGERED())
      do_blocking_move_to_z(current_position.z + BACKLASH_MEASUREMENT_RESOLUTION, MMM_TO_MMS(BACKLASH_MEASUREMENT_FEEDRATE));

    // The backlash from all probe points is averaged, so count the number of measurements
    measured_mm.z += current_position.z - start_height;
    measured_count.z++;
  }

#endif

#endif // BACKLASH_COMPENSATION
