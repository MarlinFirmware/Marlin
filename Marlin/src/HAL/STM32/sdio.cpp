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

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDIO_SUPPORT)

#include "sdio.h"

#include <stdint.h>
#include <stdbool.h>

// use local drivers
#if defined(STM32F103xE) || defined(STM32F103xG)
  #include <stm32f1xx_hal_rcc_ex.h>
  #include <stm32f1xx_hal_sd.h>
#elif defined(STM32F4xx)
  #include <stm32f4xx_hal_rcc.h>
  #include <stm32f4xx_hal_dma.h>
  #include <stm32f4xx_hal_gpio.h>
  #include <stm32f4xx_hal_sd.h>
#elif defined(STM32F7xx)
  #include <stm32f7xx_hal_rcc.h>
  #include <stm32f7xx_hal_dma.h>
  #include <stm32f7xx_hal_gpio.h>
  #include <stm32f7xx_hal_sd.h>
#else
  #error "SDIO only supported with STM32F103xE, STM32F103xG, STM32F4xx, or STM32F7xx."
#endif

SD_HandleTypeDef hsd;  // create SDIO structure
// F4 supports one DMA for RX and another for TX, but Marlin will never
// do read and write at same time, so we use the same DMA for both.
DMA_HandleTypeDef hdma_sdio;

/*
  SDIO_INIT_CLK_DIV is 118
  SDIO clock frequency is 48MHz / (TRANSFER_CLOCK_DIV + 2)
  SDIO init clock frequency should not exceed 400KHz = 48MHz / (118 + 2)

  Default TRANSFER_CLOCK_DIV is 2 (118 / 40)
  Default SDIO clock frequency is 48MHz / (2 + 2) = 12 MHz
  This might be too fast for stable SDIO operations

  MKS Robin board seems to have stable SDIO with BusWide 1bit and ClockDiv 8 i.e. 4.8MHz SDIO clock frequency
  Additional testing is required as there are clearly some 4bit initialization problems
*/

#ifndef USBD_OK
  #define USBD_OK 0
#endif

// Target Clock, configurable. Default is 18MHz, from STM32F1
#ifndef SDIO_CLOCK
  #define SDIO_CLOCK 18000000 // 18 MHz
#endif

// SDIO retries, configurable. Default is 3, from STM32F1
#ifndef SDIO_READ_RETRIES
  #define SDIO_READ_RETRIES 3
#endif

// SDIO Max Clock (naming from STM Manual, don't change)
#define SDIOCLK 48000000

static uint32_t clock_to_divider(uint32_t clk) {
  // limit the SDIO master clock to 8/3 of PCLK2. See STM32 Manuals
  // Also limited to no more than 48Mhz (SDIOCLK).
  const uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
  clk = min(clk, (uint32_t)(pclk2 * 8 / 3));
  clk = min(clk, (uint32_t)SDIOCLK);
  // Round up divider, so we don't run the card over the speed supported,
  // and subtract by 2, because STM32 will add 2, as written in the manual:
  // SDIO_CK frequency = SDIOCLK / [CLKDIV + 2]
  return pclk2 / clk + (pclk2 % clk != 0) - 2;
}

void go_to_transfer_speed() {
  /* Default SDIO peripheral configuration for SD card initialization */
  hsd.Init.ClockEdge           = hsd.Init.ClockEdge;
  hsd.Init.ClockBypass         = hsd.Init.ClockBypass;
  hsd.Init.ClockPowerSave      = hsd.Init.ClockPowerSave;
  hsd.Init.BusWide             = hsd.Init.BusWide;
  hsd.Init.HardwareFlowControl = hsd.Init.HardwareFlowControl;
  hsd.Init.ClockDiv            = clock_to_divider(SDIO_CLOCK);

  /* Initialize SDIO peripheral interface with default configuration */
  SDIO_Init(hsd.Instance, hsd.Init);
}

void SD_LowLevel_Init(void) {
  uint32_t tempreg;

  __HAL_RCC_GPIOC_CLK_ENABLE(); //enable GPIO clocks
  __HAL_RCC_GPIOD_CLK_ENABLE(); //enable GPIO clocks

  GPIO_InitTypeDef  GPIO_InitStruct;

  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = 1;  //GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  #if DISABLED(STM32F1xx)
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  #endif

  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_12;  // D0 & SCK
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  #if PINS_EXIST(SDIO_D1, SDIO_D2, SDIO_D3)  // define D1-D3 only if have a four bit wide SDIO bus
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;  // D1-D3
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  #endif

  // Configure PD.02 CMD line
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // Setup DMA
  #if defined(STM32F1xx)
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Instance = DMA2_Channel4;
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
  #elif defined(STM32F4xx)
    hdma_sdio.Init.Mode = DMA_PFCTRL;
    hdma_sdio.Instance = DMA2_Stream3;
    hdma_sdio.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio.Init.PeriphBurst = DMA_PBURST_INC4;
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  #endif
  HAL_NVIC_EnableIRQ(SDIO_IRQn);
  hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
  hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
  __HAL_LINKDMA(&hsd, hdmarx, hdma_sdio);
  __HAL_LINKDMA(&hsd, hdmatx, hdma_sdio);

  #if defined(STM32F1xx)
    __HAL_RCC_SDIO_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
  #else
    __HAL_RCC_SDIO_FORCE_RESET();
    delay(2);
    __HAL_RCC_SDIO_RELEASE_RESET();
    delay(2);
    __HAL_RCC_SDIO_CLK_ENABLE();

    __HAL_RCC_DMA2_FORCE_RESET();
    delay(2);
    __HAL_RCC_DMA2_RELEASE_RESET();
    delay(2);
    __HAL_RCC_DMA2_CLK_ENABLE();
  #endif

  //Initialize the SDIO (with initial <400Khz Clock)
  tempreg = 0;  //Reset value
  tempreg |= SDIO_CLKCR_CLKEN;  // Clock enabled
  tempreg |= SDIO_INIT_CLK_DIV; // Clock Divider. Clock = 48000 / (118 + 2) = 400Khz
  // Keep the rest at 0 => HW_Flow Disabled, Rising Clock Edge, Disable CLK ByPass, Bus Width = 0, Power save Disable
  SDIO->CLKCR = tempreg;

  // Power up the SDIO
  SDIO_PowerState_ON(SDIO);
  hsd.Instance = SDIO;
}

