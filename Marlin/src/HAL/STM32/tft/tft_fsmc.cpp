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

#include "../../platforms.h"

#ifdef HAL_STM32

#include "../../../inc/MarlinConfig.h"

#if HAS_FSMC_TFT

#include "tft_fsmc.h"
#include "pinconfig.h"

SRAM_HandleTypeDef TFT_FSMC::SRAMx;
DMA_HandleTypeDef TFT_FSMC::DMAtx;
LCD_CONTROLLER_TypeDef *TFT_FSMC::LCD;

void TFT_FSMC::init() {
  uint32_t controllerAddress;
  uint32_t nsBank = (uint32_t)pinmap_peripheral(digitalPinToPinName(TFT_CS_PIN), PINMAP_TFT_CS);
  
  #if defined(FMC_NORSRAM_DEVICE)
    FMC_NORSRAM_TimingTypeDef timing, extTiming;

    // Perform the SRAM1 memory initialization sequence
    SRAMx.Instance = FMC_NORSRAM_DEVICE;
    SRAMx.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    // SRAMx.Init
    SRAMx.Init.NSBank = nsBank;
    SRAMx.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
    SRAMx.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
    SRAMx.Init.MemoryDataWidth = TERN(TFT_INTERFACE_FMC_8BIT, FMC_NORSRAM_MEM_BUS_WIDTH_8, FMC_NORSRAM_MEM_BUS_WIDTH_16);
    SRAMx.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    SRAMx.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    SRAMx.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
    SRAMx.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    SRAMx.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
    SRAMx.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    SRAMx.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
    SRAMx.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    
    SRAMx.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    SRAMx.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;

    SRAMx.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    SRAMx.Init.PageSize = FMC_PAGE_SIZE_NONE;
  #elif defined(FSMC_NORSRAM_DEVICE)
    FSMC_NORSRAM_TimingTypeDef timing, extTiming;
    
    // Perform the SRAM1 memory initialization sequence
    SRAMx.Instance = FSMC_NORSRAM_DEVICE;
    SRAMx.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    // SRAMx.Init
    SRAMx.Init.NSBank = nsBank;
    SRAMx.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    SRAMx.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    SRAMx.Init.MemoryDataWidth = TERN(TFT_INTERFACE_FSMC_8BIT, FSMC_NORSRAM_MEM_BUS_WIDTH_8, FSMC_NORSRAM_MEM_BUS_WIDTH_16);
    SRAMx.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    SRAMx.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    SRAMx.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    SRAMx.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    SRAMx.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    SRAMx.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    SRAMx.Init.ExtendedMode = FSMC_EXTENDED_MODE_ENABLE;
    SRAMx.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    SRAMx.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    #ifdef STM32F4xx
      SRAMx.Init.PageSize = FSMC_PAGE_SIZE_NONE;
    #endif
  #else
    #error "Selected MCU does not support FSMC/FMC"
  #endif
  
  // Read Timing - relatively slow to ensure ID information is correctly read from TFT controller
  // Can be decreases from 15-15-24 to 4-4-8 with risk of stability loss
  timing.AddressSetupTime = 15;
  timing.AddressHoldTime = 15;
  timing.DataSetupTime = 24;
  timing.BusTurnAroundDuration = 0;
  timing.CLKDivision = 16;
  timing.DataLatency = 17;
  
  // Write Timing
  // Can be decreased from 8-15-8 to 0-0-1 with risk of stability loss
  extTiming.AddressSetupTime = 8;
  extTiming.AddressHoldTime = 15;
  extTiming.DataSetupTime = 8;
  extTiming.BusTurnAroundDuration = 0;
  extTiming.CLKDivision = 16;
  extTiming.DataLatency = 17;
  
  #if defined(FMC_NORSRAM_DEVICE)
    timing.AccessMode = FMC_ACCESS_MODE_A;
    extTiming.AccessMode = FMC_ACCESS_MODE_A;
    __HAL_RCC_FMC_CLK_ENABLE();
    controllerAddress = FMC_BANK1_1;
  #else
    timing.AccessMode = FSMC_ACCESS_MODE_A;
    extTiming.AccessMode = FSMC_ACCESS_MODE_A;
    __HAL_RCC_FSMC_CLK_ENABLE();
    controllerAddress = FSMC_BANK1_1;
  #endif

  for (uint16_t i = 0; PINMAP_TFT_IO[i].pin != NC; i++)
    pinmap_pinout(PINMAP_TFT_IO[i].pin, PINMAP_TFT_IO);
  pinmap_pinout(digitalPinToPinName(TFT_CS_PIN), PINMAP_TFT_CS);
  pinmap_pinout(digitalPinToPinName(TFT_RS_PIN), PINMAP_TFT_RS);

  // Note: Code below assume that MCU in LQFP-100 package has only one NE pin. STM32H743 has two - NE1 and NE2.
  #ifdef PF0
    switch (nsBank) {
      #if defined(FMC_NORSRAM_DEVICE)
        case FMC_NORSRAM_BANK2: controllerAddress = FMC_BANK1_2 ; break;
        case FMC_NORSRAM_BANK3: controllerAddress = FMC_BANK1_3 ; break;
        case FMC_NORSRAM_BANK4: controllerAddress = FMC_BANK1_4 ; break;
      #else
        case FSMC_NORSRAM_BANK2: controllerAddress = FSMC_BANK1_2 ; break;
        case FSMC_NORSRAM_BANK3: controllerAddress = FSMC_BANK1_3 ; break;
        case FSMC_NORSRAM_BANK4: controllerAddress = FSMC_BANK1_4 ; break;
      #endif
    }
  #endif

  controllerAddress |= (uint32_t)pinmap_peripheral(digitalPinToPinName(TFT_RS_PIN), PINMAP_TFT_RS);

  HAL_SRAM_Init(&SRAMx, &timing, &extTiming);

  #ifdef STM32F1xx
    __HAL_RCC_DMA1_CLK_ENABLE();
    DMAtx.Instance = DMA1_Channel1;
  #elif defined(STM32F4xx)
    __HAL_RCC_DMA2_CLK_ENABLE();
    DMAtx.Instance = DMA2_Stream0;
    DMAtx.Init.Channel = DMA_CHANNEL_0;
    DMAtx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    DMAtx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    DMAtx.Init.MemBurst = DMA_MBURST_SINGLE;
    DMAtx.Init.PeriphBurst = DMA_PBURST_SINGLE;
  #endif

  DMAtx.Init.Direction = DMA_MEMORY_TO_MEMORY;
  DMAtx.Init.MemInc = DMA_MINC_DISABLE;
  DMAtx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  DMAtx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DMAtx.Init.Mode = DMA_NORMAL;
  DMAtx.Init.Priority = DMA_PRIORITY_HIGH;

  LCD = (LCD_CONTROLLER_TypeDef *)controllerAddress;
}

