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

#if ENABLED(SCALE_WORKSPACE)

  #include "../gcode.h"
  #include "../../module/motion.h"
  
  /**
   * G51: Set Workspace Scaling
   *
   * Scale the current workspace coordinate system.
   *
   * Parameters:
   *   X<axis units> x coordinate of the scaling center
   *   Y<axis units> y coordinate of the scaling center
   *   Z<axis units> z coordinate of the scaling center
   *   I<scaling factor> scaling factor for X axis
   *   J<scaling factor> scaling factor for Y axis
   *   K<scaling factor> scaling factor for Z axis
   *   P<scaling factor> scaling factor
   */

  void GcodeSuite::G51() {
  
    if (parser.seenval('P')) {
      const float scaling_factor = parser.value_float();
      scaling_factor_x = scaling_factor;
      TERN_(HAS_Y_AXIS, scaling_factor_y = scaling_factor);
      TERN_(HAS_Z_AXIS, scaling_factor_z = scaling_factor);
    }
    else {
      if (parser.seenval('I'))
        scaling_factor_x = parser.value_float();
      #if HAS_Y_AXIS
        if (parser.seenval('J'))
          scaling_factor_y = parser.value_float();
      #endif
      #if HAS_Z_AXIS
        if (parser.seenval('K'))
          scaling_factor_z = parser.value_float();
      #endif
    }

    scaling_center_x = parser.seenval('X') ? LOGICAL_TO_NATIVE(parser.value_axis_units(X_AXIS), X_AXIS) : current_position.x;
    TERN_(HAS_Y_AXIS, scaling_center_y = parser.seenval('Y') ? LOGICAL_TO_NATIVE(parser.value_axis_units(Y_AXIS), Y_AXIS) : current_position.y);
    TERN_(HAS_Z_AXIS, scaling_center_z = parser.seenval('Z') ? LOGICAL_TO_NATIVE(parser.value_axis_units(Z_AXIS), Z_AXIS) : current_position.z);

    SERIAL_ECHOLNPGM("Workspace scaling set");
  }

  void GcodeSuite::G50() {
    scaling_factor_x = 1.0f;
    scaling_center_x = 0.0f;
    scaling_factor_y = 1.0f;
    scaling_center_y = 0.0f;
    #if HAS_Z_AXIS
      scaling_factor_z = 1.0f;
      scaling_center_z = 0.0f;
    #endif
    SERIAL_ECHOLNPGM("Workspace scaling canceled");
  }


#endif // SCALE_WORKSPACE
