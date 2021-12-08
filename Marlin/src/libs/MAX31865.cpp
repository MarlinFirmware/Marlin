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
 * Based on Based on Adafruit MAX31865 library:
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

#include "../inc/MarlinConfig.h"

#if HAS_MAX31865 && !USE_ADAFRUIT_MAX31865

#include "MAX31865.h"

#ifndef MAX31865_MIN_SAMPLING_TIME_MSEC
  #define MAX31865_MIN_SAMPLING_TIME_MSEC 0
#endif

#ifdef TARGET_LPC1768
  #include <SoftwareSPI.h>
#endif

#define DEBUG_OUT ENABLED(DEBUG_MAX31865)
#include "../core/debug_out.h"

// The maximum speed the MAX31865 can do is 5 MHz
SPISettings MAX31865::spiConfig = SPISettings(
  TERN(TARGET_LPC1768, SPI_QUARTER_SPEED, TERN(ARDUINO_ARCH_STM32, SPI_CLOCK_DIV4, 500000)),
  MSBFIRST,
  SPI_MODE1 // CPOL0 CPHA1
);

#if DISABLED(LARGE_PINMAP)

  /**
   * Create the interface object using software (bitbang) SPI for PIN values
   * less than or equal to 127.
   *
   * @param spi_cs    the SPI CS pin to use
   * @param spi_mosi  the SPI MOSI pin to use
   * @param spi_miso  the SPI MISO pin to use
   * @param spi_clk   the SPI clock pin to use
  */
  MAX31865::MAX31865(int8_t spi_cs, int8_t spi_mosi, int8_t spi_miso, int8_t spi_clk) {
    cselPin = spi_cs;
    mosiPin = spi_mosi;
    misoPin = spi_miso;
    sclkPin = spi_clk;
  }

  /**
   * Create the interface object using hardware SPI for PIN for PIN values less
   * than or equal to 127.
   *
   * @param spi_cs  the SPI CS pin to use along with the default SPI device
   */
  MAX31865::MAX31865(int8_t spi_cs) {
    cselPin = spi_cs;
    sclkPin = misoPin = mosiPin = -1;
  }

#else // LARGE_PINMAP

  /**
   * Create the interface object using software (bitbang) SPI for PIN values
   * which are larger than 127. If you have PIN values less than or equal to
   * 127 use the other call for SW SPI.
   *
   * @param spi_cs       the SPI CS pin to use
   * @param spi_mosi     the SPI MOSI pin to use
   * @param spi_miso     the SPI MISO pin to use
   * @param spi_clk      the SPI clock pin to use
   * @param pin_mapping  set to 1 for positive pin values
   */
  MAX31865::MAX31865(uint32_t spi_cs, uint32_t spi_mosi, uint32_t spi_miso, uint32_t spi_clk, uint8_t pin_mapping) {
    cselPin = spi_cs;
    mosiPin = spi_mosi;
    misoPin = spi_miso;
    sclkPin = spi_clk;
  }

  /**
   * Create the interface object using hardware SPI for PIN values which are
   * larger than 127. If you have PIN values less than or equal to 127 use
   * the other call for HW SPI.
   *
   * @param spi_cs       the SPI CS pin to use along with the default SPI device
   * @param pin_mapping  set to 1 for positive pin values
   */
  MAX31865::MAX31865(uint32_t spi_cs, uint8_t pin_mapping) {
    cselPin = spi_cs;
    sclkPin = misoPin = mosiPin = -1UL;  //-1UL or 0xFFFFFFFF or 4294967295
  }

#endif // LARGE_PINMAP

/**
 *
 * Instance & Class methods
 *
 */

/**
 * Initialize the SPI interface and set the number of RTD wires used
 *
 * @param wires  The number of wires in enum format. Can be MAX31865_2WIRE, MAX31865_3WIRE, or MAX31865_4WIRE.
 * @param zero   The resistance of the RTD at 0 degC, in ohms.
 * @param ref    The resistance of the reference resistor, in ohms.
 * @param wire   The resistance of the wire connecting the sensor to the RTD, in ohms.
 */
