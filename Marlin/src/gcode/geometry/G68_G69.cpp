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

#if ENABLED(ROTATE_WORKSPACE)

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * G68: Set Workspace Rotation
 *
 * Set the rotation (about Z axis) for the current workspace (begins at 0).
 *
 * Parameters:
 *   X<linear>  X coordinate of the rotation center for the current workspace
 *   Y<linear>  Y coordinate of the rotation center for the current workspace
 *   R<float>   Rotation angle in degrees (Required)
 *
 * Example:
 *   G68 R45  ; Rotate active workspace by 45° counter-clockwise (when viewed from positive Z)
 *   G68 R-30 ; Rotate active workspace by -30°
 *   G68 R180 ; Rotate active workspace by 180°
 *
 * NOTES:
 *   - Only rotation is set. No translation/offset is changed.
 *   - All subsequent moves are rotated by the specified angle.
 *   - It is an error to change workspace or working plane while workspace rotation is active
 *     (https://forums.autodesk.com/t5/fusion-manufacture-forum/probing-and-updating-wcs-for-angle/td-p/9487027 ,
 *      https://www.machsupport.com/forum/index.php?topic=43012)
 */
void GcodeSuite::G68() {
  float input_deg;

  if (!parser.seenval('R')) {
    SERIAL_ECHO_MSG("G68: Missing 'R' parameter (rotation angle).");
    return;
  }
  else {
    input_deg = parser.value_float();
  }

  #if ENABLED(LIMIT_ROTATION_ANGLE)
    //#define USE_45DEG_INCREMENTS // Allow 45-degree increments on square beds

    // Check if the input angle is allowed
    const bool is_valid = !(ABS(input_deg) % TERN(USE_45DEG_INCREMENTS, 45, 90));
    if (!is_valid) {
      SERIAL_ECHO_MSG("G68: Rotation angle must be a multiple of " TERN(USE_45DEG_INCREMENTS, "45", "90") ".");
      return;
    }

  #else // !LIMIT_ROTATION_ANGLE

    // Parse rotation angle (float)
    input_deg = parser.value_float();

  #endif

  if (input_deg != rotation_angle) {
    rotation_angle = input_deg;
    SERIAL_ECHO_MSG("G68: Workspace rotation set to: ", input_deg, " deg.");
  }

  rotation_center.x = X_CENTER;
  TERN_(HAS_Y_AXIS, rotation_center.y = Y_CENTER);
}

/**
 * G69: Cancel Workspace Rotation
 */
void GcodeSuite::G69() {
  rotation_angle = 0.0f;
  SERIAL_ECHO_MSG("G68: Workspace rotation canceled");
}

#endif // ROTATE_WORKSPACE
