/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * PersistentStore for Arduino-style EEPROM interface
 * with simple implementations supplied by Marlin.
 */
#ifdef ARDUINO_ARCH_HC32

#include "../../inc/MarlinConfig.h"

#if ENABLED(IIC_BL24CXX_EEPROM)

#include "../shared/eeprom_api.h"
#include "../shared/eeprom_if.h"

#ifndef MARLIN_EEPROM_SIZE
  #error "MARLIN_EEPROM_SIZE is required for IIC_BL24CXX_EEPROM."
#endif

size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE - eeprom_exclude_size; }

bool PersistentStore::access_start() {
  eeprom_init();
  return true;
}

bool PersistentStore::access_finish() { return true; }

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    uint8_t v = *value;
    uint8_t * const p = (uint8_t * const)REAL_EEPROM_ADDR(pos);

    // EEPROM has only ~100,000 write cycles,
    // so only write bytes that have changed!
    if (v != eeprom_read_byte(p)) {
      eeprom_write_byte(p, v);
      delay(2);
      if (eeprom_read_byte(p) != v) {
        SERIAL_ECHO_MSG(STR_ERR_EEPROM_WRITE);
        return true;
      }
    }

    crc16(crc, &v, 1);
    pos++;
    value++;
  }

  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing /*=true*/) {
  do {
    const uint8_t c = eeprom_read_byte((uint8_t*)REAL_EEPROM_ADDR(pos));
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);

  return false;
}

#endif // IIC_BL24CXX_EEPROM
#endif // ARDUINO_ARCH_HC32
