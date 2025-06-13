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
 * M550: Set or Report Machine Name
 *
 *   <name> - Set the name using the "string" parameter
 *
 * With GCODE_QUOTED_STRINGS:
 *   "<name>" - Set the name using the "string" parameter in quotes
 *              NOTE: Works with or without quotes
 *
 * Without parameters, this reports the current machine name
 */
void GcodeSuite::M550() {
  if (parser.has_string()) {
      machine_name = parser.string_arg;
      machine_name.trim();
    }
  else {
    SERIAL_ECHOLNPGM("RepRap name: ", &machine_name);
    return;
  }

  ui.reset_status(false);
}

#endif // CONFIGURABLE_MACHINE_NAME
