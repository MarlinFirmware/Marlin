/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
 /**
 * \file
 * \brief Minimal AVR Serial driver.
 */
#ifndef MinimumSerial_h
#define MinimumSerial_h
#include "SysCall.h"
//==============================================================================
/**
 * \class MinimumSerial
 * \brief mini serial class for the %SdFat library.
 */
class MinimumSerial : public Print {
 public:
  /** \return true for hardware serial */
  operator bool() { return true; }
  /**
   * \return one if data is available.
   */
  int available();
  /**
   * Set baud rate for serial port zero and enable in non interrupt mode.
   * Do not call this function if you use another serial library.
   * \param[in] baud rate
   */
  void begin(uint32_t baud);
  /** Wait for write done. */
  void flush();
  /**
   *  Unbuffered read
   *  \return -1 if no character is available or an available character.
   */
  int read();
  /**
   * Unbuffered write
   *
   * \param[in] b byte to write.
   * \return 1
   */
  size_t write(uint8_t b);
  using Print::write;
};
#endif  // MinimumSerial_h
