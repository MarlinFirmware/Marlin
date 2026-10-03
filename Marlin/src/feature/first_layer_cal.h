/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * first_layer_cal.h - Pattern generator for First Layer Calibration (M1005)
 *
 * Produces zig-zag rows from back to front, then a solid patch between
 * last two rows. First row starts wide and narrows, as a purge.
 * Pure math with no machine state, so it can be unit tested.
 */

#include <math.h>
#include <stdint.h>

#define FLC_PURGE_SEGMENT   25.0f   // (mm) Maximum length of each wide purge segment
#define FLC_PATCH_WIDTH     20.0f   // (mm) Solid patch size
#define FLC_PATCH_HEIGHT    12.0f
#define FLC_ROUND_BED_SPAN   0.8f   // Portion of printable radius spanned by rows in Y

typedef struct { float x, y, e; } flc_segment_t;  // Line end point and filament length

class FLCPattern {
public:
  // Rows span a rectangle. Returns false if area is too small.
  bool init_rect(const float x_min, const float x_max, const float y_min, const float y_max,
                 const uint8_t nrows, const float lh, const float lw, const float fil_dia) {
    round = false;
    cy = radius = 0;
    cx = (x_min + x_max) * 0.5f;
    half_x = (x_max - x_min) * 0.5f;
    return init(y_min, y_max, nrows, lh, lw, fil_dia);
  }

  // Rows are chords of a circle. Returns false if area is too small.
  bool init_round(const float center_x, const float center_y, const float r,
                  const uint8_t nrows, const float lh, const float lw, const float fil_dia) {
    round = true;
    cx = center_x; cy = center_y; radius = half_x = r;
    const float half_y = r * FLC_ROUND_BED_SPAN;
    return init(center_y - half_y, center_y + half_y, nrows, lh, lw, fil_dia);
  }

  // Where first line starts
  float start_x() const { return x; }
  float start_y() const { return y; }

  uint8_t patch_lines() const { return lines; }
  float row_pitch() const { return pitch; }

  // Cross-section of extruded line with rounded ends (mm^2)
  static float area(const float lh, const float lw) { return float(M_PI) * lh * lh * 0.25f + lh * (lw - lh); }

  // Get next line. Returns false when pattern is complete.
  bool next(flc_segment_t &seg) {
    switch (state) {
      case PURGE_WIDE: {
        purge = fminf(FLC_PURGE_SEGMENT, fabsf(row_end() - x) / 3);
        emit(seg, x + dir * purge, y, area(h, 4 * w) / area(h, w));
        state = PURGE_NARROW;
      } break;

      case PURGE_NARROW:
        emit(seg, x + dir * purge, y, area(h, 2 * w) / area(h, w));
        state = ROW;
        break;

      case ROW:
        emit(seg, row_end(), y);
        state = (row + 1 < rows) ? CONNECT : lines ? PATCH_STEP : DONE;
        break;

      case CONNECT:
        emit(seg, x, y - pitch);
        dir = -dir;
        ++row;
        state = ROW;
        break;

      case PATCH_STEP:
        emit(seg, x, y + spacing);
        state = PATCH_LINE;
        break;

      case PATCH_LINE:
        emit(seg, x - dir * patch_w, y);
        dir = -dir;
        state = --lines ? PATCH_STEP : DONE;
        break;

      case DONE: return false;
    }
    return true;
  }

private:
  enum : uint8_t { PURGE_WIDE, PURGE_NARROW, ROW, CONNECT, PATCH_STEP, PATCH_LINE, DONE } state;
  bool round;
  uint8_t rows, row, lines;
  float cx, cy, half_x, radius,       // Area
        h, w, epm,                    // Line height, width, filament per mm
        pitch, spacing, patch_w, purge,
        x, y, dir;                    // Cursor

  // Half row length at given Y
  float row_half(const float ry) const {
    if (!round) return half_x;
    const float d = radius * radius - (ry - cy) * (ry - cy);
    return d > 0 ? sqrtf(d) : 0;
  }

  // End each row where straight step to next row stays inside area
  float row_end() const {
    const float half = row + 1 < rows ? fminf(row_half(y), row_half(y - pitch)) : row_half(y);
    return cx + dir * half;
  }

  void emit(flc_segment_t &seg, const float nx, const float ny, const float e_mul=1.0f) {
    seg.x = nx; seg.y = ny;
    seg.e = sqrtf((nx - x) * (nx - x) + (ny - y) * (ny - y)) * epm * e_mul;
    x = nx; y = ny;
  }

  bool init(const float y_front, const float y_back, const uint8_t nrows,
            const float lh, const float lw, const float fil_dia) {
    state = DONE;
    if (nrows < 2 || lh <= 0 || lw < lh || fil_dia <= 0) return false;
    rows = nrows; h = lh; w = lw;
    epm = area(h, w) / (float(M_PI) * fil_dia * fil_dia * 0.25f);
    pitch = (y_back - y_front) / (rows - 1);

    // Fit patch between last two rows
    const float patch_h = fminf(FLC_PATCH_HEIGHT, pitch - 4 * w);
    if (patch_h < 4 * w || row_half(y_back) * 2 < 3 * (FLC_PURGE_SEGMENT)) return false;
    spacing = w - h * (1.0f - float(M_PI) * 0.25f);
    lines = uint8_t(patch_h / spacing + 0.5f);
    patch_w = fminf(FLC_PATCH_WIDTH, row_half(y_front));

    // Start at back left and work toward front
    row = 0; dir = 1.0f;
    y = y_back;
    x = cx - row_half(y);
    state = PURGE_WIDE;
    return true;
  }
};
