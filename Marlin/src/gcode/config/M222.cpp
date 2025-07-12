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

#include "../gcode.h"
#include "../../feature/spindle_laser.h"

#if HAS_CUTTER

/**
 * M222: Set/read spindle override (M222 S120)
 */

void GcodeSuite::M222() {
  if (parser.seenval('S')) {
    const uint8_t new_percentage = constrain((uint8_t)parser.value_int(), MIN_SPINDLE_OVERRIDE, MAX_SPINDLE_OVERRIDE);
    if (new_percentage != cutter.spindle_override) {
      cutter.spindle_override = new_percentage;
      if (cutter.enable_state)
        cutter.set_ocr(cutter.upower_to_ocr(cutter.unitPower));
    }
  }

  SERIAL_ECHOLNPGM("SPINDLE-OVERRIDE:", cutter.spindle_override);
}

#endif // Spindle Override
