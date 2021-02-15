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
#if defined(ARDUINO_ARCH_SAMD)
#include "SdSpiDriver.h"
/** Use Adafruit_ZeroDMA library if nonzero */
#define USE_SAMD_DMA_RECV 0
#define USE_SAMD_DMA_SEND 0

#if USE_SAMD_DMA_RECV || USE_SAMD_DMA_SEND
#include <Adafruit_ZeroDMA.h>
#include "utility/dma.h"
// Three DMA channels are used. SPI DMA read requires two channels,
// one for the "dummy writes" to initiate SPI transfers, the other
// to read each incoming byte. Write requires only one channel, and
// there's probably ways to "recycle" the first of the read channels,
// but that'll take some work and is not done here, have plenty of
// channels to go around. Each channel uses one descriptor.
#define CHANNEL_READ_TX 0
#define CHANNEL_READ_RX 1
#define CHANNEL_WRITE   2
static Adafruit_ZeroDMA DMAchannel[3];
static DmacDescriptor  *channelDescriptor[3];
static volatile bool    dma_busy = false;
static uint8_t          dum = 0xFF; // Nonsense for SPI transfer init
//------------------------------------------------------------------------------
/** Transfer-complete callback for Adafruit_ZeroDMA library.
 *
 */
static void dma_callback(Adafruit_ZeroDMA *dma) {
  dma_busy = false;
}
#endif // end USE_SAMD_DMA
//------------------------------------------------------------------------------
/** Set SPI options for access to SD/SDHC cards.
 *
 * \param[in] divisor SCK clock divider relative to the APB1 or APB2 clock.
 */
void SdSpiAltDriver::activate() {
  m_spi->beginTransaction(m_spiSettings);
}
//------------------------------------------------------------------------------
/** Initialize the SPI bus.
 *
 * \param[in] chipSelectPin SD card chip select pin.
 */
void SdSpiAltDriver::begin(uint8_t csPin) {
  m_csPin = csPin;
  pinMode(m_csPin, OUTPUT);
  digitalWrite(m_csPin, HIGH);
  m_spi->begin();

#if USE_SAMD_DMA_RECV || USE_SAMD_DMA_SEND

  // First channel and descriptor are for SPI read "dummy writes" (TX)
  DMAchannel[CHANNEL_READ_TX].allocate(); // Allocate DMA channel
  channelDescriptor[CHANNEL_READ_TX] =
    DMAchannel[CHANNEL_READ_TX].addDescriptor(
    (void *)&dum,                     // Source address (nonsense)
    (void *)m_spi->getDataRegister(), // Dest
    0,                                // Count (0 for now, set later)
    DMA_BEAT_SIZE_BYTE,               // Bytes/hwords/words
    false,                            // Don't increment source address
    false);                           // Don't increment dest address
  DMAchannel[CHANNEL_READ_TX].setTrigger(m_spi->getDMAC_ID_TX());
  DMAchannel[CHANNEL_READ_TX].setAction(DMA_TRIGGER_ACTON_BEAT);
  DMAchannel[CHANNEL_READ_TX].setCallback(dma_callback);

  // Second channel and descriptor are for actual SPI read (RX)
  DMAchannel[CHANNEL_READ_RX].allocate(); // Allocate DMA channel
  channelDescriptor[CHANNEL_READ_RX] =
    DMAchannel[CHANNEL_READ_RX].addDescriptor(
    (void *)m_spi->getDataRegister(), // Source address
    NULL,                             // Dest address (NULL for now, set later)
    0,                                // Count (0 for now, set later)
    DMA_BEAT_SIZE_BYTE,               // Bytes/hwords/words
    false,                            // Don't increment source address
    true);                            // Increment dest address
  DMAchannel[CHANNEL_READ_RX].setTrigger(m_spi->getDMAC_ID_RX());
  DMAchannel[CHANNEL_READ_RX].setAction(DMA_TRIGGER_ACTON_BEAT);
  DMAchannel[CHANNEL_READ_RX].setCallback(dma_callback);

  // SPI write is its own channel and descriptor
  DMAchannel[CHANNEL_WRITE].allocate(); // Allocate DMA channel
  channelDescriptor[CHANNEL_WRITE] =
    DMAchannel[CHANNEL_WRITE].addDescriptor(
    NULL,                             // Src address (NULL for now, set later)
    (void *)m_spi->getDataRegister(), // Dest address
    0,                                // Count (0 for now, set later)
    DMA_BEAT_SIZE_BYTE,               // Bytes/hwords/words
    true,                             // Increment source address
    false);                           // Don't increment dest address
  DMAchannel[CHANNEL_WRITE].setTrigger(m_spi->getDMAC_ID_TX());
  DMAchannel[CHANNEL_WRITE].setAction(DMA_TRIGGER_ACTON_BEAT);
  DMAchannel[CHANNEL_WRITE].setCallback(dma_callback);
#endif // USE_SAMD_DMA
}
//------------------------------------------------------------------------------
/**
 * End SPI transaction.
 */
void SdSpiAltDriver::deactivate() {
  m_spi->endTransaction();
}
//------------------------------------------------------------------------------
/** Receive a byte.
 *
 * \return The byte.
 */
uint8_t SdSpiAltDriver::receive() {
  return m_spi->transfer(0XFF);
}
//------------------------------------------------------------------------------
/** Receive multiple bytes.
 *
 * \param[out] buf Buffer to receive the data.
 * \param[in] n Number of bytes to receive.
 *
 * \return Zero for no error or nonzero error code.
 */
uint8_t SdSpiAltDriver::receive(uint8_t* buf, size_t n) {
#if USE_SAMD_DMA_RECV
  // Configure the SPI read "dummy write" (TX) length (n)
  DMAchannel[CHANNEL_READ_TX].changeDescriptor(
    channelDescriptor[CHANNEL_READ_TX], NULL, NULL, n);

  // Configure the SPI read (RX) length (n) into buf
  DMAchannel[CHANNEL_READ_RX].changeDescriptor(
    channelDescriptor[CHANNEL_READ_RX], NULL, (void *)buf, n);

  dma_busy = true;
  DMAchannel[CHANNEL_READ_RX].startJob(); // Start the RX job BEFORE the
  DMAchannel[CHANNEL_READ_TX].startJob(); // TX job! That's the secret sauce.
  while(dma_busy);
#else // USE_SAMD_DMA_RECV
  while(n--) *buf++ = m_spi->transfer(0xFF);
#endif // USE_SAMD_DMA_RECV
  return 0;
}
//------------------------------------------------------------------------------
/** Send a byte.
 *
 * \param[in] b Byte to send
 */
void SdSpiAltDriver::send(uint8_t b) {
  m_spi->transfer(b);
}
//------------------------------------------------------------------------------
/** Send multiple bytes.
 *
 * \param[in] buf Buffer for data to be sent.
 * \param[in] n Number of bytes to send.
 */
void SdSpiAltDriver::send(const uint8_t* buf , size_t n) {
#if USE_SAMD_DMA_SEND
  DMAchannel[CHANNEL_WRITE].changeDescriptor(
    channelDescriptor[CHANNEL_WRITE], (void *)buf, NULL, n);
  dma_busy = true;
  DMAchannel[CHANNEL_WRITE].startJob();
  while(dma_busy);
#else // USE_SAMD_DMA_SEND
  while(n--) m_spi->transfer(*buf++);
#endif // USE_SAMD_DMA_SEND
}
#endif  // defined(ARDUINO_ARCH_SAMD)
