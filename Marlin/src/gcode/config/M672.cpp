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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(SMART_EFFECTOR)

#include "../gcode.h"
#include "../../HAL/shared/Delay.h"
#include "../parser.h"

/**
 * M672 - Set/clear Duet Smart Effector sensitivity
 *
 *  One of these is required:
 *    S<sensitivity> - 0-255
 *    R              - Resets sensitivity to default
 */

/**
 * The Marlin format for the M672 command is different than shown in the Duet Smart Effector
 * documantation https://duet3d.dozuki.com/Wiki/Smart_effector_and_carriage_adapters_for_delta_printer#main
 *
 * Setting custom sensitivity:
 *   Duet:   M672 S105:aaa:bbb
 *   Marlin: M672 Saaa
 *
 *   aaa by the desired sensitivity and bbb is 255 - aaa.
 *
 * Revert sensitivity to factory settings:
 *   Duet:   M672 S105:131:131
 *   Marlin: M672 R
 *
 */

#define M672_PROGBYTE    105								// magic byte to start programming custom sensitivity
#define M672_ERASEBYTE   131								// magic byte to clear custom sensitivity


// The protocol for each byte sent is:
//    0 0 1 0 b7 b6 b5 b4 /b4 b3 b2 b1 b0 /b0

void M672_send(uint8_t b) {    // bit rate requirement: 1KHz +/- 30%
  for (uint8_t bits = 0; bits < 14; bits++) {
    switch(bits) {
      case 0:
      case 1:
      case 3: {OUT_WRITE(M672_MOD_PIN, LOW); break;}
      case 2: {OUT_WRITE(M672_MOD_PIN, HIGH); break;}
      case 7:
      case 12: {OUT_WRITE(M672_MOD_PIN, b & 0x80); break;}   // don't shift
      case 8:
      case 13: {OUT_WRITE(M672_MOD_PIN, !(b & 0x80)); b <<= 1; break;}  // send inverted previous bit
      default: {OUT_WRITE(M672_MOD_PIN, b & 0x80); b <<= 1; break;}
    }
    DELAY_US(1000);
  }
}


void GcodeSuite::M672() {
  SERIAL_ECHO_MSG("M672\n");
  if (parser.seen('R')) {
    M672_send(M672_ERASEBYTE);
    M672_send(M672_ERASEBYTE);
  }
  else if (parser.seen('S')) {
    const int8_t M672_sensitivity = parser.byteval('S');
    M672_send(M672_PROGBYTE);
    M672_send(M672_sensitivity);
    M672_send(255 - M672_sensitivity);
  }
  else {
    SERIAL_ECHO_MSG("'S' or 'R' parameter required\n");
  }

  OUT_WRITE(M672_MOD_PIN, LOW);  // keep Smart Effector in normal mode
}

#endif // SMART_EFFECTOR
