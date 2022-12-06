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

/*
  HAL SPI implementation by Martin Turski, company owner of EirDev
  Inclusion date: 24th of November, 2022
  Contact mail: turningtides@outlook.de
  ---

  Contact Martin if there is any grave SPI design or functionality issue.
  Include a link to the Marlin FW GitHub issue post. Otherwise the mail
  may be ignored. This implementation has been created specifically for the
  Marlin FW. It was made with performance and simplicity-of-maintenance in
  mind, while keeping all the SPI requirements in check.

  Original pull request: https://github.com/MarlinFirmware/Marlin/pull/24911
*/

#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

// ------------------------
// Public functions
// ------------------------

  // If properly ported, use fast HW SPI implementation originally found in STM32 tft_spi.cpp
#if !ENABLED(SOFTWARE_SPI) && (defined(STM32F1xx) || defined(STM32F4xx)) && !ENABLED(HALSPI_HW_GENERIC)

#include "pinconfig.h"

#include "../shared/ARM/HAL_NVIC.h"

  // ------------------------
  // Hardware SPI
  // STM32F1yyxx: https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
  // ------------------------

#ifdef STM32F1xx
  #include "stm32f1xx_hal.h"
#elif defined(STM32F4xx)
  #include "stm32f4xx_hal.h"
#else
  #error Fast HW SPI is currently only supported on STM32F1 and STM32F4 hardware.
#endif

#ifndef HALSPI_LOOPBEEP_TIMEOUT
  // Timeout in milliseconds (consider that interrupts will increase the time by a lot)
  #define HALSPI_LOOPBEEP_TIMEOUT 3000
#endif

  static SPI_HandleTypeDef SPIhx;
  static DMA_HandleTypeDef DMAhx;

  uint8_t _HAL_GetClockDivider( uint32_t spibasefreq, uint32_t speed )
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

  static int _spi_pin_sck = NC;
  static int _spi_pin_miso = NC;
  static int _spi_pin_mosi = NC;
  static int _spi_pin_cs = NC;

#ifdef HAL_SPI_SUPPORTS_ASYNC
  // We optimize this to be only one CPU-core only, no scheduler.
  // Only one SPI connection is supported at-a-time, current revision.
  static volatile bool _spi_busy = false;

  struct spiStatusInfo
  {
    bool async_busy = false;
    void (*asyncCompleteCallback)(void*) = nullptr;
    void *async_ud = nullptr;
  };
#if 0
#ifdef SPI1_BASE
  spiStatusInfo spiStatus_1;
#endif
#ifdef SPI2_BASE
  spiStatusInfo spiStatus_2;
#endif
#ifdef SPI3_BASE
  spiStatusInfo spiStatus_3;
#endif
#ifdef SPI4_BASE
  spiStatusInfo spiStatus_4;
#endif
#ifdef SPI5_BASE
  spiStatusInfo spiStatus_5;
#endif
#ifdef SPI6_BASE
  spiStatusInfo spiStatus_6;
#endif
#else
  spiStatusInfo __spi_status;
#endif

  static spiStatusInfo* SPI_GetRuntimeInfo( SPI_TypeDef *instance )
  {
#if 0
#ifdef SPI1_BASE
    if (instance == SPI1) {
      return spiStatus_1;
    }
#endif
#ifdef SPI2_BASE
    if (instance == SPI2) {
      return spiStatus_2;
    }
#endif
#ifdef SPI3_BASE
    if (instance == SPI3) {
      return spiStatus_3;
    }
#endif
#ifdef SPI4_BASE
    if (instance == SPI4) {
      return spiStatus_4;
    }
#endif
#ifdef SPI5_BASE
    if (instance == SPI5) {
      return spiStatus_5;
    }
#endif
#ifdef SPI6_BASE
    if (instance == SPI6) {
      return spiStatus_6;
    }
#endif
    return nullptr;
#else
    return &__spi_status;
#endif
  }

#if 0
  static DMA_Channel_TypeDef* DMA_Channel_GetConfig( DMA_TypeDef *DMA, unsigned int channelNum )
  {
#ifdef DMA1_BASE
    if (DMA == DMA1)
    {
  #ifdef DMA1_Channel1_BASE
      if (channelNum == 1)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA1_Channel1_BASE - DMA1_BASE) );
      }
  #endif
  #ifdef DMA1_Channel2_BASE
      if (channelNum == 2)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA1_Channel2_BASE - DMA1_BASE) );
      }
  #endif
  #ifdef DMA1_Channel3_BASE
      if (channelNum == 3)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA1_Channel3_BASE - DMA1_BASE) );
      }
  #endif
  #ifdef DMA1_Channel4_BASE
      if (channelNum == 4)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA1_Channel4_BASE - DMA1_BASE) );
      }
  #endif
  #ifdef DMA1_Channel5_BASE
      if (channelNum == 5)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA1_Channel5_BASE - DMA1_BASE) );
      }
  #endif
  #ifdef DMA1_Channel6_BASE
      if (channelNum == 6)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA1_Channel6_BASE - DMA1_BASE) );
      }
  #endif
  #ifdef DMA1_Channel7_BASE
      if (channelNum == 7)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA1_Channel7_BASE - DMA1_BASE) );
      }
  #endif
    }
