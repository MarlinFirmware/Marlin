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

class HAL_spi_L6470 {
  public:
    uint8_t L6470_SpiTransfer_Mode_0(uint8_t b);
    uint8_t L6470_SpiTransfer_Mode_3(uint8_t b);
    uint8_t L6470_transfer(uint8_t data, int16_t ss_pin, const uint8_t chain_position);
    void L6470_transfer(uint8_t L6470_buf[], const uint8_t length);
    void L6470_spi_init();
  private:
};

extern HAL_spi_L6470 hal_spi_l6470;