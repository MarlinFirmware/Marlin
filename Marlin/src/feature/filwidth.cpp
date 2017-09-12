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

#include "../inc/MarlinConfig.h"

#if ENABLED(FILAMENT_WIDTH_SENSOR)

#include "filwidth.h"

bool filament_sensor = false;                                 // M405/M406 turns filament sensor control ON/OFF.
float filament_width_nominal = DEFAULT_NOMINAL_FILAMENT_DIA,  // Nominal filament width. Change with M404.
      filament_width_meas = DEFAULT_MEASURED_FILAMENT_DIA;    // Measured filament diameter
uint8_t meas_delay_cm = MEASUREMENT_DELAY_CM,                 // Distance delay setting
        measurement_delay[MAX_MEASUREMENT_DELAY + 1];         // Ring buffer to delayed measurement. Store extruder factor after subtracting 100
int8_t filwidth_delay_index[2] = { 0, -1 };                   // Indexes into ring buffer

#endif // FILAMENT_WIDTH_SENSOR
