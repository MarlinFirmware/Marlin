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
#pragma once

#include "HAL.h"
#include <SPI.h>

/**
 * Marlin currently requires 3 SPI classes:
 *
 * SPIClass:
 *  This class is normally provided by frameworks and has a semi-default interface.
 *  This is needed because some libraries reference it globally.
 *
 * SPISettings:
 *  Container for SPI configs for SPIClass. As above, libraries may reference it globally.
 *
 * These two classes are often provided by frameworks so we cannot extend them to add
 * useful methods for Marlin.
 *
 * MarlinSPI:
 *  Provides the default SPIClass interface plus some Marlin goodies such as a simplified
 *  interface for SPI DMA transfer.
 *
 */

#define DATA_SIZE_8BIT SPI_DATASIZE_8BIT
#define DATA_SIZE_16BIT SPI_DATASIZE_16BIT
  
#if defined(STM32F1xx)
  #define SPI1_DMA_CLK_ENABLE()   __HAL_RCC_DMA1_CLK_ENABLE()
  #define SPI1_DMA_CLK_DISABLE()  __HAL_RCC_DMA1_CLK_DISABLE()
  #define SPI1_DMA_RX             DMA1_Channel2
  #define SPI1_DMA_TX             DMA1_Channel3

  #define SPI2_DMA_CLK_ENABLE()   __HAL_RCC_DMA1_CLK_ENABLE()
  #define SPI2_DMA_CLK_DISABLE()  __HAL_RCC_DMA1_CLK_DISABLE()
  #define SPI2_DMA_RX             DMA1_Channel4
  #define SPI2_DMA_TX             DMA1_Channel5

  #define SPI3_DMA_CLK_ENABLE()   __HAL_RCC_DMA2_CLK_ENABLE()
  #define SPI3_DMA_CLK_DISABLE()  __HAL_RCC_DMA2_CLK_DISABLE()
  #define SPI3_DMA_RX             DMA2_Channel1
  #define SPI3_DMA_TX             DMA2_Channel2
#elif defined(STM32F4xx)
  #define SPI1_DMA_CLK_ENABLE()   __HAL_RCC_DMA2_CLK_ENABLE()
  #define SPI1_DMA_CLK_DISABLE()  __HAL_RCC_DMA2_CLK_DISABLE()
  #define SPI1_DMA_RX             DMA2_Stream0
  #define SPI1_DMA_TX             DMA2_Stream3

  #define SPI2_DMA_CLK_ENABLE()   __HAL_RCC_DMA1_CLK_ENABLE()
  #define SPI2_DMA_CLK_DISABLE()  __HAL_RCC_DMA1_CLK_DISABLE()
  #define SPI2_DMA_RX             DMA1_Stream3
  #define SPI2_DMA_TX             DMA1_Stream4
  
  #define SPI3_DMA_CLK_ENABLE()   __HAL_RCC_DMA1_CLK_ENABLE()
  #define SPI3_DMA_CLK_DISABLE()  __HAL_RCC_DMA1_CLK_DISABLE()
  #define SPI3_DMA_RX             DMA1_Stream2
  #define SPI3_DMA_TX             DMA1_Stream5
#endif

#if ENABLED(MARLIN_DEV_MODE)
  #include "../../MarlinCore.h"
  #define HALOK(HAL_CMD) {if((HAL_CMD) != HAL_OK) {SERIAL_ERROR_MSG("Message: ", PSTR(STRINGIFY(HAL_CMD)), " failed in ", __FILE__, " at line ", __LINE__);serial_delay(20);kill();}}
#else
  #define HALOK(HAL_CMD) (HAL_CMD)
#endif

class MarlinSPI : public SPIClass {
public:
  MarlinSPI() : SPIClass(){}

  MarlinSPI(pin_t mosi, pin_t miso, pin_t sclk, pin_t ssel = (pin_t)NC) : SPIClass(mosi, miso, sclk, ssel){}
  
  virtual void begin(uint8_t _pin = CS_PIN_CONTROLLED_BY_USER) { SPIClass::begin(_pin); dmaInit(); } 
  virtual void end() { dmaDeinit(); SPIClass::end(); } 
  virtual void beginTransaction(uint8_t pin, SPISettings settings) { SPIClass::beginTransaction(pin, settings); dmaInit();  } 
  virtual void endTransaction(uint8_t pin) { dmaInit(); SPIClass::endTransaction(pin); } 

  // using SPIClass::transfer;
  // virtual byte transfer(uint8_t _pin, uint8_t data, SPITransferMode _mode = SPI_LAST){
  //   DEBUG_LOG("transfer BEGIN");
  //   byte ret = SPIClass::transfer(_pin, data, _mode);     
  //   DEBUG_LOG("transfer END");
  //   return ret;
  // }

  uint8_t dmaTransfer(const void *transmitBuf, void *receiveBuf, uint16_t length);
  uint8_t dmaSend(const void * transmitBuf, uint16_t length, bool minc = true);

private:
  void dmaInit();
  void dmaDeinit();
  DMA_HandleTypeDef _dmaTx;
  DMA_HandleTypeDef _dmaRx;
};
