/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

/**
 * SPI pin definitions for AT32F403
 * AT32 uses standard SPI peripheral naming: SPI1, SPI2, SPI3
 */

#ifdef ARDUINO_ARCH_AT32

// Default SPI pins (can be overridden by board pins files)
#ifndef SPI_SCK_PIN
  #define SPI_SCK_PIN PC3
#endif

#ifndef SPI_MISO_PIN
  #define SPI_MISO_PIN PC4
#endif

#ifndef SPI_MOSI_PIN
  #define SPI_MOSI_PIN PC5
#endif

#ifndef SPI_SS_PIN
  #define SPI_SS_PIN PA4
#endif

#endif // ARDUINO_ARCH_AT32
