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

/**
 * delta.cpp
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(DELTA)

#include "delta.h"
#include "motion.h"

// For homing:
#include "stepper.h"
#include "endstops.h"
#include "../lcd/ultralcd.h"
#include "../Marlin.h"

#if ENABLED(SENSORLESS_HOMING)
  #include "../feature/tmc_util.h"
#endif

// Initialized by settings.load()
float delta_height,
      delta_endstop_adj[ABC] = { 0 },
      delta_radius,
      delta_diagonal_rod,
      delta_segments_per_second,
      delta_calibration_radius,
      delta_tower_angle_trim[ABC];

float delta_tower[ABC][2],
      delta_diagonal_rod_2_tower[ABC],
      delta_clip_start_height = Z_MAX_POS;

float delta_safe_distance_from_top();

/**
 * Recalculate factors used for delta kinematics whenever
 * settings have been changed (e.g., by M665).
 */
void recalc_delta_settings() {
  const float trt[ABC] = DELTA_RADIUS_TRIM_TOWER,
              drt[ABC] = DELTA_DIAGONAL_ROD_TRIM_TOWER;
  delta_tower[A_AXIS][X_AXIS] = cos(RADIANS(210 + delta_tower_angle_trim[A_AXIS])) * (delta_radius + trt[A_AXIS]); // front left tower
  delta_tower[A_AXIS][Y_AXIS] = sin(RADIANS(210 + delta_tower_angle_trim[A_AXIS])) * (delta_radius + trt[A_AXIS]);
  delta_tower[B_AXIS][X_AXIS] = cos(RADIANS(330 + delta_tower_angle_trim[B_AXIS])) * (delta_radius + trt[B_AXIS]); // front right tower
  delta_tower[B_AXIS][Y_AXIS] = sin(RADIANS(330 + delta_tower_angle_trim[B_AXIS])) * (delta_radius + trt[B_AXIS]);
  delta_tower[C_AXIS][X_AXIS] = cos(RADIANS( 90 + delta_tower_angle_trim[C_AXIS])) * (delta_radius + trt[C_AXIS]); // back middle tower
  delta_tower[C_AXIS][Y_AXIS] = sin(RADIANS( 90 + delta_tower_angle_trim[C_AXIS])) * (delta_radius + trt[C_AXIS]);
  delta_diagonal_rod_2_tower[A_AXIS] = sq(delta_diagonal_rod + drt[A_AXIS]);
  delta_diagonal_rod_2_tower[B_AXIS] = sq(delta_diagonal_rod + drt[B_AXIS]);
  delta_diagonal_rod_2_tower[C_AXIS] = sq(delta_diagonal_rod + drt[C_AXIS]);
  update_software_endstops(Z_AXIS);
  axis_homed[X_AXIS] = axis_homed[Y_AXIS] = axis_homed[Z_AXIS] = false;
}

/**
 * Delta Inverse Kinematics
 *
 * Calculate the tower positions for a given machine
 * position, storing the result in the delta[] array.
 *
 * This is an expensive calculation, requiring 3 square
 * roots per segmented linear move, and strains the limits
 * of a Mega2560 with a Graphical Display.
 *
 * Suggested optimizations include:
 *
 * - Disable the home_offset (M206) and/or position_shift (G92)
 *   features to remove up to 12 float additions.
 *
 * - Use a fast-inverse-sqrt function and add the reciprocal.
 *   (see above)
 */

#if ENABLED(DELTA_FAST_SQRT) && defined(__AVR__)
  /**
   * Fast inverse sqrt from Quake III Arena
   * See: https://en.wikipedia.org/wiki/Fast_inverse_square_root
   */
  float Q_rsqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5f;
    x2 = number * 0.5f;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5F3759DF - ( i >> 1 );               // what the f***?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
  }
#endif

#define DELTA_DEBUG() do { \
    SERIAL_ECHOPAIR("cartesian X:", raw[X_AXIS]); \
    SERIAL_ECHOPAIR(" Y:", raw[Y_AXIS]);          \
    SERIAL_ECHOLNPAIR(" Z:", raw[Z_AXIS]);        \
    SERIAL_ECHOPAIR("delta A:", delta[A_AXIS]);   \
    SERIAL_ECHOPAIR(" B:", delta[B_AXIS]);        \
    SERIAL_ECHOLNPAIR(" C:", delta[C_AXIS]);      \
  }while(0)

void inverse_kinematics(const float raw[XYZ]) {
  DELTA_IK(raw);
  // DELTA_DEBUG();
}

/**
 * Calculate the highest Z position where the
 * effector has the full range of XY motion.
 */
float delta_safe_distance_from_top() {
  float cartesian[XYZ] = { 0, 0, 0 };
  inverse_kinematics(cartesian);
  float distance = delta[A_AXIS];
  cartesian[Y_AXIS] = DELTA_PRINTABLE_RADIUS;
  inverse_kinematics(cartesian);
  return FABS(distance - delta[A_AXIS]);
}

/**
 * Delta Forward Kinematics
 *
 * See the Wikipedia article "Trilateration"
 * https://en.wikipedia.org/wiki/Trilateration
 *
 * Establish a new coordinate system in the plane of the
 * three carriage points. This system has its origin at
 * tower1, with tower2 on the X axis. Tower3 is in the X-Y
 * plane with a Z component of zero.
 * We will define unit vectors in this coordinate system
 * in our original coordinate system. Then when we calculate
 * the Xnew, Ynew and Znew values, we can translate back into
 * the original system by moving along those unit vectors
 * by the corresponding values.
 *
 * Variable names matched to Marlin, c-version, and avoid the
 * use of any vector library.
 *
 * by Andreas Hardtung 2016-06-07
 * based on a Java function from "Delta Robot Kinematics V3"
 * by Steve Graves
 *
 * The result is stored in the cartes[] array.
 */
