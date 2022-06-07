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
  #include <stm32f1xx.h>
#elif defined(STM32F4xx)
  #include <stm32f4xx.h>
#elif defined(STM32F7xx)
  #include <stm32f7xx.h>
#elif defined(STM32H7xx)
  #include <stm32h7xx.h>
#else
  #error "SDIO only supported with STM32F103xE, STM32F103xG, STM32F4xx, STM32F7xx, or STM32H7xx."
#endif

// Target Clock, configurable. Default is 18MHz, from STM32F1
#ifndef SDIO_CLOCK
  #define SDIO_CLOCK 18000000 // 18 MHz
#endif

#define SD_TIMEOUT              1000 // ms

// SDIO Max Clock (naming from STM Manual, don't change)
#define SDIOCLK 48000000

#if defined(STM32F1xx)
  DMA_HandleTypeDef hdma_sdio;
  extern "C" void DMA2_Channel4_5_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_sdio);
  }
#elif defined(STM32F4xx)
  DMA_HandleTypeDef hdma_sdio_rx;
  DMA_HandleTypeDef hdma_sdio_tx;
  extern "C" void DMA2_Stream3_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_sdio_rx);
  }

  extern "C" void DMA2_Stream6_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_sdio_tx);
  }
#elif defined(STM32H7xx)
  #define __HAL_RCC_SDIO_FORCE_RESET          __HAL_RCC_SDMMC1_FORCE_RESET
  #define __HAL_RCC_SDIO_RELEASE_RESET        __HAL_RCC_SDMMC1_RELEASE_RESET
  #define __HAL_RCC_SDIO_CLK_ENABLE           __HAL_RCC_SDMMC1_CLK_ENABLE
  #define SDIO                                SDMMC1
  #define SDIO_IRQn                           SDMMC1_IRQn
  #define SDIO_IRQHandler                     SDMMC1_IRQHandler
  #define SDIO_CLOCK_EDGE_RISING              SDMMC_CLOCK_EDGE_RISING
  #define SDIO_CLOCK_POWER_SAVE_DISABLE       SDMMC_CLOCK_POWER_SAVE_DISABLE
  #define SDIO_BUS_WIDE_1B                    SDMMC_BUS_WIDE_1B
  #define SDIO_BUS_WIDE_4B                    SDMMC_BUS_WIDE_4B
  #define SDIO_HARDWARE_FLOW_CONTROL_DISABLE  SDMMC_HARDWARE_FLOW_CONTROL_DISABLE
#endif

uint8_t waitingRxCplt = 0;
uint8_t waitingTxCplt = 0;
SD_HandleTypeDef hsd;

extern "C" void SDIO_IRQHandler(void) {
  HAL_SD_IRQHandler(&hsd);
}

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsdio) {
  waitingTxCplt = 0;
}

void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsdio) {
  waitingRxCplt = 0;
}

void HAL_SD_MspInit(SD_HandleTypeDef *hsd) {
  pinmap_pinout(PC_12, PinMap_SD);
  pinmap_pinout(PD_2, PinMap_SD);
  pinmap_pinout(PC_8, PinMap_SD);
  #if PINS_EXIST(SDIO_D1, SDIO_D2, SDIO_D3)  // define D1-D3 only if have a four bit wide SDIO bus
    // D1-D3
    pinmap_pinout(PC_9, PinMap_SD);
    pinmap_pinout(PC_10, PinMap_SD);
    pinmap_pinout(PC_11, PinMap_SD);
  #endif

  __HAL_RCC_SDIO_CLK_ENABLE();
  HAL_NVIC_EnableIRQ(SDIO_IRQn);

  // DMA Config
  #if defined(STM32F1xx)
    __HAL_RCC_DMA2_CLK_ENABLE();
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
    hdma_sdio.Instance = DMA2_Channel4;
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_sdio);

    __HAL_LINKDMA(hsd, hdmarx ,hdma_sdio);
    __HAL_LINKDMA(hsd, hdmatx, hdma_sdio);
  #elif defined(STM32F4xx)
    __HAL_RCC_DMA2_CLK_ENABLE();
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
    hdma_sdio_rx.Instance = DMA2_Stream3;
    hdma_sdio_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_rx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_sdio_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_rx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    HAL_DMA_Init(&hdma_sdio_rx);

    __HAL_LINKDMA(hsd,hdmarx,hdma_sdio_rx);

    hdma_sdio_tx.Instance = DMA2_Stream6;
    hdma_sdio_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdio_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio_tx.Init.Mode = DMA_PFCTRL;
    hdma_sdio_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_sdio_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio_tx.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    HAL_DMA_Init(&hdma_sdio_tx);

    __HAL_LINKDMA(hsd,hdmatx,hdma_sdio_tx);
  #endif
}

