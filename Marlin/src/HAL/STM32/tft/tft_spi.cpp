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

#include "../../platforms.h"

#ifdef HAL_STM32

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_TFT

#include "tft_spi.h"
#include "pinconfig.h"

SPI_HandleTypeDef TFT_SPI::SPIx;
DMA_HandleTypeDef TFT_SPI::DMAtx;

void TFT_SPI::Init() {
  SPI_TypeDef *spiInstance;

  OUT_WRITE(TFT_A0_PIN, HIGH);
  OUT_WRITE(TFT_CS_PIN, HIGH);

  if ((spiInstance = (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_SCK_PIN),  PinMap_SPI_SCLK)) == NP) return;
  if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_MOSI_PIN), PinMap_SPI_MOSI)) return;

  #if PIN_EXISTS(TFT_MISO) && TFT_MISO_PIN != TFT_MOSI_PIN
    if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_MISO_PIN), PinMap_SPI_MISO)) return;
  #endif

  SPIx.Instance                = spiInstance;
  SPIx.State                   = HAL_SPI_STATE_RESET;
  SPIx.Init.NSS                = SPI_NSS_SOFT;
  SPIx.Init.Mode               = SPI_MODE_MASTER;
  SPIx.Init.Direction          = (TFT_MISO_PIN == TFT_MOSI_PIN) ? SPI_DIRECTION_1LINE : SPI_DIRECTION_2LINES;
  SPIx.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_2;
  SPIx.Init.CLKPhase           = SPI_PHASE_1EDGE;
  SPIx.Init.CLKPolarity        = SPI_POLARITY_LOW;
  SPIx.Init.DataSize           = SPI_DATASIZE_8BIT;
  SPIx.Init.FirstBit           = SPI_FIRSTBIT_MSB;
  SPIx.Init.TIMode             = SPI_TIMODE_DISABLE;
  SPIx.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
  SPIx.Init.CRCPolynomial      = 10;

  pinmap_pinout(digitalPinToPinName(TFT_SCK_PIN), PinMap_SPI_SCLK);
  pinmap_pinout(digitalPinToPinName(TFT_MOSI_PIN), PinMap_SPI_MOSI);
  #if PIN_EXISTS(TFT_MISO) && TFT_MISO_PIN != TFT_MOSI_PIN
    pinmap_pinout(digitalPinToPinName(TFT_MISO_PIN), PinMap_SPI_MISO);
  #endif
  pin_PullConfig(get_GPIO_Port(STM_PORT(digitalPinToPinName(TFT_SCK_PIN))), STM_LL_GPIO_PIN(digitalPinToPinName(TFT_SCK_PIN)), GPIO_PULLDOWN);

  #ifdef SPI1_BASE
    if (SPIx.Instance == SPI1) {
      __HAL_RCC_SPI1_CLK_ENABLE();
      #ifdef STM32F1xx
        __HAL_RCC_DMA1_CLK_ENABLE();
        DMAtx.Instance = DMA1_Channel3;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA2_CLK_ENABLE();
        DMAtx.Instance = DMA2_Stream3;
        DMAtx.Init.Channel = DMA_CHANNEL_3;
      #endif
      SPIx.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_4;
    }
  #endif
  #ifdef SPI2_BASE
    if (SPIx.Instance == SPI2) {
      __HAL_RCC_SPI2_CLK_ENABLE();
      #ifdef STM32F1xx
        __HAL_RCC_DMA1_CLK_ENABLE();
        DMAtx.Instance = DMA1_Channel5;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA1_CLK_ENABLE();
        DMAtx.Instance = DMA1_Stream4;
        DMAtx.Init.Channel = DMA_CHANNEL_0;
      #endif
    }
  #endif
  #ifdef SPI3_BASE
    if (SPIx.Instance == SPI3) {
      __HAL_RCC_SPI3_CLK_ENABLE();
      #ifdef STM32F1xx
        __HAL_RCC_DMA2_CLK_ENABLE();
        DMAtx.Instance = DMA2_Channel2;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA1_CLK_ENABLE();
        DMAtx.Instance = DMA1_Stream5;
        DMAtx.Init.Channel = DMA_CHANNEL_0;
      #endif
    }
  #endif

  HAL_SPI_Init(&SPIx);

  DMAtx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  DMAtx.Init.PeriphInc = DMA_PINC_DISABLE;
  DMAtx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  DMAtx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DMAtx.Init.Mode = DMA_NORMAL;
  DMAtx.Init.Priority = DMA_PRIORITY_LOW;
  #ifdef STM32F4xx
    DMAtx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  #endif
}

void TFT_SPI::DataTransferBegin(uint16_t DataSize) {
  SPIx.Init.DataSize = DataSize == DATASIZE_8BIT ?  SPI_DATASIZE_8BIT : SPI_DATASIZE_16BIT;
  HAL_SPI_Init(&SPIx);
  WRITE(TFT_CS_PIN, LOW);
}

#ifdef TFT_DEFAULT_DRIVER
  #include "../../../lcd/tft_io/tft_ids.h"
#endif

uint32_t TFT_SPI::GetID() {
  uint32_t id;
  id = ReadID(LCD_READ_ID);
  if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF) {
    id = ReadID(LCD_READ_ID4);
    #ifdef TFT_DEFAULT_DRIVER
      if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
        id = TFT_DEFAULT_DRIVER;
    #endif
   }
  return id;
}

