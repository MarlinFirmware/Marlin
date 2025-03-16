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
#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)

#include "../shared/eeprom_api.h"
#include <EEPROM.h>

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE 0x1000 // 4KB
#endif
size_t PersistentStore::capacity()    { return MARLIN_EEPROM_SIZE - eeprom_exclude_size; }

bool PersistentStore::access_start()  { return EEPROM.begin(MARLIN_EEPROM_SIZE); }
bool PersistentStore::access_finish() { EEPROM.end(); return true; }

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  for (size_t i = 0; i < size; i++) {
    const int p = REAL_EEPROM_ADDR(pos);
    EEPROM.write(p, value[i]);
    crc16(crc, &value[i], 1);
    ++pos;
  }
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  for (size_t i = 0; i < size; i++) {
    const int p = REAL_EEPROM_ADDR(pos);
    uint8_t c = EEPROM.read(p);
    if (writing) value[i] = c;
    crc16(crc, &c, 1);
    ++pos;
  }
  return false;
}

#endif // EEPROM_SETTINGS
#endif // ARDUINO_ARCH_ESP32
