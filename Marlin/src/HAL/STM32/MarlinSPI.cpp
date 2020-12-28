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
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "MarlinSPI.h"

static void spi_init(spi_t *obj, uint32_t speed, spi_mode_e mode, uint8_t msb, uint32_t dataSize) {
  spi_init(obj, speed, mode, msb);
  // spi_init set 8bit always
  // TODO: copy the code from spi_init and handle data size, to avoid double init always!!
  if (dataSize != SPI_DATASIZE_8BIT) {
    obj->handle.Init.DataSize = dataSize;
    HAL_SPI_Init(&obj->handle);
    __HAL_SPI_ENABLE(&obj->handle);
  }
}

void MarlinSPI::setClockDivider(uint8_t _div) {
  _speed = spi_getClkFreq(&_spi);// / _div;
  _clockDivider = _div;
}

void MarlinSPI::begin(void) {
  //TODO: only call spi_init if any parameter changed!!
  spi_init(&_spi, _speed, _dataMode, _bitOrder, _dataSize);
}

void MarlinSPI::setupDma(SPI_HandleTypeDef &_spiHandle, DMA_HandleTypeDef &_dmaHandle, uint32_t direction, bool minc) {
  _dmaHandle.Init.Direction = direction;
  _dmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
  _dmaHandle.Init.Mode = DMA_NORMAL;
  _dmaHandle.Init.Priority = DMA_PRIORITY_LOW;
  _dmaHandle.Init.MemInc = minc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE;

  if (_dataSize == DATA_SIZE_8BIT) {
    _dmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    _dmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  }
  else {
    _dmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    _dmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  }
  #ifdef STM32F4xx
    _dmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  #endif

  // start DMA hardware
  // TODO: check if hardware is already enabled
  #ifdef SPI1_BASE
    if (_spiHandle.Instance == SPI1) {
      #ifdef STM32F1xx
        __HAL_RCC_DMA1_CLK_ENABLE();
        _dmaHandle.Instance = (direction == DMA_MEMORY_TO_PERIPH) ? DMA1_Channel3 : DMA1_Channel2;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA2_CLK_ENABLE();
        _dmaHandle.Init.Channel = DMA_CHANNEL_3;
        _dmaHandle.Instance = (direction == DMA_MEMORY_TO_PERIPH) ? DMA2_Stream3 : DMA2_Stream0;
      #endif
    }
  #endif
  #ifdef SPI2_BASE
    if (_spiHandle.Instance == SPI2) {
      #ifdef STM32F1xx
        __HAL_RCC_DMA1_CLK_ENABLE();
        _dmaHandle.Instance = (direction == DMA_MEMORY_TO_PERIPH) ? DMA1_Channel5 : DMA1_Channel4;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA1_CLK_ENABLE();
        _dmaHandle.Init.Channel = DMA_CHANNEL_0;
        _dmaHandle.Instance = (direction == DMA_MEMORY_TO_PERIPH) ? DMA1_Stream4 : DMA1_Stream3;
      #endif
    }
  #endif
  #ifdef SPI3_BASE
    if (_spiHandle.Instance == SPI3) {
      #ifdef STM32F1xx
        __HAL_RCC_DMA2_CLK_ENABLE();
        _dmaHandle.Instance = (direction == DMA_MEMORY_TO_PERIPH) ? DMA2_Channel2 : DMA2_Channel1;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA1_CLK_ENABLE();
        _dmaHandle.Init.Channel = DMA_CHANNEL_0;
        _dmaHandle.Instance = (direction == DMA_MEMORY_TO_PERIPH) ? DMA1_Stream5 : DMA1_Stream2;
      #endif
    }
  #endif

  HAL_DMA_Init(&_dmaHandle);
}

byte MarlinSPI::transfer(uint8_t _data) {
  uint8_t rxData = 0xFF;
  HAL_SPI_TransmitReceive(&_spi.handle, &_data, &rxData, 1, HAL_MAX_DELAY);
  return rxData;
}

uint8_t MarlinSPI::dmaTransfer(const void *transmitBuf, void *receiveBuf, uint16_t length) {
  const uint8_t ff = 0xFF;

  //if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) //only enable if disabled
  __HAL_SPI_ENABLE(&_spi.handle);

  if (receiveBuf) {
    setupDma(_spi.handle, _dmaRx, DMA_PERIPH_TO_MEMORY, true);
    HAL_DMA_Start(&_dmaRx, (uint32_t)&(_spi.handle.Instance->DR), (uint32_t)receiveBuf, length);
    SET_BIT(_spi.handle.Instance->CR2, SPI_CR2_RXDMAEN); /* Enable Rx DMA Request */
  }

  // check for 2 lines transfer
  bool mincTransmit = true;
  if (transmitBuf == nullptr && _spi.handle.Init.Direction == SPI_DIRECTION_2LINES && _spi.handle.Init.Mode == SPI_MODE_MASTER) {
    transmitBuf = &ff;
    mincTransmit = false;
  }

  if (transmitBuf) {
    setupDma(_spi.handle, _dmaTx, DMA_MEMORY_TO_PERIPH, mincTransmit);
    HAL_DMA_Start(&_dmaTx, (uint32_t)transmitBuf, (uint32_t)&(_spi.handle.Instance->DR), length);
    SET_BIT(_spi.handle.Instance->CR2, SPI_CR2_TXDMAEN);   /* Enable Tx DMA Request */
  }

  if (transmitBuf) {
    HAL_DMA_PollForTransfer(&_dmaTx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
    HAL_DMA_Abort(&_dmaTx);
    HAL_DMA_DeInit(&_dmaTx);
  }

  // while ((_spi.handle.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE) {}

  if (receiveBuf) {
    HAL_DMA_PollForTransfer(&_dmaRx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
    HAL_DMA_Abort(&_dmaRx);
    HAL_DMA_DeInit(&_dmaRx);
  }

  return 1;
}

uint8_t MarlinSPI::dmaSend(const void * transmitBuf, uint16_t length, bool minc) {
  setupDma(_spi.handle, _dmaTx, DMA_MEMORY_TO_PERIPH, minc);
  HAL_DMA_Start(&_dmaTx, (uint32_t)transmitBuf, (uint32_t)&(_spi.handle.Instance->DR), length);
  __HAL_SPI_ENABLE(&_spi.handle);
  SET_BIT(_spi.handle.Instance->CR2, SPI_CR2_TXDMAEN);   /* Enable Tx DMA Request */
  HAL_DMA_PollForTransfer(&_dmaTx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
  HAL_DMA_Abort(&_dmaTx);
  // DeInit objects
  HAL_DMA_DeInit(&_dmaTx);
  return 1;
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
