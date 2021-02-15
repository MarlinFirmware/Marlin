/**
 * @file Adafruit_FlashTransport_QSPI.cpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach for Adafruit Industries LLC
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

#ifdef NRF52840_XXAA

#include "Adafruit_FlashTransport.h"
#include "nrfx_qspi.h"
#include <Arduino.h>

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
  // Init QSPI flash
  nrfx_qspi_config_t qspi_cfg = {
      .xip_offset = 0,
      .pins =
          {
              .sck_pin = (uint8_t)g_ADigitalPinMap[_sck],
              .csn_pin = (uint8_t)g_ADigitalPinMap[_cs],
              .io0_pin = (uint8_t)g_ADigitalPinMap[_io0],
              .io1_pin = (uint8_t)g_ADigitalPinMap[_io1],
              .io2_pin = (uint8_t)g_ADigitalPinMap[_io2],
              .io3_pin = (uint8_t)g_ADigitalPinMap[_io3],
          },
      .prot_if = {.readoc = NRF_QSPI_READOC_READ4O, // 0x6B read command
                  .writeoc = NRF_QSPI_WRITEOC_PP4O, // 0x32 write command
                  .addrmode = NRF_QSPI_ADDRMODE_24BIT,
                  .dpmconfig = false},
      .phy_if =
          {
              .sck_delay = 10,
              .dpmen = false,
              .spi_mode = NRF_QSPI_MODE_0,
              .sck_freq = NRF_QSPI_FREQ_32MDIV16, // start with low 2 Mhz speed
          },
      .irq_priority = 7};

  // No callback for blocking API
  nrfx_qspi_init(&qspi_cfg, NULL, NULL);
}

void Adafruit_FlashTransport_QSPI::setClockSpeed(uint32_t clock_hz,
                                                 uint32_t read_hz) {
  (void)read_hz;
  // Start at 16 MHz and go down.
  // At 32 MHz nRF52840 doesn't work reliably !!!
  // clkdiv = 0 is 32 Mhz
  // clkdiv = 1 is 16 MHz, etc.
  uint8_t clkdiv = 1;
  while ((32000000UL / (clkdiv + 1) > clock_hz) && (clkdiv < 16)) {
    clkdiv++;
  }

  // delay is set to one freq period
  uint8_t delay = 1;

  if (clkdiv)
    delay = (1UL << (clkdiv - 1));

  NRF_QSPI->IFCONFIG1 &=
      ~(QSPI_IFCONFIG1_SCKFREQ_Msk | QSPI_IFCONFIG1_SCKDELAY_Msk);
  NRF_QSPI->IFCONFIG1 |= (clkdiv << QSPI_IFCONFIG1_SCKFREQ_Pos) |
                         (delay << QSPI_IFCONFIG1_SCKDELAY_Pos);
}

bool Adafruit_FlashTransport_QSPI::runCommand(uint8_t command) {
  nrf_qspi_cinstr_conf_t cinstr_cfg = {.opcode = command,
                                       .length = NRF_QSPI_CINSTR_LEN_1B,
                                       .io2_level = true,
                                       .io3_level = true,
                                       .wipwait = false,
                                       .wren = false};

  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL) == NRFX_SUCCESS;
}

bool Adafruit_FlashTransport_QSPI::readCommand(uint8_t command,
                                               uint8_t *response,
                                               uint32_t len) {
  nrf_qspi_cinstr_conf_t cinstr_cfg = {.opcode = command,
                                       .length =
                                           (nrf_qspi_cinstr_len_t)(len + 1),
                                       .io2_level = true,
                                       .io3_level = true,
                                       .wipwait = false,
                                       .wren = false};
  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, response) == NRFX_SUCCESS;
}

bool Adafruit_FlashTransport_QSPI::writeCommand(uint8_t command,
                                                uint8_t const *data,
                                                uint32_t len) {
  nrf_qspi_cinstr_conf_t cinstr_cfg = {
      .opcode = command,
      .length = (nrf_qspi_cinstr_len_t)(len + 1),
      .io2_level = true,
      .io3_level = true,
      .wipwait = false,
      .wren = false // We do this manually.
  };
  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, NULL) == NRFX_SUCCESS;
}

bool Adafruit_FlashTransport_QSPI::eraseCommand(uint8_t command,
                                                uint32_t address) {
  nrf_qspi_erase_len_t erase_len;

  if (command == SFLASH_CMD_ERASE_SECTOR) {
    erase_len = NRF_QSPI_ERASE_LEN_4KB;
  } else if (command == SFLASH_CMD_ERASE_BLOCK) {
    erase_len = NRF_QSPI_ERASE_LEN_64KB;
  } else {
    return false;
  }

  return NRFX_SUCCESS == nrfx_qspi_erase(erase_len, address);
}

//--------------------------------------------------------------------+
// Read & Write
//--------------------------------------------------------------------+
static uint32_t read_write_odd(bool read_op, uint32_t addr, uint8_t *data,
                               uint32_t len) {
  uint8_t buf4[4] __attribute__((aligned(4)));
  uint32_t count = 4 - (((uint32_t)data) & 0x03);
  count = min(count, len);

  if (read_op) {
    if (NRFX_SUCCESS != nrfx_qspi_read(buf4, 4, addr)) {
      return 0;
    }

    memcpy(data, buf4, count);
  } else {
    memset(buf4, 0xff, 4);
    memcpy(buf4, data, count);

    if (NRFX_SUCCESS != nrfx_qspi_write(buf4, 4, addr)) {
      return 0;
    }
  }

  return count;
}

static bool read_write_memory(bool read_op, uint32_t addr, uint8_t *data,
                              uint32_t len) {
  // buffer is not 4-byte aligned
  if (((uint32_t)data) & 3) {
    uint32_t const count = read_write_odd(read_op, addr, data, len);
    if (!count) {
      return false;
    }

    data += count;
    addr += count;
    len -= count;
  }

  // nrfx_qspi_read works in 4 byte increments, though it doesn't
  // signal an error if sz is not a multiple of 4.  Read (directly into data)
  // all but the last 1, 2, or 3 bytes depending on the (remaining) length.
  if (len > 3) {
    uint32_t const len4 = len & ~(uint32_t)3;

    if (read_op) {
      if (NRFX_SUCCESS != nrfx_qspi_read(data, len4, addr)) {
        return 0;
      }
    } else {
      if (NRFX_SUCCESS != nrfx_qspi_write(data, len4, addr)) {
        return 0;
      }
    }

    data += len4;
    addr += len4;
    len -= len4;
  }

  // Now, if we have any bytes left over, we must do a final read of 4
  // bytes and copy 1, 2, or 3 bytes to data.
  if (len) {
    if (!read_write_odd(read_op, addr, data, len)) {
      return false;
    }
  }

  return true;
}

bool Adafruit_FlashTransport_QSPI::readMemory(uint32_t addr, uint8_t *data,
                                              uint32_t len) {
  return read_write_memory(true, addr, data, len);
}

bool Adafruit_FlashTransport_QSPI::writeMemory(uint32_t addr,
                                               uint8_t const *data,
                                               uint32_t len) {
  return read_write_memory(false, addr, (uint8_t *)data, len);
}

#endif
