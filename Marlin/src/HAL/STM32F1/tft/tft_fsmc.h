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

    static uint32_t ReadID(uint16_t Reg);
    static void Transmit(uint16_t Data);
    static void Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);
    static void TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);

  public:
    static void Init();
    static uint32_t GetID();
    static bool isBusy();
    static void Abort();

    static void DataTransferBegin(uint16_t DataWidth = DATASIZE_16BIT) {};
    static void DataTransferEnd() {};

    static void WriteData(uint16_t Data) { Transmit(Data); }
    static void WriteReg(uint16_t Reg);

    static void WriteSequence_DMA(uint16_t *Data, uint16_t Count) { TransmitDMA(DMA_PINC_ENABLE, Data, Count); }
    static void WriteMultiple_DMA(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_PINC_DISABLE, &Data, Count); }

    static void WriteSequence(uint16_t *Data, uint16_t Count) { Transmit(DMA_PINC_ENABLE, Data, Count); }
    static void WriteMultiple(uint16_t Color, uint32_t Count) {
      while (Count > 0) {
        Transmit(DMA_PINC_DISABLE, &Color, Count > DMA_MAX_WORDS ? DMA_MAX_WORDS : Count);
        Count = Count > DMA_MAX_WORDS ? Count - DMA_MAX_WORDS : 0;
      }
    }
};
