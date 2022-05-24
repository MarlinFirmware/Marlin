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
    softSpiInit(); // Define pin modes for Software SPI
  else {
    DEBUG_ECHOLNPGM("Init MAX31865 Hardware SPI");
    SPI.begin();    // Start and configure hardware SPI
  }

  initFixedFlags(wires);

  DEBUG_ECHOLNPGM("MAX31865 Regs: CFG ", readRegister8(MAX31865_CONFIG_REG),
    "|RTD ", readRegister16(MAX31865_RTDMSB_REG),
    "|HTHRS ", readRegister16(MAX31865_HFAULTMSB_REG),
    "|LTHRS ", readRegister16(MAX31865_LFAULTMSB_REG),
    "|FLT  ", readRegister8(MAX31865_FAULTSTAT_REG));

  // fault detection cycle seems to initialize the sensor better
  runAutoFaultDetectionCycle(); // also initializes flags

  if (lastFault)
    SERIAL_ECHOLNPGM("MAX31865 init fault ", lastFault);

  writeRegister16(MAX31865_HFAULTMSB_REG, 0xFFFF);
  writeRegister16(MAX31865_LFAULTMSB_REG, 0);

  #if ENABLED(MAX31865_USE_AUTO_MODE) // make a proper first read to initialize _lastRead

   uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG);

    #if MAX31865_IGNORE_INITIAL_FAULTY_READS > 0
      rtd = fixFault(rtd);
    #endif

    if (rtd & 1) {
      lastRead = 0xFFFF; // some invalid value
      lastFault = readRegister8(MAX31865_FAULTSTAT_REG);
      clearFault(); // also clears the bias voltage flag, so no further action is required

      DEBUG_ECHOLNPGM("MAX31865 read fault: ", rtd);
    }
    else {
      DEBUG_ECHOLNPGM("RTD MSB:", (rtd >> 8), "  RTD LSB:", (rtd & 0x00FF));
      lastRead = rtd;
      TERN_(MAX31865_USE_READ_ERROR_DETECTION, lastReadStamp = millis());
    }

  #else

    enableBias();
    DELAY_US(2000); // according to the datasheet, 10.5τ+1msec (see below)
    oneShot();
    DELAY_US(63000);
    uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG);

    #if MAX31865_IGNORE_INITIAL_FAULTY_READS > 0
      rtd = fixFault(rtd);
    #endif

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

  #endif // MAX31865_USE_AUTO_MODE

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

void MAX31865::runAutoFaultDetectionCycle() {
  writeRegister8(MAX31865_CONFIG_REG, (stdFlags & 0x11) | 0x84 ); // cfg reg = 100X010Xb
  DELAY_US(600);
  for (int i = 0; i < 10 && (readRegister8(MAX31865_CONFIG_REG) & 0xC) > 0; i++) DELAY_US(100); // Fault det completes when bits 2 and 3 are zero (or after 10 tries)
  readFault();
  clearFault();
}

/**
 * Set a value in the configuration register.
 *
 * @param config  8-bit value for the config item
 * @param enable  whether to enable or disable the value
 */
void MAX31865::setConfig(uint8_t config, bool enable) {
  uint8_t t = stdFlags;
  if (enable)
    t |= config;
  else
    t &= ~config;
  writeRegister8(MAX31865_CONFIG_REG, t);
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
    stdFlags |= MAX31865_CONFIG_3WIRE;   // 3 wire
  else
    stdFlags &= ~MAX31865_CONFIG_3WIRE;  // 2 or 4 wire
}

#if MAX31865_IGNORE_INITIAL_FAULTY_READS > 0

  inline uint16_t MAX31865::fixFault(uint16_t rtd) {
    if (!ignore_faults || !(rtd & 1))
      return rtd;

    ignore_faults--;
    clearFault();

    DEBUG_ECHOLNPGM("MAX31865 ignoring fault ", (MAX31865_IGNORE_INITIAL_FAULTY_READS) - ignore_faults);

    return rtd & ~1;  // 0xFFFE
  }

#endif

inline uint16_t MAX31865::readRawImmediate() {
  uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG);
  DEBUG_ECHOLNPGM("MAX31865 RTD MSB:", (rtd >> 8), " LSB:", (rtd & 0x00FF));

  #if MAX31865_IGNORE_INITIAL_FAULTY_READS > 0
    rtd = fixFault(rtd);
  #endif

  if (rtd & 1) {
    lastFault = readRegister8(MAX31865_FAULTSTAT_REG);
    lastRead |= 1;
    clearFault(); // also clears the bias voltage flag, so no further action is required
    DEBUG_ECHOLNPGM("MAX31865 read fault: ", lastFault);
  }
  else {
    TERN_(MAX31865_USE_READ_ERROR_DETECTION, const millis_t ms = millis());
    if (TERN0(MAX31865_USE_READ_ERROR_DETECTION, ABS((int)(lastRead - rtd)) > 500 && PENDING(ms, lastReadStamp + 1000))) {
      // If 2 readings within 1s differ too much (~20°C) it's a read error.
      lastFault = 0x01;
      lastRead |= 1;
      DEBUG_ECHOLNPGM("MAX31865 read error: ", rtd);
    }
    else {
      lastRead = rtd;
      TERN_(MAX31865_USE_READ_ERROR_DETECTION, lastReadStamp = ms);
    }
  }

  return rtd;
}

