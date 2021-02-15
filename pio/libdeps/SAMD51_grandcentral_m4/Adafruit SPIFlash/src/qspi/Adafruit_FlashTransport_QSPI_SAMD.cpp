/**
 * @file Adafruit_QSPI.cpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach and Dean Miller for Adafruit Industries LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifdef __SAMD51__

#include "Adafruit_FlashTransport.h"
#include "wiring_private.h"
#include <Arduino.h>

static void _run_instruction(uint8_t command, uint32_t iframe, uint32_t addr,
                             uint8_t *buffer, uint32_t size);

// Turn off cache and invalidate all data in it.
static void samd_peripherals_disable_and_clear_cache(void) {
  CMCC->CTRL.bit.CEN = 0;
  while (CMCC->SR.bit.CSTS) {
  }
  CMCC->MAINT0.bit.INVALL = 1;
}

// Enable cache
static void samd_peripherals_enable_cache(void) { CMCC->CTRL.bit.CEN = 1; }

Adafruit_FlashTransport_QSPI::Adafruit_FlashTransport_QSPI(void)
    : Adafruit_FlashTransport_QSPI(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0,
                                   PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3) {}

Adafruit_FlashTransport_QSPI::Adafruit_FlashTransport_QSPI(
    int8_t sck, int8_t cs, int8_t io0, int8_t io1, int8_t io2, int8_t io3) {
  _addr_len = 3; // work with most device if not set
  _cmd_read = SFLASH_CMD_QUAD_READ;
  _sck = sck;
  _cs = cs;
  _io0 = io0;
  _io1 = io1;
  _io2 = io2;
  _io3 = io3;
}

void Adafruit_FlashTransport_QSPI::begin(void) {
  MCLK->APBCMASK.bit.QSPI_ = true;
  MCLK->AHBMASK.bit.QSPI_ = true;
  MCLK->AHBMASK.bit.QSPI_2X_ = false; // Only true if we are doing DDR.

  QSPI->CTRLA.bit.SWRST = 1;

  // set all pins to QSPI periph
  pinPeripheral(_sck, PIO_COM);
  pinPeripheral(_cs, PIO_COM);
  pinPeripheral(_io0, PIO_COM);
  pinPeripheral(_io1, PIO_COM);
  pinPeripheral(_io2, PIO_COM);
  pinPeripheral(_io3, PIO_COM);

  QSPI->BAUD.reg =
      QSPI_BAUD_BAUD(VARIANT_MCK / 4000000UL); // start with low 4Mhz, Mode 0
  QSPI->CTRLB.reg = QSPI_CTRLB_MODE_MEMORY | QSPI_CTRLB_CSMODE_NORELOAD |
                    QSPI_CTRLB_DATALEN_8BITS | QSPI_CTRLB_CSMODE_LASTXFER;

  QSPI->CTRLA.bit.ENABLE = 1;
}

bool Adafruit_FlashTransport_QSPI::runCommand(uint8_t command) {
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI |
                    QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_READ | QSPI_INSTRFRAME_INSTREN;

  _run_instruction(command, iframe, 0, NULL, 0);
  return true;
}

bool Adafruit_FlashTransport_QSPI::readCommand(uint8_t command,
                                               uint8_t *response,
                                               uint32_t len) {
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI |
                    QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_READ | QSPI_INSTRFRAME_INSTREN |
                    QSPI_INSTRFRAME_DATAEN;

  samd_peripherals_disable_and_clear_cache();
  _run_instruction(command, iframe, 0, response, len);
  samd_peripherals_enable_cache();

  return true;
}

bool Adafruit_FlashTransport_QSPI::writeCommand(uint8_t command,
                                                uint8_t const *data,
                                                uint32_t len) {
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI |
                    QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_WRITE | QSPI_INSTRFRAME_INSTREN |
                    (data != NULL ? QSPI_INSTRFRAME_DATAEN : 0);

  samd_peripherals_disable_and_clear_cache();
  _run_instruction(command, iframe, 0, (uint8_t *)data, len);
  samd_peripherals_enable_cache();

  return true;
}

bool Adafruit_FlashTransport_QSPI::eraseCommand(uint8_t command,
                                                uint32_t address) {
  // Sector Erase
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI |
                    QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_WRITE | QSPI_INSTRFRAME_INSTREN |
                    QSPI_INSTRFRAME_ADDREN;

  _run_instruction(command, iframe, address, NULL, 0);
  return true;
}

bool Adafruit_FlashTransport_QSPI::readMemory(uint32_t addr, uint8_t *data,
                                              uint32_t len) {
  // Command 0x6B 1 line address, 4 line Data
  // Quad output mode, read memory type
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_QUAD_OUTPUT |
                    QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_READMEMORY |
                    QSPI_INSTRFRAME_INSTREN | QSPI_INSTRFRAME_ADDREN |
                    QSPI_INSTRFRAME_DATAEN | QSPI_INSTRFRAME_DUMMYLEN(8);

  samd_peripherals_disable_and_clear_cache();
  _run_instruction(SFLASH_CMD_QUAD_READ, iframe, addr, data, len);
  samd_peripherals_enable_cache();

  return true;
}

bool Adafruit_FlashTransport_QSPI::writeMemory(uint32_t addr,
                                               uint8_t const *data,
                                               uint32_t len) {
  uint32_t iframe =
      QSPI_INSTRFRAME_WIDTH_QUAD_OUTPUT | QSPI_INSTRFRAME_ADDRLEN_24BITS |
      QSPI_INSTRFRAME_TFRTYPE_WRITEMEMORY | QSPI_INSTRFRAME_INSTREN |
      QSPI_INSTRFRAME_ADDREN | QSPI_INSTRFRAME_DATAEN;

  samd_peripherals_disable_and_clear_cache();
  _run_instruction(SFLASH_CMD_QUAD_PAGE_PROGRAM, iframe, addr, (uint8_t *)data,
                   len);
  samd_peripherals_enable_cache();

  return true;
}

/**************************************************************************/
/*!
 @brief set the clock speed
 @param clock_hz clock speed in hertz
 */
