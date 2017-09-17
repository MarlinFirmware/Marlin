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

#include "../../inc/MarlinConfig.h"

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"

#if ENABLED(DUAL_X_CARRIAGE)

  /**
   * M605: Set dual x-carriage movement mode
   *
   *    M605 S0: Full control mode. The slicer has full control over x-carriage movement
   *    M605 S1: Auto-park mode. The inactive head will auto park/unpark without slicer involvement
   *    M605 S2 [Xnnn] [Rmmm]: Duplication mode. The second extruder will duplicate the first with nnn
   *                         units x-offset and an optional differential hotend temperature of
   *                         mmm degrees. E.g., with "M605 S2 X100 R2" the second extruder will duplicate
   *                         the first with a spacing of 100mm in the x direction and 2 degrees hotter.
   *
   *    Note: the X axis should be homed after changing dual x-carriage mode.
   */
  void GcodeSuite::M605() {
    stepper.synchronize();
    if (parser.seen('S')) dual_x_carriage_mode = (DualXMode)parser.value_byte();
    switch (dual_x_carriage_mode) {
      case DXC_FULL_CONTROL_MODE:
      case DXC_AUTO_PARK_MODE:
        break;
      case DXC_DUPLICATION_MODE:
        if (parser.seen('X')) duplicate_extruder_x_offset = max(parser.value_linear_units(), X2_MIN_POS - x_home_pos(0));
        if (parser.seen('R')) duplicate_extruder_temp_offset = parser.value_celsius_diff();
        SERIAL_ECHO_START();
        SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(hotend_offset[X_AXIS][0]);
        SERIAL_CHAR(',');
        SERIAL_ECHO(hotend_offset[Y_AXIS][0]);
        SERIAL_CHAR(' ');
        SERIAL_ECHO(duplicate_extruder_x_offset);
        SERIAL_CHAR(',');
        SERIAL_ECHOLN(hotend_offset[Y_AXIS][1]);
        break;
      default:
        dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;
        break;
    }
    active_extruder_parked = false;
    extruder_duplication_enabled = false;
    delayed_move_time = 0;
  }

#elif ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)

  void GcodeSuite::M605() {
    stepper.synchronize();
    extruder_duplication_enabled = parser.intval('S') == (int)DXC_DUPLICATION_MODE;
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_DUPLICATION_MODE, extruder_duplication_enabled ? MSG_ON : MSG_OFF);
  }

#endif // DUAL_NOZZLE_DUPLICATION_MODE

#endif // DUAL_X_CARRIAGE || DUAL_NOZZLE_DUPLICATION_MODE
