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

#include "../../shared/HAL_SPI.h"

#include <stdint.h>

#define MSBFIRST 1
#define SPI_MODE3 0

class SPISettings {
  public:
    SPISettings(uint32_t speed, int, int) : spi_speed(speed) {};
    uint32_t spiRate() { return spi_speed; }
  private:
    uint32_t spi_speed;
};

class SPIClass {
  public:
    void begin();
    void beginTransaction(SPISettings);
    void endTransaction() {};
    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
};

extern SPIClass SPI;
