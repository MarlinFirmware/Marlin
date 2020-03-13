/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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

#ifdef ADAFRUIT_GRAND_CENTRAL_M4

 /*
  * AGCM4 Default SPI Pins
  *
  *         SS    SCK   MISO   MOSI
  *       +-------------------------+
  *  SPI  | 53    52     50     51  |
  *  SPI1 | 83    81     80     82  |
  *       +-------------------------+
  * Any pin can be used for Chip Select (SS_PIN)
  */
  #ifndef SCK_PIN
    #define SCK_PIN       52
  #endif
  #ifndef MISO_PIN
    #define MISO_PIN      50
  #endif
  #ifndef MOSI_PIN
    #define MOSI_PIN      51
  #endif
  #ifndef SDSS
    #define SDSS          53
  #endif

#else

  #error "Unsupported board!"

#endif

#define SS_PIN        SDSS
