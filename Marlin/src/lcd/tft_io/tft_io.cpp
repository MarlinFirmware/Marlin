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

void TFT_IO::InitTFT() {
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

  // io.Init();
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

    lcd_id = io.GetID() & 0xFFFF;

    switch (lcd_id) {
      case LTDC_RGB:
        break;
      case ST7796:    // ST7796S    480x320
        DEBUG_ECHO_MSG(" ST7796S");
        write_esc_sequence(st7796s_init);
        break;
      case ST7789:    // ST7789V    320x240
        DEBUG_ECHO_MSG(" ST7789V");
        write_esc_sequence(st7789v_init);
        break;
      case SSD1963:    // SSD1963
        DEBUG_ECHO_MSG(" SSD1963");
        write_esc_sequence(ssd1963_init);
        break;
      case ST7735:    // ST7735     160x128
        DEBUG_ECHO_MSG(" ST7735");
        write_esc_sequence(st7735_init);
        break;
      case R61505:    // R61505U    320x240
        DEBUG_ECHO_MSG(" R61505U");
        write_esc_sequence(r61505_init);
        break;
      case ILI9328:   // ILI9328    320x240
        DEBUG_ECHO_MSG(" ILI9328");
        write_esc_sequence(ili9328_init);
        break;
      case ILI9341:   // ILI9341    320x240
        DEBUG_ECHO_MSG(" ILI9341");
        write_esc_sequence(ili9341_init);
        break;
      case ILI9488:   // ILI9488    480x320
      case ILI9488_ID1: // 0x8066 ILI9488    480x320
        DEBUG_ECHO_MSG(" ILI9488");
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

void TFT_IO::set_window(uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) {
  #ifdef OFFSET_X
    Xmin += OFFSET_X; Xmax += OFFSET_X;
  #endif
  #ifdef OFFSET_Y
    Ymin += OFFSET_Y; Ymax += OFFSET_Y;
  #endif

  switch (lcd_id) {
    case LTDC_RGB:
      io.WriteReg(0x01);
      io.WriteData(Xmin);
      io.WriteReg(0x02);
      io.WriteData(Xmax);
      io.WriteReg(0x03);
      io.WriteData(Ymin);
      io.WriteReg(0x04);
      io.WriteData(Ymax);
      io.WriteReg(0x00);
      break;
    case ST7735:    // ST7735     160x128
    case ST7789:    // ST7789V    320x240
    case ST7796:    // ST7796     480x320
    case ILI9341:   // ILI9341    320x240
    case ILI9488:   // ILI9488    480x320
    case SSD1963:   // SSD1963
    case ILI9488_ID1: // 0x8066 ILI9488    480x320
      io.DataTransferBegin(DATASIZE_8BIT);

      // CASET: Column Address Set
      io.WriteReg(ILI9341_CASET);
      io.WriteData((Xmin >> 8) & 0xFF);
      io.WriteData(Xmin & 0xFF);
      io.WriteData((Xmax >> 8) & 0xFF);
      io.WriteData(Xmax & 0xFF);

      // RASET: Row Address Set
      io.WriteReg(ILI9341_PASET);
      io.WriteData((Ymin >> 8) & 0xFF);
      io.WriteData(Ymin & 0xFF);
      io.WriteData((Ymax >> 8) & 0xFF);
      io.WriteData(Ymax & 0xFF);

      // RAMWR: Memory Write
      io.WriteReg(ILI9341_RAMWR);
      break;
    case R61505:    // R61505U    320x240
    case ILI9328:   // ILI9328    320x240
      io.DataTransferBegin(DATASIZE_16BIT);

      // Mind the mess: with landscape screen orientation 'Horizontal' is Y and 'Vertical' is X
      io.WriteReg(ILI9328_HASTART);
      io.WriteData(Ymin);
      io.WriteReg(ILI9328_HAEND);
      io.WriteData(Ymax);
      io.WriteReg(ILI9328_VASTART);
      io.WriteData(Xmin);
      io.WriteReg(ILI9328_VAEND);
      io.WriteData(Xmax);

      io.WriteReg(ILI9328_HASET);
      io.WriteData(Ymin);
      io.WriteReg(ILI9328_VASET);
      io.WriteData(Xmin);

      io.WriteReg(ILI9328_RAMWR);
      break;
    default:
      break;
  }

  io.DataTransferEnd();
}

void TFT_IO::write_esc_sequence(const uint16_t *Sequence) {
  uint16_t dataWidth, data;

  dataWidth = *Sequence++;
  io.DataTransferBegin(dataWidth);

  for (;;) {
    data = *Sequence++;
    if (data != 0xFFFF) {
      io.WriteData(data);
      continue;
    }
    data = *Sequence++;
    if (data == 0x7FFF) break;
    if (data == 0xFFFF)
      io.WriteData(0xFFFF);
    else if (data & 0x8000)
      delay(data & 0x7FFF);
    else if ((data & 0xFF00) == 0)
      io.WriteReg(data);
  }

  io.DataTransferEnd();
}

#endif // HAS_SPI_TFT || HAS_FSMC_TFT || HAS_LTDC_TFT
