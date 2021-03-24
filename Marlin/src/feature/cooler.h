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

#define _MSG_COOLER(M)  MSG_COOLER_##M
#define MSG_COOLER(M)   _MSG_COOLER(M)
#define FLOW_RATE_MLPP   0.170068   // ml per pulse

// Cooling device

class Cooler {
public:
  static float flowrate;                      // Flow meter reading in liters, 0 will result in shutdown if equiped
  static volatile uint16_t flowpulses;        // Flowmeter irq pulse count
  static millis_t flow_calc_time;             // Elasped time duration used for calculation of the flowrate
  static uint8_t mode;                        // 0 = CO2 Liquid cooling, 1 = Laser Diode TEC Heatsink Cooling
  static uint16_t capacity;                   // Cooling capacity in watts
  static uint16_t load;                       // Cooling load in watts
  static bool flowmeter;                      // Enabled = True, Disabled = False
  static bool state;                          // On = True, Off = False
  static bool flowmeter_safety;               // On = True, Off = False
  static bool fault;                          // Cooler is in a fault state On = True, Off = False
  static bool is_enabled()                    { return state; }
  static bool flowsaftey_is_enabled()         { return flowmeter_safety; }
  static void enable()                        { state = true; }
  static void disable()                       { state = false; }
  static void flowsafety_enable()             { flowmeter_safety = true; }
  static void flowsafety_disable()            { flowmeter_safety = false; }
  static void set_mode(const uint8_t m)       { mode = m; }
  static void set_flowmeter(const bool sflag) { flowmeter = sflag; }

  #if ENABLED(HAS_FLOWMETER) && PIN_EXISTS(FLOWMETER)
    static void flowmeter_ISR()               { flowpulses ++; }                      // Keep minimal,all we need here is the count
    static void flowmeter_enable() { 
      attachInterrupt(digitalPinToInterrupt(FLOWMETER_PIN), flowmeter_ISR, RISING);
    }
    static void flowmeter_disable() {
      detachInterrupt(digitalPinToInterrupt(FLOWMETER_PIN));
    }
    static void calc_flowrate() {
        flowrate = flowpulses * 60 * (1000 / FLOWMETER_INTERVAL) * FLOW_RATE_MLPP;    // Total flow until now
        flowpulses = 0;
    }
  #endif
};

extern Cooler cooler;
