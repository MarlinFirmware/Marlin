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

uint8_t TFT_SPI::clkdiv_write;
#if PIN_EXISTS(TFT_MISO)
uint8_t TFT_SPI::clkdiv_read;
#endif

bool TFT_SPI::active_transfer;
bool TFT_SPI::active_dma;

uint8_t TFT_SPI::_GetClockDivider( uint32_t spibasefreq, uint32_t speed )
{
  if ( speed >= (spibasefreq / 2) )
  {
    return SPI_BAUDRATEPRESCALER_2;
  }
  else if ( speed >= (spibasefreq / 4) )
  {
    return SPI_BAUDRATEPRESCALER_4;
  }
  else if ( speed >= (spibasefreq / 8) )
  {
    return SPI_BAUDRATEPRESCALER_8;
  }
  else if ( speed >= (spibasefreq / 16) )
  {
    return SPI_BAUDRATEPRESCALER_16;
  }
  else if ( speed >= (spibasefreq / 32) )
  {
    return SPI_BAUDRATEPRESCALER_32;
  }
  else if ( speed >= (spibasefreq / 64) )
  {
    return SPI_BAUDRATEPRESCALER_64;
  }
  else if ( speed >= (spibasefreq / 128) )
  {
    return SPI_BAUDRATEPRESCALER_128;
  }
  else
  {
    return SPI_BAUDRATEPRESCALER_256;
  }
}

extern "C" {
  #include <utility/spi_com.h>
}

void TFT_SPI::Init() {

  SPI_TypeDef *spiInstance;

  OUT_WRITE(TFT_A0_PIN, HIGH);
  OUT_WRITE(TFT_CS_PIN, HIGH);

  if ((spiInstance = (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_SCK_PIN),  PinMap_SPI_SCLK)) == NP) return;

  if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_MOSI_PIN), PinMap_SPI_MOSI)) return;

  #if PIN_EXISTS(TFT_MISO) && TFT_MISO_PIN != TFT_MOSI_PIN
    if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TFT_MISO_PIN), PinMap_SPI_MISO)) return;
  #endif

  // SPI can have different baudrates depending on read or write functionality.

#if PIN_EXISTS(TFT_MISO)
  // static clkdiv_read variable.
  bool has_clkdiv_read = false;
#endif
  // static clkdiv_write variable.
  bool has_clkdiv_write = false;

#if (PIN_EXISTS(TFT_MISO) && defined(TFT_BAUDRATE_READ)) || defined(TFT_BAURDATE_WRITE)
    spi_t tmp_spi;
    tmp_spi.pin_sclk = digitalPinToPinName(TFT_SCK_PIN);
    uint32_t spibasefreq = spi_getClkFreq(&tmp_spi);

#if PIN_EXISTS(TFT_MISO) && TFT_BAUDRATE_READ
  clkdiv_read = _GetClockDivider(spibasefreq, TFT_BAUDRATE_READ);
  has_clkdiv_read = true;
#endif

#ifdef TFT_BAUDRATE_WRITE
  clkdiv_write = _GetClockDivider(spibasefreq, TFT_BAUDRATE_WRITE);
  has_clkdiv_write = true;
#endif

#endif

  if ( !has_clkdiv_write )
  {
#ifdef SPI1_BASE
    if (spiInstance == SPI1)
    {
      clkdiv_write = SPI_BAUDRATEPRESCALER_4;
      has_clkdiv_write = true;
    }
#endif

    if ( !has_clkdiv_write )
    {
      clkdiv_write = SPI_BAUDRATEPRESCALER_2;
    }
  }

#if PIN_EXISTS(TFT_MISO)
  if ( !has_clkdiv_read )
  {
    clkdiv_read = clkdiv_write;
  }
