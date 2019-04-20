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

//
// G2/G3 Arc Support
//
#define ARC_SUPPORT               // Disable this feature to save ~3226 bytes
#if ENABLED(ARC_SUPPORT)
  #define MM_PER_ARC_SEGMENT  1   // Length of each arc segment
  #define MIN_ARC_SEGMENTS   24   // Minimum number of segments in a complete circle
  #define N_ARC_CORRECTION   25   // Number of interpolated segments between corrections
  //#define ARC_P_CIRCLES         // Enable the 'P' parameter to specify complete circles
  //#define CNC_WORKSPACE_PLANES  // Allow G2/G3 to operate in XY, ZX, or YZ planes
#endif

// Support for G5 with XYZE destination and IJPQ offsets. Requires ~2666 bytes.
//#define BEZIER_CURVE_SUPPORT

// SkeinForge sends the wrong arc G-codes when using Arc Point as fillet procedure
//#define SF_ARC_FIX
