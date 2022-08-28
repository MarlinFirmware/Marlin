/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if HAS_GCODE_M255

#include "../gcode.h"
#include "../../lcd/marlinui.h"

/**
 * M255: Set the LCD sleep timeout (in minutes)
 *  S<minutes> - Period of inactivity required for display / backlight sleep
 */
void GcodeSuite::M255() {
  if (parser.seenval('S')) {
    const int m = parser.value_int();
    #if HAS_DISPLAY_SLEEP
      ui.sleep_timeout_minutes = constrain(m, ui.sleep_timeout_min, ui.sleep_timeout_max);
    #else
      ui.backlight_timeout_minutes = constrain(m, ui.backlight_timeout_min, ui.backlight_timeout_max);
    #endif
  }
  else
    M255_report();
}

void GcodeSuite::M255_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_DISPLAY_SLEEP));
  SERIAL_ECHOLNPGM("  M255 S",
    TERN(HAS_DISPLAY_SLEEP, ui.sleep_timeout_minutes, ui.backlight_timeout_minutes),
    " ; (minutes)"
  );
}

#endif // HAS_GCODE_M255
