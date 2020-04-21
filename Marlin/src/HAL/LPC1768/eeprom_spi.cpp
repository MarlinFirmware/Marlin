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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Description: functions for I2C connected external EEPROM.
 * Not platform dependent.
 *
 * TODO: Some platform Arduino libraries define these functions
 *       so Marlin needs to add a glue layer to prevent the conflict.
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#if ENABLED(SPI_EEPROM)

#include "../shared/eeprom_api.h"
#include "../HAL.h"

#define CMD_WREN  6   // WREN
#define CMD_READ  2   // WRITE
#define CMD_WRITE 2   // WRITE

#ifndef EEPROM_SIZE
  #define EEPROM_SIZE           0x8000 // 32kB‬
#endif

#ifndef EEPROM_WRITE_DELAY
  #define EEPROM_WRITE_DELAY    7
#endif

struct PersistentStore::Helper {

  static void eeprom_write_byte(uint8_t *pos, unsigned char value) {
    uint8_t eeprom_temp[3];

    /*write enable*/
    eeprom_temp[0] = CMD_WREN;
    WRITE(SPI_EEPROM1_CS, LOW);
    spiSend(SPI_CHAN_EEPROM1, eeprom_temp, 1);
    WRITE(SPI_EEPROM1_CS, HIGH);
    delay(1);

    /*write addr*/
    eeprom_temp[0] = CMD_WRITE;
    eeprom_temp[1] = ((unsigned)pos>>8) & 0xFF;  //addr High
    eeprom_temp[2] = (unsigned)pos & 0xFF;       //addr Low
    WRITE(SPI_EEPROM1_CS, LOW);
    spiSend(SPI_CHAN_EEPROM1, eeprom_temp, 3);

    spiSend(SPI_CHAN_EEPROM1, value);
    WRITE(SPI_EEPROM1_CS, HIGH);
    delay(EEPROM_WRITE_DELAY);   // wait for page write to complete
  }

  static uint8_t eeprom_read_byte(uint8_t *pos) {

    uint8_t v;
    uint8_t eeprom_temp[3];

    // set read location
    // begin transmission from device
    eeprom_temp[0] = CMD_READ;
    eeprom_temp[1] = ((unsigned)pos>>8) & 0xFF; // addr High
    eeprom_temp[2] = (unsigned)pos& 0xFF;       // addr Low
    WRITE(SPI_EEPROM1_CS, HIGH);
    WRITE(SPI_EEPROM1_CS, LOW);
    spiSend(SPI_CHAN_EEPROM1, eeprom_temp, 3);

    v = spiRec(SPI_CHAN_EEPROM1);
    WRITE(SPI_EEPROM1_CS, HIGH);
    return v;
  }
};

bool PersistentStore::access_start() {
  return true;
}

bool PersistentStore::access_finish() {
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    uint8_t v = *value;

    // EEPROM has only ~100,000 write cycles,
    // so only write bytes that have changed!
    uint8_t * const p = (uint8_t * const)pos;
    if (v != Helper::eeprom_read_byte(p)) {

      Helper::eeprom_write_byte(p, v);
      const uint8_t r = Helper::eeprom_read_byte(p);

      if (r != v) {
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
    // Read from external EEPROM
    const uint8_t c = Helper::eeprom_read_byte((uint8_t*)pos);

    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
  return false;
}

size_t PersistentStore::capacity() {
  return EEPROM_SIZE;
}

#endif // SPI_EEPROM
#endif // TARGET_LPC1768
