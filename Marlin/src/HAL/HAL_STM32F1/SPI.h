/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/
#pragma once

#include <libmaple/libmaple_types.h>
#include <libmaple/spi.h>
#include <libmaple/dma.h>

#include <boards.h>
#include <stdint.h>
#include <wirish.h>

// SPI_HAS_TRANSACTION means SPI has
//   - beginTransaction()
//   - endTransaction()
//   - usingInterrupt()
//   - SPISetting(clock, bitOrder, dataMode)
//#define SPI_HAS_TRANSACTION

#define SPI_CLOCK_DIV2   SPI_BAUD_PCLK_DIV_2
#define SPI_CLOCK_DIV4   SPI_BAUD_PCLK_DIV_4
#define SPI_CLOCK_DIV8   SPI_BAUD_PCLK_DIV_8
#define SPI_CLOCK_DIV16  SPI_BAUD_PCLK_DIV_16
#define SPI_CLOCK_DIV32  SPI_BAUD_PCLK_DIV_32
#define SPI_CLOCK_DIV64  SPI_BAUD_PCLK_DIV_64
#define SPI_CLOCK_DIV128 SPI_BAUD_PCLK_DIV_128
#define SPI_CLOCK_DIV256 SPI_BAUD_PCLK_DIV_256

/*
 * Roger Clark. 20150106
 * Commented out redundant AVR defined
 *
#define SPI_MODE_MASK 0x0C     // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03    // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR

// define SPI_AVR_EIMSK for AVR boards with external interrupt pins
#if defined(EIMSK)
  #define SPI_AVR_EIMSK EIMSK
#elif defined(GICR)
  #define SPI_AVR_EIMSK GICR
#elif defined(GIMSK)
  #define SPI_AVR_EIMSK GIMSK
#endif
*/

#ifndef STM32_LSBFIRST
  #define STM32_LSBFIRST 0
#endif
#ifndef STM32_MSBFIRST
  #define STM32_MSBFIRST 1
#endif

// PC13 or PA4
#define BOARD_SPI_DEFAULT_SS PA4
//#define BOARD_SPI_DEFAULT_SS PC13

#define SPI_MODE0 SPI_MODE_0
#define SPI_MODE1 SPI_MODE_1
#define SPI_MODE2 SPI_MODE_2
#define SPI_MODE3 SPI_MODE_3

#define DATA_SIZE_8BIT SPI_CR1_DFF_8_BIT
#define DATA_SIZE_16BIT SPI_CR1_DFF_16_BIT

typedef enum {
  SPI_STATE_IDLE,
  SPI_STATE_READY,
  SPI_STATE_RECEIVE,
  SPI_STATE_TRANSMIT,
  SPI_STATE_TRANSFER
} spi_mode_t;

class SPISettings {
public:
  SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) {
    if (__builtin_constant_p(clock))
      init_AlwaysInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
    else
      init_MightInline(clock, bitOrder, dataMode, DATA_SIZE_8BIT);
  }
  SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) {
    if (__builtin_constant_p(clock))
      init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
    else
      init_MightInline(clock, bitOrder, dataMode, dataSize);
  }
  SPISettings(uint32_t clock) {
    if (__builtin_constant_p(clock))
      init_AlwaysInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
    else
      init_MightInline(clock, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
  }
  SPISettings() {
    init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT);
  }
private:
  void init_MightInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) {
    init_AlwaysInline(clock, bitOrder, dataMode, dataSize);
  }
  void init_AlwaysInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode, uint32_t dataSize) __attribute__((__always_inline__)) {
    this->clock = clock;
    this->bitOrder = bitOrder;
    this->dataMode = dataMode;
    this->dataSize = dataSize;
  }
  uint32_t clock;
  uint32_t dataSize;
  uint32_t clockDivider;
  BitOrder bitOrder;
  uint8_t dataMode;
  uint8_t _SSPin;
  volatile spi_mode_t state;
  spi_dev *spi_d;
  dma_channel spiRxDmaChannel, spiTxDmaChannel;
  dma_dev* spiDmaDev;
  void (*receiveCallback)(void) = NULL;
  void (*transmitCallback)(void) = NULL;

  friend class SPIClass;
};

