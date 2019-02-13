/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * u8g_com_stm32duino_fsmc.cpp
 *
 * Communication interface for FSMC
 */

#if defined(STM32F1) || defined(STM32F1xx)

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_LCD

#include "U8glib.h"
#include "libmaple/fsmc.h"
#include "libmaple/gpio.h"
#include "boards.h"

#define LCD_READ_ID     0x04   /* Read display identification information */

/* Timing configuration */
#define FSMC_ADDRESS_SETUP_TIME   15  // AddressSetupTime
#define FSMC_DATA_SETUP_TIME      15  // DataSetupTime

void LCD_IO_Init(uint8_t cs, uint8_t rs);
void LCD_IO_WriteData(uint16_t RegValue);
void LCD_IO_WriteReg(uint8_t Reg);
uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize);

static uint8_t msgInitCount = 2; // Ignore all messages until 2nd U8G_COM_MSG_INIT

uint8_t u8g_com_stm32duino_fsmc_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount--;
    if (msgInitCount) return -1;
  }

  static uint8_t isCommand;

  switch(msg) {
    case U8G_COM_MSG_STOP:
      break;
    case U8G_COM_MSG_INIT:
      u8g_SetPIOutput(u8g, U8G_PI_RESET);

      LCD_IO_Init(u8g->pin_list[U8G_PI_CS], u8g->pin_list[U8G_PI_A0]);
      u8g_Delay(100);

      if (arg_ptr != NULL)
        *((uint32_t *)arg_ptr) = LCD_IO_ReadData(LCD_READ_ID, 3);

      isCommand = 0;
      break;

    case U8G_COM_MSG_ADDRESS:           // define cmd (arg_val = 0) or data mode (arg_val = 1)
      isCommand = arg_val == 0 ? 1 : 0;
      break;

    case U8G_COM_MSG_RESET:
      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      if (isCommand)
        LCD_IO_WriteReg(arg_val);
      else
        LCD_IO_WriteData((uint16_t)arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:

      for (uint8_t i = 0; i < arg_val; i += 2)
        LCD_IO_WriteData(*(uint16_t *)(((uint32_t)arg_ptr) + i));
      break;
  }
  return 1;
}

/**
 * FSMC LCD IO
 */
#define __ASM __asm
#define __STATIC_INLINE static inline

__attribute__((always_inline)) __STATIC_INLINE void __DSB(void) {
  __ASM volatile ("dsb 0xF":::"memory");
}

#define FSMC_CS_NE1  PD7
#define FSMC_CS_NE2  PG9
#define FSMC_CS_NE3  PG10
#define FSMC_CS_NE4  PG12

#define FSMC_RS_A0   PF0
#define FSMC_RS_A1   PF1
#define FSMC_RS_A2   PF2
#define FSMC_RS_A3   PF3
#define FSMC_RS_A4   PF4
#define FSMC_RS_A5   PF5
#define FSMC_RS_A6   PF12
#define FSMC_RS_A7   PF13
#define FSMC_RS_A8   PF14
#define FSMC_RS_A9   PF15
#define FSMC_RS_A10  PG0
#define FSMC_RS_A11  PG1
#define FSMC_RS_A12  PG2
#define FSMC_RS_A13  PG3
#define FSMC_RS_A14  PG4
#define FSMC_RS_A15  PG5
#define FSMC_RS_A16  PD11
#define FSMC_RS_A17  PD12
#define FSMC_RS_A18  PD13
#define FSMC_RS_A19  PE3
#define FSMC_RS_A20  PE4
#define FSMC_RS_A21  PE5
#define FSMC_RS_A22  PE6
#define FSMC_RS_A23  PE2
#define FSMC_RS_A24  PG13
#define FSMC_RS_A25  PG14

static uint8_t fsmcInit = 0;

typedef struct {
  __IO uint16_t REG;
  __IO uint16_t RAM;
} LCD_CONTROLLER_TypeDef;

LCD_CONTROLLER_TypeDef *LCD;

void LCD_IO_Init(uint8_t cs, uint8_t rs) {
  uint32_t controllerAddress;

  if (fsmcInit) return;
  fsmcInit = 1;

  switch(cs) {
    case FSMC_CS_NE1: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION1; break;
    case FSMC_CS_NE2: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION2; break;
    case FSMC_CS_NE3: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION3; break;
    case FSMC_CS_NE4: controllerAddress = (uint32_t)FSMC_NOR_PSRAM_REGION4; break;
    default: return;
  }

  #define _ORADDR(N) controllerAddress |= (_BV32(N) - 2)

  switch(rs) {
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
    case FSMC_RS_A16: _ORADDR(17); break;
    case FSMC_RS_A17: _ORADDR(18); break;
    case FSMC_RS_A18: _ORADDR(19); break;
    case FSMC_RS_A19: _ORADDR(20); break;
    case FSMC_RS_A20: _ORADDR(21); break;
    case FSMC_RS_A21: _ORADDR(22); break;
    case FSMC_RS_A22: _ORADDR(23); break;
    case FSMC_RS_A23: _ORADDR(24); break;
    case FSMC_RS_A24: _ORADDR(25); break;
    case FSMC_RS_A25: _ORADDR(26); break;
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

  gpio_set_mode(GPIOD,  4, GPIO_AF_OUTPUT_PP);   // FSMC_NOE
  gpio_set_mode(GPIOD,  5, GPIO_AF_OUTPUT_PP);   // FSMC_NWE

  gpio_set_mode(PIN_MAP[cs].gpio_device, PIN_MAP[cs].gpio_bit, GPIO_AF_OUTPUT_PP);  //FSMC_CS_NEx
  gpio_set_mode(PIN_MAP[rs].gpio_device, PIN_MAP[rs].gpio_bit, GPIO_AF_OUTPUT_PP);  //FSMC_RS_Ax

  FSMC_NOR_PSRAM4_BASE->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
  FSMC_NOR_PSRAM4_BASE->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;

  afio_remap(AFIO_REMAP_FSMC_NADV);

  LCD = (LCD_CONTROLLER_TypeDef*)controllerAddress;
}

void LCD_IO_WriteData(uint16_t RegValue) {
  LCD->RAM = RegValue;
  __DSB();
}

void LCD_IO_WriteReg(uint8_t Reg) {
  LCD->REG = (uint16_t)Reg;
  __DSB();
}

uint32_t LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize) {
  volatile uint32_t data;
  LCD->REG = (uint16_t)RegValue;
  __DSB();

  data = LCD->RAM; // dummy read
  data = LCD->RAM & 0x00FF;

  while (--ReadSize) {
    data <<= 8;
    data |= (LCD->RAM & 0x00FF);
  }
  return (uint32_t)data;
}

#endif // HAS_GRAPHICAL_LCD

#endif // STM32F1 || STM32F1xx
