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
 *
 */
#pragma once

/**
 * Anycubic Kobra Go — TriGorilla Gen V3.0.6 (HC32F460KETA)
 * Issue: https://github.com/MarlinFirmware/Marlin/issues/25266
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME        "Trigorilla Gen V3.0.6"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME   "Anycubic Kobra Go"
#endif

// LeviQ probe is fixed-mount on the Kobra Go — no servo deploy needed.

#include "pins_TRIGORILLA_V306_common.h"
