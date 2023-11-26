/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_HC32

#include "sdio.h"
#include <gpio/gpio.h>
#include <sd_card.h>

//
// SDIO configuration
//

#define SDIO_PERIPHERAL M4_SDIOC1

// Use DMA2 channel 0
#define SDIO_DMA_PERIPHERAL M4_DMA2
#define SDIO_DMA_CHANNEL DmaCh0

// SDIO read/write operation retries and timeouts
#define SDIO_READ_RETRIES 3
#define SDIO_READ_TIMEOUT 100 // ms

#define SDIO_WRITE_RETRIES 1
#define SDIO_WRITE_TIMEOUT 100 // ms

//
// HAL functions
//

#define WITH_RETRY(retries, fn)                     \
  for (int retry = 0; retry < (retries); retry++) { \
    MarlinHAL::watchdog_refresh();                  \
    yield();                                        \
    fn                                              \
  }

stc_sd_handle_t *handle;

bool SDIO_Init() {
  // Configure SDIO pins
  GPIO_SetFunc(BOARD_SDIO_D0, Func_Sdio);
  GPIO_SetFunc(BOARD_SDIO_D1, Func_Sdio);
  GPIO_SetFunc(BOARD_SDIO_D2, Func_Sdio);
  GPIO_SetFunc(BOARD_SDIO_D3, Func_Sdio);
  GPIO_SetFunc(BOARD_SDIO_CLK, Func_Sdio);
  GPIO_SetFunc(BOARD_SDIO_CMD, Func_Sdio);
  GPIO_SetFunc(BOARD_SDIO_DET, Func_Sdio);

  // Create DMA configuration
  stc_sdcard_dma_init_t *dmaConf = new stc_sdcard_dma_init_t;
  dmaConf->DMAx = SDIO_DMA_PERIPHERAL;
  dmaConf->enDmaCh = SDIO_DMA_CHANNEL;

  // Create handle in DMA mode
  handle = new stc_sd_handle_t;
  handle->SDIOCx = SDIO_PERIPHERAL;
  handle->enDevMode = SdCardDmaMode;
  handle->pstcDmaInitCfg = dmaConf;

  // Create card configuration
  // This should be a fairly safe configuration for most cards
  stc_sdcard_init_t cardConf = {
    .enBusWidth = SdiocBusWidth4Bit,
    .enClkFreq = SdiocClk400K,
    .enSpeedMode = SdiocNormalSpeedMode,
    //.pstcInitCfg = NULL,
  };

  // Initialize sd card
  en_result_t rc = SDCARD_Init(handle, &cardConf);
  if (rc != Ok) printf("SDIO_Init() error (rc=%u)\n", rc);

  return rc == Ok;
}

bool SDIO_ReadBlock(uint32_t block, uint8_t *dst) {
  CORE_ASSERT(handle != NULL, "SDIO not initialized");
  CORE_ASSERT(dst != NULL, "SDIO_ReadBlock dst is NULL");

  WITH_RETRY(SDIO_READ_RETRIES, {
    en_result_t rc = SDCARD_ReadBlocks(handle, block, 1, dst, SDIO_READ_TIMEOUT);
    if (rc == Ok) return true;
    printf("SDIO_ReadBlock error (rc=%u; ErrorCode=%lu)\n", rc, handle->u32ErrorCode);
  })

  return false;
}

bool SDIO_WriteBlock(uint32_t block, const uint8_t *src) {
  CORE_ASSERT(handle != NULL, "SDIO not initialized");
  CORE_ASSERT(src != NULL, "SDIO_WriteBlock src is NULL");

  WITH_RETRY(SDIO_WRITE_RETRIES, {
    en_result_t rc = SDCARD_WriteBlocks(handle, block, 1, (uint8_t *)src, SDIO_WRITE_TIMEOUT);
    if (rc == Ok) return true;
    printf("SDIO_WriteBlock error (rc=%u; ErrorCode=%lu)\n", rc, handle->u32ErrorCode);
  })

  return false;
}

bool SDIO_IsReady() {
  CORE_ASSERT(handle != NULL, "SDIO not initialized");
  return bool(handle->stcCardStatus.READY_FOR_DATA);
}

uint32_t SDIO_GetCardSize() {
  CORE_ASSERT(handle != NULL, "SDIO not initialized");

  // Multiply number of blocks with block size to get size in bytes
  const uint64_t cardSizeBytes = uint64_t(handle->stcSdCardInfo.u32LogBlockNbr) * uint64_t(handle->stcSdCardInfo.u32LogBlockSize);

  // If the card is bigger than ~4Gb (maximum a 32bit integer can hold), clamp to the maximum value of a 32 bit integer
  return _MAX(cardSizeBytes, UINT32_MAX);
}

#endif // ARDUINO_ARCH_HC32
