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
 * MORGAN_SCARA was developed by QHARLEY in South Africa in 2012-2013.
 * Implemented and slightly reworked by JCERNY in June, 2014.
 *
 * Mostly Printed SCARA is an open source design by Tyler Williams. See:
 *   https://www.thingiverse.com/thing:2487048
 *   https://www.thingiverse.com/thing:1241491
 */
//#define MORGAN_SCARA
//#define MP_SCARA
#if ANY(MORGAN_SCARA, MP_SCARA)
  // If movement is choppy try lowering this value
  #define DEFAULT_SEGMENTS_PER_SECOND 200

  // Length of inner and outer support arms. Measure arm lengths precisely.
  #define SCARA_LINKAGE_1 150       // (mm)
  #define SCARA_LINKAGE_2 150       // (mm)

  // SCARA tower offset (position of Tower relative to bed zero position)
  // This needs to be reasonably accurate as it defines the printbed position in the SCARA space.
  #define SCARA_OFFSET_X  100       // (mm)
  #define SCARA_OFFSET_Y  -56       // (mm)

  #if ENABLED(MORGAN_SCARA)

    //#define DEBUG_SCARA_KINEMATICS
    #define FEEDRATE_SCALING        // Convert XY feedrate from mm/s to degrees/s on the fly

    // Radius around the center where the arm cannot reach
    #define MIDDLE_DEAD_ZONE_R   0  // (mm)

    #define THETA_HOMING_OFFSET  0  // Calculated from Calibration Guide and M360 / M114. See https://www.morgan3dp.com/morgan-calibration-guide/
    #define PSI_HOMING_OFFSET    0  // Calculated from Calibration Guide and M364 / M114. See https://www.morgan3dp.com/morgan-calibration-guide/

  #elif ENABLED(MP_SCARA)

    #define SCARA_OFFSET_THETA1  12 // degrees
    #define SCARA_OFFSET_THETA2 131 // degrees

  #endif

#endif
