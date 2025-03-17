/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef TARGET_LPC5528

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_TFT

#include "tft_spi.h"

//TFT_SPI tft;

SPIClass TFT_SPI::SPIx(1);

#define TFT_CS_H  WRITE(TFT_CS_PIN, HIGH)
#define TFT_CS_L  WRITE(TFT_CS_PIN, LOW)

#define TFT_DC_H  WRITE(TFT_DC_PIN, HIGH)
#define TFT_DC_L  WRITE(TFT_DC_PIN, LOW)

#define TFT_RST_H WRITE(TFT_RESET_PIN, HIGH)
#define TFT_RST_L WRITE(TFT_RESET_PIN, LOW)

#define TFT_BLK_H WRITE(TFT_BACKLIGHT_PIN, HIGH)
#define TFT_BLK_L WRITE(TFT_BACKLIGHT_PIN, LOW)

void TFT_SPI::init() {
  class SPISettings spiConfig;

  SET_OUTPUT(TFT_BACKLIGHT_PIN);
  SET_OUTPUT(TFT_DC_PIN);
  SET_OUTPUT(TFT_CS_PIN);
  SET_OUTPUT(TFT_RESET_PIN);

  #if PIN_EXISTS(TFT_RESET)
    SET_OUTPUT(TFT_RESET_PIN);
    TFT_RST_H;
    delay(100);
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT)
    SET_OUTPUT(TFT_BACKLIGHT_PIN);
    TFT_BLK_H;
  #endif

  SET_OUTPUT(TFT_DC_PIN);
  SET_OUTPUT(TFT_CS_PIN);

  TFT_DC_H;
  TFT_CS_H;

  spiConfig = SPISettings(10000000, kSPI_MsbFirst, SPI_MODE0, false);
  HS_SPI.beginTransaction(spiConfig);

  /**
   * STM32F1 APB2 = 72MHz, APB1 = 36MHz, max SPI speed of this MCU if 18Mhz
   * STM32F1 has 3 SPI ports, SPI1 in APB2, SPI2/SPI3 in APB1
   * so the minimum prescale of SPI1 is DIV4, SPI2/SPI3 is DIV2
   */
  #if 0
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
  #endif

  //#if TFT_MISO_PIN == BOARD_SPI1_MISO_PIN
  //  SPIx.setModule(1);
  //#elif TFT_MISO_PIN == BOARD_SPI2_MISO_PIN
  //  SPIx.setModule(2);
  //#endif
  //SPIx.setClock(SPI_CLOCK_MAX_TFT);
  //SPIx.setBitOrder(kSPI_MsbFirst);
  //SPIx.setDataMode(SPI_MODE0);
}

void TFT_SPI::dataTransferBegin(const uint16_t dataWidth/*=DATASIZE_16BIT*/) {
  class SPISettings spiConfig;
  spiConfig = SPISettings(10000000, kSPI_MsbFirst, SPI_MODE0, kSPI_Data8Bits, false);
  HS_SPI.beginTransaction(spiConfig);
  TFT_CS_L;
}

uint32_t TFT_SPI::getID() {
  //uint32_t id;
  //id = readID(LCD_READ_ID);
  //if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
  //  id = readID(LCD_READ_ID4);
  //return id;
}

uint32_t TFT_SPI::readID(const uint16_t inReg) {
  //#if PIN_EXISTS(TFT_MISO)
  //  uint32_t data = 0;
  //  uint8_t d = 0;
  //  SPIx.setDataSize(kSPI_Data8Bits);
  //  SPIx.setClock(SPI_CLOCK_DIV64);
  //  SPIx.begin();
  //  TFT_CS_L;
  //  WriteReg(inReg);

  //  LOOP_L_N(i, 4) {
  //    SPIx.read((uint8_t*)&d, 1);
  //    data = (data << 8) | d;
  //  }

  //  DataTransferEnd();
  //  SPIx.setClock(SPI_CLOCK_MAX_TFT);
  //  return data >> 7;
  //#endif

  return true;
}

bool TFT_SPI::isBusy() {
  return false;
}

void TFT_SPI::abort() {
  dataTransferEnd();
}

//void trans16(uint16_t data) {
//  HS_SPI.transfer(data >> 8);
//  HS_SPI.transfer(data);
//}

void TFT_SPI::transmit(uint16_t Data) {
  //class SPISettings spiConfig;
  //spiConfig = SPISettings(10000000, kSPI_MsbFirst, SPI_MODE0, kSPI_Data8Bits, false);  // true
  //HS_SPI.beginTransaction(spiConfig);

  HS_SPI.transfer(Data);
  //trans16(Data);
}

void TFT_SPI::transmitDMA(uint32_t memoryIncrease, uint16_t *data, uint16_t count) {
  if (memoryIncrease) {
    class SPISettings spiConfigSPI8;
    spiConfigSPI8 = SPISettings(50000000, kSPI_MsbFirst, SPI_MODE0, kSPI_Data16Bits, true);  // true
    HS_SPI.beginTransaction(spiConfigSPI8);
    WRITE(TFT_DC_PIN, HIGH);
    WRITE(TFT_CS_PIN, LOW);

    constexpr uint16_t MAX_NotDMA = (0x0400 - 1);
    while (count > 0) {
      HS_SPI.dmaSend(data, (count > MAX_NotDMA ? MAX_NotDMA : count), memoryIncrease);
      data += MAX_NotDMA;
      count = count > MAX_NotDMA ? count - MAX_NotDMA : 0;
    }

    // No DMA, a little speed
    //constexpr uint16_t MAX_NotDMA = (0x0400 - 1);
    //while (count > 0) {
    //  HS_SPI.transfer16Notdma(data, count > MAX_NotDMA ? MAX_NotDMA : count);
    //  data += MAX_NotDMA;
    //  count = count > MAX_NotDMA ? count - MAX_NotDMA : 0;
    //}

    // No DMA, low speed
    //for (int i = 0; i < count; i++)
    //  HS_SPI.transfer16Notdma(data[i]);
  }
  else {
    class SPISettings spiConfigSPI8;
    spiConfigSPI8 = SPISettings(50000000, kSPI_MsbFirst, SPI_MODE0, kSPI_Data16Bits, true);  // true
    HS_SPI.beginTransaction(spiConfigSPI8);
    WRITE(TFT_DC_PIN, HIGH);
    WRITE(TFT_CS_PIN, LOW);
    HS_SPI.dmaSend(Data, count, memoryIncrease);
  }

  dataTransferEnd();
}

#endif // HAS_SPI_TFT
#endif // TARGET_LPC5528
