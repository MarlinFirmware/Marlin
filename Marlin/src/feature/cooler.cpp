/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#if HAS_COOLER

#include "cooler.h"
Cooler cooler;

uint16_t Cooler::flowrate;        // Flow meter reading in liters, 0 will result in shutdown if equiped
uint8_t Cooler::mode = 0;         // 0 = CO2 Liquid cooling, 1 = Laser Diode TEC Heatsink Cooling
uint16_t Cooler::capacity;        // Cooling capacity in watts
uint16_t Cooler::load;            // Cooling load in watts
bool Cooler::flowmeter = false;
bool Cooler::state = false;       // on = true, off = false

#endif
