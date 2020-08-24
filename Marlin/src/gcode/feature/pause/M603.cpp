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

#if ENABLED(ADVANCED_PAUSE_FEATURE)

#include "../../gcode.h"
#include "../../../feature/pause.h"
#include "../../../module/motion.h"
#include "../../../module/printcounter.h"

#if EXTRUDERS > 1
  #include "../../../module/tool_change.h"
#endif

/**
 * M603: Configure filament change
 *
 *  T[toolhead] - Select extruder to configure, active extruder if not specified
 *  U[distance] - Retract distance for removal, for the specified extruder
 *  L[distance] - Extrude distance for insertion, for the specified extruder
 *
 */
void GcodeSuite::M603() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  // Unload length
  if (parser.seen('U')) {
    fc_settings[target_extruder].unload_length = ABS(parser.value_axis_units(E_AXIS));
    #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
      NOMORE(fc_settings[target_extruder].unload_length, EXTRUDE_MAXLENGTH);
    #endif
  }

  // Load length
  if (parser.seen('L')) {
    fc_settings[target_extruder].load_length = ABS(parser.value_axis_units(E_AXIS));
    #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
      NOMORE(fc_settings[target_extruder].load_length, EXTRUDE_MAXLENGTH);
    #endif
  }
}

#endif // ADVANCED_PAUSE_FEATURE
