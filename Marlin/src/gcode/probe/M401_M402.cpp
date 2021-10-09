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

#if HAS_BED_PROBE

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/probe.h"
#if ENABLED(BLTOUCH)
  #include "../../feature/bltouch.h"
#endif

/**
 * M401: Deploy and activate the Z probe
 */
void GcodeSuite::M401() {
  #if ENABLED(BLTOUCH)
    const bool seen_S = parser.seen('S'),
             to_enable = (seen_S && parser.value_bool());
    if (seen_S)
      if(to_enable)
       bltouch.bltouch_high_speed = true;
      else
        bltouch.bltouch_high_speed = false;
  #endif
  probe.deploy();
  TERN_(PROBE_TARE, probe.tare());
  report_current_position();
}

/**
 * M402: Deactivate and stow the Z probe
 */
void GcodeSuite::M402() {
  probe.stow();
  probe.move_z_after_probing();
  report_current_position();
}

#endif // HAS_BED_PROBE
