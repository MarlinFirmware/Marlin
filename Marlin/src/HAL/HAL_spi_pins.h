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


#ifndef HAL_SPI_PINS_H_
#define HAL_SPI_PINS_H_

#ifdef ARDUINO_ARCH_SAM

  #include "HAL_DUE/spi_pins.h"

#elif defined(ARDUINO_ARCH_AVR)

  #include "HAL_AVR/spi_pins.h"

#else

  #error Unsupported Platform!

#endif

#endif /* HAL_SPI_PINS_H_ */