uint32_t TFT_FSMC::getID() {
  uint32_t id;
  writeReg(0);
  id = LCD->RAM;

  if (id == 0)
    id = readID(LCD_READ_ID);
  if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
    id = readID(LCD_READ_ID4);
  return id;
}

uint32_t TFT_FSMC::readID(const tft_data_t inReg) {
  uint32_t id;
  writeReg(inReg);
  id = LCD->RAM; // dummy read
  id = inReg << 24;
  id |= (LCD->RAM & 0x00FF) << 16;
  id |= (LCD->RAM & 0x00FF) << 8;
  id |= LCD->RAM & 0x00FF;
  return id;
}

bool TFT_FSMC::isBusy() {
  #ifdef STM32F1xx
    #define __IS_DMA_ENABLED(__HANDLE__)      ((__HANDLE__)->Instance->CCR & DMA_CCR_EN)
    #define __IS_DMA_CONFIGURED(__HANDLE__)   ((__HANDLE__)->Instance->CPAR != 0)
  #elif defined(STM32F4xx)
    #define __IS_DMA_ENABLED(__HANDLE__)      ((__HANDLE__)->Instance->CR & DMA_SxCR_EN)
    #define __IS_DMA_CONFIGURED(__HANDLE__)   ((__HANDLE__)->Instance->PAR != 0)
  #endif

  if (!__IS_DMA_CONFIGURED(&DMAtx)) return false;

  // Check if DMA transfer error or transfer complete flags are set
  if ((__HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TE_FLAG_INDEX(&DMAtx)) == 0) && (__HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TC_FLAG_INDEX(&DMAtx)) == 0)) return true;

  __DSB();
  abort();
  return false;
}

void TFT_FSMC::abort() {
  HAL_DMA_Abort(&DMAtx);  // Abort DMA transfer if any
  HAL_DMA_DeInit(&DMAtx); // Deconfigure DMA
}

void TFT_FSMC::transmitDMA(uint32_t memoryIncrease, uint16_t *data, uint16_t count) {
  DMAtx.Init.PeriphInc = memoryIncrease;
  HAL_DMA_Init(&DMAtx);
  HAL_DMA_Start(&DMAtx, (uint32_t)data, (uint32_t)&(LCD->RAM), count);

  TERN_(TFT_SHARED_IO, while (isBusy()));
}

void TFT_FSMC::transmit(uint32_t memoryIncrease, uint16_t *data, uint16_t count) {
  DMAtx.Init.PeriphInc = memoryIncrease;
  HAL_DMA_Init(&DMAtx);
  dataTransferBegin();
  HAL_DMA_Start(&DMAtx, (uint32_t)data, (uint32_t)&(LCD->RAM), count);
  HAL_DMA_PollForTransfer(&DMAtx, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
  abort();
}

#endif // HAS_FSMC_TFT
#endif // HAL_STM32