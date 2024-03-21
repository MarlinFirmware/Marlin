/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (c) 2009 by William Greiman
 */

/**
 * For TARGET_LPC5528
 */

/**
 * Hardware SPI and Software SPI implementations are included in this file.
 * The hardware SPI runs faster and has higher throughput but is not compatible
 * with some LCD interfaces/adapters.
 *
 * Control of the slave select pin(s) is handled by the calling routines.
 *
 * Some of the LCD interfaces/adapters result in the LCD SPI and the SD card
 * SPI sharing pins. The SCK, MOSI & MISO pins can NOT be set/cleared with
 * WRITE nor digitalWrite when the hardware SPI module within the LPC17xx is
 * active. If any of these pins are shared then the software SPI must be used.
 *
 * A more sophisticated hardware SPI can be found at the following link.
 * This implementation has not been fully debugged.
 * https://github.com/MarlinFirmware/Marlin/tree/071c7a78f27078fd4aee9a3ef365fcf5e143531e
 */

#ifdef TARGET_LPC5528

#include "../../inc/MarlinConfig.h"
#include <SPI.h>

// Hardware SPI and SPIClass

#include "../shared/HAL_SPI.h"

// ------------------------
// Public functions
// ------------------------

#if ENABLED(LPC_SOFTWARE_SPI)

  // Software SPI

  #include <SoftwareSPI.h>

  static uint8_t SPI_speed = SPI_FULL_SPEED;

  static uint8_t spiTransfer(uint8_t b) {
    return swSpiTransfer(b, SPI_speed, SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN);
  }

  void spiBegin() {
    swSpiBegin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN);
  }

  void spiInit(uint8_t spiRate) {
    SPI_speed = swSpiInit(spiRate, SD_SCK_PIN, SD_MOSI_PIN);
  }

  uint8_t spiRec() { return spiTransfer(0xFF); }

  void spiRead(uint8_t*buf, uint16_t nbyte) {
    for (int i = 0; i < nbyte; i++)
      buf[i] = spiTransfer(0xFF);
  }

  void spiSend(uint8_t b) { (void)spiTransfer(b); }

  void spiSend(const uint8_t *buf, size_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++)
      (void)spiTransfer(buf[i]);
  }

  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    (void)spiTransfer(token);
    for (uint16_t i = 0; i < 512; i++)
      (void)spiTransfer(buf[i]);
  }

#else

  #ifdef SD_SPI_SPEED
    #define INIT_SPI_SPEED SD_SPI_SPEED
  #else
    #define INIT_SPI_SPEED SPI_FULL_SPEED
  #endif

  void spiBegin() { spiInit(INIT_SPI_SPEED); } // Set up SCK, MOSI & MISO pins for SSP0

  void spiInit(uint8_t spiRate) {
    class SPISettings spiConfig;
    uint32_t baudrate;

    switch (spiRate) {
      case SPI_FULL_SPEED:      baudrate = 12000000; break;
      case SPI_HALF_SPEED:      baudrate = 12000000; break;
      case SPI_QUARTER_SPEED:   baudrate = 12000000; break;
      case SPI_EIGHTH_SPEED:    baudrate = 12000000; break;
      case SPI_SIXTEENTH_SPEED: baudrate = 12000000; break;
      case SPI_SPEED_5:         baudrate = 12000000; break;
      case SPI_SPEED_6:         baudrate = 12000000; break;
      default:                  baudrate = 12000000; break;
    }
    spiConfig = SPISettings(baudrate,kSPI_MsbFirst,SPI_MODE0,kSPI_Data8Bits,false);
    SPI_4.beginTransaction(spiConfig);
  }

  static uint8_t doio(uint8_t b) {
    return SPI_4.transfer(b & 0x00FF);
  }

  void spiSend(uint8_t b) { doio(b); }

  void spiSend(const uint8_t *buf, size_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++) doio(buf[i]);
  }

  void spiSend(uint32_t chan, byte b) {}

  void spiSend(uint32_t chan, const uint8_t *buf, size_t nbyte) {}

  // Read single byte from SPI
  uint8_t spiRec() { return doio(0xFF); }

  uint8_t spiRec(uint32_t chan) { return 0; }

  // Read from SPI into buffer
  void spiRead(uint8_t *buf, uint16_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++) buf[i] = doio(0xFF);
  }

  uint8_t spiTransfer(uint8_t b) { return doio(b); }

  // Write from buffer to SPI
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
   (void)spiTransfer(token);
    for (uint16_t i = 0; i < 512; i++)
      (void)spiTransfer(buf[i]);
  }

  // Begin SPI transaction, set clock, bit order, data mode
  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
    // TODO: Implement this method
  }

#endif // LPC_SOFTWARE_SPI

// Retain the pin init state of the SPI, to avoid init more than once,
// even if more instances of SPIClass exist
// static bool spiInitialised[BOARD_NR_SPI] = { false };

#endif // TARGET_LPC5528
