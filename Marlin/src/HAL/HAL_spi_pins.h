/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef HAL_SPI_PINS_H_
#define HAL_SPI_PINS_H_

#ifdef ARDUINO_ARCH_SAM
  #include "HAL_DUE/spi_pins.h"

#elif defined(IS_32BIT_TEENSY)
  #include "HAL_TEENSY35_36/spi_pins.h"

#elif defined(__AVR__)
  #include "HAL_AVR/spi_pins.h"

#elif defined(TARGET_LPC1768)
  #include "HAL_LPC1768/spi_pins.h"
#elif defined(__STM32F1__)
    #include "HAL_STM32F1/spi_pins.h"
#else
  #error "Unsupported Platform!"
#endif

#endif // HAL_SPI_PINS_H_
