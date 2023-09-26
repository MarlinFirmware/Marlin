/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
CONFIG_VERSION(02020000)

//
// Heated Chamber options
//
#if TEMP_SENSOR_CHAMBER

  #if DISABLED(PIDTEMPCHAMBER)
    #define CHAMBER_CHECK_INTERVAL 5000   // (ms) Interval between checks in bang-bang control
    #if ENABLED(CHAMBER_LIMIT_SWITCHING)
      #define CHAMBER_HYSTERESIS 2        // (°C) Only set the relevant heater state when ABS(T-target) > CHAMBER_HYSTERESIS
    #endif
  #endif

  //#define HEATER_CHAMBER_PIN      P2_04   // Required heater on/off pin (example: SKR 1.4 Turbo HE1 plug)
  //#define HEATER_CHAMBER_INVERTING false
  //#define FAN1_PIN                   -1   // Remove the fan signal on pin P2_04 (example: SKR 1.4 Turbo HE1 plug)

  //#define CHAMBER_FAN               // Enable a fan on the chamber
  #if ENABLED(CHAMBER_FAN)
    //#define CHAMBER_FAN_INDEX   2   // Index of a fan to repurpose as the chamber fan. (Default: first unused fan)
    #define CHAMBER_FAN_MODE      2   // Fan control mode: 0=Static; 1=Linear increase when temp is higher than target; 2=V-shaped curve; 3=similar to 1 but fan is always on.
    #if CHAMBER_FAN_MODE == 0
      #define CHAMBER_FAN_BASE  255   // Chamber fan PWM (0-255)
    #elif CHAMBER_FAN_MODE == 1
      #define CHAMBER_FAN_BASE  128   // Base chamber fan PWM (0-255); turns on when chamber temperature is above the target
      #define CHAMBER_FAN_FACTOR 25   // PWM increase per °C above target
    #elif CHAMBER_FAN_MODE == 2
      #define CHAMBER_FAN_BASE  128   // Minimum chamber fan PWM (0-255)
      #define CHAMBER_FAN_FACTOR 25   // PWM increase per °C difference from target
    #elif CHAMBER_FAN_MODE == 3
      #define CHAMBER_FAN_BASE  128   // Base chamber fan PWM (0-255)
      #define CHAMBER_FAN_FACTOR 25   // PWM increase per °C above target
    #endif
  #endif

  //#define CHAMBER_VENT              // Enable a servo-controlled vent on the chamber
  #if ENABLED(CHAMBER_VENT)
    #define CHAMBER_VENT_SERVO_NR  1  // Index of the vent servo
    #define HIGH_EXCESS_HEAT_LIMIT 5  // How much above target temp to consider there is excess heat in the chamber
    #define LOW_EXCESS_HEAT_LIMIT  3
    #define MIN_COOLING_SLOPE_TIME_CHAMBER_VENT 20
    #define MIN_COOLING_SLOPE_DEG_CHAMBER_VENT 1.5
  #endif

#endif
