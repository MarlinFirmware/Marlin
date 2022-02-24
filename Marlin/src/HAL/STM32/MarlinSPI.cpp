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

#include "../platforms.h"

#if defined(HAL_STM32) && !defined(STM32H7xx)

#include "MarlinSPI.h"
#include <stm32yyxx_ll_spi.h>

SPIClass SPI = MarlinSPI();

inline void DMA_Enable(DMA_HandleTypeDef *hdma, uint32_t memInc) {
  hdma->Init.MemInc=memInc;
  HALOK(HAL_DMA_Init(hdma));
}

inline void DMA_Disable(DMA_HandleTypeDef *hdma) {
  HAL_DMA_Abort(hdma);
  HAL_DMA_DeInit(hdma);
}

void MarlinSPI::dmaInit() {
  // spi_init set 8bit always
  // if (_dataSize != SPI_DATASIZE_8BIT) {
  //   _spi.handle.Init.DataSize = _dataSize;
  // }

   //Configure Rx&Tx DMAs
  _dmaRx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  _dmaRx.Init.PeriphInc = DMA_PINC_DISABLE;
  _dmaRx.Init.MemInc = DMA_MINC_ENABLE;
  _dmaRx.Init.PeriphDataAlignment = _spi.handle.Init.DataSize == SPI_DATASIZE_8BIT ? DMA_PDATAALIGN_BYTE : DMA_PDATAALIGN_HALFWORD;
  _dmaRx.Init.MemDataAlignment = _spi.handle.Init.DataSize == SPI_DATASIZE_8BIT ? DMA_PDATAALIGN_BYTE : DMA_PDATAALIGN_HALFWORD;
  _dmaRx.Init.Mode = DMA_NORMAL;
  _dmaRx.Init.Priority = DMA_PRIORITY_LOW;

  _dmaTx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  _dmaTx.Init.PeriphInc = DMA_PINC_DISABLE;
  _dmaTx.Init.MemInc = DMA_MINC_ENABLE;
  _dmaTx.Init.PeriphDataAlignment = _spi.handle.Init.DataSize == SPI_DATASIZE_8BIT ? DMA_PDATAALIGN_BYTE : DMA_PDATAALIGN_HALFWORD;
  _dmaTx.Init.MemDataAlignment = _spi.handle.Init.DataSize == SPI_DATASIZE_8BIT ? DMA_PDATAALIGN_BYTE : DMA_PDATAALIGN_HALFWORD;
  _dmaTx.Init.Mode = DMA_NORMAL;
  _dmaTx.Init.Priority = DMA_PRIORITY_LOW;

  #ifdef STM32F4xx
    _dmaRx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    _dmaTx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  #endif

  #ifdef SPI1_BASE
    if(_spi.handle.Instance==SPI1){
      SPI1_DMA_CLK_ENABLE();
      _dmaRx.Instance = SPI1_DMA_RX;
      _dmaTx.Instance = SPI1_DMA_TX;
      #ifdef STM32F4xx
        _dmaRx.Init.Channel = DMA_CHANNEL_3;
        _dmaTx.Init.Channel = DMA_CHANNEL_3;
      #endif
    }
  #endif
  #ifdef SPI2_BASE
    if(_spi.handle.Instance==SPI2){
      SPI2_DMA_CLK_ENABLE();
      _dmaRx.Instance = SPI2_DMA_RX;
      _dmaTx.Instance = SPI2_DMA_TX;
      #ifdef STM32F4xx
        _dmaRx.Init.Channel = DMA_CHANNEL_0;
        _dmaTx.Init.Channel = DMA_CHANNEL_0;
      #endif
    }
  #endif
  #ifdef SPI3_BASE
    if(_spi.handle.Instance==SPI3){
      SPI3_DMA_CLK_ENABLE();
      _dmaRx.Instance = SPI3_DMA_RX;
      _dmaTx.Instance = SPI3_DMA_TX;
      #ifdef STM32F4xx
        _dmaRx.Init.Channel = DMA_CHANNEL_0;
        _dmaTx.Init.Channel = DMA_CHANNEL_0;
      #endif
    }
  #endif
  __HAL_LINKDMA(&_spi.handle,hdmarx,_dmaRx);
  __HAL_LINKDMA(&_spi.handle,hdmatx,_dmaTx);
}

