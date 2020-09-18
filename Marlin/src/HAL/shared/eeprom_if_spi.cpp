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
 * Platform-independent Arduino functions for SPI EEPROM.
 * Enable USE_SHARED_EEPROM if not supplied by the framework.
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(SPI_EEPROM)

#include "eeprom_if.h"

void eeprom_init() {}

#if ENABLED(USE_SHARED_EEPROM)

#define CMD_WREN  6   // WREN
#define CMD_READ  2   // WRITE
#define CMD_WRITE 2   // WRITE

#ifndef EEPROM_WRITE_DELAY
  #define EEPROM_WRITE_DELAY    7
#endif

uint8_t eeprom_read_byte(uint8_t* pos) {
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

void eeprom_write_byte(uint8_t* pos, uint8_t value) {
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

#endif // USE_SHARED_EEPROM
#endif // I2C_EEPROM
