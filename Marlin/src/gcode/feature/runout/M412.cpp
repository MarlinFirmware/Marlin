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

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

#include "../../gcode.h"
#include "../../../feature/runout.h"

/**
 * M412: Enable / Disable filament runout detection
 */
void GcodeSuite::M412() {
  if (parser.seen('S')) {
    runout.reset();
    if(parser.value_bool()) {
      runout.enabled = true;
      SERIAL_ECHOLN("Runout Enabled");
    }
    else {
      runout.enabled = false;
      SERIAL_ECHOLN("Runout Disabled");
    }
    
  }
  else if (parser.seen('R')) {
    runout.reset();
    SERIAL_ECHOLN("Runout Reset");
  }
  else if (parser.seen('H')) {
    runout.host_handling = parser.value_bool();
    if(runout.host_handling)
      SERIAL_ECHOLN("Runout Host Mode");
    else
      SERIAL_ECHOLN("Runout Local Mode");
  }
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Filament runout ");
    serialprintln_onoff(runout.enabled);
  }
}

#endif // FILAMENT_RUNOUT_SENSOR
