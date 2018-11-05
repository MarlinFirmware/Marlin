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
#pragma once

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

#include "../inc/MarlinConfig.h"
#include <math.h>

struct linear_fit_data {
  float xbar, ybar, zbar,
        x2bar, y2bar, z2bar,
        xybar, xzbar, yzbar,
        max_absx, max_absy,
        A, B, D, N;
};

void inline incremental_LSF_reset(struct linear_fit_data *lsf) {
  memset(lsf, 0, sizeof(linear_fit_data));
}

void inline incremental_WLSF(struct linear_fit_data *lsf, const float &x, const float &y, const float &z, const float &w) {
  // weight each accumulator by factor w, including the "number" of samples
  // (analogous to calling inc_LSF twice with same values to weight it by 2X)
  lsf->xbar  += w * x;
  lsf->ybar  += w * y;
  lsf->zbar  += w * z;
  lsf->x2bar += w * x * x;  // don't use sq(x) -- let compiler re-use w*x four times
  lsf->y2bar += w * y * y;
  lsf->z2bar += w * z * z;
  lsf->xybar += w * x * y;
  lsf->xzbar += w * x * z;
  lsf->yzbar += w * y * z;
  lsf->N     += w;
  lsf->max_absx = MAX(ABS(w * x), lsf->max_absx);
  lsf->max_absy = MAX(ABS(w * y), lsf->max_absy);
}

void inline incremental_LSF(struct linear_fit_data *lsf, const float &x, const float &y, const float &z) {
  lsf->xbar += x;
  lsf->ybar += y;
  lsf->zbar += z;
  lsf->x2bar += sq(x);
  lsf->y2bar += sq(y);
  lsf->z2bar += sq(z);
  lsf->xybar += x * y;
  lsf->xzbar += x * z;
  lsf->yzbar += y * z;
  lsf->max_absx = MAX(ABS(x), lsf->max_absx);
  lsf->max_absy = MAX(ABS(y), lsf->max_absy);
  lsf->N += 1.0;
}

int finish_incremental_LSF(struct linear_fit_data *);
