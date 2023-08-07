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
  static uint32_t readID(const uint16_t inReg);
  static void transmit(uint16_t data);
  static void transmit(uint32_t memoryIncrease, uint16_t *data, uint16_t count);
  static void transmitDMA(uint32_t memoryIncrease, uint16_t *data, uint16_t count);

public:
  static SPIClass SPIx;

  static void init();
  static uint32_t getID();
  static bool isBusy();
  static void abort();

  static void dataTransferBegin(uint16_t dataWidth=DATASIZE_16BIT);
  static void dataTransferEnd() { WRITE(TFT_CS_PIN, HIGH); SSP_Cmd(LPC_SSPx, DISABLE); };
  static void dataTransferAbort();

  static void writeData(uint16_t data) { transmit(data); }
  static void writeReg(const uint16_t inReg) { WRITE(TFT_DC_PIN, LOW); transmit(inReg); WRITE(TFT_DC_PIN, HIGH); }

  static void writeSequence_DMA(uint16_t *data, uint16_t count) { transmitDMA(DMA_MINC_ENABLE, data, count); }
  static void writeMultiple_DMA(uint16_t color, uint16_t count) { static uint16_t data; data = color; transmitDMA(DMA_MINC_DISABLE, &data, count); }

  static void writeSequence(uint16_t *data, uint16_t count) { transmit(DMA_MINC_ENABLE, data, count); }
  static void writeMultiple(uint16_t color, uint32_t count) {
    while (count > 0) {
      transmit(DMA_MINC_DISABLE, &color, count > DMA_MAX_WORDS ? DMA_MAX_WORDS : count);
      count = count > DMA_MAX_WORDS ? count - DMA_MAX_WORDS : 0;
    }
  }
};
