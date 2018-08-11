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

/**
 * HAL/HAL_SPI.h
 * Core Marlin definitions for SPI, implemented in the HALs
 */

#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_

#include <stdint.h>

/**
 * SPI speed where 0 <= index <= 6
 *
 * Approximate rates :
 *
 *  0 :  8 - 10 MHz
 *  1 :  4 - 5 MHz
 *  2 :  2 - 2.5 MHz
 *  3 :  1 - 1.25 MHz
 *  4 :  500 - 625 kHz
 *  5 :  250 - 312 kHz
 *  6 :  125 - 156 kHz
 *
 *  On AVR, actual speed is F_CPU/2^(1 + index).
 *  On other platforms, speed should be in range given above where possible.
 */

#define SPI_FULL_SPEED      0   // Set SCK to max rate
#define SPI_HALF_SPEED      1   // Set SCK rate to half of max rate
#define SPI_QUARTER_SPEED   2   // Set SCK rate to quarter of max rate
#define SPI_EIGHTH_SPEED    3   // Set SCK rate to 1/8 of max rate
#define SPI_SIXTEENTH_SPEED 4   // Set SCK rate to 1/16 of max rate
#define SPI_SPEED_5         5   // Set SCK rate to 1/32 of max rate
#define SPI_SPEED_6         6   // Set SCK rate to 1/64 of max rate

#define SPI_LSBFIRST 0
#define SPI_MSBFIRST 1

#define SPI_DATAMODE_0 0x00
#define SPI_DATAMODE_1 0x04
#define SPI_DATAMODE_2 0x08
#define SPI_DATAMODE_3 0x0C

// Standard SPI functions
/** Initialise SPI bus */
void spiBegin(void);
/** Configure SPI for specified SPI speed */
void spiInit(uint8_t spiRate);
/** Write single byte to SPI */
void spiSend(uint8_t b);
/** Read single byte from SPI */
uint8_t spiRec(void);
/** Read from SPI into buffer */
void spiRead(uint8_t* buf, uint16_t nbyte);
/** Write token and then write from 512 byte buffer to SPI (for SD card) */
void spiSendBlock(uint8_t token, const uint8_t* buf);
/** Begin SPI transaction, set clock, bit order, data mode */
void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode);

#endif // _HAL_SPI_H_
