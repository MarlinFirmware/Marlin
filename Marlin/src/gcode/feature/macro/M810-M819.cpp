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

#if ENABLED(GCODE_MACROS)

#include "../../gcode.h"
#include "../../queue.h"
#include "../../parser.h"

char gcode_macros[GCODE_MACROS_SLOTS][GCODE_MACROS_SLOT_SIZE + 1] = {{ 0 }};

/**
 * M810_819: Set/execute a G-code macro.
 *
 * Usage:
 *   M810 <command>|...   Set Macro 0 to the given commands, separated by the pipe character
 *   M810                 Execute Macro 0
 */
void GcodeSuite::M810_819() {
  const uint8_t index = parser.codenum - 810;
  if (index >= GCODE_MACROS_SLOTS) return;

  const size_t len = strlen(parser.string_arg);

  if (len) {
    // Set a macro
    if (len > GCODE_MACROS_SLOT_SIZE)
      SERIAL_ERROR_MSG("Macro too long.");
    else {
      char c, *s = parser.string_arg, *d = gcode_macros[index];
      do {
        c = *s++;
        *d++ = c == '|' ? '\n' : c;
      } while (c);
    }
  }
  else {
    // Execute a macro
    char * const cmd = gcode_macros[index];
    if (strlen(cmd)) process_subcommands_now(cmd);
  }
}

#endif // GCODE_MACROS
