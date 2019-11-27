/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

//
// SPI device access definitions
//
#define SPIBUS_MOSI     0
#define SPIBUS_MISO     1
#define SPIBUS_CLCK     2
#define SPIBUS_MODE     3

#define SPIDEV_TYPE     0 //device type
#define SPIDEV_BUS      1 //attached to bus
#define SPIDEV_CS       2 //selection
#define SPIDEV_SW       3 //detection
#define SPIDEV_DLV      4 //level when detected

#define DEVTYPE_SD      0
#define DEVTYPE_DRIVER  1
#define DEVTYPE_DISPLAY 2
#define DEVTYPE_SENSOR  3
#define DEVTYPE_EEPROM  4

#define IS_DEV_SD(X)     SPI_Devices[X][SPIDEV_TYPE] == DEVTYPE_SD
#define IS_DEV_EEPROM(X) SPI_Devices[X][SPIDEV_TYPE] == DEVTYPE_EEPROM

#define BUS_OF_DEV(X)   SPI_Devices[X][SPIDEV_BUS]
#define CS_OF_DEV(X)    SPI_Devices[X][SPIDEV_CS]
#define SW_OF_SD(X)     SPI_Devices[X][SPIDEV_SW]
#define DLV_OF_SD(X)    SPI_Devices[X][SPIDEV_DLV]

//
// Standard SPI functions
//

bool spiInitialized(uint8_t bus_num);

// Initialize and configure SPI bus for specified clock speed
void spiInit(uint8_t bus_num, uint8_t spiRate);

// Write single byte to SPI bus, regardless of device
void spiSend(uint8_t bus_num, uint8_t b);

// Read a single byte from SPI bus, regardless of device
uint8_t spiRec(uint8_t bus_num);

// Read from SPI bus into buffer
void spiRead(uint8_t bus_num, uint8_t* buf, uint16_t nbyte);

// Write token and then write from 512 byte buffer to SPI bus (for SD card)
void spiSendBlock(uint8_t bus_num, uint8_t token, const uint8_t* buf);


// Write single byte to SPI device
void spiSendDevice(uint8_t dev_num, uint8_t b);

// Read single byte from SPI device
uint8_t spiRecDevice(uint8_t dev_num);

// Read from SPI device into buffer
void spiReadDevice(uint8_t dev_num, uint8_t* buf, uint16_t nbyte);

// Write token and then write from 512 byte buffer to SPI device (for SD card)
void spiSendBlockDevice(uint8_t dev_num, uint8_t token, const uint8_t* buf);

// Begin SPI transaction, set clock, bit order, data mode
void spiBeginTransaction(uint8_t dev_num, uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode);

//
// Extended SPI functions taking a channel number (Hardware SPI only)
//

// Write single byte to specified SPI channel
void spiSend(uint8_t dev_num, uint32_t chan, byte b);

// Write buffer to specified SPI channel
void spiSend(uint8_t dev_num, uint32_t chan, const uint8_t* buf, size_t n);

// Read single byte from specified SPI channel
uint8_t spiRec(uint8_t dev_num, uint32_t chan);
