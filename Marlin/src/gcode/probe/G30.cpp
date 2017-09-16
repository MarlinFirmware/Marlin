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

#include "../../inc/MarlinConfig.h"

#if HAS_BED_PROBE

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/probe.h"
#include "../../feature/bedlevel/bedlevel.h"

/**
 * G30: Do a single Z probe at the current XY
 *
 * Parameters:
 *
 *   X   Probe X position (default current X)
 *   Y   Probe Y position (default current Y)
 *   S0  Leave the probe deployed
 */
void GcodeSuite::G30() {
  const float xpos = parser.linearval('X', current_position[X_AXIS] + X_PROBE_OFFSET_FROM_EXTRUDER),
              ypos = parser.linearval('Y', current_position[Y_AXIS] + Y_PROBE_OFFSET_FROM_EXTRUDER);

  if (!position_is_reachable_by_probe_xy(xpos, ypos)) return;

  // Disable leveling so the planner won't mess with us
  #if HAS_LEVELING
    set_bed_leveling_enabled(false);
  #endif

  setup_for_endstop_or_probe_move();

  const float measured_z = probe_pt(xpos, ypos, parser.boolval('S', true), 1);

  if (!isnan(measured_z)) {
    SERIAL_PROTOCOLPAIR("Bed X: ", FIXFLOAT(xpos));
    SERIAL_PROTOCOLPAIR(" Y: ", FIXFLOAT(ypos));
    SERIAL_PROTOCOLLNPAIR(" Z: ", FIXFLOAT(measured_z));
  }

  clean_up_after_endstop_or_probe_move();

  report_current_position();
}

#endif // HAS_BED_PROBE
