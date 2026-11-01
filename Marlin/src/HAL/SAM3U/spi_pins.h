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

/**
 * SPI pins for Atmel SAM3U (AT91SAM3U4E)
 *
 * The chip's SPI peripheral has one fixed set of data/clock pads:
 *
 *   MISO = PA13    MOSI = PA14    SPCK = PA15    NPCS0 = PA16
 *
 * On the 4pi this bus carries the AD5206 digital potentiometer that sets the
 * stepper driver reference voltages; its chip select is NPCS0 (see
 * DIGIPOTSS_PIN in the board's pins file).
 *
 * NOTE this header is included *after* pins.h in the config cascade, so it can
 * only react to SD_SS_PIN - it cannot supply a default for it. By the time we
 * get here pins_postprocess.h has already set SD_SS_PIN to -1 if no board or
 * user setting provided one. Chip selects therefore belong in the pins file or
 * the user's configuration, never here.
 *
 * The 4pi's own SD socket is on HSMCI, not SPI, so there is deliberately no
 * default SD chip select for this board. To use an external SPI card reader on
 * the expansion header, set SD_SS_PIN to one of EXP_CS1_PIN / EXP_CS2_PIN /
 * EXP_CS3_PIN.
 */

#include "include/pinmapping.h"

// The bus itself is fixed in silicon and is safe to define unconditionally.
#ifndef SD_SCK_PIN
  #define SD_SCK_PIN  PA15
#endif
#ifndef SD_MISO_PIN
  #define SD_MISO_PIN PA13
#endif
#ifndef SD_MOSI_PIN
  #define SD_MOSI_PIN PA14
#endif

#define SCK_PIN       SD_SCK_PIN
#define MISO_PIN      SD_MISO_PIN
#define MOSI_PIN      SD_MOSI_PIN

// This HAL bit-bangs the bus (see HAL_SPI.cpp)
#ifndef SOFTWARE_SPI
  #define SOFTWARE_SPI
#endif