/**
 * Read the raw 16-bit value from the RTD_REG in one shot mode. This will include
 * the fault bit, D0.
 *
 * @return The raw unsigned 16-bit register value with ERROR bit attached, NOT temperature!
 */
uint16_t MAX31865::readRaw() {

  #if ENABLED(MAX31865_USE_AUTO_MODE)

    readRawImmediate();

  #else

    const millis_t ms = millis();

    if (PENDING(ms, nextEventStamp)) {
      DEBUG_ECHOLNPGM("MAX31865 waiting for event ", nextEvent);
      return lastRead;
    }

    switch (nextEvent) {
      case SETUP_BIAS_VOLTAGE:
        enableBias();
        nextEventStamp = ms + 2; // wait at least 10.5*τ (τ = 100nF*430Ω max for PT100 / 10nF*4.3ΚΩ for PT1000 = 43μsec) + 1msec
        nextEvent = SETUP_1_SHOT_MODE;
        DEBUG_ECHOLNPGM("MAX31865 bias voltage enabled");
        break;

      case SETUP_1_SHOT_MODE:
        oneShot();
        nextEventStamp = ms + TERN(MAX31865_50HZ_FILTER, 63, 52); // wait at least 52msec for 60Hz (63msec for 50Hz) before reading RTD register
        nextEvent = READ_RTD_REG;
        DEBUG_ECHOLNPGM("MAX31865 1 shot mode enabled");
        break;

      case READ_RTD_REG:

        if (!(readRawImmediate() & 1))   // if clearFault() was not invoked, need to clear the bias voltage and 1-shot flags
          resetFlags();

        nextEvent = SETUP_BIAS_VOLTAGE;
        nextEventStamp = ms + (MAX31865_MIN_SAMPLING_TIME_MSEC); // next step should not occur within less than MAX31865_MIN_SAMPLING_TIME_MSEC from the last one
        break;
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

/**
 * MAX31865 SPI Timing constants
 * See MAX31865 datasheet (https://datasheets.maximintegrated.com/en/ds/MAX31865.pdf)
 * All timings in nsec, minimum values.
 */

#define MAX31865_SPI_TIMING_TCC    400    // CS to SCLK setup
#define MAX31865_SPI_TIMING_TDC     35    // Data to SCLK setup
#define MAX31865_SPI_TIMING_TCL    100    // SCK half period
#define MAX31865_SPI_TIMING_TCCH   100    // SCK to CS hold
#define MAX31865_SPI_TIMING_TCWH   400    // CS inactive time (min)

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

  spiBeginTransaction();
  spiTransfer(addr);

  while (n--) {
    buffer[0] = spiTransfer(0xFF);
    buffer++;
  }

  spiEndTransaction();
}

void MAX31865::writeRegister16(uint8_t addr, uint16_t data) {
  spiBeginTransaction();
  spiTransfer(addr | 0x80); // make sure top bit is set
  spiTransfer(data >> 8);
  spiTransfer(data & 0xFF);
  spiEndTransaction();
}

/**
 * Write an 8-bit value to a register. Set D7 to 1 to specify a write.
 *
 * @param addr  the address to write to
 * @param data  the data to write
 */
void MAX31865::writeRegister8(uint8_t addr, uint8_t data) {
  spiBeginTransaction();
  spiTransfer(addr | 0x80); // make sure top bit is set
  spiTransfer(data);
  spiEndTransaction();
}

void MAX31865::spiBeginTransaction() {
  digitalWrite(sclkPin, LOW); // ensure CPOL0
  DELAY_NS_VAR(MAX31865_SPI_TIMING_TCWH); // ensure minimum time of CS inactivity after previous operation
  digitalWrite(cselPin, LOW);
  DELAY_NS_VAR(MAX31865_SPI_TIMING_TCC);

  if (sclkPin == TERN(LARGE_PINMAP, -1UL, 255))
    SPI.beginTransaction(spiConfig);
  else
    digitalWrite(sclkPin, HIGH);
}

void MAX31865::spiEndTransaction() {
  if (sclkPin == TERN(LARGE_PINMAP, -1UL, 255))
    SPI.endTransaction();
  else
    digitalWrite(sclkPin, LOW);

  DELAY_NS_VAR(MAX31865_SPI_TIMING_TCCH);

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

  uint8_t reply = 0;
  for (int i = 7; i >= 0; i--) {
    digitalWrite(mosiPin, x & _BV(i));
    DELAY_NS_VAR(MAX31865_SPI_TIMING_TDC);
    digitalWrite(sclkPin, LOW);
    DELAY_NS_VAR(MAX31865_SPI_TIMING_TCL - MAX31865_SPI_TIMING_TDC);
    reply <<= 1;
    if (digitalRead(misoPin)) reply |= 1;
    DELAY_NS_VAR(MAX31865_SPI_TIMING_TDC);
    digitalWrite(sclkPin, HIGH);
    DELAY_NS_VAR(MAX31865_SPI_TIMING_TCL - MAX31865_SPI_TIMING_TDC);
  }
  return reply;
}

void MAX31865::softSpiInit() {
  DEBUG_ECHOLNPGM("Initializing MAX31865 Software SPI");
  pinMode(sclkPin, OUTPUT);
  digitalWrite(sclkPin, LOW);
  pinMode(mosiPin, OUTPUT);
  pinMode(misoPin, INPUT);
}

#endif // HAS_MAX31865 && !USE_ADAFRUIT_MAX31865
