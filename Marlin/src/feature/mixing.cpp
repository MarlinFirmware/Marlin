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

#include "../inc/MarlinConfig.h"

#if ENABLED(MIXING_EXTRUDER)

#if ENABLED(DIRECT_MIXING_IN_G1)
  #include "../gcode/parser.h"
#endif

float mixing_factor[MIXING_STEPPERS]; // Reciprocal of mix proportion. 0.0 = off, otherwise <= 1.0. (Array must sum to 1.0.)

#if MIXING_VIRTUAL_TOOLS > 1

  float mixing_virtual_tool_mix[MIXING_VIRTUAL_TOOLS][MIXING_STEPPERS];

  void mixing_tools_init() {
    // Virtual Tools 0, 1, 2, 3 = Filament 1, 2, 3, 4, etc.
    for (uint8_t t = 0; t < MIXING_VIRTUAL_TOOLS && t < MIXING_STEPPERS; t++)
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
        mixing_virtual_tool_mix[t][i] = (t == i) ? 1.0 : 0.0;

    // Remaining virtual tools are 100% filament 1
    #if MIXING_STEPPERS < MIXING_VIRTUAL_TOOLS
      for (uint8_t t = MIXING_STEPPERS; t < MIXING_VIRTUAL_TOOLS; t++)
        for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
          mixing_virtual_tool_mix[t][i] = (i == 0) ? 1.0 : 0.0;
    #endif

    // Initialize mixing to tool 0 color
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
      mixing_factor[i] = mixing_virtual_tool_mix[0][i];
  }

#endif // MIXING_VIRTUAL_TOOLS > 1

void normalize_mix() {
  float mix_total = 0.0;
  for (uint8_t i = 0; i < MIXING_STEPPERS; i++) mix_total += mixing_factor[i];
  // Scale all values if they don't add up to ~1.0
  if (!NEAR(mix_total, 1.0)) {
    SERIAL_PROTOCOLLNPGM("Warning: Mix factors must add up to 1.0. Scaling.");
    const float inverse_sum = RECIPROCAL(mix_total);
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++) mixing_factor[i] *= inverse_sum;
  }
}

#if ENABLED(DIRECT_MIXING_IN_G1)
  // Get mixing parameters from the GCode
  // The total "must" be 1.0 (but it will be normalized)
  // If no mix factors are given, the old mix is preserved
  void gcode_get_mix() {
    const char mixing_codes[] = { 'A', 'B'
      #if MIXING_STEPPERS > 2
        , 'C'
        #if MIXING_STEPPERS > 3
          , 'D'
          #if MIXING_STEPPERS > 4
            , 'H'
            #if MIXING_STEPPERS > 5
              , 'I'
            #endif // MIXING_STEPPERS > 5
          #endif // MIXING_STEPPERS > 4
        #endif // MIXING_STEPPERS > 3
      #endif // MIXING_STEPPERS > 2
    };
    byte mix_bits = 0;
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++) {
      if (parser.seenval(mixing_codes[i])) {
        SBI(mix_bits, i);
        mixing_factor[i] = MAX(parser.value_float(), 0.0);
      }
    }
    // If any mixing factors were included, clear the rest
    // If none were included, preserve the last mix
    if (mix_bits) {
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
        if (!TEST(mix_bits, i)) mixing_factor[i] = 0.0;
      normalize_mix();
    }
  }
#endif

#endif // MIXING_EXTRUDER
