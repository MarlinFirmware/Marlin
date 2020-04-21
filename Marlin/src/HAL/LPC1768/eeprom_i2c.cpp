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
 *
 * TODO: Some platform Arduino libraries define these functions
 *       so Marlin needs to add a glue layer to prevent the conflict.
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#if ENABLED(I2C_EEPROM)

#include "../shared/eeprom_api.h"
#include <Wire.h>

#ifndef EEPROM_DEVICE_ADRESS
  #define EEPROM_DEVICE_ADRESS  0x50
#endif

#ifndef EEPROM_SIZE
  #define EEPROM_SIZE           0x8000 // 32kB‬
#endif

#ifndef EEPROM_WRITE_DELAY
  #define EEPROM_WRITE_DELAY    5
#endif

// ------------------------
// Private Variables
// ------------------------
static uint8_t eeprom_device_address = EEPROM_DEVICE_ADRESS;

struct PersistentStore::Helper {

  static void eeprom_init() {
    Wire.begin();
  }

  static void eeprom_write_byte(uint8_t *pos, unsigned char value) {
    unsigned eeprom_address = (unsigned) pos;

    eeprom_init();

    Wire.beginTransmission(I2C_ADDRESS(eeprom_device_address));
    Wire.write((int)(eeprom_address >> 8));   // MSB
    Wire.write((int)(eeprom_address & 0xFF)); // LSB
    Wire.write(value);
    Wire.endTransmission();

    // wait for write cycle to complete
    // this could be done more efficiently with "acknowledge polling"
    delay(EEPROM_WRITE_DELAY);
  }

  static uint8_t eeprom_read_byte(uint8_t *pos) {

    unsigned eeprom_address = (unsigned)pos;
    uint8_t device_address = I2C_ADDRESS(eeprom_device_address);

    eeprom_init();

    Wire.beginTransmission(device_address);
    Wire.write((int)(eeprom_address >> 8));   // MSB
    Wire.write((int)(eeprom_address & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(device_address, (uint8_t)1);

    return Wire.available() ? Wire.read() : 0xFF;
  }
};

bool PersistentStore::access_start() {
  return true;
}

bool PersistentStore::access_finish() {
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    uint8_t v = *value;

    // EEPROM has only ~100,000 write cycles,
    // so only write bytes that have changed!
    uint8_t * const p = (uint8_t * const)pos;
    if (v != Helper::eeprom_read_byte(p)) {

      Helper::eeprom_write_byte(p, v);
      const uint8_t r = Helper::eeprom_read_byte(p);

      if (r != v) {
        SERIAL_ECHO_MSG(STR_ERR_EEPROM_WRITE);
        return true;
      }
    }

    crc16(crc, &v, 1);
    pos++;
    value++;
  };

  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  do {
    // Read from external EEPROM
    const uint8_t c = Helper::eeprom_read_byte((uint8_t*)pos);

    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
  return false;
}

size_t PersistentStore::capacity() {
  return EEPROM_SIZE;
}

#endif // I2C_EEPROM
#endif // TARGET_LPC1768
