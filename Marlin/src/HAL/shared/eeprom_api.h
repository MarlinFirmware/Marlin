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
#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../../libs/crc16.h"

// For testing. Define with -DEEPROM_EXCL_ZONE=919,926 in INI files.
//#define EEPROM_EXCL_ZONE 919,926  // Test a range
//#define EEPROM_EXCL_ZONE 333      // Test a single byte

#ifdef EEPROM_EXCL_ZONE
  static constexpr int eeprom_exclude_zone[] = { EEPROM_EXCL_ZONE },
                       eeprom_exclude_size = eeprom_exclude_zone[COUNT(eeprom_exclude_zone) - 1] - eeprom_exclude_zone[0] + 1;
  #define REAL_EEPROM_ADDR(A) (A < eeprom_exclude_zone[0] ? (A) : (A) + eeprom_exclude_size)
#else
  #define REAL_EEPROM_ADDR(A) (A)
  static constexpr int eeprom_exclude_size = 0;
#endif

class PersistentStore {
public:

  // Total available persistent storage space (in bytes)
  static size_t capacity();

  // Prepare to read or write
  static bool access_start();

  // Housecleaning after read or write
  static bool access_finish();

  // Write one or more bytes of data and update the CRC
  // Return 'true' on write error
  static bool write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc);

  // Read one or more bytes of data and update the CRC
  // Return 'true' on read error
  static bool read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing=true);

  // Write one or more bytes of data
  // Return 'true' on write error
  static bool write_data(const int pos, const uint8_t *value, const size_t size=sizeof(uint8_t)) {
    int data_pos = pos;
    uint16_t crc = 0;
    return write_data(data_pos, value, size, &crc);
  }

  // Write a single byte of data
  // Return 'true' on write error
  static bool write_data(const int pos, const uint8_t value) { return write_data(pos, &value); }

  // Read one or more bytes of data
  // Return 'true' on read error
  static bool read_data(const int pos, uint8_t *value, const size_t size=1) {
    int data_pos = pos;
    uint16_t crc = 0;
    return read_data(data_pos, value, size, &crc);
  }
};

extern PersistentStore persistentStore;
