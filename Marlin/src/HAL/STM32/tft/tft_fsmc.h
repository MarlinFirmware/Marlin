/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#if HAS_GRAPHICAL_TFT && HAS_FSMC_TFT

extern const PinMap PinMap_FSMC[];
extern const PinMap PinMap_FSMC_CS[];
extern const PinMap PinMap_FSMC_RS[];

#if defined(STM32F1xx)
#include "stm32f1xx_hal.h"
//#elif defined(STM32F4xx)
//#include "stm32f4xx_hal.h"
#endif

#ifndef LCD_READ_ID
  #define LCD_READ_ID 0x04   // Read display identification information (0xD3 on ILI9341)
#endif

#define DATASIZE_8BIT    SPI_DATASIZE_8BIT
#define DATASIZE_16BIT   SPI_DATASIZE_16BIT
#define TFT_IO TFT_FSMC

typedef struct {
  __IO uint16_t REG;
  __IO uint16_t RAM;
} LCD_CONTROLLER_TypeDef;

class TFT_FSMC {
  private:
    static SRAM_HandleTypeDef SRAMx;
    static DMA_HandleTypeDef DMAtx;

    static LCD_CONTROLLER_TypeDef *LCD;

    static void Transmit(uint16_t Data) { LCD->RAM = Data; __DSB(); }
    static void TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);

  public:
    static void Init();
    static uint32_t GetID();
    static bool isBusy();
    static void Abort() { __HAL_DMA_DISABLE(&DMAtx); }

    static void DataTransferBegin(uint16_t DataWidth = DATASIZE_16BIT) {}
    static void DataTransferEnd() {};

    static void WriteData(uint16_t Data) { Transmit(Data); }
    static void WriteReg(uint16_t Reg) { LCD->REG = Reg; __DSB(); }

    static void WriteSequence(uint16_t *Data, uint16_t Count) { TransmitDMA(DMA_PINC_ENABLE, Data, Count); }
    static void WriteMultiple(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_PINC_DISABLE, &Data, Count); }
  };

#endif // HAS_GRAPHICAL_TFT && HAS_FSMC_TFT
