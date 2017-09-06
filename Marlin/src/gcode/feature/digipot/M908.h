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
 * M908: Control digital trimpot directly (M908 P<pin> S<current>)
 */
void gcode_M908() {
  #if HAS_DIGIPOTSS
    stepper.digitalPotWrite(
      parser.intval('P'),
      parser.intval('S')
    );
  #endif
  #ifdef DAC_STEPPER_CURRENT
    dac_current_raw(
      parser.byteval('P', -1),
      parser.ushortval('S', 0)
    );
  #endif
}
