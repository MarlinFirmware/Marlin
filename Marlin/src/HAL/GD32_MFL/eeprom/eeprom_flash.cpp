/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2026 Camiel Gubbels / Erik van der Zalm
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
 * Implementation of EEPROM settings in the last two pages of flash bank 0
 */
#ifdef ARDUINO_ARCH_MFL

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#include "../../shared/eeprom_api.h"
#include <FMC.hpp>

#define EEPROM_PAGE_SIZE  0x800UL // Flash bank 0 has 2K pages
#define EEPROM_PAGES      2
#define EEPROM_FLASH_SIZE (EEPROM_PAGE_SIZE * EEPROM_PAGES)

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE EEPROM_FLASH_SIZE // 4K
#endif

static_assert(MARLIN_EEPROM_SIZE <= EEPROM_FLASH_SIZE, "MARLIN_EEPROM_SIZE must fit in two 2K flash pages.");
static_assert(MARLIN_EEPROM_SIZE % 4 == 0, "MARLIN_EEPROM_SIZE must be a multiple of 4.");

// Last two pages of bank 0, which is the first 512K of flash (or all of it, if smaller)
static uint32_t eeprom_flash_addr() {
  const uint32_t flash_kb = *reinterpret_cast<const uint16_t*>(fmc::Flash_Size_Addess);
  return 0x08000000UL + _MIN(flash_kb, 512UL) * 1024UL - EEPROM_FLASH_SIZE;
}

// Linker symbols locating the end of the firmware image in flash
extern "C" char _sidata[], _sdata[], _edata[];

// The firmware must not extend into the EEPROM pages
static bool eeprom_pages_free() {
  const uint32_t image_end = uint32_t(_sidata) + uint32_t(_edata - _sdata);
  return image_end <= eeprom_flash_addr();
}

#define _ALIGN(x) __attribute__((aligned(x)))
static uint8_t _ALIGN(4) eeprom_buffer[MARLIN_EEPROM_SIZE];
static bool eeprom_loaded = false, eeprom_data_written = false;

size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE - eeprom_exclude_size; }

bool PersistentStore::access_start() {
  if (!eeprom_pages_free()) {
    SERIAL_ERROR_MSG("Firmware overlaps the EEPROM flash pages.");
    return false;
  }
  if (!eeprom_loaded) {
    memcpy(eeprom_buffer, reinterpret_cast<const void*>(eeprom_flash_addr()), MARLIN_EEPROM_SIZE);
    eeprom_loaded = true;
  }
  return true;
}

bool PersistentStore::access_finish() {
  if (!eeprom_data_written) return true;

  const uint32_t addr = eeprom_flash_addr();
  auto &fmc = fmc::FMC::get_instance();
  bool success = true;

  // Code can't be fetched from flash bank 0 while it is erased / programmed
  hal.isr_off();

  fmc.unlock();
  fmc.clear_flag(fmc::Status_Flags::FLAG_BANK0_END);
  fmc.clear_flag(fmc::Status_Flags::FLAG_BANK0_WPERR);
  fmc.clear_flag(fmc::Status_Flags::FLAG_BANK0_PGERR);

  for (uint8_t p = 0; p < EEPROM_PAGES && success; ++p)
    if (fmc.erase_page(addr + p * EEPROM_PAGE_SIZE) != fmc::FMC_Error_Type::READY) success = false;

  if (success) {
    const uint32_t *src = reinterpret_cast<const uint32_t*>(eeprom_buffer);
    for (uint32_t a = addr; a < addr + MARLIN_EEPROM_SIZE; a += 4, ++src)
      if (fmc.program_word(a, *src) != fmc::FMC_Error_Type::READY) { success = false; break; }
  }

  fmc.lock();

  hal.isr_on();

  if (success) eeprom_data_written = false;
  return success;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  for (size_t i = 0; i < size; ++i) eeprom_buffer[pos + i] = value[i];
  eeprom_data_written = true;
  crc16(crc, value, size);
  pos += size;
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, const size_t size, uint16_t *crc, const bool writing/*=true*/) {
  for (size_t i = 0; i < size; ++i) {
    const uint8_t c = eeprom_buffer[pos + i];
    if (writing) value[i] = c;
    crc16(crc, &c, 1);
  }
  pos += size;
  return false;
}

#endif // FLASH_EEPROM_EMULATION
#endif // ARDUINO_ARCH_MFL
