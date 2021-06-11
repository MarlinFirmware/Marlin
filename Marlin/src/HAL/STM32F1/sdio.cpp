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
#ifdef ARDUINO_ARCH_STM32F1

#include <libmaple/stm32.h>

#include "../../inc/MarlinConfig.h" // Allow pins/pins.h to set density

#if EITHER(STM32_HIGH_DENSITY, STM32_XL_DENSITY)

#include "sdio.h"

SDIO_CardInfoTypeDef SdCard;

bool SDIO_Init() {
  uint32_t count = 0U;
  SdCard.CardType = SdCard.CardVersion = SdCard.Class = SdCard.RelCardAdd = SdCard.BlockNbr = SdCard.BlockSize = SdCard.LogBlockNbr = SdCard.LogBlockSize = 0;

  sdio_begin();
  sdio_set_dbus_width(SDIO_CLKCR_WIDBUS_1BIT);

  dma_init(SDIO_DMA_DEV);
  dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
  dma_set_priority(SDIO_DMA_DEV, SDIO_DMA_CHANNEL, DMA_PRIORITY_MEDIUM);

  if (!SDIO_CmdGoIdleState()) return false;
  if (!SDIO_CmdGoIdleState()) return false; /* Hotplugged cards tends to miss first CMD0, so give them a second chance. */

  SdCard.CardVersion = SDIO_CmdOperCond() ? CARD_V2_X : CARD_V1_X;

  do {
    if (count++ == SDMMC_MAX_VOLT_TRIAL) return false;
    SDIO_CmdAppOperCommand(SdCard.CardVersion == CARD_V2_X ? SDMMC_HIGH_CAPACITY : SDMMC_STD_CAPACITY);
  } while ((SDIO_GetResponse(SDIO_RESP1) & 0x80000000) == 0);

  SdCard.CardType = (SDIO_GetResponse(SDIO_RESP1) & SDMMC_HIGH_CAPACITY) ? CARD_SDHC_SDXC : CARD_SDSC;

  if (!SDIO_CmdSendCID()) return false;
  if (!SDIO_CmdSetRelAdd(&SdCard.RelCardAdd)) return false; /* Send CMD3 SET_REL_ADDR with argument 0. SD Card publishes its RCA. */
  if (!SDIO_CmdSendCSD(SdCard.RelCardAdd << 16U)) return false;

  SdCard.Class = (SDIO_GetResponse(SDIO_RESP2) >> 20U);

  if (SdCard.CardType == CARD_SDHC_SDXC) {
    SdCard.LogBlockNbr = SdCard.BlockNbr = (((SDIO_GetResponse(SDIO_RESP2) & 0x0000003FU) << 26U) | ((SDIO_GetResponse(SDIO_RESP3) & 0xFFFF0000U) >> 6U)) + 1024;
    SdCard.LogBlockSize = SdCard.BlockSize = 512U;
  }
  else {
    SdCard.BlockNbr  = ((((SDIO_GetResponse(SDIO_RESP2) & 0x000003FFU) << 2U ) | ((SDIO_GetResponse(SDIO_RESP3) & 0xC0000000U) >> 30U)) + 1U) * (4U << ((SDIO_GetResponse(SDIO_RESP3) & 0x00038000U) >> 15U));
    SdCard.BlockSize = 1U << ((SDIO_GetResponse(SDIO_RESP2) >> 16) & 0x0FU);
    SdCard.LogBlockNbr =  (SdCard.BlockNbr) * ((SdCard.BlockSize) / 512U);
    SdCard.LogBlockSize = 512U;
  }

  if (!SDIO_CmdSelDesel(SdCard.RelCardAdd << 16U)) return false;
  if (!SDIO_CmdAppSetClearCardDetect(SdCard.RelCardAdd << 16U)) return false;
  if (!SDIO_CmdAppSetBusWidth(SdCard.RelCardAdd << 16U, 2)) return false;

  sdio_set_dbus_width(SDIO_CLKCR_WIDBUS_4BIT);
  sdio_set_clock(SDIO_CLOCK);
  return true;
}

