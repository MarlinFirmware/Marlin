/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#if defined(STM32GENERIC) && (defined(STM32F4) || defined(STM32F7))

#include "../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#include "../shared/eeprom_api.h"
#include "eeprom_emul.h"

// FLASH_FLAG_PGSERR (Programming Sequence Error) was renamed to
// FLASH_FLAG_ERSERR (Erasing Sequence Error) in STM32F4/7

#ifdef STM32F7
  #define FLASH_FLAG_PGSERR FLASH_FLAG_ERSERR
#else
  //#define FLASH_FLAG_PGSERR FLASH_FLAG_ERSERR
#endif

void ee_write_byte(uint8_t *pos, unsigned char value) {
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

  const unsigned eeprom_address = (unsigned)pos;
  if (EE_WriteVariable(eeprom_address, uint16_t(value)) != EE_OK)
    for (;;) HAL_Delay(1); // Spin forever until watchdog reset

  HAL_FLASH_Lock();
}

uint8_t ee_read_byte(uint8_t *pos) {
  uint16_t data = 0xFF;
  const unsigned eeprom_address = (unsigned)pos;
  (void)EE_ReadVariable(eeprom_address, &data); // Data unchanged on error
  return uint8_t(data);
}

#ifndef MARLIN_EEPROM_SIZE
  #error "MARLIN_EEPROM_SIZE is required for Flash-based EEPROM."
#endif
size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE; }

bool PersistentStore::access_finish() { return true; }

bool PersistentStore::access_start() {
  static bool ee_initialized = false;
  if (!ee_initialized) {
    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    /* EEPROM Init */
    if (EE_Initialize() != EE_OK)
      for (;;) HAL_Delay(1); // Spin forever until watchdog reset

    HAL_FLASH_Lock();
    ee_initialized = true;
  }
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    uint8_t * const p = (uint8_t * const)pos;
    uint8_t v = *value;
    // EEPROM has only ~100,000 write cycles,
    // so only write bytes that have changed!
    if (v != ee_read_byte(p)) {
      ee_write_byte(p, v);
      if (ee_read_byte(p) != v) {
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
    uint8_t c = ee_read_byte((uint8_t*)pos);
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
  return false;
}

#endif // FLASH_EEPROM_EMULATION
#endif // STM32GENERIC && (STM32F4 || STM32F7)
