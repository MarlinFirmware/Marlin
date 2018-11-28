/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2016 Victor Perez victor_pv@hotmail.com
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
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS) && DISABLED(FLASH_EEPROM_EMULATION)

#include "../shared/persistent_store_api.h"

#include "../../sd/cardreader.h"

#define HAL_STM32F1_EEPROM_SIZE 4096
char HAL_STM32F1_eeprom_content[HAL_STM32F1_EEPROM_SIZE];

char eeprom_filename[] = "eeprom.dat";

bool PersistentStore::access_start() {
  if (!card.flag.cardOK) return false;
  int16_t bytes_read = 0;
  constexpr char eeprom_zero = 0xFF;
  card.openFile(eeprom_filename, true);
  bytes_read = card.read(HAL_STM32F1_eeprom_content, HAL_STM32F1_EEPROM_SIZE);
  if (bytes_read < 0) return false;
  for (; bytes_read < HAL_STM32F1_EEPROM_SIZE; bytes_read++)
    HAL_STM32F1_eeprom_content[bytes_read] = eeprom_zero;
  card.closefile();
  return true;
}

bool PersistentStore::access_finish() {
  if (!card.flag.cardOK) return false;
  card.openFile(eeprom_filename, true);
  int16_t bytes_written = card.write(HAL_STM32F1_eeprom_content, HAL_STM32F1_EEPROM_SIZE);
  card.closefile();
  return (bytes_written == HAL_STM32F1_EEPROM_SIZE);
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, const size_t size, uint16_t *crc) {
  for (int i = 0; i < size; i++)
    HAL_STM32F1_eeprom_content[pos + i] = value[i];
  crc16(crc, value, size);
  pos += size;
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t* value, const size_t size, uint16_t *crc, const bool writing/*=true*/) {
  for (int i = 0; i < size; i++) {
    uint8_t c = HAL_STM32F1_eeprom_content[pos + i];
    if (writing) value[i] = c;
    crc16(crc, &c, 1);
  }
  pos += size;
  return false;
}

size_t PersistentStore::capacity() { return HAL_STM32F1_EEPROM_SIZE; }

#endif // EEPROM_SETTINGS

#endif // __STM32F1__
