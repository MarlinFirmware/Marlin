/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//#define DELTA
#if ENABLED(DELTA)

  #define SLOWDOWN false

  // Make delta curves from many straight lines (linear interpolation).
  // This is a trade-off between visible corners (not enough segments)
  // and processor overload (too many expensive sqrt calls).
  #define DELTA_SEGMENTS_PER_SECOND 200

  // After homing move down to a height where XY movement is unconstrained
  //#define DELTA_HOME_TO_SAFE_ZONE

  // Delta calibration menu
  // uncomment to add three points calibration menu option.
  // See http://minow.blogspot.com/index.html#4918805519571907051
  //#define DELTA_CALIBRATION_MENU

  // uncomment to add G33 Delta Auto-Calibration (Enable EEPROM_SETTINGS to store results)
  //#define DELTA_AUTO_CALIBRATION

  // NOTE NB all values for DELTA_* values MUST be floating point, so always have a decimal point in them

  #if ENABLED(DELTA_AUTO_CALIBRATION)
    // set the default number of probe points : n*n (1 -> 7)
    #define DELTA_CALIBRATION_DEFAULT_POINTS 4
  #endif

  #if EITHER(DELTA_AUTO_CALIBRATION, DELTA_CALIBRATION_MENU)
    // Set the radius for the calibration probe points - max DELTA_PRINTABLE_RADIUS for non-eccentric probes
    #define DELTA_CALIBRATION_RADIUS 121.5 // (mm)
    // Set the steprate for papertest probing
    #define PROBE_MANUALLY_STEP 0.05      // (mm)
  #endif

  // Print surface diameter/2 minus unreachable space (avoid collisions with vertical towers).
  #define DELTA_PRINTABLE_RADIUS 140.0    // (mm)

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

  /**
   * Allen key retractable z-probe as seen on many Kossel delta printers - http://reprap.org/wiki/Kossel#Automatic_bed_leveling_probe
   * Deploys by touching z-axis belt. Retracts by pushing the probe down. Uses Z_MIN_PIN.
   */
  //#define Z_PROBE_ALLEN_KEY
  #if ENABLED(Z_PROBE_ALLEN_KEY)
    // 2 or 3 sets of coordinates for deploying and retracting the spring loaded touch probe on G29,
    // if servo actuated touch probe is not defined. Uncomment as appropriate for your printer/probe.

    #define Z_PROBE_ALLEN_KEY_DEPLOY_1_X 30.0
    #define Z_PROBE_ALLEN_KEY_DEPLOY_1_Y DELTA_PRINTABLE_RADIUS
    #define Z_PROBE_ALLEN_KEY_DEPLOY_1_Z 100.0
    #define Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE XY_PROBE_SPEED

    #define Z_PROBE_ALLEN_KEY_DEPLOY_2_X 0.0
    #define Z_PROBE_ALLEN_KEY_DEPLOY_2_Y DELTA_PRINTABLE_RADIUS
    #define Z_PROBE_ALLEN_KEY_DEPLOY_2_Z 100.0
    #define Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE (XY_PROBE_SPEED)/10

    #define Z_PROBE_ALLEN_KEY_DEPLOY_3_X Z_PROBE_ALLEN_KEY_DEPLOY_2_X * 0.75
    #define Z_PROBE_ALLEN_KEY_DEPLOY_3_Y Z_PROBE_ALLEN_KEY_DEPLOY_2_Y * 0.75
    #define Z_PROBE_ALLEN_KEY_DEPLOY_3_Z Z_PROBE_ALLEN_KEY_DEPLOY_2_Z
    #define Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE XY_PROBE_SPEED

    #define Z_PROBE_ALLEN_KEY_STOW_1_X -64.0 // Move the probe into position
    #define Z_PROBE_ALLEN_KEY_STOW_1_Y 56.0
    #define Z_PROBE_ALLEN_KEY_STOW_1_Z 23.0
    #define Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE XY_PROBE_SPEED

    #define Z_PROBE_ALLEN_KEY_STOW_2_X -64.0 // Push it down
    #define Z_PROBE_ALLEN_KEY_STOW_2_Y 56.0
    #define Z_PROBE_ALLEN_KEY_STOW_2_Z 3.0
    #define Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE (XY_PROBE_SPEED)/10

    #define Z_PROBE_ALLEN_KEY_STOW_3_X -64.0 // Move it up to clear
    #define Z_PROBE_ALLEN_KEY_STOW_3_Y 56.0
    #define Z_PROBE_ALLEN_KEY_STOW_3_Z 50.0
    #define Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE XY_PROBE_SPEED

    #define Z_PROBE_ALLEN_KEY_STOW_4_X 0.0
    #define Z_PROBE_ALLEN_KEY_STOW_4_Y 0.0
    #define Z_PROBE_ALLEN_KEY_STOW_4_Z Z_PROBE_ALLEN_KEY_STOW_3_Z
    #define Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE XY_PROBE_SPEED

  #endif // Z_PROBE_ALLEN_KEY

#endif // DELTA
