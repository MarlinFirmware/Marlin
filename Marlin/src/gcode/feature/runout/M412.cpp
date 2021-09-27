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
 * M412: Enable / Disable filament runout detection
 *
 * Parameters
 *  R         : Reset the runout sensor
 *  S<bool>   : Reset and enable/disable the runout sensor
 *  H<bool>   : Enable/disable host handling of filament runout
 *  D<linear> : Extra distance to continue after runout is triggered
 */
void GcodeSuite::M412() {
  if (parser.seen("RS"
    TERN_(HAS_FILAMENT_RUNOUT_DISTANCE, "D")
    TERN_(HOST_ACTION_COMMANDS, "H")
  )) {
    #if ENABLED(HOST_ACTION_COMMANDS)
      if (parser.seen('H')) runout.host_handling = parser.value_bool();
    #endif
    const bool seenR = parser.seen_test('R'), seenS = parser.seen('S');
    if (seenR || seenS) runout.reset();
    if (seenS) runout.enabled = parser.value_bool();
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      if (parser.seen('D')) runout.set_runout_distance(parser.value_linear_units());
    #endif
  }
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Filament runout ");
    serialprint_onoff(runout.enabled);
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      SERIAL_ECHOPGM(" ; Distance ", runout.runout_distance(), "mm");
    #endif
    #if ENABLED(HOST_ACTION_COMMANDS)
      SERIAL_ECHOPGM(" ; Host handling ");
      serialprint_onoff(runout.host_handling);
    #endif
    SERIAL_EOL();
  }
}

void GcodeSuite::M412_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, PSTR(STR_FILAMENT_RUNOUT_SENSOR));
  SERIAL_ECHOPGM(
    "  M412 S", runout.enabled
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      , " D", LINEAR_UNIT(runout.runout_distance())
    #endif
    , " ; Sensor "
  );
  serialprintln_onoff(runout.enabled);
}

#endif // HAS_FILAMENT_SENSOR
