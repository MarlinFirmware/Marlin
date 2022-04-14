/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include <stdint.h>

/**
 * DiskIO Interface
 *
 * Interface for low level disk io
 */
class DiskIODriver {
public:
  /**
   * Initialize an SD flash memory card with default clock rate and chip
   * select pin.  See sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin).
   *
   * \return true for success or false for failure.
   */
  virtual bool init(const uint8_t sckRateID, const pin_t chipSelectPin) = 0; //TODO: only for SPI

  /**
   * Read a card's CSD register. The CSD contains Card-Specific Data that
   * provides information regarding access to the card's contents.
   *
   * \param[out] csd pointer to area for returned data.
   *
   * \return true for success or false for failure.
   */
  virtual bool readCSD(csd_t* csd) = 0;

  virtual bool readStart(const uint32_t block) = 0;
  virtual bool readData(uint8_t* dst) = 0;
  virtual bool readStop() = 0;

  virtual bool writeStart(const uint32_t block, const uint32_t) = 0;
  virtual bool writeData(const uint8_t* src) = 0;
  virtual bool writeStop() = 0;

  virtual bool readBlock(uint32_t block, uint8_t* dst) = 0;
  virtual bool writeBlock(uint32_t blockNumber, const uint8_t* src) = 0;

  virtual uint32_t cardSize() = 0;

  virtual bool isReady() = 0;

  virtual void idle() = 0;
};