void forward_kinematics_DELTA(float z1, float z2, float z3) {
  // Create a vector in old coordinates along x axis of new coordinate
  float p12[3] = { delta_tower[B_AXIS][X_AXIS] - delta_tower[A_AXIS][X_AXIS], delta_tower[B_AXIS][Y_AXIS] - delta_tower[A_AXIS][Y_AXIS], z2 - z1 };

  // Get the Magnitude of vector.
  float d = SQRT( sq(p12[0]) + sq(p12[1]) + sq(p12[2]) );

  // Create unit vector by dividing by magnitude.
  float ex[3] = { p12[0] / d, p12[1] / d, p12[2] / d };

  // Get the vector from the origin of the new system to the third point.
  float p13[3] = { delta_tower[C_AXIS][X_AXIS] - delta_tower[A_AXIS][X_AXIS], delta_tower[C_AXIS][Y_AXIS] - delta_tower[A_AXIS][Y_AXIS], z3 - z1 };

  // Use the dot product to find the component of this vector on the X axis.
  float i = ex[0] * p13[0] + ex[1] * p13[1] + ex[2] * p13[2];

  // Create a vector along the x axis that represents the x component of p13.
  float iex[3] = { ex[0] * i, ex[1] * i, ex[2] * i };

  // Subtract the X component from the original vector leaving only Y. We use the
  // variable that will be the unit vector after we scale it.
  float ey[3] = { p13[0] - iex[0], p13[1] - iex[1], p13[2] - iex[2] };

  // The magnitude of Y component
  float j = SQRT( sq(ey[0]) + sq(ey[1]) + sq(ey[2]) );

  // Convert to a unit vector
  ey[0] /= j; ey[1] /= j;  ey[2] /= j;

  // The cross product of the unit x and y is the unit z
  // float[] ez = vectorCrossProd(ex, ey);
  float ez[3] = {
    ex[1] * ey[2] - ex[2] * ey[1],
    ex[2] * ey[0] - ex[0] * ey[2],
    ex[0] * ey[1] - ex[1] * ey[0]
  };

  // We now have the d, i and j values defined in Wikipedia.
  // Plug them into the equations defined in Wikipedia for Xnew, Ynew and Znew
  float Xnew = (delta_diagonal_rod_2_tower[A_AXIS] - delta_diagonal_rod_2_tower[B_AXIS] + sq(d)) / (d * 2),
        Ynew = ((delta_diagonal_rod_2_tower[A_AXIS] - delta_diagonal_rod_2_tower[C_AXIS] + HYPOT2(i, j)) / 2 - i * Xnew) / j,
        Znew = SQRT(delta_diagonal_rod_2_tower[A_AXIS] - HYPOT2(Xnew, Ynew));

  // Start from the origin of the old coordinates and add vectors in the
  // old coords that represent the Xnew, Ynew and Znew to find the point
  // in the old system.
  cartes[X_AXIS] = delta_tower[A_AXIS][X_AXIS] + ex[0] * Xnew + ey[0] * Ynew - ez[0] * Znew;
  cartes[Y_AXIS] = delta_tower[A_AXIS][Y_AXIS] + ex[1] * Xnew + ey[1] * Ynew - ez[1] * Znew;
  cartes[Z_AXIS] =             z1 + ex[2] * Xnew + ey[2] * Ynew - ez[2] * Znew;
}

/**
 * A delta can only safely home all axes at the same time
 * This is like quick_home_xy() but for 3 towers.
 */
bool home_delta() {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS(">>> home_delta", current_position);
  #endif
  // Init the current position of all carriages to 0,0,0
  ZERO(current_position);
  sync_plan_position();

  // Disable stealthChop if used. Enable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING)
    sensorless_homing_per_axis(A_AXIS);
    sensorless_homing_per_axis(B_AXIS);
    sensorless_homing_per_axis(C_AXIS);
  #endif

  // Move all carriages together linearly until an endstop is hit.
  current_position[X_AXIS] = current_position[Y_AXIS] = current_position[Z_AXIS] = (delta_height + 10);
  feedrate_mm_s = homing_feedrate(X_AXIS);
  line_to_current_position();
  stepper.synchronize();

  // Re-enable stealthChop if used. Disable diag1 pin on driver.
  #if ENABLED(SENSORLESS_HOMING)
    sensorless_homing_per_axis(A_AXIS, false);
    sensorless_homing_per_axis(B_AXIS, false);
    sensorless_homing_per_axis(C_AXIS, false);
  #endif

  // If an endstop was not hit, then damage can occur if homing is continued.
  // This can occur if the delta height not set correctly.
  if (!(Endstops::endstop_hit_bits & (_BV(X_MAX) | _BV(Y_MAX) | _BV(Z_MAX)))) {
    LCD_MESSAGEPGM(MSG_ERR_HOMING_FAILED);
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_HOMING_FAILED);
    return false;
  }

  endstops.hit_on_purpose(); // clear endstop hit flags

  // At least one carriage has reached the top.
  // Now re-home each carriage separately.
  HOMEAXIS(A);
  HOMEAXIS(B);
  HOMEAXIS(C);

  // Set all carriages to their home positions
  // Do this here all at once for Delta, because
  // XYZ isn't ABC. Applying this per-tower would
  // give the impression that they are the same.
  LOOP_XYZ(i) set_axis_is_at_home((AxisEnum)i);

  SYNC_PLAN_POSITION_KINEMATIC();

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("<<< home_delta", current_position);
  #endif

  return true;
}

#endif // DELTA
