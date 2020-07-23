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

#include "../../inc/MarlinConfig.h"

#if HAS_DUPLICATION_MODE

//#define DEBUG_DXC_MODE

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/stepper.h"
#include "../../module/tool_change.h"
#include "../../module/planner.h"

#define DEBUG_OUT ENABLED(DEBUG_DXC_MODE)
#include "../../core/debug_out.h"

#if ENABLED(DUAL_X_CARRIAGE)

  /**
   * M605: Set dual x-carriage movement mode
   *
   *   M605 S0 : (FULL_CONTROL) The slicer has full control over both X-carriages and can achieve optimal travel
   *             results as long as it supports dual X-carriages.
   *
   *   M605 S1 : (AUTO_PARK) The firmware automatically parks and unparks the X-carriages on tool-change so that
   *             additional slicer support is not required.
   *
   *   M605 S2 X R : (DUPLICATION) The firmware moves the second X-carriage and extruder in synchronization with
   *             the first X-carriage and extruder, to print 2 copies of the same object at the same time.
   *             Set the constant X-offset and temperature differential with M605 S2 X[offs] R[deg] and
   *             follow with "M605 S2" to initiate duplicated movement. For example, use "M605 S2 X100 R2" to
   *             make a copy 100mm to the right with E1 2° hotter than E0.
   *
   *   M605 S3 : (MIRRORED) Formbot/Vivedino-inspired mirrored mode in which the second extruder duplicates
   *             the movement of the first except the second extruder is reversed in the X axis.
   *             The temperature differential and initial X offset must be set with "M605 S2 X[offs] R[deg]",
   *             then followed by "M605 S3" to initiate mirrored movement.
   *
   *    M605 W  : IDEX What? command.
   *
   *    Note: the X axis should be homed after changing Dual X-carriage mode.
   */
  void GcodeSuite::M605() {
    planner.synchronize();

    if (parser.seen('S')) {
      const DualXMode previous_mode = dual_x_carriage_mode;

      dual_x_carriage_mode = (DualXMode)parser.value_byte();
      mirrored_duplication_mode = false;

      if (dual_x_carriage_mode == DXC_MIRRORED_MODE) {
        if (previous_mode != DXC_DUPLICATION_MODE) {
          SERIAL_ECHOLNPGM("Printer must be in DXC_DUPLICATION_MODE prior to ");
          SERIAL_ECHOLNPGM("specifying DXC_MIRRORED_MODE.");
          dual_x_carriage_mode = DEFAULT_DUAL_X_CARRIAGE_MODE;
          return;
        }
        mirrored_duplication_mode = true;
        stepper.set_directions();
        float x_jog = current_position.x - .1;
        for (uint8_t i = 2; --i;) {
          planner.buffer_line(x_jog, current_position.y, current_position.z, current_position.e, feedrate_mm_s, 0);
          x_jog += .1;
        }
        return;
      }

      switch (dual_x_carriage_mode) {
        case DXC_FULL_CONTROL_MODE:
        case DXC_AUTO_PARK_MODE:
          break;
        case DXC_DUPLICATION_MODE:
          // Set the X offset, but no less than the safety gap
          if (parser.seen('X')) duplicate_extruder_x_offset = _MAX(parser.value_linear_units(), (X2_MIN_POS) - (X1_MIN_POS));
          if (parser.seen('R')) duplicate_extruder_temp_offset = parser.value_celsius_diff();
          // Always switch back to tool 0
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
        DEBUG_ECHO_START();
        DEBUG_ECHOPGM("Dual X Carriage Mode ");
        switch (dual_x_carriage_mode) {
          case DXC_FULL_CONTROL_MODE: DEBUG_ECHOPGM("FULL_CONTROL"); break;
          case DXC_AUTO_PARK_MODE:    DEBUG_ECHOPGM("AUTO_PARK");    break;
          case DXC_DUPLICATION_MODE:  DEBUG_ECHOPGM("DUPLICATION");  break;
          case DXC_MIRRORED_MODE:     DEBUG_ECHOPGM("MIRRORED");     break;
        }
        DEBUG_ECHOPAIR("\nActive Ext: ", int(active_extruder));
        if (!active_extruder_parked) DEBUG_ECHOPGM(" NOT ");
        DEBUG_ECHOPGM(" parked.");
        DEBUG_ECHOPAIR("\nactive_extruder_x_pos: ", current_position.x);
        DEBUG_ECHOPAIR("\ninactive_extruder_x_pos: ", inactive_extruder_x_pos);
        DEBUG_ECHOPAIR("\nextruder_duplication_enabled: ", int(extruder_duplication_enabled));
        DEBUG_ECHOPAIR("\nduplicate_extruder_x_offset: ", duplicate_extruder_x_offset);
        DEBUG_ECHOPAIR("\nduplicate_extruder_temp_offset: ", duplicate_extruder_temp_offset);
        DEBUG_ECHOPAIR("\ndelayed_move_time: ", delayed_move_time);
        DEBUG_ECHOPAIR("\nX1 Home X: ", x_home_pos(0), "\nX1_MIN_POS=", int(X1_MIN_POS), "\nX1_MAX_POS=", int(X1_MAX_POS));
        DEBUG_ECHOPAIR("\nX2 Home X: ", x_home_pos(1), "\nX2_MIN_POS=", int(X2_MIN_POS), "\nX2_MAX_POS=", int(X2_MAX_POS));
        DEBUG_ECHOPAIR("\nX2_HOME_DIR=", int(X2_HOME_DIR), "\nX2_HOME_POS=", int(X2_HOME_POS));
        DEBUG_ECHOPAIR("\nDEFAULT_DUAL_X_CARRIAGE_MODE=", STRINGIFY(DEFAULT_DUAL_X_CARRIAGE_MODE));
        DEBUG_ECHOPAIR("\toolchange_settings.z_raise=", toolchange_settings.z_raise);
        DEBUG_ECHOPAIR("\nDEFAULT_DUPLICATION_X_OFFSET=", int(DEFAULT_DUPLICATION_X_OFFSET));
        DEBUG_EOL();

        HOTEND_LOOP() {
          DEBUG_ECHOPAIR_P(SP_T_STR, int(e));
          LOOP_XYZ(a) DEBUG_ECHOPAIR("  hotend_offset[", int(e), "].", XYZ_CHAR(a) | 0x20, "=", hotend_offset[e][a]);
          DEBUG_EOL();
        }
        DEBUG_EOL();
      }
    #endif // DEBUG_DXC_MODE
  }

#elif ENABLED(MULTI_NOZZLE_DUPLICATION)

  /**
   * M605: Set multi-nozzle duplication mode
   *
   *  S2       - Enable duplication mode
   *  P[mask]  - Bit-mask of nozzles to include in the duplication set.
   *             A value of 0 disables duplication.
   *  E[index] - Last nozzle index to include in the duplication set.
   *             A value of 0 disables duplication.
   */
  void GcodeSuite::M605() {
    bool ena = false;
    if (parser.seen("EPS")) {
      planner.synchronize();
      if (parser.seenval('P')) duplication_e_mask = parser.value_int();   // Set the mask directly
      else if (parser.seenval('E')) duplication_e_mask = pow(2, parser.value_int() + 1) - 1; // Set the mask by E index
      ena = (2 == parser.intval('S', extruder_duplication_enabled ? 2 : 0));
      extruder_duplication_enabled = ena && (duplication_e_mask >= 3);
    }
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(STR_DUPLICATION_MODE);
    serialprint_onoff(extruder_duplication_enabled);
    if (ena) {
      SERIAL_ECHOPGM(" ( ");
      HOTEND_LOOP() if (TEST(duplication_e_mask, e)) { SERIAL_ECHO(e); SERIAL_CHAR(' '); }
      SERIAL_CHAR(')');
    }
    SERIAL_EOL();
  }

#endif // MULTI_NOZZLE_DUPLICATION

#endif // HAS_DUPICATION_MODE
