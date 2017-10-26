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

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"
#include "../../gcode/parser.h"

int16_t GET_PIN_MAP_INDEX(pin_t pin) {
  const uint8_t pin_port = LPC1768_PIN_PORT(pin),
                pin_pin = LPC1768_PIN_PIN(pin);
  for (size_t i = 0; i < NUM_DIGITAL_PINS; ++i)
    if (LPC1768_PIN_PORT(pin_map[i]) == pin_port && LPC1768_PIN_PIN(pin_map[i]) == pin_pin)
      return i;

  return -1;
}

int16_t PARSED_PIN_INDEX(char code, int16_t dval) {
  if (parser.seenval(code)) {
    int port, pin;
    if (sscanf(parser.strval(code), "%d.%d", &port, &pin) == 2)
      for (size_t i = 0; i < NUM_DIGITAL_PINS; ++i)
        if (LPC1768_PIN_PORT(pin_map[i]) == port && LPC1768_PIN_PIN(pin_map[i]) == pin)
          return i;
  }

  return dval;
}

#endif // TARGET_LPC1768