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
// Motherboard Sensor options
//
#if TEMP_SENSOR_BOARD
  #define THERMAL_PROTECTION_BOARD   // Halt the printer if the board sensor leaves the temp range below.
  #define BOARD_MINTEMP           8  // (°C)
  #define BOARD_MAXTEMP          70  // (°C)
  //#define TEMP_BOARD_PIN -1        // Board temp sensor pin override.
#endif

//
// SoC Sensor options
//
#if TEMP_SENSOR_SOC
  #define THERMAL_PROTECTION_SOC     // Halt the printer if the SoC sensor leaves the temp range below.
  #define SOC_MAXTEMP            85  // (°C)
#endif

/**
 * Thermal Protection provides additional protection to your printer from damage
 * and fire. Marlin always includes safe min and max temperature ranges which
 * protect against a broken or disconnected thermistor wire.
 *
 * The issue: If a thermistor falls out, it will report the much lower
 * temperature of the air in the room, and the the firmware will keep
 * the heater on.
 *
 * The solution: Once the temperature reaches the target, start observing.
 * If the temperature stays too far below the target (hysteresis) for too
 * long (period), the firmware will halt the machine as a safety precaution.
 *
 * If you get false positives for "Thermal Runaway", increase
 * THERMAL_PROTECTION_HYSTERESIS and/or THERMAL_PROTECTION_PERIOD
 */
#if ENABLED(THERMAL_PROTECTION_HOTENDS)
  #define THERMAL_PROTECTION_PERIOD 40        // Seconds
  #define THERMAL_PROTECTION_HYSTERESIS 4     // Degrees Celsius

  //#define ADAPTIVE_FAN_SLOWING              // Slow down the part-cooling fan if the temperature drops
  #if ENABLED(ADAPTIVE_FAN_SLOWING)
    //#define REPORT_ADAPTIVE_FAN_SLOWING     // Report fan slowing activity to the console
    #if ANY(MPCTEMP, PIDTEMP)
      //#define TEMP_TUNING_MAINTAIN_FAN      // Don't slow down the fan speed during M303 or M306 T
    #endif
  #endif

  /**
   * Whenever an M104, M109, or M303 increases the target temperature, the
   * firmware will wait for the WATCH_TEMP_PERIOD to expire. If the temperature
   * hasn't increased by WATCH_TEMP_INCREASE degrees, the machine is halted and
   * requires a hard reset. This test restarts with any M104/M109/M303, but only
   * if the current temperature is far enough below the target for a reliable
   * test.
   *
   * If you get false positives for "Heating failed", increase WATCH_TEMP_PERIOD
   * and/or decrease WATCH_TEMP_INCREASE. WATCH_TEMP_INCREASE should not be set
   * below 2.
   */
  #define WATCH_TEMP_PERIOD  40               // Seconds
  #define WATCH_TEMP_INCREASE 2               // Degrees Celsius
#endif

/**
 * Thermal Protection parameters for the bed are just as above for hotends.
 */
#if ENABLED(THERMAL_PROTECTION_BED)
  #define THERMAL_PROTECTION_BED_PERIOD        20 // Seconds
  #define THERMAL_PROTECTION_BED_HYSTERESIS     2 // Degrees Celsius

  /**
   * As described above, except for the bed (M140/M190/M303).
   */
  #define WATCH_BED_TEMP_PERIOD                60 // Seconds
  #define WATCH_BED_TEMP_INCREASE               2 // Degrees Celsius
#endif

/**
 * Thermal Protection parameters for the heated chamber.
 */
#if ENABLED(THERMAL_PROTECTION_CHAMBER)
  #define THERMAL_PROTECTION_CHAMBER_PERIOD    20 // Seconds
  #define THERMAL_PROTECTION_CHAMBER_HYSTERESIS 2 // Degrees Celsius

  /**
   * Heated chamber watch settings (M141/M191).
   */
  #define WATCH_CHAMBER_TEMP_PERIOD            60 // Seconds
  #define WATCH_CHAMBER_TEMP_INCREASE           2 // Degrees Celsius
#endif

/**
 * Thermal Protection parameters for the laser cooler.
 */
#if ENABLED(THERMAL_PROTECTION_COOLER)
  #define THERMAL_PROTECTION_COOLER_PERIOD     10 // Seconds
  #define THERMAL_PROTECTION_COOLER_HYSTERESIS  3 // Degrees Celsius

  /**
   * Laser cooling watch settings (M143/M193).
   */
  #define WATCH_COOLER_TEMP_PERIOD             60 // Seconds
  #define WATCH_COOLER_TEMP_INCREASE            3 // Degrees Celsius
#endif

#if ANY(THERMAL_PROTECTION_HOTENDS, THERMAL_PROTECTION_BED, THERMAL_PROTECTION_CHAMBER, THERMAL_PROTECTION_COOLER)
  /**
   * Thermal Protection Variance Monitor - EXPERIMENTAL
   * Kill the machine on a stuck temperature sensor.
   *
   * This feature may cause some thermally-stable systems to halt. Be sure to test it thoroughly under
   * a variety of conditions. Disable if you get false positives.
   *
   * This feature ensures that temperature sensors are updating regularly. If sensors die or get "stuck",
   * or if Marlin stops reading them, temperatures will remain constant while heaters may still be powered!
   * This feature only monitors temperature changes so it should catch any issue, hardware or software.
   *
   * By default it uses the THERMAL_PROTECTION_*_PERIOD constants (above) for the time window, within which
   * at least one temperature change must occur, to indicate that sensor polling is working. If any monitored
   * heater's temperature remains totally constant (without even a fractional change) during this period, a
   * thermal malfunction error occurs and the printer is halted.
   *
   * A very stable heater might produce a false positive and halt the printer. In this case, try increasing
   * the corresponding THERMAL_PROTECTION_*_PERIOD constant a bit. Keep in mind that uncontrolled heating
   * shouldn't be allowed to persist for more than a minute or two.
   *
   * Be careful to distinguish false positives from real sensor issues before disabling this feature. If the
   * heater's temperature appears even slightly higher than expected after restarting, you may have a real
   * thermal malfunction. Check the temperature graph in your host for any unusual bumps.
   */
  //#define THERMAL_PROTECTION_VARIANCE_MONITOR
  #if ENABLED(THERMAL_PROTECTION_VARIANCE_MONITOR)
    // Variance detection window to override the THERMAL_PROTECTION...PERIOD settings above.
    // Keep in mind that some heaters heat up faster than others.
    //#define THERMAL_PROTECTION_VARIANCE_MONITOR_PERIOD 30  // (s) Override all watch periods
  #endif
#endif
