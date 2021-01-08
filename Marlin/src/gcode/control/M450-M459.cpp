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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_TOOLS

#include "../../feature/tool.h"
#include "../gcode.h"

static void set_printer_mode(const ToolType new_tool) {

  if (new_tool != TOOL_TYPE_NONE && new_tool < TOOL_TYPE_COUNT)
    tool.select(new_tool);

  tool.report();
}

/**
 * M450: Set and/or report current tool type
 *
 *  S<type> - The new tool type
 */
inline void GcodeSuite::M450() {
  set_printer_mode((ToolType)(parser.seen('S') ? parser.value_byte() : -1));
}

/**
 * M451: Select the FFF extruder tool
 */
inline void GcodeSuite::M451() { set_printer_mode(TOOL_TYPE_EXTRUDER); }

/**
 * M452: Select the Laser tool
 */
inline void GcodeSuite::M452() { set_printer_mode(TOOL_TYPE_LASER); }

/**
 * M453: Select the Spindle tool
 */
inline void GcodeSuite::M453() { set_printer_mode(TOOL_TYPE_SPINDLE); }

#endif // HAS_TOOLS