/*
 * Kept for compat.
 */
static const uint8_t ff = 0xFF;

/**
 * @brief Wirish SPI interface.
 *
 * This implementation uses software slave management, so the caller
 * is responsible for controlling the slave select line.
 */
class SPIClass {

public:
  /**
   * @param spiPortNumber Number of the SPI port to manage.
   */
  SPIClass(uint32_t spiPortNumber);

  /**
   * @brief Equivalent to begin(SPI_1_125MHZ, MSBFIRST, 0).
   */
  void begin();

  /**
   * @brief Turn on a SPI port and set its GPIO pin modes for use as a slave.
   *
   * SPI port is enabled in full duplex mode, with software slave management.
   *
   * @param bitOrder Either LSBFIRST (little-endian) or MSBFIRST(big-endian)
   * @param mode SPI mode to use
   */
  void beginSlave(uint32_t bitOrder, uint32_t mode);

  /**
   * @brief Equivalent to beginSlave(MSBFIRST, 0).
   */
  void beginSlave();

  /**
   * @brief Disables the SPI port, but leaves its GPIO pin modes unchanged.
   */
  void end();

  void beginTransaction(SPISettings settings) { beginTransaction(BOARD_SPI_DEFAULT_SS, settings); }
  void beginTransaction(uint8_t pin, SPISettings settings);
  void endTransaction();

  void beginTransactionSlave(SPISettings settings);

  void setClockDivider(uint32_t clockDivider);
  void setBitOrder(BitOrder bitOrder);
  void setDataMode(uint8_t dataMode);

  // SPI Configuration methods
  void attachInterrupt();
  void detachInterrupt();

  /* Victor Perez. Added to change datasize from 8 to 16 bit modes on the fly.
   * Input parameter should be SPI_CR1_DFF set to 0 or 1 on a 32bit word.
   * Requires an added function spi_data_size on STM32F1 / cores / maple / libmaple / spi.c
   */
  void setDataSize(uint32_t ds);

  /* Victor Perez 2017. Added to set and clear callback functions for callback
   * on DMA transfer completion.
   * onReceive used to set the callback in case of dmaTransfer (tx/rx), once rx is completed
   * onTransmit used to set the callback in case of dmaSend (tx only). That function
   * will NOT be called in case of TX/RX
   */
  void onReceive(void(*)(void));
  void onTransmit(void(*)(void));

  /*
   * I/O
   */

  /**
   * @brief Return the next unread byte/word.
   *
   * If there is no unread byte/word waiting, this function will block
   * until one is received.
   */
  uint16_t read();

  /**
   * @brief Read length bytes, storing them into buffer.
   * @param buffer Buffer to store received bytes into.
   * @param length Number of bytes to store in buffer. This
   *               function will block until the desired number of
   *               bytes have been read.
   */
  void read(uint8_t *buffer, uint32_t length);

  /**
   * @brief Transmit one byte/word.
   * @param data to transmit.
   */
  void write(uint16_t data);
  void write16(uint16_t data); // write 2 bytes in 8 bit mode (DFF=0)

  /**
   * @brief Transmit one byte/word a specified number of times.
   * @param data to transmit.
   */
  void write(uint16_t data, uint32_t n);

  /**
   * @brief Transmit multiple bytes/words.
   * @param buffer Bytes/words to transmit.
   * @param length Number of bytes/words in buffer to transmit.
   */
  void write(const void * buffer, uint32_t length);

  /**
   * @brief Transmit a byte, then return the next unread byte.
   *
   * This function transmits before receiving.
   *
   * @param data Byte to transmit.
   * @return Next unread byte.
   */
  uint8_t transfer(uint8_t data) const;
  uint16_t transfer16(uint16_t data) const;

