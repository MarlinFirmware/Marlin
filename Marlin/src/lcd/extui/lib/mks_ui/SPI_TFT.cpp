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

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_LVGL_UI_SPI)

#include "SPI_TFT.h"
#include "pic_manager.h"

#include "../../../../inc/MarlinConfig.h"

#include <SPI.h>

TFT SPI_TFT;

#ifndef SPI_TFT_MISO_PIN
  #define SPI_TFT_MISO_PIN PA6
#endif
#ifndef SPI_TFT_MOSI_PIN
  #define SPI_TFT_MOSI_PIN PA7
#endif
#ifndef SPI_TFT_SCK_PIN
  #define SPI_TFT_SCK_PIN  PA5
#endif
#ifndef SPI_TFT_CS_PIN
  #define SPI_TFT_CS_PIN   PD11
#endif
#ifndef SPI_TFT_DC_PIN
  #define SPI_TFT_DC_PIN   PD10
#endif
#ifndef SPI_TFT_RST_PIN
  #define SPI_TFT_RST_PIN  PC6
#endif

// use SPI1 for the spi tft.
void TFT::spi_init(uint8_t spiRate) {

  SPI_TFT_CS_H;

  /**
   * STM32F1 APB2 = 72MHz, APB1 = 36MHz, max SPI speed of this MCU if 18Mhz
   * STM32F1 has 3 SPI ports, SPI1 in APB2, SPI2/SPI3 in APB1
   * so the minimum prescale of SPI1 is DIV4, SPI2/SPI3 is DIV2
   */
  uint8_t clock;
  switch (spiRate) {
    case SPI_FULL_SPEED:    clock = SPI_CLOCK_DIV4;  break;
    case SPI_HALF_SPEED:    clock = SPI_CLOCK_DIV4; break;
    case SPI_QUARTER_SPEED: clock = SPI_CLOCK_DIV8; break;
    case SPI_EIGHTH_SPEED:  clock = SPI_CLOCK_DIV16; break;
    case SPI_SPEED_5:       clock = SPI_CLOCK_DIV32; break;
    case SPI_SPEED_6:       clock = SPI_CLOCK_DIV64; break;
    default:                clock = SPI_CLOCK_DIV2;        // Default from the SPI library
  }
  SPI.setModule(1);
  SPI.begin();
  SPI.setClockDivider(clock);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
}

uint8_t TFT::spi_Rec() {
  uint8_t returnByte = SPI.transfer(ff);
  return returnByte;
}

uint8_t TFT::spi_read_write_byte(uint8_t data) {
  uint8_t returnByte = SPI.transfer(data);
  return returnByte;
}

/**
 * @brief  Receive a number of bytes from the SPI port to a buffer
 *
 * @param  buf   Pointer to starting address of buffer to write to.
 * @param  nbyte Number of bytes to receive.
 * @return Nothing
 *
 * @details Uses DMA
 */
void TFT::spi_Read(uint8_t* buf, uint16_t nbyte) {SPI.dmaTransfer(0, const_cast<uint8_t*>(buf), nbyte);}

/**
 * @brief  Send a single byte on SPI port
 *
 * @param  b Byte to send
 *
 * @details
 */
void TFT::spi_Send(uint8_t b) {SPI.send(b);}

/**
 * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
 *
 * @param  buf   Pointer with buffer start address
 * @return Nothing
 *
 * @details Use DMA
 */
void TFT::spi_SendBlock(uint8_t token, const uint8_t* buf) {
  SPI.send(token);
  SPI.dmaSend(const_cast<uint8_t*>(buf), 512);
}

void TFT::LCD_WR_REG(uint8_t cmd) {
  SPI_TFT_CS_L;
  SPI_TFT_DC_L;
  spi_Send(cmd);
  SPI_TFT_CS_H;
}
void TFT::LCD_WR_DATA(uint8_t data) {
  SPI_TFT_CS_L;
  SPI_TFT_DC_H;
  spi_Send(data);
  SPI_TFT_CS_H;
}
void TFT::LCD_WriteRAM_Prepare() {LCD_WR_REG(0X2C);}
void TFT::SetCursor(uint16_t x, uint16_t y) {
  LCD_WR_REG(0x2A);
  LCD_WR_DATA(x >> 8);
  LCD_WR_DATA(x);
  LCD_WR_DATA(x >> 8);
  LCD_WR_DATA(x);

  LCD_WR_REG(0x2B);
  LCD_WR_DATA(y >> 8);
  LCD_WR_DATA(y);
  LCD_WR_DATA(y >> 8);
  LCD_WR_DATA(y);
}

void TFT::SetPoint(uint16_t x, uint16_t y, uint16_t point) {
  if ((x > 480) || (y > 320)) return;

  SetCursor(x, y);

  LCD_WriteRAM_Prepare();
  LCD_WR_DATA((uint8_t)(point >> 8));
  LCD_WR_DATA((uint8_t)point);
}

