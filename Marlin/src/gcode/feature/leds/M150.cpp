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
 * With NEOPIXEL_LED:
 *  I<index>  Set the NeoPixel index to affect. Default: All
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
 *   M150 I1 R       ; Set NEOPIXEL index 1 to red
 */
void GcodeSuite::M150() {
  #if ENABLED(NEOPIXEL_LED)
    neo.set_neo_index(parser.intval('I', -1));
  #endif
  leds.set_color(MakeLEDColor(
    parser.seen('R') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('U') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('B') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('W') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('P') ? (parser.has_value() ? parser.value_byte() : 255) : neo.brightness()
  ));
}

#endif // HAS_COLOR_LEDS
