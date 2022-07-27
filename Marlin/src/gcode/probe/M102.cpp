/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * M102.cpp - Configure Bed Distance Sensor
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(BD_SENSOR)

#include "../gcode.h"
#include "../../feature/bedlevel/bdl/bdl.h"

/**
 * M102: Configure the Bed Distance Sensor
 */
void GcodeSuite::M102() {
  if (parser.seenval('S'))
    bdl.config_state = parser.value_int();
  else
    M102_report();
}

void GcodeSuite::M102_report(const bool forReplay/*=true*/) {
  report_heading(forReplay, F("Bed Distance Sensor"));
  SERIAL_ECHOLNPGM("  M102 S", bdl.config_state);
}

#endif // BD_SENSOR
