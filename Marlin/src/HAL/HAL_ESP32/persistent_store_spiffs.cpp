/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS) && DISABLED(FLASH_EEPROM_EMULATION)

#include "../shared/persistent_store_api.h"

#include "SPIFFS.h"
#include "FS.h"
#include "spiffs.h"

#define HAL_ESP32_EEPROM_SIZE 4096

File eeprom_file;

bool PersistentStore::access_start() {
  if (spiffs_initialized) {
    eeprom_file = SPIFFS.open("/eeprom.dat", "r+");

    size_t file_size = eeprom_file.size();
    if (file_size < HAL_ESP32_EEPROM_SIZE) {
      bool write_ok = eeprom_file.seek(file_size);

      while (write_ok && file_size < HAL_ESP32_EEPROM_SIZE) {
        write_ok = eeprom_file.write(0xFF) == 1;
        file_size++;
      }
    }
    return true;
  }
  return false;
}

bool PersistentStore::access_finish() {
  eeprom_file.close();
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  if (!eeprom_file.seek(pos)) return true; // return true for any error
  if (eeprom_file.write(value, size) != size) return true;

  crc16(crc, value, size);
  pos += size;

  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  if (!eeprom_file.seek(pos)) return true; // return true for any error

  if (writing) {
    if (eeprom_file.read(value, size) != size) return true;
    crc16(crc, value, size);
  }
  else {
    uint8_t tmp[size];
    if (eeprom_file.read(tmp, size) != size) return true;
    crc16(crc, tmp, size);
  }

  pos += size;

  return false;
}

size_t PersistentStore::capacity() { return HAL_ESP32_EEPROM_SIZE; }

#endif // EEPROM_SETTINGS
#endif // ARDUINO_ARCH_ESP32
