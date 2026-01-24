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

#include "../../inc/MarlinConfigPre.h"

#if HAS_SPI_TFT || HAS_FSMC_TFT || HAS_LTDC_TFT

#include "tft_io.h"
#include "tft_ids.h"

#if TFT_DRIVER == ST7735 || TFT_DRIVER == AUTO
  #include "st7735.h"
#endif
#if TFT_DRIVER == ST7789 || TFT_DRIVER == AUTO
  #include "st7789v.h"
#endif
#if TFT_DRIVER == ST7796 || TFT_DRIVER == AUTO
  #include "st7796s.h"
#endif
#if TFT_DRIVER == R61505 || TFT_DRIVER == AUTO
  #include "r65105.h"
#endif
#if TFT_DRIVER == ILI9488 || TFT_DRIVER == ILI9488_ID1 || TFT_DRIVER == AUTO
  #include "ili9488.h"
#endif
#if TFT_DRIVER == SSD1963 || TFT_DRIVER == AUTO
  #include "ssd1963.h"
#endif

#include "ili9341.h"
#include "ili9328.h"

#if HAS_LCD_BRIGHTNESS
  #include "../marlinui.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_GRAPHICAL_TFT)
#include "../../core/debug_out.h"

TFT_IO_DRIVER TFT_IO::io;
uint32_t TFT_IO::lcd_id = 0xFFFFFFFF;

void TFT_IO::initTFT() {
  if (lcd_id != 0xFFFFFFFF) return;

  #if PIN_EXISTS(TFT_BACKLIGHT)
    OUT_WRITE(TFT_BACKLIGHT_PIN, LOW);
  #endif

  #if PIN_EXISTS(TFT_RESET)
    OUT_WRITE(TFT_RESET_PIN, HIGH);
    delay(10);
    WRITE(TFT_RESET_PIN, LOW);
    delay(10);
    WRITE(TFT_RESET_PIN, HIGH);
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT)
    WRITE(TFT_BACKLIGHT_PIN, DISABLED(DELAYED_BACKLIGHT_INIT));
    #if HAS_LCD_BRIGHTNESS && DISABLED(DELAYED_BACKLIGHT_INIT)
      ui._set_brightness();
    #endif
  #endif

  //io.init();
  delay(100);

  #if TFT_DRIVER != AUTO
    lcd_id = TFT_DRIVER;
  #endif

  #if TFT_DRIVER == ST7735
    write_esc_sequence(st7735_init);
  #elif TFT_DRIVER == SSD1963
    write_esc_sequence(ssd1963_init);
  #elif TFT_DRIVER == ST7789
    write_esc_sequence(st7789v_init);
  #elif TFT_DRIVER == ST7796
    write_esc_sequence(st7796s_init);
  #elif TFT_DRIVER == R61505
    write_esc_sequence(r61505_init);
  #elif TFT_DRIVER == ILI9328
    write_esc_sequence(ili9328_init);
  #elif TFT_DRIVER == ILI9341
    write_esc_sequence(ili9341_init);
  #elif TFT_DRIVER == ILI9488
    write_esc_sequence(ili9488_init);
  #elif TFT_DRIVER == AUTO // autodetect

    lcd_id = io.getID() & 0xFFFF;

    switch (lcd_id) {
      case LTDC_RGB:
        break;
      case ST7796:
        DEBUG_ECHO_MSG(" ST7796S");       // 480x320
        write_esc_sequence(st7796s_init);
        break;
      case ST7789:
        DEBUG_ECHO_MSG(" ST7789V");       // 320x240
        write_esc_sequence(st7789v_init);
        break;
      case SSD1963:
        DEBUG_ECHO_MSG(" SSD1963");
        write_esc_sequence(ssd1963_init);
        break;
      case ST7735:
        DEBUG_ECHO_MSG(" ST7735");        // 160x128
        write_esc_sequence(st7735_init);
        break;
      case R61505:
        DEBUG_ECHO_MSG(" R61505U");       // 320x240
        write_esc_sequence(r61505_init);
        break;
      case ILI9328:
        DEBUG_ECHO_MSG(" ILI9328");       // 320x240
        write_esc_sequence(ili9328_init);
        break;
      case ILI9341:
        DEBUG_ECHO_MSG(" ILI9341");       // 320x240
        write_esc_sequence(ili9341_init);
        break;
      case ILI9488:
      case ILI9488_ID1:
        DEBUG_ECHO_MSG(" ILI9488");       // 480x320
        write_esc_sequence(ili9488_init);
        break;
      default:
        lcd_id = 0;
    }
  #else
    #error "Unsupported TFT driver"
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT) && ENABLED(DELAYED_BACKLIGHT_INIT)
    TERN(HAS_LCD_BRIGHTNESS, ui._set_brightness(), WRITE(TFT_BACKLIGHT_PIN, HIGH));
  #endif
}

