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

#ifndef LCD_READ_ID
  #define LCD_READ_ID  0x04   // Read display identification information (0xD3 on ILI9341)
#endif
#ifndef LCD_READ_ID4
  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
#endif

#include <libmaple/dma.h>

#ifndef FSMC_DMA_DEV
  #define FSMC_DMA_DEV      DMA2
#endif
#ifndef FSMC_DMA_CHANNEL
  #define FSMC_DMA_CHANNEL  DMA_CH5
#endif

#define DATASIZE_8BIT  DMA_SIZE_8BITS
#define DATASIZE_16BIT DMA_SIZE_16BITS
#define TFT_IO_DRIVER  TFT_FSMC
#define DMA_MAX_WORDS  0xFFFF

#define DMA_PINC_ENABLE   DMA_PINC_MODE
#define DMA_PINC_DISABLE  0

typedef struct {
  __IO uint16_t REG;
  __IO uint16_t RAM;
} LCD_CONTROLLER_TypeDef;

class TFT_FSMC {
  private:
    static LCD_CONTROLLER_TypeDef *LCD;

    static uint32_t readID(const uint16_t inReg);
    static void transmit(uint16_t data);
    static void transmit(uint32_t memoryIncrease, uint16_t *data, uint16_t count);
    static void transmitDMA(uint32_t memoryIncrease, uint16_t *data, uint16_t count);

  public:
    static void init();
    static uint32_t getID();
    static bool isBusy();
    static void abort();

    static void dataTransferBegin(uint16_t dataWidth=DATASIZE_16BIT) {};
    static void dataTransferEnd() {};

    static void writeData(uint16_t data) { transmit(data); }
    static void writeReg(const uint16_t inReg);

    static void writeSequence_DMA(uint16_t *data, uint16_t count) { transmitDMA(DMA_PINC_ENABLE, data, count); }
    static void writeMultiple_DMA(uint16_t color, uint16_t count) { static uint16_t data; data = color; transmitDMA(DMA_PINC_DISABLE, &data, count); }

    static void writeSequence(uint16_t *data, uint16_t count) { transmit(DMA_PINC_ENABLE, data, count); }
    static void writeMultiple(uint16_t color, uint32_t count) {
      while (count > 0) {
        transmit(DMA_PINC_DISABLE, &color, count > DMA_MAX_WORDS ? DMA_MAX_WORDS : count);
        count = count > DMA_MAX_WORDS ? count - DMA_MAX_WORDS : 0;
      }
    }
};
