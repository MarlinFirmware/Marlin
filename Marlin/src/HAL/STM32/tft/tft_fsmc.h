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
  #error "FSMC/FMC TFT is currently only supported on STM32F1 and STM32F4 hardware."
#endif

#ifndef HAL_SRAM_MODULE_ENABLED
  #error "SRAM module disabled for the STM32 framework (HAL_SRAM_MODULE_ENABLED)! Please consult the development team."
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
#define DMA_MAX_WORDS  0xFFFF

#if ANY(TFT_INTERFACE_FSMC_8BIT, TFT_INTERFACE_FMC_8BIT)
  #define TFT_DATASIZE DATASIZE_8BIT
  typedef uint8_t tft_data_t;
#else
  #define TFT_DATASIZE DATASIZE_16BIT
  typedef uint16_t tft_data_t;
#endif

typedef struct {
  __IO tft_data_t REG;
  __IO tft_data_t RAM;
} LCD_CONTROLLER_TypeDef;

class TFT_FSMC {
  private:
    static SRAM_HandleTypeDef SRAMx;
    static DMA_HandleTypeDef DMAtx;

    static LCD_CONTROLLER_TypeDef *LCD;

    static uint32_t readID(const tft_data_t reg);
    static void transmit(tft_data_t data) { LCD->RAM = data; __DSB(); }
    static void transmit(uint32_t memoryIncrease, uint16_t *data, uint16_t count);
    static void transmitDMA(uint32_t memoryIncrease, uint16_t *data, uint16_t count);

  public:
    static void init();
    static uint32_t getID();
    static bool isBusy();
    static void abort();

    static void dataTransferBegin(uint16_t dataWidth=TFT_DATASIZE) {}
    static void dataTransferEnd() {}

    static void writeData(uint16_t data) { transmit(tft_data_t(data)); }
    static void writeReg(const uint16_t inReg) { LCD->REG = tft_data_t(inReg); __DSB(); }

    static void writeSequence_DMA(uint16_t *data, uint16_t count) { transmitDMA(DMA_PINC_ENABLE, data, count); }
    static void writeMultiple_DMA(uint16_t color, uint16_t count) { static uint16_t data; data = color; transmitDMA(DMA_PINC_DISABLE, &data, count); }

    static void writeSequence(uint16_t *data, uint16_t count) { transmit(DMA_PINC_ENABLE, data, count); }
    static void writeMultiple(uint16_t color, uint32_t count) {
      while (count > 0) {
        transmit(DMA_MINC_DISABLE, &color, count > DMA_MAX_WORDS ? DMA_MAX_WORDS : count);
        count = count > DMA_MAX_WORDS ? count - DMA_MAX_WORDS : 0;
      }
    }
};

