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
   *   P<index>  Workspace index (Optional, default: current)
   *   R<deg>    Rotation angle in degrees (Required)
   *
   * Example:
   *   G68 R45    ; Rotate current workspace by 45°
   *   G68 R-30   ; Rotate current workspace by -30°
   *   G68 P2 R90 ; Rotate workspace 2 by 90° around
   *   G68 P1     ; Set active workspace to 1 (no rotation change)
   *
   * NOTES:
   *   - Only rotation is set. No translation/offset is changed.
   *   - All subsequent moves are rotated by the specified angle.
   */
  void GcodeSuite::G68() {
    const int P = parser.seenval('P') ? parser.value_int() : active_workspace;
    float rotation_angle[MAX_COORDINATE_SYSTEMS] = { 0 };
    float r_angle = rotation_angle[active_workspace];

    if (parser.seenval('P')) {
      if (P < 0 || P >= MAX_COORDINATE_SYSTEMS) {
        SERIAL_ECHOLNPGM("Invalid workspace index.");
        return;
      }
      active_workspace = P;
      SERIAL_ECHOLN("Active workspace set to ", P, ".");
    }

    if (parser.seenval('P') && !parser.seenval('R')) {
      return;
    }

    if (parser.seenval('R')) {
      // Parse R parameter (rotation angle)
      r_angle = parser.value_float();
      rotation_angle[active_workspace] = r_angle;

      float center_x = rotation_center_x;
      center_x = (X_MIN_POS + X_MAX_POS) * 0.5f;
      rotation_center_x = center_x;

      float center_y = rotation_center_y;
      center_y = (Y_MIN_POS + Y_MAX_POS) * 0.5f;
      rotation_center_y = center_y;
      SERIAL_ECHOLN("Workspace ", P, " rotation set to: ", r_angle, " deg.");
    }
    else {
      SERIAL_ECHOLNPGM("Missing R parameter (rotation angle).");
    }
  }

#endif // ROTATE_WORKSPACE
