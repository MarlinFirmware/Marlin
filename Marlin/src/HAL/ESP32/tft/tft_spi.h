/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#include <SPI.h>

#ifndef LCD_READ_ID
  #define LCD_READ_ID  0x04   // Read display identification information (0xD3 on ILI9341)
#endif
#ifndef LCD_READ_ID4
  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
#endif

#ifndef TFT_MISO_PIN
  #define TFT_MISO_PIN -1   // Panels that never reply omit this
#endif

#ifndef TFT_SPI_DEVICE
  #define TFT_SPI_DEVICE VSPI // HSPI is taken by the SD card on most ESP32 boards
#endif

#ifndef TFT_SPI_MODE
  #define TFT_SPI_MODE SPI_MODE0  // Some panels clock on the opposite edge (SPI_MODE2)
#endif

#ifndef SPI_CLOCK_MAX_TFT
  #define SPI_CLOCK_MAX_TFT 40000000  // 40 MHz
#endif

/**
 * The ESP32 SPI peripheral is byte-oriented, so unlike the STM32 and LPC HALs
 * there is no hardware frame-size to switch. The requested width is recorded
 * and applied when each value is transmitted.
 */
#define DATASIZE_8BIT   8
#define DATASIZE_16BIT 16

#define TFT_IO_DRIVER  TFT_SPI
#define DMA_MAX_WORDS  0xFFFF

class TFT_SPI {
private:
  static uint8_t dataSize;
  static bool inTransfer;

  static uint32_t readID(const uint16_t inReg);
  static void transmit(uint16_t data);
  static void transmit(uint16_t *data, uint16_t count);

public:
  static SPIClass SPIx;   // Shared with the touch controller on the same bus

  static void init();
  static uint32_t getID();

  // Transfers are blocking, so there is never anything in flight to wait for.
  static bool isBusy() { return false; }
  static void abort() { dataTransferEnd(); }

  static void dataTransferBegin(uint16_t dataWidth=DATASIZE_16BIT);
  static void dataTransferEnd();
  static void dataTransferAbort() { dataTransferEnd(); }

  static void writeData(uint16_t data) { transmit(data); }
  static void writeReg(const uint16_t inReg) {
    WRITE(TFT_A0_PIN, LOW);
    SPIx.transfer(uint8_t(inReg));  // Commands are always a single byte
    WRITE(TFT_A0_PIN, HIGH);
  }

  /**
   * Each of these opens and closes its own transfer, as the other HALs do:
   * set_window() has already closed the one it used before pixels are written.
   *
   * No DMA engine is used here, so the "DMA" entry points are the blocking ones.
   */
  static void writeSequence(uint16_t *data, uint16_t count) { dataTransferBegin(); transmit(data, count); dataTransferEnd(); }
  static void writeMultiple(uint16_t color, uint32_t count);

  static void writeSequence_DMA(uint16_t *data, uint16_t count) { writeSequence(data, count); }
  static void writeMultiple_DMA(uint16_t color, uint16_t count) { writeMultiple(color, count); }
};
