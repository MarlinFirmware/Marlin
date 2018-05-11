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

#if ENABLED(SCARA_FEEDRATE_SCALING) && ENABLED(AUTO_BED_LEVELING_BILINEAR)
  #include "../../feature/bedlevel/abl/abl.h"
#endif

#if N_ARC_CORRECTION < 1
  #undef N_ARC_CORRECTION
  #define N_ARC_CORRECTION 1
#endif

/**
 * Plan an arc in 2 dimensions
 *
 * The arc is approximated by generating many small linear segments.
 * The length of each segment is configured in MM_PER_ARC_SEGMENT (Default 1mm)
 * Arcs should only be made relatively large (over 5mm), as larger arcs with
 * larger segments will tend to be more efficient. Your slicer should have
 * options for G2/G3 arc generation. In future these options may be GCode tunable.
 */
void plan_arc(
  const float (&cart)[XYZE],  // Destination position
  const float (&offset)[2],   // Center of rotation relative to current_position
  const uint8_t clockwise     // Clockwise?
) {
  #if ENABLED(CNC_WORKSPACE_PLANES)
    AxisEnum p_axis, q_axis, l_axis;
    switch (gcode.workspace_plane) {
      default:
      case GcodeSuite::PLANE_XY: p_axis = X_AXIS; q_axis = Y_AXIS; l_axis = Z_AXIS; break;
      case GcodeSuite::PLANE_ZX: p_axis = Z_AXIS; q_axis = X_AXIS; l_axis = Y_AXIS; break;
      case GcodeSuite::PLANE_YZ: p_axis = Y_AXIS; q_axis = Z_AXIS; l_axis = X_AXIS; break;
    }
  #else
    constexpr AxisEnum p_axis = X_AXIS, q_axis = Y_AXIS, l_axis = Z_AXIS;
  #endif

  // Radius vector from center to current location
  float r_P = -offset[0], r_Q = -offset[1];

  const float radius = HYPOT(r_P, r_Q),
              center_P = current_position[p_axis] - r_P,
              center_Q = current_position[q_axis] - r_Q,
              rt_X = cart[p_axis] - center_P,
              rt_Y = cart[q_axis] - center_Q,
              linear_travel = cart[l_axis] - current_position[l_axis],
              extruder_travel = cart[E_AXIS] - current_position[E_AXIS];

  // CCW angle of rotation between position and target from the circle center. Only one atan2() trig computation required.
  float angular_travel = ATAN2(r_P * rt_Y - r_Q * rt_X, r_P * rt_X + r_Q * rt_Y);
  if (angular_travel < 0) angular_travel += RADIANS(360);
  if (clockwise) angular_travel -= RADIANS(360);

  // Make a circle if the angular rotation is 0 and the target is current position
  if (angular_travel == 0 && current_position[p_axis] == cart[p_axis] && current_position[q_axis] == cart[q_axis])
    angular_travel = RADIANS(360);

  const float flat_mm = radius * angular_travel,
              mm_of_travel = linear_travel ? HYPOT(flat_mm, linear_travel) : FABS(flat_mm);
  if (mm_of_travel < 0.001) return;

  uint16_t segments = FLOOR(mm_of_travel / (MM_PER_ARC_SEGMENT));
  if (segments == 0) segments = 1;

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
   * holds for everything, but very small circles and large MM_PER_ARC_SEGMENT values. In other words,
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
  float raw[XYZE];
  const float theta_per_segment = angular_travel / segments,
              linear_per_segment = linear_travel / segments,
              extruder_per_segment = extruder_travel / segments,
              sin_T = theta_per_segment,
              cos_T = 1 - 0.5 * sq(theta_per_segment); // Small angle approximation

  // Initialize the linear axis
  raw[l_axis] = current_position[l_axis];

  // Initialize the extruder axis
  raw[E_AXIS] = current_position[E_AXIS];

  const float fr_mm_s = MMS_SCALED(feedrate_mm_s);

  millis_t next_idle_ms = millis() + 200UL;

  #if ENABLED(SCARA_FEEDRATE_SCALING)
    // SCARA needs to scale the feed rate from mm/s to degrees/s
    const float inv_segment_length = 1.0 / (MM_PER_ARC_SEGMENT),
                inverse_secs = inv_segment_length * fr_mm_s;
    float oldA = planner.position_float[A_AXIS],
          oldB = planner.position_float[B_AXIS];
  #endif

  #if N_ARC_CORRECTION > 1
    int8_t arc_recalc_count = N_ARC_CORRECTION;
  #endif

  for (uint16_t i = 1; i < segments; i++) { // Iterate (segments-1) times

    thermalManager.manage_heater();
    if (ELAPSED(millis(), next_idle_ms)) {
      next_idle_ms = millis() + 200UL;
      idle();
    }

    #if N_ARC_CORRECTION > 1
      if (--arc_recalc_count) {
        // Apply vector rotation matrix to previous r_P / 1
        const float r_new_Y = r_P * sin_T + r_Q * cos_T;
        r_P = r_P * cos_T - r_Q * sin_T;
        r_Q = r_new_Y;
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
      r_P = -offset[0] * cos_Ti + offset[1] * sin_Ti;
      r_Q = -offset[0] * sin_Ti - offset[1] * cos_Ti;
    }

    // Update raw location
    raw[p_axis] = center_P + r_P;
    raw[q_axis] = center_Q + r_Q;
    raw[l_axis] += linear_per_segment;
    raw[E_AXIS] += extruder_per_segment;

    clamp_to_software_endstops(raw);

    #if ENABLED(SCARA_FEEDRATE_SCALING)
      // For SCARA scale the feed rate from mm/s to degrees/s
      // i.e., Complete the angular vector in the given time.
      inverse_kinematics(raw);
      ADJUST_DELTA(raw);
      planner.buffer_segment(delta[A_AXIS], delta[B_AXIS], raw[Z_AXIS], raw[E_AXIS], HYPOT(delta[A_AXIS] - oldA, delta[B_AXIS] - oldB) * inverse_secs, active_extruder);
      oldA = delta[A_AXIS]; oldB = delta[B_AXIS];
    #elif HAS_UBL_AND_CURVES
      float pos[XYZ] = { raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS] };
      planner.apply_leveling(pos);
      planner.buffer_segment(pos[X_AXIS], pos[Y_AXIS], pos[Z_AXIS], raw[E_AXIS], fr_mm_s, active_extruder);
    #else
      planner.buffer_line_kinematic(raw, fr_mm_s, active_extruder);
    #endif
  }

  // Ensure last segment arrives at target location.
  #if ENABLED(SCARA_FEEDRATE_SCALING)
    inverse_kinematics(cart);
    ADJUST_DELTA(cart);
    const float diff2 = HYPOT2(delta[A_AXIS] - oldA, delta[B_AXIS] - oldB);
    if (diff2)
      planner.buffer_segment(delta[A_AXIS], delta[B_AXIS], cart[Z_AXIS], cart[E_AXIS], SQRT(diff2) * inverse_secs, active_extruder);
  #elif HAS_UBL_AND_CURVES
    float pos[XYZ] = { cart[X_AXIS], cart[Y_AXIS], cart[Z_AXIS] };
    planner.apply_leveling(pos);
    planner.buffer_segment(pos[X_AXIS], pos[Y_AXIS], pos[Z_AXIS], cart[E_AXIS], fr_mm_s, active_extruder);
  #else
    planner.buffer_line_kinematic(cart, fr_mm_s, active_extruder);
  #endif

  COPY(current_position, cart);
} // plan_arc

