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
 * Implementation of EEPROM settings in SD Card
 */
#ifdef ARDUINO_ARCH_HC32

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDCARD_EEPROM_EMULATION)

#include "../shared/eeprom_api.h"
#include "../../sd/cardreader.h"

#define EEPROM_FILENAME "eeprom.dat"

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE 0x1000 // 4KB
#endif

size_t PersistentStore::capacity() {
  return MARLIN_EEPROM_SIZE;
}

#define _ALIGN(x) __attribute__((aligned(x)))
static char _ALIGN(4) HAL_eeprom_data[MARLIN_EEPROM_SIZE];

bool PersistentStore::access_start() {
  if (!card.isMounted()) return false;

  MediaFile file, root = card.getroot();
  if (!file.open(&root, EEPROM_FILENAME, O_RDONLY))
    return true; // False aborts the save

  int bytes_read = file.read(HAL_eeprom_data, MARLIN_EEPROM_SIZE);
  if (bytes_read < 0) return false;

  for (; bytes_read < MARLIN_EEPROM_SIZE; bytes_read++)
    HAL_eeprom_data[bytes_read] = 0xFF;

  file.close();
  return true;
}

bool PersistentStore::access_finish() {
  if (!card.isMounted()) return false;

  MediaFile file, root = card.getroot();
  int bytes_written = 0;
  if (file.open(&root, EEPROM_FILENAME, O_CREAT | O_WRITE | O_TRUNC)) {
    bytes_written = file.write(HAL_eeprom_data, MARLIN_EEPROM_SIZE);
    file.close();
  }

  return (bytes_written == MARLIN_EEPROM_SIZE);
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  for (size_t i = 0; i < size; i++) HAL_eeprom_data[pos + i] = value[i];

  crc16(crc, value, size);
  pos += size;
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, const size_t size, uint16_t *crc, const bool writing /*=true*/) {
  for (size_t i = 0; i < size; i++) {
    uint8_t c = HAL_eeprom_data[pos + i];
    if (writing) value[i] = c;
    crc16(crc, &c, 1);
  }

  pos += size;
  return false;
}

#endif // SDCARD_EEPROM_EMULATION
#endif // ARDUINO_ARCH_HC32
