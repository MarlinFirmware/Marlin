/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if HAS_COLOR_LEDS

#include "../../gcode.h"
#include "../../../feature/leds/leds.h"

/**
 * M150: Set Status LED Color - Use R-U-B-W for R-G-B-W
 *       and Brightness       - Use P (for NEOPIXEL only)
 *
 * Always sets all 3 or 4 components. If a component is left out, set to 0.
 *                                    If brightness is left out, no value changed
 *
 * Examples:
 *
 *   M150 R255       ; Turn LED red
 *   M150 R255 U127  ; Turn LED orange (PWM only)
 *   M150            ; Turn LED off
 *   M150 R U B      ; Turn LED white
 *   M150 W          ; Turn LED white using a white LED
 *   M150 P127       ; Set LED 50% brightness
 *   M150 P          ; Set LED full brightness
 */
void GcodeSuite::M150() {
  leds.set_color(MakeLEDColor(
    parser.seen('R') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('U') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('B') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('W') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('P') ? (parser.has_value() ? parser.value_byte() : 255) : neo.brightness()
  ));
}

#endif // HAS_COLOR_LEDS
