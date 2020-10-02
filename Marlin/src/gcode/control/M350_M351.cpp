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

#if HAS_MICROSTEPS

#include "../gcode.h"
#include "../../module/stepper.h"

/**
 * M350: Set axis microstepping modes. S sets mode for all drivers.
 *
 * Warning: Steps-per-unit remains unchanged.
 */
void GcodeSuite::M350() {
  if (parser.seen('S')) LOOP_LE_N(i, 4) stepper.microstep_mode(i, parser.value_byte());
  LOOP_XYZE(i) if (parser.seen(axis_codes[i])) stepper.microstep_mode(i, parser.value_byte());
  if (parser.seen('B')) stepper.microstep_mode(4, parser.value_byte());
  stepper.microstep_readings();
}

/**
 * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z E B
 *       S# determines MS1, MS2 or MS3, X# sets the pin high/low.
 */
void GcodeSuite::M351() {
  if (parser.seenval('S')) switch (parser.value_byte()) {
    case 1:
      LOOP_XYZE(i) if (parser.seenval(axis_codes[i])) stepper.microstep_ms(i, parser.value_byte(), -1, -1);
      if (parser.seenval('B')) stepper.microstep_ms(4, parser.value_byte(), -1, -1);
      break;
    case 2:
      LOOP_XYZE(i) if (parser.seenval(axis_codes[i])) stepper.microstep_ms(i, -1, parser.value_byte(), -1);
      if (parser.seenval('B')) stepper.microstep_ms(4, -1, parser.value_byte(), -1);
      break;
    case 3:
      LOOP_XYZE(i) if (parser.seenval(axis_codes[i])) stepper.microstep_ms(i, -1, -1, parser.value_byte());
      if (parser.seenval('B')) stepper.microstep_ms(4, -1, -1, parser.value_byte());
      break;
  }
  stepper.microstep_readings();
}

#endif // HAS_MICROSTEPS
