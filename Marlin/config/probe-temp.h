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
 * Thermal Probe Compensation
 *
 * Adjust probe measurements to compensate for distortion associated with the temperature
 * of the probe, bed, and/or hotend.
 * Use G76 to automatically calibrate this feature for probe and bed temperatures.
 * (Extruder temperature/offset values must be calibrated manually.)
 * Use M871 to set temperature/offset values manually.
 * For more details see https://marlinfw.org/docs/features/probe_temp_compensation.html
 */
//#define PTC_PROBE    // Compensate based on probe temperature
//#define PTC_BED      // Compensate based on bed temperature
//#define PTC_HOTEND   // Compensate based on hotend temperature

#if ANY(PTC_PROBE, PTC_BED, PTC_HOTEND)
  /**
   * If the probe is outside the defined range, use linear extrapolation with the closest
   * point and the point with index PTC_LINEAR_EXTRAPOLATION. e.g., If set to 4 it will use the
   * linear extrapolation between data[0] and data[4] for values below PTC_PROBE_START.
   */
  //#define PTC_LINEAR_EXTRAPOLATION 4

  #if ENABLED(PTC_PROBE)
    // Probe temperature calibration generates a table of values starting at PTC_PROBE_START
    // (e.g., 30), in steps of PTC_PROBE_RES (e.g., 5) with PTC_PROBE_COUNT (e.g., 10) samples.
    #define PTC_PROBE_START   30    // (°C)
    #define PTC_PROBE_RES      5    // (°C)
    #define PTC_PROBE_COUNT   10
    #define PTC_PROBE_ZOFFS   { 0 } // (µm) Z adjustments per sample
  #endif

  #if ENABLED(PTC_BED)
    // Bed temperature calibration builds a similar table.
    #define PTC_BED_START     60    // (°C)
    #define PTC_BED_RES        5    // (°C)
    #define PTC_BED_COUNT     10
    #define PTC_BED_ZOFFS     { 0 } // (µm) Z adjustments per sample
  #endif

  #if ENABLED(PTC_HOTEND)
    // Note: There is no automatic calibration for the hotend. Use M871.
    #define PTC_HOTEND_START 180    // (°C)
    #define PTC_HOTEND_RES     5    // (°C)
    #define PTC_HOTEND_COUNT  20
    #define PTC_HOTEND_ZOFFS  { 0 } // (µm) Z adjustments per sample
  #endif

  // G76 options
  #if ALL(PTC_PROBE, PTC_BED)
    // Park position to wait for probe cooldown
    #define PTC_PARK_POS   { 0, 0, 100 }

    // Probe position to probe and wait for probe to reach target temperature
    //#define PTC_PROBE_POS  { 12.0f, 7.3f } // Example: MK52 magnetic heatbed
    #define PTC_PROBE_POS  { 90, 100 }

    // The temperature the probe should be at while taking measurements during
    // bed temperature calibration.
    #define PTC_PROBE_TEMP    30  // (°C)

    // Height above Z=0.0 to raise the nozzle. Lowering this can help the probe to heat faster.
    // Note: The Z=0.0 offset is determined by the probe Z offset (e.g., as set with M851 Z).
    #define PTC_PROBE_HEATING_OFFSET 0.5
  #endif
#endif // PTC_PROBE || PTC_BED || PTC_HOTEND
