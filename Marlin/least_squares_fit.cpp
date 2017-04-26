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
 * Least Squares Best Fit  By Roxy and Ed Williams
 *
 * This algorithm is high speed and has a very small code footprint.
 * Its results are identical to both the Iterative Least-Squares published
 * earlier by Roxy and the QR_SOLVE solution. If used in place of QR_SOLVE
 * it saves roughly 10K of program memory.   It also does not require all of 
 * coordinates to be present during the calculations.  Each point can be 
 * probed and then discarded.
 *
 */

#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)  // Currently only used by UBL, but is applicable to Grid Based (Linear) Bed Leveling

#include "macros.h"
#include <math.h>

#include "least_squares_fit.h"

void incremental_LSF_reset(struct linear_fit_data *lsf) {
	lsf->n = 0;
	lsf->A = 0.0;					// probably a memset() can be done to zero 
	lsf->B = 0.0;                                   // this whole structure
	lsf->D = 0.0;
	lsf->xbar = lsf->ybar = lsf->zbar = 0.0;
	lsf->x2bar = lsf->y2bar = lsf->z2bar = 0.0;
	lsf->xybar = lsf->xzbar = lsf->yzbar = 0.0;
	lsf->max_absx = lsf->max_absy = 0.0;
    }

void incremental_LSF(struct linear_fit_data *lsf, float x, float y, float z) {
	lsf->xbar += x;
	lsf->ybar += y;
	lsf->zbar += z;
	lsf->x2bar += x*x;
	lsf->y2bar += y*y;
	lsf->z2bar += z*z;
	lsf->xybar += x*y;
	lsf->xzbar += x*z;
	lsf->yzbar += y*z;
	lsf->max_absx = (fabs(x) > lsf->max_absx) ? fabs(x) : lsf->max_absx;
	lsf->max_absy = (fabs(y) > lsf->max_absy) ? fabs(y) : lsf->max_absy;
	lsf->n++;
	return;
  }

int finish_incremental_LSF(struct linear_fit_data *lsf) {
	float DD, N;

	N = (float) lsf->n;
	lsf->xbar /= N;
	lsf->ybar /= N;
	lsf->zbar /= N;
	lsf->x2bar = lsf->x2bar/N - lsf->xbar*lsf->xbar;
	lsf->y2bar = lsf->y2bar/N - lsf->ybar*lsf->ybar;
	lsf->z2bar = lsf->z2bar/N - lsf->zbar*lsf->zbar;
	lsf->xybar = lsf->xybar/N - lsf->xbar*lsf->ybar;
	lsf->yzbar = lsf->yzbar/N - lsf->ybar*lsf->zbar;
	lsf->xzbar = lsf->xzbar/N - lsf->xbar*lsf->zbar;

	DD = lsf->x2bar*lsf->y2bar - lsf->xybar*lsf->xybar;
	if (fabs(DD) <= 1e-10*(lsf->max_absx+lsf->max_absy)) 
	  return -1;
	
	lsf->A = (lsf->yzbar*lsf->xybar - lsf->xzbar*lsf->y2bar) / DD;
	lsf->B = (lsf->xzbar*lsf->xybar - lsf->yzbar*lsf->x2bar) / DD;
	lsf->D = -(lsf->zbar + lsf->A*lsf->xbar + lsf->B*lsf->ybar);
	return 0;
}
#endif