void TFT_IO::set_window(uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax) {
  #ifdef OFFSET_X
    xMin += OFFSET_X; xMax += OFFSET_X;
  #endif
  #ifdef OFFSET_Y
    yMin += OFFSET_Y; yMax += OFFSET_Y;
  #endif

  switch (lcd_id) {
    case LTDC_RGB:
      io.writeReg(0x01);
      io.writeData(xMin);
      io.writeReg(0x02);
      io.writeData(xMax);
      io.writeReg(0x03);
      io.writeData(yMin);
      io.writeReg(0x04);
      io.writeData(yMax);
      io.writeReg(0x00);
      break;
    case ST7735:    // ST7735     160x128
    case ST7789:    // ST7789V    320x240
    case ST7796:    // ST7796     480x320
    case ILI9341:   // ILI9341    320x240
    case ILI9488:   // ILI9488    480x320
    case SSD1963:   // SSD1963
    case ILI9488_ID1: // 0x8066 ILI9488    480x320
      io.dataTransferBegin(DATASIZE_8BIT);

      // CASET: Column Address Set
      io.writeReg(ILI9341_CASET);
      io.writeData((xMin >> 8) & 0xFF);
      io.writeData(xMin & 0xFF);
      io.writeData((xMax >> 8) & 0xFF);
      io.writeData(xMax & 0xFF);

      // RASET: Row Address Set
      io.writeReg(ILI9341_PASET);
      io.writeData((yMin >> 8) & 0xFF);
      io.writeData(yMin & 0xFF);
      io.writeData((yMax >> 8) & 0xFF);
      io.writeData(yMax & 0xFF);

      // RAMWR: Memory Write
      io.writeReg(ILI9341_RAMWR);
      break;
    case R61505:    // R61505U    320x240
    case ILI9328:   // ILI9328    320x240
      io.dataTransferBegin(DATASIZE_16BIT);

      // Mind the mess: with landscape screen orientation 'Horizontal' is Y and 'Vertical' is X
      io.writeReg(ILI9328_HASTART);
      io.writeData(yMin);
      io.writeReg(ILI9328_HAEND);
      io.writeData(yMax);
      io.writeReg(ILI9328_VASTART);
      io.writeData(xMin);
      io.writeReg(ILI9328_VAEND);
      io.writeData(xMax);

      io.writeReg(ILI9328_HASET);
      io.writeData(yMin);
      io.writeReg(ILI9328_VASET);
      io.writeData(xMin);

      io.writeReg(ILI9328_RAMWR);
      break;
    default:
      break;
  }

  io.dataTransferEnd();
}

void TFT_IO::write_esc_sequence(const uint16_t *sequence) {
  uint16_t dataWidth, data;

  dataWidth = *sequence++;
  io.dataTransferBegin(dataWidth);

  for (;;) {
    data = *sequence++;
    if (data != 0xFFFF) {
      io.writeData(data);
      continue;
    }
    data = *sequence++;
    if (data == 0x7FFF) break;
    if (data == 0xFFFF)
      io.writeData(0xFFFF);
    else if (data & 0x8000)
      delay(data & 0x7FFF);
    else if ((data & 0xFF00) == 0)
      io.writeReg(data);
  }

  io.dataTransferEnd();
}

#endif // HAS_SPI_TFT || HAS_FSMC_TFT || HAS_LTDC_TFT
