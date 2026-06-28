/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(AUTO_FIRST_LAYER_Z_ADJUST)

#include "../gcode.h"
#include "../parser.h"

void say_aflza() {
    const bool active = parser.aflza_active;
    SERIAL_ECHOLN(F("Auto First Layer Z Adjust "), active ? F("A") : F("Dea"), F("ctivated"));
    SERIAL_ECHOLN(F("Configured Layer Height "), parser.calibrated_first_layer_height, F(" mm"));
    SERIAL_ECHOLN(parser.z_hop ? F(" Orca") : F(" Prusa"), F("Slicer"));
}

void GcodeSuite::M429_report(const bool forReplay) {
    SERIAL_ECHOLN(F("Auto First Layer Z Adjust "), parser.aflza_active ? F("A") : F("Dea"), F("ctivated"));
    SERIAL_ECHOLN(F("Configured Layer Height "), parser.calibrated_first_layer_height, F(" mm"));
    SERIAL_ECHOLN(parser.z_hop ? F(" Orca") : F(" Prusa"), F("Slicer"));
}

/**
 * M429: Auto-adjust first layer nozzle height.
 *
 * Parameters:
 *   S<bool>       Activate/Deactivate the Auto-Adjust First Layer Z feature.
 *   H<float>      Set the calibrated first layer height to <val> mm (0.0 .. 1.0)
 *   O<bool>       Set the slicer type : true OrcaSlicer and clones, false PrusaSlicer and clones
 *
 * Examples:
 *   M429 S1       : Activate the Auto First Layer Z Adjust feature
 *   M429 S0       : Deactivate the Auto First Layer Z Adjust feature
 *   M429 H0.3     : Set the calibrated first layer height to <val> mm (0.0 .. 1.0)
 */
void GcodeSuite::M429() {
  if (!parser.seen("SHO")) { say_aflza(); return; }

  // S<bool> Activate/Deactivate
  if (parser.seen('S')) {
    const bool on = parser.value_bool();
    parser.aflza_active = on;
    SERIAL_ECHOLNPGM("Auto First Layer Z Adjust ", on ? F("A") : F("Dea"), "ctivated");
  }

  // H<float> Set Calibrated Height
  if (parser.seen('H')) {
    const float height = parser.value_float();
    if (WITHIN(height, 0.0f, 1.0f)) {
      parser.calibrated_first_layer_height = height;
      SERIAL_ECHOLNPGM("Calibrated First Layer Height set to ", height, " mm");
    }
    else {
      SERIAL_ECHOLNPGM("?Invalid Calibrated First Layer Height [H]. (0.0 .. 1.0 mm)");
    }
  }

  // O<bool> Set Slicer Type
  if (parser.seenval('O')) {
    const slicer_id_t slicer_id = parser.value_ushort(); // 0 Prusa, 1 Orca, ...
    parser.slicer_type = slicer_id;
    parser.z_hop = (slicer_id == SlicerType::ORCA);
    SERIAL_ECHOLN(parser.z_hop ? F(" Orca") : F(" Prusa"), F("Slicer"));
  }
}

#endif // AUTO_FIRST_LAYER_Z_ADJUST
