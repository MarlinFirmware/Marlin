#include "../../../inc/MarlinConfig.h"

#if defined(__STM32F1__) && ENABLED(TOUCH_CALIBRATION)

#include "ili9341.h"

#include "fsmc.h"

void ILI9341_Init(void) {
  TOUCH_LCD_IO_Init();

  /* Sleep In Command */
  LCD_IO_WriteReg(ILI9341_SLEEP_IN);
  /* SW Reset Command */
  LCD_IO_WriteReg(ILI9341_SWRESET);
  /* Wait for 200ms */
  LCD_Delay(200);

  /* Normal display for Driver Down side */
  LCD_IO_WriteReg(ILI9341_NORMAL_DISPLAY);
  LCD_IO_WriteData(0xE8); // MY and ML flipped +  bit 3 RGB and BGR changed.

  /* Color mode 16bits/pixel */
  LCD_IO_WriteReg(ILI9341_COLOR_MODE);
  LCD_IO_WriteData(0x55);

  /* Set Column address CASET */
  LCD_IO_WriteReg(ILI9341_CASET);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(0x01);
  LCD_IO_WriteData(0x3F);
  /* Set Row address RASET */
  LCD_IO_WriteReg(ILI9341_RASET);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(0xEF);

  /* VCOM setting */
  LCD_IO_WriteReg(ILI9341_VCOM_CTRL1);
  LCD_IO_WriteData(0x3E);
  LCD_IO_WriteData(0x28);
  LCD_IO_WriteReg(ILI9341_VCOM_CTRL2);
  LCD_IO_WriteData(0x86);

  /* Frame Rate Control in normal mode */
  LCD_IO_WriteReg(ILI9341_FR_CTRL);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(0x18);

  /* Power Control */
  LCD_IO_WriteReg(ILI9341_POWER_CTRL1);
  LCD_IO_WriteData(0x23);
  LCD_IO_WriteReg(ILI9341_POWER_CTRL2);
  LCD_IO_WriteData(0x10);

 /* Sleep Out Command */
  LCD_IO_WriteReg(ILI9341_SLEEP_OUT);
  /* Wait for 120ms */
  LCD_Delay(120);

  /* Display ON command */
  ILI9341_DisplayOn();
}

void ILI9341_DisplayOn(void) {
  /* Display ON command */
  LCD_IO_WriteReg(ILI9341_DISPLAY_ON);

  /* Sleep Out command */
  LCD_IO_WriteReg(ILI9341_SLEEP_OUT);
}

void ILI9341_WriteRam(void) {
  LCD_IO_WriteReg(ILI9341_WRITE_RAM);
}

void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos) {
  /* CASET: Comumn Addrses Set */
  LCD_IO_WriteReg(ILI9341_CASET);
  LCD_IO_WriteData((Xpos >> 8) & 0xFF);
  LCD_IO_WriteData(Xpos & 0xFF);
  LCD_IO_WriteData(0x01);
  LCD_IO_WriteData(0x3F);

  /* RASET: Row Addrses Set */
  LCD_IO_WriteReg(ILI9341_RASET);
  LCD_IO_WriteData((Ypos >> 8) & 0xFF);
  LCD_IO_WriteData(Ypos & 0xFF);
  LCD_IO_WriteData(0x00);
  LCD_IO_WriteData(0xEF);

  LCD_IO_WriteReg(ILI9341_WRITE_RAM);
}

void ILI9341_SetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) {
  /* CASET: Comumn Addrses Set */
  LCD_IO_WriteReg(ILI9341_CASET);
  LCD_IO_WriteData((Xmin >> 8) & 0xFF);
  LCD_IO_WriteData(Xmin & 0xFF);
  LCD_IO_WriteData((Xmax >> 8) & 0xFF);
  LCD_IO_WriteData(Xmax & 0xFF);

  /* RASET: Row Addrses Set */
  LCD_IO_WriteReg(ILI9341_RASET);
  LCD_IO_WriteData((Ymin >> 8) & 0xFF);
  LCD_IO_WriteData(Ymin & 0xFF);
  LCD_IO_WriteData((Ymax >> 8) & 0xFF);
  LCD_IO_WriteData(Ymax & 0xFF);

  LCD_IO_WriteReg(ILI9341_WRITE_RAM);
}

#endif
