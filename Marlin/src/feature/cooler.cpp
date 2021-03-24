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

#if HAS_COOLER || HAS_FLOWMETER

#include "cooler.h"
Cooler cooler;

float Cooler::flowrate;                 // Flow meter reading in liters per minute, if less than 1.5Lpm, laser will be shut off with saftey true
volatile uint16_t Cooler::flowpulses;   // flowmeter irq pulse count
millis_t Cooler::flow_calc_time = FLOWMETER_INTERVAL; 
uint8_t Cooler::mode = 0;               // 0 = CO2 Liquid cooling, 1 = Laser Diode TEC Heatsink Cooling
uint16_t Cooler::capacity;              // Cooling capacity in watts
uint16_t Cooler::load;                  // Cooling load in watts
bool Cooler::flowmeter = false;
bool Cooler::state = false;             // on = true, off = false
bool Cooler::fault = false;             // Cooler is in a fault state On = True, Off = False
bool Cooler::flowmeter_safety = true;   // on = true, off = false

#endif
