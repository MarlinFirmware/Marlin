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
#pragma once

#include "../../../inc/MarlinConfig.h"

#include <SPI.h>
#include <lpc17xx_ssp.h>
// #include <lpc17xx_gpdma.h>

#define IS_SPI(N) (BOARD_NR_SPI >= N && (TFT_SCK_PIN == BOARD_SPI##N##_SCK_PIN) && (TFT_MOSI_PIN == BOARD_SPI##N##_MOSI_PIN) && (TFT_MISO_PIN == BOARD_SPI##N##_MISO_PIN))
#if IS_SPI(1)
  #define TFT_SPI_DEVICE  1
  #define LPC_SSPx  LPC_SSP0
#elif IS_SPI(2)
  #define TFT_SPI_DEVICE  2
  #define LPC_SSPx  LPC_SSP1
#else
  #error "Invalid TFT SPI configuration."
#endif
#undef IS_SPI

#ifndef LCD_READ_ID
  #define LCD_READ_ID  0x04   // Read display identification information (0xD3 on ILI9341)
#endif
#ifndef LCD_READ_ID4
  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
#endif

#define DATASIZE_8BIT     SSP_DATABIT_8
#define DATASIZE_16BIT    SSP_DATABIT_16
#define TFT_IO_DRIVER     TFT_SPI
#define DMA_MAX_WORDS     0xFFF

#define DMA_MINC_ENABLE   1
#define DMA_MINC_DISABLE  0

class TFT_SPI {
private:
  static uint32_t ReadID(uint16_t Reg);
  static void Transmit(uint16_t Data);
  static void Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);
  static void TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);

public:
  static SPIClass SPIx;

  static void Init();
  static uint32_t GetID();
  static bool isBusy();
  static void Abort();

  static void DataTransferBegin(uint16_t DataWidth = DATASIZE_16BIT);
  static void DataTransferEnd() { WRITE(TFT_CS_PIN, HIGH); SSP_Cmd(LPC_SSPx, DISABLE); };
  static void DataTransferAbort();

  static void WriteData(uint16_t Data) { Transmit(Data); }
  static void WriteReg(uint16_t Reg) { WRITE(TFT_DC_PIN, LOW); Transmit(Reg); WRITE(TFT_DC_PIN, HIGH); }

  static void WriteSequence_DMA(uint16_t *Data, uint16_t Count) { TransmitDMA(DMA_MINC_ENABLE, Data, Count); }
  static void WriteMultiple_DMA(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_MINC_DISABLE, &Data, Count); }

  static void WriteSequence(uint16_t *Data, uint16_t Count) { Transmit(DMA_MINC_ENABLE, Data, Count); }
  static void WriteMultiple(uint16_t Color, uint32_t Count) {
    while (Count > 0) {
      Transmit(DMA_MINC_DISABLE, &Color, Count > DMA_MAX_WORDS ? DMA_MAX_WORDS : Count);
      Count = Count > DMA_MAX_WORDS ? Count - DMA_MAX_WORDS : 0;
    }
  }
};
