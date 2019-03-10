/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//#define DEBUG_DXC_MODE

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
   *    M605 S3 : Enable Scaled Duplication mode.  The second extruder will duplicate the first extruder's
   *              movement similar to the M605 S2 mode.   However, the second extruder will be producing
   *              a scaled image of the first extruder.  The initial x-offset and temperature differential are
   *              set with M605 S2 [Xnnn] [Rmmm] and then followed with a M605 S3 to start the mirrored movement.
   *    M605 W  : IDEX What? command.
   *
   *    Note: the X axis should be homed after changing dual x-carriage mode.
   */
  void GcodeSuite::M605() {
    planner.synchronize();

    if (parser.seen('S')) {
      const DualXMode previous_mode = dual_x_carriage_mode;

      dual_x_carriage_mode = (DualXMode)parser.value_byte();
      scaled_duplication_mode = false;

      if (dual_x_carriage_mode == DXC_SCALED_DUPLICATION_MODE) {
        if (previous_mode != DXC_DUPLICATION_MODE) {
          SERIAL_ECHOLNPGM("Printer must be in DXC_DUPLICATION_MODE prior to ");
          SERIAL_ECHOLNPGM("specifying DXC_SCALED_DUPLICATION_MODE.");
          dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;
          return;
        }
        scaled_duplication_mode = true;
        stepper.set_directions();
        float x_jog = current_position[X_AXIS] - .1;
        for (uint8_t i = 2; --i;) {
          planner.buffer_line(x_jog, current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], feedrate_mm_s, 0);
          x_jog += .1;
        }
        return;
      }

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
      stepper.set_directions();
      delayed_move_time = 0;
    }
    else if (!parser.seen('W'))  // if no S or W parameter, the DXC mode gets reset to the user's default
      dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;

    #ifdef DEBUG_DXC_MODE

      if (parser.seen('W')) {
        SERIAL_ECHO_START();
        SERIAL_ECHOPGM("IDEX mode: ");
        switch (dual_x_carriage_mode) {
          case DXC_FULL_CONTROL_MODE:       SERIAL_ECHOPGM("DXC_FULL_CONTROL_MODE");       break;
          case DXC_AUTO_PARK_MODE:          SERIAL_ECHOPGM("DXC_AUTO_PARK_MODE");          break;
          case DXC_DUPLICATION_MODE:        SERIAL_ECHOPGM("DXC_DUPLICATION_MODE");        break;
          case DXC_SCALED_DUPLICATION_MODE: SERIAL_ECHOPGM("DXC_SCALED_DUPLICATION_MODE"); break;
        }
        SERIAL_ECHOPAIR("\nActive Ext: ", int(active_extruder));
        if (!active_extruder_parked) SERIAL_ECHOPGM(" NOT ");
        SERIAL_ECHOPGM(" parked.");
        SERIAL_ECHOPAIR("\nactive_extruder_x_pos: ", current_position[X_AXIS]);
        SERIAL_ECHOPAIR("\ninactive_extruder_x_pos: ", inactive_extruder_x_pos);
        SERIAL_ECHOPAIR("\nextruder_duplication_enabled: ", int(extruder_duplication_enabled));
        SERIAL_ECHOPAIR("\nduplicate_extruder_x_offset: ", duplicate_extruder_x_offset);
        SERIAL_ECHOPAIR("\nduplicate_extruder_temp_offset: ", duplicate_extruder_temp_offset);
        SERIAL_ECHOPAIR("\ndelayed_move_time: ", delayed_move_time);
        SERIAL_ECHOPAIR("\nX1 Home X: ", x_home_pos(0));
        SERIAL_ECHOPAIR("\nX1_MIN_POS=", int(X1_MIN_POS));
        SERIAL_ECHOPAIR("\nX1_MAX_POS=", int(X1_MAX_POS));
        SERIAL_ECHOPAIR("\nX2 Home X: ", x_home_pos(1));
        SERIAL_ECHOPAIR("\nX2_MIN_POS=", int(X2_MIN_POS));
        SERIAL_ECHOPAIR("\nX2_MAX_POS=", int(X2_MAX_POS));
        SERIAL_ECHOPAIR("\nX2_HOME_DIR=", int(X2_HOME_DIR));
        SERIAL_ECHOPAIR("\nX2_HOME_POS=", int(X2_HOME_POS));
        SERIAL_ECHOPAIR("\nDEFAULT_DUAL_X_CARRIAGE_MODE=", STRINGIFY(DEFAULT_DUAL_X_CARRIAGE_MODE));
        SERIAL_ECHOPAIR("\nTOOLCHANGE_ZRAISE=", float(TOOLCHANGE_ZRAISE));
        SERIAL_ECHOPAIR("\nDEFAULT_DUPLICATION_X_OFFSET=", int(DEFAULT_DUPLICATION_X_OFFSET));
        SERIAL_EOL();

        for (uint8_t i = 0; i < 2; i++) {
          SERIAL_ECHOPAIR(" nozzle:", int(i));
          LOOP_XYZ(j) {
            SERIAL_ECHOPGM("    hotend_offset[");
            SERIAL_CHAR(axis_codes[j]);
            SERIAL_ECHOPAIR("_AXIS][", int(i));
            SERIAL_ECHOPAIR("]=", hotend_offset[j][i]);
          }
          SERIAL_EOL();
        }
        SERIAL_EOL();
      }
    #endif // DEBUG_DXC_MODE
  }

#elif ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)

  void GcodeSuite::M605() {
    planner.synchronize();
    extruder_duplication_enabled = parser.intval('S') == (int)DXC_DUPLICATION_MODE;
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_DUPLICATION_MODE);
    serialprintln_onoff(extruder_duplication_enabled);
  }

#endif // DUAL_NOZZLE_DUPLICATION_MODE

#endif // DUAL_X_CARRIAGE || DUAL_NOZZLE_DUPLICATION_MODE
