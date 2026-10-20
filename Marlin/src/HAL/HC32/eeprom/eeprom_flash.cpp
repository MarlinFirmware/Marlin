/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Implementation of EEPROM settings in the last sector of internal flash
 */
#ifdef ARDUINO_ARCH_HC32

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#include "../../shared/eeprom_api.h"
#include <hc32_ddl.h>

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE 0x1000 // 4K
#endif

#define EEPROM_SECTOR_SIZE 0x2000UL // 8K flash sectors
#define EEPROM_FLASH_ADDR (uint32_t(HC32_FLASH_SIZE) * 1024UL - EEPROM_SECTOR_SIZE) // Last sector of flash

static_assert(MARLIN_EEPROM_SIZE <= EEPROM_SECTOR_SIZE, "MARLIN_EEPROM_SIZE must fit in one 8K flash sector.");
static_assert(MARLIN_EEPROM_SIZE % 4 == 0, "MARLIN_EEPROM_SIZE must be a multiple of 4.");

// Linker symbols locating the end of the firmware image in flash
extern "C" char __etext_ret_ram[], __data_start_ret_ram__[], __data_end_ret_ram__[];

#define _ALIGN(x) __attribute__((aligned(x)))
static uint8_t _ALIGN(4) eeprom_buffer[MARLIN_EEPROM_SIZE];
static bool eeprom_loaded = false, eeprom_data_written = false;

// The firmware must not extend into the EEPROM sector
static bool eeprom_sector_free() {
  const uint32_t image_end = uint32_t(__etext_ret_ram) + uint32_t(__data_end_ret_ram__ - __data_start_ret_ram__);
  return image_end <= EEPROM_FLASH_ADDR;
}

size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE - eeprom_exclude_size; }

bool PersistentStore::access_start() {
  if (!eeprom_sector_free()) {
    SERIAL_ERROR_MSG("Firmware overlaps the EEPROM flash sector.");
    return false;
  }
  if (!eeprom_loaded) {
    memcpy(eeprom_buffer, (const void*)EEPROM_FLASH_ADDR, MARLIN_EEPROM_SIZE);
    eeprom_loaded = true;
  }
  return true;
}

bool PersistentStore::access_finish() {
  if (!eeprom_data_written) return true;

  bool success = true;

  // Code can't be fetched from flash during erase / program
  TERN_(HAS_PAUSE_SERVO_OUTPUT, PAUSE_SERVO_OUTPUT());
  hal.isr_off();

  EFM_Unlock();
  EFM_FlashCmd(Enable);
  while (EFM_GetFlagStatus(EFM_FLAG_RDY) != Set) { /* nada */ }

  if (EFM_SectorErase(EEPROM_FLASH_ADDR) != Ok)
    success = false;
  else {
    const uint32_t *src = (const uint32_t*)eeprom_buffer;
    for (uint32_t addr = EEPROM_FLASH_ADDR; addr < EEPROM_FLASH_ADDR + MARLIN_EEPROM_SIZE; addr += 4, ++src)
      if (EFM_SingleProgram(addr, *src) != Ok) { success = false; break; }
  }

  EFM_Lock();

  hal.isr_on();
  TERN_(HAS_PAUSE_SERVO_OUTPUT, RESUME_SERVO_OUTPUT());

  if (success) eeprom_data_written = false;
  return success;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    const uint8_t v = *value;
    if (v != eeprom_buffer[pos]) {
      eeprom_buffer[pos] = v;
      eeprom_data_written = true;
    }
    crc16(crc, &v, 1);
    pos++;
    value++;
  }
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, const size_t size, uint16_t *crc, const bool writing/*=true*/) {
  for (size_t i = 0; i < size; i++) {
    const uint8_t c = eeprom_buffer[pos + i];
    if (writing) value[i] = c;
    crc16(crc, &c, 1);
  }
  pos += size;
  return false;
}

#endif // FLASH_EEPROM_EMULATION
#endif // ARDUINO_ARCH_HC32
