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

#include <SPIFFS.h>
#include <FS.h>
#include "spiffs.h"

#define HAL_ESP32_EEPROM_SIZE 4096
#define HAL_ESP32_EEPROM_FILE_PATH "/eeprom.dat"

File eeprom_file;

bool PersistentStore::access_start() {
  if (spiffs_initialized) {
    eeprom_file = SPIFFS.open(HAL_ESP32_EEPROM_FILE_PATH, "r+");

    size_t file_size = eeprom_file.size();
    if (file_size < HAL_ESP32_EEPROM_SIZE) {
      SERIAL_ECHO_MSG("SPIFFS EEPROM settings file " HAL_ESP32_EEPROM_FILE_PATH " is too small or did not exist, expanding.");
      SERIAL_ECHO_START(); SERIAL_ECHOLNPAIR(" file size: ", file_size, ", required size: ", HAL_ESP32_EEPROM_SIZE);

      // mode r+ does not allow to expand the file (at least on ESP32 SPIFFS9, so we close, reopen "a", append, close, reopen "r+"
      eeprom_file.close();

      eeprom_file = SPIFFS.open(HAL_ESP32_EEPROM_FILE_PATH, "a");
      for (size_t i = eeprom_file.size(); i < HAL_ESP32_EEPROM_SIZE; i++)
        eeprom_file.write(0xFF);
      eeprom_file.close();

      eeprom_file = SPIFFS.open(HAL_ESP32_EEPROM_FILE_PATH, "r+");
      file_size = eeprom_file.size();
      if (file_size < HAL_ESP32_EEPROM_SIZE) {
        SERIAL_ERROR_MSG("Failed to expand " HAL_ESP32_EEPROM_FILE_PATH " to required size. SPIFFS partition full?");
        SERIAL_ERROR_START(); SERIAL_ECHOLNPAIR(" file size: ", file_size, ", required size: ", HAL_ESP32_EEPROM_SIZE);
        SERIAL_ERROR_START(); SERIAL_ECHOLNPAIR(" SPIFFS used bytes: ", SPIFFS.usedBytes(), ", total bytes: ", SPIFFS.totalBytes());
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
