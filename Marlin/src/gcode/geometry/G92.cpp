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

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"

#if ENABLED(I2C_POSITION_ENCODERS)
  #include "../../feature/I2CPositionEncoder.h"
#endif

/**
 * G92: Set current position to given X Y Z E
 */
void GcodeSuite::G92() {
  bool didXYZ = false,
       didE = parser.seenval('E');

  if (!didE) stepper.synchronize();

  LOOP_XYZE(i) {
    if (parser.seenval(axis_codes[i])) {
      #if IS_SCARA
        current_position[i] = parser.value_axis_units((AxisEnum)i);
        if (i != E_AXIS) didXYZ = true;
      #else
        #if HAS_POSITION_SHIFT
          const float p = current_position[i];
        #endif
        const float v = parser.value_axis_units((AxisEnum)i);

        current_position[i] = v;

        if (i != E_AXIS) {
          didXYZ = true;
          #if HAS_POSITION_SHIFT
            position_shift[i] += v - p; // Offset the coordinate space
            update_software_endstops((AxisEnum)i);

            #if ENABLED(I2C_POSITION_ENCODERS)
              I2CPEM.encoders[I2CPEM.idx_from_axis((AxisEnum)i)].set_axis_offset(position_shift[i]);
            #endif

          #endif
        }
      #endif
    }
  }
  if (didXYZ)
    SYNC_PLAN_POSITION_KINEMATIC();
  else if (didE)
    sync_plan_position_e();

  report_current_position();
}
