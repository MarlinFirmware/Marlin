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
  #define TMC_SAY_CURRENT(Q) tmc_get_current(stepper##Q)
  #define TMC_SET_CURRENT(Q) tmc_set_current(stepper##Q, value)

  bool report = true;

  #if AXIS_IS_TMC(X) || AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3)
    const uint8_t index = parser.byteval('I');
  #endif

  LOOP_XYZE(i) if (uint16_t value = parser.intval(axis_codes[i])) {
    report = false;
    switch (i) {
      case X_AXIS:
        #if AXIS_IS_TMC(X)
          if (index == 0) TMC_SET_CURRENT(X);
        #endif
        #if AXIS_IS_TMC(X2)
          if (index == 1) TMC_SET_CURRENT(X2);
        #endif
        break;
      case Y_AXIS:
        #if AXIS_IS_TMC(Y)
          if (index == 0) TMC_SET_CURRENT(Y);
        #endif
        #if AXIS_IS_TMC(Y2)
          if (index == 1) TMC_SET_CURRENT(Y2);
        #endif
        break;
      case Z_AXIS:
        #if AXIS_IS_TMC(Z)
          if (index == 0) TMC_SET_CURRENT(Z);
        #endif
        #if AXIS_IS_TMC(Z2)
          if (index == 1) TMC_SET_CURRENT(Z2);
        #endif
        #if AXIS_IS_TMC(Z3)
          if (index == 2) TMC_SET_CURRENT(Z3);
        #endif
        break;
      case E_AXIS: {
        const int8_t target_extruder = get_target_extruder_from_command();
        if (target_extruder < 0) return;
        switch (target_extruder) {
          #if AXIS_IS_TMC(E0)
            case 0: TMC_SET_CURRENT(E0); break;
          #endif
          #if AXIS_IS_TMC(E1)
            case 1: TMC_SET_CURRENT(E1); break;
          #endif
          #if AXIS_IS_TMC(E2)
            case 2: TMC_SET_CURRENT(E2); break;
          #endif
          #if AXIS_IS_TMC(E3)
            case 3: TMC_SET_CURRENT(E3); break;
          #endif
          #if AXIS_IS_TMC(E4)
            case 4: TMC_SET_CURRENT(E4); break;
          #endif
          #if AXIS_IS_TMC(E5)
            case 5: TMC_SET_CURRENT(E5); break;
          #endif
        }
      } break;
    }
  }

  if (report) {
    #if AXIS_IS_TMC(X)
      TMC_SAY_CURRENT(X);
    #endif
    #if AXIS_IS_TMC(X2)
      TMC_SAY_CURRENT(X2);
    #endif
    #if AXIS_IS_TMC(Y)
      TMC_SAY_CURRENT(Y);
    #endif
    #if AXIS_IS_TMC(Y2)
      TMC_SAY_CURRENT(Y2);
    #endif
    #if AXIS_IS_TMC(Z)
      TMC_SAY_CURRENT(Z);
    #endif
    #if AXIS_IS_TMC(Z2)
      TMC_SAY_CURRENT(Z2);
    #endif
    #if AXIS_IS_TMC(Z3)
      TMC_SAY_CURRENT(Z3);
    #endif
    #if AXIS_IS_TMC(E0)
      TMC_SAY_CURRENT(E0);
    #endif
    #if AXIS_IS_TMC(E1)
      TMC_SAY_CURRENT(E1);
    #endif
    #if AXIS_IS_TMC(E2)
      TMC_SAY_CURRENT(E2);
    #endif
    #if AXIS_IS_TMC(E3)
      TMC_SAY_CURRENT(E3);
    #endif
    #if AXIS_IS_TMC(E4)
      TMC_SAY_CURRENT(E4);
    #endif
    #if AXIS_IS_TMC(E5)
      TMC_SAY_CURRENT(E5);
    #endif
  }
}

#endif // HAS_TRINAMIC
