
#ifndef _SoftWire_h
#define _SoftWire_h
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the MIT License.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#warning "Using custom Wire.h"

#include <Arduino.h>
#include <inttypes.h>
#include "Stream.h"

#ifndef I2C_BUFFER_LENGTH
  #define I2C_BUFFER_LENGTH 32
#endif

#define I2C_READ 1
#define I2C_WRITE 0
#define I2C_MAXWAIT 5000

class SoftI2C : public Stream
{
private:
  bool i2cInit(void);
  bool i2cStart(uint8_t addr);
  bool i2cStartWait(uint8_t addr);
  bool i2cRepStart(uint8_t addr);
  void i2cStop(void);
  bool i2cWrite(uint8_t value);
  uint8_t i2cRead(bool last);

  uint8_t rxBuffer[I2C_BUFFER_LENGTH];
  uint8_t rxBufferIndex;
  uint8_t rxBufferLength;
  uint8_t isTransmitting;
  uint8_t error;

  inline void sdaLow();
  inline void sdaHigh();
  inline void sclLow();
  inline void sclHigh();
  inline uint8_t readSda();
  inline uint8_t readScl();

  GPIO_TypeDef *PORT_SDA;
  GPIO_TypeDef *PORT_SCL;
  uint32_t SDA_PIN_MASK;
  uint32_t SCL_PIN_MASK;
  uint32_t SDA_MODDER_OUTPUT;
  uint32_t SDA_MODDER_INPUT;
  uint32_t SCL_MODDER_OUTPUT;
  uint32_t SCL_MODDER_INPUT;
  uint32_t SCL_BSRR_SET_LOW;
  uint32_t SDA_BSRR_SET_LOW;
  uint32_t SCL_PUPDR_PULLUP;
  uint32_t SDA_PUPDR_PULLUP;
  
  void pinsSetup(uint8_t dataPin, uint8_t clockPin);
public:

  SoftI2C(uint8_t dataPin, uint8_t clockPin);

  void begin(void);
  void begin(uint8_t dataPin, uint8_t clockPin);
  void end(void);

  void beginTransmission(uint8_t address);
  void beginTransmission(int address);
  uint8_t endTransmission(uint8_t sendStop);
  uint8_t endTransmission(void);

  size_t write(uint8_t data);
  size_t write(const uint8_t *data, size_t quantity);

  uint8_t requestFrom(uint8_t address, uint8_t quantity,
              uint32_t iaddress, uint8_t isize, uint8_t sendStop);
  uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
  uint8_t requestFrom(int address, int quantity, int sendStop);
  uint8_t requestFrom(uint8_t address, uint8_t quantity);
  uint8_t requestFrom(int address, int quantity);
  void setClock(uint32_t clock){};
  int available(void);
  int read(void);
  int peek(void);
  void flush(void);

  inline size_t write(unsigned long n) { return write((uint8_t)n); }
  inline size_t write(long n) { return write((uint8_t)n); }
  inline size_t write(unsigned int n) { return write((uint8_t)n); }
  inline size_t write(int n) { return write((uint8_t)n); }
};
#endif 
#pragma once

extern SoftI2C Wire;

