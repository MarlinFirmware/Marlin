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

/**
 * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z E B
 *       S# determines MS1 or MS2, X# sets the pin high/low.
 */
void gcode_M351() {
  if (parser.seenval('S')) switch (parser.value_byte()) {
    case 1:
      LOOP_XYZE(i) if (parser.seenval(axis_codes[i])) stepper.microstep_ms(i, parser.value_byte(), -1);
      if (parser.seenval('B')) stepper.microstep_ms(4, parser.value_byte(), -1);
      break;
    case 2:
      LOOP_XYZE(i) if (parser.seenval(axis_codes[i])) stepper.microstep_ms(i, -1, parser.value_byte());
      if (parser.seenval('B')) stepper.microstep_ms(4, -1, parser.value_byte());
      break;
  }
  stepper.microstep_readings();
}