bool SDIO_ReadBlock_DMA(uint32_t blockAddress, uint8_t *data) {
  if (SDIO_GetCardState() != SDIO_CARD_TRANSFER) return false;
  if (blockAddress >= SdCard.LogBlockNbr) return false;
  if ((0x03 & (uint32_t)data)) return false; // misaligned data

  if (SdCard.CardType != CARD_SDHC_SDXC) { blockAddress *= 512U; }

  dma_setup_transfer(SDIO_DMA_DEV, SDIO_DMA_CHANNEL, &SDIO->FIFO, DMA_SIZE_32BITS, data, DMA_SIZE_32BITS, DMA_MINC_MODE);
  dma_set_num_transfers(SDIO_DMA_DEV, SDIO_DMA_CHANNEL, 128);
  dma_clear_isr_bits(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
  dma_enable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);

  sdio_setup_transfer(SDIO_DATA_TIMEOUT * (F_CPU / 1000U), 512, SDIO_BLOCKSIZE_512 | SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN | SDIO_DIR_RX);

  if (!SDIO_CmdReadSingleBlock(blockAddress)) {
    SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
    dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
    return false;
  }

  while (!SDIO_GET_FLAG(SDIO_STA_DATAEND | SDIO_STA_TRX_ERROR_FLAGS)) { /* wait */ }

  //If there were SDIO errors, do not wait DMA.
  if (SDIO->STA & SDIO_STA_TRX_ERROR_FLAGS) {
    SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
    dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
    return false;
  }

  //Wait for DMA transaction to complete
  while ((DMA2_BASE->ISR & (DMA_ISR_TEIF4|DMA_ISR_TCIF4)) == 0 ) { /* wait */ }

  if (DMA2_BASE->ISR & DMA_ISR_TEIF4) {
    dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
    SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
    return false;
  }

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
  return true;
}

bool SDIO_ReadBlock(uint32_t blockAddress, uint8_t *data) {
  uint32_t retries = SDIO_READ_RETRIES;
  while (retries--) if (SDIO_ReadBlock_DMA(blockAddress, data)) return true;
  return false;
}

uint32_t millis();

bool SDIO_WriteBlock(uint32_t blockAddress, const uint8_t *data) {
  if (SDIO_GetCardState() != SDIO_CARD_TRANSFER) return false;
  if (blockAddress >= SdCard.LogBlockNbr) return false;
  if ((0x03 & (uint32_t)data)) return false; // misaligned data

  if (SdCard.CardType != CARD_SDHC_SDXC) { blockAddress *= 512U; }

  dma_setup_transfer(SDIO_DMA_DEV, SDIO_DMA_CHANNEL, &SDIO->FIFO, DMA_SIZE_32BITS, (volatile void *) data, DMA_SIZE_32BITS, DMA_MINC_MODE | DMA_FROM_MEM);
  dma_set_num_transfers(SDIO_DMA_DEV, SDIO_DMA_CHANNEL, 128);
  dma_clear_isr_bits(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
  dma_enable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);

  if (!SDIO_CmdWriteSingleBlock(blockAddress)) {
    dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);
    return false;
  }

  sdio_setup_transfer(SDIO_DATA_TIMEOUT * (F_CPU / 1000U), 512U, SDIO_BLOCKSIZE_512 | SDIO_DCTRL_DMAEN | SDIO_DCTRL_DTEN);

  while (!SDIO_GET_FLAG(SDIO_STA_DATAEND | SDIO_STA_TRX_ERROR_FLAGS)) { /* wait */ }

  dma_disable(SDIO_DMA_DEV, SDIO_DMA_CHANNEL);

  if (SDIO_GET_FLAG(SDIO_STA_TRX_ERROR_FLAGS)) {
    SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);
    return false;
  }

  SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS | SDIO_ICR_DATA_FLAGS);

  uint32_t timeout = millis() + SDIO_WRITE_TIMEOUT;
  while (timeout > millis()) {
    if (SDIO_GetCardState() == SDIO_CARD_TRANSFER) {
      return true;
    }
  }
  return false;
}

inline uint32_t SDIO_GetCardState() { return SDIO_CmdSendStatus(SdCard.RelCardAdd << 16U) ? (SDIO_GetResponse(SDIO_RESP1) >> 9U) & 0x0FU : SDIO_CARD_ERROR; }

// ------------------------
// SD Commands and Responses
// ------------------------

void SDIO_SendCommand(uint16_t command, uint32_t argument) { SDIO->ARG = argument; SDIO->CMD = (uint32_t)(SDIO_CMD_CPSMEN | command); }
uint8_t SDIO_GetCommandResponse() { return (uint8_t)(SDIO->RESPCMD); }
uint32_t SDIO_GetResponse(uint32_t response) { return SDIO->RESP[response]; }

