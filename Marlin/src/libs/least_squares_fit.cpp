/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Least Squares Best Fit by Roxy and Ed Williams
 *
 * This algorithm is high speed and has a very small code footprint.
 * Its results are identical to both the Iterative Least-Squares published
 * earlier by Roxy and the QR_SOLVE solution. If used in place of QR_SOLVE
 * it saves roughly 10K of program memory. It also does not require all of
 * coordinates to be present during the calculations. Each point can be
 * probed and then discarded.
 */

#include "../inc/MarlinConfig.h"

#if NEED_LSF

#include "least_squares_fit.h"

#include <math.h>

int finish_incremental_LSF(struct linear_fit_data *lsf) {

  const float N = lsf->N;

  if (N == 0.0)
    return 1;

  const float RN = 1.0f / N,
              xbar = lsf->xbar * RN,
              ybar = lsf->ybar * RN,
              zbar = lsf->zbar * RN,
              x2bar = lsf->x2bar * RN - sq(xbar),
              y2bar = lsf->y2bar * RN - sq(ybar),
              xybar = lsf->xybar * RN - xbar * ybar,
              yzbar = lsf->yzbar * RN - ybar * zbar,
              xzbar = lsf->xzbar * RN - xbar * zbar,
              DD = x2bar * y2bar - sq(xybar);

  if (ABS(DD) <= 1e-10 * (lsf->max_absx + lsf->max_absy))
    return 1;

  lsf->A = (yzbar * xybar - xzbar * y2bar) / DD;
  lsf->B = (xzbar * xybar - yzbar * x2bar) / DD;
  lsf->D = -(zbar + lsf->A * xbar + lsf->B * ybar);
  return 0;
}

#endif // NEED_LSF