/**************************************************************************/
void Adafruit_FlashTransport_QSPI::setClockSpeed(uint32_t clock_hz,
                                                 uint32_t read_hz) {
  (void)read_hz;
  QSPI->BAUD.bit.BAUD = VARIANT_MCK / clock_hz;
}

/**************************************************************************/
/*!
 @brief Run a single QSPI instruction.
 @param command instruction code
 @param iframe iframe register value (configured by caller according to command
 code)
 @param addr the address to read or write from. If the instruction doesn't
 require an address, this parameter is meaningless.
 @param buffer pointer to the data to be written or stored depending on the type
 is Read or Write
 @param size the number of bytes to read or write.
 */
/**************************************************************************/
static void _run_instruction(uint8_t command, uint32_t iframe, uint32_t addr,
                             uint8_t *buffer, uint32_t size) {
  if (command == SFLASH_CMD_ERASE_SECTOR || command == SFLASH_CMD_ERASE_BLOCK) {
    QSPI->INSTRADDR.reg = addr;
  }

  QSPI->INSTRCTRL.bit.INSTR = command;
  QSPI->INSTRFRAME.reg = iframe;

  // Dummy read of INSTRFRAME needed to synchronize.
  // See Instruction Transmission Flow Diagram, figure 37.9, page 995
  // and Example 4, page 998, section 37.6.8.5.
  (volatile uint32_t) QSPI->INSTRFRAME.reg;

  if (buffer && size) {
    uint8_t *qspi_mem = (uint8_t *)(QSPI_AHB + addr);
    uint32_t const tfr_type = iframe & QSPI_INSTRFRAME_TFRTYPE_Msk;

    if ((tfr_type == QSPI_INSTRFRAME_TFRTYPE_READ) ||
        (tfr_type == QSPI_INSTRFRAME_TFRTYPE_READMEMORY)) {
      memcpy(buffer, qspi_mem, size);
    } else {
      memcpy(qspi_mem, buffer, size);
    }
  }

  QSPI->CTRLA.reg = QSPI_CTRLA_ENABLE | QSPI_CTRLA_LASTXFER;

  while (!QSPI->INTFLAG.bit.INSTREND) {
    yield();
  }
  QSPI->INTFLAG.bit.INSTREND = 1;
}

#endif
