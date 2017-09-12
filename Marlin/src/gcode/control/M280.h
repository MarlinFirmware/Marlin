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
 * M280: Get or set servo position. P<index> [S<angle>]
 */
void gcode_M280() {
  if (!parser.seen('P')) return;
  const int servo_index = parser.value_int();
  if (WITHIN(servo_index, 0, NUM_SERVOS - 1)) {
    if (parser.seen('S'))
      MOVE_SERVO(servo_index, parser.value_int());
    else {
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR(" Servo ", servo_index);
      SERIAL_ECHOLNPAIR(": ", servo[servo_index].read());
    }
  }
  else {
    SERIAL_ERROR_START();
    SERIAL_ECHOPAIR("Servo ", servo_index);
    SERIAL_ECHOLNPGM(" out of range");
  }
}