void MAX31865::begin(max31865_numwires_t wires, float zero_res, float ref_res, float wire_res) {
  zeroRes = zero_res;
  refRes = ref_res;
  wireRes = wire_res;

  pinMode(cselPin, OUTPUT);
  digitalWrite(cselPin, HIGH);

  if (sclkPin != TERN(LARGE_PINMAP, -1UL, 255))
    softSpiBegin(SPI_QUARTER_SPEED); // Define pin modes for Software SPI
  else {
    DEBUG_ECHOLNPGM("Initializing MAX31865 Hardware SPI");
    SPI.begin();    // Start and configure hardware SPI
  }

  initFixedFlags(wires);

  clearFault(); // also initializes flags

  #if DISABLED(MAX31865_USE_AUTO_MODE) // make a proper first 1 shot read to initialize _lastRead

    enableBias();
    DELAY_US(11500);
    oneShot();
    DELAY_US(65000);
    uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG);

    if (rtd & 1) {
      lastRead = 0xFFFF; // some invalid value
      lastFault = readRegister8(MAX31865_FAULTSTAT_REG);
      clearFault(); // also clears the bias voltage flag, so no further action is required

      DEBUG_ECHOLNPGM("MAX31865 read fault: ", rtd);
    }
    else {
      DEBUG_ECHOLNPGM("RTD MSB:", (rtd >> 8), "  RTD LSB:", (rtd & 0x00FF));

      resetFlags();

      lastRead = rtd;
      nextEvent = SETUP_BIAS_VOLTAGE;
      millis_t now = millis();
      nextEventStamp = now + MAX31865_MIN_SAMPLING_TIME_MSEC;

      TERN_(MAX31865_USE_READ_ERROR_DETECTION, lastReadStamp = now);
    }

  #endif // !MAX31865_USE_AUTO_MODE

  DEBUG_ECHOLNPGM(
    TERN(LARGE_PINMAP, "LARGE_PINMAP", "Regular")
    " begin call with cselPin: ", cselPin,
    " misoPin: ", misoPin,
    " sclkPin: ", sclkPin,
    " mosiPin: ", mosiPin,
    " config: ", readRegister8(MAX31865_CONFIG_REG)
  );
}

/**
 * Return and clear the last fault value
 *
 * @return The raw unsigned 8-bit FAULT status register or spike fault
 */
uint8_t MAX31865::readFault() {
  uint8_t r = lastFault;
  lastFault = 0;
  return r;
}

/**
 * Clear last fault
 */
void MAX31865::clearFault() {
  setConfig(MAX31865_CONFIG_FAULTSTAT, 1);
}

/**
 * Reset flags
 */
void MAX31865::resetFlags() {
  writeRegister8(MAX31865_CONFIG_REG, stdFlags);
}

/**
 * Enable the bias voltage on the RTD sensor
 */
void MAX31865::enableBias() {
  setConfig(MAX31865_CONFIG_BIAS, 1);
}

/**
 * Start a one-shot temperature reading.
 */
void MAX31865::oneShot() {
  setConfig(MAX31865_CONFIG_1SHOT | MAX31865_CONFIG_BIAS, 1);
}

/**
 * Initialize standard flags with flags that will not change during operation (Hz, polling mode and no. of wires)
 *
 * @param wires The number of wires in enum format
 */
void MAX31865::initFixedFlags(max31865_numwires_t wires) {

  // set config-defined flags (same for all sensors)
  stdFlags = TERN(MAX31865_50HZ_FILTER, MAX31865_CONFIG_FILT50HZ, MAX31865_CONFIG_FILT60HZ) |
              TERN(MAX31865_USE_AUTO_MODE, MAX31865_CONFIG_MODEAUTO | MAX31865_CONFIG_BIAS, MAX31865_CONFIG_MODEOFF);

  if (wires == MAX31865_3WIRE)
    stdFlags |= MAX31865_CONFIG_3WIRE;
  else  // 2 or 4 wire
    stdFlags &= ~MAX31865_CONFIG_3WIRE;
}

/**
 * Read the raw 16-bit value from the RTD_REG in one shot mode. This will include
 * the fault bit, D0.
 *
 * @return The raw unsigned 16-bit register value with ERROR bit attached, NOT temperature!
 */
