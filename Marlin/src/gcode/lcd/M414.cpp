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

#if HAS_MULTI_LANGUAGE

#include "../gcode.h"
#include "../../MarlinCore.h"
#include "../../lcd/marlinui.h"

/**
 * M414: Set the language for the UI
 *
 * Parameters
 *  S<index> : The language to select
 */
void GcodeSuite::M414() {

  if (parser.seenval('S'))
    ui.set_language(parser.value_byte());
  else
    M414_report();

}

void GcodeSuite::M414_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_UI_LANGUAGE));
  SERIAL_ECHOLNPGM("  M414 S", ui.language);
}

#endif // HAS_MULTI_LANGUAGE
