/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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
#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfig.h"

#include "../shared/HAL_SPI.h"

#if !ENABLED(SOFTWARE_SPI) && ENABLED(HALSPI_HW_GENERIC)

// ------------------------
// Hardware SPI
// ------------------------

static void _spi_on_error() {
  for (;;) {
#if defined(HALSPI_DO_ERRORBEEPS) && PIN_EXISTS(BEEPER)
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(500);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(500);
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(500);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(500);
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(150);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(150);
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(150);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(150);
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(150);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(3000);
#endif
  }
}

static void __attribute__((unused)) _spi_infobeep(uint32_t code) {
#if PIN_EXISTS(BEEPER)
  OUT_WRITE(BEEPER_PIN, HIGH);
  delay(500);
  OUT_WRITE(BEEPER_PIN, LOW);
  delay(500);
  for (uint32_t n = 0; n < code; n++) {
    OUT_WRITE(BEEPER_PIN, HIGH);
    delay(200);
    OUT_WRITE(BEEPER_PIN, LOW);
    delay(200);
  }
  delay(300);
  OUT_WRITE(BEEPER_PIN, HIGH);
  delay(400);
  OUT_WRITE(BEEPER_PIN, LOW);
  delay(1000);
#endif
}

#include <pins_arduino.h>
#include <SPI.h>

static SPISettings spiConfig;

static uint32_t _spi_clock;
static int _spi_bitOrder;
static int _spi_clkmode;
static int _spi_pin_cs;
static bool _spi_initialized = false;
static bool _spi_transaction_is_running = false;

void spiBegin() {
  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH);
  #endif
}

void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
  if (hint_sck != -1) {
    SET_OUTPUT(hint_sck);
  }
  if (hint_miso != -1) {
    SET_INPUT(hint_miso);
  }
  if (hint_mosi != -1) {
    SET_OUTPUT(hint_mosi);
  }
  if (hint_cs != -1) {
    SET_OUTPUT(hint_cs);
  }

  if (_spi_initialized)
    _spi_on_error();

  _spi_clock = maxClockFreq;
  _spi_bitOrder = MSBFIRST;
  _spi_clkmode = SPI_MODE0;
  _spi_pin_cs = hint_cs;
  spiConfig = SPISettings(maxClockFreq, _spi_bitOrder, _spi_clkmode);
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/SPI/src/SPI.cpp SPIClass::begin method.
  SPI.begin(hint_sck, hint_miso, hint_mosi);
  _spi_initialized = true;
  _spi_transaction_is_running = false;
}

void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
  uint32_t clock;

  switch (spiRate) {
    case SPI_FULL_SPEED:      clock = 16000000; break;
    case SPI_HALF_SPEED:      clock = 8000000;  break;
    case SPI_QUARTER_SPEED:   clock = 4000000;  break;
    case SPI_EIGHTH_SPEED:    clock = 2000000;  break;
    case SPI_SIXTEENTH_SPEED: clock = 1000000;  break;
    case SPI_SPEED_5:         clock = 500000;   break;
    case SPI_SPEED_6:         clock = 250000;   break;
    default:                  clock = 1000000; // Default from the SPI library
  }
  spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
}

static void _maybe_start_transaction() {
  if (_spi_transaction_is_running) return;
  SPI.beginTransaction(spiConfig);
  if (_spi_pin_cs != -1)
    OUT_WRITE(_spi_pin_cs, LOW);
  _spi_transaction_is_running = true;
}

void spiClose() {
  if (!_spi_initialized)
    _spi_on_error();
  if (_spi_transaction_is_running) {
    if (_spi_pin_cs != -1)
      OUT_WRITE(_spi_pin_cs, HIGH);
    SPI.endTransaction();
    _spi_transaction_is_running = false;
  }
  SPI.end();
  _spi_initialized = false;
}

void spiSetBitOrder(int bitOrder) {
  bool update = false;
  if (bitOrder == SPI_BITORDER_MSB) {
    _spi_bitOrder = MSBFIRST;
    update = true;
  }
  else if (bitOrder == SPI_BITORDER_LSB) {
    _spi_bitOrder = LSBFIRST;
    update = true;
  }
  if (update) {
    spiConfig = SPISettings(_spi_clock, _spi_bitOrder, _spi_clkmode);
    if (_spi_transaction_is_running) {
      // On ESP32 it is safe to keep the chip-select LOW even if restarting
      // a transaction, judging by the SPI library and HAL sources.
      SPI.endTransaction();
      SPI.beginTransaction(spiConfig);
    }
  }
}

void spiSetClockMode(int mode) {
  bool update = false;
  if (mode == SPI_CLKMODE_0) {
    _spi_clkmode = SPI_MODE0;
    update = true;
  }
  else if (mode == SPI_CLKMODE_1) {
    _spi_clkmode = SPI_MODE1;
    update = true;
  }
  else if (mode == SPI_CLKMODE_2) {
    _spi_clkmode = SPI_MODE2;
    update = true;
  }
  else if (mode == SPI_CLKMODE_3) {
    _spi_clkmode = SPI_MODE3;
    update = true;
  }
  if (update) {
    spiConfig = SPISettings(_spi_clock, _spi_bitOrder, _spi_clkmode);
    if (_spi_transaction_is_running) {
      SPI.endTransaction();
      SPI.beginTransaction(spiConfig);
    }
  }
}

void spiEstablish() {
  _maybe_start_transaction();
}

uint8_t spiRec(uint8_t txval) {
  _maybe_start_transaction();
  uint8_t returnByte = SPI.transfer(txval);
  return returnByte;
}

uint16_t spiRec16(uint16_t txval) {
  _maybe_start_transaction();
  uint16_t res = SPI.transfer16(txval);
  return res;
}

void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
  if (nbyte == 0) return;
  _maybe_start_transaction();
  for (uint16_t n = 0; n < nbyte; n++)
    buf[n] = SPI.transfer(txval);
}

void spiSend(uint8_t b) {
  _maybe_start_transaction();
  SPI.write(b);
}

void spiSend16(uint16_t v) {
  _maybe_start_transaction();
  SPI.write16(v);
}

void spiSendBlock(uint8_t token, const uint8_t *buf) {
  _maybe_start_transaction();
  SPI.write(token);
  SPI.writeBytes(buf, 512);
}

void spiWrite(const uint8_t *buf, uint16_t cnt) {
  _maybe_start_transaction();
  SPI.writeBytes(buf, cnt);
}

void spiWrite16(const uint16_t *buf, uint16_t cnt) {
  if (cnt == 0) return;
  _maybe_start_transaction();
  for (uint16_t n = 0; n < cnt; n++)
    SPI.write16(buf[n]);
}

void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
  if (repcnt == 0) return;
  _maybe_start_transaction();
  for (uint16_t n = 0; n < repcnt; n++)
    SPI.write(val);
}

void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
  if (repcnt == 0) return;
  _maybe_start_transaction();
  for (uint16_t n = 0; n < repcnt; n++) {
    SPI.write16(val);
  }
}

#endif

#endif