/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if HOTENDS > 2 || E_STEPPERS > 1
  #error "Creality V252 supports up to 2 hotends and 1 E stepper."
#endif

#define BOARD_INFO_NAME "Creality3D V252"

#if !AXIS_DRIVER_TYPE_X(TMC2208_STANDALONE) || !AXIS_DRIVER_TYPE_Y(TMC2208_STANDALONE) || !AXIS_DRIVER_TYPE_Z(TMC2208_STANDALONE) || !AXIS_DRIVER_TYPE_E0(TMC2208_STANDALONE)
  #error "Creality V252 requires TMC2208_STANDALONE for X,Y,Z and E."
#endif

//
// Based on RAMPS CREALITY
//
#include "pins_RAMPS_CREALITY.h"
