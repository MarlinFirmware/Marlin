
#include "../../../inc/MarlinConfig.h"

#if defined(__STM32F1__) && ENABLED(TOUCH_CALIBRATION)

#include "fsmc.h"
//#include <stm32f1xx_hal.h>
#if 0// Now using HAL one...

static uint32_t fsmcInit;

void TOUCH_LCD_IO_Init(void) {
  if (fsmcInit) { return; }
  fsmcInit = 1;

  rcc_clk_enable(RCC_FSMC);

  /* Data lines */
  gpio_set_mode(GPIOD,  0, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOD,  1, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOD,  8, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOD,  9, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOD, 10, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOD, 14, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOD, 15, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE,  7, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE,  8, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE,  9, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE, 10, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE, 11, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE, 12, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE, 13, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE, 14, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOE, 15, GPIO_AF_OUTPUT_PP);

  gpio_set_mode(GPIOD,  4, GPIO_AF_OUTPUT_PP);   // NOE
  gpio_set_mode(GPIOD,  5, GPIO_AF_OUTPUT_PP);   // NWE

  gpio_set_mode(GPIOD,  7, GPIO_AF_OUTPUT_PP);   // NE1
  gpio_set_mode(GPIOD, 11, GPIO_AF_OUTPUT_PP);   // A16

#ifdef STM32_XL_DENSITY
  FSMC_NOR_PSRAM4_BASE->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
  FSMC_NOR_PSRAM4_BASE->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;
#else
  FSMC_NOR_PSRAM1_BASE->BCR = FSMC_BCR_WREN | FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_MBKEN;
  FSMC_NOR_PSRAM1_BASE->BTR = (FSMC_DATA_SETUP_TIME << 8) | FSMC_ADDRESS_SETUP_TIME;
#endif

  afio_remap(AFIO_REMAP_FSMC_NADV);
}
#endif

void TOUCH_LCD_IO_WriteData(uint16_t RegValue) {
  LCD->RAM = RegValue;
  __DSB();
}

void TOUCH_LCD_IO_WriteReg(uint16_t Reg) {
  LCD->REG = Reg;
  __DSB();
}

uint32_t TOUCH_LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize) {
  volatile uint32_t data;
  LCD->REG = RegValue;
  __DSB();

  data = LCD->RAM; // dummy read

  data = LCD->RAM & 0x00ff;

  while (--ReadSize) {
    data <<= 8;
    data |= (LCD->RAM & 0x00ff);
  }

  return (uint32_t) data;
}

uint16_t TOUCH_LCD_IO_ReadReg(uint16_t RegValue) {
  LCD->REG = RegValue;
  __DSB();

  return LCD->RAM;
}

uint16_t TOUCH_LCD_IO_ReadRam(void) {
  return LCD->RAM;
}

void TOUCH_LCD_Delay(uint32_t milliseconds) {
  delay(milliseconds);
}

void TOUCH_LCD_BacklightOn(void) {
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
}

void TOUCH_LCD_BacklightOff(void) {
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
  digitalWrite(LCD_BACKLIGHT_PIN, LOW);
}

void TOUCH_LCD_Reset(void) {
  pinMode(LCD_RESET_PIN, OUTPUT);
  digitalWrite(LCD_RESET_PIN, LOW);
  delay (100);
  digitalWrite(LCD_RESET_PIN, HIGH);
  delay (100);
}

#endif
