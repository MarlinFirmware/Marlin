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

#include "../gcode.h"
#include "../../Marlin.h" // for pin_is_protected
#include "../../inc/MarlinConfig.h"

/**
 * M42: Change pin status via GCode
 *
 *  P<pin>  Pin number (LED if omitted)
 *            For LPC1768 use M42 P1.20 S255 if wanting to set P1_20 to logic 1
 *              NOTE - Repetier Host truncates trailing zeros on a decimal when
 *                     sending commands so typing M42 P1.20 S255 results in 
 *                     M42 P1.2 S255 being sent.  Pronterface doesn't have this issue.
 *                     
 *  S<byte> Pin status from 0 - 255
 */
void GcodeSuite::M42() {
  if (!parser.seenval('S')) return;
  const byte pin_status = parser.value_byte();

  int pin_number = PARSED_PIN_INDEX('P', GET_PIN_MAP_INDEX(LED_PIN));
  if (pin_number < 0) return;

  const pin_t pin = GET_PIN_MAP_PIN(pin_number);
  if (pin_is_protected(pin)) {
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_PROTECTED_PIN);
    return;
  }

  pinMode(pin, OUTPUT);
  digitalWrite(pin, pin_status);
  analogWrite(pin, pin_status);

  #if FAN_COUNT > 0
    switch (pin) {
      #if HAS_FAN0
        case FAN_PIN: fanSpeeds[0] = pin_status; break;
      #endif
      #if HAS_FAN1
        case FAN1_PIN: fanSpeeds[1] = pin_status; break;
      #endif
      #if HAS_FAN2
        case FAN2_PIN: fanSpeeds[2] = pin_status; break;
      #endif
    }
  #endif
}
