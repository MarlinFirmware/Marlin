/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef _SOFTWARE_SPI_H_
#define _SOFTWARE_SPI_H_

#include <pinmapping.h>

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

void swSpiBegin(const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin);

// Returns the spi_speed value to be passed to swSpiTransfer
uint8_t swSpiInit(const uint8_t spiRate, const pin_t sck_pin, const pin_t mosi_pin);

uint8_t swSpiTransfer(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin);

#endif // _SOFTWARE_SPI_H_
