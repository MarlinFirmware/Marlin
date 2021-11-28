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

#include "../inc/MarlinConfig.h"

#if HAS_MAX31865 && !USE_ADAFRUIT_MAX31865

#include "MAX31865.h"

#ifndef MAX31865_MIN_SAMPLING_TIME_MSEC
  #define MAX31865_MIN_SAMPLING_TIME_MSEC 0
#endif

#ifdef TARGET_LPC1768
  #include <SoftwareSPI.h>
#endif

// The maximum speed the MAX31865 can do is 5 MHz
SPISettings MAX31865::spiConfig = SPISettings(
  TERN(TARGET_LPC1768, SPI_QUARTER_SPEED, TERN(ARDUINO_ARCH_STM32, SPI_CLOCK_DIV4, 500000)),
  MSBFIRST,
  SPI_MODE1 // CPOL0 CPHA1
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
 * @param wireRes The resistance of the wire connecting the sensor to the RTD, in ohms.
 */
void MAX31865::begin(max31865_numwires_t wires, float zero, float ref, float wireRes) {
  Rzero = zero;
  Rref = ref;
  RwireRes = wireRes;

  if (_sclk != TERN(LARGE_PINMAP, -1UL, 255)) {
    OUT_WRITE(_cs, HIGH);

    softSpiBegin(SPI_QUARTER_SPEED); // Define pin modes for Software SPI
  }
  else {
    #ifdef MAX31865_DEBUG
      SERIAL_ECHOLNPGM("Initializing MAX31865 Hardware SPI");
    #endif
    
    digitalWrite(_cs, HIGH);
    SPI.begin();    // Start and configure hardware SPI
  }

  initFixedFlags(wires);
  
  clearFault(); // resets fault and initializes flags

  #ifndef MAX31865_USE_AUTO_MODE // make a proper first 1 shot read to initialize _lastRead

    enableBias();
    DELAY_US(11500);
    oneShot();
    DELAY_US(65000);
    uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG);

    if (rtd & 1) { 
      _lastRead = 0xFFFF; // some invalid value
      _lastFault = readRegister8(MAX31865_FAULTSTAT_REG); // keep the fault in a variable and reset flag
      clearFault(); // also clears the bias voltage flag, so no further action is required

    #ifdef MAX31865_DEBUG
      SERIAL_ECHOLNPGM("MAX31865 read fault: ", rtd);
    #endif
    }
    else {
    #ifdef MAX31865_DEBUG
      SERIAL_ECHOLNPGM("RTD MSB:", (rtd >> 8), "  RTD LSB:", (rtd & 0x00FF));
    #endif
      resetFlags();
      _lastRead = rtd;
      _lastReadStamp = millis();
    }

  #endif

  #ifdef MAX31865_DEBUG_SPI
    SERIAL_ECHOLNPGM(
      TERN(LARGE_PINMAP, "LARGE_PINMAP", "Regular")
      " begin call with _cs: ", _cs,
      " _miso: ", _miso,
      " _sclk: ", _sclk,
      " _mosi: ", _mosi,
      " config: ", readRegister8(MAX31865_CONFIG_REG)
    );
  #endif
}

/**
 * Return and clear the last fault value
 *
 * @return The raw unsigned 8-bit FAULT status register or spike fault
 */
uint8_t MAX31865::readFault() {
  uint8_t r = _lastFault;
  _lastFault = 0;
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
  writeRegister8(MAX31865_CONFIG_REG, _stdFlags);
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
  _stdFlags = TERN(MAX31865_50HZ_FILTER, MAX31865_CONFIG_FILT50HZ, MAX31865_CONFIG_FILT60HZ) | 
              TERN(MAX31865_USE_AUTO_MODE, MAX31865_CONFIG_MODEAUTO | MAX31865_CONFIG_BIAS, MAX31865_CONFIG_MODEOFF);
  
  if (wires == MAX31865_3WIRE)
    _stdFlags |= MAX31865_CONFIG_3WIRE;
  else // 2 or 4 wire
    _stdFlags &= ~MAX31865_CONFIG_3WIRE;
}

/**
 * Read the raw 16-bit value from the RTD_REG in one shot mode. This will include
 * the fault bit, D0.
 *
 * @return The raw unsigned 16-bit register value with ERROR bit attached, NOT temperature!
 */
