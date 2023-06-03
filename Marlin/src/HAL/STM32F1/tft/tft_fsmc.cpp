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

#ifdef __STM32F1__

#include "../../../inc/MarlinConfig.h"

#if HAS_FSMC_TFT

#include "tft_fsmc.h"
#include <libmaple/fsmc.h>
#include <libmaple/gpio.h>
#include <libmaple/dma.h>

LCD_CONTROLLER_TypeDef *TFT_FSMC::LCD;

/**
 * FSMC LCD IO
 */
#define __ASM __asm
#define __STATIC_INLINE static inline

__attribute__((always_inline)) __STATIC_INLINE void __DSB() {
  __ASM volatile ("dsb 0xF":::"memory");
}

#define FSMC_CS_NE1   PD7

#if ENABLED(STM32_XL_DENSITY)
  #define FSMC_CS_NE2 PG9
  #define FSMC_CS_NE3 PG10
  #define FSMC_CS_NE4 PG12

  #define FSMC_RS_A0  PF0
  #define FSMC_RS_A1  PF1
  #define FSMC_RS_A2  PF2
  #define FSMC_RS_A3  PF3
  #define FSMC_RS_A4  PF4
  #define FSMC_RS_A5  PF5
  #define FSMC_RS_A6  PF12
  #define FSMC_RS_A7  PF13
  #define FSMC_RS_A8  PF14
  #define FSMC_RS_A9  PF15
  #define FSMC_RS_A10 PG0
  #define FSMC_RS_A11 PG1
  #define FSMC_RS_A12 PG2
  #define FSMC_RS_A13 PG3
  #define FSMC_RS_A14 PG4
  #define FSMC_RS_A15 PG5
#endif

#define FSMC_RS_A16   PD11
#define FSMC_RS_A17   PD12
#define FSMC_RS_A18   PD13
#define FSMC_RS_A19   PE3
#define FSMC_RS_A20   PE4
#define FSMC_RS_A21   PE5
#define FSMC_RS_A22   PE6
#define FSMC_RS_A23   PE2

#if ENABLED(STM32_XL_DENSITY)
  #define FSMC_RS_A24 PG13
  #define FSMC_RS_A25 PG14
#endif

/* Timing configuration */
#define FSMC_ADDRESS_SETUP_TIME   15  // AddressSetupTime
#define FSMC_DATA_SETUP_TIME      15  // DataSetupTime

static uint8_t fsmcInit = 0;
void TFT_FSMC::Init() {
  uint8_t cs = FSMC_CS_PIN, rs = FSMC_RS_PIN;
  uint32_t controllerAddress;

  #if ENABLED(LCD_USE_DMA_FSMC)
    dma_init(FSMC_DMA_DEV);
    dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
    dma_set_priority(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, DMA_PRIORITY_MEDIUM);
  #endif

  struct fsmc_nor_psram_reg_map* fsmcPsramRegion;

  if (fsmcInit) return;
  fsmcInit = 1;

  switch (cs) {
    case FSMC_CS_NE1: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION1; fsmcPsramRegion = FSMC_NOR_PSRAM1_BASE; break;
    #if ENABLED(STM32_XL_DENSITY)
      case FSMC_CS_NE2: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION2; fsmcPsramRegion = FSMC_NOR_PSRAM2_BASE; break;
      case FSMC_CS_NE3: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION3; fsmcPsramRegion = FSMC_NOR_PSRAM3_BASE; break;
      case FSMC_CS_NE4: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION4; fsmcPsramRegion = FSMC_NOR_PSRAM4_BASE; break;
    #endif
    default: return;
  }

  #define _ORADDR(N) controllerAddress |= (_BV32(N) - 2)

  switch (rs) {
    #if ENABLED(STM32_XL_DENSITY)
      case FSMC_RS_A0:  _ORADDR( 1); break;
      case FSMC_RS_A1:  _ORADDR( 2); break;
      case FSMC_RS_A2:  _ORADDR( 3); break;
      case FSMC_RS_A3:  _ORADDR( 4); break;
      case FSMC_RS_A4:  _ORADDR( 5); break;
      case FSMC_RS_A5:  _ORADDR( 6); break;
      case FSMC_RS_A6:  _ORADDR( 7); break;
      case FSMC_RS_A7:  _ORADDR( 8); break;
      case FSMC_RS_A8:  _ORADDR( 9); break;
      case FSMC_RS_A9:  _ORADDR(10); break;
      case FSMC_RS_A10: _ORADDR(11); break;
      case FSMC_RS_A11: _ORADDR(12); break;
      case FSMC_RS_A12: _ORADDR(13); break;
      case FSMC_RS_A13: _ORADDR(14); break;
      case FSMC_RS_A14: _ORADDR(15); break;
      case FSMC_RS_A15: _ORADDR(16); break;
    #endif
    case FSMC_RS_A16: _ORADDR(17); break;
    case FSMC_RS_A17: _ORADDR(18); break;
    case FSMC_RS_A18: _ORADDR(19); break;
    case FSMC_RS_A19: _ORADDR(20); break;
    case FSMC_RS_A20: _ORADDR(21); break;
    case FSMC_RS_A21: _ORADDR(22); break;
    case FSMC_RS_A22: _ORADDR(23); break;
    case FSMC_RS_A23: _ORADDR(24); break;
    #if ENABLED(STM32_XL_DENSITY)
      case FSMC_RS_A24: _ORADDR(25); break;
      case FSMC_RS_A25: _ORADDR(26); break;
    #endif
    default: return;
  }

  rcc_clk_enable(RCC_FSMC);

  gpio_set_mode(GPIOD, 14, GPIO_AF_OUTPUT_PP);  // FSMC_D00
  gpio_set_mode(GPIOD, 15, GPIO_AF_OUTPUT_PP);  // FSMC_D01
  gpio_set_mode(GPIOD,  0, GPIO_AF_OUTPUT_PP);  // FSMC_D02
  gpio_set_mode(GPIOD,  1, GPIO_AF_OUTPUT_PP);  // FSMC_D03
  gpio_set_mode(GPIOE,  7, GPIO_AF_OUTPUT_PP);  // FSMC_D04
  gpio_set_mode(GPIOE,  8, GPIO_AF_OUTPUT_PP);  // FSMC_D05
  gpio_set_mode(GPIOE,  9, GPIO_AF_OUTPUT_PP);  // FSMC_D06
  gpio_set_mode(GPIOE, 10, GPIO_AF_OUTPUT_PP);  // FSMC_D07
  gpio_set_mode(GPIOE, 11, GPIO_AF_OUTPUT_PP);  // FSMC_D08
  gpio_set_mode(GPIOE, 12, GPIO_AF_OUTPUT_PP);  // FSMC_D09
  gpio_set_mode(GPIOE, 13, GPIO_AF_OUTPUT_PP);  // FSMC_D10
  gpio_set_mode(GPIOE, 14, GPIO_AF_OUTPUT_PP);  // FSMC_D11
  gpio_set_mode(GPIOE, 15, GPIO_AF_OUTPUT_PP);  // FSMC_D12
  gpio_set_mode(GPIOD,  8, GPIO_AF_OUTPUT_PP);  // FSMC_D13
  gpio_set_mode(GPIOD,  9, GPIO_AF_OUTPUT_PP);  // FSMC_D14
  gpio_set_mode(GPIOD, 10, GPIO_AF_OUTPUT_PP);  // FSMC_D15

  gpio_set_mode(GPIOD,  4, GPIO_AF_OUTPUT_PP);  // FSMC_NOE
  gpio_set_mode(GPIOD,  5, GPIO_AF_OUTPUT_PP);  // FSMC_NWE

  gpio_set_mode(PIN_MAP[cs].gpio_device, PIN_MAP[cs].gpio_bit, GPIO_AF_OUTPUT_PP);  //FSMC_CS_NEx
  gpio_set_mode(PIN_MAP[rs].gpio_device, PIN_MAP[rs].gpio_bit, GPIO_AF_OUTPUT_PP);  //FSMC_RS_Ax

  fsmcPsramRegion->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
  fsmcPsramRegion->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;

  afio_remap(AFIO_REMAP_FSMC_NADV);

  LCD = (LCD_CONTROLLER_TypeDef*)controllerAddress;
}

