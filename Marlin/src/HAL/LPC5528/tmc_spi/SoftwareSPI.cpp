#ifdef TARGET_LPC5528

#include "../../../inc/MarlinConfig.h"

// --------------------------------------------------------------------------
// Software SPI
// --------------------------------------------------------------------------

/**
 * This software SPI runs at multiple rates. The SD software provides an index
 * (spiRate) of 0-6. The mapping is:
 *     0 - about 5 MHz peak (6 MHz on LPC1769)
 *     1-2 - about 2 MHz peak
 *     3 - about 1 MHz peak
 *     4 - about 500 kHz peak
 *     5 - about 250 kHz peak
 *     6 - about 125 kHz peak
 */

uint8_t swSpiTransfer(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin) {
  for (uint8_t i = 0; i < 8; i++) {
    if (spi_speed == 0) {
      WRITE(mosi_pin, !!(b & 0x80));
      WRITE(sck_pin, HIGH);
      b <<= 1;
      if (miso_pin >= 0 && READ(miso_pin)) b |= 1;
      WRITE(sck_pin, LOW);
    }
    else {
      const uint8_t state = (b & 0x80) ? HIGH : LOW;
      for (uint8_t j = 0; j < spi_speed; j++)
        WRITE(mosi_pin, state);

      for (uint8_t j = 0; j < spi_speed + (miso_pin >= 0 ? 0 : 1); j++)
        WRITE(sck_pin, HIGH);

      b <<= 1;
      if (miso_pin >= 0 && READ(miso_pin)) b |= 1;

      for (uint8_t j = 0; j < spi_speed; j++)
        WRITE(sck_pin, LOW);
    }
  }
  return b;
}

void swSpiBegin(const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin) {
  SET_OUTPUT(sck_pin);
  SET_INPUT(miso_pin);
  SET_OUTPUT(mosi_pin);
}

uint8_t swSpiInit(const uint8_t spiRate, const pin_t sck_pin, const pin_t mosi_pin) {
  WRITE(mosi_pin, HIGH);
  WRITE(sck_pin, LOW);
  return (SystemCoreClock == 120000000 ? 44 : 38) / POW(2, 6 - MIN(spiRate, 6));
}

#endif // TARGET_LPC5528
