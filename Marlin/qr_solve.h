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

#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_GRID)

void daxpy(int n, float da, float dx[], int incx, float dy[], int incy);
float ddot(int n, float dx[], int incx, float dy[], int incy);
float dnrm2(int n, float x[], int incx);
void dqrank(float a[], int lda, int m, int n, float tol, int* kr,
            int jpvt[], float qraux[]);
void dqrdc(float a[], int lda, int n, int p, float qraux[], int jpvt[],
           float work[], int job);
int dqrls(float a[], int lda, int m, int n, float tol, int* kr, float b[],
          float x[], float rsd[], int jpvt[], float qraux[], int itask);
void dqrlss(float a[], int lda, int m, int n, int kr, float b[], float x[],
            float rsd[], int jpvt[], float qraux[]);
int dqrsl(float a[], int lda, int n, int k, float qraux[], float y[],
          float qy[], float qty[], float b[], float rsd[], float ab[], int job);
void dscal(int n, float sa, float x[], int incx);
void dswap(int n, float x[], int incx, float y[], int incy);
void qr_solve(float x[], int m, int n, float a[], float b[]);

#endif
