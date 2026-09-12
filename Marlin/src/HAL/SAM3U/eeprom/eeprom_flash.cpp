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

/**
 * EEPROM emulation in the SAM3U's embedded flash.
 *
 * The SAM3U4E has no EEPROM, so persistent settings live in a reserved region
 * at the very top of flash. buildroot/share/PlatformIO/variants/SAM3U/
 * sam3u4e_marlin.ld shortens the rom region by MARLIN_EEPROM_SIZE and exports
 * _seeprom, so the linker can never place code there.
 *
 * Reads come straight out of the memory-mapped flash. Writes go through a
 * single 256-byte page cache - one page, not a full shadow of the region,
 * because RAM is the scarce resource here (48KB total). Marlin writes its
 * settings sequentially, so one cached page is enough to avoid thrashing.
 *
 * Programming detail: while a flash page is being written the controller
 * cannot serve instruction fetches, so the routine that issues the command
 * lives in RAM (.ramfunc, copied there by the CMSIS startup code) and runs
 * with interrupts masked. Both flash planes are covered - the EEPROM region
 * sits in bank 1, which EEFC1 controls.
 */

#ifdef __SAM3U4E__

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#include "../../shared/eeprom_api.h"
#include "../clock.h"

// Flash geometry (datasheet Table 8-2): 2 banks x 512 pages x 256 bytes
#define FLASH_PAGE_SIZE   256U
#define FLASH_BANK_SIZE   0x20000U            // 128KB per bank
#define FLASH_BASE_ADDR   0x00080000U
#define FLASH_BANK1_ADDR  (FLASH_BASE_ADDR + FLASH_BANK_SIZE)

#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE 0x1000U          // 4KB = 16 flash pages
#endif

// Start of the reserved region, exported by the linker script
extern "C" uint32_t _seeprom;
#define EEPROM_BASE_ADDR ((uint32_t)&_seeprom)

// EEFC command key
#define EEFC_FKEY 0x5AU

static uint8_t  page_buffer[FLASH_PAGE_SIZE];
static uint32_t cached_page = 0xFFFFFFFF;     // Flash address of the cached page
static bool     page_dirty = false;

/**
 * Issue an "erase page and write page" for one page. Runs from RAM: the flash
 * cannot be read while it is being programmed.
 *
 * addr must be page-aligned and lie in the EEPROM region (bank 1).
 */
__attribute__((section(".ramfunc"), noinline))
static void flash_write_page(const uint32_t addr, const uint8_t * const data) {

  // Fill the controller's latch buffer by writing through the flash mapping.
  // The latch takes 32-bit writes only.
  volatile uint32_t *dst = (volatile uint32_t *)addr;
  const uint32_t *src = (const uint32_t *)data;
  for (uint32_t i = 0; i < FLASH_PAGE_SIZE / 4; i++) dst[i] = src[i];

  // Page number within its bank, and the controller that owns that bank
  Efc * const efc = addr >= FLASH_BANK1_ADDR ? EFC1 : EFC0;
  const uint32_t page = ((addr - (addr >= FLASH_BANK1_ADDR ? FLASH_BANK1_ADDR : FLASH_BASE_ADDR)) / FLASH_PAGE_SIZE);

  efc->EEFC_FCR = EEFC_FCR_FKEY(EEFC_FKEY) | EEFC_FCR_FARG(page) | EEFC_FCR_FCMD_EWP;

  while (!(efc->EEFC_FSR & EEFC_FSR_FRDY)) { /* programming */ }
}

// Flush the cached page back to flash, if it was modified
static void flush_page() {
  if (!page_dirty || cached_page == 0xFFFFFFFF) return;

  const bool irq_was_on = !__get_PRIMASK();
  __disable_irq();

  flash_write_page(cached_page, page_buffer);

  if (irq_was_on) __enable_irq();

  page_dirty = false;
}

// Make sure the page containing addr is the cached one
static void load_page(const uint32_t addr) {
  const uint32_t page_addr = addr & ~(FLASH_PAGE_SIZE - 1);
  if (page_addr == cached_page) return;

  flush_page();
  memcpy(page_buffer, (const void *)page_addr, FLASH_PAGE_SIZE);
  cached_page = page_addr;
}

// ------------------------
// PersistentStore
// ------------------------

size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE; }

bool PersistentStore::access_start() {
  cached_page = 0xFFFFFFFF;
  page_dirty = false;
  return true;
}

bool PersistentStore::access_finish() {
  flush_page();
  cached_page = 0xFFFFFFFF;
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    const uint8_t v = *value;
    const uint32_t addr = EEPROM_BASE_ADDR + pos;

    load_page(addr);

    uint8_t &slot = page_buffer[addr & (FLASH_PAGE_SIZE - 1)];
    if (slot != v) { slot = v; page_dirty = true; }

    crc16(crc, &v, 1);
    pos++;
    value++;
  }
  return false; // Success
}

bool PersistentStore::read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  while (size--) {
    const uint32_t addr = EEPROM_BASE_ADDR + pos;

    // Serve from the cache if this page has unflushed changes, else from flash
    const uint8_t c = (page_dirty && (addr & ~(FLASH_PAGE_SIZE - 1)) == cached_page)
                    ? page_buffer[addr & (FLASH_PAGE_SIZE - 1)]
                    : *(const uint8_t *)addr;

    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  }
  return false; // Success
}

#endif // FLASH_EEPROM_EMULATION
#endif // __SAM3U4E__
