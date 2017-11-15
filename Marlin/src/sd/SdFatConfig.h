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
 * SdFatConfig.h
 * Arduino SdFat Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */

#ifndef _SDFATCONFIG_H_
#define _SDFATCONFIG_H_

#include "../inc/MarlinConfig.h"


/**
 * To use multiple SD cards set USE_MULTIPLE_CARDS nonzero.
 *
 * Using multiple cards costs 400 - 500  bytes of flash.
 *
 * Each card requires about 550 bytes of SRAM so use of a Mega is recommended.
 */
#define USE_MULTIPLE_CARDS 0

/**
 * Call flush for endl if ENDL_CALLS_FLUSH is nonzero
 *
 * The standard for iostreams is to call flush.  This is very costly for
 * SdFat.  Each call to flush causes 2048 bytes of I/O to the SD.
 *
 * SdFat has a single 512 byte buffer for SD I/O so it must write the current
 * data block to the SD, read the directory block from the SD, update the
 * directory entry, write the directory block to the SD and read the data
 * block back into the buffer.
 *
 * The SD flash memory controller is not designed for this many rewrites
 * so performance may be reduced by more than a factor of 100.
 *
 * If ENDL_CALLS_FLUSH is zero, you must call flush and/or close to force
 * all data to be written to the SD.
 */
#define ENDL_CALLS_FLUSH 0

/**
 * Allow use of deprecated functions if ALLOW_DEPRECATED_FUNCTIONS is nonzero
 */
#define ALLOW_DEPRECATED_FUNCTIONS 1

/**
 * Allow FAT12 volumes if FAT12_SUPPORT is nonzero.
 * FAT12 has not been well tested.
 */
#define FAT12_SUPPORT 0

/**
 * SPI init rate for SD initialization commands. Must be 5 (F_CPU/64)
 * or 6 (F_CPU/128).
 */
#define SPI_SD_INIT_RATE 5

/**
 * Set the SS pin high for hardware SPI.  If SS is chip select for another SPI
 * device this will disable that device during the SD init phase.
 */
#define SET_SPI_SS_HIGH 1

/**
 * Define MEGA_SOFT_SPI nonzero to use software SPI on Mega Arduinos.
 * Pins used are SS 10, MOSI 11, MISO 12, and SCK 13.
 *
 * MEGA_SOFT_SPI allows an unmodified Adafruit GPS Shield to be used
 * on Mega Arduinos.  Software SPI works well with GPS Shield V1.1
 * but many SD cards will fail with GPS Shield V1.0.
 */
#define MEGA_SOFT_SPI 0

// Set USE_SOFTWARE_SPI nonzero to ALWAYS use Software SPI.
#define USE_SOFTWARE_SPI 0

// Define software SPI pins so Mega can use unmodified 168/328 shields
#define SOFT_SPI_CS_PIN   10 // Software SPI chip select pin for the SD
#define SOFT_SPI_MOSI_PIN 11 // Software SPI Master Out Slave In pin
#define SOFT_SPI_MISO_PIN 12 // Software SPI Master In Slave Out pin
#define SOFT_SPI_SCK_PIN  13 // Software SPI Clock pin

/**
 * The __cxa_pure_virtual function is an error handler that is invoked when
 * a pure virtual function is called.
 */
#define USE_CXA_PURE_VIRTUAL 1

/**
 * Defines for 8.3 and long (vfat) filenames
 */

#define FILENAME_LENGTH 13 // Number of UTF-16 characters per entry

// Total bytes needed to store a single long filename
#define LONG_FILENAME_LENGTH (FILENAME_LENGTH * MAX_VFAT_ENTRIES + 1)

#endif // _SDFATCONFIG_H_
