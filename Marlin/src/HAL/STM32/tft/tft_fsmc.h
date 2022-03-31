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

#ifdef STM32F1xx
  #include "stm32f1xx_hal.h"
#elif defined(STM32F4xx)
  #include "stm32f4xx_hal.h"
#else
  #error "FSMC TFT is currently only supported on STM32F1 and STM32F4 hardware."
#endif

#ifndef LCD_READ_ID
  #define LCD_READ_ID  0x04   // Read display identification information (0xD3 on ILI9341)
#endif
#ifndef LCD_READ_ID4
  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
#endif

#define DATASIZE_8BIT  SPI_DATASIZE_8BIT
#define DATASIZE_16BIT SPI_DATASIZE_16BIT
#define TFT_IO_DRIVER  TFT_FSMC

#define TFT_DATASIZE TERN(TFT_INTERFACE_FSMC_8BIT, DATASIZE_8BIT, DATASIZE_16BIT)
typedef TERN(TFT_INTERFACE_FSMC_8BIT, uint8_t, uint16_t) tft_data_t;

typedef struct {
  __IO tft_data_t REG;
  __IO tft_data_t RAM;
} LCD_CONTROLLER_TypeDef;

class TFT_FSMC {
  private:
    static SRAM_HandleTypeDef SRAMx;
    static DMA_HandleTypeDef DMAtx;

    static LCD_CONTROLLER_TypeDef *LCD;

    static uint32_t ReadID(tft_data_t Reg);
    static void Transmit(tft_data_t Data) { LCD->RAM = Data; __DSB(); }
    static void TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count);

  public:
    static void Init();
    static uint32_t GetID();
    static bool isBusy();
    static void Abort() { __HAL_DMA_DISABLE(&DMAtx); }

    static void DataTransferBegin(uint16_t DataWidth = TFT_DATASIZE) {}
    static void DataTransferEnd() {};

    static void WriteData(uint16_t Data) { Transmit(tft_data_t(Data)); }
    static void WriteReg(uint16_t Reg) { LCD->REG = tft_data_t(Reg); __DSB(); }

    static void WriteSequence(uint16_t *Data, uint16_t Count) { TransmitDMA(DMA_PINC_ENABLE, Data, Count); }
    static void WriteMultiple(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_PINC_DISABLE, &Data, Count); }
    static void WriteMultiple(uint16_t Color, uint32_t Count) {
      static uint16_t Data; Data = Color;
      while (Count > 0) {
        TransmitDMA(DMA_MINC_DISABLE, &Data, Count > 0xFFFF ? 0xFFFF : Count);
        Count = Count > 0xFFFF ? Count - 0xFFFF : 0;
      }
    }
};

#ifdef STM32F1xx
  #define FSMC_PIN_DATA   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, AFIO_NONE)
#elif defined(STM32F4xx)
  #define FSMC_PIN_DATA   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF12_FSMC)
  #define FSMC_BANK1_1    0x60000000U
  #define FSMC_BANK1_2    0x64000000U
  #define FSMC_BANK1_3    0x68000000U
  #define FSMC_BANK1_4    0x6C000000U
#else
  #error No configuration for this MCU
#endif

const PinMap PinMap_FSMC[] = {
  {PD_14,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D00
  {PD_15,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D01
  {PD_0,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D02
  {PD_1,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D03
  {PE_7,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D04
  {PE_8,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D05
  {PE_9,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D06
  {PE_10,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D07
  #if DISABLED(TFT_INTERFACE_FSMC_8BIT)
    {PE_11,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D08
    {PE_12,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D09
    {PE_13,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D10
    {PE_14,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D11
    {PE_15,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D12
    {PD_8,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D13
    {PD_9,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D14
    {PD_10,  FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_D15
  #endif
  {PD_4,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_NOE
  {PD_5,   FSMC_NORSRAM_DEVICE, FSMC_PIN_DATA}, // FSMC_NWE
  {NC,    NP,    0}
};

const PinMap PinMap_FSMC_CS[] = {
  {PD_7,  (void *)FSMC_NORSRAM_BANK1, FSMC_PIN_DATA}, // FSMC_NE1
  #ifdef PF0
    {PG_9,  (void *)FSMC_NORSRAM_BANK2, FSMC_PIN_DATA}, // FSMC_NE2
    {PG_10, (void *)FSMC_NORSRAM_BANK3, FSMC_PIN_DATA}, // FSMC_NE3
    {PG_12, (void *)FSMC_NORSRAM_BANK4, FSMC_PIN_DATA}, // FSMC_NE4
  #endif
  {NC,    NP,    0}
};

#if ENABLED(TFT_INTERFACE_FSMC_8BIT)
  #define FSMC_RS(A)  (void *)((2 << (A-1)) - 1)
#else
  #define FSMC_RS(A)  (void *)((2 << A) - 2)
#endif

const PinMap PinMap_FSMC_RS[] = {
  #ifdef PF0
    {PF_0,  FSMC_RS( 0), FSMC_PIN_DATA}, // FSMC_A0
    {PF_1,  FSMC_RS( 1), FSMC_PIN_DATA}, // FSMC_A1
    {PF_2,  FSMC_RS( 2), FSMC_PIN_DATA}, // FSMC_A2
    {PF_3,  FSMC_RS( 3), FSMC_PIN_DATA}, // FSMC_A3
    {PF_4,  FSMC_RS( 4), FSMC_PIN_DATA}, // FSMC_A4
    {PF_5,  FSMC_RS( 5), FSMC_PIN_DATA}, // FSMC_A5
    {PF_12, FSMC_RS( 6), FSMC_PIN_DATA}, // FSMC_A6
    {PF_13, FSMC_RS( 7), FSMC_PIN_DATA}, // FSMC_A7
    {PF_14, FSMC_RS( 8), FSMC_PIN_DATA}, // FSMC_A8
    {PF_15, FSMC_RS( 9), FSMC_PIN_DATA}, // FSMC_A9
    {PG_0,  FSMC_RS(10), FSMC_PIN_DATA}, // FSMC_A10
    {PG_1,  FSMC_RS(11), FSMC_PIN_DATA}, // FSMC_A11
    {PG_2,  FSMC_RS(12), FSMC_PIN_DATA}, // FSMC_A12
    {PG_3,  FSMC_RS(13), FSMC_PIN_DATA}, // FSMC_A13
    {PG_4,  FSMC_RS(14), FSMC_PIN_DATA}, // FSMC_A14
    {PG_5,  FSMC_RS(15), FSMC_PIN_DATA}, // FSMC_A15
  #endif
  {PD_11, FSMC_RS(16), FSMC_PIN_DATA}, // FSMC_A16
  {PD_12, FSMC_RS(17), FSMC_PIN_DATA}, // FSMC_A17
  {PD_13, FSMC_RS(18), FSMC_PIN_DATA}, // FSMC_A18
  {PE_3,  FSMC_RS(19), FSMC_PIN_DATA}, // FSMC_A19
  {PE_4,  FSMC_RS(20), FSMC_PIN_DATA}, // FSMC_A20
  {PE_5,  FSMC_RS(21), FSMC_PIN_DATA}, // FSMC_A21
  {PE_6,  FSMC_RS(22), FSMC_PIN_DATA}, // FSMC_A22
  {PE_2,  FSMC_RS(23), FSMC_PIN_DATA}, // FSMC_A23
  #ifdef PF0
    {PG_13, FSMC_RS(24), FSMC_PIN_DATA}, // FSMC_A24
    {PG_14, FSMC_RS(25), FSMC_PIN_DATA}, // FSMC_A25
  #endif
  {NC,    NP,    0}
};
