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
 *  P "<name>" Set the name using the 'P' parameter (RepRapFirmware)
 *  "<name>" Set the name using the "string" parameter
 */
void GcodeSuite::M550() {
  bool did_set = true;

  if (parser.seenval('P'))
    machine_name = parser.value_string();
  else if (TERN(GCODE_QUOTED_STRINGS, false, parser.seen('P')))
    machine_name = parser.string_arg[0] == 'P' ? &parser.string_arg[1] : parser.string_arg;
  else if (parser.string_arg && parser.string_arg[0])
    machine_name = parser.string_arg;
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
