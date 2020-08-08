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

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_TFT

#include "tft_spi.h"

// TFT_SPI tft;

SPIClass TFT_SPI::SPIx(1);

#define SPI_TFT_CS_H  OUT_WRITE(TFT_CS_PIN, HIGH)
#define SPI_TFT_CS_L  OUT_WRITE(TFT_CS_PIN, LOW)

#define SPI_TFT_DC_H  OUT_WRITE(TFT_DC_PIN, HIGH)
#define SPI_TFT_DC_L  OUT_WRITE(TFT_DC_PIN, LOW)

#define SPI_TFT_RST_H OUT_WRITE(TFT_RST_PIN, HIGH)
#define SPI_TFT_RST_L OUT_WRITE(TFT_RST_PIN, LOW)

#define SPI_TFT_BLK_H OUT_WRITE(TFT_BACKLIGHT_PIN, HIGH)
#define SPI_TFT_BLK_L OUT_WRITE(TFT_BACKLIGHT_PIN, LOW)

void TFT_SPI::Init() {
  #if PIN_EXISTS(TFT_RESET)
    // OUT_WRITE(TFT_RESET_PIN, HIGH);
    SPI_TFT_RST_H;
    delay(100);
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT)
    // OUT_WRITE(TFT_BACKLIGHT_PIN, HIGH);
    SPI_TFT_BLK_H;
  #endif

  SPI_TFT_DC_H;
  SPI_TFT_CS_H;

  /**
   * STM32F1 APB2 = 72MHz, APB1 = 36MHz, max SPI speed of this MCU if 18Mhz
   * STM32F1 has 3 SPI ports, SPI1 in APB2, SPI2/SPI3 in APB1
   * so the minimum prescale of SPI1 is DIV4, SPI2/SPI3 is DIV2
   */
  #if SPI_DEVICE == 1
    #define SPI_CLOCK_MAX SPI_CLOCK_DIV4
  #else
    #define SPI_CLOCK_MAX SPI_CLOCK_DIV2
  #endif
  uint8_t  clock;
  uint8_t spiRate = SPI_FULL_SPEED;
  switch (spiRate) {
    case SPI_FULL_SPEED:    clock = SPI_CLOCK_MAX ;  break;
    case SPI_HALF_SPEED:    clock = SPI_CLOCK_DIV4 ; break;
    case SPI_QUARTER_SPEED: clock = SPI_CLOCK_DIV8 ; break;
    case SPI_EIGHTH_SPEED:  clock = SPI_CLOCK_DIV16; break;
    case SPI_SPEED_5:       clock = SPI_CLOCK_DIV32; break;
    case SPI_SPEED_6:       clock = SPI_CLOCK_DIV64; break;
    default:                clock = SPI_CLOCK_DIV2;  // Default from the SPI library
  }
  SPIx.setModule(1);
  SPIx.setClockDivider(clock);
  SPIx.setBitOrder(MSBFIRST);
  SPIx.setDataMode(SPI_MODE0);
}

void TFT_SPI::DataTransferBegin(uint16_t DataSize) {
  SPIx.setDataSize(DataSize);
  SPIx.begin();
  SPI_TFT_CS_L;
}

uint32_t TFT_SPI::GetID() {
  uint32_t id;
  id = ReadID(LCD_READ_ID);

  if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
    id = ReadID(LCD_READ_ID4);
  return id;
}

uint32_t TFT_SPI::ReadID(uint16_t Reg) {
  #if !PIN_EXISTS(TFT_MISO)
    return 0;
  #else
    uint16_t d = 0;
    DataTransferBegin(DATASIZE_8BIT);
    WriteReg(Reg);

    SPI.read((uint8_t*)&d, 1); //dummy read
    SPI.read((uint8_t*)&d, 1);

    DataTransferEnd();

    return d >> 7;
  #endif
}

bool TFT_SPI::isBusy() {
  return false;
}

void TFT_SPI::Abort() {
  DataTransferEnd();
}

void TFT_SPI::Transmit(uint16_t Data) {
  SPIx.send(Data);
}

void TFT_SPI::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {
  DataTransferBegin();
  SPI_TFT_DC_H;
  if (MemoryIncrease == DMA_MINC_ENABLE) {
    SPIx.dmaSend(Data, Count, true);
  }
  else {
    SPIx.dmaSend(Data, Count, false);
  }

  DataTransferEnd();
}

#endif // HAS_SPI_TFT
