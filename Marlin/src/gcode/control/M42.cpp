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
#include "../../MarlinCore.h" // for pin_is_protected
#include "../../inc/MarlinConfig.h"

#if HAS_FAN
  #include "../../module/temperature.h"
#endif

/**
 * M42: Change pin status via GCode
 *
 *  P<pin>  Pin number (LED if omitted)
 *          For LPC1768 specify pin P1_02 as M42 P102,
 *                                  P1_20 as M42 P120, etc.
 *
 *  S<byte> Pin status from 0 - 255
 *  I       Flag to ignore Marlin's pin protection
 *
 *  M<mode> Pin mode: 0=INPUT  1=OUTPUT  2=INPUT_PULLUP  3=INPUT_PULLDOWN
 */
void GcodeSuite::M42() {
  const int pin_index = PARSED_PIN_INDEX('P', GET_PIN_MAP_INDEX(LED_PIN));
  if (pin_index < 0) return;

  const pin_t pin = GET_PIN_MAP_PIN(pin_index);

  if (!parser.boolval('I') && pin_is_protected(pin)) return protected_pin_err();

  if (parser.seenval('M')) {
    switch (parser.value_byte()) {
      case 0: pinMode(pin, INPUT); break;
      case 1: pinMode(pin, OUTPUT); break;
      case 2: pinMode(pin, INPUT_PULLUP); break;
      #ifdef INPUT_PULLDOWN
        case 3: pinMode(pin, INPUT_PULLDOWN); break;
      #endif
      default: SERIAL_ECHOLNPGM("Invalid Pin Mode"); return;
    }
  }

  if (!parser.seenval('S')) return;
  const byte pin_status = parser.value_byte();

  #if HAS_FAN
    switch (pin) {
      #if HAS_FAN0
        case FAN0_PIN: thermalManager.fan_speed[0] = pin_status; return;
      #endif
      #if HAS_FAN1
        case FAN1_PIN: thermalManager.fan_speed[1] = pin_status; return;
      #endif
      #if HAS_FAN2
        case FAN2_PIN: thermalManager.fan_speed[2] = pin_status; return;
      #endif
      #if HAS_FAN3
        case FAN3_PIN: thermalManager.fan_speed[3] = pin_status; return;
      #endif
      #if HAS_FAN4
        case FAN4_PIN: thermalManager.fan_speed[4] = pin_status; return;
      #endif
      #if HAS_FAN5
        case FAN5_PIN: thermalManager.fan_speed[5] = pin_status; return;
      #endif
      #if HAS_FAN6
        case FAN6_PIN: thermalManager.fan_speed[6] = pin_status; return;
      #endif
      #if HAS_FAN7
        case FAN7_PIN: thermalManager.fan_speed[7] = pin_status; return;
      #endif
    }
  #endif

  pinMode(pin, OUTPUT);
  extDigitalWrite(pin, pin_status);
  analogWrite(pin, pin_status);
}
