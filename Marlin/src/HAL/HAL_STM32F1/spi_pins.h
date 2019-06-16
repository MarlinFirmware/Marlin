/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */
/**
 * Define SPI Pins: SCK, MISO, MOSI, SS
 *
 * STM32F1 Default SPI Pins
 * 
 * SPI1
 * SCK_PIN  PA5
 * MISO_PIN PA6
 * MOSI_PIN PA7
 * SS_PIN   PA4
 * 
 * SPI2
 * SCK_PIN  PB13
 * MISO_PIN PB14
 * MOSI_PIN PB15
 * SS_PIN   PB12
 * 
 * SPI3
 * SCK_PIN  PB3
 * MISO_PIN PB4
 * MOSI_PIN PB5
 * SS_PIN   PA15
 *
 * Any PIN can be used for Chip Select (SS_PIN)
 * 
 * SPI1 is enabled by default
 */

#ifndef SCK_PIN
  #define SCK_PIN  PA5
#endif
#ifndef MISO_PIN
  #define MISO_PIN PA6
#endif
#ifndef MOSI_PIN
  #define MOSI_PIN PA7
#endif
#ifndef SS_PIN
  #define SS_PIN   PA4
#endif
#undef SDSS
#define SDSS       SS_PIN

#if ENABLED(ENABLE_SPI3)
  #define SPI_DEVICE 3
#elif ENABLED(ENABLE_SPI2)
  #define SPI_DEVICE 2
#else
  #define SPI_DEVICE 1
#endif
