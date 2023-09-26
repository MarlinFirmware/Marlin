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

// Enable for TPARA kinematics and configure below
//#define AXEL_TPARA
#if ENABLED(AXEL_TPARA)
  #define DEBUG_TPARA_KINEMATICS
  #define DEFAULT_SEGMENTS_PER_SECOND 200

  // Length of inner and outer support arms. Measure arm lengths precisely.
  #define TPARA_LINKAGE_1 120       // (mm)
  #define TPARA_LINKAGE_2 120       // (mm)

  // SCARA tower offset (position of Tower relative to bed zero position)
  // This needs to be reasonably accurate as it defines the printbed position in the SCARA space.
  #define TPARA_OFFSET_X    0       // (mm)
  #define TPARA_OFFSET_Y    0       // (mm)
  #define TPARA_OFFSET_Z    0       // (mm)

  #define FEEDRATE_SCALING        // Convert XY feedrate from mm/s to degrees/s on the fly

  // Radius around the center where the arm cannot reach
  #define MIDDLE_DEAD_ZONE_R   0  // (mm)

  // Calculated from Calibration Guide and M360 / M114. See https://www.morgan3dp.com/morgan-calibration-guide/
  #define THETA_HOMING_OFFSET  0
  #define PSI_HOMING_OFFSET    0
#endif
