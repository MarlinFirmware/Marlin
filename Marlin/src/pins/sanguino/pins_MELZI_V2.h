/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Melzi V2.0 as found at https://www.reprap.org/wiki/Melzi
 */

#define BOARD_INFO_NAME "Melzi V2"

#if HAS_MARLINUI_U8GLIB
  #ifndef BOARD_ST7920_DELAY_1
    #define BOARD_ST7920_DELAY_1 DELAY_NS(0)
  #endif
  #ifndef BOARD_ST7920_DELAY_2
    #define BOARD_ST7920_DELAY_2 DELAY_NS(188)
  #endif
  #ifndef BOARD_ST7920_DELAY_3
    #define BOARD_ST7920_DELAY_3 DELAY_NS(0)
  #endif
#endif

#include "pins_MELZI.h"
