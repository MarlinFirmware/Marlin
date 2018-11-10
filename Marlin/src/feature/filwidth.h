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
#pragma once

#include "../inc/MarlinConfig.h"

extern bool filament_sensor;                                // M405/M406 turns filament sensor control ON/OFF.
extern float filament_width_nominal,                        // Nominal filament width. Change with M404.
             filament_width_meas;                           // Measured filament diameter
extern uint8_t meas_delay_cm;                               // Distance delay setting
extern int8_t measurement_delay[MAX_MEASUREMENT_DELAY + 1], // Ring buffer to delayed measurement. Store extruder factor after subtracting 100
              filwidth_delay_index[2];                      // Indexes into ring buffer
