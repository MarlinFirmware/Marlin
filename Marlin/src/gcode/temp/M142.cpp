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
 * The active zones are used by M140 / M190 without A or K, by preheat, and for the whole-bed temperature.
 *
 * Parameters (use one):
 *   K<mask>   - Bitmask. Bit n selects zone n.
 *   A<index>  - Select a single zone by index (0-based).
 *   P<preset> - Select a preset from BED_ZONE_MASKS by index.
 *
 * With no parameters report the current mask.
 */
void GcodeSuite::M142() {
  if (!parser.seen_any()) return M142_report();

  constexpr uint16_t all_zones = (1U << BED_ZONES_COUNT) - 1U;
  uint16_t mask = 0;

  if (parser.seenval('K'))
    mask = uint16_t(parser.value_ulong()) & all_zones;
  else if (parser.seenval('A')) {
    const uint8_t z = parser.value_byte();
    if (z >= BED_ZONES_COUNT) { SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No bed zone A", z)); return; }
    mask = _BV(z);
  }
  else if (parser.seenval('P')) {
    #if BED_ZONE_MASK_COUNT
      constexpr uint16_t presets[BED_ZONE_MASK_COUNT] = BED_ZONE_MASKS;
      const uint8_t p = parser.value_byte();
      if (p >= BED_ZONE_MASK_COUNT) { SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No bed zone preset P", p)); return; }
      mask = presets[p] & all_zones;
    #else
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No bed zone presets"));
      return;
    #endif
  }

  if (!mask) { SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No bed zones selected")); return; }
  thermalManager.bed_zone_mask = mask;
}

void GcodeSuite::M142_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F("Active bed zones (bit n = zone n)"));
  SERIAL_ECHOLNPGM("  M142 K", thermalManager.bed_zone_mask);
}

#endif // HAS_BED_ZONES
