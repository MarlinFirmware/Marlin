/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(EDITABLE_SERVO_ANGLES)

#include "../gcode.h"
#include "../../module/servo.h"

void GcodeSuite::M281() {
  if (!parser.seenval('P')) return;
  const int servo_index = parser.value_int();
  if (WITHIN(servo_index, 0, NUM_SERVOS - 1)) {
    bool angle_change = false;
    if (parser.seen('L')) {
      servo_angles[servo_index][0] = parser.value_int();
      angle_change = true;
    }
    if (parser.seen('U')) {
      servo_angles[servo_index][1] = parser.value_int();
      angle_change = true;
    }
    if (!angle_change) {
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR(" Servo ", servo_index);
      SERIAL_ECHOPAIR(" L", servo_angles[servo_index][0]);
      SERIAL_ECHOLNPAIR(" U", servo_angles[servo_index][1]);
    }
  }
  else {
    SERIAL_ERROR_START();
    SERIAL_ECHOPAIR("Servo ", servo_index);
    SERIAL_ECHOLNPGM(" out of range");
  }
}

#endif // EDITABLE_SERVO_ANGLES
