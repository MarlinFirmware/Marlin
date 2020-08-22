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

#include "../../../inc/MarlinConfig.h"

#if NEOPIXELX2 

#include "../../gcode.h"
#include "../../../feature/leds/leds.h"

/**
 * M151: Set Second LED Color - Use R-U-B-W for R-G-B-W
 *       and Brightness       - Use P (for NEOPIXEL only)
 *
 * Always sets all 3 or 4 components. If a component is left out, set to 0.
 *                                    If brightness is left out, no value changed
 *
 * With NEOPIXEL2:
 *  I<index>  Set the Neopixel2 index to affect. Default: All
 *
 * Examples:
 *
 *   M151 R255       ; Turn LED red
 *   M151 R255 U127  ; Turn LED orange (PWM only)
 *   M151            ; Turn LED off
 *   M151 R U B      ; Turn LED white
 *   M151 W          ; Turn LED white using a white LED
 *   M151 P127       ; Set LED 50% brightness
 *   M151 P          ; Set LED full brightness
 *   M151 I1 R       ; Set NEOPIXEL2 index 1 to red
 */
void GcodeSuite::M151() {
  #if ENABLED(NEOPIXEL2)
    neo2.set_neo_index(parser.intval('I', -1));
  #endif
  leds2.set_color(MakeLEDColor2(
    parser.seen('R') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('U') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('B') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('W') ? (parser.has_value() ? parser.value_byte() : 255) : 0,
    parser.seen('P') ? (parser.has_value() ? parser.value_byte() : 255) : neo2.brightness()
  ));
}

#endif // HAS_COLOR_LEDS
