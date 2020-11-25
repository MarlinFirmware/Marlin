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
#pragma once

#include "../../inc/MarlinConfig.h"

#if HAS_SPI_TFT || HAS_FSMC_TFT

#if HAS_SPI_TFT
  #include HAL_PATH(../../HAL, tft/tft_spi.h)
#elif HAS_FSMC_TFT
  #include HAL_PATH(../../HAL, tft/tft_fsmc.h)
#else
  #error "TFT IO only supports SPI or FSMC interface"
#endif

#define TFT_EXCHANGE_XY (1UL << 1)
#define TFT_INVERT_X    (1UL << 2)
#define TFT_INVERT_Y    (1UL << 3)

#define TFT_NO_ROTATION           (0x00)
#define TFT_ROTATE_90             (TFT_EXCHANGE_XY | TFT_INVERT_X)
#define TFT_ROTATE_180            (TFT_INVERT_X    | TFT_INVERT_Y)
#define TFT_ROTATE_270            (TFT_EXCHANGE_XY | TFT_INVERT_Y)

#define TFT_MIRROR_X              (TFT_INVERT_Y)
#define TFT_MIRROR_Y              (TFT_INVERT_X)

#define TFT_ROTATE_90_MIRROR_X    (TFT_ROTATE_90 ^ TFT_INVERT_Y)
#define TFT_ROTATE_90_MIRROR_Y    (TFT_ROTATE_90 ^ TFT_INVERT_X)

#define TFT_ROTATE_180_MIRROR_X   (TFT_ROTATE_180 ^ TFT_INVERT_Y)
#define TFT_ROTATE_180_MIRROR_Y   (TFT_ROTATE_180 ^ TFT_INVERT_X)

#define TFT_ROTATE_270_MIRROR_X   (TFT_ROTATE_270 ^ TFT_INVERT_Y)
#define TFT_ROTATE_270_MIRROR_Y   (TFT_ROTATE_270 ^ TFT_INVERT_X)

// TFT_ROTATION is user configurable
#ifndef TFT_ROTATION
  #define TFT_ROTATION TFT_NO_ROTATION
#endif


// TFT_ORIENTATION is the "sum" of TFT_DEFAULT_ORIENTATION plus user TFT_ROTATION
#define TFT_ORIENTATION ((TFT_DEFAULT_ORIENTATION) ^ (TFT_ROTATION))

#define TFT_COLOR_RGB   (1UL << 3)
#define TFT_COLOR_BGR   (1UL << 4)

// Each TFT Driver is responsible for its default color mode.
// #ifndef TFT_COLOR
//   #define TFT_COLOR   TFT_COLOR_RGB
// #endif

#define TOUCH_ORIENTATION_NONE  0
#define TOUCH_LANDSCAPE         1
#define TOUCH_PORTRAIT          2

#ifndef TOUCH_ORIENTATION
  #define TOUCH_ORIENTATION    TOUCH_LANDSCAPE
#endif

#define SSD1963         0x5761
#define ST7735          0x89F0
#define ST7789          0x8552
#define ST7796          0x7796
#define R61505          0x1505
#define ILI9328         0x9328
#define ILI9341         0x9341
#define ILI9488         0x9488
#define ILI9488_ID1     0x8066 //Some ILI9488 have 0x8066 in the 0x04
#define LERDGE_ST7796   0xFFFE
#define AUTO            0xFFFF

#ifndef TFT_DRIVER
  #define TFT_DRIVER    AUTO
#endif

#define ESC_REG(x)        0xFFFF, 0x00FF & (uint16_t)x
#define ESC_DELAY(x)      0xFFFF, 0x8000 | (x & 0x7FFF)
#define ESC_END           0xFFFF, 0x7FFF
#define ESC_FFFF          0xFFFF, 0xFFFF

class TFT_IO {
public:
  static TFT_IO_DRIVER io;

  static void InitTFT();
  static void set_window(uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax);
  static void write_esc_sequence(const uint16_t *Sequence);

  // Deletaged methods
  inline static void Init() { io.Init(); };
  inline static bool isBusy() { return io.isBusy(); };
  inline static void Abort() { io.Abort(); };
  inline static uint32_t GetID() { return io.GetID(); };

  inline static void DataTransferBegin(uint16_t DataWidth = DATASIZE_16BIT) { io.DataTransferBegin(DataWidth); }
  inline static void DataTransferEnd() { io.DataTransferEnd(); };
  // inline static void DataTransferAbort() { io.DataTransferAbort(); };

  inline static void WriteData(uint16_t Data) { io.WriteData(Data); };
  inline static void WriteReg(uint16_t Reg) { io.WriteReg(Reg); };

  inline static void WriteSequence(uint16_t *Data, uint16_t Count) { io.WriteSequence(Data, Count); };
  // static void WriteMultiple(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_MINC_DISABLE, &Data, Count); }
  inline static void WriteMultiple(uint16_t Color, uint32_t Count) { io.WriteMultiple(Color, Count); };

protected:
  static uint32_t lcd_id;
};

#endif // HAS_SPI_TFT || HAS_FSMC_TFT
