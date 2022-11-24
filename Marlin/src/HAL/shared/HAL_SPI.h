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
#pragma once

/**
 * HAL/shared/HAL_SPI.h
 * Core Marlin definitions for SPI, implemented in the HALs
 */

#include "Marduino.h"
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
#define SPI_SPEED_DEFAULT   255 // Let the framework decide (usually recommended value)    

// Transmission order for binary polynomes (8bit, 16bit).
// This is significant because numbers are sent bit-by-bit.
#define SPI_BITORDER_MSB        0   // most-significant bit first
#define SPI_BITORDER_LSB        1   // least-significant bit first
#define SPI_BITORDER_DEFAULT    SPI_BITORDER_MSB

#define SPI_CLKMODE_0           0   // data latched on first edge, clock starts LOW
#define SPI_CLKMODE_1           1   // data latched on the second edge, clock starts LOW
#define SPI_CLKMODE_2           2   // data latched on the first edge, clock starts HIGH
#define SPI_CLKMODE_3           3   // data latched on the second edge, clock starts HIGH
#define SPI_CLKMODE_DEFAULT     SPI_CLKMODE_0

//
// Standard SPI functions
//

// Initialize SPI bus (global).
void spiBegin();

// Configure SPI based on maximum clock frequency/rate.
// Pass 0 for maxClockFreq to get a recommended default.
void spiInitEx(uint32_t maxClockFreq, int hint_sck = -1, int hint_miso = -1, int hint_mosi = -1, int hint_cs = -1);

// Configure SPI for specified SPI speed
void spiInit(uint8_t spiRate, int hint_sck = -1, int hint_miso = -1, int hint_mosi = -1, int hint_cs = -1);

// Terminates SPI connection.
void spiClose();

// Specifies the bit-order, either LSB or MSB.
void spiSetBitOrder(int bitOrder);

// Specifies the CLKMODE.
void spiSetClockMode(int mode);

// Write single byte to SPI.
// The byte is sent in the requested bit-order.
void spiSend(uint8_t b);

// Write a 16bit number to SPI.
// The number is sent in the requested bit-order.
void spiSend16(uint16_t v);

// Read single byte from SPI
// The byte is received in the requested bit-order.
uint8_t spiRec(uint8_t txval = 0xFF);

// Read 16bit number from SPI.
// The number is received in the requested bit-order.
uint16_t spiRec16(uint16_t txval = 0xFFFF);

// Read from SPI into buffer
// Each 8bit polynome is transmitted in the requested bit-order.
// Do not be confused with treating the buffer as an array of bits:
//  the whole array is not ordered but each invidual byte is.
void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval = 0xFF);

// Write token and then write from 512 byte buffer to SPI (for SD card)
// Similar to writing each byte by itself.
void spiSendBlock(uint8_t token, const uint8_t *buf);

// Send a buffer of 8bit numbers, in the correct bit-order, through SPI.
// Can be transmitted through DMA for optimal performance.
void spiWrite(const uint8_t *buf, uint16_t nbyte);

// Send a buffer of 16bit numbers, in the correct bit-order, through SPI.
// Similar to spiWrite but supports more data size.
void spiWrite16(const uint16_t *buf, uint16_t nbyte);

// Send a repetition of 8bit numbers.
void spiWriteRepeat(uint8_t val, uint16_t repcnt);

// Send a repetition of 16bit numbers.
void spiWriteRepeat16(uint16_t val, uint16_t repcnt);

#if (defined(ESP_PLATFORM) || defined(STM32F1xx) || defined(STM32F4xx) || defined(TARGET_LPC1768)) && !defined(SOFTWARE_SPI) && defined(USE_SPI_DMA_TC)

#define HAL_SPI_SUPPORTS_ASYNC

// Requests asynchronous SPI transfer to peripheral (DMA).
void spiWriteAsync(const uint8_t *buf, uint16_t nbyte, void (*completeCallback)(void*) = nullptr, void *ud = nullptr);
void spiWriteAsync16(const uint16_t *buf, uint16_t ndata, void (*completeCallback)(void*) = nullptr, void *ud = nullptr);

// Aborts a running async SPI transfer.
void spiAsyncAbort();

// Waits for a running async SPI transfer.
void spiAsyncJoin();

// Returns true if the DMA transfer is still running.
bool spiAsyncIsRunning();

#endif // async support