#endif

  SPIx.Instance                = spiInstance;
  SPIx.State                   = HAL_SPI_STATE_RESET;
  SPIx.Init.NSS                = SPI_NSS_SOFT;
  SPIx.Init.Mode               = SPI_MODE_MASTER;
  SPIx.Init.Direction          = (TFT_MISO_PIN != NC && TFT_MISO_PIN == TFT_MOSI_PIN) ? SPI_DIRECTION_1LINE : SPI_DIRECTION_2LINES;
  //SPIx.Init.BaudRatePrescaler  = clkdiv;
  SPIx.Init.CLKPhase           = SPI_PHASE_1EDGE;
  SPIx.Init.CLKPolarity        = SPI_POLARITY_LOW;
  //SPIx.Init.DataSize           = SPI_DATASIZE_8BIT;
  SPIx.Init.FirstBit           = SPI_FIRSTBIT_MSB;
  SPIx.Init.TIMode             = SPI_TIMODE_DISABLE;
  SPIx.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
  SPIx.Init.CRCPolynomial      = 10;

  DMAtx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  DMAtx.Init.PeriphInc = DMA_PINC_DISABLE;
  DMAtx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  DMAtx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DMAtx.Init.Mode = DMA_NORMAL;
  DMAtx.Init.Priority = DMA_PRIORITY_LOW;
  #ifdef STM32F4xx
    DMAtx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  #endif

  active_transfer = false;
  active_dma = false;
}

