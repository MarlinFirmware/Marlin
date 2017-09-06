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

#ifndef _TMC2130_H_
#define _TMC2130_H_

#include <TMC2130Stepper.h>

#include "../inc/MarlinConfig.h"

extern bool auto_current_control;

void tmc2130_checkOverTemp(void);

#if ENABLED(SENSORLESS_HOMING)
  void tmc2130_sensorless_homing(TMC2130Stepper &st, bool enable=true);
#endif

#endif // _TMC2130_H_
