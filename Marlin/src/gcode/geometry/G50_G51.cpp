/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(SCALE_WORKSPACE)

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * G50: Cancel Workspace Scaling
 */
void GcodeSuite::G50() {
  scaling_factor.reset();
  scaling_center.reset();
  SERIAL_ECHO_MSG("G50: Workspace scaling canceled");
  scaling_flag = false;
}

/**
 * G51: Set Workspace Scaling
 *
 * Scale the current workspace coordinate system.
 *
 * Parameters:
 * X<linear>  X coordinate of the scaling center
 * Y<linear>  Y coordinate of the scaling center
 * Z<linear>  Z coordinate of the scaling center
 * I<float>   scaling factor for X axis
 * J<float>   scaling factor for Y axis
 * K<float>   scaling factor for Z axis
 * P<float>   scaling factor
 * C<bool>    Use current position for axes (X, Y, Z)
 */
void GcodeSuite::G51() {
  bool use_current_pos = parser.seen('C'); // Check if 'C' parameter is present

  if (parser.seenval('P')) {
    const float sf = parser.value_float();
    scaling_factor.x = sf;
    TERN_(HAS_Y_AXIS, scaling_factor.y = sf);
    TERN_(HAS_Z_AXIS, scaling_factor.z = sf);
    SERIAL_ECHO_MSG("G51: Workspace scaling set to: ", sf);
  }
  else {
    if (parser.seenval('I')) scaling_factor.x = parser.value_float();
    #if HAS_Y_AXIS
      if (parser.seenval('J')) scaling_factor.y = parser.value_float();
    #endif
    #if HAS_Z_AXIS
      if (parser.seenval('K')) scaling_factor.z = parser.value_float();
    #endif

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPGM_P(
      PSTR("G51: Workspace scaling set to: X"), scaling_factor.x
      #if HAS_Y_AXIS
        , SP_Y_STR, scaling_factor.y
      #endif
      #if HAS_Z_AXIS
        , SP_Z_STR, scaling_factor.z
      #endif
    );
  }

  rotation_center.x = X_CENTER;
  TERN_(HAS_Y_AXIS, rotation_center.y = Y_CENTER);

  // X-axis scaling
  const bool seenX = parser.seen('X'), hasX = seenX && parser.has_value();
  if (use_current_pos && hasX) {
    SERIAL_ECHO_MSG("?(X) cannot have a value when used with 'C'.");
    return;
  }
  scaling_center.x = (
    (use_current_pos && seenX)
      ? current_position.x
      : hasX
        ? LOGICAL_TO_NATIVE(parser.value_axis_units(X_AXIS), X_AXIS)
        : rotation_center.x
  );

  // Y-axis scaling
  #if HAS_Y_AXIS
    const bool seenY = parser.seen('Y'), hasY = seenY && parser.has_value();
    if (use_current_pos && hasY) {
      SERIAL_ECHO_MSG("?(Y) cannot have a value when used with 'C'.");
      return;
    }
    scaling_center.y = (
      (use_current_pos && seenY)
        ? current_position.y
        : hasY
          ? LOGICAL_TO_NATIVE(parser.value_axis_units(Y_AXIS), Y_AXIS)
          : rotation_center.y
    );
  #endif

  // Z-axis scaling
  #if SCALE_Z_FROM_NONZERO
    const bool seenZ = parser.seen('Z'), hasZ = seenZ && parser.has_value();
    if (use_current_pos && hasZ) {
      SERIAL_ECHO_MSG("?(Z) cannot have a value when used with 'C'.");
      return;
    }
    scaling_center.z = (
      (use_current_pos && seenZ)
        ? current_position.z
        : hasZ
          ? LOGICAL_TO_NATIVE(parser.value_axis_units(Z_AXIS), Z_AXIS)
          : 0.0f
    );
  #endif

  SERIAL_ECHO_START();
  SERIAL_ECHOLNPGM_P(
    PSTR("Workspace scaling center X"), scaling_center.x
    #if HAS_Y_AXIS
      , SP_Y_STR, scaling_center.y
    #endif
    #if HAS_Z_AXIS
      , SP_Z_STR, scaling_center.z
    #endif
  );

  scaling_flag = true; // Set flag to true
}

#endif // SCALE_WORKSPACE
