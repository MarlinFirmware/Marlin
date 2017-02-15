/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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


#ifndef SPI_PINS_H_
#define SPI_PINS_H_

/**
 * Define SPI Pins: SCK, MISO, MOSI, SS
 *
 * Available chip select pins for HW SPI are 4 10 52 77
 */
#if (SDSS == 4) || (SDSS == 10) || (SDSS == 52) || (SDSS == 77)
  #if (SDSS == 4)
    #define SPI_PIN         87
    #define SPI_CHAN         1
  #elif (SDSS == 10)
    #define SPI_PIN         77
    #define SPI_CHAN         0
  #elif (SDSS == 52)
    #define SPI_PIN         86
    #define SPI_CHAN         2
  #else
    #define SPI_PIN         77
    #define SPI_CHAN         0
  #endif
  #define SCK_PIN           76
  #define MISO_PIN          74
  #define MOSI_PIN          75
#else
  // defaults
  #define SOFTWARE_SPI
  #define SCK_PIN           52
  #define MISO_PIN          50
  #define MOSI_PIN          51
#endif

#define SS_PIN            SDSS // A.28, A.29, B.21, C.26, C.29

#endif /* SPI_PINS_H_ */