uint16_t MAX31865::readRaw() {

  uint16_t rtd = _lastRead;

 #ifndef MAX31865_USE_AUTO_MODE

  switch (_lastStep) {
  case 0: // first step sets up bias voltage
    if (millis() - _lastStamp < MAX31865_MIN_SAMPLING_TIME_MSEC) // first step waits for at least MIN_SAMPLING msec before enabling bias
      return _lastRead;

    enableBias();

    _lastStamp = millis();
    _lastStep = 1;
    break;
  
  case 1: // second step waits for at least 11msec before enabling 1shot
    if (millis() - _lastStamp < 11)
      return _lastRead;

    oneShot();

    _lastStamp = millis();
    _lastStep = 2;
    break;

  case 2: // second step waits for at least 65msec before making a read
    if (millis() - _lastStamp < 65)
      return _lastRead;
#endif

    rtd = readRegister16(MAX31865_RTDMSB_REG);

    if (rtd & 1) { 
      _lastFault = readRegister8(MAX31865_FAULTSTAT_REG); // keep the fault in a variable and reset flag
      _lastRead |= 1;
      clearFault(); // also clears the bias voltage flag, so no further action is required

    #ifdef MAX31865_DEBUG
      SERIAL_ECHOLNPGM("MAX31865 read fault: ", rtd);
    #endif
    } 
  #ifdef MAX31865_USE_READ_ERROR_DETECTION
    else if (abs(_lastRead - rtd) > 500 && millis() - _lastReadStamp < 1000) { // if two readings within a second differ too much (~20°C), consider it a read error.
      _lastFault = 0x01;
      _lastRead |= 1; // make it an error

    #ifdef MAX31865_DEBUG
      SERIAL_ECHOLNPGM("MAX31865 read error: ", rtd);
    #endif
    }
  #endif    
    else {
      _lastRead = rtd;
    #ifdef MAX31865_USE_READ_ERROR_DETECTION
      _lastReadStamp = millis();
    #endif
    }

#ifndef MAX31865_USE_AUTO_MODE
    _lastStep = 0;
    break;
  }
#endif

#ifdef MAX31865_DEBUG
  #ifndef MAX31865_USE_AUTO_MODE
    if (_lastStep == 0) {
  #endif
      SERIAL_ECHOLNPGM("RTD MSB:", (rtd >> 8), "  RTD LSB:", (rtd & 0x00FF));
  #ifndef MAX31865_USE_AUTO_MODE
    }
  #endif

#endif

  return _lastRead;
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
  return ((readRaw() / 65536.0f) * Rref - RwireRes);
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
  return temperature(((adcVal) / 32768.0f) * Rref - RwireRes);
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
  uint8_t t = _stdFlags;
  if (enable)
    t |= config;
  else
    t &= ~config; // disable
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
  if (_sclk == TERN(LARGE_PINMAP, -1UL, 255)) {
    SPI.beginTransaction(spiConfig);
    digitalWrite(_cs, LOW);
  }
  else {
    WRITE(_sclk, LOW);
    WRITE(_cs, LOW);
  }

  #ifdef TARGET_LPC1768
    DELAY_CYCLES(_spi_speed);
  #endif

  spiTransfer(addr);

  while (n--) {
    buffer[0] = spiTransfer(0xFF);
    #ifdef MAX31865_DEBUG_SPI
      SERIAL_ECHOLNPGM("buffer read ", n, " data: ", buffer[0]);
    #endif
    buffer++;
  }

  if (_sclk == TERN(LARGE_PINMAP, -1UL, 255)) {
    SPI.endTransaction();
    digitalWrite(_cs, HIGH);  
  }
  else {
    WRITE(_cs, HIGH);
  }
}

/**
 * Write an 8-bit value to a register. Set D7 to 1 to specify a write.
 *
 * @param addr  the address to write to
 * @param data  the data to write
 */
void MAX31865::writeRegister8(uint8_t addr, uint8_t data) {
  if (_sclk == TERN(LARGE_PINMAP, -1UL, 255)) {
    SPI.beginTransaction(spiConfig);
    digitalWrite(_cs, LOW);
  }
  else {
    WRITE(_sclk, LOW);
    WRITE(_cs, LOW);
  }

  #ifdef TARGET_LPC1768
    DELAY_CYCLES(_spi_speed);
  #endif

  spiTransfer(addr | 0x80); // make sure top bit is set
  spiTransfer(data);

  if (_sclk == TERN(LARGE_PINMAP, -1UL, 255)) {
    SPI.endTransaction();
    digitalWrite(_cs, HIGH);  
  }
  else {
    WRITE(_cs, HIGH);
  }
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
  if (_sclk == TERN(LARGE_PINMAP, -1UL, 255))
    return SPI.transfer(x);

  #ifdef TARGET_LPC1768
    return swSpiTransfer(x, _spi_speed, _sclk, _miso, _mosi);
  #else
    uint8_t reply = 0;
    for (int i = 7; i >= 0; i--) {
      WRITE(_sclk, HIGH);           DELAY_NS_VAR(_spi_delay);
      reply <<= 1;
      WRITE(_mosi, x & _BV(i));     DELAY_NS_VAR(_spi_delay);
      if (READ(_miso)) reply |= 1;
      WRITE(_sclk, LOW);            DELAY_NS_VAR(_spi_delay);
    }
    return reply;
  #endif
}

void MAX31865::softSpiBegin(const uint8_t spi_speed) {
  #ifdef MAX31865_DEBUG
    SERIAL_ECHOLNPGM("Initializing MAX31865 Software SPI");
  #endif
  #ifdef TARGET_LPC1768
    swSpiBegin(_sclk, _miso, _mosi);
    _spi_speed = swSpiInit(spi_speed, _sclk, _mosi);
  #else
    _spi_delay = (100UL << spi_speed) / 3; // Calculate delay in ns. Top speed is ~10MHz, or 100ns delay between bits.
    OUT_WRITE(_sclk, LOW);
    SET_OUTPUT(_mosi);
    SET_INPUT(_miso);
  #endif
}

#endif // HAS_MAX31865 && !USE_ADAFRUIT_MAX31865
