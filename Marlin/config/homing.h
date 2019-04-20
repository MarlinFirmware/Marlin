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

// Direction of endstops when homing; 1=MAX, -1=MIN
// :[-1,1]
#define X_HOME_DIR -1
#define Y_HOME_DIR -1
#define Z_HOME_DIR -1

// The center of the bed is at (X=0, Y=0)
//#define BED_CENTER_AT_0_0

// Manually set the home position. Leave these undefined for automatic settings.
// For DELTA this is the top-center of the Cartesian print volume.
//#define MANUAL_X_HOME_POS 0
//#define MANUAL_Y_HOME_POS 0
//#define MANUAL_Z_HOME_POS 0

// Homing speeds (mm/m)
#define HOMING_FEEDRATE_XY (50*60)
#define HOMING_FEEDRATE_Z  (4*60)

//#define Z_HOMING_HEIGHT 4  // (mm) Minimal Z height before homing (G28) for Z clearance above the bed, clamps, ...
                             // Be sure you have this distance over your Z_MAX_POS in case.

// Don't raise Z (lower the bed) if Z is "unknown." (For beds that fall when Z is powered off.)
//#define UNKNOWN_Z_NO_RAISE

// Inhibit movement until all axes have been homed
//#define NO_MOTION_BEFORE_HOMING

// Require rehoming after steppers are deactivated
//#define HOME_AFTER_DEACTIVATE

/**
 * Use "Z Safe Homing" to ensure the probe is within the bed area on G28 Z.
 *
 * With this feature enabled:
 *
 * - Allow Z homing only after X and Y homing AND stepper drivers still enabled.
 * - If stepper drivers time out, it will need X and Y homing again before Z homing.
 * - Move the Z probe (or nozzle) to a defined XY point before Z Homing when homing all axes (G28).
 * - Prevent Z homing when the Z probe is outside bed area.
 */
//#define Z_SAFE_HOMING
#if ENABLED(Z_SAFE_HOMING)
  #define Z_SAFE_HOMING_X_POINT ((X_BED_SIZE) / 2)    // X point for Z homing when homing all axes (G28).
  #define Z_SAFE_HOMING_Y_POINT ((Y_BED_SIZE) / 2)    // Y point for Z homing when homing all axes (G28).
#endif

// Validate that endstops are triggered on homing moves
#define VALIDATE_HOMING_ENDSTOPS

// Homing hits each endstop, retracts by these distances, then does a slower bump.
#define X_HOME_BUMP_MM 5
#define Y_HOME_BUMP_MM 5
#define Z_HOME_BUMP_MM 2
#define HOMING_BUMP_DIVISOR { 2, 2, 4 }  // Re-Bump Speed Divisor (Divides the Homing Feedrate)
//#define QUICK_HOME                     // If homing includes X and Y, do a diagonal move initially
//#define HOMING_BACKOFF_MM { 2, 2, 2 }  // (mm) Move away from the endstops after homing

// When G28 is called, this option will make Y home before X
//#define HOME_Y_BEFORE_X

// Enable this if X or Y can't home without homing the other axis first.
//#define CODEPENDENT_XY_HOMING
