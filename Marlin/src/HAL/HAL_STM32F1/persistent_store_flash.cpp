/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * persistent_store_flash.cpp
 * HAL for stm32duino and compatible (STM32F1)
 * Implementation of EEPROM settings in SDCard
 */

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

// This is for EEPROM emulation in flash
#if BOTH(EEPROM_SETTINGS, FLASH_EEPROM_EMULATION)

#include "../shared/persistent_store_api.h"

#include <flash_stm32.h>
#include <EEPROM.h>

// Store settings in the last two pages
// Flash pages must be erased before writing, so keep track.
bool firstWrite = false;
uint32_t pageBase = EEPROM_START_ADDRESS;

bool PersistentStore::access_start() {
  firstWrite = true;
  return true;
}

bool PersistentStore::access_finish() {
  FLASH_Lock();
  firstWrite = false;
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, const size_t size, uint16_t *crc) {
  FLASH_Status status;

  if (firstWrite) {
    FLASH_Unlock();
    status = FLASH_ErasePage(EEPROM_PAGE0_BASE);
    if (status != FLASH_COMPLETE) return true;
    status = FLASH_ErasePage(EEPROM_PAGE1_BASE);
    if (status != FLASH_COMPLETE) return true;
    firstWrite = false;
  }

  // First write full words
  int i = 0;
  int wordsToWrite = size / sizeof(uint16_t);
  uint16_t* wordBuffer = (uint16_t *)value;
  while (wordsToWrite) {
    status = FLASH_ProgramHalfWord(pageBase + pos + (i * 2), wordBuffer[i]);
    if (status != FLASH_COMPLETE) return true;
    wordsToWrite--;
    i++;
  }

  // Now, write any remaining single byte
  if (size & 1) {
    uint16_t temp = value[size - 1];
    status = FLASH_ProgramHalfWord(pageBase + pos + i, temp);
    if (status != FLASH_COMPLETE) return true;
  }

  crc16(crc, value, size);
  pos += ((size + 1) & ~1);
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t* value, const size_t size, uint16_t *crc, const bool writing/*=true*/) {
  for (uint16_t i = 0; i < size; i++) {
    byte* accessPoint = (byte*)(pageBase + pos + i);
    uint8_t c = *accessPoint;
    if (writing) value[i] = c;
    crc16(crc, &c, 1);
  }
  pos += ((size + 1) & ~1);
  return false;
}

size_t PersistentStore::capacity() { return E2END + 1; }

#endif // EEPROM_SETTINGS && EEPROM FLASH
#endif // __STM32F1__
