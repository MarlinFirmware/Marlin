/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * Define SPI Pins: SCK, MISO, MOSI, SS
 */
// All pins are defined in pins_NUCELO_STMH743ZI.h
//#define SCK_PIN   PA5
//#define MISO_PIN  PA6
//#define MOSI_PIN  PB5
#define SS_PIN    PA8

//SPIClass SPI_6(SPI6, SPI6_MOSI_PIN, SPI6_MISO_PIN, SPI6_SCK_PIN)
#define SPI6          PD11
#define SPI6_MOSI_PIN PD10
#define SPI6_MISO_PIN PD9
#define SPI6_SCK_PIN  PD8
