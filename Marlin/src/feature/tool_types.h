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
 * feature/tool_types.h
 */

enum ToolType : char {
  TOOL_TYPE_EXTRUDER, // M450 S0 or M451
  TOOL_TYPE_LASER,    // M450 S1 or M452
  TOOL_TYPE_CNC_MILL, // M450 S2 or M453
  TOOL_TYPE_NONE = -1
};

extern ToolType tool_type;

void set_tool_type(const ToolType type);
