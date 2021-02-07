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

#include "../../shared/HAL_SPI.h"

#include <stdint.h>
#include <lpc17xx_ssp.h>
#include <lpc17xx_gpdma.h>

//#define MSBFIRST 1

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define DATA_SIZE_8BIT SSP_DATABIT_8
#define DATA_SIZE_16BIT SSP_DATABIT_16

#define SPI_CLOCK_MAX_TFT  30000000UL
#define SPI_CLOCK_DIV2     8333333 //(SCR:  2)  desired: 8,000,000  actual: 8,333,333  +4.2%  SPI_FULL_SPEED
#define SPI_CLOCK_DIV4     4166667 //(SCR:  5)  desired: 4,000,000  actual: 4,166,667  +4.2%  SPI_HALF_SPEED
#define SPI_CLOCK_DIV8     2083333 //(SCR: 11)  desired: 2,000,000  actual: 2,083,333  +4.2%  SPI_QUARTER_SPEED
#define SPI_CLOCK_DIV16    1000000 //(SCR: 24)  desired: 1,000,000  actual: 1,000,000         SPI_EIGHTH_SPEED
#define SPI_CLOCK_DIV32     500000 //(SCR: 49)  desired:   500,000  actual:   500,000         SPI_SPEED_5
#define SPI_CLOCK_DIV64     250000 //(SCR: 99)  desired:   250,000  actual:   250,000         SPI_SPEED_6
#define SPI_CLOCK_DIV128    125000 //(SCR:199)  desired:   125,000  actual:   125,000         Default from HAL.h

#define SPI_CLOCK_MAX SPI_CLOCK_DIV2

#define BOARD_NR_SPI 2

//#define BOARD_SPI1_NSS_PIN      PA4 ?!
#define BOARD_SPI1_SCK_PIN      P0_15
#define BOARD_SPI1_MISO_PIN     P0_17
#define BOARD_SPI1_MOSI_PIN     P0_18

//#define BOARD_SPI2_NSS_PIN      PB12 ?!
#define BOARD_SPI2_SCK_PIN      P0_07
#define BOARD_SPI2_MISO_PIN     P0_08
#define BOARD_SPI2_MOSI_PIN     P0_09

class SPISettings {
public:
  SPISettings(uint32_t spiRate, int inBitOrder, int inDataMode) {
    init_AlwaysInline(spiRate2Clock(spiRate), inBitOrder, inDataMode, DATA_SIZE_8BIT);
  }
  SPISettings(uint32_t inClock, uint8_t inBitOrder, uint8_t inDataMode, uint32_t inDataSize) {
    if (__builtin_constant_p(inClock))
      init_AlwaysInline(inClock, inBitOrder, inDataMode, inDataSize);
    else
      init_MightInline(inClock, inBitOrder, inDataMode, inDataSize);
  }
  SPISettings() {
    init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
  }

  //uint32_t spiRate() const { return spi_speed; }

  static inline uint32_t spiRate2Clock(uint32_t spiRate) {
    uint32_t Marlin_speed[7]; // CPSR is always 2
    Marlin_speed[0] = 8333333; //(SCR:  2)  desired: 8,000,000  actual: 8,333,333  +4.2%  SPI_FULL_SPEED
    Marlin_speed[1] = 4166667; //(SCR:  5)  desired: 4,000,000  actual: 4,166,667  +4.2%  SPI_HALF_SPEED
    Marlin_speed[2] = 2083333; //(SCR: 11)  desired: 2,000,000  actual: 2,083,333  +4.2%  SPI_QUARTER_SPEED
    Marlin_speed[3] = 1000000; //(SCR: 24)  desired: 1,000,000  actual: 1,000,000         SPI_EIGHTH_SPEED
    Marlin_speed[4] =  500000; //(SCR: 49)  desired:   500,000  actual:   500,000         SPI_SPEED_5
    Marlin_speed[5] =  250000; //(SCR: 99)  desired:   250,000  actual:   250,000         SPI_SPEED_6
    Marlin_speed[6] =  125000; //(SCR:199)  desired:   125,000  actual:   125,000         Default from HAL.h
    return Marlin_speed[spiRate > 6 ? 6 : spiRate];
  }

private:
  void init_MightInline(uint32_t inClock, uint8_t inBitOrder, uint8_t inDataMode, uint32_t inDataSize) {
    init_AlwaysInline(inClock, inBitOrder, inDataMode, inDataSize);
  }
  void init_AlwaysInline(uint32_t inClock, uint8_t inBitOrder, uint8_t inDataMode, uint32_t inDataSize) __attribute__((__always_inline__)) {
    clock    = inClock;
    bitOrder = inBitOrder;
    dataMode = inDataMode;
    dataSize = inDataSize;
  }

  //uint32_t spi_speed;
  uint32_t clock;
  uint32_t dataSize;
  //uint32_t clockDivider;
  uint8_t bitOrder;
  uint8_t dataMode;
  LPC_SSP_TypeDef *spi_d;

  friend class SPIClass;
};

/**
 * @brief Wirish SPI interface.
 *
 * This is the same interface is available across HAL
 *
 * This implementation uses software slave management, so the caller
 * is responsible for controlling the slave select line.
 */
class SPIClass {
public:
  /**
   * @param spiPortNumber Number of the SPI port to manage.
   */
  SPIClass(uint8_t spiPortNumber);

  /**
   * Init using pins
   */
  SPIClass(pin_t mosi, pin_t miso, pin_t sclk, pin_t ssel = (pin_t)-1);

  /**
   * Select and configure the current selected SPI device to use
   */
  void begin();

  /**
   * Disable the current SPI device
   */
  void end();

  void beginTransaction(const SPISettings&);
  void endTransaction() {}

  // Transfer using 1 "Data Size"
  uint8_t transfer(uint16_t data);
  // Transfer 2 bytes in 8 bit mode
  uint16_t transfer16(uint16_t data);

  void send(uint8_t data);

  uint16_t read();
  void read(uint8_t *buf, uint32_t len);

  void dmaSend(void *buf, uint16_t length, bool minc);

  /**
   * @brief Sets the number of the SPI peripheral to be used by
   *        this HardwareSPI instance.
   *
   * @param spi_num Number of the SPI port. 1-2 in low density devices
   *     or 1-3 in high density devices.
   */
  void setModule(uint8_t device);

  void setClock(uint32_t clock);
  void setBitOrder(uint8_t bitOrder);
  void setDataMode(uint8_t dataMode);
  void setDataSize(uint32_t ds);

  inline uint32_t getDataSize() { return _currentSetting->dataSize; }

private:
  SPISettings _settings[BOARD_NR_SPI];
  SPISettings *_currentSetting;

  void updateSettings();
};

extern SPIClass SPI;