void HAL_SD_MspInit(SD_HandleTypeDef *hsd) { // application specific init
  UNUSED(hsd);   // Prevent unused argument(s) compilation warning
  __HAL_RCC_SDIO_CLK_ENABLE();  // turn on SDIO clock
}

bool SDIO_Init() {
  uint8_t retryCnt = SDIO_READ_RETRIES;

  bool status;
  hsd.Instance = SDIO;
  hsd.State = HAL_SD_STATE_RESET;

  SD_LowLevel_Init();

  uint8_t retry_Cnt = retryCnt;
  for (;;) {
    TERN_(USE_WATCHDOG, HAL_watchdog_refresh());
    status = (bool) HAL_SD_Init(&hsd);
    if (!status) break;
    if (!--retry_Cnt) return false;   // return failing status if retries are exhausted
  }

  go_to_transfer_speed();

  #if PINS_EXIST(SDIO_D1, SDIO_D2, SDIO_D3) // go to 4 bit wide mode if pins are defined
    retry_Cnt = retryCnt;
    for (;;) {
      TERN_(USE_WATCHDOG, HAL_watchdog_refresh());
      if (!HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B)) break;  // some cards are only 1 bit wide so a pass here is not required
      if (!--retry_Cnt) break;
    }
    if (!retry_Cnt) {  // wide bus failed, go back to one bit wide mode
      hsd.State = (HAL_SD_StateTypeDef) 0;  // HAL_SD_STATE_RESET
      SD_LowLevel_Init();
      retry_Cnt = retryCnt;
      for (;;) {
        TERN_(USE_WATCHDOG, HAL_watchdog_refresh());
        status = (bool) HAL_SD_Init(&hsd);
        if (!status) break;
        if (!--retry_Cnt) return false;   // return failing status if retries are exhausted
      }
      go_to_transfer_speed();
    }
  #endif

  return true;
}

static bool SDIO_ReadWriteBlock_DMA(uint32_t block, const uint8_t *src, uint8_t *dst) {
  if (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) return false;

  TERN_(USE_WATCHDOG, HAL_watchdog_refresh());

  HAL_StatusTypeDef ret;
  if (src) {
    hdma_sdio.Init.Direction = DMA_MEMORY_TO_PERIPH;
    HAL_DMA_Init(&hdma_sdio);
    ret = HAL_SD_WriteBlocks_DMA(&hsd, (uint8_t *)src, block, 1);
  }
  else {
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    HAL_DMA_Init(&hdma_sdio);
    ret = HAL_SD_ReadBlocks_DMA(&hsd, (uint8_t *)dst, block, 1);
  }

  if (ret != HAL_OK) {
    HAL_DMA_Abort_IT(&hdma_sdio);
    HAL_DMA_DeInit(&hdma_sdio);
    return false;
  }

  millis_t timeout = millis() + 500;
  // Wait the transfer
  while (hsd.State != HAL_SD_STATE_READY) {
    if (ELAPSED(millis(), timeout)) {
      HAL_DMA_Abort_IT(&hdma_sdio);
      HAL_DMA_DeInit(&hdma_sdio);
      return false;
    }
  }

  while (__HAL_DMA_GET_FLAG(&hdma_sdio, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_sdio)) != 0
      || __HAL_DMA_GET_FLAG(&hdma_sdio, __HAL_DMA_GET_TE_FLAG_INDEX(&hdma_sdio)) != 0) { /* nada */ }

  HAL_DMA_Abort_IT(&hdma_sdio);
  HAL_DMA_DeInit(&hdma_sdio);

  timeout = millis() + 500;
  while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) if (ELAPSED(millis(), timeout)) return false;

  return true;
}

bool SDIO_ReadBlock(uint32_t block, uint8_t *dst) {
  uint8_t retries = SDIO_READ_RETRIES;
  while (retries--) if (SDIO_ReadWriteBlock_DMA(block, nullptr, dst)) return true;
  return false;
}

bool SDIO_WriteBlock(uint32_t block, const uint8_t *src) {
  uint8_t retries = SDIO_READ_RETRIES;
  while (retries--) if (SDIO_ReadWriteBlock_DMA(block, src, nullptr)) return true;
  return false;
}

bool SDIO_IsReady() {
  return hsd.State == HAL_SD_STATE_READY;
}

uint32_t SDIO_GetCardSize() {
  return (uint32_t)(hsd.SdCard.BlockNbr) * (hsd.SdCard.BlockSize);
}

#if defined(STM32F1xx)
  #define DMA_IRQ_HANDLER DMA2_Channel4_5_IRQHandler
#elif defined(STM32F4xx)
  #define DMA_IRQ_HANDLER DMA2_Stream3_IRQHandler
#else
  #error "Unknown STM32 architecture."
#endif

extern "C" void SDIO_IRQHandler(void) { HAL_SD_IRQHandler(&hsd); }
extern "C" void DMA_IRQ_HANDLER(void) { HAL_DMA_IRQHandler(&hdma_sdio); }

#endif // SDIO_SUPPORT
#endif // HAL_STM32
