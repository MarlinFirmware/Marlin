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


#ifndef SPI_PINS_LPC1768_H
#define SPI_PINS_LPC1768_H

#define SOFTWARE_SPI
/** onboard SD card */
//#define SCK_PIN           P0_7
//#define MISO_PIN          P0_8
//#define MOSI_PIN          P0_9
//#define SS_PIN            P0_6
/** external */
#define SCK_PIN           52 //P0_15
#define MISO_PIN          50 //P0_17
#define MOSI_PIN          51 //P0_18
#define SS_PIN            53 //P1_23
#endif /* SPI_PINS_LPC1768_H */
