/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Hardware and software SPI implementations are included in this file.
 *
 * Control of the slave select pin(s) is handled by the calling routines and
 * SAMD51 let hardware SPI handling to remove SS from its logic.
 */

#ifdef __SAMD51__

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../inc/MarlinConfig.h"
#include <SPI.h>

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if EITHER(SOFTWARE_SPI, FORCE_SOFT_SPI)

  // ------------------------
  // Software SPI
  // ------------------------
  #error "Software SPI not supported for SAMD51. Use Hardware SPI."

  #if SD_CONNECTION_IS(ONBOARD)
  #endif

#else // !SOFTWARE_SPI

  #ifdef ADAFRUIT_GRAND_CENTRAL_M4
    #if SD_CONNECTION_IS(ONBOARD)
      #define sdSPI SDCARD_SPI
    #else
      #define sdSPI SPI
    #endif
  #endif

  static SPISettings spiConfig;

  // ------------------------
  // Hardware SPI
  // ------------------------
  void spiBegin(void) {
    spiInit(SPI_HALF_SPEED);
  }

  void spiInit(uint8_t spiRate) {
    // Use datarates Marlin uses
    uint32_t clock;
    switch (spiRate) {
      case SPI_FULL_SPEED:      clock = 8000000; break;
      case SPI_HALF_SPEED:      clock = 4000000; break;
      case SPI_QUARTER_SPEED:   clock = 2000000; break;
      case SPI_EIGHTH_SPEED:    clock = 1000000; break;
      case SPI_SIXTEENTH_SPEED: clock =  500000; break;
      case SPI_SPEED_5:         clock =  250000; break;
      case SPI_SPEED_6:         clock =  125000; break;
      default:                  clock = 4000000; break; // Default from the SPI library
    }
    spiConfig = SPISettings(clock, MSBFIRST, SPI_MODE0);
    sdSPI.begin();
  }

  /**
   * @brief  Receives a single byte from the SPI port.
   *
   * @return Byte received
   *
   * @details
   */
  uint8_t spiRec(void) {
    sdSPI.beginTransaction(spiConfig);
    uint8_t returnByte = sdSPI.transfer(0xFF);
    sdSPI.endTransaction();
    return returnByte;
  }

  /**
   * @brief  Receives a number of bytes from the SPI port to a buffer
   *
   * @param  buf   Pointer to starting address of buffer to write to.
   * @param  nbyte Number of bytes to receive.
   * @return Nothing
   */
  void spiRead(uint8_t* buf, uint16_t nbyte) {
    if (nbyte == 0) return;
    memset(buf, 0xFF, nbyte);
    sdSPI.beginTransaction(spiConfig);
    sdSPI.transfer(buf, nbyte);
    sdSPI.endTransaction();
  }

  /**
   * @brief  Sends a single byte on SPI port
   *
   * @param  b Byte to send
   *
   * @details
   */
  void spiSend(uint8_t b) {
    sdSPI.beginTransaction(spiConfig);
    sdSPI.transfer(b);
    sdSPI.endTransaction();
  }

  /**
   * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
   *
   * @param  buf   Pointer with buffer start address
   * @return Nothing
   *
   * @details Uses DMA
   */
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    sdSPI.beginTransaction(spiConfig);
    sdSPI.transfer(token);
    sdSPI.transfer((uint8_t*)buf, nullptr, 512);
    sdSPI.endTransaction();
  }

  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
    spiConfig = SPISettings(spiClock, (BitOrder)bitOrder, dataMode);
    sdSPI.beginTransaction(spiConfig);
  }
#endif // !SOFTWARE_SPI

#endif // __SAMD51__
