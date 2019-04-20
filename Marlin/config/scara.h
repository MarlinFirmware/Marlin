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
 * MORGAN_SCARA was developed by QHARLEY in South Africa in 2012-2013.
 * Implemented and slightly reworked by JCERNY in June, 2014.
 */

// Specify the specific SCARA model
#define MORGAN_SCARA
#if ENABLED(MORGAN_SCARA)

  //#define DEBUG_SCARA_KINEMATICS
  #define SCARA_FEEDRATE_SCALING // Convert XY feedrate from mm/s to degrees/s on the fly

  // If movement is choppy try reducing this value
  #define SCARA_SEGMENTS_PER_SECOND 200

  // Length of inner and outer support arms. Measure arm lengths precisely.
  #define SCARA_LINKAGE_1   150 // (mm)
  #define SCARA_LINKAGE_2   150 // (mm)

  // SCARA tower offset (position of Tower relative to bed zero position)
  // This needs to be reasonably accurate as it defines the printbed position in the SCARA space.
  #define SCARA_OFFSET_X    100 // (mm)
  #define SCARA_OFFSET_Y    -56 // (mm)

  // Radius around the center where the arm cannot reach
  #define MIDDLE_DEAD_ZONE_R  0 // (mm)

  #define THETA_HOMING_OFFSET 0  //calculatated from Calibration Guide and command M360 / M114 see picture in http://reprap.harleystudio.co.za/?page_id=1073
  #define PSI_HOMING_OFFSET   0  //calculatated from Calibration Guide and command M364 / M114 see picture in http://reprap.harleystudio.co.za/?page_id=1073

#endif
