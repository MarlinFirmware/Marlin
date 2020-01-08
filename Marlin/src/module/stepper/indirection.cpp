/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * stepper/indirection.cpp
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "../../inc/MarlinConfig.h"
#include "indirection.h"

void restore_stepper_drivers() {
  #if HAS_TRINAMIC
    restore_trinamic_drivers();
  #endif
}

void reset_stepper_drivers() {
  #if HAS_DRIVER(TMC26X)
    tmc26x_init_to_defaults();
  #endif

  #if HAS_L64XX
    L64xxManager.init_to_defaults();
  #endif

  #if HAS_TRINAMIC
    reset_trinamic_drivers();
  #endif
}
