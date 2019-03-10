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
#ifdef TARGET_LPC1768

/**
 * Emulate EEPROM storage using Flash Memory
 *
 * Use a single 32K flash sector to store EEPROM data. To reduce the
 * number of erase operations a simple "levelling" scheme is used that
 * maintains a number of EEPROM "slots" within the larger flash sector.
 * Each slot is used in turn and the entire sector is only erased when all
 * slots have been used.
 *
 * A simple RAM image is used to hold the EEPROM data during I/O operations
 * and this is flushed to the next available slot when an update is complete.
 * If RAM usage becomes an issue we could store this image in one of the two
 * 16Kb I/O buffers (intended to hold DMA USB and Ethernet data, but currently
 * unused).
 */
#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EEPROM_SETTINGS)

#include "persistent_store_api.h"
#include "../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

extern "C" {
  #include "lpc17xx_iap.h"
}

#define SECTOR_START(sector)  ((sector < 16) ? (sector * 0x1000) : ((sector - 14) * 0x8000))
#define EEPROM_SECTOR 29
#define EEPROM_SIZE (4096)
#define SECTOR_SIZE (32768)
#define EEPROM_SLOTS (SECTOR_SIZE/EEPROM_SIZE)
#define EEPROM_ERASE (0xFF)
#define SLOT_ADDRESS(sector, slot) (((uint8_t *)SECTOR_START(sector)) + slot * EEPROM_SIZE)

static uint8_t ram_eeprom[EEPROM_SIZE] __attribute__((aligned(4))) = {0};
static bool eeprom_dirty = false;
static int current_slot = 0;

bool PersistentStore::access_start() {
  uint32_t first_nblank_loc, first_nblank_val;
  IAP_STATUS_CODE status;

  // discover which slot we are currently using.
  __disable_irq();
  status = BlankCheckSector(EEPROM_SECTOR, EEPROM_SECTOR, &first_nblank_loc, &first_nblank_val);
  __enable_irq();

  if (status == CMD_SUCCESS) {
    // sector is blank so nothing stored yet
    for (int i = 0; i < EEPROM_SIZE; i++) ram_eeprom[i] = EEPROM_ERASE;
    current_slot = EEPROM_SLOTS;
  } else {
    // current slot is the first non blank one
    current_slot = first_nblank_loc / EEPROM_SIZE;
    uint8_t *eeprom_data = SLOT_ADDRESS(EEPROM_SECTOR, current_slot);
    // load current settings
    for (int i = 0; i < EEPROM_SIZE; i++) ram_eeprom[i] = eeprom_data[i];
  }
  eeprom_dirty = false;

  return true;
}

bool PersistentStore::access_finish() {
  if (eeprom_dirty) {
    IAP_STATUS_CODE status;
    if (--current_slot < 0) {
      // all slots have been used, erase everything and start again
      __disable_irq();
      status = EraseSector(EEPROM_SECTOR, EEPROM_SECTOR);
      __enable_irq();

      current_slot = EEPROM_SLOTS - 1;
    }

    __disable_irq();
    status = CopyRAM2Flash(SLOT_ADDRESS(EEPROM_SECTOR, current_slot), ram_eeprom, IAP_WRITE_4096);
    __enable_irq();

    if (status != CMD_SUCCESS) return false;
    eeprom_dirty = false;
  }
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  for (size_t i = 0; i < size; i++) ram_eeprom[pos + i] = value[i];
  eeprom_dirty = true;
  crc16(crc, value, size);
  pos += size;
  return false;  // return true for any error
}

bool PersistentStore::read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  const uint8_t * const buff = writing ? &value[0] : &ram_eeprom[pos];
  if (writing) for (size_t i = 0; i < size; i++) value[i] = ram_eeprom[pos + i];
  crc16(crc, buff, size);
  pos += size;
  return false;  // return true for any error
}

size_t PersistentStore::capacity() { return EEPROM_SIZE; }

#endif // FLASH_EEPROM_EMULATION
#endif // EEPROM_SETTINGS
#endif // TARGET_LPC1768
