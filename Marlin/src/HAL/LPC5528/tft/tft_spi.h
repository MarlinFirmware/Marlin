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
#pragma once

#ifdef TARGET_LPC5528  //caden
//  #include "stm32f1xx_hal.h"
//#elif defined(STM32F4xx)
//  #include "stm32f4xx_hal.h"
//#else
//  #error SPI TFT is currently only supported on STM32F1 and STM32F4 hardware.
#endif

//#ifndef LCD_READ_ID
//  #define LCD_READ_ID 0x04   // Read display identification information (0xD3 on ILI9341)
//#endif
//#ifndef LCD_READ_ID4
//  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
//#endif

#define DMA_MINC_ENABLE 1   // caden
#define DMA_MINC_DISABLE 0  // caden
#define DATASIZE_8BIT    kSPI_Data8Bits   // caden
#define DATASIZE_16BIT   kSPI_Data16Bits  // caden
#define TFT_IO_DRIVER TFT_SPI

class TFT_SPI {
private:
  static uint32_t ReadID(uint16_t Reg);
  static void Transmit(uint16_t Data);
  static void TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);

public:
  static SPIClass SPIx;

  static void Init();
  static uint32_t GetID();
  static bool isBusy();
  static void Abort();

  static void DataTransferBegin(uint16_t DataWidth = DATASIZE_16BIT);
  static void DataTransferEnd() { WRITE(TFT_CS_PIN, HIGH);  HS_SPI.end(); };
  static void DataTransferAbort();

  static void WriteData(uint16_t Data) { Transmit(Data); }
  static void WriteReg(uint16_t Reg) { WRITE(TFT_DC_PIN, LOW); Transmit(Reg); WRITE(TFT_DC_PIN, HIGH); }

  static void WriteSequence(uint16_t *Data, uint16_t Count) { TransmitDMA(DMA_MINC_ENABLE, Data,Count); }
  // static void WriteMultiple(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_MINC_DISABLE, &Data, Count); }
  static void WriteMultiple(uint16_t Color, uint32_t Count) {
    static uint16_t Data; Data = Color;
    //LPC dma can only write 0xFFF bytes at once.
    #define MAX_DMA_SIZE (0x0400 - 1)
    while (Count > 0) {
      TransmitDMA(DMA_MINC_DISABLE, &Data, Count > MAX_DMA_SIZE ? MAX_DMA_SIZE : Count);
      Count = Count > MAX_DMA_SIZE ? Count - MAX_DMA_SIZE : 0;
    }
    #undef MAX_DMA_SIZE
  }
};
