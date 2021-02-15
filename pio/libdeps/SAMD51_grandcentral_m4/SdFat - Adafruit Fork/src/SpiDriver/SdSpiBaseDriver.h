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
#ifndef SdSpiBaseDriver_h
#define SdSpiBaseDriver_h
/**
 * \class SdSpiBaseDriver
 * \brief SPI base driver.
 */
class SdSpiBaseDriver {
 public:
  /** Set SPI options for access to SD/SDHC cards.
   *
   */
  virtual void activate() = 0;
  /** Initialize the SPI bus.
   *
   * \param[in] chipSelectPin SD card chip select pin.
   */
  virtual void begin(uint8_t chipSelectPin) = 0;
  /**
   * End SPI transaction.
   */
  virtual void deactivate() = 0;
  /** Receive a byte.
   *
   * \return The byte.
   */
  virtual uint8_t receive() = 0;
  /** Receive multiple bytes.
   *
   * \param[out] buf Buffer to receive the data.
   * \param[in] n Number of bytes to receive.
   *
   * \return Zero for no error or nonzero error code.
   */
  virtual uint8_t receive(uint8_t* buf, size_t n) = 0;
  /** Send a byte.
   *
   * \param[in] data Byte to send
   */
  virtual void send(uint8_t data) = 0;
  /** Send multiple bytes.
  *
  * \param[in] buf Buffer for data to be sent.
  * \param[in] n Number of bytes to send.
  */
  virtual void send(const uint8_t* buf, size_t n) = 0;
  /** Set CS low. */
  virtual void select() = 0;
  /** Save SPI settings.
   * \param[in] spiSettings SPI speed, mode, and bit order.
   */
  virtual void setSpiSettings(SPISettings spiSettings) = 0;
  /** Set CS high. */
  virtual void unselect() = 0;
};
#endif  // SdSpiBaseDriver_h
