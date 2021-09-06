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

#include "../../inc/MarlinConfig.h"

#if ENABLED(ARC_SUPPORT)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/temperature.h"

#if ENABLED(DELTA)
  #include "../../module/delta.h"
#elif ENABLED(SCARA)
  #include "../../module/scara.h"
#endif

#if N_ARC_CORRECTION < 1
  #undef N_ARC_CORRECTION
  #define N_ARC_CORRECTION 1
#endif
#ifndef MIN_CIRCLE_SEGMENTS
  #define MIN_CIRCLE_SEGMENTS 72  // 5° per segment
#endif
#if !defined(MAX_ARC_SEGMENT_MM) && defined(MIN_ARC_SEGMENT_MM)
  #define MAX_ARC_SEGMENT_MM MIN_ARC_SEGMENT_MM
#elif !defined(MIN_ARC_SEGMENT_MM) && defined(MAX_ARC_SEGMENT_MM)
  #define MIN_ARC_SEGMENT_MM MAX_ARC_SEGMENT_MM
#endif

#define ARC_LIJK_CODE(L,I,J,K)    CODE_N(SUB2(LINEAR_AXES),L,I,J,K)
#define ARC_LIJKE_CODE(L,I,J,K,E) ARC_LIJK_CODE(L,I,J,K); CODE_ITEM_E(E)

/**
 * Plan an arc in 2 dimensions, with linear motion in the other axes.
 * The arc is traced with many small linear segments according to the configuration.
 */
