/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../platforms.h"

#ifdef __PLAT_RP2040__

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#include "../../shared/eeprom_api.h"

// NOTE: The Bigtreetech SKR Pico has an onboard W25Q16 flash module

// RP2040 Flash-based EEPROM emulation using internal flash memory
#include <hardware/flash.h>
#include <hardware/sync.h>

// Flash sector size is already defined in hardware/flash.h as FLASH_SECTOR_SIZE
// Place EEPROM emulation at the end of flash, before the filesystem
// This assumes 2MB flash, adjust if using different flash size
#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE size_t(E2END + 1)
#endif

static uint8_t eeprom_buffer[MARLIN_EEPROM_SIZE];
static bool eeprom_data_written = false;
static bool eeprom_initialized = false;
size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE; }

bool PersistentStore::access_start() {
  if (!eeprom_initialized) {
    // Read from flash into buffer
    const uint8_t *flash_data = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
    memcpy(eeprom_buffer, flash_data, MARLIN_EEPROM_SIZE);
    eeprom_initialized = true;
  }
  return true;
}

bool PersistentStore::access_finish() {
  if (eeprom_data_written) {
    TERN_(HAS_PAUSE_SERVO_OUTPUT, PAUSE_SERVO_OUTPUT());

    // Disable interrupts during flash write
    const uint32_t intstate = save_and_disable_interrupts();

    // Erase and program the sector
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, eeprom_buffer, MARLIN_EEPROM_SIZE);

    // Restore interrupts
    restore_interrupts(intstate);

    TERN_(HAS_PAUSE_SERVO_OUTPUT, RESUME_SERVO_OUTPUT());
    eeprom_data_written = false;
  }
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    uint8_t v = *value;
    if (pos < (int)MARLIN_EEPROM_SIZE && v != eeprom_buffer[pos]) {
      eeprom_buffer[pos] = v;
      eeprom_data_written = true;
    }
    crc16(crc, &v, 1);
    pos++;
    value++;
  }
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  do {
    const uint8_t c = (pos < (int)MARLIN_EEPROM_SIZE) ? eeprom_buffer[pos] : 0xFF;
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
  return false;
}

#endif // FLASH_EEPROM_EMULATION
#endif // __PLAT_RP2040__
