/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2017 Victor Perez
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
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 */

/**
 * Adapted to the STM32F7 HAL
 */

#ifdef STM32F7

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../HAL.h"
#include "../HAL_SPI.h"
#include "pins_arduino.h"
#include "spi_pins.h"
#include "../../core/macros.h"


// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

static SPISettings spiConfig;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if ENABLED(SOFTWARE_SPI)
  // --------------------------------------------------------------------------
  // Software SPI
  // --------------------------------------------------------------------------
  #error "Software SPI not supported for STM32F7. Use hardware SPI."

#else

// --------------------------------------------------------------------------
// Hardware SPI
// --------------------------------------------------------------------------

/**
 * VGPV SPI speed start and F_CPU/2, by default 72/2 = 36Mhz
 */

/**
 * @brief  Begin SPI port setup
 *
 * @return Nothing
 *
 * @details Only configures SS pin since libmaple creates and initialize the SPI object
 */
void spiBegin(void) {
  #if !PIN_EXISTS(SS)
    #error SS_PIN not defined!
  #endif

  SET_OUTPUT(SS_PIN);
  WRITE(SS_PIN, HIGH);
}

/** Configure SPI for specified SPI speed */
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
    clock = 4000000; // Default from the SPI libarary
  }
  spiConfig = SPISettings(clock, MSBFIRST, SPI_MODE0);
  SPI.begin();
}

/**
 * @brief  Receives a single byte from the SPI port.
 *
 * @return Byte received
 *
 * @details
 */
uint8_t spiRec(void) {
  SPI.beginTransaction(spiConfig);
  uint8_t returnByte = SPI.transfer(0xFF);
  SPI.endTransaction();
  return returnByte;
}

/**
 * @brief  Receives a number of bytes from the SPI port to a buffer
 *
 * @param  buf   Pointer to starting address of buffer to write to.
 * @param  nbyte Number of bytes to receive.
 * @return Nothing
 *
 * @details Uses DMA
 */
void spiRead(uint8_t* buf, uint16_t nbyte) {
  SPI.beginTransaction(spiConfig);
  SPI.dmaTransfer(0, const_cast<uint8_t*>(buf), nbyte);
  SPI.endTransaction();
}

/**
 * @brief  Sends a single byte on SPI port
 *
 * @param  b Byte to send
 *
 * @details
 */
void spiSend(uint8_t b) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer(b);
  SPI.endTransaction();
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
  SPI.beginTransaction(spiConfig);
  SPI.transfer(token);
  SPI.dmaSend(const_cast<uint8_t*>(buf), 512);
  SPI.endTransaction();
}

#endif // SOFTWARE_SPI

#endif // STM32F7
