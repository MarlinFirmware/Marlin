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

#include "../../inc/MarlinConfig.h"

#if ENABLED(CNC_WORKSPACE_PLANES)

#include "../gcode.h"

inline void report_workspace_plane() {
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM("Workspace Plane ");
  serialprintPGM(
      gcode.workspace_plane == GcodeSuite::PLANE_YZ ? PSTR("YZ\n")
    : gcode.workspace_plane == GcodeSuite::PLANE_ZX ? PSTR("ZX\n")
                                                    : PSTR("XY\n")
  );
}

inline void set_workspace_plane(const GcodeSuite::WorkspacePlane plane) {
  gcode.workspace_plane = plane;
  if (DEBUGGING(INFO)) report_workspace_plane();
}

/**
 * G17: Select Plane XY
 * G18: Select Plane ZX
 * G19: Select Plane YZ
 */
void GcodeSuite::G17() { set_workspace_plane(PLANE_XY); }
void GcodeSuite::G18() { set_workspace_plane(PLANE_ZX); }
void GcodeSuite::G19() { set_workspace_plane(PLANE_YZ); }

#endif // CNC_WORKSPACE_PLANES
