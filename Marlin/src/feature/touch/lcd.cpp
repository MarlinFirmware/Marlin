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

#if ENABLED(TOUCH_CALIBRATION)

#include "lcd.h"
#include "font.h"

extern uint16_t reg00, lcdId;
extern uint16_t color, bgColor;

void lcdInit()
{
  switch(lcdId) {
    case 0x1505: break; // R61505U
    case 0x8552: st7789v_Init(); break; // ST7789V
    case 0x8989: break; // SSD1289
    case 0x9325: break; // ILI9325
    case 0x9328: ili9328_Init(); break; // ILI9328
    case 0x9341: ILI9341_Init(); break; // ILI9341
    case 0x0404: break; // No LCD Controller detected
    default: break; // Unknown LCD Controller
  }
}

void lcdSetWindow(uint16_t Xmin, uint16_t Ymin, uint16_t XMax, uint16_t Ymax)
{
  switch(lcdId) {
  //case 0x1505: break; // R61505U
    case 0x8552: st7789v_SetWindow(Xmin, Ymin, XMax, Ymax); break; // ST7789V
  //case 0x8989: break; // SSD1289
  //case 0x9325: break; // ILI9325
    case 0x9328: ili9328_SetWindow(Xmin, Ymin, XMax, Ymax); break; // ILI9328
    case 0x9341: ILI9341_SetWindow(Xmin, Ymin, XMax, Ymax); break; // ILI9341
    case 0x0404: break; // No LCD Controller detected
    default: // Unknown LCD Controller
      if (reg00 != 0)
        ili9328_SetWindow(Xmin, Ymin, XMax, Ymax); // try "0x20 0x21 0x22" commands
      else
        st7789v_SetWindow(Xmin, Ymin, XMax, Ymax); // try "0x2A 0x2B 0x2C" commands
      break;
  }
}

void lcdWriteRam()
{
  switch(lcdId) {
  //case 0x1505: break; // R61505U
    case 0x8552: st7789v_WriteRam(); break; // ST7789V
  //case 0x8989: break; // SSD1289
  //case 0x9325: break; // ILI9325
    case 0x9328: ili9328_WriteRam(); break; // ILI9328
    case 0x9341: ILI9341_WriteRam(); break; // ILI9341
    case 0x0404: break; // No LCD Controller detected
    default: // Unknown LCD Controller
      if (reg00 == 0)
        ili9328_WriteRam(); // try "0x20 0x21 0x22" commands
      else
        st7789v_WriteRam(); // try "0x2A 0x2B 0x2C" commands
      break;
  }
}

void lcdPrint(const uint16_t x, const uint16_t y, char* const text)
{
  for (uint16_t l = 0; (*(uint8_t*)(text + l) != 0) && ((x + l * 8 + 8) < 320); l ++) {
    const uint8_t character = (*(uint8_t*)(text + l) < 32 || *(uint8_t*)(text + l) > 127) ? 0 : *(text + l) - 32;
    lcdSetWindow(x + l * 8, y, x + l * 8 + 7, y + 15);
    for (uint16_t i = 0; i < 2; i++)
      for (uint16_t j = 0; j < 8; j++)
        for (uint16_t k = 0; k < 16; k += 2)
          TOUCH_LCD_IO_WriteData(font[character][i + k] & (128 >> j) ? color : bgColor);
  }
}

void lcdFill(uint16_t color, uint32_t count)
{
  #if ENABLED(LCD_USE_DMA_FSMC)
    #ifdef USE_DOGM_IO
      LCD_IO_WriteMultiple(color, count);
    #else
      while (count > 0) {
        dma_setup_transfer(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, &color, DMA_SIZE_16BITS, &LCD->RAM, DMA_SIZE_16BITS, DMA_MEM_2_MEM);
        dma_set_num_transfers(FSMC_DMA_DEV, FSMC_DMA_CHANNEL, count > 65535 ? 65535 : count);
        dma_clear_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);
        dma_enable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);

        while ((dma_get_isr_bits(FSMC_DMA_DEV, FSMC_DMA_CHANNEL) & 0x0A) == 0) {};
        dma_disable(FSMC_DMA_DEV, FSMC_DMA_CHANNEL);

        count = count > 65535 ? count - 65535 : 0;
      }
    #endif
  #else
    // may have more optimised ways for spi...
    for (uint32_t c=0; c < count; c++)
      TOUCH_LCD_IO_WriteData(color);
  #endif
}

void lcdClear(uint16_t color)
{
  lcdSetWindow(0, 0, 319, 239);
  lcdFill(color, 76800);
}

void lcdBacklightOn() {
  #if PIN_EXISTS(LCD_BACKLIGHT)
    OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH);
  #endif
}

void lcdBacklightOff() {
  #if PIN_EXISTS(LCD_BACKLIGHT)
    OUT_WRITE(LCD_BACKLIGHT_PIN, LOW);
  #endif
}

void lcdReset() {
  #if PIN_EXISTS(LCD_RESET)
    OUT_WRITE(LCD_RESET_PIN, LOW);
    LCD_Delay(100);
    WRITE(LCD_RESET_PIN, HIGH);
    LCD_Delay(100);
  #endif
}

#endif // TOUCH_CALIBRATION
