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

// Useful for RTD debugging.
//#define MAX31865_DEBUG
//#define MAX31865_DEBUG_SPI

//TODO: switch to SPIclass/SoftSPI

#include "../inc/MarlinConfig.h"

#if HAS_MAX31865 && !LIB_USR_MAX31865

#include "MAX31865.h"

// The maximum speed the MAX31865 can do is 5 MHz
SPISettings MAX31865::spiConfig = SPISettings(
  #if defined(TARGET_LPC1768)
    SPI_QUARTER_SPEED
  #elif defined(ARDUINO_ARCH_STM32)
    SPI_CLOCK_DIV4
  #else
    500000
  #endif
  , MSBFIRST
  , SPI_MODE_1 // CPOL0 CPHA1
);

#ifndef LARGE_PINMAP

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
    _cs = spi_cs;
    _mosi = spi_mosi;
    _miso = spi_miso;
    _sclk = spi_clk;
  }

  /**
   * Create the interface object using hardware SPI for PIN for PIN values less
   * than or equal to 127.
   *
   * @param spi_cs  the SPI CS pin to use along with the default SPI device
   */
  MAX31865::MAX31865(int8_t spi_cs) {
    _cs = spi_cs;
    _sclk = _miso = _mosi = -1;
  }

#else

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
  MAX31865::MAX31865(uint32_t spi_cs, uint32_t spi_mosi,
                     uint32_t spi_miso, uint32_t spi_clk,
                     uint8_t pin_mapping) {
    _cs = spi_cs;
    _mosi = spi_mosi;
    _miso = spi_miso;
    _sclk = spi_clk;
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
    _cs = spi_cs;
    _sclk = _miso = _mosi = -1UL;  //-1UL or 0xFFFFFFFF or 4294967295
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
 */
void MAX31865::begin(max31865_numwires_t wires, float zero, float ref) {
  Rzero = zero;
  Rref = ref;

  OUT_WRITE(_cs, HIGH);

  if (_sclk != TERN(LARGE_PINMAP, -1UL, -1)) {
    // define pin modes for Software SPI
    #ifdef MAX31865_DEBUG
      SERIAL_ECHOLN("Initializing MAX31865 Software SPI");
    #endif

    OUT_WRITE(_sclk, LOW);
    SET_OUTPUT(_mosi);
    SET_INPUT(_miso);
  } else {
    // start and configure hardware SPI
    #ifdef MAX31865_DEBUG
      SERIAL_ECHOLN("Initializing MAX31865 Hardware SPI");
    #endif

    SPI.begin();
  }

  setWires(wires);
  enableBias(false);
  autoConvert(false);
  clearFault();

  #ifdef MAX31865_DEBUG_SPI
    #ifndef LARGE_PINMAP
      SERIAL_ECHOLNPAIR(
        "Regular begin call with _cs: ", _cs,
        " _miso: ", _miso,
        " _sclk: ", _sclk,
        " _mosi: ", _mosi
      );
    #else
      SERIAL_ECHOLNPAIR(
        "LARGE_PINMAP begin call with _cs: ", _cs,
        " _miso: ", _miso,
        " _sclk: ", _sclk,
        " _mosi: ", _mosi
      );
    #endif // LARGE_PINMAP

    SERIAL_ECHOLNPAIR("config: ", readRegister8(MAX31856_CONFIG_REG));
    SERIAL_EOL();
  #endif // MAX31865_DEBUG_SPI
}

/**
 * Read the raw 8-bit FAULTSTAT register
 *
 * @return The raw unsigned 8-bit FAULT status register
 */
uint8_t MAX31865::readFault() {
  return readRegister8(MAX31856_FAULTSTAT_REG);
}

/**
 * Clear all faults in FAULTSTAT.
 */
void MAX31865::clearFault() {
  setConfig(MAX31856_CONFIG_FAULTSTAT, 1);
}

/**
 * Whether we want to have continuous conversions (50/60 Hz)
 *
 * @param b  If true, auto conversion is enabled
 */
void MAX31865::autoConvert(bool b) {
  setConfig(MAX31856_CONFIG_MODEAUTO, b);
}

/**
 * Whether we want filter out 50Hz noise or 60Hz noise
 *
 * @param b  If true, 50Hz noise is filtered, else 60Hz(default)
 */
void MAX31865::enable50HzFilter(bool b) {
  setConfig(MAX31856_CONFIG_FILT50HZ, b);
}

/**
 * Enable the bias voltage on the RTD sensor
 *
 * @param b  If true bias is enabled, else disabled
 */
void MAX31865::enableBias(bool b) {
  setConfig(MAX31856_CONFIG_BIAS, b);

  // From the datasheet:
  // Note that if VBIAS is off (to reduce supply current between conversions), any filter
  // capacitors at the RTDIN inputs need to charge before an accurate conversion can be
  // performed. Therefore, enable VBIAS and wait at least 10.5 time constants of the input
  // RC network plus an additional 1ms before initiating the conversion.
  if (b)
    DELAY_US(11500); //11.5ms
}

/**
 * Start a one-shot temperature reading.
 */
void MAX31865::oneShot() {
  setConfig(MAX31856_CONFIG_1SHOT, 1);

  // From the datasheet:
  // Note that a single conversion requires approximately 52ms in 60Hz filter
  // mode or 62.5ms in 50Hz filter mode to complete. 1-Shot is a self-clearing bit.
  // TODO: switch this out depending on the filter mode.
  DELAY_US(65000); // 65ms
}

/**
 * How many wires we have in our RTD setup, can be MAX31865_2WIRE,
 * MAX31865_3WIRE, or MAX31865_4WIRE
 *
 * @param wires The number of wires in enum format
 */
void MAX31865::setWires(max31865_numwires_t wires) {
  uint8_t t = readRegister8(MAX31856_CONFIG_REG);
  if (wires == MAX31865_3WIRE)
    t |= MAX31856_CONFIG_3WIRE;
  else // 2 or 4 wire
    t &= ~MAX31856_CONFIG_3WIRE;
  writeRegister8(MAX31856_CONFIG_REG, t);
}

/**
 * Read the raw 16-bit value from the RTD_REG in one shot mode. This will include
 * the fault bit, D0.
 *
 * @return The raw unsigned 16-bit register value with ERROR bit attached, NOT temperature!
 */
uint16_t MAX31865::readRaw() {
  clearFault();
  enableBias(true);

  oneShot();
  uint16_t rtd = readRegister16(MAX31856_RTDMSB_REG);

  #ifdef MAX31865_DEBUG
    SERIAL_ECHOLNPAIR("RTD MSB:", (rtd >> 8), "  RTD LSB:", (rtd & 0x00FF));
  #endif

  // Disable the bias to lower power dissipation between reads.
  // If the ref resistor heats up, the temperature reading will be skewed.
  enableBias(false);

  return rtd;
}

/**
 * Calculate and return the resistance value of the connected RTD.
 *
 * @param  refResistor The value of the matching reference resistor, usually 430 or 4300
 * @return             The raw RTD resistance value, NOT temperature!
 */
float MAX31865::readResistance() {
  // Strip the error bit (D0) and convert to a float ratio.
  // less precise method: (readRaw() * Rref) >> 16
  return (((readRaw() >> 1) / 32768.0f) * Rref);
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
float MAX31865::temperature(uint16_t adcVal) {
  return temperature(((adcVal) / 32768.0f) * Rref);
}

/**
 * Calculate the temperature in C from the RTD resistance.
 * Uses the technique outlined in this PDF:
 * http://www.analog.com/media/en/technical-documentation/application-notes/AN709_0.pdf
 *
 * @param    Rrtd  the resistance value in ohms
 * @return         the temperature in degC
 */
float MAX31865::temperature(float Rrtd) {
  float temp = (RTD_Z1 + sqrt(RTD_Z2 + (RTD_Z3 * Rrtd))) / RTD_Z4;

  // From the PDF...
  //
  // The previous equation is valid only for temperatures of 0°C and above.
  // The equation for RRTD(t) that defines negative temperature behavior is a
  // fourth-order polynomial (after expanding the third term) and is quite
  // impractical to solve for a single expression of temperature as a function
  // of resistance.
  //
  if (temp < 0) {
    Rrtd = (Rrtd / Rzero) * 100; // normalize to 100 ohm
    float rpoly = Rrtd;

    temp = -242.02 + (2.2228 * rpoly);
    rpoly *= Rrtd; // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rrtd; // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rrtd; // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rrtd; // ^5
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
  uint8_t t = readRegister8(MAX31856_CONFIG_REG);
  if (enable)
    t |= config;
  else
    t &= ~config; // disable
  writeRegister8(MAX31856_CONFIG_REG, t);
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
  uint8_t buffer[2] = {0, 0};
  readRegisterN(addr, buffer, 2);

  uint16_t ret = buffer[0];
  ret <<= 8;
  ret |= buffer[1];

  return ret;
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
  if (_sclk == TERN(LARGE_PINMAP, -1UL, -1))
    SPI.beginTransaction(spiConfig);
  else
    WRITE(_sclk, LOW);

  WRITE(_cs, LOW);
  spixfer(addr);

  while (n--) {
    buffer[0] = spixfer(0xFF);
    #ifdef MAX31865_DEBUG_SPI
      SERIAL_ECHOLNPAIR("buffer read ", n, " data: ", buffer[0]);
    #endif
    buffer++;
  }

  if (_sclk == TERN(LARGE_PINMAP, -1UL, -1))
    SPI.endTransaction();

  WRITE(_cs, HIGH);
}

/**
 * Write an 8-bit value to a register. Set D7 to 1 to specify a write.
 *
 * @param addr  the address to write to
 * @param data  the data to write
 */
void MAX31865::writeRegister8(uint8_t addr, uint8_t data) {
  if (_sclk == TERN(LARGE_PINMAP, -1UL, -1))
    SPI.beginTransaction(spiConfig);
  else
    WRITE(_sclk, LOW);

  WRITE(_cs, LOW);

  spixfer(addr | 0x80); // make sure top bit is set
  spixfer(data);

  if (_sclk == TERN(LARGE_PINMAP, -1UL, -1))
    SPI.endTransaction();

  WRITE(_cs, HIGH);
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
uint8_t MAX31865::spixfer(uint8_t x) {
  if (_sclk == TERN(LARGE_PINMAP, -1UL, -1))
    return SPI.transfer(x);

  uint8_t reply = 0;
  for (int i = 7; i >= 0; i--) {
    reply <<= 1;
    WRITE(_sclk, HIGH);
    WRITE(_mosi, x & (1 << i));
    WRITE(_sclk, LOW);
    if (READ(_miso))
      reply |= 1;
  }

  return reply;
}

#endif // HAS_MAX31865 && !LIB_USR_MAX31865
