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
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../../inc/MarlinConfig.h"

#if HAS_FSMC_TFT

#include "tft_fsmc.h"
#include "pinconfig.h"

SRAM_HandleTypeDef TFT_FSMC::SRAMx;
DMA_HandleTypeDef TFT_FSMC::DMAtx;

#if !IS_ANET_ET
  LCD_CONTROLLER_TypeDef *TFT_FSMC::LCD;
#endif

void TFT_FSMC::Init() {
  #if !IS_ANET_ET
    uint32_t controllerAddress;
  #endif

  #if PIN_EXISTS(TFT_RESET)
    OUT_WRITE(TFT_RESET_PIN, HIGH);
    HAL_Delay(100);
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT)
    OUT_WRITE(TFT_BACKLIGHT_PIN, HIGH);
  #endif

  FSMC_NORSRAM_TimingTypeDef Timing, ExtTiming;

  #if !IS_ANET_ET
    const uint32_t NSBank = (uint32_t)pinmap_peripheral(digitalPinToPinName(TFT_CS_PIN), PinMap_FSMC_CS);
  #endif

  // Perform the SRAM1 memory initialization sequence
  SRAMx.Instance = FSMC_NORSRAM_DEVICE;
  SRAMx.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  // SRAMx.Init
  SRAMx.Init.NSBank = TERN(IS_ANET_ET, FSMC_NORSRAM_BANK1, NSBank);
  SRAMx.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  SRAMx.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  SRAMx.Init.MemoryDataWidth = TERN(IS_ANET_ET, FSMC_NORSRAM_MEM_BUS_WIDTH_8, FSMC_NORSRAM_MEM_BUS_WIDTH_16);
  SRAMx.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  SRAMx.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  SRAMx.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  SRAMx.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  SRAMx.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  SRAMx.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  SRAMx.Init.ExtendedMode = TERN(IS_ANET_ET, FSMC_EXTENDED_MODE_DISABLE, FSMC_EXTENDED_MODE_ENABLE);
  SRAMx.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  SRAMx.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  #ifdef STM32F4xx
    SRAMx.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  #endif
  // Read Timing - relatively slow to ensure ID information is correctly read from TFT controller
  // Can be decreases from 15-15-24 to 4-4-8 with risk of stability loss
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 24;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = TERN(IS_ANET_ET, 0, 16);
  Timing.DataLatency = TERN(IS_ANET_ET, 0, 17);
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  // Write Timing
  // Can be decreases from 8-15-8 to 0-0-1 with risk of stability loss
  ExtTiming.AddressSetupTime = 8;
  ExtTiming.AddressHoldTime = 15;
  ExtTiming.DataSetupTime = 8;
  ExtTiming.BusTurnAroundDuration = 0;
  ExtTiming.CLKDivision = 16;
  ExtTiming.DataLatency = 17;
  ExtTiming.AccessMode = FSMC_ACCESS_MODE_A;

  __HAL_RCC_FSMC_CLK_ENABLE();

  for (uint16_t i = 0; PinMap_FSMC[i].pin != NC; i++)
    pinmap_pinout(PinMap_FSMC[i].pin, PinMap_FSMC);
  pinmap_pinout(digitalPinToPinName(TFT_CS_PIN), PinMap_FSMC_CS);
  pinmap_pinout(digitalPinToPinName(TFT_RS_PIN), PinMap_FSMC_RS);

  #if !IS_ANET_ET
    controllerAddress = FSMC_BANK1_1;
  #endif

  #ifdef PF0
    switch (NSBank) {
      case FSMC_NORSRAM_BANK2: controllerAddress = FSMC_BANK1_2 ; break;
      case FSMC_NORSRAM_BANK3: controllerAddress = FSMC_BANK1_3 ; break;
      case FSMC_NORSRAM_BANK4: controllerAddress = FSMC_BANK1_4 ; break;
    }
  #endif

  #if !IS_ANET_ET
    controllerAddress |= (uint32_t)pinmap_peripheral(digitalPinToPinName(TFT_RS_PIN), PinMap_FSMC_RS);
  #endif

  HAL_SRAM_Init(&SRAMx, &Timing, &ExtTiming);

  __HAL_RCC_DMA2_CLK_ENABLE();

  #ifdef STM32F1xx
    DMAtx.Instance = DMA2_Channel1;
  #elif defined(STM32F4xx)
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

  #if IS_ANET_ET
    //controllerAddress = (uint32_t)0x60000000U;
    //LCD = (LCD_CONTROLLER_TypeDef *)controllerAddress;
    //LCD->RAM = (uint16_t *)LCD_RAM_ADR;
  #else
    LCD = (LCD_CONTROLLER_TypeDef *)controllerAddress;
  #endif
}

uint32_t TFT_FSMC::GetID() {
  #if IS_ANET_ET
    return 0x8552;
  #else
    uint32_t id;
    WriteReg(0x0000);
    id = LCD->RAM;

    if (id == 0)
      id = ReadID(LCD_READ_ID);
    if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
      id = ReadID(LCD_READ_ID4);
    return id;
  #endif
}

#if !IS_ANET_ET

 uint32_t TFT_FSMC::ReadID(uint16_t Reg) {
   uint32_t id;
   WriteReg(Reg);
   id = LCD->RAM; // dummy read
   id = Reg << 24;
   id |= (LCD->RAM & 0x00FF) << 16;
   id |= (LCD->RAM & 0x00FF) << 8;
   id |= (LCD->RAM & 0x00FF);
   return id;
 }

#endif

bool TFT_FSMC::isBusy() {
  if (__IS_DMA_ENABLED(&DMAtx))
    if (__HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TC_FLAG_INDEX(&DMAtx)) != 0 || __HAL_DMA_GET_FLAG(&DMAtx, __HAL_DMA_GET_TE_FLAG_INDEX(&DMAtx)) != 0)
      Abort();
  return __IS_DMA_ENABLED(&DMAtx);
}

void TFT_FSMC::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  DMAtx.Init.PeriphInc = MemoryIncrease;
  HAL_DMA_Init(&DMAtx);

  __HAL_DMA_CLEAR_FLAG(&DMAtx, __HAL_DMA_GET_TC_FLAG_INDEX(&DMAtx));
  __HAL_DMA_CLEAR_FLAG(&DMAtx, __HAL_DMA_GET_TE_FLAG_INDEX(&DMAtx));

  #ifdef STM32F1xx
    DMAtx.Instance->CNDTR = Count;
    DMAtx.Instance->CPAR = (uint32_t)Data;
    DMAtx.Instance->CMAR = (uint32_t)&(LCD_RAM);
  #elif defined(STM32F4xx)
    //#if IS_ANET_ET
      //DMAtx.Instance->NDTR = (Count*2);
    //#endif
    DMAtx.Instance->NDTR = Count;
    DMAtx.Instance->PAR = (uint32_t)Data;
    DMAtx.Instance->M0AR = (uint32_t)TERN(IS_ANET_ET, LCD_RAM_ADR, &LCD->RAM);
  #endif
  __HAL_DMA_ENABLE(&DMAtx);
}

#endif // HAS_FSMC_TFT
#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
