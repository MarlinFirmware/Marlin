/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 */

/**
 *
 * For TARGET_LPC1768
 */

#include "src/inc/MarlinConfig.h"

#ifdef TARGET_LPC1768

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

uint8_t swSpiTransfer(uint8_t b, uint8_t spi_speed, pin_t sck_pin, pin_t miso_pin, pin_t mosi_pin) {
  for (uint8_t i = 0; i < 8; i++) {
    if (spi_speed == 0) {
      if (b & 0x80)
        WRITE(mosi_pin, HIGH);
      else
        WRITE(mosi_pin, LOW);

      WRITE(sck_pin, HIGH);
      
      b <<= 1;

      if (miso_pin >= 0)
        if (READ(miso_pin)) b |= 1;

      WRITE(sck_pin, LOW);
    }
    else {
      if (b & 0x80)
        for (uint8_t j = 0; j < spi_speed; j++)
          WRITE(mosi_pin, HIGH);
      else
        for (uint8_t j = 0; j < spi_speed; j++)
          WRITE(mosi_pin, LOW);

      for (uint8_t j = 0; j < spi_speed + (miso_pin >= 0 ? 0 : 1); j++)
        WRITE(sck_pin, HIGH);
      
      b <<= 1;

      if (miso_pin >= 0)
        if (READ(miso_pin)) b |= 1;

      for (uint8_t j = 0; j < spi_speed; j++)
        WRITE(sck_pin, LOW);
    }
  }

  return b;
}

void swSpiBegin(pin_t sck_pin, pin_t miso_pin, pin_t mosi_pin) {
  SET_OUTPUT(sck_pin);
  if (VALID_PIN(miso_pin))
    SET_INPUT(miso_pin);
  SET_OUTPUT(mosi_pin);
}

uint8_t swSpiInit(uint8_t spiRate, pin_t sck_pin, pin_t mosi_pin) {
  uint8_t spi_speed = 0;

  spiRate = MIN(spiRate, 6);

  if (SystemCoreClock == 120000000)
    spi_speed = 44 / POW(2, 6 - spiRate);
  else
    spi_speed = 38 / POW(2, 6 - spiRate);

  WRITE(mosi_pin, HIGH);
  WRITE(sck_pin, LOW);

  return spi_speed;
}

#endif // TARGET_LPC1768
