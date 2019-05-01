/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/*****************************************************************
 * GT2560 V3.0 pin assignment (for Mecreator 2)
 *****************************************************************/

#define BOARD_NAME "GT2560 V3.0 (MC2)"

#include "pins_GT2560_V3.h"

#undef X_MIN_PIN
#define X_MIN_PIN          22

#undef X_MAX_PIN
#define X_MAX_PIN          24

#undef Y_MIN_PIN
#define Y_MIN_PIN          26

#undef Y_MAX_PIN
#define Y_MAX_PIN          28