#endif
#ifdef DMA2_BASE
    if (DMA == DMA2)
    {
  #ifdef DMA2_Channel1_BASE
      if (channelNum == 1)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA2_Channel1_BASE - DMA2_BASE) );
      }
  #endif
  #ifdef DMA2_Channel2_BASE
      if (channelNum == 2)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA2_Channel2_BASE - DMA2_BASE) );
      }
  #endif
  #ifdef DMA2_Channel3_BASE
      if (channelNum == 3)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA2_Channel3_BASE - DMA2_BASE) );
      }
  #endif
  #ifdef DMA2_Channel4_BASE
      if (channelNum == 4)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA2_Channel4_BASE - DMA2_BASE) );
      }
  #endif
  #ifdef DMA2_Channel5_BASE
      if (channelNum == 5)
      {
        return (DMA_Channel_TypeDef*)( (uint32_t)DMA + (DMA2_Channel5_BASE - DMA2_BASE) );
      }
  #endif
    }
#endif

    return nullptr;
  }
#endif

  // The interrupt handler.
  static void _DMA_Interrupt( void );

  static void NVIC_DMA_Interrupt_Enable(DMA_TypeDef *DMA, uint32_t channelNum) {
#ifdef DMA1_BASE
    if (DMA == DMA1) {
      if (channelNum >= 1 && channelNum <= 7) {
        // See page 204 of STM32F1xx ref manual (interrupt table).
        IRQn_Type inum = (IRQn_Type)( 10 + channelNum );

        HAL_NVIC_EnableIRQ(inum);
        HAL_NVIC_SetPriority(inum, 5, 0);
      }
    }
#endif
#ifdef DMA2_BASE
    if (DMA == DMA2) {
      if (channelNum >= 1 && channelNum <= 3) {
        // See page 204 of STM32F1xx ref manual (interrupt table).
        IRQn_Type inum = (IRQn_Type)( 55 + channelNum );

        HAL_NVIC_EnableIRQ(inum);
        HAL_NVIC_SetPriority(inum, 5, 0);
      }
      else if (channelNum == 4 || channelNum == 5) {
        HAL_NVIC_EnableIRQ((IRQn_Type)59);
        HAL_NVIC_SetPriority((IRQn_Type)59, 5, 0);
      }
    }
#endif
  }

  static void NVIC_DMA_Interrupt_Disable(DMA_TypeDef *DMA, uint32_t channelNum) {
#ifdef DMA1_BASE
    if (DMA == DMA1) {
      if (channelNum >= 1 && channelNum <= 7) {
        // See page 204 of STM32F1xx ref manual (interrupt table).
        IRQn_Type inum = (IRQn_Type)( 10 + channelNum );

        HAL_NVIC_SetPriority(inum, 255, 0);
        HAL_NVIC_DisableIRQ(inum);
      }
    }
#endif
#ifdef DMA2_BASE
    if (DMA == DMA2) {
      if (channelNum >= 1 && channelNum <= 3) {
        // See page 204 of STM32F1xx ref manual (interrupt table).
        IRQn_Type inum = (IRQn_Type)( 55 + channelNum );

        HAL_NVIC_SetPriority(inum, 255, 0);
        HAL_NVIC_DisableIRQ(inum);
      }
      else if (channelNum == 4 || channelNum == 5) {
        HAL_NVIC_SetPriority((IRQn_Type)59, 255, 0);
        HAL_NVIC_DisableIRQ((IRQn_Type)59);
      }
    }
#endif
  }

  static void DMA_Channel_TXCompleteInterruptSet( DMA_Channel_TypeDef *DMAch, bool enable )
  {
    if (enable)
      DMAch->CCR |= 0x02;
    else
      DMAch->CCR &= ~0x02;
  }
  static void DMA_Channel_TXErrorInterruptSet( DMA_Channel_TypeDef *DMAch, bool enable )
  {
    if (enable)
      DMAch->CCR |= 0x04;
    else
      DMAch->CCR &= ~0x04;
  }
#if 0
  static void DMA_Channel_Enable( DMA_Channel_TypeDef *DMAch, bool enable )
  {
    if (enable)
      DMAch->CCR |= 0x01;
    else
      DMAch->CCR &= ~0x01;
  }
#endif

  static void DMA_Channel_ResetTXComplete( DMA_TypeDef *DMA, unsigned int channelNum )
  {
    CBI(DMA->IFCR, ( 1 + (4 * (channelNum - 1)) ));
  }
  static void DMA_Channel_ResetTXError( DMA_TypeDef *DMA, unsigned int channelNum )
  {
    CBI(DMA->IFCR, ( 2 + (4 * (channelNum - 1)) ));
  }

  static bool DMA_Channel_TXComplete( DMA_TypeDef *DMA, unsigned int channelNum )
  {
    return TEST(DMA->ISR, ( 1 + (4 * (channelNum - 1)) ));
  }
  static bool DMA_Channel_TXError( DMA_TypeDef *DMA, unsigned int channelNum )
  {
    return TEST(DMA->ISR, ( 2 + (4 * (channelNum - 1)) ));
  }
