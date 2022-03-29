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

#define BOARD_INFO_NAME "Anet ET4P 1.x"

//
// TMC2208 Configuration_adv defaults for Anet ET4P-MB_V1.x
//
#if !AXIS_DRIVER_TYPE_X(TMC2208_STANDALONE) || !AXIS_DRIVER_TYPE_Y(TMC2208_STANDALONE) || !AXIS_DRIVER_TYPE_Z(TMC2208_STANDALONE) || !AXIS_DRIVER_TYPE_E0(TMC2208_STANDALONE)
  #error "ANET_ET4P requires ([XYZ]|E0)_DRIVER_TYPE set to TMC2208_STANDALONE."
#endif

#include "pins_ANET_ET4.h"
