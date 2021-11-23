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
void GcodeSuite::M3426()
{
  // Set the target address
  uint8_t channel = 1;
  uint8_t gain = 1;

  bool inverted = false;

  // Select the channel 1 or 2
  if (parser.seen('C'))
  {
    channel = parser.value_byte();
  }

  if (parser.seen('G'))
  {
    gain = parser.value_byte();
  }

  if (parser.seen('I'))
  {
    if (parser.value_byte() == 1)
    {
      inverted = true;
    }
  }

  if ((channel <= 2) && ((gain == 1) || (gain == 2) || (gain == 4) || (gain == 8)))
  {
    int16_t result = mcp3426.ReadValue(channel, gain);

    if (mcp3426.Error == false)
    {

      if (inverted)
      {
        //Should we invert the reading (32767 - ADC value) ?
        //Caters for end devices which expect values to increase when in reality then decrease
        //For instance pressure sensor in a vacuum when the end device expects a positive pressure
        result = INT16_MAX - result;
      }

      SERIAL_ECHOPGM(STR_OK);
      SERIAL_ECHOPGM(" V:");
      SERIAL_ECHO(result);
      SERIAL_ECHOPGM(" C:");
      SERIAL_ECHO(channel);
      SERIAL_ECHOPGM(" G:");
      SERIAL_ECHO(gain);
      SERIAL_ECHOPGM(" I:");
      SERIAL_ECHO(inverted ? 1 : 0);
      SERIAL_EOL();
    }
    else
    {
      SERIAL_ERROR_MSG("MCP342X i2c error");
    }
  }
  else
  {
    SERIAL_ERROR_MSG("MCP342X Bad request");
  }
}

#endif
