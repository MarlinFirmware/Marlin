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

#if ENABLED(EXT_SOLENOID) || ENABLED(MANUAL_SOLENOID_CONTROL)

#include "../gcode.h"
#include "../../feature/solenoid.h"
#include "../../module/motion.h"

/**
 * M380: Enable solenoid on the active extruder
 *
 *   S<index> to specify a solenoid (Requires MANUAL_SOLENOID_CONTROL)
 */
void GcodeSuite::M380() {
  #if ENABLED(MANUAL_SOLENOID_CONTROL)
    enable_solenoid(parser.seenval('S') ? parser.value_int() : active_extruder);
  #else
    enable_solenoid_on_active_extruder();
  #endif
}

/**
 * M381: Disable all solenoids
 */
void GcodeSuite::M381() { disable_all_solenoids(); }

#endif // EXT_SOLENOID || MANUAL_SOLENOID_CONTROL
