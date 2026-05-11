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
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (c) 2009 by William Greiman
 */

#include <src/inc/MarlinConfig.h>
#include <SPI.h>

#include <pinmapping.h>
#include "../hardware/bus/spi.h"

static SpiBus &spi_bus = spi_bus_by_pins<SD_SCK_PIN, SD_MOSI_PIN, SD_MISO_PIN>();

uint8_t spiTransfer(uint8_t b) {
  return spi_bus.transfer(b);
}

void spiBegin() {
  OUT_WRITE(SD_SS_PIN, HIGH);
  SET_OUTPUT(SD_SCK_PIN);
  SET_INPUT(SD_MISO_PIN);
  SET_OUTPUT(SD_MOSI_PIN);
}

void spiInit(uint8_t spiRate) {
  // SPI_speed = swSpiInit(spiRate, SD_SCK_PIN, SD_MOSI_PIN);
  WRITE(SD_MOSI_PIN, HIGH);
  WRITE(SD_SCK_PIN, LOW);
}

uint8_t spiRec() { return spiTransfer(0xFF); }

void spiRead(uint8_t*buf, uint16_t nbyte) {
  spi_bus.transfer<uint8_t>(nullptr, buf, nbyte);
}

void spiSend(uint8_t b) { (void)spiTransfer(b); }

void spiSend(const uint8_t* buf, size_t nbyte) {
  spi_bus.transfer<uint8_t>((uint8_t*)buf, nullptr, nbyte);
}

void spiSendBlock(uint8_t token, const uint8_t* buf) {
  (void)spiTransfer(token);
  spi_bus.transfer<uint8_t>((uint8_t*)buf, nullptr, 512);
}

SPIClass::SPIClass(uint8_t spiPortNumber) {
  _settings[0].dataSize = DATA_SIZE_8BIT;
  _settings[0].clock = 0;
  _settings[0].dataMode = 0;
  _settings[0].bitOrder = 0;
  setModule(spiPortNumber);
}

void SPIClass::begin() {

}

void SPIClass::end() {

}

void SPIClass::beginTransaction(const SPISettings& s) {
  setClock(s.clock);
  setDataMode(s.dataMode);
  setDataSize(s.dataSize);
  setBitOrder(s.bitOrder);
}

void SPIClass::endTransaction() {

}

// Transfer using 1 "Data Size"
uint8_t SPIClass::transfer(uint16_t data) {
  return spiTransfer(data);
}
// Transfer 2 bytes in 8 bit mode
uint16_t SPIClass::transfer16(uint16_t data) {
  return (spiTransfer(data >> 8) << 8) | (spiTransfer(data & 0xFF) & 0xFF);
}

void SPIClass::send(uint8_t data) {
  spiSend(data);
}

uint16_t SPIClass::read() {
  return spiRec();
}

void SPIClass::read(uint8_t *buf, uint32_t len) {
  spiRead(buf, len);
}

void SPIClass::dmaSend(void *buf, uint16_t length, bool minc) {
  uint8_t *ptr = (uint8_t*)buf;
  while(length--) {
    if (_currentSetting->dataSize == DATA_SIZE_16BIT) spiSend(*(ptr + 1));
    spiSend(*ptr);
    if (minc) ptr += _currentSetting->dataSize;
  }
}

uint8_t SPIClass::dmaTransfer(const void * transmitBuf, void * receiveBuf, uint16_t length) {
  //todo: transmit?!
  read((uint8_t*)receiveBuf, length);
  return 1;
}

void SPIClass::setModule(uint8_t device) {
  _currentSetting = &_settings[device - 1];
}

void SPIClass::setClock(uint32_t clock) {
  _currentSetting->clock = clock;
}

void SPIClass::setBitOrder(uint8_t bitOrder) {
  _currentSetting->bitOrder = bitOrder;
}

void SPIClass::setDataMode(uint8_t dataMode) {
  _currentSetting->dataMode = dataMode;
}

void SPIClass::setDataSize(uint32_t ds) {
  _currentSetting->dataSize = ds;
}

SPIClass SPI(1);
