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

#if ENABLED(HAVE_TMC2130)

#include "../../gcode.h"
#include "../../../feature/tmc2130.h"
#include "../../../module/stepper_indirection.h"

inline void tmc2130_get_current(TMC2130Stepper &st, const char name) {
  SERIAL_CHAR(name);
  SERIAL_ECHOPGM(" axis driver current: ");
  SERIAL_ECHOLN(st.getCurrent());
}
inline void tmc2130_set_current(TMC2130Stepper &st, const char name, const int mA) {
  st.setCurrent(mA, R_SENSE, HOLD_MULTIPLIER);
  tmc2130_get_current(st, name);
}

/**
 * M906: Set motor current in milliamps using axis codes X, Y, Z, E
 * Report driver currents when no axis specified
 *
 * S1: Enable automatic current control
 * S0: Disable
 */
void GcodeSuite::M906() {
  uint16_t values[XYZE];
  LOOP_XYZE(i)
    values[i] = parser.intval(axis_codes[i]);

  #if ENABLED(X_IS_TMC2130)
    if (values[X_AXIS]) tmc2130_set_current(stepperX, 'X', values[X_AXIS]);
    else tmc2130_get_current(stepperX, 'X');
  #endif
  #if ENABLED(Y_IS_TMC2130)
    if (values[Y_AXIS]) tmc2130_set_current(stepperY, 'Y', values[Y_AXIS]);
    else tmc2130_get_current(stepperY, 'Y');
  #endif
  #if ENABLED(Z_IS_TMC2130)
    if (values[Z_AXIS]) tmc2130_set_current(stepperZ, 'Z', values[Z_AXIS]);
    else tmc2130_get_current(stepperZ, 'Z');
  #endif
  #if ENABLED(E0_IS_TMC2130)
    if (values[E_AXIS]) tmc2130_set_current(stepperE0, 'E', values[E_AXIS]);
    else tmc2130_get_current(stepperE0, 'E');
  #endif

  #if ENABLED(AUTOMATIC_CURRENT_CONTROL)
    if (parser.seen('S')) auto_current_control = parser.value_bool();
  #endif
}

#endif // HAVE_TMC2130
