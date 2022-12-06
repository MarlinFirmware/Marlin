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
 * For TARGET_LPC1768
 */

/**
 * Hardware SPI and Software SPI implementations are included in this file.
 *
 * Control of the slave select pin(s) is handled by the calling routines.
 *
 * Some of the LCD interfaces/adapters result in the LCD SPI and the SD card
 * SPI sharing pins. The SCK, MOSI & MISO pins can NOT be set/cleared with
 * WRITE nor digitalWrite when the hardware SPI module within the LPC17xx is
 * active. If any of these pins are shared then the software SPI must be used.
 */

// Actually: LPC176x/LPC175x
#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#include "../shared/HAL_SPI.h"

// ------------------------
// Public functions
// ------------------------
#if ENABLED(SOFTWARE_SPI)

  // Software SPI

  #include <SoftwareSPI.h>

  static uint8_t SPI_speed = SPI_FULL_SPEED;
  static int _spi_bit_order = SPI_BITORDER_DEFAULT;
  static int _spi_sck_pin, _spi_miso_pin, _spi_mosi_pin;
  static int _spi_cs_pin;

  static uint8_t spiTransfer(uint8_t b) {
    return swSpiTransfer(b, SPI_speed, _spi_sck_pin, _spi_miso_pin, _spi_mosi_pin);
  }

  void spiBegin() {
    //swSpiBegin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN);
  }

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    int use_sck = ( ( hint_sck >= 0 ) ? hint_sck : SD_SCK_PIN );
    int use_miso = ( ( hint_miso >= 0 ) ? hint_miso : SD_MISO_PIN );
    int use_mosi = ( ( hint_mosi >= 0 ) ? hint_mosi : SD_MOSI_PIN );
    _spi_sck_pin = use_sck;
    _spi_miso_pin = use_miso;
    _spi_mosi_pin = use_mosi;
    swSpiBegin(use_sck, use_miso, use_mosi);
    SPI_speed = swSpiInit(spiRate, use_sck, use_mosi);
    _spi_bit_order = SPI_BITORDER_DEFAULT;
    _spi_cs_pin = hint_cs;
    if (hint_cs >= 0)
      OUT_WRITE(hint_cs, LOW);
  }

  void spiInitEx(uint32_t maxClockFreq, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use datarates Marlin uses
    uint8_t spiRate;
    if (maxClockFreq >= 20000000) {
      spiRate = SPI_FULL_SPEED;
    }
    else if (maxClockFreq >= 5000000) {
      spiRate = SPI_HALF_SPEED;
    }
    else if (maxClockFreq >= 2500000) {
      spiRate = SPI_QUARTER_SPEED;
    }
    else if (maxClockFreq >= 1250000) {
      spiRate = SPI_EIGHTH_SPEED;
    }
    else if (maxClockFreq >= 625000) {
      spiRate = SPI_SPEED_5;
    }
    else if (maxClockFreq >= 300000) {
      spiRate = SPI_SPEED_6;
    }
    else
      spiRate = SPI_SPEED_6;
    
    spiInit(spiRate, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  void spiClose() {
    if (_spi_cs_pin >= 0)
      OUT_WRITE(_spi_cs_pin, HIGH);
  }

  static inline uint8_t _flip_bits_8(uint8_t v) {
    uint8_t result = 0;
    for (int n = 0; n < 8; n++)
    {
      result <<= 1;
      bool bitval = ( v & ( 1 << n ) ) != 0;
      result |= bitval;
    }
    return result;
  }

  void spiSetBitOrder(int bitOrder) {
    _spi_bit_order = bitOrder;
  }

  void spiSetClockMode(int mode) {
    if (mode != SPI_CLKMODE_0) {
      // FATAL ERROR: not supported.
      while (true) {
#if ENABLED(HALSPI_DO_ERRORBEEPS) && PIN_EXISTS(BEEPER)
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(500);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(500);
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(300);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(1500);
#endif
      }
    }
  }

  void spiEstablish() {
    _maybe_start_transaction();
  }

  uint8_t spiRec(uint8_t txval) { return (_spi_bit_order == SPI_BITORDER_MSB) ? spiTransfer(txval) : _flip_bits_8(spiTransfer(_flip_bits_8(txval))); }
  
  uint16_t spiRec16(uint16_t txval) {
    bool msb = (_spi_bit_order == SPI_BITORDER_MSB);
    uint8_t tx_first, tx_second;
    if (msb) {
      tx_first = ( txval >> 8 );
      tx_second = ( txval & 0xFF );
    }
    else {
      tx_first = ( txval & 0xFF );
      tx_second = ( txval >> 8 );
    }

    uint16_t v = ( msb ? ( (uint16_t)spiRec(tx_first) << 8 ) : spiRec(tx_first) );
    v |= ( msb ? spiRec(tx_second) : ( (uint16_t)spiRec(tx_second) << 8 ) );
    return v;
  }

  void spiRead(uint8_t*buf, uint16_t nbyte, uint8_t txval) {
    for (int i = 0; i < nbyte; i++)
      buf[i] = spiRec(txval);
  }

  void spiSend(uint8_t b) { (void)spiTransfer((_spi_bit_order == SPI_BITORDER_MSB) ? b : _flip_bits_8(b)); }

  void spiSend16(uint16_t v) {
    bool msb = (_spi_bit_order == SPI_BITORDER_MSB);
    spiSend(msb ? (v >> 8) : (v&0xFF));
    spiSend(msb ? (v&0xFF) : (v >> 8));
  }

  void spiSend(const uint8_t *buf, size_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++)
      spiSend(buf[i]);
  }

  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    spiSend(token);
    for (uint16_t i = 0; i < 512; i++)
      spiSend(buf[i]);
  }

  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend(buf[n]);
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    for (uint16_t n = 0; n < cnt; n++)
      spiSend16(buf[n]);
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend(val);
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    for (uint16_t n = 0; n < repcnt; n++)
      spiSend16(val);
  }
  
#endif

#endif // TARGET_LPC1768
