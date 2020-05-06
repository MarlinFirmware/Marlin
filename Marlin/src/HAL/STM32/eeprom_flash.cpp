/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#include "../shared/eeprom_api.h"


// Only STM32F4 can support wear leveling at this time
#ifndef STM32F4xx
  #undef FLASH_EEPROM_LEVELING
#endif

/**
 * The STM32 HAL supports chips that deal with "pages" and some with "sectors" and some that
 * even have multiple "banks" of flash.
 *
 * This code is a bit of a mashup of
 *   framework-arduinoststm32/cores/arduino/stm32/stm32_eeprom.c
 *   hal/hal_lpc1768/persistent_store_flash.cpp
 *
 * This has only be written against those that use a single "sector" design.
 *
 * Those that deal with "pages" could be made to work. Looking at the STM32F07 for example, there are
 * 128 "pages", each 2kB in size. If we continued with our EEPROM being 4Kb, we'd always need to operate
 * on 2 of these pages. Each write, we'd use 2 different pages from a pool of pages until we are done.
 */

#if ENABLED(FLASH_EEPROM_LEVELING)

  #include "stm32_def.h"

  #define DEBUG_OUT ENABLED(EEPROM_CHITCHAT)
  #include "src/core/debug_out.h"

  #ifndef EEPROM_SIZE
    #define EEPROM_SIZE           0x1000  // 4kB
  #endif

  #ifndef FLASH_SECTOR
    #define FLASH_SECTOR          (FLASH_SECTOR_TOTAL - 1)
  #endif
  #ifndef FLASH_UNIT_SIZE
    #define FLASH_UNIT_SIZE       0x20000 // 128kB
  #endif

  #define FLASH_ADDRESS_START     (FLASH_END - ((FLASH_SECTOR_TOTAL - FLASH_SECTOR) * FLASH_UNIT_SIZE) + 1)
  #define FLASH_ADDRESS_END       (FLASH_ADDRESS_START + FLASH_UNIT_SIZE  - 1)

  #define EEPROM_SLOTS            (FLASH_UNIT_SIZE/EEPROM_SIZE)
  #define SLOT_ADDRESS(slot)      (FLASH_ADDRESS_START + (slot * EEPROM_SIZE))

  #define UNLOCK_FLASH()          if (!flash_unlocked) { \
                                    HAL_FLASH_Unlock(); \
                                    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | \
                                                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR); \
                                    flash_unlocked = true; \
                                  }
  #define LOCK_FLASH()            if (flash_unlocked) { HAL_FLASH_Lock(); flash_unlocked = false; }

  #define EMPTY_UINT32            ((uint32_t)-1)
  #define EMPTY_UINT8             ((uint8_t)-1)

  static uint8_t ram_eeprom[EEPROM_SIZE] __attribute__((aligned(4))) = {0};
  static int current_slot = -1;

  static_assert(0 == EEPROM_SIZE % 4, "EEPROM_SIZE must be a multiple of 4"); // Ensure copying as uint32_t is safe
  static_assert(0 == FLASH_UNIT_SIZE % EEPROM_SIZE, "EEPROM_SIZE must divide evenly into your FLASH_UNIT_SIZE");
  static_assert(FLASH_UNIT_SIZE >= EEPROM_SIZE, "FLASH_UNIT_SIZE must be greater than or equal to your EEPROM_SIZE");
  static_assert(IS_FLASH_SECTOR(FLASH_SECTOR), "FLASH_SECTOR is invalid");
  static_assert(IS_POWER_OF_2(FLASH_UNIT_SIZE), "FLASH_UNIT_SIZE should be a power of 2, please check your chip's spec sheet");

#endif

static bool eeprom_data_written = false;

