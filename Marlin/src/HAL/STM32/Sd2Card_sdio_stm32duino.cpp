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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDIO_SUPPORT) && !defined(STM32GENERIC)

#include <stdint.h>
#include <stdbool.h>

#if NONE(STM32F103xE, STM32F103xG, STM32F4xx, STM32F7xx)
  #error "ERROR - Only STM32F103xE, STM32F103xG, STM32F4xx or STM32F7xx CPUs supported"
#endif

#ifdef USBD_USE_CDC_COMPOSITE

  // use USB drivers

  extern "C" { int8_t SD_MSC_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
               int8_t SD_MSC_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
               extern SD_HandleTypeDef hsd;
  }

  bool SDIO_Init() {
    if (hsd.State == HAL_SD_STATE_READY) return 1;  // return passing status
    return 0;                                       // return failing status
  }

  bool SDIO_ReadBlock(uint32_t block, uint8_t *src) {
    int8_t status = SD_MSC_Read(0, (uint8_t*)src, block, 1); // read one 512 byte block
    return (bool) status;
  }

  bool SDIO_WriteBlock(uint32_t block, const uint8_t *src) {
    int8_t status = SD_MSC_Write(0, (uint8_t*)src, block, 1); // write one 512 byte block
    return (bool) status;
  }

#else // !USBD_USE_CDC_COMPOSITE

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
    #error "ERROR - Only STM32F103xE, STM32F103xG, STM32F4xx or STM32F7xx CPUs supported"
  #endif

  SD_HandleTypeDef hsd;  // create SDIO structure

  #define TRANSFER_CLOCK_DIV (uint8_t(SDIO_INIT_CLK_DIV) / 40)

  #ifndef USBD_OK
    #define USBD_OK 0
  #endif

  void go_to_transfer_speed() {
    SD_InitTypeDef Init;

    /* Default SDIO peripheral configuration for SD card initialization */
    Init.ClockEdge           = hsd.Init.ClockEdge;
    Init.ClockBypass         = hsd.Init.ClockBypass;
    Init.ClockPowerSave      = hsd.Init.ClockPowerSave;
    Init.BusWide             = hsd.Init.BusWide;
    Init.HardwareFlowControl = hsd.Init.HardwareFlowControl;
    Init.ClockDiv            = TRANSFER_CLOCK_DIV;

    /* Initialize SDIO peripheral interface with default configuration */
    SDIO_Init(hsd.Instance, Init);
  }

  void SD_LowLevel_Init(void) {
    uint32_t tempreg;

    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //enable GPIO clocks
    __HAL_RCC_GPIOD_CLK_ENABLE(); //enable GPIO clocks

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_12;  // D0 & SCK
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = 1;  //GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    #if PINS_EXIST(SDIO_D1, SDIO_D2, SDIO_D3)  // define D1-D3 only if have a four bit wide SDIO bus
      GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;  // D1-D3
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = 1;  // GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    #endif

    // Configure PD.02 CMD line
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    RCC->APB2RSTR &= ~RCC_APB2RSTR_SDIORST_Msk;  // take SDIO out of reset
    RCC->APB2ENR  |=  RCC_APB2RSTR_SDIORST_Msk;  // enable SDIO clock

    // Enable the DMA2 Clock

    //Initialize the SDIO (with initial <400Khz Clock)
    tempreg = 0;  //Reset value
    tempreg |= SDIO_CLKCR_CLKEN;  // Clock enabled
    tempreg |= (uint32_t)0x76;    // Clock Divider. Clock = 48000 / (118 + 2) = 400Khz
    // Keep the rest at 0 => HW_Flow Disabled, Rising Clock Edge, Disable CLK ByPass, Bus Width = 0, Power save Disable
    SDIO->CLKCR = tempreg;

    // Power up the SDIO
    SDIO->POWER = 0x03;
  }

  void HAL_SD_MspInit(SD_HandleTypeDef *hsd) { // application specific init
    UNUSED(hsd);   /* Prevent unused argument(s) compilation warning */
    __HAL_RCC_SDIO_CLK_ENABLE();  // turn on SDIO clock
  }

  constexpr uint8_t SD_RETRY_COUNT = TERN(SD_CHECK_AND_RETRY, 3, 1);

  bool SDIO_Init() {
    //init SDIO and get SD card info

    uint8_t retryCnt = SD_RETRY_COUNT;

    bool status;
    hsd.Instance = SDIO;
    hsd.State = (HAL_SD_StateTypeDef) 0;  // HAL_SD_STATE_RESET
    SD_LowLevel_Init();

    uint8_t retry_Cnt = retryCnt;
    for (;;) {
      status = (bool) HAL_SD_Init(&hsd);
      if (!status) break;
      if (!--retry_Cnt) return false;   // return failing status if retries are exhausted
    }

    go_to_transfer_speed();

    #if PINS_EXIST(SDIO_D1, SDIO_D2, SDIO_D3) // go to 4 bit wide mode if pins are defined
      retry_Cnt = retryCnt;
      for (;;) {
        if (!HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B)) break;  // some cards are only 1 bit wide so a pass here is not required
        if (!--retry_Cnt) break;
      }
      if (!retry_Cnt) {  // wide bus failed, go back to one bit wide mode
        hsd.State = (HAL_SD_StateTypeDef) 0;  // HAL_SD_STATE_RESET
        SD_LowLevel_Init();
        retry_Cnt = retryCnt;
        for (;;) {
          status = (bool) HAL_SD_Init(&hsd);
          if (!status) break;
          if (!--retry_Cnt) return false;   // return failing status if retries are exhausted
        }
      }
    #endif

    return true;
  }

  void init_SDIO_pins(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /**SDIO GPIO Configuration
    PC8     ------> SDIO_D0
    PC12    ------> SDIO_CK
    PD2     ------> SDIO_CMD
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  }

  //bool SDIO_init() { return (bool) (SD_SDIO_Init() ? 1 : 0);}
  //bool SDIO_Init_C() { return (bool) (SD_SDIO_Init() ? 1 : 0);}

  bool SDIO_ReadBlock(uint32_t block, uint8_t *dst) {
    hsd.Instance = SDIO;
    uint8_t retryCnt = SD_RETRY_COUNT;

    bool status;
    for (;;) {
      status = (bool) HAL_SD_ReadBlocks(&hsd, (uint8_t*)dst, block, 1, 1000);  // read one 512 byte block with 500mS timeout
      status |= (bool) HAL_SD_GetCardState(&hsd);     // make sure all is OK
      if (!status) break;       // return passing status
      if (!--retryCnt) break;   // return failing status if retries are exhausted
    }
    return status;

    /*
    return (bool) ((status_read | status_card) ? 1 : 0);

    if (SDIO_GetCardState() != SDIO_CARD_TRANSFER) return false;
    if (blockAddress >= SdCard.LogBlockNbr) return false;
    if ((0x03 & (uint32_t)data)) return false; // misaligned data

    if (SdCard.CardType != CARD_SDHC_SDXC) { blockAddress *= 512U; }

    if (!SDIO_CmdReadSingleBlock(blockAddress)) {
      SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
      dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
      return false;
    }

    while (!SDIO_GET_FLAG(SDIO_STA_DATAEND | SDIO_STA_TRX_ERROR_FLAGS)) {}

    dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);

    if (SDIO->STA & SDIO_STA_RXDAVL) {
      while (SDIO->STA & SDIO_STA_RXDAVL) (void)SDIO->FIFO;
      SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
      return false;
    }

    if (SDIO_GET_FLAG(SDIO_STA_TRX_ERROR_FLAGS)) {
      SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
      return false;
    }
    SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
    */

    return true;
  }

  bool SDIO_WriteBlock(uint32_t block, const uint8_t *src) {
    hsd.Instance = SDIO;
    uint8_t retryCnt = SD_RETRY_COUNT;

    bool status;
    for (;;) {
      status = (bool) HAL_SD_WriteBlocks(&hsd, (uint8_t*)src, block, 1, 500);  // write one 512 byte block with 500mS timeout
      status |= (bool) HAL_SD_GetCardState(&hsd);     // make sure all is OK
      if (!status) break;       // return passing status
      if (!--retryCnt) break;   // return failing status if retries are exhausted
    }
    return status;
  }

#endif // !USBD_USE_CDC_COMPOSITE
#endif // SDIO_SUPPORT
