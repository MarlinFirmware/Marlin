/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#define COOLER_CLASS
#define _MSG_COOLER(M) MSG_COOLER_##M
#define MSG_COOLER(M) _MSG_COOLER(M)

// Cooling device properties
  typedef struct CoolerProp {
  uint16_t flowrate;             // Flow meter reading in liters, 0 will result in shutdown if equiped 
  uint8_t mode = 0;             // 0 = CO2 Liquid cooling, 1 = Laser Diode TEC Heatsink Cooling 
  uint16_t capacity;            // Cooling capacity in watts
  uint16_t load;                // Cooling load in watts
  bool flowmeter = false;
  bool state = false ;          // on = true, off = false
  } cooler_prop_t;

class Cooler {

public:

  cooler_prop_t cooling;
  bool is_enabled() { return cooling.state;};
  void enable() { cooling.state = true;};
  void disable() { cooling.state = false;};
  void set_mode(uint8_t mode) { cooling.mode = mode;};
  void set_flowmeter(const bool sflag) { cooling.flowmeter = sflag;};
  uint16_t get_flowrate() { return cooling.flowrate;};
  void update_flowrate(uint16_t flow) { cooling.flowrate = flow;};
  //static void init_cooler(void);



};