void TFT_FSMC::Transmit(uint16_t Data) {
  LCD->RAM = Data;
  __DSB();
}

void TFT_FSMC::WriteReg(uint16_t Reg) {
  LCD->REG = Reg;
  __DSB();
}

uint32_t TFT_FSMC::GetID() {
  uint32_t id;
  WriteReg(0x0000);
  id = LCD->RAM;

  if (id == 0)
    id = ReadID(LCD_READ_ID);
  if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
    id = ReadID(LCD_READ_ID4);
  if ((id & 0xFF00) == 0 && (id & 0xFF) != 0)
    id = ReadID(LCD_READ_ID4);
  return id;
}

 uint32_t TFT_FSMC::ReadID(uint16_t Reg) {
   uint32_t id;
   WriteReg(Reg);
   id = LCD->RAM; // dummy read
   id = Reg << 24;
   id |= (LCD->RAM & 0x00FF) << 16;
   id |= (LCD->RAM & 0x00FF) << 8;
   id |= LCD->RAM & 0x00FF;
   return id;
 }

bool TFT_FSMC::isBusy() {
  #define __IS_DMA_CONFIGURED(__DMAx__, __CHx__)   (dma_channel_regs(__DMAx__, __CHx__)->CPAR != 0)

  if (!__IS_DMA_CONFIGURED(FSMC_DMA_DEV, FSMC_DMA_CHANNEL)) return false;

  // Check if DMA transfer error or transfer complete flags are set
  if ((dma_get_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL) & (DMA_ISR_TCIF | DMA_ISR_TEIF)) == 0) return true;

  __DSB();
  Abort();
  return false;
}

void TFT_FSMC::Abort() {
  dma_channel_reg_map *channel_regs = dma_channel_regs(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);

  dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL); // Abort DMA transfer if any

  // Deconfigure DMA
  channel_regs->CCR   = 0U;
  channel_regs->CNDTR = 0U;
  channel_regs->CMAR  = 0U;
  channel_regs->CPAR  = 0U;
}

void TFT_FSMC::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  // TODO: HAL STM32 uses DMA2_Channel1 for FSMC on STM32F1
  dma_setup_transfer(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, Data, DMA_SIZE_16BITS, &LCD->RAM, DMA_SIZE_16BITS, DMA_MEM_2_MEM | MemoryIncrease);
  dma_set_num_transfers(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, Count);
  dma_clear_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
  dma_enable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);

  TERN_(TFT_SHARED_IO, while (isBusy()));
}

void TFT_FSMC::Transmit(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  #if defined(FSMC_DMA_DEV) && defined(FSMC_DMA_CHANNEL)
    dma_setup_transfer(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, Data, DMA_SIZE_16BITS, &LCD->RAM, DMA_SIZE_16BITS, DMA_MEM_2_MEM | MemoryIncrease);
    dma_set_num_transfers(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, Count);
    dma_clear_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
    dma_enable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);

    while ((dma_get_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL) & (DMA_CCR_TEIE | DMA_CCR_TCIE)) == 0) {}
    Abort();
  #endif
}

#endif // HAS_FSMC_TFT

#endif // __STM32F1__