bool PersistentStore::access_start() {

  #if ENABLED(FLASH_EEPROM_LEVELING)

    if (current_slot == -1 || eeprom_data_written) {
      // This must be the first time since power on that we have accessed the storage, or someone
      // loaded and called write_data and never called access_finish.
      // Lets go looking for the slot that holds our configuration.
      if (eeprom_data_written) DEBUG_ECHOLN("Dangling EEPROM write_data");
      uint32_t address = FLASH_ADDRESS_START;
      while (address <= FLASH_ADDRESS_END) {
        uint32_t address_value = (*(__IO uint32_t*)address);
        if (address_value != EMPTY_UINT32) {
          current_slot = (address - FLASH_ADDRESS_START) / EEPROM_SIZE;
          break;
        }
        address += sizeof(uint32_t);
      }
      if (current_slot == -1) {
        // We didn't find anything, so we'll just intialize to empty
        for (int i = 0; i < EEPROM_SIZE; i++) ram_eeprom[i] = EMPTY_UINT8;
        current_slot = EEPROM_SLOTS;
      }
      else {
        // load current settings
        uint8_t *eeprom_data = (uint8_t *)SLOT_ADDRESS(current_slot);
        for (int i = 0; i < EEPROM_SIZE; i++) ram_eeprom[i] = eeprom_data[i];
        DEBUG_ECHOLNPAIR("EEPROM loaded from slot ", current_slot, ".");
      }
      eeprom_data_written = false;
    }

  #else
    eeprom_buffer_fill();
  #endif

  return true;
}

bool PersistentStore::access_finish() {

  if (eeprom_data_written) {

    #if ENABLED(FLASH_EEPROM_LEVELING)

      HAL_StatusTypeDef status = HAL_ERROR;
      bool flash_unlocked = false;

      if (--current_slot < 0) {
        // all slots have been used, erase everything and start again

        FLASH_EraseInitTypeDef EraseInitStruct;
        uint32_t SectorError = 0;

        EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Sector = FLASH_SECTOR;
        EraseInitStruct.NbSectors = 1;

        current_slot = EEPROM_SLOTS - 1;
        UNLOCK_FLASH();

        status = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
        if (status != HAL_OK) {
          DEBUG_ECHOLNPAIR("HAL_FLASHEx_Erase=", status);
          DEBUG_ECHOLNPAIR("GetError=", HAL_FLASH_GetError());
          DEBUG_ECHOLNPAIR("SectorError=", SectorError);
          LOCK_FLASH();
          return false;
        }
      }

      UNLOCK_FLASH();

      uint32_t offset = 0;
      uint32_t address = SLOT_ADDRESS(current_slot);
      uint32_t address_end = address + EEPROM_SIZE;
      uint32_t data = 0;

      bool success = true;

      while (address < address_end) {
        memcpy(&data, ram_eeprom + offset, sizeof(uint32_t));
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);
        if (status == HAL_OK) {
          address += sizeof(uint32_t);
          offset += sizeof(uint32_t);
        }
        else {
          DEBUG_ECHOLNPAIR("HAL_FLASH_Program=", status);
          DEBUG_ECHOLNPAIR("GetError=", HAL_FLASH_GetError());
          DEBUG_ECHOLNPAIR("address=", address);
          success = false;
          break;
        }
      }

      LOCK_FLASH();

      if (success) {
        eeprom_data_written = false;
        DEBUG_ECHOLNPAIR("EEPROM saved to slot ", current_slot, ".");
      }

      return success;

    #else
      eeprom_buffer_flush();
      eeprom_data_written = false;
    #endif
  }

  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    uint8_t v = *value;
    #if ENABLED(FLASH_EEPROM_LEVELING)
      if (v != ram_eeprom[pos]) {
        ram_eeprom[pos] = v;
        eeprom_data_written = true;
      }
    #else
      if (v != eeprom_buffered_read_byte(pos)) {
        eeprom_buffered_write_byte(pos, v);
        eeprom_data_written = true;
      }
    #endif
    crc16(crc, &v, 1);
    pos++;
    value++;
  }
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  do {
    const uint8_t c = TERN(FLASH_EEPROM_LEVELING, ram_eeprom[pos], eeprom_buffered_read_byte(pos));
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
  return false;
}

size_t PersistentStore::capacity() {
  return TERN(FLASH_EEPROM_LEVELING, EEPROM_SIZE, E2END + 1);
}

#endif // FLASH_EEPROM_EMULATION
#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
