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

#include "../../../inc/MarlinConfig.h"

#if HAS_TRINAMIC

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper_indirection.h"

/**
 * M906: Set motor current in milliamps using axis codes X, Y, Z, E
 * Report driver currents when no axis specified
 */
void GcodeSuite::M906() {
  #define TMC_SAY_CURRENT(Q) tmc_get_current(stepper##Q, TMC_##Q)
  #define TMC_SET_CURRENT(Q) tmc_set_current(stepper##Q, value)

  bool report = true;
  const uint8_t index = parser.byteval('I');
  LOOP_XYZE(i) if (uint16_t value = parser.intval(axis_codes[i])) {
    report = false;
    switch (i) {
      case X_AXIS:
        #if X_IS_TRINAMIC
          if (index == 0) TMC_SET_CURRENT(X);
        #endif
        #if X2_IS_TRINAMIC
          if (index == 1) TMC_SET_CURRENT(X2);
        #endif
        break;
      case Y_AXIS:
        #if Y_IS_TRINAMIC
          if (index == 0) TMC_SET_CURRENT(Y);
        #endif
        #if Y2_IS_TRINAMIC
          if (index == 1) TMC_SET_CURRENT(Y2);
        #endif
        break;
      case Z_AXIS:
        #if Z_IS_TRINAMIC
          if (index == 0) TMC_SET_CURRENT(Z);
        #endif
        #if Z2_IS_TRINAMIC
          if (index == 1) TMC_SET_CURRENT(Z2);
        #endif
        break;
      case E_AXIS: {
        if (get_target_extruder_from_command()) return;
        switch (target_extruder) {
          #if E0_IS_TRINAMIC
            case 0: TMC_SET_CURRENT(E0); break;
          #endif
          #if E1_IS_TRINAMIC
            case 1: TMC_SET_CURRENT(E1); break;
          #endif
          #if E2_IS_TRINAMIC
            case 2: TMC_SET_CURRENT(E2); break;
          #endif
          #if E3_IS_TRINAMIC
            case 3: TMC_SET_CURRENT(E3); break;
          #endif
          #if E4_IS_TRINAMIC
            case 4: TMC_SET_CURRENT(E4); break;
          #endif
        }
      } break;
    }
  }

  if (report) LOOP_XYZE(i) switch (i) {
    case X_AXIS:
      #if X_IS_TRINAMIC
        TMC_SAY_CURRENT(X);
      #endif
      #if X2_IS_TRINAMIC
        TMC_SAY_CURRENT(X2);
      #endif
      break;
    case Y_AXIS:
      #if Y_IS_TRINAMIC
        TMC_SAY_CURRENT(Y);
      #endif
      #if Y2_IS_TRINAMIC
        TMC_SAY_CURRENT(Y2);
      #endif
      break;
    case Z_AXIS:
      #if Z_IS_TRINAMIC
        TMC_SAY_CURRENT(Z);
      #endif
      #if Z2_IS_TRINAMIC
        TMC_SAY_CURRENT(Z2);
      #endif
      break;
    case E_AXIS:
      #if E0_IS_TRINAMIC
        TMC_SAY_CURRENT(E0);
      #endif
      #if E1_IS_TRINAMIC
        TMC_SAY_CURRENT(E1);
      #endif
      #if E2_IS_TRINAMIC
        TMC_SAY_CURRENT(E2);
      #endif
      #if E3_IS_TRINAMIC
        TMC_SAY_CURRENT(E3);
      #endif
      #if E4_IS_TRINAMIC
        TMC_SAY_CURRENT(E4);
      #endif
      break;
  }
}

#endif // HAS_TRINAMIC
