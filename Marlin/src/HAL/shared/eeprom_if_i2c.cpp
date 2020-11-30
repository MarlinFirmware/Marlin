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
 * Platform-independent Arduino functions for I2C EEPROM.
 * Enable USE_SHARED_EEPROM if not supplied by the framework.
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(I2C_EEPROM)

#include "eeprom_if.h"
#include <Wire.h>

void eeprom_init() { Wire.begin(); }

#if ENABLED(USE_SHARED_EEPROM)

#ifndef EEPROM_WRITE_DELAY
  #define EEPROM_WRITE_DELAY    5
#endif
#ifndef EEPROM_DEVICE_ADDRESS
  #define EEPROM_DEVICE_ADDRESS  0x50
#endif

static constexpr uint8_t eeprom_device_address = I2C_ADDRESS(EEPROM_DEVICE_ADDRESS);

// ------------------------
// Public functions
// ------------------------

void eeprom_write_byte(uint8_t *pos, unsigned char value) {
  const unsigned eeprom_address = (unsigned)pos;

  Wire.beginTransmission(eeprom_device_address);
  Wire.write(int(eeprom_address >> 8));   // MSB
  Wire.write(int(eeprom_address & 0xFF)); // LSB
  Wire.write(value);
  Wire.endTransmission();

  // wait for write cycle to complete
  // this could be done more efficiently with "acknowledge polling"
  delay(EEPROM_WRITE_DELAY);
}

uint8_t eeprom_read_byte(uint8_t *pos) {
  const unsigned eeprom_address = (unsigned)pos;

  Wire.beginTransmission(eeprom_device_address);
  Wire.write(int(eeprom_address >> 8));   // MSB
  Wire.write(int(eeprom_address & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(eeprom_device_address, (byte)1);
  return Wire.available() ? Wire.read() : 0xFF;
}

#endif // USE_SHARED_EEPROM
#endif // I2C_EEPROM
