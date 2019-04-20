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
 * Automatic backlash, position and hotend offset calibration
 *
 * Enable G425 to run automatic calibration using an electrically-
 * conductive cube, bolt, or washer mounted on the bed.
 *
 * G425 uses the probe to touch the top and sides of the calibration object
 * on the bed and measures and/or correct positional offsets, axis backlash
 * and hotend offsets.
 *
 * Note: HOTEND_OFFSET and CALIBRATION_OBJECT_CENTER must be set to within
 *       ±5mm of true values for G425 to succeed.
 */
//#define CALIBRATION_GCODE
#if ENABLED(CALIBRATION_GCODE)

  #define CALIBRATION_MEASUREMENT_RESOLUTION     0.01 // mm

  #define CALIBRATION_FEEDRATE_SLOW             60    // mm/m
  #define CALIBRATION_FEEDRATE_FAST           1200    // mm/m
  #define CALIBRATION_FEEDRATE_TRAVEL         3000    // mm/m

  // The following parameters refer to the conical section of the nozzle tip.
  #define CALIBRATION_NOZZLE_TIP_HEIGHT          1.0  // mm
  #define CALIBRATION_NOZZLE_OUTER_DIAMETER      2.0  // mm

  // Uncomment to enable reporting (required for "G425 V", but consumes PROGMEM).
  //#define CALIBRATION_REPORTING

  // The true location and dimension the cube/bolt/washer on the bed.
  #define CALIBRATION_OBJECT_CENTER     { 264.0, -22.0,  -2.0} // mm
  #define CALIBRATION_OBJECT_DIMENSIONS {  10.0,  10.0,  10.0} // mm

  // Comment out any sides which are unreachable by the probe. For best
  // auto-calibration results, all sides must be reachable.
  #define CALIBRATION_MEASURE_RIGHT
  #define CALIBRATION_MEASURE_FRONT
  #define CALIBRATION_MEASURE_LEFT
  #define CALIBRATION_MEASURE_BACK

  // Probing at the exact top center only works if the center is flat. If
  // probing on a screwhead or hollow washer, probe near the edges.
  //#define CALIBRATION_MEASURE_AT_TOP_EDGES

  // Define pin which is read during calibration
  #ifndef CALIBRATION_PIN
    #define CALIBRATION_PIN -1 // Override in pins.h or set to -1 to use your Z endstop
    #define CALIBRATION_PIN_INVERTING false // set to true to invert the pin
    //#define CALIBRATION_PIN_PULLDOWN
    #define CALIBRATION_PIN_PULLUP
  #endif
#endif
