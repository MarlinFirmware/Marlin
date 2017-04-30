/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Incremental Least Squares Best Fit  By Roxy and Ed Williams
 *
 * This algorithm is high speed and has a very small code footprint.
 * Its results are identical to both the Iterative Least-Squares published
 * earlier by Roxy and the QR_SOLVE solution. If used in place of QR_SOLVE
 * it saves roughly 10K of program memory.   And even better...  the data
 * fed into the algorithm does not need to all be present at the same time.  
 * A point can be probed and its values fed into the algorithm and then discarded.
 *
 */

#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)  // Currently only used by UBL, but is applicable to Grid Based (Linear) Bed Leveling

#include "Marlin.h"
#include "macros.h"
#include <math.h>

struct linear_fit_data {
  int n;
  float xbar, ybar, zbar;
  float x2bar, y2bar, z2bar;
  float xybar, xzbar, yzbar;
  float max_absx, max_absy;
  float A, B, D;
};

void incremental_LSF_reset(struct linear_fit_data *); 
void incremental_LSF(struct linear_fit_data *, float x, float y, float z);
int finish_incremental_LSF(struct linear_fit_data *);

#endif