#ifdef FMC_NORSRAM_DEVICE // Flexible Memory Controller on STM32F446

  #ifdef STM32F4xx
    #define FMC_PIN_DATA   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF12_FMC)
    #define FMC_BANK1_1    0x60000000U
    #define FMC_BANK1_2    0x64000000U
    #define FMC_BANK1_3    0x68000000U
    #define FMC_BANK1_4    0x6C000000U
  #else
    #error "No configuration for this MCU"
  #endif

  #define PINMAP_TFT_IO     pinMap_FMC
  #define PINMAP_TFT_CS     pinMap_FMC_CS
  #define PINMAP_TFT_RS     pinMap_FMC_RS

  const PinMap pinMap_FMC[] = {
    {PD_14,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D00
    {PD_15,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D01
    {PD_0,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D02
    {PD_1,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D03
    {PE_7,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D04
    {PE_8,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D05
    {PE_9,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D06
    {PE_10,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D07
    #if DISABLED(TFT_INTERFACE_FMC_8BIT)
      {PE_11,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D08
      {PE_12,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D09
      {PE_13,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D10
      {PE_14,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D11
      {PE_15,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D12
      {PD_8,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D13
      {PD_9,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D14
      {PD_10,  FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_D15
    #endif
    {PD_4,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_NOE
    {PD_5,   FMC_NORSRAM_DEVICE, FMC_PIN_DATA}, // FMC_NWE
    {NC,    NP,    0}
  };

  const PinMap pinMap_FMC_CS[] = {
    {PD_7,  (void *)FMC_NORSRAM_BANK1, FMC_PIN_DATA}, // FMC_NE1
    #ifdef PF0
      {PG_9,  (void *)FMC_NORSRAM_BANK2, FMC_PIN_DATA}, // FMC_NE2
      {PG_10, (void *)FMC_NORSRAM_BANK3, FMC_PIN_DATA}, // FMC_NE3
      {PG_12, (void *)FMC_NORSRAM_BANK4, FMC_PIN_DATA}, // FMC_NE4
    #endif
    {NC,    NP,    0}
  };

  #if ENABLED(TFT_INTERFACE_FMC_8BIT)
    #define FMC_RS(A) (void *)((2 << (A - 1)) - 1)
  #else
    #define FMC_RS(A) (void *)((2 << A) - 2)
  #endif

  const PinMap pinMap_FMC_RS[] = {
    #ifdef PF0
      {PF_0,  FMC_RS( 0), FMC_PIN_DATA}, // FMC_A0
      {PF_1,  FMC_RS( 1), FMC_PIN_DATA}, // FMC_A1
      {PF_2,  FMC_RS( 2), FMC_PIN_DATA}, // FMC_A2
      {PF_3,  FMC_RS( 3), FMC_PIN_DATA}, // FMC_A3
      {PF_4,  FMC_RS( 4), FMC_PIN_DATA}, // FMC_A4
      {PF_5,  FMC_RS( 5), FMC_PIN_DATA}, // FMC_A5
      {PF_12, FMC_RS( 6), FMC_PIN_DATA}, // FMC_A6
      {PF_13, FMC_RS( 7), FMC_PIN_DATA}, // FMC_A7
      {PF_14, FMC_RS( 8), FMC_PIN_DATA}, // FMC_A8
      {PF_15, FMC_RS( 9), FMC_PIN_DATA}, // FMC_A9
      {PG_0,  FMC_RS(10), FMC_PIN_DATA}, // FMC_A10
      {PG_1,  FMC_RS(11), FMC_PIN_DATA}, // FMC_A11
      {PG_2,  FMC_RS(12), FMC_PIN_DATA}, // FMC_A12
      {PG_3,  FMC_RS(13), FMC_PIN_DATA}, // FMC_A13
      {PG_4,  FMC_RS(14), FMC_PIN_DATA}, // FMC_A14
      {PG_5,  FMC_RS(15), FMC_PIN_DATA}, // FMC_A15
    #endif
    {PD_11, FMC_RS(16), FMC_PIN_DATA}, // FMC_A16
    {PD_12, FMC_RS(17), FMC_PIN_DATA}, // FMC_A17
    {PD_13, FMC_RS(18), FMC_PIN_DATA}, // FMC_A18
    {PE_3,  FMC_RS(19), FMC_PIN_DATA}, // FMC_A19
    {PE_4,  FMC_RS(20), FMC_PIN_DATA}, // FMC_A20
    {PE_5,  FMC_RS(21), FMC_PIN_DATA}, // FMC_A21
    {PE_6,  FMC_RS(22), FMC_PIN_DATA}, // FMC_A22
    {PE_2,  FMC_RS(23), FMC_PIN_DATA}, // FMC_A23
    #ifdef PF0
      {PG_13, FMC_RS(24), FMC_PIN_DATA}, // FMC_A24
      {PG_14, FMC_RS(25), FMC_PIN_DATA}, // FMC_A25
    #endif
    {NC,    NP,    0}
  };

#elif defined(FSMC_NORSRAM_DEVICE) // Flexible Static Memory Controller on STM32F103 and STM32F407

  #ifdef STM32F1xx
    #define FSMC_PIN_DATA   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, AFIO_NONE)
  #elif defined(STM32F4xx)
    #define FSMC_PIN_DATA   STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF12_FSMC)
    #define FSMC_BANK1_1    0x60000000U
    #define FSMC_BANK1_2    0x64000000U
    #define FSMC_BANK1_3    0x68000000U
    #define FSMC_BANK1_4    0x6C000000U
  #else
    #error "No configuration for this MCU"
  #endif
  
  #define PINMAP_TFT_IO     pinMap_FSMC
  #define PINMAP_TFT_CS     pinMap_FSMC_CS
  #define PINMAP_TFT_RS     pinMap_FSMC_RS

  const PinMap pinMap_FSMC[] = {
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

  const PinMap pinMap_FSMC_CS[] = {
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

  const PinMap pinMap_FSMC_RS[] = {
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
#else
  #error "Selected MCU does not support FSMC/FMC"
#endif