// Call before any HAL initialization (DMA or SPI).
void TFT_SPI::HALPrepare(eSPIMode spiMode)
{
  uint8_t clkdiv = 1;
#if PIN_EXISTS(TFT_MISO)
  if (spiMode == eSPIMode::READ)
  {
    clkdiv = clkdiv_read;
  }
#endif
  if (spiMode == eSPIMode::WRITE)
  {
    clkdiv = clkdiv_write;
  }

  SPIx.Init.BaudRatePrescaler = clkdiv;

  pinmap_pinout(digitalPinToPinName(TFT_SCK_PIN), PinMap_SPI_SCLK);
  pinmap_pinout(digitalPinToPinName(TFT_MOSI_PIN), PinMap_SPI_MOSI);
  #if PIN_EXISTS(TFT_MISO) && TFT_MISO_PIN != TFT_MOSI_PIN
    pinmap_pinout(digitalPinToPinName(TFT_MISO_PIN), PinMap_SPI_MISO);
  #endif
  //pinmap_pinout(digitalPinToPinName(TFT_CS_PIN), PinMap_SPI_SSEL);

  pin_PullConfig(get_GPIO_Port(STM_PORT(digitalPinToPinName(TFT_SCK_PIN))), STM_LL_GPIO_PIN(digitalPinToPinName(TFT_SCK_PIN)), GPIO_PULLDOWN);

  #ifdef SPI1_BASE
    if (SPIx.Instance == SPI1) {
      __HAL_RCC_SPI1_CLK_ENABLE();
      __HAL_RCC_SPI1_FORCE_RESET();
      __HAL_RCC_SPI1_RELEASE_RESET();
      #ifdef STM32F1xx
        __HAL_RCC_DMA1_CLK_ENABLE();
        DMAtx.Instance = DMA1_Channel3;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA2_CLK_ENABLE();
        DMAtx.Instance = DMA2_Stream3;
        DMAtx.Init.Channel = DMA_CHANNEL_3;
      #endif
    }
  #endif

  #ifdef SPI2_BASE
    if (SPIx.Instance == SPI2) {
      __HAL_RCC_SPI2_CLK_ENABLE();
      __HAL_RCC_SPI2_FORCE_RESET();
      __HAL_RCC_SPI2_RELEASE_RESET();
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
      __HAL_RCC_SPI3_FORCE_RESET();
      __HAL_RCC_SPI3_RELEASE_RESET();
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

  #ifdef SPI4_BASE
    if (SPIx.Instance == SPI4) {
      __HAL_RCC_SPI4_CLK_ENABLE();
      __HAL_RCC_SPI4_FORCE_RESET();
      __HAL_RCC_SPI4_RELEASE_RESET();
      #ifdef STM32F1xx
        __HAL_RCC_DMA2_CLK_ENABLE();
        DMAtx.Instance = DMA2_Channel4;
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA2_CLK_ENABLE();
        DMAtx.Instance = DMA2_Stream5;
        DMAtx.Init.Channel = DMA_CHANNEL_0;
      #endif
    }
  #endif
}

void TFT_SPI::HALDismantle(void)
{
  #ifdef SPI1_BASE
    if (SPIx.Instance == SPI1) {
      __HAL_RCC_SPI1_FORCE_RESET();
      __HAL_RCC_SPI1_RELEASE_RESET();
      __HAL_RCC_SPI1_CLK_DISABLE();
      #ifdef STM32F1xx
        __HAL_RCC_DMA1_CLK_DISABLE();
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA2_CLK_DISABLE();
      #endif
    }
  #endif

  #ifdef SPI2_BASE
    if (SPIx.Instance == SPI2) {
      __HAL_RCC_SPI2_FORCE_RESET();
      __HAL_RCC_SPI2_RELEASE_RESET();
      __HAL_RCC_SPI2_CLK_DISABLE();
      #ifdef STM32F1xx
        __HAL_RCC_DMA1_CLK_DISABLE();
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA1_CLK_DISABLE();
      #endif
    }
  #endif

  #ifdef SPI3_BASE
    if (SPIx.Instance == SPI3) {
      __HAL_RCC_SPI3_FORCE_RESET();
      __HAL_RCC_SPI3_RELEASE_RESET();
      __HAL_RCC_SPI3_CLK_DISABLE();
      #ifdef STM32F1xx
        __HAL_RCC_DMA2_CLK_DISABLE();
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA1_CLK_DISABLE();
      #endif
    }
  #endif

  #ifdef SPI4_BASE
    if (SPIx.Instance == SPI4) {
      __HAL_RCC_SPI4_FORCE_RESET();
      __HAL_RCC_SPI4_RELEASE_RESET();
      __HAL_RCC_SPI4_CLK_DISABLE();
      #ifdef STM32F1xx
        __HAL_RCC_DMA2_CLK_DISABLE();
      #elif defined(STM32F4xx)
        __HAL_RCC_DMA2_CLK_DISABLE();
      #endif
  #endif
}

void TFT_SPI::DataTransferBegin(uint16_t DataSize, eSPIMode spiMode) {
  HALPrepare(spiMode);
  SPIx.Init.DataSize = ( DataSize == DATASIZE_8BIT ) ?  SPI_DATASIZE_8BIT : SPI_DATASIZE_16BIT;
  HAL_SPI_Init(&SPIx);
  WRITE(TFT_CS_PIN, LOW);
  active_transfer = true;
}

void TFT_SPI::DataTransferEnd(void)
{
  // TODO: apply strong integrity to the active_transfer variable!
  HAL_SPI_DeInit(&SPIx);
  HALDismantle();
  WRITE(TFT_CS_PIN, HIGH);
  active_transfer = false;
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
    uint32_t i;

    DataTransferBegin(DATASIZE_8BIT, eSPIMode::READ);
    WriteReg(Reg);

    if (SPIx.Init.Direction == SPI_DIRECTION_1LINE) SPI_1LINE_RX(&SPIx);
    __HAL_SPI_ENABLE(&SPIx);

    for (i = 0; i < 4; i++) {
      #if TFT_MISO_PIN != TFT_MOSI_PIN
        //if (hspi->Init.Direction == SPI_DIRECTION_2LINES) {
          while (!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_TXE)) {}
          SPIx.Instance->DR = 0;
        //}
      #endif
      while (!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_RXNE)) {}
      Data = (Data << 8) | SPIx.Instance->DR;
    }

    __HAL_SPI_DISABLE(&SPIx);
    DataTransferEnd();
  #endif

  return Data >> 7;
}

