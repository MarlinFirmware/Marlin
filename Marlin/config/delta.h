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

// Enable for DELTA kinematics and configure below
//#define DELTA
#if ENABLED(DELTA)

  // Make delta curves from many straight lines (linear interpolation).
  // This is a trade-off between visible corners (not enough segments)
  // and processor overload (too many expensive sqrt calls).
  #define DEFAULT_SEGMENTS_PER_SECOND 200

  // After homing move down to a height where XY movement is unconstrained
  //#define DELTA_HOME_TO_SAFE_ZONE

  // Delta calibration menu
  // Add three-point calibration to the MarlinUI menu.
  // See http://minow.blogspot.com/index.html#4918805519571907051
  //#define DELTA_CALIBRATION_MENU

  // G33 Delta Auto-Calibration. Enable EEPROM_SETTINGS to store results.
  //#define DELTA_AUTO_CALIBRATION

  #if ENABLED(DELTA_AUTO_CALIBRATION)
    // Default number of probe points : n*n (1 -> 7)
    #define DELTA_CALIBRATION_DEFAULT_POINTS 4
  #endif

  #if ANY(DELTA_AUTO_CALIBRATION, DELTA_CALIBRATION_MENU)
    // Step size for paper-test probing
    #define PROBE_MANUALLY_STEP 0.05      // (mm)
  #endif

  // Print surface diameter/2 minus unreachable space (avoid collisions with vertical towers).
  #define PRINTABLE_RADIUS       140.0    // (mm)

  // Maximum reachable area
  #define DELTA_MAX_RADIUS       140.0    // (mm)

  // Center-to-center distance of the holes in the diagonal push rods.
  #define DELTA_DIAGONAL_ROD 250.0        // (mm)

  // Distance between bed and nozzle Z home position
  #define DELTA_HEIGHT 250.00             // (mm) Get this value from G33 auto calibrate

  #define DELTA_ENDSTOP_ADJ { 0.0, 0.0, 0.0 } // Get these values from G33 auto calibrate

  // Horizontal distance bridged by diagonal push rods when effector is centered.
  #define DELTA_RADIUS 124.0              // (mm) Get this value from G33 auto calibrate

  // Trim adjustments for individual towers
  // tower angle corrections for X and Y tower / rotate XYZ so Z tower angle = 0
  // measured in degrees anticlockwise looking from above the printer
  #define DELTA_TOWER_ANGLE_TRIM { 0.0, 0.0, 0.0 } // Get these values from G33 auto calibrate

  // Delta radius and diagonal rod adjustments (mm)
  //#define DELTA_RADIUS_TRIM_TOWER { 0.0, 0.0, 0.0 }
  //#define DELTA_DIAGONAL_ROD_TRIM_TOWER { 0.0, 0.0, 0.0 }
#endif
