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

SPIClass TFT_SPI::SPIx(TFT_SPI_DEVICE);

void TFT_SPI::Init() {
  #if PIN_EXISTS(TFT_RESET)
    OUT_WRITE(TFT_RESET_PIN, HIGH);
    delay(100);
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT)
    OUT_WRITE(TFT_BACKLIGHT_PIN, HIGH);
  #endif

  OUT_WRITE(TFT_DC_PIN, HIGH);
  OUT_WRITE(TFT_CS_PIN, HIGH);

  SPIx.setModule(TFT_SPI_DEVICE);
  SPIx.setClock(SPI_CLOCK_MAX_TFT);
  SPIx.setBitOrder(MSBFIRST);
  SPIx.setDataMode(SPI_MODE0);
}

void TFT_SPI::DataTransferBegin(uint16_t DataSize) {
  SPIx.setDataSize(DataSize);
  SPIx.begin();
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
  uint32_t data = 0;

  #if PIN_EXISTS(TFT_MISO)
    uint8_t d = 0;
    SPIx.setDataSize(DATASIZE_8BIT);
    SPIx.setClock(SPI_CLOCK_DIV64);
    SPIx.begin();
    WRITE(TFT_CS_PIN, LOW);
    WriteReg(Reg);

    LOOP_L_N(i, 4) {
      SPIx.read((uint8_t*)&d, 1);
      data = (data << 8) | d;
    }

    DataTransferEnd();
    SPIx.setClock(SPI_CLOCK_MAX_TFT);
  #endif

  return data >> 7;
}

bool TFT_SPI::isBusy() {
  #define __IS_DMA_CONFIGURED(__HANDLE__)   ((__HANDLE__)->DMACCSrcAddr != 0)

  // DMA Channel 0 is hardcoded in dmaSendAsync() and dmaSend()
  if (!__IS_DMA_CONFIGURED(LPC_GPDMACH0)) return false;

  if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)) {
    // You should not be here - DMA transfer error flag is set
    // Abort DMA transfer and release SPI
  }
  else {
    // Check if DMA transfer completed flag is set
    if (!GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0)) return true;
    // Check if SPI TX butter is empty and SPI is idle
    if ((SSP_GetStatus(LPC_SSPx, SSP_STAT_TXFIFO_EMPTY) == RESET) || (SSP_GetStatus(LPC_SSPx, SSP_STAT_BUSY) == SET)) return true;
  }

  Abort();
  return false;
}

void TFT_SPI::Abort() {
  // DMA Channel 0 is hardcoded in dmaSendAsync() and dmaSend()

  // Disable DMA
  GPDMA_ChannelCmd(0, DISABLE);

  // Clear ERR and TC
  GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, 0);
  GPDMA_ClearIntPending(GPDMA_STATCLR_INTERR, 0);

  // Disable DMA on SPI
  SSP_DMACmd(LPC_SSPx, SSP_DMA_TX, DISABLE);

  // Deconfigure DMA Channel 0
  LPC_GPDMACH0->DMACCControl  = 0U;
  LPC_GPDMACH0->DMACCConfig   = 0U;
  LPC_GPDMACH0->DMACCSrcAddr  = 0U;
  LPC_GPDMACH0->DMACCDestAddr = 0U;

  DataTransferEnd();
}

void TFT_SPI::Transmit(uint16_t Data) { SPIx.transfer(Data); }

void TFT_SPI::Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  DataTransferBegin(DATASIZE_16BIT);
  SPIx.dmaSend(Data, Count, MemoryIncrease);
  Abort();
}

void TFT_SPI::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  DataTransferBegin(DATASIZE_16BIT);
  SPIx.dmaSendAsync(Data, Count, MemoryIncrease);

  TERN_(TFT_SHARED_SPI, while (isBusy()));
}

#endif // HAS_SPI_TFT
