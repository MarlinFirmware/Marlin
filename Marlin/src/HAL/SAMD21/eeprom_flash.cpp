/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * SAMD21 HAL developed by Bart Meijer (brupje)
 * Based on SAMD51 HAL by Giuliano Zaro (AKA GMagician)
 */
#ifdef __SAMD21__

#include "../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#define TOTAL_FLASH_SIZE (MARLIN_EEPROM_SIZE+255)/256*256

/* reserve flash memory */
static const uint8_t flashdata[TOTAL_FLASH_SIZE]  __attribute__((__aligned__(256))) { }; \

#include "../shared/eeprom_api.h"

size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE; }

/*
  const uint8_t psz = NVMCTRL->SEESTAT.bit.PSZ,
                sblk = NVMCTRL->SEESTAT.bit.SBLK;

  return (
    (!psz && !sblk)         ? 0
   : (psz <= 2)              ? (0x200 << psz)
   : (sblk == 1 || psz == 3) ?  4096
   : (sblk == 2 || psz == 4) ?  8192
   : (sblk <= 4 || psz == 5) ? 16384
   : (sblk >= 9 && psz == 7) ? 65536
                             : 32768
  ) - eeprom_exclude_size;
}
*/

uint32_t PAGE_SIZE;
uint32_t ROW_SIZE;
bool hasWritten = false;
uint8_t * buffer;

void _erase(const volatile void *flash_ptr) {
  NVMCTRL->ADDR.reg = ((uint32_t)flash_ptr) / 2;
  NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER;
  while (!NVMCTRL->INTFLAG.bit.READY) { }

}

void erase(const volatile void *flash_ptr, uint32_t size) {
  const uint8_t *ptr = (const uint8_t *)flash_ptr;
  while (size > ROW_SIZE) {
    _erase(ptr);
    ptr += ROW_SIZE;
    size -= ROW_SIZE;
  }
  _erase(ptr);
}

bool PersistentStore::access_start() {
  /* clear page buffer*/
  NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_PBC;
  while (NVMCTRL->INTFLAG.bit.READY == 0) { }

  PAGE_SIZE =  pow(2,3 + NVMCTRL->PARAM.bit.PSZ);
  ROW_SIZE= PAGE_SIZE * 4;
  /*NVMCTRL->SEECFG.reg = NVMCTRL_SEECFG_WMODE_BUFFERED;  // Buffered mode and segment reallocation active
  if (NVMCTRL->SEESTAT.bit.RLOCK)
    NVMCTRL_CMD(NVMCTRL_CTRLB_CMD_USEE);  */  // Unlock E2P data write access
  //   erase(&flashdata[0], TOTAL_FLASH_SIZE);
  return true;
}

bool PersistentStore::access_finish() {
  if (hasWritten) {
    erase(&flashdata[0], TOTAL_FLASH_SIZE);

    NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_PBC;
    while (NVMCTRL->INTFLAG.bit.READY == 0) { }

     NVMCTRL->CTRLB.bit.MANW = 0;

    volatile uint32_t *dst_addr =  (volatile uint32_t *) &flashdata;

    uint32_t *pointer = (uint32_t *) buffer;
    for (uint32_t i = 0; i < TOTAL_FLASH_SIZE; i += 4) {
      *dst_addr = (uint32_t) *pointer;
      pointer++;
      dst_addr ++;
    }

    // Execute "WP" Write Page
    NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP;
    while (NVMCTRL->INTFLAG.bit.READY == 0) { }

    free(buffer);
    hasWritten = false;
  }
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  if (!hasWritten) {
    // init temp buffer
    buffer = (uint8_t *) malloc(MARLIN_EEPROM_SIZE);
    hasWritten = true;
  }

  memcpy(buffer+pos,value,size);
  pos += size;
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  volatile uint8_t *dst_addr =  (volatile uint8_t *) &flashdata;
  dst_addr += pos;

  memcpy(value, (const void *)dst_addr, size);
  pos += size;
  return false;
}

#endif // FLASH_EEPROM_EMULATION
#endif // __SAMD21__