uint32_t TFT_SPI::ReadID(uint16_t Reg) {
  uint32_t Data = 0;
  #if PIN_EXISTS(TFT_MISO)
    uint32_t BaudRatePrescaler = SPIx.Init.BaudRatePrescaler;
    uint32_t i;

    SPIx.Init.BaudRatePrescaler = SPIx.Instance == SPI1 ? SPI_BAUDRATEPRESCALER_8 : SPI_BAUDRATEPRESCALER_4;
    DataTransferBegin(DATASIZE_8BIT);
    WriteReg(Reg);

    if (SPIx.Init.Direction == SPI_DIRECTION_1LINE) SPI_1LINE_RX(&SPIx);
    __HAL_SPI_ENABLE(&SPIx);

    for (i = 0; i < 4; i++) {
      #if TFT_MISO_PIN != TFT_MOSI_PIN
        while (!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_TXE)) {}
        SPIx.Instance->DR = 0;
      #endif
      while (!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_RXNE)) {}
      Data = (Data << 8) | SPIx.Instance->DR;
    }

    DataTransferEnd();

    SPIx.Init.BaudRatePrescaler   = BaudRatePrescaler;
  #endif

  return Data >> 7;
}

bool TFT_SPI::isBusy() {
  #ifdef STM32F1xx
    #define __IS_DMA_ENABLED(__HANDLE__)      ((__HANDLE__)->Instance->CCR & DMA_CCR_EN)
    #define __IS_DMA_CONFIGURED(__HANDLE__)   ((__HANDLE__)->Instance->CPAR != 0)
  #elif defined(STM32F4xx)
    #define __IS_DMA_ENABLED(__HANDLE__)      ((__HANDLE__)->Instance->CR & DMA_SxCR_EN)
    #define __IS_DMA_CONFIGURED(__HANDLE__)   ((__HANDLE__)->Instance->PAR != 0)
  #endif

  if (!__IS_DMA_CONFIGURED(&DMAtx)) return false;

  if (__HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TE_FLAG_INDEX(&DMAtx))) {
    // You should not be here - DMA transfer error flag is set
    // Abort DMA transfer and release SPI
  }
  else {
    // Check if DMA transfer completed flag is set
    if (__HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TC_FLAG_INDEX(&DMAtx)) == 0) return true;
    // Check if SPI transmit butter is empty and SPI is idle
    if ((!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_TXE)) || (__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_BSY))) return true;
  }

  Abort();
  return false;
}

void TFT_SPI::Abort() {
  HAL_DMA_Abort(&DMAtx);  // Abort DMA transfer if any
  HAL_DMA_DeInit(&DMAtx);

  CLEAR_BIT(SPIx.Instance->CR2, SPI_CR2_TXDMAEN);

  DataTransferEnd();      // Stop SPI and deselect CS
}

void TFT_SPI::Transmit(uint16_t Data) {
  #if TFT_MISO_PIN == TFT_MOSI_PIN
    SPI_1LINE_TX(&SPIx);
  #endif

  __HAL_SPI_ENABLE(&SPIx);

  SPIx.Instance->DR = Data;

  while (!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_TXE)) {}
  while ( __HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_BSY)) {}

  #if TFT_MISO_PIN != TFT_MOSI_PIN
    __HAL_SPI_CLEAR_OVRFLAG(&SPIx);   // Clear overrun flag in 2 Lines communication mode because received data is not read
  #endif
}

void TFT_SPI::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  DMAtx.Init.MemInc = MemoryIncrease;
  HAL_DMA_Init(&DMAtx);

  #if TFT_MISO_PIN == TFT_MOSI_PIN
    SPI_1LINE_TX(&SPIx);
  #endif

  DataTransferBegin();

  HAL_DMA_Start(&DMAtx, (uint32_t)Data, (uint32_t)&(SPIx.Instance->DR), Count);
  __HAL_SPI_ENABLE(&SPIx);

  SET_BIT(SPIx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request

  TERN_(TFT_SHARED_IO, while (isBusy()));
}

void TFT_SPI::Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  DMAtx.Init.MemInc = MemoryIncrease;
  HAL_DMA_Init(&DMAtx);

  if (TFT_MISO_PIN == TFT_MOSI_PIN)
    SPI_1LINE_TX(&SPIx);

  DataTransferBegin();

  HAL_DMA_Start(&DMAtx, (uint32_t)Data, (uint32_t)&(SPIx.Instance->DR), Count);
  __HAL_SPI_ENABLE(&SPIx);

  SET_BIT(SPIx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request

  HAL_DMA_PollForTransfer(&DMAtx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
  Abort();
}

#if ENABLED(USE_SPI_DMA_TC)
  void TFT_SPI::TransmitDMA_IT(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {

    DMAtx.Init.MemInc = MemoryIncrease;
    HAL_DMA_Init(&DMAtx);

    if (TFT_MISO_PIN == TFT_MOSI_PIN)
      SPI_1LINE_TX(&SPIx);

    DataTransferBegin();

    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_DMA_Start_IT(&DMAtx, (uint32_t)Data, (uint32_t)&(SPIx.Instance->DR), Count);
    __HAL_SPI_ENABLE(&SPIx);

    SET_BIT(SPIx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request
  }

  extern "C" void DMA2_Stream3_IRQHandler(void) { TFT_SPI::DMA_IRQHandler(); }
#endif

#endif // HAS_SPI_TFT
#endif // HAL_STM32
