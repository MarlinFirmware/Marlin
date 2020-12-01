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

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_TFT

#include "tft_spi.h"
#include "pinconfig.h"

SPI_HandleTypeDef TFT_SPI::SPIx;
DMA_HandleTypeDef TFT_SPI::DMAtx;

void TFT_SPI::Init() {
  SPI_TypeDef *spiInstance;

  #if PIN_EXISTS(TFT_RESET)
    OUT_WRITE(TFT_RESET_PIN, HIGH);
    HAL_Delay(100);
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT)
    OUT_WRITE(TFT_BACKLIGHT_PIN, HIGH);
  #endif

  OUT_WRITE(TFT_A0_PIN, HIGH);
  OUT_WRITE(TFT_CS_PIN, HIGH);

  if ((spiInstance = (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_SCK_PIN),  PinMap_SPI_SCLK)) == NP) return;
  if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_MOSI_PIN), PinMap_SPI_MOSI)) return;

  #if PIN_EXISTS(TFT_MISO) && (TFT_MISO_PIN != TFT_MOSI_PIN)
    if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_MISO_PIN), PinMap_SPI_MISO)) return;
  #endif

  SPIx.Instance                = spiInstance;
  SPIx.State                   = HAL_SPI_STATE_RESET;
  SPIx.Init.NSS                = SPI_NSS_SOFT;
  SPIx.Init.Mode               = SPI_MODE_MASTER;
  SPIx.Init.Direction          =
  #if TFT_MISO_PIN == TFT_MOSI_PIN
                                 SPI_DIRECTION_1LINE;
  #else
                                 SPI_DIRECTION_2LINES;
  #endif
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
  #if PIN_EXISTS(TFT_MISO) && (TFT_MISO_PIN != TFT_MOSI_PIN)
    pinmap_pinout(digitalPinToPinName(TFT_MISO_PIN), PinMap_SPI_MISO);
  #endif
  pin_PullConfig(get_GPIO_Port(STM_PORT(digitalPinToPinName(TFT_SCK_PIN))), STM_LL_GPIO_PIN(digitalPinToPinName(TFT_SCK_PIN)), GPIO_PULLDOWN);

  #ifdef SPI1_BASE
    if (SPIx.Instance == SPI1) {
      __HAL_RCC_SPI1_CLK_ENABLE();
      __HAL_RCC_DMA1_CLK_ENABLE();
      SPIx.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_4;
      DMAtx.Instance = DMA1_Channel3;
    }
  #endif
  #ifdef SPI2_BASE
    if (SPIx.Instance == SPI2) {
      __HAL_RCC_SPI2_CLK_ENABLE();
      __HAL_RCC_DMA1_CLK_ENABLE();
      DMAtx.Instance = DMA1_Channel5;
    }
  #endif
  #ifdef SPI3_BASE
    if (SPIx.Instance == SPI3) {
      __HAL_RCC_SPI3_CLK_ENABLE();
      __HAL_RCC_DMA2_CLK_ENABLE();
      DMAtx.Instance = DMA2_Channel2;
    }
  #endif

  HAL_SPI_Init(&SPIx);

  DMAtx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  DMAtx.Init.PeriphInc = DMA_PINC_DISABLE;
  DMAtx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  DMAtx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DMAtx.Init.Mode = DMA_NORMAL;
  DMAtx.Init.Priority = DMA_PRIORITY_LOW;
}

void TFT_SPI::DataTransferBegin(uint16_t DataSize) {
  SPIx.Init.DataSize = DataSize == DATASIZE_8BIT ?  SPI_DATASIZE_8BIT : SPI_DATASIZE_16BIT;
  HAL_SPI_Init(&SPIx);
  WRITE(TFT_CS_PIN, LOW);
}

uint32_t TFT_SPI::GetID() {
  uint32_t id;
  id = ReadID(LCD_READ_ID);

  if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
    id = ReadID(LCD_READ_ID4);
  return id;
}

uint32_t TFT_SPI::ReadID(uint16_t Reg) {
  #if !PIN_EXISTS(TFT_MISO)
    return 0;
  #else
    uint32_t BaudRatePrescaler = SPIx.Init.BaudRatePrescaler;
    uint32_t i, Data = 0;

    SPIx.Init.BaudRatePrescaler = SPIx.Instance == SPI1 ? SPI_BAUDRATEPRESCALER_8 : SPI_BAUDRATEPRESCALER_4;
    DataTransferBegin(DATASIZE_8BIT);
    WriteReg(Reg);

    if (SPIx.Init.Direction == SPI_DIRECTION_1LINE) SPI_1LINE_RX(&SPIx);
    __HAL_SPI_ENABLE(&SPIx);

    for (i = 0; i < 4; i++) {
      #if TFT_MISO_PIN != TFT_MOSI_PIN
        //if (hspi->Init.Direction == SPI_DIRECTION_2LINES) {
          while ((SPIx.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE) {}
          SPIx.Instance->DR = 0;
        //}
      #endif
      while ((SPIx.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE) {}
      Data = (Data << 8) | SPIx.Instance->DR;
    }

    __HAL_SPI_DISABLE(&SPIx);
    DataTransferEnd();

    SPIx.Init.BaudRatePrescaler   = BaudRatePrescaler;

    return Data >> 7;
  #endif
}

bool TFT_SPI::isBusy() {
  if (DMAtx.Instance->CCR & DMA_CCR_EN)
    if (__HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TC_FLAG_INDEX(&DMAtx)) != 0 || __HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TE_FLAG_INDEX(&DMAtx)) != 0)
      Abort();
  return DMAtx.Instance->CCR & DMA_CCR_EN;
}

void TFT_SPI::Abort() {
  __HAL_DMA_DISABLE(&DMAtx);
  DataTransferEnd();
}

void TFT_SPI::Transmit(uint16_t Data) {
  #if TFT_MISO_PIN == TFT_MOSI_PIN
    SPI_1LINE_TX(&SPIx);
  #endif

  __HAL_SPI_ENABLE(&SPIx);

  SPIx.Instance->DR = Data;

  while ((SPIx.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE) {}
  while ((SPIx.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY) {}

  #if TFT_MISO_PIN != TFT_MOSI_PIN
    __HAL_SPI_CLEAR_OVRFLAG(&SPIx);   /* Clear overrun flag in 2 Lines communication mode because received is not read */
  #endif
}

void TFT_SPI::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  DMAtx.Init.MemInc = MemoryIncrease;
  HAL_DMA_Init(&DMAtx);

  DataTransferBegin();

  #if TFT_MISO_PIN == TFT_MOSI_PIN
    SPI_1LINE_TX(&SPIx);
  #endif

  DMAtx.DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << DMAtx.ChannelIndex);
  DMAtx.Instance->CNDTR = Count;
  DMAtx.Instance->CPAR = (uint32_t)&(SPIx.Instance->DR);
  DMAtx.Instance->CMAR = (uint32_t)Data;
  __HAL_DMA_ENABLE(&DMAtx);
  __HAL_SPI_ENABLE(&SPIx);

  SET_BIT(SPIx.Instance->CR2, SPI_CR2_TXDMAEN);   /* Enable Tx DMA Request */
}

#endif // HAS_SPI_TFT
