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

#ifdef STM32F1xx
  #include "stm32f1xx_hal.h"
#elif defined(STM32F4xx)
  #include "stm32f4xx_hal.h"
#else
  #error SPI TFT is currently only supported on STM32F1 and STM32F4 hardware.
#endif

#ifndef LCD_READ_ID
  #define LCD_READ_ID 0x04   // Read display identification information (0xD3 on ILI9341)
#endif
#ifndef LCD_READ_ID4
  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
#endif

#define DATASIZE_8BIT  SPI_DATASIZE_8BIT
#define DATASIZE_16BIT SPI_DATASIZE_16BIT
#define DATASIZE_32BIT SPI_DATASIZE_32BIT
#define TFT_IO_DRIVER  TFT_SPI
#define DMA_MAX_WORDS  0xFFFF

class TFT_SPI {
private:
  static SPI_HandleTypeDef SPIx;
  static DMA_HandleTypeDef DMAtx;

  static uint32_t ReadID(uint16_t Reg);
  static void Transmit(uint16_t Data);
  static void Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);
  static void TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);
  #if ENABLED(USE_SPI_DMA_TC)
    static void TransmitDMA_IT(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);
  #endif

public:
  static void Init();
  static uint32_t GetID();
  static bool isBusy();
  static void Abort();

  static void DataTransferBegin(uint16_t DataWidth = DATASIZE_16BIT);
  static void DataTransferEnd() { WRITE(TFT_CS_PIN, HIGH); __HAL_SPI_DISABLE(&SPIx); };
  static void DataTransferAbort();

  static void WriteData(uint16_t Data) { Transmit(Data); }
  static void WriteReg(uint16_t Reg) { WRITE(TFT_A0_PIN, LOW); Transmit(Reg); WRITE(TFT_A0_PIN, HIGH); }

  static void WriteSequence_DMA(uint16_t *Data, uint16_t Count) { TransmitDMA(DMA_MINC_ENABLE, Data, Count); }
  static void WriteMultiple_DMA(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_MINC_DISABLE, &Data, Count); }

  #if ENABLED(USE_SPI_DMA_TC)
    static void WriteSequenceIT(uint16_t *Data, uint16_t Count) { TransmitDMA_IT(DMA_MINC_ENABLE, Data, Count); }
    inline static void DMA_IRQHandler() { HAL_DMA_IRQHandler(&TFT_SPI::DMAtx); }
  #endif

  static void WriteSequence(uint16_t *Data, uint16_t Count) { Transmit(DMA_MINC_ENABLE, Data, Count); }
  static void WriteMultiple(uint16_t Color, uint32_t Count) {
    while (Count > 0) {
      Transmit(DMA_MINC_DISABLE, &Color, Count > DMA_MAX_WORDS ? DMA_MAX_WORDS : Count);
      Count = Count > DMA_MAX_WORDS ? Count - DMA_MAX_WORDS : 0;
    }
  }
};
