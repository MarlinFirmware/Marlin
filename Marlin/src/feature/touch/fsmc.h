/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if defined(__STM32F1__) && ENABLED(TOUCH_CALIBRATION)

#if ENABLED(LCD_USE_DMA_FSMC)
  #include <libmaple/dma.h>
  #ifndef FSMC_DMA_DEV
    #define FSMC_DMA_DEV     DMA2
    #define FSMC_DMA_CHANNEL DMA_CH5
  #endif
  extern void LCD_IO_WriteMultiple(uint16_t color, uint32_t count);
#endif

// Timing configuration
#define FSMC_ADDRESS_SETUP_TIME   15  // AddressSetupTime
#define FSMC_DATA_SETUP_TIME      15  // DataSetupTime

#define __IO volatile
#define __ASM __asm
#define __STATIC_INLINE static inline

typedef struct {
  __IO uint16_t REG;
  __IO uint16_t RAM;
} LCD_CONTROLLER_TypeDef;

#define LCD ((LCD_CONTROLLER_TypeDef *) (0x60000000 | 0x0001FFFE)) // FSMC_NE1 FSMC_A16

__attribute__((always_inline)) __STATIC_INLINE void __DSB(void) {
  __ASM volatile ("dsb 0xF":::"memory");
}

#define TOUCH_LCD_IO_Init() {}
uint16_t TOUCH_LCD_IO_ReadReg(uint16_t RegValue);

#define USE_DOGM_IO
#ifdef USE_DOGM_IO
  // HAL (STM32F1 dogm fsmc)
  extern void LCD_IO_WriteData(uint16_t RegValue);
  extern void LCD_IO_WriteReg(uint16_t Reg);
  extern uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
  #define TOUCH_LCD_IO_WriteData(r)  LCD_IO_WriteData(r)
  #define TOUCH_LCD_IO_WriteReg(r)   LCD_IO_WriteReg(r)
  #define TOUCH_LCD_IO_ReadData(r,s) LCD_IO_ReadData(r,s)
#else
  void TOUCH_LCD_IO_WriteData(uint16_t RegValue);
  void TOUCH_LCD_IO_WriteReg(uint16_t Reg);
  uint32_t TOUCH_LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);
  // Rename to avoid conflict with HAL functions
  #define LCD_IO_WriteData(r)  TOUCH_LCD_IO_WriteData(r)
  #define LCD_IO_WriteReg(r)   TOUCH_LCD_IO_WriteReg(r)
#endif

#else // extern... to code for spi tft

  #define TOUCH_LCD_IO_WriteData(r)  /*LCD_IO_WriteData(r)*/
  #define TOUCH_LCD_IO_WriteReg(r)   /*LCD_IO_WriteReg(r)*/
  #define TOUCH_LCD_IO_ReadData(r,s) /*LCD_IO_ReadData(r,s)*/
  #define TOUCH_LCD_IO_ReadReg(x)    /*LCD_IO_ReadReg(x)*/

#endif