void TFT::SetWindows(uint16_t x, uint16_t y, uint16_t with, uint16_t height) {
  LCD_WR_REG(0x2A);
  LCD_WR_DATA(x >> 8);
  LCD_WR_DATA(x);
  LCD_WR_DATA((x + with - 1) >> 8);
  LCD_WR_DATA((x + with - 1));

  LCD_WR_REG(0x2B);
  LCD_WR_DATA(y >> 8);
  LCD_WR_DATA(y);
  LCD_WR_DATA((y + height - 1) >> 8);
  LCD_WR_DATA(y + height - 1);
}

void TFT::LCD_init() {
  SPI_TFT_RST_H;
  delay(150);
  SPI_TFT_RST_L;
  delay(150);
  SPI_TFT_RST_H;

  delay(120);
  LCD_WR_REG(0x11);
  delay(120);

  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0xC3);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x96);

  LCD_WR_REG(0x36);
  LCD_WR_DATA(0x28);

  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x55);

  LCD_WR_REG(0xB4);
  LCD_WR_DATA(0x01);
  LCD_WR_REG(0xB7);
  LCD_WR_DATA(0xC6);
  LCD_WR_REG(0xE8);
  LCD_WR_DATA(0x40);
  LCD_WR_DATA(0x8A);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x29);
  LCD_WR_DATA(0x19);
  LCD_WR_DATA(0xA5);
  LCD_WR_DATA(0x33);
  LCD_WR_REG(0xC1);
  LCD_WR_DATA(0x06);
  LCD_WR_REG(0xC2);
  LCD_WR_DATA(0xA7);
  LCD_WR_REG(0xC5);
  LCD_WR_DATA(0x18);
  LCD_WR_REG(0xE0);     // Positive Voltage Gamma Control
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x15);
  LCD_WR_DATA(0x2F);
  LCD_WR_DATA(0x54);
  LCD_WR_DATA(0x42);
  LCD_WR_DATA(0x3C);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x18);
  LCD_WR_DATA(0x1B);
  LCD_WR_REG(0xE1);     // Negative Voltage Gamma Control
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x2D);
  LCD_WR_DATA(0x43);
  LCD_WR_DATA(0x42);
  LCD_WR_DATA(0x3B);
  LCD_WR_DATA(0x16);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x1B);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x3C);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x69);
  delay(120);     // Delay 120ms
  LCD_WR_REG(0x29);     // Display ON

  LCD_clear(0x0000);    //
  LCD_Draw_Logo();
  SPI_TFT_BLK_H;
  delay(2000);
}

void TFT::LCD_clear(uint16_t color) {
  unsigned int i;
  uint8_t tbuf[960];

  SetCursor(0, 0);
  SetWindows(0, 0, 480 - 1, 320 - 1);
  LCD_WriteRAM_Prepare();
  SPI_TFT_CS_L;
  SPI_TFT_DC_H;
  for (i = 0; i < 960;) {
    tbuf[i]     = color >> 8;
    tbuf[i + 1] = color;
    i += 2;
  }
  for (i = 0; i < 320; i++) {
    // for (m=0;m<480;m++)
    // {
    // LCD_WR_DATA(color>>8);
    // LCD_WR_DATA(color);
    SPI.dmaSend(tbuf, 960, true);
    // SPI_TFT_CS_H;
    // }
  }
  SPI_TFT_CS_H;
}

extern unsigned char bmp_public_buf[17 * 1024];

void TFT::LCD_Draw_Logo() {
  uint16_t i,y_off = 0;
  uint16_t *p_index;
  uint16_t Color;

  #if 1
    for (y_off = 0; y_off < 320; y_off ++) {
      Pic_Logo_Read((uint8_t *)"", (uint8_t *)bmp_public_buf, 960);

      SPI_TFT.spi_init(SPI_FULL_SPEED);
      SetWindows(0, y_off, 480, 1);
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      for (i = 0; i < 960;) {
        p_index = (uint16_t *)(&bmp_public_buf[i]);
        Color = (*p_index >> 8);
        *p_index = Color | ((*p_index & 0xFF) << 8);
        i+=2;
      }
      SPI_TFT_CS_L;
      SPI_TFT_DC_H;
      SPI.dmaSend(bmp_public_buf,960,true);
      SPI_TFT_CS_H;
    }

  #else

    for (index = 0; index < 40; index ++) {
      Pic_Logo_Read((uint8_t *)"", bmp_public_buf, 480*8*2);
      i = 0;
      SetCursor(0,0);
      SetWindows(0, y_off * 8, 480, 8);
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      for (i = 0; i < 480 * 8 * 2;) {
        p_index = (uint16_t *)(&bmp_public_buf[i]);
        Color = (*p_index >> 8);
        *p_index = Color | ((*p_index & 0xFF) << 8);
        i += 2;
      }
      SPI_TFT_CS_L;
      SPI_TFT_DC_H;
      SPI.dmaSend(bmp_public_buf,480*8*2,true);
      SPI_TFT_CS_H;

      y_off++;
    }
  #endif

  SetWindows(0, 0, 479, 319);
}

#endif // HAS_TFT_LVGL_UI_SPI
