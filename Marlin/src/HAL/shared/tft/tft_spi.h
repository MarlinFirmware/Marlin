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

#ifndef LCD_READ_ID
  #define LCD_READ_ID 0x04   // Read display identification information (0xD3 on ILI9341)
#endif
#ifndef LCD_READ_ID4
  #define LCD_READ_ID4 0xD3   // Read display identification information (0xD3 on ILI9341)
#endif

#define TFT_IO_DRIVER  TFT_SPI

#define TFT_SUPPORTS_8BIT

class TFT_SPI {

  enum class eSPIMode
  {
    READ,
    WRITE
  };

private:

  static uint32_t ReadID(uint16_t Reg);
  static void Transmit(uint16_t Data);
  static void Transmit8(uint8_t Data);
  static void TransmitDMA(const uint16_t *Data, uint16_t Count);
  #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
    static void _TXComplete(void *ud);
    static void TransmitDMA_Async(const uint16_t *Data, uint16_t Count, void (*completeCallback)(void*) = nullptr, void *ud = nullptr);
  #endif
  static void TransmitRepeat(uint16_t val, uint16_t repcnt);

public:
  static void Init();
  static uint32_t GetID();
  static bool isBusy();
  static void Abort();

  static void DataTransferBegin(eSPIMode spiMode = eSPIMode::WRITE);
  static void DataTransferEnd();

  // Call DataTransferBegin for these two methods.
  static void WriteData(uint16_t Data) { Transmit(Data); }
  static void WriteData8(uint8_t Data) { Transmit8(Data); }
  static void WriteReg(uint16_t Reg) { OUT_WRITE(TFT_A0_PIN, LOW); Transmit(Reg); OUT_WRITE(TFT_A0_PIN, HIGH); }
  static void WriteReg8(uint8_t Reg) { OUT_WRITE(TFT_A0_PIN, LOW); Transmit8(Reg); OUT_WRITE(TFT_A0_PIN, HIGH); }

  // Do not call DataTransferBegin for the following methods.
  static void WriteSequence(const uint16_t *Data, uint16_t Count) { TransmitDMA(Data, Count); }

  #if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
    static void WriteSequenceAsync(const uint16_t *Data, uint16_t Count, void (*completeCallback)(void*) = nullptr, void *ud = nullptr) { TransmitDMA_Async(Data, Count, completeCallback, ud); }
  #endif

  static void WriteMultiple(uint16_t Color, uint16_t Count) { TransmitRepeat(Color, Count); }
  static void WriteMultiple(uint16_t Color, uint32_t Count) {
    while (Count > 0) {
      TransmitRepeat(Color, Count > 0xFFFF ? 0xFFFF : Count);
      Count = Count > 0xFFFF ? Count - 0xFFFF : 0;
    }
  }
};
