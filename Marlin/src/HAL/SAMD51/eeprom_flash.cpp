/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
#ifdef __SAMD51__

#include "../../inc/MarlinConfig.h"

#if ENABLED(FLASH_EEPROM_EMULATION)

#include "../shared/eeprom_api.h"

#define NVMCTRL_CMD(c)    do{                                                 \
                            SYNC(!NVMCTRL->STATUS.bit.READY);                 \
                            NVMCTRL->INTFLAG.bit.DONE = true;                 \
                            NVMCTRL->CTRLB.reg = c | NVMCTRL_CTRLB_CMDEX_KEY; \
                            SYNC(NVMCTRL->INTFLAG.bit.DONE);                  \
                          }while(0)
#define NVMCTRL_FLUSH()   do{                                           \
                            if (NVMCTRL->SEESTAT.bit.LOAD)              \
                              NVMCTRL_CMD(NVMCTRL_CTRLB_CMD_SEEFLUSH);  \
                          }while(0)

size_t PersistentStore::capacity() {
  const uint8_t psz = NVMCTRL->SEESTAT.bit.PSZ,
                sblk = NVMCTRL->SEESTAT.bit.SBLK;

  return   (!psz && !sblk)         ? 0
         : (psz <= 2)              ? (0x200 << psz)
         : (sblk == 1 || psz == 3) ?  4096
         : (sblk == 2 || psz == 4) ?  8192
         : (sblk <= 4 || psz == 5) ? 16384
         : (sblk >= 9 && psz == 7) ? 65536
                                   : 32768;
}

bool PersistentStore::access_start() {
  NVMCTRL->SEECFG.reg = NVMCTRL_SEECFG_WMODE_BUFFERED;  // Buffered mode and segment reallocation active
  if (NVMCTRL->SEESTAT.bit.RLOCK)
    NVMCTRL_CMD(NVMCTRL_CTRLB_CMD_USEE);    // Unlock E2P data write access
  return true;
}

bool PersistentStore::access_finish() {
  NVMCTRL_FLUSH();
  if (!NVMCTRL->SEESTAT.bit.LOCK)
    NVMCTRL_CMD(NVMCTRL_CTRLB_CMD_LSEE);    // Lock E2P data write access
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    const uint8_t v = *value;
    SYNC(NVMCTRL->SEESTAT.bit.BUSY);
    if (NVMCTRL->INTFLAG.bit.SEESFULL)
      NVMCTRL_FLUSH();      // Next write will trigger a sector reallocation. I need to flush 'pagebuffer'
    ((volatile uint8_t *)SEEPROM_ADDR)[pos] = v;
    SYNC(!NVMCTRL->INTFLAG.bit.SEEWRC);
    crc16(crc, &v, 1);
    pos++;
    value++;
  }
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  while (size--) {
    SYNC(NVMCTRL->SEESTAT.bit.BUSY);
    uint8_t c = ((volatile uint8_t *)SEEPROM_ADDR)[pos];
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  }
  return false;
}

#endif // FLASH_EEPROM_EMULATION
#endif // __SAMD51__
