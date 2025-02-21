/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../config/DGUS_Addr.h"

#define VPFLAG_NONE        0
#define VPFLAG_AUTOUPLOAD  (1U << 0) // Upload on every DGUS update
#define VPFLAG_RXSTRING    (1U << 1) // Treat the received data as a string (terminated with 0xFFFF)

struct DGUS_VP {
  DGUS_Addr addr;
  uint8_t   size;
  uint8_t   flags;
  void      *extra;

  // Callback that will be called if the display modified the value.
  // nullptr makes it readonly for the display.
  void (*rx_handler)(DGUS_VP &, void *);
  void (*tx_handler)(DGUS_VP &);
};
