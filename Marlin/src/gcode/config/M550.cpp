/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(CONFIGURABLE_MACHINE_NAME)

#include "../gcode.h"
#include "../../MarlinCore.h"
#include "../../lcd/marlinui.h"

/**
 * M550: Set machine name
 *
 * Parameters:
 *   P<name> - Set the name using the 'P' parameter and following string without spaces
 *             (NOTE: ALL CAPS unless GCODE_CASE_INSENSITIVE is enabled.)
 *
 * With GCODE_QUOTED_STRINGS:
 *   P "<name>" Get the name from the 'P' parameter, quoting required for spaces in the name
 *   "<name>" Get the name from the "string" parameter
 */
void GcodeSuite::M550() {
  bool did_set = true;

  if (parser.seenval('P')) {
    #if ENABLED(GCODE_QUOTED_STRINGS)
      machine_name = &parser.string_arg[1];
    #else
      machine_name = parser.value_string();
    #endif
  }
  #if ENABLED(GCODE_QUOTED_STRINGS)
    else if (parser.has_string())
      machine_name = parser.string_arg;
  #endif
  else
    did_set = false;

  if (did_set) {
    machine_name.trim();
    ui.reset_status(false);
  }
  else
    SERIAL_ECHOLNPGM("RepRap name: ", &machine_name);
}

#endif // CONFIGURABLE_MACHINE_NAME
