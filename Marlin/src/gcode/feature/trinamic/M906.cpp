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
#include "../../../module/planner.h"

/**
 * M906: Set motor current in milliamps using axis codes X, Y, Z, E
 * Report driver currents when no axis specified
 */
inline void GcodeSuite::M906() {
  uint16_t values[XYZE];
  LOOP_XYZE(i)
    values[i] = parser.intval(axis_codes[i]);

  #if X_IS_TRINAMIC
    if (values[X_AXIS]) tmc_set_current(stepperX, extended_axis_codes[TMC_X], values[X_AXIS]);
    else tmc_get_current(stepperX, extended_axis_codes[TMC_X]);
  #endif
  #if X2_IS_TRINAMIC
    if (values[X_AXIS]) tmc_set_current(stepperX2, extended_axis_codes[TMC_X2], values[X_AXIS]);
    else tmc_get_current(stepperX2, extended_axis_codes[TMC_X2]);
  #endif
  #if Y_IS_TRINAMIC
    if (values[Y_AXIS]) tmc_set_current(stepperY, extended_axis_codes[TMC_Y], values[Y_AXIS]);
    else tmc_get_current(stepperY, extended_axis_codes[TMC_Y]);
  #endif
  #if Y2_IS_TRINAMIC
    if (values[Y_AXIS]) tmc_set_current(stepperY2, extended_axis_codes[TMC_Y2], values[Y_AXIS]);
    else tmc_get_current(stepperY2, extended_axis_codes[TMC_Y2]);
  #endif
  #if Z_IS_TRINAMIC
    if (values[Z_AXIS]) tmc_set_current(stepperZ, extended_axis_codes[TMC_Z], values[Z_AXIS]);
    else tmc_get_current(stepperZ, extended_axis_codes[TMC_Z]);
  #endif
  #if Z2_IS_TRINAMIC
    if (values[Z_AXIS]) tmc_set_current(stepperZ2, extended_axis_codes[TMC_Z2], values[Z_AXIS]);
    else tmc_get_current(stepperZ2, extended_axis_codes[TMC_Z2]);
  #endif
  #if E0_IS_TRINAMIC
    if (values[E_AXIS]) tmc_set_current(stepperE0, extended_axis_codes[TMC_E0], values[E_AXIS]);
    else tmc_get_current(stepperE0, extended_axis_codes[TMC_E0]);
  #endif
  #if E1_IS_TRINAMIC
    if (values[E_AXIS]) tmc_set_current(stepperE1, extended_axis_codes[TMC_E1], values[E_AXIS]);
    else tmc_get_current(stepperE1, extended_axis_codes[TMC_E1]);
  #endif
  #if E2_IS_TRINAMIC
    if (values[E_AXIS]) tmc_set_current(stepperE2, extended_axis_codes[TMC_E2], values[E_AXIS]);
    else tmc_get_current(stepperE2, extended_axis_codes[TMC_E2]);
  #endif
  #if E3_IS_TRINAMIC
    if (values[E_AXIS]) tmc_set_current(stepperE3, extended_axis_codes[TMC_E3], values[E_AXIS]);
    else tmc_get_current(stepperE3, extended_axis_codes[TMC_E3]);
  #endif
  #if E4_IS_TRINAMIC
    if (values[E_AXIS]) tmc_set_current(stepperE4, extended_axis_codes[TMC_E4], values[E_AXIS]);
    else tmc_get_current(stepperE4, extended_axis_codes[TMC_E4]);
  #endif

}

#endif // HAS_TRINAMIC