bool TFT_SPI::isBusy() {
  if (active_dma)
  {
    #ifdef STM32F1xx
      volatile bool dmaEnabled = (DMAtx.Instance->CCR & DMA_CCR_EN) != RESET;
    #elif defined(STM32F4xx)
      volatile bool dmaEnabled = DMAtx.Instance->CR & DMA_SxCR_EN;
    #endif
  #if 0
    if (dmaEnabled) {
      if (__HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TC_FLAG_INDEX(&DMAtx)) != 0 || __HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TE_FLAG_INDEX(&DMAtx)) != 0)
        Abort();
    }
    else
      Abort();
  #endif
    return dmaEnabled;
  }
  else
  {
    #if 0
    Abort();
    #endif
    return false;
  }
}

void TFT_SPI::Abort() {
  if (active_transfer)
  {
    // Wait for any running spi
    while (!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_TXE)) { }
    while ( __HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_BSY)) { }
    if (active_dma)
    {
      // First, abort any running dma
      HAL_DMA_Abort(&DMAtx);
      HAL_DMA_PollForTransfer(&DMAtx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
      // DeInit objects
      HAL_DMA_DeInit(&DMAtx);
      active_dma = false;
    }
    // Deselect CS
    DataTransferEnd();
  }
}

void TFT_SPI::Transmit(uint16_t Data) {
#if PIN_EXISTS(TFT_MISO)
  if (TFT_MISO_PIN == TFT_MOSI_PIN)
    SPI_1LINE_TX(&SPIx);
#endif

  __HAL_SPI_ENABLE(&SPIx);

  SPIx.Instance->DR = Data;

  while (!__HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_TXE)) {}
  while ( __HAL_SPI_GET_FLAG(&SPIx, SPI_FLAG_BSY)) {}

#if PIN_EXISTS(TFT_MISO)
  if (TFT_MISO_PIN != TFT_MOSI_PIN)
    __HAL_SPI_CLEAR_OVRFLAG(&SPIx);   // Clear overrun flag in 2 Lines communication mode because received is not read
#endif
}

void TFT_SPI::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {

  // Wait last dma finish, to start another
  while (isBusy()) { /* nada */ }

#if PIN_EXISTS(TFT_MISO)
  if (TFT_MISO_PIN == TFT_MOSI_PIN)
    SPI_1LINE_TX(&SPIx);
#endif

  DataTransferBegin();

  DMAtx.Init.MemInc = MemoryIncrease;
  HAL_DMA_Init(&DMAtx);
  active_dma = true;

  HAL_DMA_Start(&DMAtx, (uint32_t)Data, (uint32_t)&(SPIx.Instance->DR), Count);

  __HAL_SPI_ENABLE(&SPIx);

  SET_BIT(SPIx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request

  HAL_DMA_PollForTransfer(&DMAtx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);

  Abort();
}

#if ENABLED(USE_SPI_DMA_TC)

  void TFT_SPI::TransmitDMA_IT(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {

#if PIN_EXISTS(TFT_MISO)
    if (TFT_MISO_PIN == TFT_MOSI_PIN)
      SPI_1LINE_TX(&SPIx);
#endif

    DataTransferBegin();

    DMAtx.Init.MemInc = MemoryIncrease;
    HAL_DMA_Init(&DMAtx);
    active_dma = true;

    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_DMA_Start_IT(&DMAtx, (uint32_t)Data, (uint32_t)&(SPIx.Instance->DR), Count);
    __HAL_SPI_ENABLE(&SPIx);

    SET_BIT(SPIx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request
  }

  extern "C" void DMA2_Stream3_IRQHandler(void) { HAL_DMA_IRQHandler(&TFT_SPI::DMAtx); }

#endif

#endif // HAS_SPI_TFT
#endif // HAL_STM32
