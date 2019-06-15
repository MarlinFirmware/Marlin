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
 * Any PIN can be used for Chip Select (SS)
 * 
 * SPI1 is enabled by default
 */
#if ENABLED(ENABLE_SPI3)
  #define SPI_DEVICE 3
  #define SCK_PIN   BOARD_SPI3_SCK_PIN
  #define MISO_PIN  BOARD_SPI3_MISO_PIN
  #define MOSI_PIN  BOARD_SPI3_MOSI_PIN
  #define SS_PIN    BOARD_SPI3_NSS_PIN
#elif ENABLED(ENABLE_SPI2)
  #define SPI_DEVICE 2
  #define SCK_PIN   BOARD_SPI2_SCK_PIN
  #define MISO_PIN  BOARD_SPI2_MISO_PIN
  #define MOSI_PIN  BOARD_SPI2_MOSI_PIN
  #define SS_PIN    BOARD_SPI2_NSS_PIN
#else
  #define SPI_DEVICE 1
  #define SCK_PIN   BOARD_SPI1_SCK_PIN
  #define MISO_PIN  BOARD_SPI1_MISO_PIN
  #define MOSI_PIN  BOARD_SPI1_MOSI_PIN
  #define SS_PIN    BOARD_SPI1_NSS_PIN
#endif