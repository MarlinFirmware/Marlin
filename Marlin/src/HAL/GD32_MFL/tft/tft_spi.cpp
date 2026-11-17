/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../platforms.h"

#ifdef ARDUINO_ARCH_MFL

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_TFT

#include "tft_spi.h"

#include <SPI.hpp>

// Registers the core does not wrap
constexpr uint32_t afio_mapr_addr = 0x40010004UL,   // AFIO_MAPR
                   gpioa_ctl0_addr = 0x40010800UL;  // GPIOA_CTL0
constexpr uint32_t spi_ctl0_swnssen = 1U << 9;

uint16_t TFT_SPI::dataWidth = DATASIZE_16BIT;

static spi::SPI *spi_dev = nullptr;

/**
 * Send one byte and wait until it has left the shift register.
 *
 * TBE only frees the transmit buffer, so D/C or CS must not change while the
 * byte is still on the wire. The margin shrinks with a faster core clock,
 * which is why this only showed up at 120 MHz.
 */
static inline void txByte(uint8_t b) {
  if (!spi_dev) return;
  for (uint32_t i = 100000U; i; --i)
    if (spi_dev->get_flag(spi::Status_Flags::FLAG_TBE)) break;
  spi_dev->data_transmit(b);
  (void)spi_dev->data_receive();        // Clear RBNE before it can set RXORERR
  for (uint32_t i = 2000U; i; --i)      // Bounded: a stuck bus must not hang
    if (!spi_dev->get_flag(spi::Status_Flags::FLAG_TRANS)) break;
}

void TFT_SPI::init() {
  #if PIN_EXISTS(TFT_POWER)
    OUT_WRITE(TFT_POWER_PIN, HIGH);
  #endif

  #if PIN_EXISTS(TFT_RESET)
    OUT_WRITE(TFT_RESET_PIN, HIGH);
    delay(100);
  #endif

  #if PIN_EXISTS(TFT_BACKLIGHT)
    OUT_WRITE(TFT_BACKLIGHT_PIN, HIGH);
  #endif

  OUT_WRITE(TFT_CS_PIN, HIGH);
  OUT_WRITE(TFT_DC_PIN, HIGH);

  spi_dev = &spi::SPI::get_instance(spi::SPI_Base::SPI0_BASE).value();

  // With SWNSS low a master takes a mode fault: no clock, MSTR cleared
  spi_dev->nss_internal_high();

  SPI.begin();

  // Reconfigure with the peripheral disabled so the prescaler is latched
  spi_dev->set_enable(false);
  SPI.setClockDivider(SPI_CLOCK_MAX_TFT);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(TFT_SPI_MODE);

  // PA4 is both the panel CS and the SPI NSS pin, so force software NSS
  spi_dev->nss_internal_high();
  *spi_dev->reg_address(spi::SPI_Regs::CTL0) |= spi_ctl0_swnssen;
  spi_dev->set_enable(true);

  // The core's pin lookup can fail silently, leaving SCK/MOSI as GPIOs
  {
    volatile uint32_t *const afio_mapr = reinterpret_cast<volatile uint32_t*>(afio_mapr_addr);
    *afio_mapr &= ~0x1U;                                     // SPI0_REMAP = 0

    volatile uint32_t *const gpio_ctl0 = reinterpret_cast<volatile uint32_t*>(gpioa_ctl0_addr);
    uint32_t v = *gpio_ctl0;
    v = (v & ~(0xFU << 16)) | (0x2U << 16);                  // PA4  CS   : output push-pull
    v = (v & ~(0xFU << 20)) | (0xBU << 20);                  // PA5  SCK  : AF push-pull 50 MHz
    v = (v & ~(0xFU << 24)) | (0x2U << 24);                  // PA6  DC   : output push-pull
    v = (v & ~(0xFU << 28)) | (0xBU << 28);                  // PA7  MOSI : AF push-pull 50 MHz
    *gpio_ctl0 = v;
  }
}

/**
 * Assert CS. CONFERR latches until it is written back, so clear the status
 * register first.
 */
void TFT_SPI::dataTransferBegin(uint16_t dataSize) {
  dataWidth = dataSize;
  if (spi_dev) *spi_dev->reg_address(spi::SPI_Regs::STAT) = 0U;
  WRITE(TFT_CS_PIN, LOW);
}

void TFT_SPI::dataTransferEnd() {
  WRITE(TFT_CS_PIN, HIGH);              // txByte() already drained the shifter
}

void TFT_SPI::writeData(uint16_t data) {
  if (dataWidth == DATASIZE_8BIT)
    txByte((uint8_t)data);
  else {
    txByte((uint8_t)(data >> 8));
    txByte((uint8_t)data);
  }
}

void TFT_SPI::writeReg(const uint16_t inReg) {
  WRITE(TFT_DC_PIN, LOW);
  txByte((uint8_t)inReg);
  WRITE(TFT_DC_PIN, HIGH);
}

void TFT_SPI::writeSequence(uint16_t *data, uint16_t count) {
  dataTransferBegin(DATASIZE_16BIT);
  WRITE(TFT_DC_PIN, HIGH);
  while (count--) writeData(*data++);
  dataTransferEnd();
}

void TFT_SPI::writeMultiple(uint16_t color, uint32_t count) {
  dataTransferBegin(DATASIZE_16BIT);
  WRITE(TFT_DC_PIN, HIGH);
  while (count--) writeData(color);
  dataTransferEnd();
}

/**
 * The panel has no MISO line, so its ID cannot be read back.
 */
uint32_t TFT_SPI::getID() {
  #ifdef TFT_DEFAULT_DRIVER
    return TFT_DEFAULT_DRIVER;
  #else
    return 0;
  #endif
}

#endif // HAS_SPI_TFT
#endif // ARDUINO_ARCH_MFL
