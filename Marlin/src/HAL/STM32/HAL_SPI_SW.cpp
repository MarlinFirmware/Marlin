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
#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

// ------------------------
// Public functions
// ------------------------

#if ENABLED(SOFTWARE_SPI)

  #include <SPI.h>

  // ------------------------
  // Software SPI
  // ------------------------

  // TODO: this software SPI is really bad... it tries to use SPI clock-mode 3 only...??????

  #include "../shared/Delay.h"

  void spiBegin(void) {
    #if PIN_EXISTS(SD_SS)
      OUT_WRITE(SD_SS_PIN, HIGH);
    #endif
    OUT_WRITE(SD_SCK_PIN, HIGH);
    SET_INPUT(SD_MISO_PIN);
    OUT_WRITE(SD_MOSI_PIN, HIGH);
  }

  // Use function with compile-time value so we can actually reach the desired frequency
  // Need to adjust this a little bit: on a 72MHz clock, we have 14ns/clock
  // and we'll use ~3 cycles to jump to the method and going back, so it'll take ~40ns from the given clock here
  #define CALLING_COST_NS  (3U * 1000000000U) / (F_CPU)
  static void (*delaySPIFunc)();
  void delaySPI_125()  { DELAY_NS(125 - CALLING_COST_NS); }
  void delaySPI_250()  { DELAY_NS(250 - CALLING_COST_NS); }
  void delaySPI_500()  { DELAY_NS(500 - CALLING_COST_NS); }
  void delaySPI_1000() { DELAY_NS(1000 - CALLING_COST_NS); }
  void delaySPI_2000() { DELAY_NS(2000 - CALLING_COST_NS); }
  void delaySPI_4000() { DELAY_NS(4000 - CALLING_COST_NS); }

  static int _spi_bit_order = SPI_BITORDER_DEFAULT;

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use datarates Marlin uses
    switch (spiRate) {
      case SPI_FULL_SPEED:   delaySPIFunc =  &delaySPI_125; break;  // desired: 8,000,000  actual: ~1.1M
      case SPI_HALF_SPEED:   delaySPIFunc =  &delaySPI_125; break;  // desired: 4,000,000  actual: ~1.1M
      case SPI_QUARTER_SPEED:delaySPIFunc =  &delaySPI_250; break;  // desired: 2,000,000  actual: ~890K
      case SPI_EIGHTH_SPEED: delaySPIFunc =  &delaySPI_500; break;  // desired: 1,000,000  actual: ~590K
      case SPI_SPEED_5:      delaySPIFunc = &delaySPI_1000; break;  // desired:   500,000  actual: ~360K
      case SPI_SPEED_6:      delaySPIFunc = &delaySPI_2000; break;  // desired:   250,000  actual: ~210K
      default:               delaySPIFunc = &delaySPI_4000; break;  // desired:   125,000  actual: ~123K
    }
    // TODO: there is an issue on Github by BTT(?) that this does not use software but hardware SPI.
    // we are kind of lying to the user here, is that OK?
    SPI.setMISO(hint_miso);
    SPI.setMOSI(hint_mosi);
    SPI.setSCLK(hint_sck);
    SPI.begin();
  }

  void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use datarates Marlin uses
    uint8_t spiRate;
    if (maxClockFreq >= 20000000) {
      spiRate = SPI_FULL_SPEED;
    }
    else if (maxClockFreq >= 5000000) {
      spiRate = SPI_HALF_SPEED;
    }
    else if (maxClockFreq >= 2500000) {
      spiRate = SPI_QUARTER_SPEED;
    }
    else if (maxClockFreq >= 1250000) {
      spiRate = SPI_EIGHTH_SPEED;
    }
    else if (maxClockFreq >= 625000) {
      spiRate = SPI_SPEED_5;
    }
    else if (maxClockFreq >= 300000) {
      spiRate = SPI_SPEED_6;
    }
    else
      spiRate = SPI_SPEED_6;
    
    spiInit(spiRate, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  void spiClose() {
    SPI.end();
  }

  void spiSetBitOrder(int bitOrder) {
    _spi_bit_order = bitOrder;
  }

  void spiSetClockMode(int clockMode) {
    if (clockMode != SPI_CLKMODE_3)
      _spi_on_error();
  }

  uint8_t HAL_SPI_STM32_SpiTransfer_Mode_3(uint8_t b) { // using Mode 3
    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );
    uint8_t result = 0;
    for (uint8_t bits = 0; bits < 8; bits++) {
      int bitidx = ( msb ? 7-bits : bits );
      WRITE(SD_SCK_PIN, LOW);
      WRITE(SD_MOSI_PIN, (b & ( 1 << bitidx )) != 0);

      delaySPIFunc();
      WRITE(SD_SCK_PIN, HIGH);
      delaySPIFunc();

      result |= ( (READ(SD_MISO_PIN) != 0) << bitidx );
    }
    DELAY_NS(125);
    return b;
  }

  uint16_t HAL_SPI_STM32_SpiTransfer_Mode_3_16bits(uint16_t v) { // using Mode 3
    bool msb = ( _spi_bit_order == SPI_BITORDER_MSB );
    uint16_t result = 0;
    for (uint8_t bits = 0; bits < 16; bits++) {
      int bitidx = ( msb ? 15-bits : bits );
      WRITE(SD_SCK_PIN, LOW);
      WRITE(SD_MOSI_PIN, (b & ( 1 << bitidx )) != 0);

      delaySPIFunc();
      WRITE(SD_SCK_PIN, HIGH);
      delaySPIFunc();

      result |= ( (READ(SD_MISO_PIN) != 0) << bitidx );
    }
    DELAY_NS(125);
    return b;
  }

  // Soft SPI receive byte
  uint8_t spiRec(uint8_t txval) {
    hal.isr_off();                                                // No interrupts during byte receive
    const uint8_t data = HAL_SPI_STM32_SpiTransfer_Mode_3(txval);
    hal.isr_on();                                                 // Enable interrupts
    return data;
  }

  uint16_t spiRec16(uint16_t txval) {
    hal.isr_off();
    uint16_t data = HAL_SPI_STM32_SpiTransfer_Mode_3_16bits(txval);
    hal.isr_on();
    return data;
  }

  // Soft SPI read data
  void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
    for (uint16_t i = 0; i < nbyte; i++)
      buf[i] = spiRec(txval);
  }

  // Soft SPI send byte
  void spiSend(uint8_t data) {
    hal.isr_off();                          // No interrupts during byte send
    HAL_SPI_STM32_SpiTransfer_Mode_3(data); // Don't care what is received
    hal.isr_on();                           // Enable interrupts
  }

  void spiSend16(uint16_t data) {
    hal.isr_off();
    HAL_SPI_STM32_SpiTransfer_Mode_3_16bits(data);
    hal.isr_on();
  }

  // Soft SPI send block
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    spiSend(token);
    for (uint16_t i = 0; i < 512; i++)
      spiSend(buf[i]);
  }

  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend(buf[n]);
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend16(buf[n]);
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend(val);
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend16(val);
  }

#endif

#endif