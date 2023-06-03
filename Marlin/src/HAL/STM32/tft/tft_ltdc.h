/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef STM32H7xx
  #include "stm32h7xx_hal.h"
#else
  #error "LTDC TFT is currently only supported on STM32H7 hardware."
#endif

#define DATASIZE_8BIT  SPI_DATASIZE_8BIT
#define DATASIZE_16BIT SPI_DATASIZE_16BIT
#define TFT_IO_DRIVER  TFT_LTDC
#define DMA_MAX_SIZE   0xFFFF

#define TFT_DATASIZE DATASIZE_16BIT
typedef uint16_t tft_data_t;

class TFT_LTDC {
  private:
    static volatile uint16_t *framebuffer;
    static uint16_t x_min, x_max, y_min, y_max, x_cur, y_cur;
    static uint8_t reg;

    static uint32_t ReadID(tft_data_t Reg);

    static uint16_t ReadPoint(uint16_t x, uint16_t y);
    static void DrawPoint(uint16_t x, uint16_t y, uint16_t color);
    static void DrawRect(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
    static void DrawImage(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *colors);
    static void Transmit(tft_data_t Data);
    static void Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);

  public:
    static void Init();
    static uint32_t GetID();
    static bool isBusy();
    static void Abort() { /*__HAL_DMA_DISABLE(&DMAtx);*/ }

    static void DataTransferBegin(uint16_t DataWidth=TFT_DATASIZE) {}
    static void DataTransferEnd() {};

    static void WriteData(uint16_t Data);
    static void WriteReg(uint16_t Reg);

    // Non-blocking DMA data transfer is not implemented for LTDC interface
    inline static void WriteSequence_DMA(uint16_t *Data, uint16_t Count) { WriteSequence(Data, Count); }
    inline static void WriteMultiple_DMA(uint16_t Color, uint16_t Count) { WriteMultiple(Color, Count); }

    static void WriteSequence(uint16_t *Data, uint16_t Count) { Transmit(DMA_PINC_ENABLE, Data, Count); }
    static void WriteMultiple(uint16_t Color, uint32_t Count) {
      while (Count > 0) {
        Transmit(DMA_PINC_DISABLE, &Color, Count > DMA_MAX_SIZE ? DMA_MAX_SIZE : Count);
        Count = Count > DMA_MAX_SIZE ? Count - DMA_MAX_SIZE : 0;
      }
    }
};

const PinMap PinMap_LTDC[] = {
  {PF_10, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_DE
  {PG_7,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_CLK
  {PI_9,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_VSYNC
  {PI_10, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_HSYNC

  {PG_6,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_R7
  {PH_12, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_R6
  {PH_11, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_R5
  {PH_10, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_R4
  {PH_9,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_R3

  {PI_2,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_G7
  {PI_1,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_G6
  {PI_0,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_G5
  {PH_15, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_G4
  {PH_14, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_G3
  {PH_13, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_G2

  {PI_7,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_B7
  {PI_6,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_B6
  {PI_5,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_B5
  {PI_4,  LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_B4
  {PG_11, LTDC, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF14_LTDC)}, // LCD_B3
  {NC,    NP,    0}
};

const PinMap PinMap_SDRAM[] = {
  {PC_0,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_SDNWE
  {PC_2,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_SDNE0
  {PC_3,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_SDCKE0
  {PE_0,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_NBL0
  {PE_1,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_NBL1
  {PF_11, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_SDNRAS
  {PG_8,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_SDCLK
  {PG_15, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_SDNCAS
  {PG_4,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_BA0
  {PG_5,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_BA1
  {PD_14, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D0
  {PD_15, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D1
  {PD_0,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D2
  {PD_1,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D3
  {PE_7,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D4
  {PE_8,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D5
  {PE_9,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D6
  {PE_10, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D7
  {PE_11, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D8
  {PE_12, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D9
  {PE_13, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D10
  {PE_14, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D11
  {PE_15, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D12
  {PD_8,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D13
  {PD_9,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D14
  {PD_10, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_D15
  {PF_0,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A0
  {PF_1,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A1
  {PF_2,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A2
  {PF_3,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A3
  {PF_4,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A4
  {PF_5,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A5
  {PF_12, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A6
  {PF_13, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A7
  {PF_14, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A8
  {PF_15, FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A9
  {PG_0,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A10
  {PG_1,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A11
  {PG_2,  FMC_Bank1_R, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF12_FMC)}, // FMC_A12
  {NC,    NP,    0}
};

const PinMap PinMap_QUADSPI[] = {
  {PB_2,  QUADSPI, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_QUADSPI)}, // QUADSPI_CLK
  {PB_10, QUADSPI, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_QUADSPI)}, // QUADSPI_BK1_NCS
  {PF_6,  QUADSPI, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_QUADSPI)}, // QUADSPI_BK1_IO3
  {PF_7,  QUADSPI, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF9_QUADSPI)}, // QUADSPI_BK1_IO2
  {PF_8,  QUADSPI, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF10_QUADSPI)}, // QUADSPI_BK1_IO0
  {PF_9,  QUADSPI, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF10_QUADSPI)}, // QUADSPI_BK1_IO1
  {NC,    NP,    0}
};