uint16_t MAX31865::readRaw() {

  #if ENABLED(MAX31865_USE_AUTO_MODE)

    const uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG);
    DEBUG_ECHOLNPGM("MAX31865 RTD MSB:", (rtd >> 8), " LSB:", (rtd & 0x00FF));

    if (rtd & 1) {
      lastFault = readRegister8(MAX31865_FAULTSTAT_REG);
      lastRead |= 1;
      clearFault(); // also clears the bias voltage flag, so no further action is required
      DEBUG_ECHOLNPGM("MAX31865 read fault: ", rtd);
    }
    #if ENABLED(MAX31865_USE_READ_ERROR_DETECTION)
      else if (ABS(lastRead - rtd) > 500 && PENDING(millis(), lastReadStamp + 1000)) { // if two readings within a second differ too much (~20°C), consider it a read error.
        lastFault = 0x01;
        lastRead |= 1;
        DEBUG_ECHOLNPGM("MAX31865 read error: ", rtd);
      }
    #endif
    else {
      lastRead = rtd;
      TERN_(MAX31865_USE_READ_ERROR_DETECTION, lastReadStamp = millis());
    }

  #else

    if (PENDING(millis(), nextEventStamp)) {
      DEBUG_ECHOLNPGM("MAX31865 waiting for event ", nextEvent);
      return lastRead;
    }

    switch (nextEvent) {
      case SETUP_BIAS_VOLTAGE:
        enableBias();
        nextEventStamp = millis() + 11; // wait at least 11msec before enabling 1shot
        nextEvent = SETUP_1_SHOT_MODE;
        DEBUG_ECHOLN("MAX31865 bias voltage enabled");
        break;

      case SETUP_1_SHOT_MODE:
        oneShot();
        nextEventStamp = millis() + 65; // wait at least 65msec before reading RTD register
        nextEvent = READ_RTD_REG;
        DEBUG_ECHOLN("MAX31865 1 shot mode enabled");
        break;

      case READ_RTD_REG: {
        const uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG);
        DEBUG_ECHOLNPGM("MAX31865 RTD MSB:", (rtd >> 8), " LSB:", (rtd & 0x00FF));

        if (rtd & 1) {
          lastFault = readRegister8(MAX31865_FAULTSTAT_REG);
          lastRead |= 1;
          clearFault(); // also clears the bias voltage flag, so no further action is required
          DEBUG_ECHOLNPGM("MAX31865 read fault: ", rtd);
        }
        #if ENABLED(MAX31865_USE_READ_ERROR_DETECTION)
          else if (ABS(lastRead - rtd) > 500 && PENDING(millis(), lastReadStamp + 1000)) { // if two readings within a second differ too much (~20°C), consider it a read error.
            lastFault = 0x01;
            lastRead |= 1;
            DEBUG_ECHOLNPGM("MAX31865 read error: ", rtd);
          }
        #endif
        else {
          lastRead = rtd;
          TERN_(MAX31865_USE_READ_ERROR_DETECTION, lastReadStamp = millis());
        }

        if (!(rtd & 1))   // if clearFault() was not invoked, need to clear the bias voltage and 1-shot flags
          resetFlags();

        nextEvent = SETUP_BIAS_VOLTAGE;
        nextEventStamp = millis() + MAX31865_MIN_SAMPLING_TIME_MSEC; // next step should not occur within less than MAX31865_MIN_SAMPLING_TIME_MSEC from the last one
      } break;
    }

  #endif

  return lastRead;
}

/**
 * Calculate and return the resistance value of the connected RTD.
 *
 * @return             The raw RTD resistance value, NOT temperature!
 */
float MAX31865::readResistance() {
  // Strip the error bit (D0) and convert to a float ratio.
  // less precise method: (readRaw() * refRes) >> 16
  return ((readRaw() * RECIPROCAL(65536.0f)) * refRes - wireRes);
}

/**
 * Read the RTD and pass it to temperature(float) for calculation.
 *
 * @return  Temperature in C
 */
float MAX31865::temperature() {
  return temperature(readResistance());
}

/**
 * Given the 15-bit ADC value, calculate the resistance and pass it to temperature(float) for calculation.
 *
 * @return  Temperature in C
 */
float MAX31865::temperature(uint16_t adc_val) {
  return temperature(((adc_val) * RECIPROCAL(32768.0f)) * refRes - wireRes);
}

/**
 * Calculate the temperature in C from the RTD resistance.
 * Uses the technique outlined in this PDF:
 * http://www.analog.com/media/en/technical-documentation/application-notes/AN709_0.pdf
 *
 * @param    rtd_res  the resistance value in ohms
 * @return            the temperature in degC
 */
float MAX31865::temperature(float rtd_res) {
  float temp = (RTD_Z1 + sqrt(RTD_Z2 + (RTD_Z3 * rtd_res))) * RECIPROCAL(RTD_Z4);

  // From the PDF...
  //
  // The previous equation is valid only for temperatures of 0°C and above.
  // The equation for RRTD(t) that defines negative temperature behavior is a
  // fourth-order polynomial (after expanding the third term) and is quite
  // impractical to solve for a single expression of temperature as a function
  // of resistance.
  //
  if (temp < 0) {
    rtd_res = (rtd_res / zeroRes) * 100; // normalize to 100 ohm
    float rpoly = rtd_res;

    temp = -242.02 + (2.2228 * rpoly);
    rpoly *= rtd_res; // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= rtd_res; // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= rtd_res; // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= rtd_res; // ^5
    temp += 1.5243e-10 * rpoly;
  }

  return temp;
}

//
// private:
//

/**
 * Set a value in the configuration register.
 *
 * @param config  8-bit value for the config item
 * @param enable  whether to enable or disable the value
 */
