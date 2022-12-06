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

/**
 * HAL SPI for Teensy 3.5 (MK64FX512) and Teensy 3.6 (MK66FX1M0)
 */

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "../../inc/MarlinConfig.h"
#include "HAL.h"

#include <SPI.h>
#include <pins_arduino.h>
#include "spi_pins.h"

static SPISettings spiConfig;
static uint32_t _spi_clock;
static int _spi_bitOrder;
static int _spi_clockMode;

void spiBegin() {
  #if PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH);
  #endif
  SET_OUTPUT(SD_SCK_PIN);
  SET_INPUT(SD_MISO_PIN);
  SET_OUTPUT(SD_MOSI_PIN);
}

void spiInitEx(uint32_t clock, const int hint_sck/*=-1*/, const int hint_miso/*=-1*/, const int hint_mosi/*=-1*/, const int hint_cs/*=-1*/) {
  // Ignore the SPI pin hints.
  _spi_clock = clock;
  _spi_bitOrder = MSBFIRST;
  _spi_clockMode = SPI_MODE0;
  spiConfig = SPISettings(clock, MSBFIRST, SPI_MODE0);
  SPI.begin();
}

void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
  // Use Marlin data-rates
  uint32_t clock;
  switch (spiRate) {
  case SPI_FULL_SPEED:    clock = 10000000; break;
  case SPI_HALF_SPEED:    clock =  5000000; break;
  case SPI_QUARTER_SPEED: clock =  2500000; break;
  case SPI_EIGHTH_SPEED:  clock =  1250000; break;
  case SPI_SPEED_5:       clock =   625000; break;
  case SPI_SPEED_6:       clock =   312500; break;
  default:
    clock = 4000000; // Default from the SPI library
  }
  spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
}

void spiClose() { SPI.end(); }

void spiSetBitOrder(int bitOrder) {
  if (bitOrder == SPI_BITORDER_MSB) {
    _spi_bitOrder = MSBFIRST;
  }
  else if (bitOrder == SPI_BITORDER_LSB) {
    _spi_bitOrder = LSBFIRST;
  }
  else return;

  spiConfig = SPISettings(_spi_clock, _spi_bitOrder, _spi_clockMode);
}

void spiSetClockMode(int clockMode) {
  if (clockMode == SPI_CLKMODE_0)
    _spi_clockMode = SPI_MODE0;
  else if (clockMode == SPI_CLKMODE_1)
    _spi_clockMode = SPI_MODE1;
  else if (clockMode == SPI_CLKMODE_2)
    _spi_clockMode = SPI_MODE2;
  else if (clockMode == SPI_CLKMODE_3)
    _spi_clockMode = SPI_MODE3;
  else return;

  spiConfig = SPISettings(_spi_clock, _spi_bitOrder, _spi_clockMode);
}

void spiEstablish() { /* do nothing */ }

uint8_t spiRec(uint8_t txval) {
  SPI.beginTransaction(spiConfig);
  uint8_t returnByte = SPI.transfer(txval);
  SPI.endTransaction();
  return returnByte;
  //SPDR = 0xFF;
  //while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  //return SPDR;
}

uint16_t spiRec16(uint16_t txval) {
  SPI.beginTransaction(spiConfig);
  uint16_t res = SPI.transfer16(txval);
  SPI.endTransaction();
  return res;
}

void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
  memset(buf, txval, nbyte);
  SPI.beginTransaction(spiConfig);
  SPI.transfer(buf, nbyte);
  SPI.endTransaction();
  //if (nbyte-- == 0) return;
  //  SPDR = 0xFF;
  //for (uint16_t i = 0; i < nbyte; i++) {
  //  while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  //  buf[i] = SPDR;
  //  SPDR = 0xFF;
  //}
  //while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  //buf[nbyte] = SPDR;
}

void spiSend(uint8_t b) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer(b);
  SPI.endTransaction();
  //SPDR = b;
  //while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
}

void spiSend16(uint16_t v) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer16(v);
  SPI.endTransaction();
}

void spiSendBlock(uint8_t token, const uint8_t *buf) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer(token);
  for (uint16_t n = 0; n < 512; n++)
    SPI.transfer(buf[n]);
#if 0
  SPDR = token;
  for (uint16_t i = 0; i < 512; i += 2) {
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i];
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i + 1];
  }
  while (!TEST(SPSR, SPIF)) { /* nada */ };
#endif
  SPI.endTransaction();
}

void spiWrite(const uint8_t *buf, uint16_t cnt) {
  SPI.beginTransaction(spiConfig);
  for (uint16_t n = 0; n < cnt; n++)
    SPI.transfer(buf[n]);
  SPI.endTransaction();
}

void spiWrite16(const uint16_t *buf, uint16_t cnt) {
  SPI.beginTransaction(spiConfig);
  for (uint16_t n = 0; n < cnt; n++)
    SPI.transfer16(buf[n]);
  SPI.endTransaction();
}

void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
  SPI.beginTransaction(spiConfig);
  for (uint16_t n = 0; n < repcnt; n++)
    SPI.transfer(val);
  SPI.endTransaction();
}

void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
  SPI.beginTransaction(spiConfig);
  for (uint16_t n = 0; n < repcnt; n++)
    SPI.transfer16(val);
  SPI.endTransaction();
}

#endif // __MK64FX512__ || __MK66FX1M0__
