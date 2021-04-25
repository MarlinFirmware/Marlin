/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

/**
 * STM32F1 Default SPI Pins
 *
 *         SS     SCK     MISO    MOSI
 *       +-----------------------------+
 *  SPI1 | PA4    PA5     PA6     PA7  |
 *  SPI2 | PB12   PB13    PB14    PB15 |
 *  SPI3 | PA15   PB3     PB4     PB5  |
 *       +-----------------------------+
 * Any pin can be used for Chip Select (SD_SS_PIN)
 * SPI1 is enabled by default
 */
#ifndef SD_SCK_PIN
  #define SD_SCK_PIN  PA5
#endif
#ifndef SD_MISO_PIN
  #define SD_MISO_PIN PA6
#endif
#ifndef SD_MOSI_PIN
  #define SD_MOSI_PIN PA7
#endif
#ifndef SD_SS_PIN
  #define SD_SS_PIN   PA4
#endif
#undef SDSS
#define SDSS    SD_SS_PIN

#ifndef SPI_DEVICE
  #define SPI_DEVICE 1
#endif