void MAX31865::setConfig(uint8_t config, bool enable) {
  uint8_t t = stdFlags;
  if (enable) t |= config; else t &= ~config;
  writeRegister8(MAX31865_CONFIG_REG, t);
}

/**
 * Read a single byte from the specified register address.
 *
 * @param  addr  the register address
 * @return       the register contents
 */
uint8_t MAX31865::readRegister8(uint8_t addr) {
  uint8_t ret = 0;
  readRegisterN(addr, &ret, 1);
  return ret;
}

/**
 * Read two bytes: 1 from the specified register address, and 1 from the next address.
 *
 * @param  addr  the first register address
 * @return       both register contents as a single 16-bit int
 */
uint16_t MAX31865::readRegister16(uint8_t addr) {
  uint8_t buffer[2] = { 0 };
  readRegisterN(addr, buffer, 2);
  return uint16_t(buffer[0]) << 8 | buffer[1];
}

/**
 * Read +n+ bytes from a specified address into +buffer+. Set D7 to 0 to specify a read.
 *
 * @param addr    the first register address
 * @param buffer  storage for the read bytes
 * @param n       the number of bytes to read
 */
void MAX31865::readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n) {
  addr &= 0x7F; // make sure top bit is not set
  if (sclkPin == TERN(LARGE_PINMAP, -1UL, 255))
    SPI.beginTransaction(spiConfig);
  else
    digitalWrite(sclkPin, LOW);

  digitalWrite(cselPin, LOW);

  #ifdef TARGET_LPC1768
    DELAY_CYCLES(spiSpeed);
  #endif

  spiTransfer(addr);

  while (n--) {
    buffer[0] = spiTransfer(0xFF);
    buffer++;
  }

  if (sclkPin == TERN(LARGE_PINMAP, -1UL, 255))
    SPI.endTransaction();

  digitalWrite(cselPin, HIGH);
}

/**
 * Write an 8-bit value to a register. Set D7 to 1 to specify a write.
 *
 * @param addr  the address to write to
 * @param data  the data to write
 */
void MAX31865::writeRegister8(uint8_t addr, uint8_t data) {
  if (sclkPin == TERN(LARGE_PINMAP, -1UL, 255))
    SPI.beginTransaction(spiConfig);
  else
    digitalWrite(sclkPin, LOW);

  digitalWrite(cselPin, LOW);

  #ifdef TARGET_LPC1768
    DELAY_CYCLES(spiSpeed);
  #endif

  spiTransfer(addr | 0x80); // make sure top bit is set
  spiTransfer(data);

  if (sclkPin == TERN(LARGE_PINMAP, -1UL, 255))
    SPI.endTransaction();

  digitalWrite(cselPin, HIGH);
}

/**
 * Transfer SPI data +x+ and read the response. From the datasheet...
 * Input data (SDI) is latched on the internal strobe edge and output data (SDO) is
 * shifted out on the shift edge. There is one clock for each bit transferred.
 * Address and data bits are transferred in groups of eight, MSB first.
 *
 * @param  x  an 8-bit chunk of data to write
 * @return    the 8-bit response
 */
uint8_t MAX31865::spiTransfer(uint8_t x) {

  if (sclkPin == TERN(LARGE_PINMAP, -1UL, 255))
    return SPI.transfer(x);

  #ifdef TARGET_LPC1768

    return swSpiTransfer(x, spiSpeed, sclkPin, misoPin, mosiPin);

  #else

    uint8_t reply = 0;
    for (int i = 7; i >= 0; i--) {
      digitalWrite(sclkPin, HIGH);       DELAY_NS_VAR(spiDelay);
      reply <<= 1;
      digitalWrite(mosiPin, x & _BV(i)); DELAY_NS_VAR(spiDelay);
      if (digitalRead(misoPin)) reply |= 1;
      digitalWrite(sclkPin, LOW);        DELAY_NS_VAR(spiDelay);
    }
    return reply;

  #endif
}

void MAX31865::softSpiBegin(const uint8_t spi_speed) {
  DEBUG_ECHOLNPGM("Initializing MAX31865 Software SPI");

  #ifdef TARGET_LPC1768
    swSpiBegin(sclkPin, misoPin, mosiPin);
    spiSpeed = swSpiInit(spi_speed, sclkPin, mosiPin);
  #else
    spiDelay = (100UL << spi_speed) / 3; // Calculate delay in ns. Top speed is ~10MHz, or 100ns delay between bits.
    pinMode(sclkPin, OUTPUT);
    digitalWrite(sclkPin, LOW);
    pinMode(mosiPin, OUTPUT);
    pinMode(misoPin, INPUT);
  #endif
}

#endif // HAS_MAX31865 && !USE_ADAFRUIT_MAX31865
