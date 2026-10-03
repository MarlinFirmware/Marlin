/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Software SPI for Atmel SAM3U (AT91SAM3U4E)
 *
 * The SAM3U's SPI peripheral is chip-select driven and awkward to share
 * between Marlin's several SPI consumers, and the only device on the 4pi's SPI
 * bus is the AD5206 digipot - a write-only, low-rate part where a bit-banged
 * master costs nothing. So this HAL bit-bangs, which also keeps the bus usable
 * on any pin combination a derived board might use.
 *
 * Mode 0 (CPOL=0, CPHA=0), MSB first.
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"
#include <SPI.h>

// Half-period delay per speed index, in loop iterations. Index 0 runs as fast
// as the pin toggles allow; each step roughly halves the rate.
static uint8_t spi_delay_cycles = 0;

static FORCE_INLINE void spi_delay() {
  for (uint8_t i = spi_delay_cycles; i--;) __asm__ __volatile__("nop");
}

void spiBegin() {
  // Only touch a chip select if the configuration actually defines one. The
  // fastio macros do no range checking, so writing pin -1 would land on PA31.
  #if PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH);
  #endif
  OUT_WRITE(SD_SCK_PIN, LOW);
  OUT_WRITE(SD_MOSI_PIN, HIGH);
  SET_INPUT_PULLUP(SD_MISO_PIN);
}

void spiInit(uint8_t spiRate) {
  // Roughly double the half-period for each step down in rate
  spi_delay_cycles = spiRate >= 6 ? 64 : (1 << spiRate) >> 1;
}

void spiSend(uint8_t b) {
  for (uint8_t bits = 8; bits--;) {
    WRITE(SD_MOSI_PIN, b & 0x80);
    b <<= 1;
    spi_delay();
    WRITE(SD_SCK_PIN, HIGH);
    spi_delay();
    WRITE(SD_SCK_PIN, LOW);
  }
}

// Full-duplex exchange of one byte: MSB first, sampled on the rising edge
uint8_t spiTransfer(uint8_t b) {
  uint8_t in = 0;
  for (uint8_t bits = 8; bits--;) {
    WRITE(SD_MOSI_PIN, b & 0x80);
    b <<= 1;
    spi_delay();
    WRITE(SD_SCK_PIN, HIGH);
    in = (in << 1) | READ(SD_MISO_PIN);
    spi_delay();
    WRITE(SD_SCK_PIN, LOW);
  }
  return in;
}

uint8_t spiRec() { return spiTransfer(0xFF); }

void spiRead(uint8_t *buf, uint16_t nbyte) {
  for (uint16_t i = 0; i < nbyte; i++) buf[i] = spiRec();
}

void spiSendBlock(uint8_t token, const uint8_t *buf) {
  spiSend(token);
  for (uint16_t i = 0; i < 512; i++) spiSend(buf[i]);
}

void spiBeginTransaction(uint32_t spiClock, uint8_t, uint8_t) {
  // Map the requested clock onto the nearest supported rate index
  uint8_t rate = 0;
  for (uint32_t clk = 8000000; rate < 6 && spiClock < clk; clk >>= 1) rate++;
  spiInit(rate);
}

// Channel-selecting variants. The 4pi has a single SPI device, so the channel
// is ignored and the caller is responsible for asserting its own chip select.
void spiSend(uint32_t, byte b) { spiSend(b); }

void spiSend(uint32_t, const uint8_t *buf, size_t n) {
  for (size_t i = 0; i < n; i++) spiSend(buf[i]);
}

// The global instance Marlin core code expects from <SPI.h>
SPIClass SPI;

#endif // __SAM3U4E__
