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

#include "../../../inc/MarlinConfig.h"

#if HAS_FILAMENT_SENSOR

#include "../../gcode.h"
#include "../../../feature/runout.h"

/**
 * M591: Configure filament runout detection
 *
 * Parameters
 *  R         : Reset the runout sensor
 *  S<bool>   : Reset and enable/disable the runout sensor
 *  H<bool>   : Enable/disable host handling of filament runout
 *  L<linear> : Extra distance to continue after runout is triggered or motion interval
 *  D<linear> : Alias for L
 *  P<index>  : Mode 0 = NONE
 *                   1 = Switch NO (HIGH = filament present)
 *                   2 = Switch NC (LOW = filament present)
 *                   3 = Encoder / Motion Sensor
 */
void GcodeSuite::M591() {
  if (parser.seen("RSDP" TERN_(HOST_ACTION_COMMANDS, "H"))) {
    #if ENABLED(HOST_ACTION_COMMANDS)
      if (parser.seen('H')) runout.host_handling = parser.value_bool();
    #endif
    const bool seenR = parser.seen_test('R'), seenS = parser.seen('S');
    if (seenR || seenS) runout.reset();
    const uint8_t tool = TERN0(MULTI_FILAMENT_SENSOR, parser.ushortval('E', active_extruder));
    if (seenS) runout.enabled[tool] = parser.value_bool();
    if (parser.seen('D') || parser.seen('L')) runout.set_runout_distance(parser.value_linear_units(), tool);
    if (parser.seen('P')) {
      const RunoutMode tmp_mode = (RunoutMode)parser.value_int();
      switch (tmp_mode) {
        case RM_NONE ... RM_OUT_ON_HIGH:
        case RM_MOTION_SENSOR:
          runout.mode[tool] = tmp_mode;
          runout.setup();
        default: break;
      }
    }
  }
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Filament runout ");
    serialprint_onoff(runout.enabled[active_extruder]);
    SERIAL_ECHOPGM(" ; Distance ", runout.runout_distance(active_extruder), "mm");
    SERIAL_ECHOPGM(" ; Mode ", runout.mode[active_extruder]);
    #if ENABLED(HOST_ACTION_COMMANDS)
      SERIAL_ECHOPGM(" ; Host handling ");
      serialprint_onoff(runout.host_handling);
    #endif
    SERIAL_EOL();
  }
}

void GcodeSuite::M591_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_FILAMENT_RUNOUT_SENSOR));
  LOOP_S_L_N(e, 1, NUM_RUNOUT_SENSORS)
    SERIAL_ECHOLNPGM(
      "  M591"
      #if MULTI_FILAMENT_SENSOR
        " E", e,
      #endif
        " S", runout.enabled[e]
      , " D", LINEAR_UNIT(runout.runout_distance(e))
      , " P", runout.mode[e]
    );
}

#endif // HAS_FILAMENT_SENSOR