#endif //HAL_SPI_SUPPORTS_ASYNC

  static bool _HAL_SPI_InitClock(int _spi_clkmode) {
    bool changed = false;
    if (_spi_clkmode == SPI_CLKMODE_0 || _spi_clkmode == SPI_CLKMODE_1) {
      if (SPIhx.Init.CLKPolarity != SPI_POLARITY_LOW) {
        SPIhx.Init.CLKPolarity = SPI_POLARITY_LOW;
        changed = true;
      }
    }
    else {
      if (SPIhx.Init.CLKPolarity != SPI_POLARITY_HIGH) {
        SPIhx.Init.CLKPolarity = SPI_POLARITY_HIGH;
        changed = true;
      }
    }

    if (_spi_clkmode == SPI_CLKMODE_0 || _spi_clkmode == SPI_CLKMODE_2) {
      if (SPIhx.Init.CLKPhase != SPI_PHASE_1EDGE) {
        SPIhx.Init.CLKPhase = SPI_PHASE_1EDGE;
        changed = true;
      }
    }
    else {
      if (SPIhx.Init.CLKPhase != SPI_PHASE_2EDGE) {
        SPIhx.Init.CLKPhase = SPI_PHASE_2EDGE;
        changed = true;
      }
    }

    return changed;
  }

  struct spi_monitored_loop
  {
  private:
#if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
    uint32_t _start_millis;
#endif
  public:
    inline spi_monitored_loop() {
#if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
      _start_millis = millis();
#endif
    }
    inline void update(unsigned int beep_code) {
#if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
      if ((millis() - _start_millis) <= HALSPI_LOOPBEEP_TIMEOUT) return;
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(500);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(1000);
      for (unsigned int n = 0; n < beep_code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(200);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(200);
      }
      delay(800);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(1000);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(2000);
#endif
    }
  };

  static void _spiOnError(unsigned int beep_code = 0) {
    for (;;) {
#if defined(HALSPI_DO_ERRORBEEPS) && PIN_EXISTS(BEEPER)
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(500);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(1000);
      for (unsigned int n = 0; n < beep_code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(200);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(200);
      }
      delay(800);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(1000);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(2000);
#endif
    }
  }

#if !ENABLED(HAL_SPI_SUPPORTS_ASYNC)
  static bool _spi_debug_inside_transaction = false;
