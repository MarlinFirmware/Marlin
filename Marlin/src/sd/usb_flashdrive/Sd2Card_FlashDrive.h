/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * \file
 * \brief Sd2Card class for V2 SD/SDHC cards
 */

/* Uncomment USB_DEBUG to enable debugging.
 *    1 - basic debugging and bounds checking
 *    2 - print each block access
 */
//#define USB_DEBUG 1

#include "../SdFatConfig.h"
#include "../SdInfo.h"

/**
 * define SOFTWARE_SPI to use bit-bang SPI
 */
#if MEGA_SOFT_SPI || USE_SOFTWARE_SPI
  #define SOFTWARE_SPI
#endif

// SPI pin definitions - do not edit here - change in SdFatConfig.h
#if DISABLED(SOFTWARE_SPI)
  // hardware pin defs
  #define SD_CHIP_SELECT_PIN  SS_PIN            // The default chip select pin for the SD card is SS.
#else  // SOFTWARE_SPI
  #define SD_CHIP_SELECT_PIN  10                // Software SPI chip select pin for the SD
#endif // SOFTWARE_SPI

class Sd2Card {
  private:

    typedef enum : uint8_t {
      USB_HOST_UNINITIALIZED,
      USB_HOST_INITIALIZED,
      USB_HOST_DELAY_INIT,
      USB_HOST_WAITING
    } state_t;

    static state_t state;

    uint32_t pos;
    #ifdef USB_DEBUG
      uint32_t lun0_capacity;
    #endif

    static inline bool ready() { return state == USB_HOST_INITIALIZED; }

  public:
    bool init(const uint8_t sckRateID=0, const pin_t chipSelectPin=SD_CHIP_SELECT_PIN);

    static void idle();

    inline bool readStart(const uint32_t block)                             { pos = block; return ready(); }
    inline bool readData(uint8_t* dst)                                      { return readBlock(pos++, dst); }
    inline bool readStop() const                                            { return true; }

    inline bool writeStart(const uint32_t block, const uint32_t eraseCount) { UNUSED(eraseCount); pos = block; return ready(); }
    inline bool writeData(uint8_t* src)                                     { return writeBlock(pos++, src); }
    inline bool writeStop() const                                           { return true; }

    bool readBlock(uint32_t block, uint8_t* dst);
    bool writeBlock(uint32_t blockNumber, const uint8_t* src);

    uint32_t cardSize();
    static bool isInserted();
};