  /**
   * @brief Sets up a DMA Transfer for "length" bytes.
   * The transfer mode (8 or 16 bit mode) is evaluated from the SPI peripheral setting.
   *
   * This function transmits and receives to buffers.
   *
   * @param transmitBuf buffer Bytes to transmit. If passed as 0, it sends FF repeatedly for "length" bytes
   * @param receiveBuf buffer Bytes to save received data.
   * @param length Number of bytes in buffer to transmit.
   */
  uint8_t dmaTransfer(const void * transmitBuf, void * receiveBuf, uint16_t length);
  void dmaTransferSet(const void *transmitBuf, void *receiveBuf);
  uint8_t dmaTransferRepeat(uint16_t length);

  /**
   * @brief Sets up a DMA Transmit for SPI 8 or 16 bit transfer mode.
   * The transfer mode (8 or 16 bit mode) is evaluated from the SPI peripheral setting.
   *
   * This function only transmits and does not care about the RX fifo.
   *
   * @param data buffer half words to transmit,
   * @param length Number of bytes in buffer to transmit.
   * @param minc Set to use Memory Increment mode, clear to use Circular mode.
   */
  uint8_t dmaSend(const void * transmitBuf, uint16_t length, bool minc = 1);
  void dmaSendSet(const void * transmitBuf, bool minc);
  uint8_t dmaSendRepeat(uint16_t length);

  uint8_t dmaSendAsync(const void * transmitBuf, uint16_t length, bool minc = 1);
  /*
   * Pin accessors
   */

  /**
   * @brief Return the number of the MISO (master in, slave out) pin
   */
  uint8_t misoPin();

  /**
   * @brief Return the number of the MOSI (master out, slave in) pin
   */
  uint8_t mosiPin();

  /**
   * @brief Return the number of the SCK (serial clock) pin
   */
  uint8_t sckPin();

  /**
   * @brief Return the number of the NSS (slave select) pin
   */
  uint8_t nssPin();

  /* Escape hatch */

  /**
   * @brief Get a pointer to the underlying libmaple spi_dev for
   *        this HardwareSPI instance.
   */
  spi_dev* c_dev(void) { return _currentSetting->spi_d; }

  spi_dev* dev() { return _currentSetting->spi_d; }

  /**
   * @brief Sets the number of the SPI peripheral to be used by
   *        this HardwareSPI instance.
   *
   * @param spi_num Number of the SPI port. 1-2 in low density devices
   *     or 1-3 in high density devices.
   */
  void setModule(int spi_num) {
    _currentSetting=&_settings[spi_num-1];// SPI channels are called 1 2 and 3 but the array is zero indexed
  }

  /* -- The following methods are deprecated --------------------------- */

  /**
   * @brief Deprecated.
   *
   * Use HardwareSPI::transfer() instead.
   *
   * @see HardwareSPI::transfer()
   */
  uint8_t send(uint8_t data);

  /**
   * @brief Deprecated.
   *
   * Use HardwareSPI::write() in combination with
   * HardwareSPI::read() (or HardwareSPI::transfer()) instead.
   *
   * @see HardwareSPI::write()
   * @see HardwareSPI::read()
   * @see HardwareSPI::transfer()
   */
  uint8_t send(uint8_t *data, uint32_t length);

  /**
   * @brief Deprecated.
   *
   * Use HardwareSPI::read() instead.
   *
   * @see HardwareSPI::read()
   */
  uint8_t recv();

private:

  SPISettings _settings[BOARD_NR_SPI];
  SPISettings *_currentSetting;

  void updateSettings();

  /*
   * Functions added for DMA transfers with Callback.
   * Experimental.
   */

  void EventCallback();

  #if BOARD_NR_SPI >= 1
    static void _spi1EventCallback();
  #endif
  #if BOARD_NR_SPI >= 2
    static void _spi2EventCallback();
  #endif
  #if BOARD_NR_SPI >= 3
    static void _spi3EventCallback();
  #endif
  /*
  spi_dev *spi_d;
  uint8_t _SSPin;
  uint32_t clockDivider;
  uint8_t dataMode;
  BitOrder bitOrder;
  */
};

extern SPIClass SPI;