#endif

  static void _SPI_Enter( SPI_TypeDef *instance ) {
#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
    spi_monitored_loop lw;
    // During testing, this loop did take longer than 1 second on the
    // MKS Robin E3D V1.1 (rarely).
    while (_spi_busy) { lw.update(12); }
    _spi_busy = true;
#else
    if (_spi_debug_inside_transaction)
      _spiOnError(11);
    _spi_debug_inside_transaction = true;
#endif
  }

  static void _SPI_Leave( SPI_TypeDef *instance ) {
#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
    _spi_busy = false;
#else
    if (!_spi_debug_inside_transaction)
      _spiOnError(12);
    _spi_debug_inside_transaction = false;
#endif
  }

  static void _SPI_AsyncBarrier() {
#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
    auto *spiInfo = SPI_GetRuntimeInfo(SPIhx.Instance);

    // Wait until any currently configured transfer has finished.
    spi_monitored_loop lw;
    while (spiInfo->async_busy) { lw.update(10); }
#endif
  }

  void spiBegin() {
    #if PIN_EXISTS(SD_SS)
      OUT_WRITE(SD_SS_PIN, HIGH);
    #endif

#if !(defined(STM32F1xx) || defined(STM32F4xx))
    memset(&SPIhx, 0, sizeof(SPIhx));
#endif
    //SPIhx.Instance                = spiInstance;
    SPIhx.State                   = HAL_SPI_STATE_RESET;
    SPIhx.Init.NSS                = SPI_NSS_SOFT;
    SPIhx.Init.Mode               = SPI_MODE_MASTER;
    //SPIhx.Init.Direction          = (TFT_MISO_PIN != NC && TFT_MISO_PIN == TFT_MOSI_PIN) ? SPI_DIRECTION_1LINE : SPI_DIRECTION_2LINES;
    //SPIhx.Init.BaudRatePrescaler  = clkdiv;
    //SPIhx.Init.CLKPhase           = SPI_PHASE_1EDGE;
    //SPIhx.Init.CLKPolarity        = SPI_POLARITY_LOW;
    _HAL_SPI_InitClock(SPI_CLKMODE_DEFAULT);
    //SPIhx.Init.DataSize           = SPI_DATASIZE_8BIT;
    SPIhx.Init.FirstBit           = SPI_FIRSTBIT_MSB;  // default.
    SPIhx.Init.TIMode             = SPI_TIMODE_DISABLE;
    SPIhx.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    SPIhx.Init.CRCPolynomial      = 10;

#if !(defined(STM32F1xx) || defined(STM32F4xx))
    memset(&DMAhx, 0, sizeof(DMAhx));
#endif
    DMAhx.Init.Direction = DMA_MEMORY_TO_PERIPH;  // target is SPI bus.
    //DMAhx.Init.MemInc = DMA_MINC_ENABLE;
    DMAhx.Init.PeriphInc = DMA_PINC_DISABLE;
    //DMAhx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    //DMAhx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    DMAhx.Init.Mode = DMA_NORMAL;
    DMAhx.Init.Priority = DMA_PRIORITY_LOW;
    #ifdef STM32F4xx
      DMAhx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    #endif
  }

  static void _HAL_SPI_Prepare(void) {
    auto name_sck_pin = digitalPinToPinName(_spi_pin_sck);

    pinmap_pinout(name_sck_pin, PinMap_SPI_SCLK);
    pinmap_pinout(digitalPinToPinName(_spi_pin_mosi), PinMap_SPI_MOSI);
    if (_spi_pin_miso != NC && _spi_pin_miso != _spi_pin_mosi) {
      pinmap_pinout(digitalPinToPinName(_spi_pin_miso), PinMap_SPI_MISO);
    }
    //pinmap_pinout(digitalPinToPinName(TFT_CS_PIN), PinMap_SPI_SSEL);

    pin_PullConfig(get_GPIO_Port(STM_PORT(name_sck_pin)), STM_LL_GPIO_PIN(name_sck_pin), GPIO_PULLDOWN);

    // See page 282 of the STM32F1xx manual (DMA1).

#if 0
    // @thinkyhead
    #ifdef STM32F1xx
      #define _DMATX_PREPARE(N1,N4,S,C1,C4) \
        __HAL_RCC_DMA##N1##_CLK_ENABLE(); \
        DMAtx.Instance = DMA##N1##_Channel##C1;
    #elif defined(STM32F4xx)
      #define _DMATX_PREPARE(N1,N4,S,C1,C4) \
        __HAL_RCC_DMA##N4##_CLK_ENABLE(); \
        DMAtx.Instance = DMA##N4##_Stream##S; \
        DMAtx.Init.Channel = DMA_CHANNEL_##C4;
    #else
      #define _DMATX_PREPARE(...) NOOP
    #endif
    #define SPIX_PREPARE(I,N1,N4,S,C1,C4) \
      if (SPIx.Instance == SPI##I) { \
        __HAL_RCC_SPI##I##_CLK_ENABLE(); \
        __HAL_RCC_SPI##I##_FORCE_RESET(); \
        __HAL_RCC_SPI##I##_RELEASE_RESET(); \
        _DMATX_PREPARE(N1,N4,S,C1,C4); \
      }
#endif

    #ifdef SPI1_BASE
      if (SPIhx.Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        #ifdef STM32F1xx
          #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
            // See page 204 of the STM32F1xx manual (interrupt table).
            nvicSetIRQHandler(DMA1_Channel3_IRQn, _DMA_Interrupt);
            nvicInstallRedirect();
            NVIC_DMA_Interrupt_Enable(DMA1, 3);
          #endif
          __HAL_RCC_DMA1_CLK_ENABLE();
          DMAhx.Instance = DMA1_Channel3;
        #elif defined(STM32F4xx)
          __HAL_RCC_DMA2_CLK_ENABLE();
          DMAhx.Instance = DMA2_Stream3;
          DMAhx.Init.Channel = DMA_CHANNEL_3;
        #endif
      }
    #endif

    #ifdef SPI2_BASE
      if (SPIhx.Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
        #ifdef STM32F1xx
          #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
            // See page 204 of the STM32F1xx manual (interrupt table).
            nvicSetIRQHandler(DMA1_Channel5_IRQn, _DMA_Interrupt);
            nvicInstallRedirect();
            NVIC_DMA_Interrupt_Enable(DMA1, 5);
          #endif
          __HAL_RCC_DMA1_CLK_ENABLE();
          DMAhx.Instance = DMA1_Channel5;
        #elif defined(STM32F4xx)
          __HAL_RCC_DMA1_CLK_ENABLE();
          DMAhx.Instance = DMA1_Stream4;
          DMAhx.Init.Channel = DMA_CHANNEL_0;
        #endif
      }
    #endif

    #ifdef SPI3_BASE
      if (SPIhx.Instance == SPI3) {
        __HAL_RCC_SPI3_CLK_ENABLE();
        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
        #ifdef STM32F1xx
          #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
            // See page 204 of the STM32F1xx manual (interrupt table).
            nvicSetIRQHandler(DMA2_Channel2_IRQn, _DMA_Interrupt);
            nvicInstallRedirect();
            NVIC_DMA_Interrupt_Enable(DMA2, 2);
          #endif
          __HAL_RCC_DMA2_CLK_ENABLE();
          DMAhx.Instance = DMA2_Channel2;
        #elif defined(STM32F4xx)
          __HAL_RCC_DMA1_CLK_ENABLE();
          DMAhx.Instance = DMA1_Stream5;
          DMAhx.Init.Channel = DMA_CHANNEL_0;
        #endif
      }
    #endif

    // TODO: there may be more than 4 SPI instances; take a look into remaining reference
    // manuals to see which ones can be enabled aswell.
  }

  static void _HAL_SPI_Dismantle(void) {
#if 0
    // @thinkyhead
    #ifdef STM32F1xx
      #define _DMATX_DISMANTLE(N1,N4) __HAL_RCC_DMA##N1##_CLK_DISABLE()
    #elif defined(STM32F4xx)
      #define _DMATX_DISMANTLE(N1,N4) __HAL_RCC_DMA##N4##_CLK_DISABLE()
    #else
      #define _DMATX_DISMANTLE(...) NOOP
    #endif
    #define SPIX_DISMANTLE(I,N1,N4) \
      if (SPIx.Instance == SPI##I) { \
        __HAL_RCC_SPI##I##_FORCE_RESET(); \
        __HAL_RCC_SPI##I##_RELEASE_RESET(); \
        __HAL_RCC_SPI##I##_CLK_DISABLE(); \
        _DMATX_DISMANTLE(N1,N4); \
      }
#endif

    #ifdef SPI1_BASE
      if (SPIhx.Instance == SPI1) {
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
        __HAL_RCC_SPI1_CLK_DISABLE();
        #ifdef STM32F1xx
          __HAL_RCC_DMA1_CLK_DISABLE();
          #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
            NVIC_DMA_Interrupt_Disable(DMA1, 3);
            // Restore the interrupt table entry.
            nvicUninstallRedirect();
          #endif
        #elif defined(STM32F4xx)
          __HAL_RCC_DMA2_CLK_DISABLE();
        #endif
      }
    #endif

    #ifdef SPI2_BASE
      if (SPIhx.Instance == SPI2) {
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
        __HAL_RCC_SPI2_CLK_DISABLE();
        #ifdef STM32F1xx
          __HAL_RCC_DMA1_CLK_DISABLE();
          #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
            NVIC_DMA_Interrupt_Disable(DMA1, 5);
            // Restore the interrupt table entry.
            nvicUninstallRedirect();
          #endif
        #elif defined(STM32F4xx)
          __HAL_RCC_DMA1_CLK_DISABLE();
        #endif
      }
    #endif

    #ifdef SPI3_BASE
      if (SPIhx.Instance == SPI3) {
        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
        __HAL_RCC_SPI3_CLK_DISABLE();
        #ifdef STM32F1xx
          __HAL_RCC_DMA2_CLK_DISABLE();
          #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
            NVIC_DMA_Interrupt_Disable(DMA2, 2);
            // Restore the interrupt table entry.
            nvicUninstallRedirect();
          #endif
        #elif defined(STM32F4xx)
          __HAL_RCC_DMA1_CLK_DISABLE();
        #endif
      }
    #endif
  }

  static void __HAL_SPI_ConfigDirection() {
    if (_spi_pin_miso == NC)
      SPI_1LINE_TX(&SPIhx);
    else if (_spi_pin_mosi == NC)
      SPI_1LINE_RX(&SPIhx);
  }

  static void _spiSecureCSEnter() {
    if (_spi_pin_cs != NC) {
      WRITE(_spi_pin_cs, HIGH);
    }
  }

  static void _spiSecureCSLeave() {
    if (_spi_pin_cs != NC) {
      WRITE(_spi_pin_cs, LOW);
    }
  }

  static bool _spi_is_transmit_sequence_initialized = false;

  static void _spiSecureShutdown() {
    _SPI_AsyncBarrier();

    if (_spi_is_transmit_sequence_initialized) {
      // Wait atleast until the hardware has accepted our transmission data item.
      // Then we know that the BSY flag is valid.
      spi_monitored_loop lw;
      while (!__HAL_SPI_GET_FLAG(&SPIhx, SPI_FLAG_TXE)) { lw.update(1); }
    }

    spi_monitored_loop lw;
    while ( __HAL_SPI_GET_FLAG(&SPIhx, SPI_FLAG_BSY)) { lw.update(2); }

    _spiSecureCSEnter();

    __HAL_SPI_DISABLE(&SPIhx);

    // Same status as just calling _spiSecureCSEnter.
  }

  static bool _spi_is_initialized = false;

  static void _HAL_SPI_UpdateTransaction(uint16_t dataSize) {
    bool requires_spi_reboot = false;

    if (_spi_is_initialized == false) {
      SPIhx.Init.DataSize = dataSize;
      HAL_SPI_Init(&SPIhx);
      _spi_is_initialized = true;
      requires_spi_reboot = true;
    }
    else if (SPIhx.Init.DataSize != dataSize) {
      _spiSecureShutdown();
      HAL_SPI_DeInit(&SPIhx);
      SPIhx.Init.DataSize = dataSize;
      HAL_SPI_Init(&SPIhx);
      requires_spi_reboot = true;
    }

    if (requires_spi_reboot) {
      __HAL_SPI_ConfigDirection();
      __HAL_SPI_ENABLE(&SPIhx);

      _spi_is_transmit_sequence_initialized = false;

      _spiSecureCSLeave();
    }
  }

  static void _HAL_SPI_EndOfTransaction() {
    if (_spi_is_initialized) {
      HAL_SPI_DeInit(&SPIhx);
      _spi_is_initialized = false;
    }
  }

  void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    int use_pin_sck, use_pin_miso, use_pin_mosi, use_pin_cs;
    if (hint_sck != NC && hint_mosi != NC)
    {
      use_pin_sck = hint_sck;
      use_pin_miso = hint_miso;
      use_pin_mosi = hint_mosi;
    }
    else
    {
      use_pin_sck = SD_SCK_PIN;
      use_pin_miso = SD_MISO_PIN;
      use_pin_mosi = SD_MOSI_PIN;
    }
    use_pin_cs = hint_cs;

    PinName name_sck_pin = digitalPinToPinName(use_pin_sck);

    SPI_TypeDef *spiInstance = (SPI_TypeDef*)pinmap_peripheral(name_sck_pin, PinMap_SPI_SCLK);

    if (spiInstance != pinmap_peripheral(digitalPinToPinName(use_pin_mosi), PinMap_SPI_MOSI))
      _spiOnError();
    if (use_pin_miso != NC && spiInstance != pinmap_peripheral(digitalPinToPinName(use_pin_miso), PinMap_SPI_MISO))
      _spiOnError();

    // Barrier.
    _SPI_Enter( spiInstance );

    _spi_pin_sck = use_pin_sck;
    _spi_pin_miso = use_pin_miso;
    _spi_pin_mosi = use_pin_mosi;
    _spi_pin_cs = use_pin_cs;

    SPIhx.Instance = spiInstance;

    // chip-select is assumed to be handled by the software to be LOW during operations (if passed as -1 to spiInitEx)
    // the pin is being adjusted to HIGH when SPI is doing critical maintenance operations
    // (changing frame width, bit-ordering, etc).

    SPIhx.Init.Direction = (_spi_pin_miso == NC || _spi_pin_mosi == NC) ? SPI_DIRECTION_1LINE : SPI_DIRECTION_2LINES;

    uint8_t clkdiv;
    if (maxClockFreq > 0) {
      spi_t tmp_spi;
      tmp_spi.pin_sclk = name_sck_pin;
      uint32_t spibasefreq = spi_getClkFreq(&tmp_spi);
      clkdiv = _HAL_GetClockDivider( spibasefreq, maxClockFreq );
    }
    else
    {
      bool has_clkdiv = false;

#ifdef SPI1_BASE
      if (spiInstance == SPI1)
      {
        clkdiv = SPI_BAUDRATEPRESCALER_4;
        has_clkdiv = true;
      }
#endif

      if (!has_clkdiv)
      {
        clkdiv = SPI_BAUDRATEPRESCALER_2;
      }
    }

    SPIhx.Init.BaudRatePrescaler = clkdiv;

    _HAL_SPI_Prepare();

    _spi_is_initialized = false;
    _spi_is_transmit_sequence_initialized = false;
  }

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
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
    spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  void spiClose() {
    // Shutdown the SPI peripheral.
    _spiSecureShutdown();

    // Close down hardware-related resources.
    _HAL_SPI_EndOfTransaction();
    _HAL_SPI_Dismantle();
    _SPI_Leave( SPIhx.Instance );
  }

  void spiSetBitOrder(int bitOrder) {
    if (bitOrder == SPI_BITORDER_MSB) {
      if (SPIhx.Init.FirstBit == SPI_FIRSTBIT_MSB) return;
      SPIhx.Init.FirstBit = SPI_FIRSTBIT_MSB;
    }
    else if (bitOrder == SPI_BITORDER_LSB) {
      if (SPIhx.Init.FirstBit == SPI_FIRSTBIT_LSB) return;
      SPIhx.Init.FirstBit = SPI_FIRSTBIT_LSB;
    }
  
    // If the SPI is already enabled, then update it.
    // When the SPI is initialized, the chip-select must be high aswell.
    if (_spi_is_initialized)
    {
      _spiSecureShutdown();

      auto tmp_SPI_CR1 = SPIhx.Instance->CR1;

      if (bitOrder == SPI_BITORDER_MSB)
        tmp_SPI_CR1 &= ~_BV(7);
      else
        tmp_SPI_CR1 |= _BV(7);

      SPIhx.Instance->CR1 = tmp_SPI_CR1;

      __HAL_SPI_ConfigDirection();
      __HAL_SPI_ENABLE(&SPIhx);

      _spi_is_transmit_sequence_initialized = false;

      _spiSecureCSLeave();
    }
  }

  void spiSetClockMode(int mode) {
    bool changed = _HAL_SPI_InitClock(mode);

    if (!changed) return;

    if (_spi_is_initialized) {
      _spiSecureShutdown();

      auto tmp_SPI_CR1 = SPIhx.Instance->CR1;

      if (mode == SPI_CLKMODE_0) {
        tmp_SPI_CR1 &= ~0x03;
      }
      else if (mode == SPI_CLKMODE_1) {
        tmp_SPI_CR1 &= ~0x02;
        tmp_SPI_CR1 |= 0x01;
      }
      else if (mode == SPI_CLKMODE_2) {
        tmp_SPI_CR1 &= ~0x01;
        tmp_SPI_CR1 |= 0x02;
      }
      else if (mode == SPI_CLKMODE_3) {
        tmp_SPI_CR1 |= 0x03;
      }

      SPIhx.Instance->CR1 = tmp_SPI_CR1;

      __HAL_SPI_ConfigDirection();
      __HAL_SPI_ENABLE(&SPIhx);

      _spi_is_transmit_sequence_initialized = false;

      _spiSecureCSLeave();
    }
  }

  void spiEstablish() {
    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_8BIT);
  }

