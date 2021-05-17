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
#pragma once

#include <stdint.h>

#define _MSG_COOLER(M) MSG_COOLER_##M
#define MSG_COOLER(M) _MSG_COOLER(M)

// Cooling device

class Cooler {
public:
  static uint16_t flowrate;        // Flow meter reading in liters, 0 will result in shutdown if equiped
  static uint8_t mode;             // 0 = CO2 Liquid cooling, 1 = Laser Diode TEC Heatsink Cooling
  static uint16_t capacity;        // Cooling capacity in watts
  static uint16_t load;            // Cooling load in watts
  static bool flowmeter;
  static bool state;               // on = true, off = false

  static bool is_enabled()                    { return state; }
  static void enable()                        { state = true; }
  static void disable()                       { state = false; }
  static void set_mode(const uint8_t m)       { mode = m; }
  static void set_flowmeter(const bool sflag) { flowmeter = sflag; }
  static uint16_t get_flowrate()              { return flowrate; }
  static void update_flowrate(uint16_t flow)  { flowrate = flow; }
  //static void init() { set_state(false); }
};

extern Cooler cooler;
