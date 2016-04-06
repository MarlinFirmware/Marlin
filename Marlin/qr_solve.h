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

#include "Configuration.h"

#if ENABLED(AUTO_BED_LEVELING_GRID)

void daxpy(int n, double da, double dx[], int incx, double dy[], int incy);
double ddot(int n, double dx[], int incx, double dy[], int incy);
double dnrm2(int n, double x[], int incx);
void dqrank(double a[], int lda, int m, int n, double tol, int* kr,
            int jpvt[], double qraux[]);
void dqrdc(double a[], int lda, int n, int p, double qraux[], int jpvt[],
           double work[], int job);
int dqrls(double a[], int lda, int m, int n, double tol, int* kr, double b[],
          double x[], double rsd[], int jpvt[], double qraux[], int itask);
void dqrlss(double a[], int lda, int m, int n, int kr, double b[], double x[],
            double rsd[], int jpvt[], double qraux[]);
int dqrsl(double a[], int lda, int n, int k, double qraux[], double y[],
          double qy[], double qty[], double b[], double rsd[], double ab[], int job);
void dscal(int n, double sa, double x[], int incx);
void dswap(int n, double x[], int incx, double y[], int incy);
void qr_solve(double x[], int m, int n, double a[], double b[]);

#endif
