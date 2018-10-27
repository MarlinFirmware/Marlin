/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * \file
 * \brief Sd2Card class for V2 SD/SDHC cards
 */

#ifndef _SD2CARD_FLASHDRIVE_H_
#define _SD2CARD_FLASHDRIVE_H_

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
  // The following three pins must not be redefined for hardware SPI.
  #define SPI_MOSI_PIN        MOSI_PIN          // SPI Master Out Slave In pin
  #define SPI_MISO_PIN        MISO_PIN          // SPI Master In Slave Out pin
  #define SPI_SCK_PIN         SCK_PIN           // SPI Clock pin
#else  // SOFTWARE_SPI
  #define SD_CHIP_SELECT_PIN  SOFT_SPI_CS_PIN   // SPI chip select pin
  #define SPI_MOSI_PIN        SOFT_SPI_MOSI_PIN // SPI Master Out Slave In pin
  #define SPI_MISO_PIN        SOFT_SPI_MISO_PIN // SPI Master In Slave Out pin
  #define SPI_SCK_PIN         SOFT_SPI_SCK_PIN  // SPI Clock pin
#endif // SOFTWARE_SPI

class Sd2Card {
  private:

    typedef enum {
      USB_HOST_DELAY_INIT,
      USB_HOST_WAITING,
      USB_HOST_UNINITIALIZED,
      USB_HOST_INITIALIZED
    } state_t;

    static state_t  state;
    static uint32_t block;

    static bool usbHostReady();

  public:
    static void idle();

    static bool isInserted();

    uint32_t cardSize();

    bool init(uint8_t sckRateID = 0, uint8_t chipSelectPin = SD_CHIP_SELECT_PIN);

    bool readData(uint8_t* dst);
    bool readStart(uint32_t blockNumber);
    bool readStop();
    bool readBlock(uint32_t block, uint8_t* dst);

    bool writeData(const uint8_t* src);
    bool writeStart(uint32_t blockNumber, uint32_t eraseCount);
    bool writeStop();
    bool writeBlock(uint32_t blockNumber, const uint8_t* src);
};

#endif  // _SD2CARD_FLASHDRIVE_H_