bool SDIO_CmdGoIdleState() { SDIO_SendCommand(CMD0_GO_IDLE_STATE, 0); return SDIO_GetCmdError(); }
bool SDIO_CmdSendCID() { SDIO_SendCommand(CMD2_ALL_SEND_CID, 0); return SDIO_GetCmdResp2(); }
bool SDIO_CmdSetRelAdd(uint32_t *rca) { SDIO_SendCommand(CMD3_SET_REL_ADDR, 0); return SDIO_GetCmdResp6(SDMMC_CMD_SET_REL_ADDR, rca); }
bool SDIO_CmdSelDesel(uint32_t address) { SDIO_SendCommand(CMD7_SEL_DESEL_CARD, address); return SDIO_GetCmdResp1(SDMMC_CMD_SEL_DESEL_CARD); }
bool SDIO_CmdOperCond() { SDIO_SendCommand(CMD8_HS_SEND_EXT_CSD, SDMMC_CHECK_PATTERN); return SDIO_GetCmdResp7(); }
bool SDIO_CmdSendCSD(uint32_t argument) { SDIO_SendCommand(CMD9_SEND_CSD, argument); return SDIO_GetCmdResp2(); }
bool SDIO_CmdSendStatus(uint32_t argument) { SDIO_SendCommand(CMD13_SEND_STATUS, argument); return SDIO_GetCmdResp1(SDMMC_CMD_SEND_STATUS); }
bool SDIO_CmdReadSingleBlock(uint32_t address) { SDIO_SendCommand(CMD17_READ_SINGLE_BLOCK, address); return SDIO_GetCmdResp1(SDMMC_CMD_READ_SINGLE_BLOCK); }
bool SDIO_CmdWriteSingleBlock(uint32_t address) { SDIO_SendCommand(CMD24_WRITE_SINGLE_BLOCK, address); return SDIO_GetCmdResp1(SDMMC_CMD_WRITE_SINGLE_BLOCK); }
bool SDIO_CmdAppCommand(uint32_t rsa) { SDIO_SendCommand(CMD55_APP_CMD, rsa); return SDIO_GetCmdResp1(SDMMC_CMD_APP_CMD); }

bool SDIO_CmdAppSetBusWidth(uint32_t rsa, uint32_t argument) {
  if (!SDIO_CmdAppCommand(rsa)) return false;
  SDIO_SendCommand(ACMD6_APP_SD_SET_BUSWIDTH, argument);
  return SDIO_GetCmdResp2();
}

bool SDIO_CmdAppOperCommand(uint32_t sdType) {
  if (!SDIO_CmdAppCommand(0)) return false;
  SDIO_SendCommand(ACMD41_SD_APP_OP_COND , SDMMC_VOLTAGE_WINDOW_SD | sdType);
  return SDIO_GetCmdResp3();
}

bool SDIO_CmdAppSetClearCardDetect(uint32_t rsa) {
  if (!SDIO_CmdAppCommand(rsa)) return false;
  SDIO_SendCommand(ACMD42_SD_APP_SET_CLR_CARD_DETECT, 0);
  return SDIO_GetCmdResp2();
}

// Wait until given flags are unset or till timeout
#define SDIO_WAIT(FLAGS) do{ \
  uint32_t count = 1 + (SDIO_CMDTIMEOUT) * ((F_CPU) / 8U / 1000U); \
  do { if (!--count) return false; } while (!SDIO_GET_FLAG(FLAGS)); \
}while(0)

bool SDIO_GetCmdError() {
  SDIO_WAIT(SDIO_STA_CMDSENT);

  SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
  return true;
}

bool SDIO_GetCmdResp1(uint8_t command) {
  SDIO_WAIT(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT);

  if (SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT)) {
    SDIO_CLEAR_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT);
    return false;
  }
  if (SDIO_GetCommandResponse() != command) return false;

  SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
  return (SDIO_GetResponse(SDIO_RESP1) & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO;
}

bool SDIO_GetCmdResp2() {
  SDIO_WAIT(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT);

  if (SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT)) {
    SDIO_CLEAR_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT);
    return false;
  }

  SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
  return true;
}

bool SDIO_GetCmdResp3() {
  SDIO_WAIT(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT);

  if (SDIO_GET_FLAG(SDIO_STA_CTIMEOUT)) {
    SDIO_CLEAR_FLAG(SDIO_STA_CTIMEOUT);
    return false;
  }

  SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
  return true;
}

bool SDIO_GetCmdResp6(uint8_t command, uint32_t *rca) {
  SDIO_WAIT(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT);

  if (SDIO_GET_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT)) {
    SDIO_CLEAR_FLAG(SDIO_STA_CCRCFAIL | SDIO_STA_CTIMEOUT);
    return false;
  }
  if (SDIO_GetCommandResponse() != command) return false;

  SDIO_CLEAR_FLAG(SDIO_ICR_CMD_FLAGS);
  if (SDIO_GetResponse(SDIO_RESP1) & (SDMMC_R6_GENERAL_UNKNOWN_ERROR | SDMMC_R6_ILLEGAL_CMD | SDMMC_R6_COM_CRC_FAILED)) return false;

  *rca = SDIO_GetResponse(SDIO_RESP1) >> 16;
  return true;
}

bool SDIO_GetCmdResp7() {
  SDIO_WAIT(SDIO_STA_CCRCFAIL | SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT);

  if (SDIO_GET_FLAG(SDIO_STA_CTIMEOUT)) {
    SDIO_CLEAR_FLAG(SDIO_STA_CTIMEOUT);
    return false;
  }

  if (SDIO_GET_FLAG(SDIO_STA_CMDREND)) { SDIO_CLEAR_FLAG(SDIO_STA_CMDREND); }
  return true;
}

#endif // STM32_HIGH_DENSITY || STM32_XL_DENSITY
#endif // ARDUINO_ARCH_STM32F1
