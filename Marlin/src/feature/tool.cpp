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

/**
 * feature/tool.cpp
 */

#include "../inc/MarlinConfig.h"

#if HAS_TOOLS

#include "tool.h"
#include "../module/motion.h"
#include "../module/temperature.h"

MarlinTool tool;

ToolType MarlinTool::type = TOOL_TYPE_EXTRUDER;

/**
 * Tool Selection:
 *  0 - Extruder
 *  1 - Laser
 *  2 - Spindle
 */
void MarlinTool::select(const ToolType intype) {
  if (intype == type) return;

  type = intype;

  switch (intype) {
    case TOOL_TYPE_NONE:
      break;

    case TOOL_TYPE_EXTRUDER:
      #if HAS_HOTEND
        current_position.e = 0;
        planner.set_e_position_mm(0);
      #endif
      break;

    case TOOL_TYPE_LASER:
      break;

    case TOOL_TYPE_SPINDLE:
      break;

    default: return;
  }

  // Set PWM Timer on or off depending on tool
  //set_pwm_frequency(FAN_PIN type == TOOL_TYPE_LASER ? 25000 : 0);

  // Kill heaters when changing tool modes
  thermalManager.disable_all_heaters();
}

void MarlinTool::report() {
  static PGMSTR(str_tooltype_0, "Extruder");
  static PGMSTR(str_tooltype_1, "Laser");
  static PGMSTR(str_tooltype_2, "Spindle");
  static PGM_P const tool_strings[] PROGMEM = { str_tooltype_0, str_tooltype_1, str_tooltype_2 };
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Tool: ");
  serialprintPGM((char*)pgm_read_ptr(&(tool_strings[type])));
  SERIAL_CHAR(' ');
  SERIAL_ECHOLN((int)(type == TOOL_TYPE_EXTRUDER ? active_extruder : 0));
}

bool MarlinTool::is_selected(const ToolType intype, const bool noerr/*=false*/) {
  if (type != intype) {
    if (!noerr) SERIAL_ERROR_MSG(STR_ERR_WRONG_TOOL);
    return true;
  }
  return false;
}

#endif // HAS_TOOLS
