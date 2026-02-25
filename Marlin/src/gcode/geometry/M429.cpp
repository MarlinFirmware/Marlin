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

#if ENABLED(ADAPTIVE_FL_Z_OFFSET)

#include "../gcode.h"
#include "../parser.h"

/**
 * M429: Configure and run the resonance test.
 *
 * Parameters:
 *   S<bool>       Activate/Deactivate the Auto First Layer Z Offset feature.
 *   H<float>      Set the calibrated first layer height to <val> mm (0.0 .. 1.0)
 *
 * Examples:
 *   M429 S1       : Activate the Auto First Layer Z Offset feature
 *   M429 S0       : Deactivate the Auto First Layer Z Offset feature
 *   M429 H0.3     : Set the calibrated first layer height to <val> mm (0.0 .. 1.0)
 *
 */

void GcodeSuite::M429() {
    if (parser.seen('S')) {
        const bool val = parser.value_bool();
        parser.adaptive_flzo_active = val;
        SERIAL_ECHOLN(F("Auto First Layer Z Offset "), val ? F("A") : F("Dea"), F("ctivated"));
    }
    if (parser.seenval('H')) {
        const float val = parser.value_float();
        if WITHIN(val, 0.0f, 1.0f) {
            parser.calibrated_first_layer_height = val;
            SERIAL_ECHOLN(F("Calibrated First Layer Height set to "), parser.calibrated_first_layer_height, F(" mm"));
        }
        else {
            SERIAL_ECHOLN(F("?Invalid Calibrated First Layer Height [H]. (0.0 .. 1.0 mm)"));
        }
   }
}
#endif