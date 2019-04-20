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

/**
 * Dual X Carriage
 *
 * This setup has two X carriages that can move independently, each with its own hotend.
 * The carriages can be used to print an object with two colors or materials, or in
 * "duplication mode" it can print two identical or X-mirrored objects simultaneously.
 * The inactive carriage is parked automatically to prevent oozing.
 * X1 is the left carriage, X2 the right. They park and home at opposite ends of the X axis.
 * By default the X2 stepper is assigned to the first unused E plug on the board.
 *
 * The following Dual X Carriage modes can be selected with M605 S<mode>:
 *
 *   0 : (FULL_CONTROL) The slicer has full control over both X-carriages and can achieve optimal travel
 *       results as long as it supports dual X-carriages. (M605 S0)
 *
 *   1 : (AUTO_PARK) The firmware automatically parks and unparks the X-carriages on tool-change so
 *       that additional slicer support is not required. (M605 S1)
 *
 *   2 : (DUPLICATION) The firmware moves the second X-carriage and extruder in synchronization with
 *       the first X-carriage and extruder, to print 2 copies of the same object at the same time.
 *       Set the constant X-offset and temperature differential with M605 S2 X[offs] R[deg] and
 *       follow with M605 S2 to initiate duplicated movement.
 *
 *   3 : (MIRRORED) Formbot/Vivedino-inspired mirrored mode in which the second extruder duplicates
 *       the movement of the first except the second extruder is reversed in the X axis.
 *       Set the initial X offset and temperature differential with M605 S2 X[offs] R[deg] and
 *       follow with M605 S3 to initiate mirrored movement.
 */
//#define DUAL_X_CARRIAGE
#if ENABLED(DUAL_X_CARRIAGE)
  #define X1_MIN_POS X_MIN_POS   // Set to X_MIN_POS
  #define X1_MAX_POS X_BED_SIZE  // Set a maximum so the first X-carriage can't hit the parked second X-carriage
  #define X2_MIN_POS    80       // Set a minimum to ensure the  second X-carriage can't hit the parked first X-carriage
  #define X2_MAX_POS   353       // Set this to the distance between toolheads when both heads are homed
  #define X2_HOME_DIR    1       // Set to 1. The second X-carriage always homes to the maximum endstop position
  #define X2_HOME_POS X2_MAX_POS // Default X2 home position. Set to X2_MAX_POS.
                      // However: In this mode the HOTEND_OFFSET_X value for the second extruder provides a software
                      // override for X2_HOME_POS. This also allow recalibration of the distance between the two endstops
                      // without modifying the firmware (through the "M218 T1 X???" command).
                      // Remember: you should set the second extruder x-offset to 0 in your slicer.

  // This is the default power-up mode which can be later using M605.
  #define DEFAULT_DUAL_X_CARRIAGE_MODE DXC_AUTO_PARK_MODE

  // Default x offset in duplication mode (typically set to half print bed width)
  #define DEFAULT_DUPLICATION_X_OFFSET 100

#endif // DUAL_X_CARRIAGE
