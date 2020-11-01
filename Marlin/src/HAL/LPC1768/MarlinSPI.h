/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 *
 */
#pragma once

#include <SPI.h>

/**
 * Marlin currently needs 3 SPI class
 *
 * SPIClass:
 *   This class normally is provide by the frameworks and have semi default interface.
 *   We need this, because some libs reference it globally.
 *
 * SPISettings:
 *  Hold spi configs for the SPIClass. Same problem about: lib may reference it globally.
 *
 * MarlinSPI:
 *   As those two classes are often provided by frameworks, we cannot alter it, to add some
 * useful methods for marlin.
 *
 * This is the reason, we need MarlinSPI. It provide the default SPIClass interface, plus some
 * marlin goods (easy inteface for SPI DMA transfer)
 *
 */

using MarlinSPI = SPIClass;
