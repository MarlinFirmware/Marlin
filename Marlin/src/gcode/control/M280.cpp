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

#if HAS_SERVOS

#include "../gcode.h"
#include "../../module/servo.h"
#include "../../module/planner.h"

/**
 * M280: Get or set servo position. P<index> [S<angle>]
 */
void GcodeSuite::M280() {

  if (!parser.seen('P')) return;

  planner.synchronize();

  const int servo_index = parser.value_int();
  if (WITHIN(servo_index, 0, NUM_SERVOS - 1)) {
    if (parser.seen('S')) {
      const int a = parser.value_int();
      if (a == -1)
        DETACH_SERVO(servo_index);
      else {
        if (parser.seen('T')) {
          // distance to move / time passed -> reach destination after t ms.
          const int b = servo[servo_index].read();
          int16_t t = parser.value_int();
          NOLESS(t,0);
          uint32_t now = millis();
          uint32_t start = now;
          uint32_t until = now+t;
          float aMinusB = (float)(a-b);

          while(now<until) {
            now = millis()+50;
            now = now < until? now : until;
            float ratio = (float)(now-start) / (float)t;
            int i = (int)( aMinusB * ratio + b );
            MOVE_SERVO(servo_index, i);
          }
        }
        MOVE_SERVO(servo_index, a);
      }
    }
    else
      SERIAL_ECHO_MSG(" Servo ", servo_index, ": ", servo[servo_index].read());
  }
  else
    SERIAL_ERROR_MSG("Servo ", servo_index, " out of range");

}

#endif // HAS_SERVOS