#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
  static void _DMA_Interrupt( void )
  {
    DMA_TypeDef *DMA = DMAhx.DmaBaseAddress;
    uint32_t channelIndex = ((DMAhx.ChannelIndex>>2)+1);

    // DMA completed successfully?
    if (DMA_Channel_TXComplete(DMA, channelIndex)) {

      // Reset TCIF.
      DMA_Channel_ResetTXComplete(DMA, channelIndex);

      // Disable the TX Complete interrupt.
      DMA_Channel_TXCompleteInterruptSet(DMAhx.Instance, false);

      auto cb = DMAhx.XferCpltCallback;

      // Terminate the peripherals.
      HAL_DMA_DeInit( &DMAhx );

      // Wait until the SPI is finished.
      spi_monitored_loop lw;
      while ( __HAL_SPI_GET_FLAG(&SPIhx, SPI_FLAG_BSY)) { lw.update(3); }

      __HAL_SPI_CLEAR_OVRFLAG(&SPIhx);

      auto *info = SPI_GetRuntimeInfo( SPIhx.Instance );
      info->async_busy = false;

      // Execute any transfer-complete callback.
      if (cb)
      {
        cb( &DMAhx );
      }
    }
    // Error during transfer?
    else if (DMA_Channel_TXError(DMA, channelIndex)) {

      // Reset TEIF.
      DMA_Channel_ResetTXError(DMA, channelIndex);

      // Disable the TX error interrupt.
      DMA_Channel_TXErrorInterruptSet(DMAhx.Instance, false);

      // Terminate the peripherals.
      HAL_DMA_DeInit( &DMAhx );

      _HAL_SPI_Dismantle();

      auto *info = SPI_GetRuntimeInfo( SPIhx.Instance );
      info->async_busy = false;
    }
  }
