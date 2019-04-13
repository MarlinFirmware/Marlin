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

/**
 * feature/runout.cpp - Runout sensor support
 */

#include "../inc/MarlinConfigPre.h"

#if HAS_FILAMENT_SENSOR

#include "runout.h"

FilamentMonitor runout;

bool FilamentMonitorBase::enabled = true,
     FilamentMonitorBase::filament_ran_out;

#if ENABLED(HOST_ACTION_COMMANDS)
  bool FilamentMonitorBase::host_handling;
#endif

#if HAS_DELAYED_RUNOUT
  int16_t FilamentMonitorBase::st_cnt;
  float RunoutResponseDelayed::runoutDistance;
#else
  int8_t RunoutResponseDebounced::runout_count;
#endif

#endif // HAS_FILAMENT_SENSOR
