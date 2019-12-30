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
 *
 *  On AVR, actual speed is F_CPU/2^(1 + index).
 *  On other platforms, speed should be in range given above where possible.
 */

#define SPI_FULL_SPEED      0   // Set SCK to max rate
#define SPI_HALF_SPEED      1   // Set SCK rate to half of max rate
#define SPI_QUARTER_SPEED   2   // Set SCK rate to quarter of max rate
#define SPI_EIGHTH_SPEED    3   // Set SCK rate to 1/8 of max rate
#define SPI_SIXTEENTH_SPEED 4   // Set SCK rate to 1/16 of max rate
#define SPI_SPEED_5         5   // Set SCK rate to 250 - 312 kHz
#define SPI_SPEED_6         6   // Set SCK rate to 125 - 156 kHz or lowest value possible for platform

#define SPI_LSB     0   //LSB first
#define SPI_MSB     1   //MSB first
#define SPI_PLO     0   //Low polarity
#define SPI_PHI     1   //High polarity
#define SPI_LTS     0   //Latch then shift
#define SPI_STL     1   //Shift then latch

//SPI device types
#define DEVTYPE_SD      0
#define DEVTYPE_DRIVER  1
#define DEVTYPE_DISPLAY 2
#define DEVTYPE_SENSOR  3
#define DEVTYPE_EEPROM  4

/**
 * Index accessors
 */

// SPI bus
#define SPIBUS_MOSI     0
#define SPIBUS_MISO     1
#define SPIBUS_CLCK     2
#define SPIBUS_MODE     3 //default mode (calls to bus, not directed to a device. Temporary for SD card, will be removed)
#define SPIBUS_BITO     4 //default bit order (calls to bus, not directed to a device. Temporary for SD card, will be removed)
#define SPIBUS_DSPD     5 //default speed (calls to bus, not directed to a device. Temporary for SD card, will be removed)

//Common SPI device properties
#define SPIDEV_TYPE     0 //device type
#define SPIDEV_BUS      1 //attached to bus
#define SPIDEV_CPOL     2 //polarity
#define SPIDEV_CPHA     3 //phase
#define SPIDEV_BITO     4 //bit order
#define SPIDEV_SPD      5 //speed
#define SPIDEV_CS       6 //selection

//SPI device properties (by type)
#define SPIDEV_SW       7 //detection
#define SPIDEV_DLV      8 //level when detected
#define SPIDEV_DT       7 //driver type
#define SPIDEV_DI       8 //driver index

//SPI Device Driver types
#define DRIVER_AXIS     0
#define DRIVER_EXTRUDER 1

#define IS_DEV_SD(X)     SPI_Devices[X][SPIDEV_TYPE] == DEVTYPE_SD
#define IS_DEV_EEPROM(X) SPI_Devices[X][SPIDEV_TYPE] == DEVTYPE_EEPROM
#define IS_DEV_DRIVER(X) SPI_Devices[X][SPIDEV_TYPE] == DEVTYPE_DRIVER

#define BUS_OF_DEV(X)       SPI_Devices[X][SPIDEV_BUS]
#define CS_OF_DEV(X)        SPI_Devices[X][SPIDEV_CS]
#define CPOL_OF_DEV(X)      SPI_Devices[X][SPIDEV_CPOL]
#define CPHA_OF_DEV(X)      SPI_Devices[X][SPIDEV_CPHA]
#define BITO_OF_DEV(X)      SPI_Devices[X][SPIDEV_BITO]
#define SPD_OF_DEV(X)       SPI_Devices[X][SPIDEV_SPD]

#define TYPE_OF_DRIVER(X)   SPI_Devices[X][SPIDEV_DT]
#define AXIS_OF_DRIVER(X)   SPI_Devices[X][SPIDEV_DI]

//SD card
#define SW_OF_SD(X)     SPI_Devices[X][SPIDEV_SW]
#define DLV_OF_SD(X)    SPI_Devices[X][SPIDEV_DLV]

//Driver
#define TYPE_OF_DRIVER(X)   SPI_Devices[X][SPIDEV_DT]
#define IDX_OF_DRIVER(X)    SPI_Devices[X][SPIDEV_DI]

//
// Calls directed to the whole bus
//

bool spiInitialized(uint8_t bus_num);

// Initialize and configure SPI bus for specified clock speed and no CRC
void spiBusInit(uint8_t bus_num, uint8_t spiRate);

// Write single byte to SPI bus, regardless of device
void spiBusSend(uint8_t bus_num, uint8_t b);

// Read a single byte from SPI bus, regardless of device
uint8_t spiBusRec(uint8_t bus_num);

// Read from SPI bus into buffer
void spiBusRead(uint8_t bus_num, uint8_t* buf, uint16_t count);

// Write from buffer to SPI bus
void spiBusWrite(uint8_t bus_num, const uint8_t* buf, uint16_t count);

//
// Calls directed to a device
//

// Write single byte to SPI device
void spiDevSend(uint8_t dev_num, uint8_t b);
// Read single byte from SPI device
uint8_t spiDevRec(uint8_t dev_num);

// Read from SPI device into buffer
void spiDevRead(uint8_t dev_num, uint8_t* buf, uint16_t count);
// Write from buffer to SPI device
void spiDevWrite(uint8_t dev_num, const uint8_t* buf, uint16_t count);

// Read from SPI device into buffer and return true if crc is OK
uint16_t spiDevReadCRC16(uint8_t dev_num, uint16_t* buf, uint16_t count);
// Write from buffer to SPI bus and returns HW calculated CRC16 on data
uint16_t spiDevWriteCRC16(uint8_t dev_num, const uint16_t* buf, uint16_t count);

//
// Extended SPI functions taking a channel number (Hardware SPI only)
//

// Write single byte to specified SPI channel
void spiChnSend(uint32_t chan, byte b);

// Write buffer to specified SPI channel
void spiChnSend(uint32_t chan, const uint8_t* buf, size_t n);

// Read single byte from specified SPI channel
uint8_t spiChnRec(uint32_t chan);
