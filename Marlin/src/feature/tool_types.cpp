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
 * feature/tool_types.cpp
 */

#include "../inc/MarlinConfig.h"

#if HAS_TOOL_TYPES

#include "tool_types.h"

ToolType tool_type;

/**
 * Tool Selection:
 *  0 - Extruder
 *  1 - Laser
 *  2 - CNC Mill
 */
void set_tool_type(const ToolType type) {
  switch (type) {
    default: return;

    case TOOL_TYPE_EXTRUDER:
      current_position.e = 0;
      planner.set_e_position_mm(0);
      break;

    case TOOL_TYPE_LASER:
      break;

    case TOOL_TYPE_CNC_MILL:
      break;
  }

  tool_type = type;

  // Turn Timer 5 on or off depending on tool
  //set_pwm_frequency(FAN_PIN tool_type == TOOL_TYPE_LASER ? 25000 : 0);

  thermalManager.tool_changed();
}

void report_tool_type() {
  const static char str_tooltype_0[] PROGMEM = "Extruder";
  const static char str_tooltype_1[] PROGMEM = "Laser";
  const static char str_tooltype_2[] PROGMEM = "CNC Mill";
  const static char* const tool_strings[] PROGMEM = { str_tooltype_0, str_tooltype_1, str_tooltype_2 };
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Tool: ");
  serialprintPGM((char*)pgm_read_word(&(tool_strings[tool_type])));
  SERIAL_CHAR(' ');
  SERIAL_ECHOLN((int)(tool_type == TOOL_TYPE_EXTRUDER ? active_extruder : 0));
}

#endif // HAS_TOOL_TYPES
