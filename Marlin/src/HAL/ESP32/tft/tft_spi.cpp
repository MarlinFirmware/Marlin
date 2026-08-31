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

#include "../../platforms.h"

#ifdef ARDUINO_ARCH_ESP32

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_TFT

#include "tft_spi.h"

//#define DEBUG_TFT_IO
#define DEBUG_OUT ENABLED(DEBUG_TFT_IO)
#include "../../../core/debug_out.h"

SPIClass TFT_SPI::SPIx(TFT_SPI_DEVICE);
uint8_t TFT_SPI::dataSize = DATASIZE_16BIT;
bool TFT_SPI::inTransfer = false;

void TFT_SPI::init() {
  #if PIN_EXISTS(TFT_RESET)
    OUT_WRITE(TFT_RESET_PIN, HIGH);
    delay(100);
  #endif

  OUT_WRITE(TFT_A0_PIN, HIGH);
  OUT_WRITE(TFT_CS_PIN, HIGH);

  /**
   * The touch controller shares this bus and is the only device that replies,
   * so MISO comes from it when the panel has no SDO of its own. begin() is a
   * no-op once the bus is up, so this must be right the first time.
   */
  #if !PIN_EXISTS(TFT_MISO) && (HAS_TFT_XPT2046 || HAS_RES_TOUCH_BUTTONS)
    constexpr int8_t misoPin = TOUCH_MISO_PIN;
  #else
    constexpr int8_t misoPin = TFT_MISO_PIN;
  #endif

  SPIx.begin(TFT_SCK_PIN, misoPin, TFT_MOSI_PIN, TFT_CS_PIN);
  SPIx.setHwCs(false);          // CS is driven by hand around each transfer
}

void TFT_SPI::dataTransferBegin(uint16_t dataWidth) {
  dataSize = uint8_t(dataWidth);
  if (inTransfer) return;         // Never nest: beginTransaction() would deadlock
  SPIx.beginTransaction(SPISettings(SPI_CLOCK_MAX_TFT, MSBFIRST, TFT_SPI_MODE));
  WRITE(TFT_CS_PIN, LOW);
  inTransfer = true;
}

void TFT_SPI::dataTransferEnd() {
  if (!inTransfer) return;
  WRITE(TFT_CS_PIN, HIGH);
  SPIx.endTransaction();
  inTransfer = false;
}

void TFT_SPI::transmit(uint16_t data) {
  if (dataSize == DATASIZE_8BIT)
    SPIx.transfer(uint8_t(data));
  else
    SPIx.write16(data);
}

void TFT_SPI::transmit(uint16_t *data, uint16_t count) {
  // writePixels sends 16-bit values MSB first, which is what the panel expects
  SPIx.writePixels(data, uint32_t(count) * 2);
}

void TFT_SPI::writeMultiple(uint16_t color, uint32_t count) {
  dataTransferBegin();
  uint8_t pattern[2] = { uint8_t(color >> 8), uint8_t(color) };
  while (count) {
    const uint32_t chunk = _MIN(count, uint32_t(DMA_MAX_WORDS));
    SPIx.writePattern(pattern, 2, chunk);
    count -= chunk;
  }
  dataTransferEnd();
}

#include "../../../lcd/tft_io/tft_ids.h"

uint32_t TFT_SPI::getID() {
  DEBUG_ECHOLNPGM("TFT_SPI::getID()");

  uint32_t id = readID(LCD_READ_ID);
  #if ENABLED(DEBUG_TFT_IO)
    char debug_register[3], debug_value[5];
    sprintf_P(debug_register, PSTR("%02X"), LCD_READ_ID);
    sprintf_P(debug_value, PSTR("%04X"), uint16_t(id));
    DEBUG_ECHOLNPGM("  readID(0x", debug_register, ") : 0x", debug_value);
  #endif

  if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF) {
    id = readID(LCD_READ_ID4);
    #if ENABLED(DEBUG_TFT_IO)
      sprintf_P(debug_register, PSTR("%02X"), LCD_READ_ID4);
      sprintf_P(debug_value, PSTR("%04X"), uint16_t(id));
      DEBUG_ECHOLNPGM("  readID(0x", debug_register, ") : 0x", debug_value);
    #endif
  }

  #ifdef TFT_DEFAULT_DRIVER
    if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF) {
      id = TFT_DEFAULT_DRIVER;
      #if ENABLED(DEBUG_TFT_IO)
        sprintf_P(debug_value, PSTR("%04X"), uint16_t(id));
        DEBUG_ECHOLNPGM("  Fallback to TFT_DEFAULT_DRIVER : 0x", debug_value);
      #endif
    }
  #endif

  return id;
}

uint32_t TFT_SPI::readID(const uint16_t inReg) {
  uint32_t data = 0;

  #if PIN_EXISTS(TFT_MISO)
    // The ID read must be slow. Many panels also return the value shifted by
    // one bit, which is corrected on the way out.
    SPIx.beginTransaction(SPISettings(SPI_CLOCK_MAX_TFT / 16, MSBFIRST, TFT_SPI_MODE));
    WRITE(TFT_CS_PIN, LOW);

    WRITE(TFT_A0_PIN, LOW);
    SPIx.transfer(uint8_t(inReg));
    WRITE(TFT_A0_PIN, HIGH);

    for (uint8_t i = 0; i < 4; ++i)
      data = (data << 8) | SPIx.transfer(0x00);

    WRITE(TFT_CS_PIN, HIGH);
    SPIx.endTransaction();
  #endif

  return data >> 7;
}

#endif // HAS_SPI_TFT
#endif // ARDUINO_ARCH_ESP32
