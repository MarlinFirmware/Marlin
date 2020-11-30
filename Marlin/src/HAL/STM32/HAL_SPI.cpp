/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"

#include <SPI.h>

// ------------------------
// Public Variables
// ------------------------

static SPISettings spiConfig;

// ------------------------
// Public functions
// ------------------------

#if ENABLED(SOFTWARE_SPI)

  // ------------------------
  // Software SPI
  // ------------------------

  #include "../shared/Delay.h"

  void spiBegin(void) {
    OUT_WRITE(SS_PIN, HIGH);
    OUT_WRITE(SCK_PIN, HIGH);
    SET_INPUT(MISO_PIN);
    OUT_WRITE(MOSI_PIN, HIGH);
  }

  static uint16_t delay_STM32_soft_spi;

  void spiInit(uint8_t spiRate) {
    // Use datarates Marlin uses
    switch (spiRate) {
      case SPI_FULL_SPEED:   delay_STM32_soft_spi =  125; break;  // desired: 8,000,000  actual: ~1.1M
      case SPI_HALF_SPEED:   delay_STM32_soft_spi =  125; break;  // desired: 4,000,000  actual: ~1.1M
      case SPI_QUARTER_SPEED:delay_STM32_soft_spi =  250; break;  // desired: 2,000,000  actual: ~890K
      case SPI_EIGHTH_SPEED: delay_STM32_soft_spi =  500; break;  // desired: 1,000,000  actual: ~590K
      case SPI_SPEED_5:      delay_STM32_soft_spi = 1000; break;  // desired:   500,000  actual: ~360K
      case SPI_SPEED_6:      delay_STM32_soft_spi = 2000; break;  // desired:   250,000  actual: ~210K
      default:               delay_STM32_soft_spi = 4000; break;  // desired:   125,000  actual: ~123K
    }
    SPI.begin();
  }

  // Begin SPI transaction, set clock, bit order, data mode
  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) { /* do nothing */ }

  uint8_t HAL_SPI_STM32_SpiTransfer_Mode_3(uint8_t b) { // using Mode 3
    for (uint8_t bits = 8; bits--;) {
      WRITE(SCK_PIN, LOW);
      WRITE(MOSI_PIN, b & 0x80);

      DELAY_NS(delay_STM32_soft_spi);
      WRITE(SCK_PIN, HIGH);
      DELAY_NS(delay_STM32_soft_spi);

      b <<= 1;        // little setup time
      b |= (READ(MISO_PIN) != 0);
    }
    DELAY_NS(125);
    return b;
  }

  // Soft SPI receive byte
  uint8_t spiRec() {
    DISABLE_ISRS();                                               // No interrupts during byte receive
    const uint8_t data = HAL_SPI_STM32_SpiTransfer_Mode_3(0xFF);
    ENABLE_ISRS();                                                // Enable interrupts
    return data;
  }

  // Soft SPI read data
  void spiRead(uint8_t *buf, uint16_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++)
      buf[i] = spiRec();
  }

  // Soft SPI send byte
  void spiSend(uint8_t data) {
    DISABLE_ISRS();                         // No interrupts during byte send
    HAL_SPI_STM32_SpiTransfer_Mode_3(data); // Don't care what is received
    ENABLE_ISRS();                          // Enable interrupts
  }

  // Soft SPI send block
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    spiSend(token);
    for (uint16_t i = 0; i < 512; i++)
      spiSend(buf[i]);
  }

#else

  // ------------------------
  // Hardware SPI
  // ------------------------

  /**
   * VGPV SPI speed start and PCLK2/2, by default 108/2 = 54Mhz
   */

  /**
   * @brief  Begin SPI port setup
   *
   * @return Nothing
   *
   * @details Only configures SS pin since stm32duino creates and initialize the SPI object
   */
  void spiBegin() {
    #if PIN_EXISTS(SS)
      OUT_WRITE(SS_PIN, HIGH);
    #endif
  }

  // Configure SPI for specified SPI speed
  void spiInit(uint8_t spiRate) {
    // Use datarates Marlin uses
    uint32_t clock;
    switch (spiRate) {
      case SPI_FULL_SPEED:    clock = 20000000; break; // 13.9mhz=20000000  6.75mhz=10000000  3.38mhz=5000000  .833mhz=1000000
      case SPI_HALF_SPEED:    clock =  5000000; break;
      case SPI_QUARTER_SPEED: clock =  2500000; break;
      case SPI_EIGHTH_SPEED:  clock =  1250000; break;
      case SPI_SPEED_5:       clock =   625000; break;
      case SPI_SPEED_6:       clock =   300000; break;
      default:
        clock = 4000000; // Default from the SPI library
    }
    spiConfig = SPISettings(clock, MSBFIRST, SPI_MODE0);

    #if ENABLED(CUSTOM_SPI_PINS)
      SPI.setMISO(MISO_PIN);
      SPI.setMOSI(MOSI_PIN);
      SPI.setSCLK(SCK_PIN);
      SPI.setSSEL(SS_PIN);
    #endif

    SPI.begin();
  }

  /**
   * @brief  Receives a single byte from the SPI port.
   *
   * @return Byte received
   *
   * @details
   */
  uint8_t spiRec() {
    uint8_t returnByte = SPI.transfer(0xFF);
    return returnByte;
  }

  /**
   * @brief  Receive a number of bytes from the SPI port to a buffer
   *
   * @param  buf   Pointer to starting address of buffer to write to.
   * @param  nbyte Number of bytes to receive.
   * @return Nothing
   *
   * @details Uses DMA
   */
  void spiRead(uint8_t* buf, uint16_t nbyte) {
    if (nbyte == 0) return;
    memset(buf, 0xFF, nbyte);
    SPI.transfer(buf, nbyte);
  }

  /**
   * @brief  Send a single byte on SPI port
   *
   * @param  b Byte to send
   *
   * @details
   */
  void spiSend(uint8_t b) {
    SPI.transfer(b);
  }

  /**
   * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
   *
   * @param  buf   Pointer with buffer start address
   * @return Nothing
   *
   * @details Use DMA
   */
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    uint8_t rxBuf[512];
    SPI.transfer(token);
    SPI.transfer((uint8_t*)buf, &rxBuf, 512);
  }

#endif // SOFTWARE_SPI

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
