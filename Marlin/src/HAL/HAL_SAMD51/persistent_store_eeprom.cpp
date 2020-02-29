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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef __SAMD51__

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)

#include "../shared/persistent_store_api.h"

#if NONE(SPI_EEPROM, I2C_EEPROM)
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
#endif

bool PersistentStore::access_start() {
  #if NONE(SPI_EEPROM, I2C_EEPROM)
    NVMCTRL->SEECFG.reg = NVMCTRL_SEECFG_WMODE_BUFFERED;  // Buffered mode and segment reallocation active
  #endif

  return true;
}

bool PersistentStore::access_finish() {
  #if NONE(SPI_EEPROM, I2C_EEPROM)
    NVMCTRL_FLUSH();
    if (!NVMCTRL->SEESTAT.bit.LOCK)
      NVMCTRL_CMD(NVMCTRL_CTRLB_CMD_LSEE);    // Lock E2P data write access
  #endif

  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  #if NONE(SPI_EEPROM, I2C_EEPROM)
    if (NVMCTRL->SEESTAT.bit.RLOCK)
      NVMCTRL_CMD(NVMCTRL_CTRLB_CMD_USEE);    // Unlock E2P data write access
  #endif

  while (size--) {
    const uint8_t v = *value;
    #if ANY(SPI_EEPROM, I2C_EEPROM)
      uint8_t * const p = (uint8_t * const)pos;
      if (v != eeprom_read_byte(p)) {
        eeprom_write_byte(p, v);
        delay(2);
        if (eeprom_read_byte(p) != v) {
          SERIAL_ECHO_MSG(STR_ERR_EEPROM_WRITE);
          return true;
        }
      }
    #else
      SYNC(NVMCTRL->SEESTAT.bit.BUSY);
      if (NVMCTRL->INTFLAG.bit.SEESFULL)
        NVMCTRL_FLUSH();      // Next write will trigger a sector reallocation. I need to flush 'pagebuffer'
      ((volatile uint8_t *)SEEPROM_ADDR)[pos] = v;
      SYNC(!NVMCTRL->INTFLAG.bit.SEEWRC);
    #endif
    crc16(crc, &v, 1);
    pos++;
    value++;
  }
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  while (size--) {
    uint8_t c;
    #if ANY(SPI_EEPROM, I2C_EEPROM)
      c = eeprom_read_byte((uint8_t*)pos);
    #else
      SYNC(NVMCTRL->SEESTAT.bit.BUSY);
      c = ((volatile uint8_t *)SEEPROM_ADDR)[pos];
    #endif
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  }
  return false;
}

size_t PersistentStore::capacity() {
  #if ANY(SPI_EEPROM, I2C_EEPROM)
    return E2END + 1;
  #else
    const uint8_t psz = NVMCTRL->SEESTAT.bit.PSZ,
                  sblk = NVMCTRL->SEESTAT.bit.SBLK;

         if (!psz && !sblk)         return     0;
    else if (psz <= 2)              return (0x200 << psz);
    else if (sblk == 1 || psz == 3) return  4096;
    else if (sblk == 2 || psz == 4) return  8192;
    else if (sblk <= 4 || psz == 5) return 16384;
    else if (sblk >= 9 && psz == 7) return 65536;
    else                            return 32768;
  #endif
}

#endif // EEPROM_SETTINGS

#endif // __SAMD51__
