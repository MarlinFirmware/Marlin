/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS) && EITHER(SPI_EEPROM, I2C_EEPROM)

#include "../shared/persistent_store_api.h"

bool PersistentStore::access_start() { return true; }
bool PersistentStore::access_finish() { return true; }

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    uint8_t * const p = (uint8_t * const)pos;
    uint8_t v = *value;
    // EEPROM has only ~100,000 write cycles,
    // so only write bytes that have changed!
    if (v != eeprom_read_byte(p)) {
      eeprom_write_byte(p, v);
      if (eeprom_read_byte(p) != v) {
        SERIAL_ECHO_MSG(MSG_ERR_EEPROM_WRITE);
        return true;
      }
    }
    crc16(crc, &v, 1);
    pos++;
    value++;
  };
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool set/*=true*/) {
  do {
    uint8_t c = eeprom_read_byte((uint8_t*)pos);
    if (set && value) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
  return false;
}

size_t PersistentStore::capacity() { return E2END + 1; }

#endif // EEPROM_SETTINGS && EITHER(SPI_EEPROM, I2C_EEPROM)
#endif // __STM32F1__