void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd) {
  #if !defined(STM32F1xx)
    __HAL_RCC_SDIO_FORCE_RESET();
    delay(10);
    __HAL_RCC_SDIO_RELEASE_RESET();
    delay(10);
  #endif
}

static uint32_t clock_to_divider(uint32_t clk) {
  #if defined(STM32H7xx)
    // SDMMC_CK frequency = sdmmc_ker_ck / [2 * CLKDIV].
    uint32_t sdmmc_clk     = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SDMMC);
    return sdmmc_clk / (2U * SDIO_CLOCK) + (sdmmc_clk % (2U * SDIO_CLOCK) != 0);
  #else
    // limit the SDIO master clock to 8/3 of PCLK2. See STM32 Manuals
    // Also limited to no more than 48Mhz (SDIOCLK).
    const uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
    clk = min(clk, (uint32_t)(pclk2 * 8 / 3));
    clk = min(clk, (uint32_t)SDIOCLK);
    // Round up divider, so we don't run the card over the speed supported,
    // and subtract by 2, because STM32 will add 2, as written in the manual:
    // SDIO_CK frequency = SDIOCLK / [CLKDIV + 2]
    return pclk2 / clk + (pclk2 % clk != 0) - 2;
  #endif
}

bool SDIO_Init() {
  HAL_StatusTypeDef sd_state = HAL_OK;
  if (hsd.Instance == SDIO)
    HAL_SD_DeInit(&hsd);

  /* HAL SD initialization */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = clock_to_divider(SDIO_CLOCK);
  sd_state = HAL_SD_Init(&hsd);

  #if PINS_EXIST(SDIO_D1, SDIO_D2, SDIO_D3)
    if (sd_state == HAL_OK) {
      sd_state = HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B);
    }
  #endif

  return (sd_state == HAL_OK) ? true : false;
}

bool SDIO_ReadBlock(uint32_t block, uint8_t *dst) {
  uint32_t timeout = HAL_GetTick() + SD_TIMEOUT;

  while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER) {
    if (HAL_GetTick() >= timeout) return false;
  }

  waitingRxCplt = 1;
  if (HAL_SD_ReadBlocks_DMA(&hsd, (uint8_t *)dst, block, 1) != HAL_OK)
    return false;

  timeout = HAL_GetTick() + SD_TIMEOUT;
  while (waitingRxCplt)
    if (HAL_GetTick() >= timeout) return false;

  return true;
}

bool SDIO_WriteBlock(uint32_t block, const uint8_t *src) {
  uint32_t timeout = HAL_GetTick() + SD_TIMEOUT;

  while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER)
    if (HAL_GetTick() >= timeout) return false;

  waitingTxCplt = 1;
  if (HAL_SD_WriteBlocks_DMA(&hsd, (uint8_t *)src, block, 1) != HAL_OK)
    return false;

  timeout = HAL_GetTick() + SD_TIMEOUT;
  while (waitingTxCplt)
    if (HAL_GetTick() >= timeout) return false;

  return true;
}

bool SDIO_IsReady() {
  return hsd.State == HAL_SD_STATE_READY;
}

uint32_t SDIO_GetCardSize() {
  return (uint32_t)(hsd.SdCard.BlockNbr) * (hsd.SdCard.BlockSize);
}

#endif // SDIO_SUPPORT
#endif // HAL_STM32
