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
   * G7: Set Workspace Rotation
   *
   * Set the rotation (about Z axis) for the current workspace.
   *
   * Parameters:
   *   P<index>  Workspace index (Optional, default: current)
   *   R<deg>    Rotation angle in degrees (Required)
   *
   * Example:
   *   G7 R45     ; Rotate current workspace by 45°
   *   G7 P2 R-30 ; Rotate workspace 2 by -30°
   *
   * NOTES:
   *   - Only rotation is set. No translation/offset is changed.
   *   - All subsequent moves are rotated by the specified angle.
   */
  void GcodeSuite::G7() {
    const int P = parser.seen('P') ? parser.value_int() : active_workspace;

    if (parser.seen('P')) {
      active_workspace = P;
    }

    if (parser.seen('P') && !parser.seen('R')) {
      // Only P given: set active workspace
      if (P < 0 || P >= MAX_ROTATABLE) {
        SERIAL_ECHOLNPGM("Invalid workspace index.");
        return;
      }
      SERIAL_ECHOLN("Active workspace set to ", P, ".");
      return;
    }

    if (!parser.seen('R')) {
      SERIAL_ECHOLNPGM("Missing R parameter (rotation angle).");
      return;
    }

    const float r = parser.value_float();
    if (P < 0 || P >= MAX_ROTATABLE) {
      SERIAL_ECHOLNPGM("Invalid workspace index.");
      return;
    }

    rotation_angles[P] = r;
    SERIAL_ECHOLN("Rotation for workspace ", P, " set to ", r, " degrees.");
  }
#endif // ROTATE_WORKSPACE
