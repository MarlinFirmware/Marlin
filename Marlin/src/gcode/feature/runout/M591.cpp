/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 *  R           : Reset the runout sensor
 *  S<bool>     : Enable/disable runout detection for the target extruder
 *  D<linear>   : Extra distance (mm) to continue after runout is triggered
 *  L<linear>   : Alias for D
 *  P<mode>     : Set sensor mode for the target extruder:
 *                  0 = NONE (disabled)
 *                  1 = Switch NO  (HIGH = filament present)
 *                  2 = Switch NC  (LOW  = filament present)
 *                  7 = Motion sensor
 *  H<bool>     : Enable/disable host-action runout handling
 *                (Requires HOST_ACTION_COMMANDS)
 *  E<index>    : Target extruder (default: motion.extruder)
 *                (Requires MULTI_FILAMENT_SENSOR)
 *
 * With no arguments: report current state for the active extruder.
 */
void GcodeSuite::M591() {

  const uint8_t tool = TERN0(MULTI_FILAMENT_SENSOR, parser.ushortval('E', motion.extruder));

  if (parser.seen("RSLDP"
    TERN_(HOST_ACTION_COMMANDS, "H")
    TERN_(MULTI_FILAMENT_SENSOR, "E")
  )) {

    #if ENABLED(HOST_ACTION_COMMANDS)
      if (parser.seen('H')) runout.host_handling = parser.value_bool();
    #endif

    const bool seenR = parser.seen_test('R'), seenS = parser.seen('S');
    if (seenR || seenS) runout.reset();
    if (seenS) runout.enabled[tool] = parser.value_bool();

    #if HAS_FILAMENT_RUNOUT_DISTANCE
      if (parser.seenval('D') || parser.seenval('L'))
        runout.set_runout_distance(parser.value_linear_units(), tool);
    #endif

    if (parser.seenval('P')) {
      const RunoutMode tmp_mode = (RunoutMode)parser.value_int();
      switch (tmp_mode) {
        case RM_NONE ... RM_OUT_ON_HIGH:
        case RM_MOTION_SENSOR:
          runout.mode[tool] = tmp_mode;
          runout.setup();
          break;
        default: break;
      }
    }
  }
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Runout sensor"
      #if ENABLED(MULTI_FILAMENT_SENSOR)
        " E", tool
      #endif
    );
    SERIAL_ECHOPGM(": ", ON_OFF(runout.enabled[tool]));
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      SERIAL_ECHOPGM(" ; D", runout.runout_distance(tool), "mm");
    #endif
    SERIAL_ECHOPGM(" ; P", (uint8_t)runout.mode[tool]);
    #if ENABLED(HOST_ACTION_COMMANDS)
      SERIAL_ECHOPGM(" ; Host ", ON_OFF(runout.host_handling));
    #endif
    SERIAL_EOL();
  }
}

void GcodeSuite::M591_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_FILAMENT_RUNOUT_SENSOR));
  for (uint8_t e = 0; e < NUM_RUNOUT_SENSORS; ++e) {
    SERIAL_ECHOPGM("  M591");
    #if ENABLED(MULTI_FILAMENT_SENSOR)
      SERIAL_ECHOPGM(" E", e);
    #endif
    SERIAL_ECHOPGM(" S", runout.enabled[e] ? 1 : 0);
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      SERIAL_ECHOPGM(" D", LINEAR_UNIT(runout.runout_distance(e)));
    #endif
    SERIAL_ECHOLNPGM(" P", (uint8_t)runout.mode[e]);
  }
}

#endif // HAS_FILAMENT_SENSOR