#endif

  static void _HAL_SPI_CheckTransactionalError() {
    if (TEST(SPIhx.Instance->SR, 3)) {
      // Underrun.
      _spiOnError(1);
    }
    if (TEST(SPIhx.Instance->SR, 4)) {
      // CRC error.
      _spiOnError(2);
    }
    if (TEST(SPIhx.Instance->SR, 5)) {
      // Mode fault.
      _spiOnError(3);
    }
    if (TEST(SPIhx.Instance->SR, 6)) {
      // Overrun.
      _spiOnError(4);
    }
    if (TEST(SPIhx.Instance->CR1, 6) == false) {
      // SPI disabled???
      _spiOnError(10);
    }
  }

  // Operates in both 8bit and 16bit modes.
  template <typename transmitType>
  static void _HAL_SPI_TransmitOnly(transmitType val) {

    __IO transmitType *DR = (__IO transmitType*)&SPIhx.Instance->DR;

    if (_spi_is_transmit_sequence_initialized)
    {
      spi_monitored_loop lw;
      while (!__HAL_SPI_GET_FLAG(&SPIhx, SPI_FLAG_TXE)) { lw.update(4); }
    }

    *DR = val;

    _spi_is_transmit_sequence_initialized = true;
  }

  template <typename transmitType>
  static void _HAL_SPI_Transmit(transmitType val) {
    _HAL_SPI_TransmitOnly <transmitType> ( val );

#ifndef SPI_ENABLE_OPTIMIZATIONS
    // Perform the regular transactional pattern.
    if (SPIhx.Init.Direction == SPI_DIRECTION_2LINES) {
      __IO transmitType *DR = (__IO transmitType*)&SPIhx.Instance->DR;

      spi_monitored_loop lw;
      while (!__HAL_SPI_GET_FLAG(&SPIhx, SPI_FLAG_RXNE)) { lw.update(5); }

      transmitType tmp = *DR;
      (void)tmp;
    }
#else
    // Do some OVRFLAG stuff that is regularly found on the internet.
    if (SPIhx.Init.Direction == SPI_DIRECTION_2LINES)
      __HAL_SPI_CLEAR_OVRFLAG(&SPIhx);   // Clear overrun flag in 2 Lines communication mode because received data is not read
#endif

    _HAL_SPI_CheckTransactionalError();
  }

  template <typename transmitType>
  static transmitType _HAL_SPI_Receive(transmitType txval) {

    __IO transmitType *DR = (__IO transmitType*)&SPIhx.Instance->DR;

    if (SPIhx.Init.Direction == SPI_DIRECTION_2LINES)
    {
      _HAL_SPI_TransmitOnly <transmitType> ( txval );
    }

    spi_monitored_loop lw;
    while (!__HAL_SPI_GET_FLAG(&SPIhx, SPI_FLAG_RXNE)) { lw.update(6); }

    transmitType result = *DR;

    _HAL_SPI_CheckTransactionalError();

    return result;
  }

  void spiSend(uint8_t b) {
    if (_spi_pin_mosi == NC) return;
    _SPI_AsyncBarrier();
    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_8BIT);
    _HAL_SPI_Transmit <uint8_t> (b);
  }

  void spiSend16(uint16_t v) {
    if (_spi_pin_mosi == NC) return;
    _SPI_AsyncBarrier();
    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_16BIT);
    _HAL_SPI_Transmit <uint16_t> (v);
  }

  uint8_t spiRec(uint8_t txval) {
    if (_spi_pin_miso == NC) return 0;
    _SPI_AsyncBarrier();
    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_8BIT);
    uint8_t res = _HAL_SPI_Receive <uint8_t> (txval);
    return res;
  }

  uint16_t spiRec16(uint16_t txval) {
    if (_spi_pin_miso == NC) return 0;
    _SPI_AsyncBarrier();
    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_16BIT);
    uint16_t res = _HAL_SPI_Receive <uint16_t> (txval);
    return res;
  }

  void spiRead(uint8_t *buf, uint16_t bufsize, uint8_t txval) {
    if (_spi_pin_miso == NC) {
      memset(buf, 0, bufsize);
    }
    else {
      _SPI_AsyncBarrier();
      _HAL_SPI_UpdateTransaction(SPI_DATASIZE_8BIT);
      for (uint16_t n = 0; n < bufsize; n++)
        buf[n] = _HAL_SPI_Receive <uint8_t> (txval);
    }
  }

  void spiSendBlock(uint8_t token, const uint8_t *block) {
    if (_spi_pin_mosi == NC) return;
    _SPI_AsyncBarrier();
    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_8BIT);
    _HAL_SPI_Transmit <uint8_t> (token);
    for (uint16_t n = 0; n < 512; n++)
      _HAL_SPI_Transmit <uint8_t> ( block[n] );
  }

  static void _HAL_DMA_ConfigureTransfer( bool memInc, uint16_t dataSize ) {
    DMAhx.Init.MemInc = ( memInc ? DMA_MINC_ENABLE : DMA_MINC_DISABLE ); // if true send one number many times, else an array.
    DMAhx.Init.PeriphDataAlignment = (dataSize == SPI_DATASIZE_8BIT) ? DMA_PDATAALIGN_BYTE : DMA_PDATAALIGN_HALFWORD;
    DMAhx.Init.MemDataAlignment = (dataSize == SPI_DATASIZE_8BIT) ? DMA_MDATAALIGN_BYTE : DMA_MDATAALIGN_HALFWORD;
    HAL_DMA_Init(&DMAhx);
  }

  // Blocking.
  static void _HAL_DMA_Transfer( const void *buf, uint32_t bufcnt, bool memInc, uint16_t dataSize ) {
    _HAL_SPI_UpdateTransaction(dataSize);

    _HAL_DMA_ConfigureTransfer(memInc, dataSize);

    HAL_DMA_Start(&DMAhx, (uint32_t)buf, (uint32_t)&(SPIhx.Instance->DR), bufcnt);

    SET_BIT(SPIhx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request

    HAL_DMA_PollForTransfer(&DMAhx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);

    HAL_DMA_DeInit(&DMAhx);

    spi_monitored_loop lw;
    while ( __HAL_SPI_GET_FLAG(&SPIhx, SPI_FLAG_BSY)) { lw.update(8); }

    // Since we have only transmitted data it is sure that we got the OVRFLAG.
    // Thus clear it.
    if (SPIhx.Init.Direction == SPI_DIRECTION_2LINES && TEST(SPIhx.Instance->SR, 6))
      __HAL_SPI_CLEAR_OVRFLAG(&SPIhx);

    _spi_is_transmit_sequence_initialized = true;
  }

  void spiWrite(const uint8_t *buf, uint16_t bufcnt) {
    if (_spi_pin_mosi == NC) return;
    if (bufcnt == 0) return;
    _SPI_AsyncBarrier();
    _HAL_DMA_Transfer( buf, bufcnt, true, SPI_DATASIZE_8BIT );
  }

  void spiWrite16(const uint16_t *buf, uint16_t bufcnt) {
    if (_spi_pin_mosi == NC) return;
    if (bufcnt == 0) return;
    _SPI_AsyncBarrier();
    _HAL_DMA_Transfer( buf, bufcnt, true, SPI_DATASIZE_16BIT );
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    if (_spi_pin_mosi == NC) return;
    if (repcnt == 0) return;
    _SPI_AsyncBarrier();
    // If async then we would need a static variable here.
    _HAL_DMA_Transfer( &val, repcnt, false, SPI_DATASIZE_8BIT );
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    if (_spi_pin_mosi == NC) return;
    if (repcnt == 0) return;
    _SPI_AsyncBarrier();
    // If async then we would need a static variable here.
    _HAL_DMA_Transfer( &val, repcnt, false, SPI_DATASIZE_16BIT );
  }

#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)

  static void _DMA_CompleteCallback( DMA_HandleTypeDef *_ ) {

    auto *spiInfo = SPI_GetRuntimeInfo(SPIhx.Instance);

    if (auto *cb = spiInfo->asyncCompleteCallback) {
      cb( spiInfo->async_ud );
    }
  }

  void spiWriteAsync(const uint8_t *buf, uint16_t nbyte, void (*completeCallback)(void*), void *ud) {

    if (_spi_pin_mosi == NC) return;

    auto *spiInfo = SPI_GetRuntimeInfo(SPIhx.Instance);

    _SPI_AsyncBarrier();
    spiInfo->async_busy = true;

    spiInfo->asyncCompleteCallback = completeCallback;
    spiInfo->async_ud = ud;

    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_8BIT);
    DMAhx.XferCpltCallback = _DMA_CompleteCallback;
    _HAL_DMA_ConfigureTransfer(true, SPI_DATASIZE_8BIT);

    HAL_DMA_Start_IT(&DMAhx, (uint32_t)buf, (uint32_t)&(SPIhx.Instance->DR), nbyte);

    SET_BIT(SPIhx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request

    if (nbyte > 0)
      _spi_is_transmit_sequence_initialized = true;
  }

  void spiWriteAsync16(const uint16_t *buf, uint16_t ndata, void (*completeCallback)(void*), void *ud) {

    if (_spi_pin_mosi == NC) return;

    auto *spiInfo = SPI_GetRuntimeInfo(SPIhx.Instance);

    _SPI_AsyncBarrier();
    spiInfo->async_busy = true;

    spiInfo->asyncCompleteCallback = completeCallback;
    spiInfo->async_ud = ud;

    _HAL_SPI_UpdateTransaction(SPI_DATASIZE_16BIT);
    DMAhx.XferCpltCallback = _DMA_CompleteCallback;
    _HAL_DMA_ConfigureTransfer(true, SPI_DATASIZE_16BIT);

    HAL_DMA_Start_IT(&DMAhx, (uint32_t)buf, (uint32_t)&(SPIhx.Instance->DR), ndata);

    SET_BIT(SPIhx.Instance->CR2, SPI_CR2_TXDMAEN);   // Enable Tx DMA Request

    if (ndata > 0)
      _spi_is_transmit_sequence_initialized = true;
  }

  void spiAsyncAbort() {

    cli();

    auto *spiInfo = SPI_GetRuntimeInfo(SPIhx.Instance);

    if (spiInfo->async_busy)
    {
      HAL_DMA_Abort(&DMAhx);
      HAL_DMA_DeInit(&DMAhx);

      sei();

      spiInfo->async_busy = false;
    }
    else
      sei();
  }

  void spiAsyncJoin() {
    auto *spiInfo = SPI_GetRuntimeInfo(SPIhx.Instance);

    spi_monitored_loop lw;
    while (spiInfo->async_busy) { lw.update(11); }
  }

  bool spiAsyncIsRunning() {
    auto *spiInfo = SPI_GetRuntimeInfo(SPIhx.Instance);

    return spiInfo->async_busy;
  }

#endif // HAL_SPI_SUPPORTS_ASYNC

  // DMA async semantics are actually really dangerous, here is why:
  // - you need to wait for DMA transfers to finish, which devs might forget
  // - it is invalid to reuse the memory while DMA is operating on it, which devs might forget
  // Thus code depending on DMA needs very special attention to get it right.

#endif

#endif // HAL_STM32