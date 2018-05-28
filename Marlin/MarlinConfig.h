/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef MARLIN_CONFIG_H
#define MARLIN_CONFIG_H

#include "boards.h"
#include "macros.h"
#include "Version.h"
#include "Configuration.h"
#include "Conditionals_LCD.h"
#include "Configuration_adv.h"

#if USE_MARLINSERIAL
  #define HardwareSerial_h // trick to disable the standard HWserial
#endif

#include "types.h"
#include "HAL.h"
#include "pins.h"
#include "Conditionals_post.h"
#include "SanityCheck.h"
#include "enum.h"
#include "language.h"
#include "utility.h"
#include "serial.h"

#endif // MARLIN_CONFIG_H