/**
 * G2: Clockwise Arc
 * G3: Counterclockwise Arc
 *
 * This command has two forms: IJ-form and R-form.
 *
 *  - I specifies an X offset. J specifies a Y offset.
 *    At least one of the IJ parameters is required.
 *    X and Y can be omitted to do a complete circle.
 *    The given XY is not error-checked. The arc ends
 *     based on the angle of the destination.
 *    Mixing I or J with R will throw an error.
 *
 *  - R specifies the radius. X or Y is required.
 *    Omitting both X and Y will throw an error.
 *    X or Y must differ from the current XY.
 *    Mixing R with I or J will throw an error.
 *
 *  - P specifies the number of full circles to do
 *    before the specified arc move.
 *
 *  Examples:
 *
 *    G2 I10           ; CW circle centered at X+10
 *    G3 X20 Y12 R14   ; CCW circle with r=14 ending at X20 Y12
 */
void GcodeSuite::G2_G3(const bool clockwise) {
  if (MOTION_CONDITIONS) {

    #if ENABLED(SF_ARC_FIX)
      const bool relative_mode_backup = relative_mode;
      relative_mode = true;
    #endif

    get_destination_from_command();

    #if ENABLED(SF_ARC_FIX)
      relative_mode = relative_mode_backup;
    #endif

    float arc_offset[2] = { 0.0, 0.0 };
    if (parser.seenval('R')) {
      const float r = parser.value_linear_units(),
                  p1 = current_position[X_AXIS], q1 = current_position[Y_AXIS],
                  p2 = destination[X_AXIS], q2 = destination[Y_AXIS];
      if (r && (p2 != p1 || q2 != q1)) {
        const float e = clockwise ^ (r < 0) ? -1 : 1,           // clockwise -1/1, counterclockwise 1/-1
                    dx = p2 - p1, dy = q2 - q1,                 // X and Y differences
                    d = HYPOT(dx, dy),                          // Linear distance between the points
                    h = SQRT(sq(r) - sq(d * 0.5)),              // Distance to the arc pivot-point
                    mx = (p1 + p2) * 0.5, my = (q1 + q2) * 0.5, // Point between the two points
                    sx = -dy / d, sy = dx / d,                  // Slope of the perpendicular bisector
                    cx = mx + e * h * sx, cy = my + e * h * sy; // Pivot-point of the arc
        arc_offset[0] = cx - p1;
        arc_offset[1] = cy - q1;
      }
    }
    else {
      if (parser.seenval('I')) arc_offset[0] = parser.value_linear_units();
      if (parser.seenval('J')) arc_offset[1] = parser.value_linear_units();
    }

    if (arc_offset[0] || arc_offset[1]) {

      #if ENABLED(ARC_P_CIRCLES)
        // P indicates number of circles to do
        int8_t circles_to_do = parser.byteval('P');
        if (!WITHIN(circles_to_do, 0, 100)) {
          SERIAL_ERROR_START();
          SERIAL_ERRORLNPGM(MSG_ERR_ARC_ARGS);
        }
        while (circles_to_do--)
          plan_arc(current_position, arc_offset, clockwise);
      #endif

      // Send the arc to the planner
      plan_arc(destination, arc_offset, clockwise);
      reset_stepper_timeout();
    }
    else {
      // Bad arguments
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM(MSG_ERR_ARC_ARGS);
    }
  }
}

#endif // ARC_SUPPORT
