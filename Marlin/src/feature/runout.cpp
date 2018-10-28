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

/**
 * feature/runout.cpp - Runout sensor support
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(FILAMENT_RUNOUT_SENSOR)

#include "runout.h"

FilamentRunoutSensor runout;

bool FilamentSensorBase::enabled = true,
     FilamentSensorBase::filament_ran_out; // = false

void FilamentSensorTypeBase::filament_present(const uint8_t extruder) {
  runout.filament_present(extruder);
}

uint8_t FilamentSensorTypeEncoder::motion_detected,
        FilamentSensorTypeEncoder::old_state; // = 0

#if FILAMENT_RUNOUT_DISTANCE_MM > 0
  float RunoutResponseDelayed::runout_distance_mm = FILAMENT_RUNOUT_DISTANCE_MM;
  int32_t RunoutResponseDelayed::steps_since_detection[EXTRUDERS];
#else
  uint8_t RunoutResponseDebounced::runout_count; // = 0
#endif

#endif // FILAMENT_RUNOUT_SENSOR
