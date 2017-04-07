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
 * This algorythm is high speed and has a very small code footprint.
 * Its results are identical to both the Iterative Least Squares published
 * earlier by Roxy and the QR_SOLVE solution.   If used in place of QR_SOLVE
 * it saves roughly 10KB of program memory.
 *
 */

#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)	// Currently only used by UBL, but is applicable to Grid Based (Linear) Bed Leveling
  #include <math.h>
  #include "ubl.h"
  #include "Marlin.h"

double linear_fit_average(double *, int);
double linear_fit_average_squared(double *, int);
double linear_fit_average_mixed_terms(double *, double *, int );
double linear_fit_average_product(double *matrix1, double *matrix2, int n);
void   linear_fit_subtract_mean(double *matrix, double bar, int n);
double linear_fit_max_abs(double *, int);

struct linear_fit linear_fit_results;

struct linear_fit *lsf_linear_fit(double *x, double *y, double *z, int n) {
	double xbar, ybar, zbar;
	double x2bar, y2bar;
	double xybar, xzbar, yzbar;
	double D;
	int i;

	linear_fit_results.A = 0.0;
	linear_fit_results.B = 0.0;
	linear_fit_results.D = 0.0;

	xbar = linear_fit_average(x, n);
	ybar = linear_fit_average(y, n);
	zbar = linear_fit_average(z, n);

	linear_fit_subtract_mean( x, xbar, n);
	linear_fit_subtract_mean( y, ybar, n);
	linear_fit_subtract_mean( z, zbar, n);

	x2bar = linear_fit_average_product( x, x, n);
	y2bar = linear_fit_average_product( y, y, n);
	xybar =	linear_fit_average_product( x, y, n);
	xzbar =	linear_fit_average_product( x, z, n);
	yzbar =	linear_fit_average_product( y, z, n);

	D = x2bar*y2bar - xybar*xybar;
	for(i=0; i<n; i++) {
		if (fabs(D) <= 1e-15*( linear_fit_max_abs(x, n) + linear_fit_max_abs(y, n))) {
			printf( "error: x,y points are collinear at index:%d \n", i );
			return NULL;
		}
	}

	linear_fit_results.A = -(xzbar*y2bar - yzbar*xybar) / D;
	linear_fit_results.B = -(yzbar*x2bar - xzbar*xybar) / D;
//	linear_fit_results.D = -(zbar - linear_fit_results->A*xbar - linear_fit_results->B*ybar);
	linear_fit_results.D = -(zbar + linear_fit_results.A*xbar + linear_fit_results.B*ybar);

	return &linear_fit_results;
}




double linear_fit_average(double *matrix, int n)
{
	int i;
	double sum=0.0;

	for (i = 0; i < n; i++)
		sum += matrix[i];
	return sum / (double) n;
}

double linear_fit_average_product(double *matrix1, double *matrix2, int n) {
	int i;
	double sum = 0.0;

	for (i = 0; i < n; i++)
		sum += matrix1[i] * matrix2[i];
	return sum / (double) n;
}



void linear_fit_subtract_mean(double *matrix, double bar, int n) {
	int i;

	for (i = 0; i < n; i++) {
		matrix[i] -= bar;
	}
	return;
}

double linear_fit_max_abs(double *matrix, int n) {
	int i;
	double max_abs = 0.0;

	for(i=0; i<n; i++)
		if ( max_abs < fabs(matrix[i]))
			max_abs = fabs(matrix[i]);
	return max_abs;
}
#endif