void plan_arc(
  const xyze_pos_t &cart,   // Destination position
  const ab_float_t &offset, // Center of rotation relative to current_position
  const bool clockwise,     // Clockwise?
  const uint8_t circles     // Take the scenic route
) {
  #if ENABLED(CNC_WORKSPACE_PLANES)
    AxisEnum axis_p, axis_q, axis_l;
    switch (gcode.workspace_plane) {
      default:
      case GcodeSuite::PLANE_XY: axis_p = X_AXIS; axis_q = Y_AXIS; axis_l = Z_AXIS; break;
      case GcodeSuite::PLANE_YZ: axis_p = Y_AXIS; axis_q = Z_AXIS; axis_l = X_AXIS; break;
      case GcodeSuite::PLANE_ZX: axis_p = Z_AXIS; axis_q = X_AXIS; axis_l = Y_AXIS; break;
    }
  #else
    constexpr AxisEnum axis_p = X_AXIS, axis_q = Y_AXIS OPTARG(HAS_Z_AXIS, axis_l = Z_AXIS);
  #endif

  // Radius vector from center to current location
  ab_float_t rvec = -offset;

  const float radius = HYPOT(rvec.a, rvec.b),
              center_P = current_position[axis_p] - rvec.a,
              center_Q = current_position[axis_q] - rvec.b,
              rt_X = cart[axis_p] - center_P,
              rt_Y = cart[axis_q] - center_Q;

  ARC_LIJK_CODE(
    const float start_L = current_position[axis_l],
    const float start_I = current_position.i,
    const float start_J = current_position.j,
    const float start_K = current_position.k
  );

  // Angle of rotation between position and target from the circle center.
  float angular_travel, abs_angular_travel;

  // Minimum number of segments in an arc move
  uint16_t min_segments = 1;

  // Do a full circle if starting and ending positions are "identical"
  if (NEAR(current_position[axis_p], cart[axis_p]) && NEAR(current_position[axis_q], cart[axis_q])) {
    // Preserve direction for circles
    angular_travel = clockwise ? -RADIANS(360) : RADIANS(360);
    abs_angular_travel = RADIANS(360);
    min_segments = MIN_CIRCLE_SEGMENTS;
  }
  else {
    // Calculate the angle
    angular_travel = ATAN2(rvec.a * rt_Y - rvec.b * rt_X, rvec.a * rt_X + rvec.b * rt_Y);

    // Angular travel too small to detect? Just return.
    if (!angular_travel) return;

    // Make sure angular travel over 180 degrees goes the other way around.
    switch (((angular_travel < 0) << 1) | clockwise) {
      case 1: angular_travel -= RADIANS(360); break; // Positive but CW? Reverse direction.
      case 2: angular_travel += RADIANS(360); break; // Negative but CCW? Reverse direction.
    }

    abs_angular_travel = ABS(angular_travel);

    // Apply minimum segments to the arc
    const float portion_of_circle = abs_angular_travel / RADIANS(360);  // Portion of a complete circle (0 < N < 1)
    min_segments = CEIL((MIN_CIRCLE_SEGMENTS) * portion_of_circle);     // Minimum segments for the arc
  }

  ARC_LIJKE_CODE(
    float travel_L = cart[axis_l] - start_L,
    float travel_I = cart.i       - start_I,
    float travel_J = cart.j       - start_J,
    float travel_K = cart.k       - start_K,
    float travel_E = cart.e       - current_position.e
  );

  // If "P" specified circles, call plan_arc recursively then continue with the rest of the arc
  if (TERN0(ARC_P_CIRCLES, circles)) {
    const float total_angular = abs_angular_travel + circles * RADIANS(360),    // Total rotation with all circles and remainder
              part_per_circle = RADIANS(360) / total_angular;                   // Each circle's part of the total

    ARC_LIJKE_CODE(
      const float per_circle_L = travel_L * part_per_circle,    // L movement per circle
      const float per_circle_I = travel_I * part_per_circle,
      const float per_circle_J = travel_J * part_per_circle,
      const float per_circle_K = travel_K * part_per_circle,
      const float per_circle_E = travel_E * part_per_circle     // E movement per circle
    );

    xyze_pos_t temp_position = current_position;
    for (uint16_t n = circles; n--;) {
      ARC_LIJKE_CODE(                                           // Destination Linear Axes
        temp_position[axis_l] += per_circle_L,
        temp_position.i       += per_circle_I,
        temp_position.j       += per_circle_J,
        temp_position.k       += per_circle_K,
        temp_position.e       += per_circle_E                   // Destination E axis
      );
      plan_arc(temp_position, offset, clockwise, 0);            // Plan a single whole circle
    }
    ARC_LIJKE_CODE(
      travel_L = cart[axis_l] - current_position[axis_l],
      travel_I = cart.i       - current_position.i,
      travel_J = cart.j       - current_position.j,
      travel_K = cart.k       - current_position.k,
      travel_E = cart.e       - current_position.e
    );
  }

  // Millimeters in the arc, assuming it's flat
  const float flat_mm = radius * abs_angular_travel;

  // Return if the move is near zero
  if (flat_mm < 0.0001f
    GANG_N(SUB2(LINEAR_AXES),
      && travel_L < 0.0001f,
      && travel_I < 0.0001f,
      && travel_J < 0.0001f,
      && travel_K < 0.0001f
    )
  ) return;

  // Feedrate for the move, scaled by the feedrate multiplier
  const feedRate_t scaled_fr_mm_s = MMS_SCALED(feedrate_mm_s);

  // Get the nominal segment length based on settings
  const float nominal_segment_mm = (
    #if ARC_SEGMENTS_PER_SEC  // Length based on segments per second and feedrate
      constrain(scaled_fr_mm_s * RECIPROCAL(ARC_SEGMENTS_PER_SEC), MIN_ARC_SEGMENT_MM, MAX_ARC_SEGMENT_MM)
    #else
      MAX_ARC_SEGMENT_MM      // Length using the maximum segment size
    #endif
  );

  // Number of whole segments based on the nominal segment length
  const float nominal_segments = _MAX(FLOOR(flat_mm / nominal_segment_mm), min_segments);

  // A new segment length based on the required minimum
  const float segment_mm = constrain(flat_mm / nominal_segments, MIN_ARC_SEGMENT_MM, MAX_ARC_SEGMENT_MM);

  // The number of whole segments in the arc, ignoring the remainder
  uint16_t segments = FLOOR(flat_mm / segment_mm);

  // Are the segments now too few to reach the destination?
  const float segmented_length = segment_mm * segments;
  const bool tooshort = segmented_length < flat_mm - 0.0001f;
  const float proportion = tooshort ? segmented_length / flat_mm : 1.0f;

  /**
   * Vector rotation by transformation matrix: r is the original vector, r_T is the rotated vector,
   * and phi is the angle of rotation. Based on the solution approach by Jens Geisler.
   *     r_T = [cos(phi) -sin(phi);
   *            sin(phi)  cos(phi)] * r ;
   *
   * For arc generation, the center of the circle is the axis of rotation and the radius vector is
   * defined from the circle center to the initial position. Each line segment is formed by successive
   * vector rotations. This requires only two cos() and sin() computations to form the rotation
   * matrix for the duration of the entire arc. Error may accumulate from numerical round-off, since
   * all double numbers are single precision on the Arduino. (True double precision will not have
   * round off issues for CNC applications.) Single precision error can accumulate to be greater than
   * tool precision in some cases. Therefore, arc path correction is implemented.
   *
   * Small angle approximation may be used to reduce computation overhead further. This approximation
   * holds for everything, but very small circles and large MAX_ARC_SEGMENT_MM values. In other words,
   * theta_per_segment would need to be greater than 0.1 rad and N_ARC_CORRECTION would need to be large
   * to cause an appreciable drift error. N_ARC_CORRECTION~=25 is more than small enough to correct for
   * numerical drift error. N_ARC_CORRECTION may be on the order a hundred(s) before error becomes an
   * issue for CNC machines with the single precision Arduino calculations.
   *
   * This approximation also allows plan_arc to immediately insert a line segment into the planner
   * without the initial overhead of computing cos() or sin(). By the time the arc needs to be applied
   * a correction, the planner should have caught up to the lag caused by the initial plan_arc overhead.
   * This is important when there are successive arc motions.
   */
  // Vector rotation matrix values
  xyze_pos_t raw;
  const float theta_per_segment = proportion * angular_travel / segments,
              sq_theta_per_segment = sq(theta_per_segment),
              sin_T = theta_per_segment - sq_theta_per_segment * theta_per_segment / 6,
              cos_T = 1 - 0.5f * sq_theta_per_segment; // Small angle approximation

  #if DISABLED(AUTO_BED_LEVELING_UBL)
    ARC_LIJK_CODE(
      const float per_segment_L = proportion * travel_L / segments,
      const float per_segment_I = proportion * travel_I / segments,
      const float per_segment_J = proportion * travel_J / segments,
      const float per_segment_K = proportion * travel_K / segments
    );
  #endif

  CODE_ITEM_E(const float extruder_per_segment = proportion * travel_E / segments);

  // For shortened segments, run all but the remainder in the loop
  if (tooshort) segments++;

  // Initialize all linear axes and E
  ARC_LIJKE_CODE(
    raw[axis_l] = current_position[axis_l],
    raw.i       = current_position.i,
    raw.j       = current_position.j,
    raw.k       = current_position.k,
    raw.e       = current_position.e
  );

  #if ENABLED(SCARA_FEEDRATE_SCALING)
    const float inv_duration = scaled_fr_mm_s / segment_mm;
  #endif

  millis_t next_idle_ms = millis() + 200UL;

  #if N_ARC_CORRECTION > 1
    int8_t arc_recalc_count = N_ARC_CORRECTION;
  #endif

  for (uint16_t i = 1; i < segments; i++) { // Iterate (segments-1) times

    thermalManager.manage_heater();
    const millis_t ms = millis();
    if (ELAPSED(ms, next_idle_ms)) {
      next_idle_ms = ms + 200UL;
      idle();
    }

    #if N_ARC_CORRECTION > 1
      if (--arc_recalc_count) {
        // Apply vector rotation matrix to previous rvec.a / 1
        const float r_new_Y = rvec.a * sin_T + rvec.b * cos_T;
        rvec.a = rvec.a * cos_T - rvec.b * sin_T;
        rvec.b = r_new_Y;
      }
      else
    #endif
    {
      #if N_ARC_CORRECTION > 1
        arc_recalc_count = N_ARC_CORRECTION;
      #endif

      // Arc correction to radius vector. Computed only every N_ARC_CORRECTION increments.
      // Compute exact location by applying transformation matrix from initial radius vector(=-offset).
      // To reduce stuttering, the sin and cos could be computed at different times.
      // For now, compute both at the same time.
      const float cos_Ti = cos(i * theta_per_segment), sin_Ti = sin(i * theta_per_segment);
      rvec.a = -offset[0] * cos_Ti + offset[1] * sin_Ti;
      rvec.b = -offset[0] * sin_Ti - offset[1] * cos_Ti;
    }

    // Update raw location
    raw[axis_p] = center_P + rvec.a;
    raw[axis_q] = center_Q + rvec.b;
    ARC_LIJKE_CODE(
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        raw[axis_l] = start_L, raw.i = start_I, raw.j = start_J, raw.k = start_K
      #else
        raw[axis_l] += per_segment_L, raw.i += per_segment_I, raw.j += per_segment_J, raw.k += per_segment_K
      #endif
      , raw.e += extruder_per_segment
    );

    apply_motion_limits(raw);

    #if HAS_LEVELING && !PLANNER_LEVELING
      planner.apply_leveling(raw);
    #endif

    if (!planner.buffer_line(raw, scaled_fr_mm_s, active_extruder, 0 OPTARG(SCARA_FEEDRATE_SCALING, inv_duration)))
      break;
  }

  // Ensure last segment arrives at target location.
  raw = cart;
  #if ENABLED(AUTO_BED_LEVELING_UBL)
    ARC_LIJK_CODE(raw[axis_l] = start_L, raw.i = start_I, raw.j = start_J, raw.k = start_K);
  #endif

  apply_motion_limits(raw);

  #if HAS_LEVELING && !PLANNER_LEVELING
    planner.apply_leveling(raw);
  #endif

  planner.buffer_line(raw, scaled_fr_mm_s, active_extruder, 0 OPTARG(SCARA_FEEDRATE_SCALING, inv_duration));

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    ARC_LIJK_CODE(raw[axis_l] = start_L, raw.i = start_I, raw.j = start_J, raw.k = start_K);
  #endif
  current_position = raw;

} // plan_arc

