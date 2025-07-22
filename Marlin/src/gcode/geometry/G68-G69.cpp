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
   *   X<axis units> x coordinate of the rotation center for the current workspace
   *   Y<axis units> y coordinate of the rotation center for the current workspace
   *   R<deg>        Rotation angle in degrees (Required)
   *
   * Example:
   *   G68 R45       ; Rotate active workspace by 45° counter-clockwise (when viewed from positive Z) 
   *                 ; around current position
   *   G68 R-30      ; Rotate active workspace by -30° around current position
   *   G68 X0 Y0 R45 ; Rotate active  workspace by 45°C around X0 Y0 (X and Y are specified in the current workspace)
   *
   * NOTES:
   *   - Only rotation is set. No translation/offset is changed.
   *   - All subsequent moves are rotated by the specified angle.
   *   - It is an error to change workspace or working plane while workspace rotation is active
   *     (https://forums.autodesk.com/t5/fusion-manufacture-forum/probing-and-updating-wcs-for-angle/td-p/9487027 , 
   *      https://www.machsupport.com/forum/index.php?topic=43012)
   */

  void GcodeSuite::G68() {
  
    if (!parser.seenval('R')) {
      SERIAL_ECHOLNPGM("Missing R parameter (rotation angle).");
      return;
    }
    else {
      rotation_angle = parser.value_float();
    }
    TERN_(HAS_X_AXIS, rotation_center_x = parser.seenval('X') ? LOGICAL_TO_NATIVE(parser.value_axis_units(X_AXIS), X_AXIS) : current_position.x);
    TERN_(HAS_Y_AXIS, rotation_center_y = parser.seenval('Y') ? LOGICAL_TO_NATIVE(parser.value_axis_units(Y_AXIS), Y_AXIS) : current_position.y);
    SERIAL_ECHOLNPGM("Workspace rotation set");
  }

  void GcodeSuite::G69() {
    rotation_angle = 0.0f;
    SERIAL_ECHOLNPGM("Workspace rotation canceled");
  }


#endif // ROTATE_WORKSPACE
