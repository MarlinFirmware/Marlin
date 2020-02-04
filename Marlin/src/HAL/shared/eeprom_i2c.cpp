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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Description: functions for I2C connected external EEPROM.
 * Not platform dependent.
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(I2C_EEPROM)

#include "../HAL.h"
#include <Wire.h>

// ------------------------
// Private Variables
// ------------------------

static uint8_t eeprom_device_address = 0x50;

// ------------------------
// Public functions
// ------------------------

static void eeprom_init() {
  Wire.begin();
}

void eeprom_write_byte(uint8_t *pos, unsigned char value) {
  unsigned eeprom_address = (unsigned) pos;

  eeprom_init();

  Wire.beginTransmission(I2C_ADDRESS(eeprom_device_address));
  Wire.write((int)(eeprom_address >> 8));   // MSB
  Wire.write((int)(eeprom_address & 0xFF)); // LSB
  Wire.write(value);
  Wire.endTransmission();

  // wait for write cycle to complete
  // this could be done more efficiently with "acknowledge polling"
  delay(5);
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void eeprom_update_block(const void *pos, void* eeprom_address, size_t n) {
  eeprom_init();

  Wire.beginTransmission(I2C_ADDRESS(eeprom_device_address));
  Wire.write((int)((unsigned)eeprom_address >> 8));   // MSB
  Wire.write((int)((unsigned)eeprom_address & 0xFF)); // LSB
  Wire.endTransmission();

  uint8_t *ptr = (uint8_t*)pos;
  uint8_t flag = 0;
  Wire.requestFrom(eeprom_device_address, (byte)n);
  for (byte c = 0; c < n && Wire.available(); c++)
    flag |= Wire.read() ^ ptr[c];

  if (flag) {
    Wire.beginTransmission(I2C_ADDRESS(eeprom_device_address));
    Wire.write((int)((unsigned)eeprom_address >> 8));   // MSB
    Wire.write((int)((unsigned)eeprom_address & 0xFF)); // LSB
    Wire.write((uint8_t*)pos, n);
    Wire.endTransmission();

    // wait for write cycle to complete
    // this could be done more efficiently with "acknowledge polling"
    delay(5);
  }
}

uint8_t eeprom_read_byte(uint8_t *pos) {
  unsigned eeprom_address = (unsigned)pos;

  eeprom_init();

  Wire.beginTransmission(I2C_ADDRESS(eeprom_device_address));
  Wire.write((int)(eeprom_address >> 8));   // MSB
  Wire.write((int)(eeprom_address & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(eeprom_device_address, (byte)1);
  return Wire.available() ? Wire.read() : 0xFF;
}

// Don't read more than 30..32 bytes at a time!
void eeprom_read_block(void* pos, const void* eeprom_address, size_t n) {
  eeprom_init();

  Wire.beginTransmission(I2C_ADDRESS(eeprom_device_address));
  Wire.write((int)((unsigned)eeprom_address >> 8));   // MSB
  Wire.write((int)((unsigned)eeprom_address & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(eeprom_device_address, (byte)n);
  for (byte c = 0; c < n; c++ )
    if (Wire.available()) *((uint8_t*)pos + c) = Wire.read();
}

#endif // I2C_EEPROM