/**
 * G2: Clockwise Arc
 * G3: Counterclockwise Arc
 *
 * This command has two forms: IJ-form (JK, KI) and R-form.
 *
 *  - Depending on the current Workspace Plane orientation,
 *    use parameters IJ/JK/KI to specify the XY/YZ/ZX offsets.
 *    At least one of the IJ/JK/KI parameters is required.
 *    XY/YZ/ZX can be omitted to do a complete circle.
 *    The given XY/YZ/ZX is not error-checked. The arc ends
 *    based on the angle of the destination.
 *    Mixing IJ/JK/KI with R will throw an error.
 *
 *  - R specifies the radius. X or Y (Y or Z / Z or X) is required.
 *      Omitting both XY/YZ/ZX will throw an error.
 *      XY/YZ/ZX must differ from the current XY/YZ/ZX.
 *      Mixing R with IJ/JK/KI will throw an error.
 *
 *  - P specifies the number of full circles to do
 *      before the specified arc move.
 *
 *  Examples:
 *
 *    G2 I10           ; CW circle centered at X+10
 *    G3 X20 Y12 R14   ; CCW circle with r=14 ending at X20 Y12
 */
void GcodeSuite::G2_G3(const bool clockwise) {
  if (MOTION_CONDITIONS) {

    TERN_(FULL_REPORT_TO_HOST_FEATURE, set_and_report_grblstate(M_RUNNING));

    #if ENABLED(SF_ARC_FIX)
      const bool relative_mode_backup = relative_mode;
      relative_mode = true;
    #endif

    get_destination_from_command();   // Get X Y [Z[I[J[K]]]] [E] F (and set cutter power)

    TERN_(SF_ARC_FIX, relative_mode = relative_mode_backup);

    ab_float_t arc_offset = { 0, 0 };
    if (parser.seenval('R')) {
      const float r = parser.value_linear_units();
      if (r) {
        const xy_pos_t p1 = current_position, p2 = destination;
        if (p1 != p2) {
          const xy_pos_t d2 = (p2 - p1) * 0.5f;          // XY vector to midpoint of move from current
          const float e = clockwise ^ (r < 0) ? -1 : 1,  // clockwise -1/1, counterclockwise 1/-1
                      len = d2.magnitude(),              // Distance to mid-point of move from current
                      h2 = (r - len) * (r + len),        // factored to reduce rounding error
                      h = (h2 >= 0) ? SQRT(h2) : 0.0f;   // Distance to the arc pivot-point from midpoint
          const xy_pos_t s = { -d2.y, d2.x };            // Perpendicular bisector. (Divide by len for unit vector.)
          arc_offset = d2 + s / len * e * h;             // The calculated offset (mid-point if |r| <= len)
        }
      }
    }
    else {
      #if ENABLED(CNC_WORKSPACE_PLANES)
        char achar, bchar;
        switch (gcode.workspace_plane) {
          default:
          case GcodeSuite::PLANE_XY: achar = 'I'; bchar = 'J'; break;
          case GcodeSuite::PLANE_YZ: achar = 'J'; bchar = 'K'; break;
          case GcodeSuite::PLANE_ZX: achar = 'K'; bchar = 'I'; break;
        }
      #else
        constexpr char achar = 'I', bchar = 'J';
      #endif
      if (parser.seenval(achar)) arc_offset.a = parser.value_linear_units();
      if (parser.seenval(bchar)) arc_offset.b = parser.value_linear_units();
    }

    if (arc_offset) {

      #if ENABLED(ARC_P_CIRCLES)
        // P indicates number of circles to do
        const int8_t circles_to_do = parser.byteval('P');
        if (!WITHIN(circles_to_do, 0, 100))
          SERIAL_ERROR_MSG(STR_ERR_ARC_ARGS);
      #else
        constexpr uint8_t circles_to_do = 0;
      #endif

      // Send the arc to the planner
      plan_arc(destination, arc_offset, clockwise, circles_to_do);
      reset_stepper_timeout();
    }
    else
      SERIAL_ERROR_MSG(STR_ERR_ARC_ARGS);

    TERN_(FULL_REPORT_TO_HOST_FEATURE, set_and_report_grblstate(M_IDLE));
  }
}

#endif // ARC_SUPPORT
