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

#define DEBUG_DXC_MODE

#if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/tool_change.h"
#include "../../module/planner.h"

#if ENABLED(DUAL_X_CARRIAGE)

  /**
   * M605: Set dual x-carriage movement mode
   *
   *    M605    : Restore user specified DEFAULT_DUAL_X_CARRIAGE_MODE
   *    M605 S0: Full control mode. The slicer has full control over x-carriage movement
   *    M605 S1: Auto-park mode. The inactive head will auto park/unpark without slicer involvement
   *    M605 S2 [Xnnn] [Rmmm]: Duplication mode. The second extruder will duplicate the first with nnn
   *                         units x-offset and an optional differential hotend temperature of
   *                         mmm degrees. E.g., with "M605 S2 X100 R2" the second extruder will duplicate
   *                         the first with a spacing of 100mm in the x direction and 2 degrees hotter.
   *    M605 S3 : Enable Symmetric Duplication mode.  The second extruder will duplicate the first extruder's
   *              movement similar to the M605 S2 mode.   However, the second extruder will be producing
   *              a mirror image of the first extruder.  The initial x-offset and temperature differential are
   *              set with M605 S2 [Xnnn] [Rmmm] and then followed with a M605 S3 to start the mirrored movement.
   *    M605 W  : IDEX What? command.
   *
   *    Note: the X axis should be homed after changing dual x-carriage mode.
   */
  void GcodeSuite::M605() {
    planner.synchronize();

    if (parser.seen('S')) {
      dual_x_carriage_mode = (DualXMode)parser.value_byte();

      switch (dual_x_carriage_mode) {
        case DXC_FULL_CONTROL_MODE:
        case DXC_AUTO_PARK_MODE:
          break;
        case DXC_DUPLICATION_MODE:
          if (parser.seen('X')) duplicate_extruder_x_offset = MAX(parser.value_linear_units(), X2_MIN_POS - x_home_pos(0));
          if (parser.seen('R')) duplicate_extruder_temp_offset = parser.value_celsius_diff();
          if (active_extruder != 0) tool_change(0);
          break;
        default:
          dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;
          break;
      }
      active_extruder_parked = false;
      extruder_duplication_enabled = false;
      delayed_move_time = 0;
    }
    else if (!parser.seen('W'))  // if no S or W parameter, the DXC mode gets reset to the user's default
      dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;

    if (parser.seen('W')) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("IDEX mode: ");
      switch (dual_x_carriage_mode) {
        case DXC_FULL_CONTROL_MODE: SERIAL_ECHOPGM("DXC_FULL_CONTROL_MODE"); break;
        case DXC_AUTO_PARK_MODE:    SERIAL_ECHOPGM("DXC_AUTO_PARK_MODE");    break;
        case DXC_DUPLICATION_MODE:  SERIAL_ECHOPGM("DXC_DUPLICATION_MODE");  break;
      }
      SERIAL_ECHOPAIR("\nActive Ext: ", int(active_extruder));
      if (!active_extruder_parked) SERIAL_ECHOPGM(" NOT ");
      SERIAL_ECHOLNPGM(" parked.");
      SERIAL_ECHOPAIR("active_extruder_x_pos: ", current_position[X_AXIS]);
      SERIAL_ECHOPAIR("   inactive_extruder_x_pos: ", inactive_extruder_x_pos);
      SERIAL_ECHOPAIR("\nT0 Home X: ", x_home_pos(0));
      SERIAL_ECHOPAIR("\nT1 Home X: ", x_home_pos(1));
      SERIAL_ECHOPAIR("\nextruder_duplication_enabled: ", int(extruder_duplication_enabled));
      SERIAL_ECHOPAIR("\nduplicate_extruder_x_offset: ", duplicate_extruder_x_offset);
      SERIAL_ECHOPAIR("\nduplicate_extruder_temp_offset: ", duplicate_extruder_temp_offset);
      SERIAL_ECHOPAIR("\ndelayed_move_time: ", delayed_move_time);
    }
  }

#elif ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)

  void GcodeSuite::M605() {
    planner.synchronize();
    extruder_duplication_enabled = parser.intval('S') == (int)DXC_DUPLICATION_MODE;
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_DUPLICATION_MODE, extruder_duplication_enabled ? MSG_ON : MSG_OFF);
  }

#endif // DUAL_NOZZLE_DUPLICATION_MODE

#endif // DUAL_X_CARRIAGE || DUAL_NOZZLE_DUPLICATION_MODE
