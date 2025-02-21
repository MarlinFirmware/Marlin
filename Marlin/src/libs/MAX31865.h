/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Based on Adafruit MAX31865 library:
 *
 * This is a library for the Adafruit PT100/P1000 RTD Sensor w/MAX31865
 * Designed specifically to work with the Adafruit RTD Sensor
 * https://www.adafruit.com/products/3328
 *
 * This sensor uses SPI to communicate, 4 pins are required to interface.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * Modifications by JoAnn Manges (@GadgetAngel)
 * Copyright (c) 2020, JoAnn Manges
 * All rights reserved.
 */
#pragma once

//#define DEBUG_MAX31865

#include "../inc/MarlinConfig.h"
#include "../HAL/shared/Delay.h"
#include HAL_PATH(../HAL, MarlinSPI.h)

#define MAX31865_CONFIG_REG 0x00
#define MAX31865_CONFIG_BIAS 0x80
#define MAX31865_CONFIG_MODEAUTO 0x40
#define MAX31865_CONFIG_MODEOFF 0x00
#define MAX31865_CONFIG_1SHOT 0x20
#define MAX31865_CONFIG_3WIRE 0x10
#define MAX31865_CONFIG_24WIRE 0x00
#define MAX31865_CONFIG_FAULTSTAT 0x02
#define MAX31865_CONFIG_FILT50HZ 0x01
#define MAX31865_CONFIG_FILT60HZ 0x00

#define MAX31865_RTDMSB_REG 0x01
#define MAX31865_RTDLSB_REG 0x02
#define MAX31865_HFAULTMSB_REG 0x03
#define MAX31865_HFAULTLSB_REG 0x04
#define MAX31865_LFAULTMSB_REG 0x05
#define MAX31865_LFAULTLSB_REG 0x06
#define MAX31865_FAULTSTAT_REG 0x07

#define MAX31865_FAULT_HIGHTHRESH 0x80  // D7
#define MAX31865_FAULT_LOWTHRESH 0x40   // D6
#define MAX31865_FAULT_REFINLOW 0x20    // D5
#define MAX31865_FAULT_REFINHIGH 0x10   // D4
#define MAX31865_FAULT_RTDINLOW 0x08    // D3
#define MAX31865_FAULT_OVUV 0x04        // D2

typedef enum max31865_numwires {
  MAX31865_2WIRE = 0,
  MAX31865_3WIRE = 1,
  MAX31865_4WIRE = 0
} max31865_numwires_t;

#if DISABLED(MAX31865_USE_AUTO_MODE)
  typedef enum one_shot_event : uint8_t {
    SETUP_BIAS_VOLTAGE,
    SETUP_1_SHOT_MODE,
    READ_RTD_REG
  } one_shot_event_t;
#endif

/* Interface class for the MAX31865 RTD Sensor reader */
class MAX31865 {
private:
  static SPISettings spiConfig;

  TERN(LARGE_PINMAP, uint32_t, uint8_t) sclkPin, misoPin, mosiPin, cselPin;

  uint16_t spiDelay;

  float resNormalizer, refRes, wireRes;

  #if ENABLED(MAX31865_USE_READ_ERROR_DETECTION)
    millis_t lastReadStamp = 0;
  #endif

  uint16_t lastRead = 0;
  uint8_t lastFault = 0;

  #if DISABLED(MAX31865_USE_AUTO_MODE)
    millis_t nextEventStamp;
    one_shot_event_t nextEvent;
  #endif

  #ifdef MAX31865_IGNORE_INITIAL_FAULTY_READS
    uint8_t ignore_faults = MAX31865_IGNORE_INITIAL_FAULTY_READS;
    uint16_t fixFault(uint16_t rtd);
  #endif

  uint8_t stdFlags = 0;

  void setConfig(uint8_t config, bool enable);

  void readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n);
  uint8_t readRegister8(uint8_t addr);
  uint16_t readRegister16(uint8_t addr);

  void writeRegister8(uint8_t addr, uint8_t reg);
  void writeRegister16(uint8_t addr, uint16_t reg);

  void softSpiInit();
  void spiBeginTransaction();
  uint8_t spiTransfer(uint8_t addr);
  void spiEndTransaction();

  void initFixedFlags(max31865_numwires_t wires);

  void enable50HzFilter(bool b);
  void enableBias();
  void oneShot();
  void resetFlags();

  uint16_t readRawImmediate();

  void runAutoFaultDetectionCycle();

public:
  #if ENABLED(LARGE_PINMAP)
    MAX31865(uint32_t spi_cs, uint8_t pin_mapping);
    MAX31865(uint32_t spi_cs, uint32_t spi_mosi, uint32_t spi_miso,
             uint32_t spi_clk, uint8_t pin_mapping);
  #else
    MAX31865(int8_t spi_cs);
    MAX31865(int8_t spi_cs, int8_t spi_mosi, int8_t spi_miso,
             int8_t spi_clk);
  #endif

  void begin(max31865_numwires_t wires, const_float_t zero_res, const_float_t ref_res, const_float_t wire_res);

  uint8_t readFault();
  void clearFault();

  uint16_t readRaw();
  float readResistance();
  float temperature();
  float temperature(const uint16_t adc_val);
  float temperature(float rtd_res);
};
