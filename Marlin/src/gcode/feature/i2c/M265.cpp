/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(I2C_SCANNER)

#include "../../../libs/hex_print.h"
#include "../../gcode.h"
#include <Wire.h> // Include Wire library for I2C communication

/**
 * M265: I2C Scanner - Scan for I2C devices on DOGLCD I2C pins
 *
 * Usage: M265
 *
 * Scans I2C addresses 0x08 to 0x77 and reports any responding devices.
 */
void GcodeSuite::M265() {
  Wire.begin();
  int device_count = 0;

  SERIAL_ECHOLNPGM("Scanning I2C (0x08-0x77)...");
  for (uint8_t address = 0x08; address <= 0x77; address++) {
    Wire.beginTransmission(address);
    const uint8_t error = Wire.endTransmission();

    if (error == 0) {
      // Device found
      device_count++;
      SERIAL_ECHOLNPGM("I2C device found at address 0x", hex_byte(address));
    }
    else if (error == 4)
      SERIAL_ECHOLNPGM("Unknown error at address 0x", hex_byte(address));

    safe_delay(5);  // Small delay between scans
  }

  SERIAL_ECHOPGM("I2C scan complete. ");
  if (device_count == 0)
    SERIAL_ECHOLNPGM("No I2C devices found");
  else {
    SERIAL_ECHOLN("Found ", device_count, " device");
    if (device_count > 1) SERIAL_CHAR('s');
    SERIAL_EOL();
  }

}

#endif // I2C_SCANNER
