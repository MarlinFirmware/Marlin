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

void report_workspace_plane() {
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Workspace Plane ");
  serialprintPGM(workspace_plane == PLANE_YZ ? PSTR("YZ\n") : workspace_plane == PLANE_ZX ? PSTR("ZX\n") : PSTR("XY\n"));
}

/**
 * G17: Select Plane XY
 * G18: Select Plane ZX
 * G19: Select Plane YZ
 */
void gcode_G17() { workspace_plane = PLANE_XY; }
void gcode_G18() { workspace_plane = PLANE_ZX; }
void gcode_G19() { workspace_plane = PLANE_YZ; }
