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
  #include "../../HAL/shared/tft/tft_spi.h"
#elif HAS_FSMC_TFT
  #include HAL_PATH(../../HAL, tft/tft_fsmc.h)
#elif HAS_LTDC_TFT
  #include HAL_PATH(../../HAL, tft/tft_ltdc.h)
#else
  #error "TFT IO only supports SPI, FSMC or LTDC interface."
#endif

#ifndef DMA_MAX_SIZE
  // uint16_t type maximum.
  #define DMA_MAX_SIZE 0xFFFF
#endif

#include "tft_orientation.h"

#ifndef TFT_DRIVER
  #define TFT_DRIVER    AUTO
#endif

#define ESC_REG(x)   0xFFFF, 0x00FF & (uint16_t)x
#define ESC_DELAY(x) 0xFFFF, 0x8000 | (x & 0x7FFF)
#define ESC_END      0xFFFF, 0x7FFF
#define ESC_FFFF     0xFFFF, 0xFFFF

#define DATASIZE_8BIT 0
#define DATASIZE_16BIT 2

class TFT_IO {
public:
  static TFT_IO_DRIVER io;

  static void InitTFT();
  static void set_window(uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax);
  static void write_esc_sequence(const uint16_t *Sequence);

  // Deletaged methods
  inline static void Init() { io.Init(); io.Abort(); };
  inline static bool isBusy() { return io.isBusy(); };
  inline static void Abort() { io.Abort(); };
  inline static uint32_t GetID() { return io.GetID(); };

  inline static void DataTransferBegin() { io.DataTransferBegin(); }
  inline static void DataTransferEnd() { io.DataTransferEnd(); };

  inline static void WriteData(uint16_t Data) { io.WriteData(Data); }
#if ENABLED(TFT_SUPPORTS_8BIT)
  inline static void WriteData8(uint8_t Data) { io.WriteData8(Data); }
#endif
  inline static void WriteReg(uint16_t Reg) { io.WriteReg(Reg); };
#if ENABLED(TFT_SUPPORTS_8BIT)
  inline static void WriteReg8(uint8_t Reg) { io.WriteReg8(Reg); }
#endif

  // Blocking IO used by TFT_CLASSIC_UI and TFT_LVGL_UI
  // These functions start data transfer and WAIT for data transfer completion
  inline static void WriteSequence(const uint16_t *Data, uint16_t Count) { io.WriteSequence(Data, Count); };

  // Non-blocking async IO
  // These functions start data transfer and DO NOT wait for data transfer completion
  #if ENABLED(TFT_SUPPORTS_ASYNC)
    inline static void WriteSequenceAsync(const uint16_t *Data, uint16_t Count, void (*completeCallback)(void*) = nullptr, void *ud = nullptr) { io.WriteSequenceAsync(Data, Count, completeCallback, ud); };
  #endif

  // static void WriteMultiple(uint16_t Color, uint16_t Count) { static uint16_t Data; Data = Color; TransmitDMA(DMA_MINC_DISABLE, &Data, Count); }
  inline static void WriteMultiple(uint16_t Color, uint32_t Count) { io.WriteMultiple(Color, Count); };

protected:
  static uint32_t lcd_id;
};