void MarlinSPI::dmaDeinit(){
  DMA_Disable(&_dmaRx);
  DMA_Disable(&_dmaTx);
  #ifdef SPI1_BASE
    if(_spi.handle.Instance==SPI1){
      SPI1_DMA_CLK_DISABLE();
    }
  #endif
  #ifdef SPI2_BASE
    if(_spi.handle.Instance==SPI2){
      SPI2_DMA_CLK_DISABLE();
  }
  #endif
  #ifdef SPI3_BASE
    if(_spi.handle.Instance==SPI3){
      SPI3_DMA_CLK_DISABLE();
    }
  #endif
}

uint8_t MarlinSPI::dmaTransfer(const void *transmitBuf, void *receiveBuf, uint16_t length) {
  // In Master mode with 2 lines, transmitting is required to receive 
  if (receiveBuf && (_spi.handle.Init.Direction == SPI_DIRECTION_2LINES) && (_spi.handle.Init.Mode == SPI_MODE_MASTER)){
    transmitBuf = receiveBuf;
  }

  __HAL_LOCK(&_spi.handle);

  if (receiveBuf){
    DMA_Enable(&_dmaRx, DMA_MINC_ENABLE);
    HALOK(HAL_DMA_Start(&_dmaRx, LL_SPI_DMA_GetRegAddr(_spi.handle.Instance), (uint32_t)receiveBuf, length));
    LL_SPI_EnableDMAReq_RX(_spi.handle.Instance); // Enable Rx DMA Request
  }
  if (transmitBuf){
    DMA_Enable(&_dmaTx, DMA_MINC_ENABLE);
    HALOK(HAL_DMA_Start(&_dmaTx, (uint32_t)transmitBuf, LL_SPI_DMA_GetRegAddr(_spi.handle.Instance), length));
    LL_SPI_EnableDMAReq_TX(_spi.handle.Instance); // Enable Tx DMA Request
  }

  if (!LL_SPI_IsEnabled(_spi.handle.Instance)) {
    __HAL_SPI_ENABLE(&_spi.handle); // Enable SPI device
  }
  
  if (transmitBuf){
    HALOK(HAL_DMA_PollForTransfer(&_dmaTx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY));
    while (!__HAL_SPI_GET_FLAG(&_spi.handle, SPI_FLAG_TXE)) {}
    while ( __HAL_SPI_GET_FLAG(&_spi.handle, SPI_FLAG_BSY)) {}
  } else if (receiveBuf) { 
    HALOK(HAL_DMA_PollForTransfer(&_dmaRx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY));
  }
  if (receiveBuf) while ( __HAL_SPI_GET_FLAG(&_spi.handle, SPI_FLAG_RXNE)) {}

  // Clear unread Rx data (overrun), if applicable
  if (!receiveBuf && _spi.handle.Init.Direction == SPI_DIRECTION_2LINES) {
    __HAL_SPI_CLEAR_OVRFLAG(&_spi.handle);
  }
  // Always disable both Rx/Tx DMA requests
  LL_SPI_DisableDMAReq_RX(_spi.handle.Instance);
  LL_SPI_DisableDMAReq_TX(_spi.handle.Instance);
  
  __HAL_UNLOCK(&_spi.handle);

  if (receiveBuf)  DMA_Disable(&_dmaRx);
  if (transmitBuf) DMA_Disable(&_dmaTx);

  return 1;
}

uint8_t MarlinSPI::dmaSend(const void * transmitBuf, uint16_t length, bool minc) {
  return dmaTransfer(transmitBuf, NULL, length); 
}

#endif // HAL_STM32 && !STM32H7xx
