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

/**
 * M672 - Set/clear Duet Smart Effector sensitivity
 *
 *  One of these is required:
 *    S<sensitivity> - 0-255
 *    R              - Resets sensitivity to default
 *
 */


#define M672_PROGBYTE    105								// magic byte to start programming custom sensitivity
#define M672_ERASEBYTE   131								// magic byte to clear custom sensitivity

void M672_SpiTransfer_Mode_0(uint8_t b) { // using Mode 0
  for (uint8_t bits = 8; bits--;) {
    OUT_WRITE(M672_MOSI_PIN, b & 0x80);
    b <<= 1;        // little setup time
    DELAY_NS(125);
    OUT_WRITE(M672_SCK_PIN, HIGH);
    DELAY_NS(1000);

    OUT_WRITE(M672_SCK_PIN, LOW);
    DELAY_NS(1000);
  }
  DELAY_NS(1000);
}

void M672_SpiTransfer_Mode_3(uint8_t b) { // using Mode 3
  for (uint8_t bits = 8; bits--;) {
    OUT_WRITE(M672_SCK_PIN, LOW);
    DELAY_NS(125);
    OUT_WRITE(M672_MOSI_PIN, b & 0x80);

    DELAY_NS(1000);
    OUT_WRITE(M672_SCK_PIN, HIGH);
    DELAY_NS(1000);

    b <<= 1;        // little setup time
  }
  DELAY_NS(1000);

}


void M672_send(uint8_t b) { M672_SpiTransfer_Mode_3(b);}

void GcodeSuite::M672() {
  SERIAL_ECHO_MSG("M672");
  if (parser.seenval('R')) {
    OUT_WRITE(M672_SCK_PIN, LOW);  // put into programming mode
    DELAY_NS(1000);
    M672_send(M672_ERASEBYTE);
    M672_send(M672_ERASEBYTE);
  }
  else if (parser.seenval('S')) {
    const int8_t M672_sensitivity = parser.byteval('S');
    OUT_WRITE(M672_SCK_PIN, LOW);  // put into programming mode
    DELAY_NS(1000);
    M672_send(M672_PROGBYTE);
    M672_send(M672_sensitivity);
    M672_send(255 - M672_sensitivity);
  }
  else
    SERIAL_ECHO_MSG("'S' or 'R' parameter required");
  }
  pinmode(M672_MOSI_PIN, INPUT);  // set MOSI pin back to input (Z-probe connector IN pin)
  pinmode(M672_SCK_PIN, INPUT);   // set SCK pin back to floating (Z-probe connector MOD pin)
}

#endif // SMART_EFFECTOR
