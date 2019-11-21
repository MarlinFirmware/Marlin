/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"

#include <SPI.h>

// ------------------------
// Public Variables
// ------------------------
static SPISettings* spiConfig[NUM_SPI_BUSES]  = { NULL };
static void* spiBus[NUM_SPI_BUSES]  = { NULL };

// ------------------------
// Public functions
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
void spiBegin(uint8_t bus_num) {
  //the bus must be initialized with spiInit
}

bool spiInitialized(uint8_t bus_num)
{
  return spiBus[bus_num] != NULL;
}

/** Configure SPI BUS for specified SPI speed */
void spiInit(uint8_t bus_num, uint8_t spiRate) {
  if (!spiInitialized(bus_num)) {
    
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

    if (HW_SPI(bus_num))
    {
      spiBus[bus_num] = new SPIClass(SPI_BusPins[bus_num][SPIBUS_MOSI], SPI_BusPins[bus_num][SPIBUS_MISO], SPI_BusPins[bus_num][SPIBUS_CLCK]);
      spiConfig[bus_num] = new SPISettings(clock, MSBFIRST, SPI_MODE0);
    }
    else
    {
      //TODO if needed.
      //spiBus[bus_num] = new SoftSPI(SPI_BusPins[bus_num][SPIBUS_MOSI], SPI_BusPins[bus_num][SPIBUS_MISO], SPI_BusPins[bus_num][SPIBUS_CLCK]);
    }
    GET_BUS(bus_num) -> begin();
  }
}

/**
 * @brief  Receives a single byte from the SPI port.
 *
 * @return Byte received
 *
 * @details
 */
uint8_t spiRec(uint8_t dev_num) { //transactions are supported only on HW SPI
  
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> beginTransaction(CS_OF_DEV(dev_num), *spiConfig[BUS_OF_DEV(dev_num)]);
  uint8_t returnByte = GET_BUS(BUS_OF_DEV(dev_num)) -> transfer(CS_OF_DEV(dev_num), 0xFF);
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> endTransaction(CS_OF_DEV(dev_num));
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
void spiRead(uint8_t dev_num, uint8_t* buf, uint16_t nbyte) {
  if (nbyte == 0) return;
  memset(buf, 0xFF, nbyte);
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> beginTransaction(CS_OF_DEV(dev_num), *spiConfig[BUS_OF_DEV(dev_num)]);
  GET_BUS(BUS_OF_DEV(dev_num)) -> transfer(CS_OF_DEV(dev_num), buf, nbyte);
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> endTransaction(CS_OF_DEV(dev_num));
}

/**
 * @brief  Sends a single byte on SPI port
 *
 * @param  b Byte to send
 *
 * @details
 */
void spiSend(uint8_t dev_num, uint8_t b) {
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> beginTransaction(CS_OF_DEV(dev_num), *spiConfig[BUS_OF_DEV(dev_num)]);
  GET_BUS(BUS_OF_DEV(dev_num)) -> transfer(CS_OF_DEV(dev_num), b);
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> endTransaction(CS_OF_DEV(dev_num));
}

/**
 * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
 *
 * @param  buf   Pointer with buffer start address
 * @return Nothing
 *
 * @details Use DMA
 */
void spiSendBlock(uint8_t dev_num, uint8_t token, const uint8_t* buf) {
  uint8_t rxBuf[512];
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> beginTransaction(CS_OF_DEV(dev_num), *spiConfig[BUS_OF_DEV(dev_num)]);
  GET_BUS(BUS_OF_DEV(dev_num)) -> transfer(CS_OF_DEV(dev_num), token, SPI_CONTINUE);
  GET_BUS(BUS_OF_DEV(dev_num)) -> transfer(CS_OF_DEV(dev_num), (uint8_t*)buf, &rxBuf, 512, SPI_LAST);
  if (HW_SPI(BUS_OF_DEV(dev_num))) ((SPIClass*)spiBus[BUS_OF_DEV(dev_num)]) -> endTransaction(CS_OF_DEV(dev_num));
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
