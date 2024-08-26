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

#if HAS_SPI_TFT
  #include HAL_PATH(../.., tft/tft_spi.h)
#elif HAS_FSMC_TFT
  #include HAL_PATH(../.., tft/tft_fsmc.h)
#elif HAS_LTDC_TFT
  #include HAL_PATH(../.., tft/tft_ltdc.h)
#else
  #error "TFT IO only supports SPI, FSMC or LTDC interface."
#endif

#ifndef DMA_MAX_WORDS
  #error "DMA_MAX_WORDS is not configured for this platform."
#endif

// Each TFT Driver is responsible for its default color mode.
// #ifndef TFT_COLOR
//   #define TFT_COLOR   TFT_COLOR_RGB
// #endif

#ifndef TFT_DRIVER
  #define TFT_DRIVER AUTO
#endif

#define ESC_REG(x)   0xFFFF, 0x00FF & (uint16_t)x
#define ESC_DELAY(x) 0xFFFF, 0x8000 | (x & 0x7FFF)
#define ESC_END      0xFFFF, 0x7FFF
#define ESC_FFFF     0xFFFF, 0xFFFF

class TFT_IO {
public:
  static TFT_IO_DRIVER io;

  static void InitTFT();
  static void set_window(uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax);
  static void write_esc_sequence(const uint16_t *Sequence);

  // Deletaged methods
  inline static void Init() { io.Init(); }
  inline static bool isBusy() { return io.isBusy(); }
  inline static void Abort() { io.Abort(); }
  inline static uint32_t GetID() { return io.GetID(); }

  inline static void DataTransferBegin(uint16_t DataWidth = DATASIZE_16BIT) { io.DataTransferBegin(DataWidth); }
  inline static void DataTransferEnd() { io.DataTransferEnd(); }

  inline static void WriteData(uint16_t Data) { io.WriteData(Data); }
  inline static void WriteReg(uint16_t Reg) { io.WriteReg(Reg); }

  // Blocking IO used by TFT_CLASSIC_UI and TFT_LVGL_UI
  // These functions start data transfer and WAIT for data transfer completion
  inline static void WriteSequence(uint16_t *Data, uint16_t Count) { io.WriteSequence(Data, Count); }
  inline static void WriteMultiple(uint16_t Color, uint32_t Count) { io.WriteMultiple(Color, Count); }

  // Non-blocking DMA-based IO used by TFT_COLOR_UI only
  // These functions start data transfer using DMA and do NOT wait for data transfer completion
  inline static void WriteSequenceDMA(uint16_t *Data, uint16_t Count) { io.WriteSequence_DMA(Data, Count); }
  inline static void WriteMultipleDMA(uint16_t Color, uint16_t Count) { io.WriteMultiple_DMA(Color, Count); }

  // Non-blocking DMA-based IO with IRQ callback used by TFT_LVGL_UI only
  // This function starts data transfer using DMA and does NOT wait for data transfer completion
  #if ENABLED(USE_SPI_DMA_TC)
    inline static void WriteSequenceIT(uint16_t *Data, uint16_t Count) { io.WriteSequenceIT(Data, Count); }
  #endif

protected:
  static uint32_t lcd_id;
};
