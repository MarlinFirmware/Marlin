/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(HAS_MCP3426_ADC)

#include "../../gcode.h"

#include "../../../feature/adc/adc_mcp3426.h"

/**
 * M3426: Read 16 bit (signed) value from I2C MCP3426 ADC device
 *
 *  M3426 C<byte-1 value in base 10> channel 1 or 2
 *  M3426 G<byte-1 value in base 10> gain 1, 2, 4 or 8
 *  M3426 I<byte-2 value in base 10> 0 or 1, invert reply
 */
void GcodeSuite::M3426() {
  uint8_t channel = parser.byteval('C', 1),       // Select the channel 1 or 2
             gain = parser.byteval('G', 1);
  const bool inverted = parser.byteval('I') == 1;

  if (channel <= 2 && (gain == 1 || gain == 2 || gain == 4 || gain == 8)) {
    int16_t result = mcp3426.ReadValue(channel, gain);

    if (mcp3426.Error == false) {
      if (inverted) {
        // Should we invert the reading (32767 - ADC value) ?
        // Caters to end devices that expect values to increase when in reality they decrease.
        // e.g., A pressure sensor in a vacuum when the end device expects a positive pressure.
        result = INT16_MAX - result;
      }
      //SERIAL_ECHOPGM(STR_OK);
      SERIAL_ECHOLNPGM("V:", result, " C:", channel, " G:", gain, " I:", inverted ? 1 : 0);
    }
    else
      SERIAL_ERROR_MSG("MCP342X i2c error");
  }
  else
    SERIAL_ERROR_MSG("MCP342X Bad request");
}

#endif // HAS_MCP3426_ADC
