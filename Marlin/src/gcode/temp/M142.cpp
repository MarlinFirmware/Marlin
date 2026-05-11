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

/**
 * gcode/temp/M142.cpp
 *
 * Bed zone mask selection
 */

#include "../../inc/MarlinConfig.h"

#if HAS_BED_ZONES

#include "../gcode.h"
#include "../../module/temperature.h"

/**
 * M142 - Set or report the active bed zone mask
 *
 * Parameters (mutually exclusive):
 *   K<mask>   - Raw uint16 bitmask. Bit n selects zone n.
 *   A<index>  - Select a single area (zone) by index (0-based). Sets mask = 1 << index.
 *   P<preset> - Select a named preset from BED_ZONE_MASKS by index.
 *
 * With no parameters: report the current mask and matching preset name (if any).
 */
void GcodeSuite::M142() {
  constexpr uint16_t all_zones = (1U << BED_ZONES_COUNT) - 1U;

  if (parser.seenval('K')) {
    thermalManager.bed_zone_mask = (uint16_t)parser.value_ulong() & all_zones;
  }
  else if (parser.seenval('A')) {
    const uint8_t idx = _MIN((uint8_t)parser.value_byte(), (uint8_t)(BED_ZONES_COUNT - 1));
    thermalManager.bed_zone_mask = 1U << idx;
  }
  else if (parser.seenval('P')) {
    #if defined(BED_ZONE_MASK_COUNT) && BED_ZONE_MASK_COUNT > 0
      const uint8_t pidx = _MIN((uint8_t)parser.value_byte(), (uint8_t)(BED_ZONE_MASK_COUNT - 1));
      static constexpr uint16_t _presets[BED_ZONE_MASK_COUNT] = BED_ZONE_MASKS;
      thermalManager.bed_zone_mask = _presets[pidx] & all_zones;
    #else
      SERIAL_ECHOLNPGM("No zone mask presets defined.");
      return;
    #endif
  }
  else {
    // Report current mask
    SERIAL_ECHOPGM("Bed zone mask: ");
    SERIAL_PRINTLN(thermalManager.bed_zone_mask, PrintBase::Bin);
    SERIAL_ECHOPGM(" (");
    SERIAL_PRINT(thermalManager.bed_zone_mask, PrintBase::Dec);
    SERIAL_ECHOPGM(")");
    #if defined(BED_ZONE_MASK_COUNT) && BED_ZONE_MASK_COUNT > 0
    {
      static constexpr uint16_t _presets[BED_ZONE_MASK_COUNT] = BED_ZONE_MASKS;
      for (uint8_t p = 0; p < BED_ZONE_MASK_COUNT; p++) {
        if ((_presets[p] & all_zones) == thermalManager.bed_zone_mask) {
          SERIAL_ECHOPGM(" [P", p, "]");
          break;
        }
      }
    }
    #endif
    SERIAL_EOL();
    return;
  }

  // Echo the new mask after setting
  SERIAL_ECHOPGM("Bed zone mask set: ");
  SERIAL_PRINTLN(thermalManager.bed_zone_mask, PrintBase::Bin);
}

#endif // HAS_BED_ZONES
