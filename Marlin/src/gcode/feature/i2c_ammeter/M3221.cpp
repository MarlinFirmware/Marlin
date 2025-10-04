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

#if ENABLED(I2C_AMMETER_GCODE)

#include "../../../feature/ammeter.h"
#include "../../gcode.h"

/**
 * M3221: read INA3221 or INA226 ammeter
 */
void GcodeSuite::M3221() {
    if (parser.seenval('C')) {
         SERIAL_ECHO(int(ammeter.read(parser.value_byte())),"mA\n"); 
         return;
    }

    if (parser.seen('V'))
    {
        SERIAL_ECHOPGM(" INA3211 @ ");
        SERIAL_PRINT(I2C_AMMETER_ADDRESS, PrintBase::Hex);
        SERIAL_ECHOLNPGM(": ");
        SERIAL_ECHO(int(ammeter.read(1)), "mA @ ", int(ammeter.readVoltage(1)), "mV, 2: ",
                    int(ammeter.read(2)), "mA @ ", int(ammeter.readVoltage(2)), "mV, 3: ",
                    int(ammeter.read(3)), "mA @ ", int(ammeter.readVoltage(3)), "mV\n");
        return;
    }

    SERIAL_ECHO_START();
    switch (ammeter.sensor_type)
    {
    case 0x3220: {
     SERIAL_ECHOPGM(" INA3211 @ ");
        SERIAL_PRINT(I2C_AMMETER_ADDRESS, PrintBase::Hex);
        SERIAL_ECHOPGM(": ");
        SERIAL_ECHO("1: ",int(ammeter.read(1)),"mA, 2: ", int(ammeter.read(2)),"mA, 3: ", int(ammeter.read(3)),"mA\n");
        break;
    }

    case 0x2260: {
         SERIAL_ECHOPGM(" INA226: ");
        SERIAL_ECHO(int(ammeter.read()),"mA\n");
        break;
    }
    default:
        SERIAL_ECHOLNPGM("Unknown sensor");
        break;
    }
      
  
    
}

#endif