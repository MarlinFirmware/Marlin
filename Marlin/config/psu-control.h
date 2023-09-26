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

/**
 * Power Supply Control
 *
 * Enable and connect the power supply to the PS_ON_PIN.
 * Specify whether the power supply is active HIGH or active LOW.
 */
//#define PSU_CONTROL
//#define PSU_NAME "Power Supply"

#if ENABLED(PSU_CONTROL)
  //#define MKS_PWC                 // Using the MKS PWC add-on
  //#define PS_OFF_CONFIRM          // Confirm dialog when power off
  //#define PS_OFF_SOUND            // Beep 1s when power off
  #define PSU_ACTIVE_STATE LOW      // Set 'LOW' for ATX, 'HIGH' for X-Box

  //#define PSU_DEFAULT_OFF               // Keep power off until enabled directly with M80
  //#define PSU_POWERUP_DELAY      250    // (ms) Delay for the PSU to warm up to full power
  //#define LED_POWEROFF_TIMEOUT 10000    // (ms) Turn off LEDs after power-off, with this amount of delay

  //#define POWER_OFF_TIMER               // Enable M81 D<seconds> to power off after a delay
  //#define POWER_OFF_WAIT_FOR_COOLDOWN   // Enable M81 S to power off only after cooldown

  //#define PSU_POWERUP_GCODE  "M355 S1"  // G-code to run after power-on (e.g., case light on)
  //#define PSU_POWEROFF_GCODE "M355 S0"  // G-code to run before power-off (e.g., case light off)

  //#define AUTO_POWER_CONTROL      // Enable automatic control of the PS_ON pin
  #if ENABLED(AUTO_POWER_CONTROL)
    #define AUTO_POWER_FANS           // Turn on PSU for fans
    #define AUTO_POWER_E_FANS         // Turn on PSU for E Fans
    #define AUTO_POWER_CONTROLLERFAN  // Turn on PSU for Controller Fan
    #define AUTO_POWER_CHAMBER_FAN    // Turn on PSU for Chamber Fan
    #define AUTO_POWER_COOLER_FAN     // Turn on PSU for Cooler Fan
    #define AUTO_POWER_SPINDLE_LASER  // Turn on PSU for Spindle/Laser
    #define POWER_TIMEOUT              30 // (s) Turn off power if the machine is idle for this duration
    //#define POWER_OFF_DELAY          60 // (s) Delay of poweroff after M81 command. Useful to let fans run for extra time.
  #endif
  #if ANY(AUTO_POWER_CONTROL, POWER_OFF_WAIT_FOR_COOLDOWN)
    //#define AUTO_POWER_E_TEMP        50 // (°C) PSU on if any extruder is over this temperature
    //#define AUTO_POWER_CHAMBER_TEMP  30 // (°C) PSU on if the chamber is over this temperature
    //#define AUTO_POWER_COOLER_TEMP   26 // (°C) PSU on if the cooler is over this temperature
  #endif
#endif
