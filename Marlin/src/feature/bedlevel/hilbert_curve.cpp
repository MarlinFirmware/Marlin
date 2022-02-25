/*********************
 * hilbert_curve.cpp *
 *********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver Labs, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../../inc/MarlinConfig.h"

#if ENABLED(UBL_HILBERT_CURVE)

#include "bedlevel.h"
#include "hilbert_curve.h"

constexpr int8_t  to_fix(int8_t  v) { return v * 2; }
constexpr int8_t  to_int(int8_t  v) { return v / 2; }
constexpr uint8_t   log2(uint8_t n) { return (n > 1) ? 1 + log2(n >> 1) : 0; }
constexpr uint8_t  order(uint8_t n) { return uint8_t(log2(n - 1)) + 1; }
constexpr uint8_t ord = order(_MAX(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y));
constexpr uint8_t dim = _BV(ord);

static inline bool eval_candidate(int8_t x, int8_t y, hilbert_curve::callback_ptr func, void *data) {
  // The print bed likely has fewer points than the full Hilbert
  // curve, so cull unnecessary points
  return x < (GRID_MAX_POINTS_X) && y < (GRID_MAX_POINTS_Y) ? func(x, y, data) : false;
}

bool hilbert_curve::hilbert(int8_t x, int8_t y, int8_t xi, int8_t xj, int8_t yi, int8_t yj, uint8_t n, hilbert_curve::callback_ptr func, void *data) {
  /**
   * Hilbert space-filling curve implementation
   *
   * x  and y  : coordinates of the bottom left corner
   * xi and xj : i and j components of the unit x vector of the frame
   * yi and yj : i and j components of the unit y vector of the frame
   *
   * From: http://www.fundza.com/algorithmic/space_filling/hilbert/basics/index.html
   */
  if (n)
    return hilbert(x,           y,           yi/2,  yj/2,  xi/2,  xj/2, n-1, func, data) ||
           hilbert(x+xi/2,      y+xj/2,      xi/2,  xj/2,  yi/2,  yj/2, n-1, func, data) ||
           hilbert(x+xi/2+yi/2, y+xj/2+yj/2, xi/2,  xj/2,  yi/2,  yj/2, n-1, func, data) ||
           hilbert(x+xi/2+yi,   y+xj/2+yj,  -yi/2, -yj/2, -xi/2, -xj/2, n-1, func, data);
  else
    return eval_candidate(to_int(x+(xi+yi)/2), to_int(y+(xj+yj)/2), func, data);
}

/**
 * Calls func(x, y, data) for all points in the Hilbert curve.
 * If that function returns true, the search is terminated.
 */
bool hilbert_curve::search(hilbert_curve::callback_ptr func, void *data) {
  return hilbert(to_fix(0), to_fix(0),to_fix(dim), to_fix(0), to_fix(0), to_fix(dim), ord, func, data);
}

/* Helper function for starting the search at a particular point */

typedef struct {
  uint8_t x, y;
  bool found_1st;
  hilbert_curve::callback_ptr func;
  void *data;
} search_from_t;

static bool search_from_helper(uint8_t x, uint8_t y, void *data) {
  search_from_t *d = (search_from_t *) data;
  if (d->x == x && d->y == y)
    d->found_1st = true;
  return d->found_1st ? d->func(x, y, d->data) : false;
}

/**
 * Same as search, except start at a specific grid intersection point.
 */
bool hilbert_curve::search_from(uint8_t x, uint8_t y, hilbert_curve::callback_ptr func, void *data) {
  search_from_t d;
  d.x = x;
  d.y = y;
  d.found_1st = false;
  d.func = func;
  d.data = data;
  // Call twice to allow search to wrap back to the beginning and picked up points prior to the start.
  return search(search_from_helper, &d) || search(search_from_helper, &d);
}

/**
 * Like search_from, but takes a bed position and starts from the nearest
 * point on the Hilbert curve.
 */
bool hilbert_curve::search_from_closest(const xy_pos_t &pos, hilbert_curve::callback_ptr func, void *data) {
  // Find closest grid intersection
  const uint8_t grid_x = LROUND(constrain(float(pos.x - (MESH_MIN_X)) / (MESH_X_DIST), 0, (GRID_MAX_POINTS_X) - 1));
  const uint8_t grid_y = LROUND(constrain(float(pos.y - (MESH_MIN_Y)) / (MESH_Y_DIST), 0, (GRID_MAX_POINTS_Y) - 1));
  return search_from(grid_x, grid_y, func, data);
}

#endif // UBL_HILBERT_CURVE
