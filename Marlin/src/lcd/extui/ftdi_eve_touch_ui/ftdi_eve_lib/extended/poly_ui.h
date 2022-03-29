/*************
 * poly_ui.h *
 *************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
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

#pragma once

/**
 * The PolyReader class iterates over an array of (x,y) pairs.
 * For supporting polygons with holes an end-of-loop marker may
 * be embedded into the data stream:
 *
 *   const PROGMEM uint16_t data[] = {
 *      x, y, x, y, ..., eol,
 *          ...
 *      x, y, x, y, ..., eol
 *   }
 *
 * The PolyReader object can be used to iterate over the points.
 *
 *   PolyReader r(data, N_ELEMENTS(data));
 *
 *   for (r.start();r.has_more(); r.next()) {
 *     uint16_t x = r.x;
 *     uint16_t y = r.y;
 *
 *     // Do something with the point
 *         ...
 *
 *     // Do something else if this point
 *     // closes a loop.
 *     if (r.end_of_loop()) {
 *       ...
 *     }
 *   }
 */

class PolyReader {
  public:
    typedef uint16_t type_t;

  private:
    static constexpr type_t eol = 0xFFFF;

    const type_t *p, *top, *end;
    type_t start_x, start_y;

    void close_loop() {
      x       = start_x;
      y       = start_y;
      start_x = eol;
      start_y = eol;
    }

  public:
    type_t x, y;

    // Begin reading a polygon data structure
    PolyReader(const uint16_t data[], const size_t n_elements) : top(data), end(data + n_elements) {
      start();
    }

    void start() {
      p       = top;
      start_x = eol;
      next();
    }

    // Reads the next point in the polygon data structure
    void next() {
      if (!p) return;

      if (p == end) {
        if (start_x != eol)
          close_loop();
        else
          p = nullptr;
      }
      else {
        x = pgm_read_word_far(p++);
        if (x == eol)
          close_loop();
        else {
          y = pgm_read_word_far(p++);
          if (start_x == eol) {
            start_x = x;
            start_y = y;
          }
        }
      }
    }

    bool has_more()       { return p != nullptr; }
    bool end_of_loop()    { return start_x == eol; }
};

/**
 * The TransformedPolyReader class works like the PolyReader,
 * but the (x,y) input is assumed to be normalized onto a
 * unit square and then mapped to the full 16-bits, i.e.
 * (0.0,1.0) => (0x0000,0xFFFE). This class will scale the
 * data to fit the entire display, a bounding box, or apply
 * some arbitrary affine transform.
 *
 * This class is suitable for reading data from "svg2cpp.py"
 */
class TransformedPolyReader : public PolyReader {
  private:
    /**
     * Fixed point type for fast transformations, supports
     * values from 0 to 1024, with 1/32 precision.
     */
    static constexpr uint8_t fract_bits = 5;
    typedef int16_t fix_t;
    fix_t makefix(float f) { return f * (1 << fract_bits); }

    // First two rows of 3x3 transformation matrix
    fix_t a, b, c;
    fix_t d, e, f;

    void transform() {
      /**
       * Values from PolyReader vary from 0 to FFFE.
       * As an approximation to dividing by FFFE,
       * we perform a bit shift right by 16.
       */
      const int32_t px = PolyReader::x;
      const int32_t py = PolyReader::y;
      const int32_t round = 1 << (fract_bits-1);
      x = (((((a * px) + (b * py)) >> 16) + c) + round) >> fract_bits;
      y = (((((d * px) + (e * py)) >> 16) + f) + round) >> fract_bits;
    }

    void set_transform(
      fix_t A, fix_t B, fix_t C,
      fix_t D, fix_t E, fix_t F
    ) {
      a = A; b = B; c = C;
      d = D; e = E; f = F;
    }

  public:
    typedef int16_t type_t;

    type_t x, y;

    TransformedPolyReader(const uint16_t data[], const size_t n) : PolyReader(data, n) {
      scale_to_fit();
      transform();
    }

    // Set an arbitrary affine transform
    void set_transform(
      float A, float B, float C,
      float D, float E, float F
    ) {
      set_transform(
        makefix(A), makefix(B), makefix(C),
        makefix(D), makefix(E), makefix(F)
      );
    }

    // Scale the data to fit a specified bounding box
    void scale_to_fit(type_t x_min, type_t y_min, type_t x_max, type_t y_max) {
      fix_t sx = makefix(x_max - x_min);
      fix_t sy = makefix(y_max - y_min);
      fix_t tx = makefix(x_min);
      fix_t ty = makefix(y_min);
      set_transform(
        sx,  0, tx,
        0,  sy, ty
      );
    }

    // Scale to fit the entire display (default)
    void scale_to_fit() {
      scale_to_fit(0, 0, FTDI::display_width, FTDI::display_height);
    }

    void next() {
      PolyReader::next();
      transform();
    }
};

/**
 * The DeduplicatedPolyReader wraps around another PolyReader
 * class to remove repeated points from the data. This could
 * happen when scaling down using TransformedPolyReader, for
 * example.
 */
template<class POLY_READER>
class DeduplicatedPolyReader : public POLY_READER {
  private:
    typename POLY_READER::type_t last_x, last_y;

    static constexpr typename POLY_READER::type_t eol = 0xFFFF;

  public:
    DeduplicatedPolyReader(const uint16_t data[], const size_t n) : POLY_READER(data, n) {
      last_x = POLY_READER::x;
      last_y = POLY_READER::y;
    }

    void next() {
      do {
        if (!POLY_READER::has_more()) return;
        POLY_READER::next();
      } while (POLY_READER::x == last_x && POLY_READER::y == last_y && !POLY_READER::end_of_loop());
      if (POLY_READER::end_of_loop()) {
        last_x = last_y = eol;
      }
      else {
        last_x = POLY_READER::x;
        last_y = POLY_READER::y;
      }
    }
};

/**
 * The helper class allows you to build an interface based on arbitrary
 * shapes.
 */
template<class POLY_READER=DeduplicatedPolyReader<TransformedPolyReader>>
class GenericPolyUI {
  protected:
    CommandProcessor &cmd;
    draw_mode_t mode;

  private:
    // Attributes used to paint buttons

    uint32_t btn_fill_color   = 0x000000;
    uint32_t btn_shadow_color = 0xF3E0E0;
    uint8_t  btn_shadow_depth = 5;
    uint32_t btn_stroke_color = 0x000000;
    uint8_t  btn_stroke_width = 28;

  public:
    enum ButtonStyle : uint8_t {
      FILL    = 1,
      STROKE  = 2,
      SHADOW  = 4,
      REGULAR = 7
    };

    typedef POLY_READER poly_reader_t;

    GenericPolyUI(CommandProcessor &c, draw_mode_t what = BOTH) : cmd(c), mode(what) {}

    // Fills a polygon with the current COLOR_RGB
    void fill(poly_reader_t r, bool clip = true) {
      using namespace FTDI;
      int16_t x, y, w, h;

      if (clip) {
        // Clipping reduces the number of pixels that are
        // filled, allowing more complex shapes to be drawn
        // in the allotted time.
        bounds(r, x, y, w, h);
        cmd.cmd(SAVE_CONTEXT());
        cmd.cmd(SCISSOR_XY(x, y));
        cmd.cmd(SCISSOR_SIZE(w, h));
      }

      Polygon p(cmd);
      p.begin_fill();
      p.begin_loop();
      for (r.start();r.has_more();r.next()) {
        p(r.x * 16, r.y * 16);
        if (r.end_of_loop()) {
          p.end_loop();
          p.begin_loop();
        }
      }
      p.end_loop();
      p.end_fill();
      if (clip)
        cmd.cmd(RESTORE_CONTEXT());
    }

    void shadow(poly_reader_t r, uint8_t offset) {
      #if FTDI_API_LEVEL >= 810
        using namespace FTDI;
        cmd.cmd(VERTEX_TRANSLATE_X(offset * 16));
        cmd.cmd(VERTEX_TRANSLATE_Y(offset * 16));
        fill(r, false);
        cmd.cmd(VERTEX_TRANSLATE_X(0));
        cmd.cmd(VERTEX_TRANSLATE_Y(0));
      #endif
    }

    // Strokes a polygon with the current COLOR_RGB
    void stroke(poly_reader_t r) {
      using namespace FTDI;
      Polygon p(cmd);
      p.begin_stroke();
      p.begin_loop();
      for (r.start();r.has_more(); r.next()) {
        p(r.x * 16, r.y * 16);
        if (r.end_of_loop()) {
          p.end_loop();
          p.begin_loop();
        }
      }
      p.end_loop();
      p.end_stroke();
    }

    // Compute the bounds of a polygon
    void bounds(poly_reader_t r, int16_t &x, int16_t &y, int16_t &w, int16_t &h) {
      int16_t x_min = INT16_MAX;
      int16_t y_min = INT16_MAX;
      int16_t x_max = INT16_MIN;
      int16_t y_max = INT16_MIN;
      for (r.start(); r.has_more(); r.next()) {
        x_min = min(x_min, int16_t(r.x));
        x_max = max(x_max, int16_t(r.x));
        y_min = min(y_min, int16_t(r.y));
        y_max = max(y_max, int16_t(r.y));
      }
      x = x_min;
      y = y_min;
      w = x_max - x_min;
      h = y_max - y_min;
    }

    /**
     * Draw shaped buttons. Buttons are drawn out of a polygon which is
     * filled and stroked on top of a drop shadow. The button will
     * become "pushed" when touched.
     */

    void button_fill(const uint32_t color) {
      btn_fill_color = color;
    }

    void button_stroke(const uint32_t color, const uint8_t width) {
      btn_stroke_color = color;
      btn_stroke_width = width;
    }

    void button_shadow(const uint32_t color, const uint8_t depth) {
      btn_shadow_color = color;
      btn_shadow_depth = depth;
    }

    void button(const uint8_t tag, poly_reader_t r, uint8_t style = REGULAR) {
      using namespace FTDI;
      // Draw the shadow
      #if FTDI_API_LEVEL >= 810
      if (mode & BACKGROUND && style & SHADOW) {
        cmd.cmd(SAVE_CONTEXT());
        cmd.cmd(TAG(tag));
        cmd.cmd(VERTEX_TRANSLATE_X(btn_shadow_depth * 16));
        cmd.cmd(VERTEX_TRANSLATE_Y(btn_shadow_depth * 16));
        cmd.cmd(COLOR_RGB(btn_shadow_color));
        fill(r, false);
        cmd.cmd(RESTORE_CONTEXT());
      }
      #endif

      if (mode & FOREGROUND) {
        cmd.cmd(SAVE_CONTEXT());
        #if FTDI_API_LEVEL >= 810
          if (EventLoop::get_pressed_tag() == tag) {
            // "Push" the button
            cmd.cmd(VERTEX_TRANSLATE_X(btn_shadow_depth * 16));
            cmd.cmd(VERTEX_TRANSLATE_Y(btn_shadow_depth * 16));
          }
        #endif
        // Draw the fill and stroke
        cmd.cmd(TAG(tag));
        if (style & FILL) {
          cmd.cmd(COLOR_RGB(btn_fill_color));
          fill(r, false);
        }
        if (style & STROKE) {
          cmd.cmd(COLOR_RGB(btn_stroke_color));
          cmd.cmd(LINE_WIDTH(btn_stroke_width));
          stroke(r);
        }
        cmd.cmd(RESTORE_CONTEXT());
      }
    }

    void color(const uint32_t color) {
      cmd.cmd(FTDI::COLOR_RGB(color));
    }
};

typedef GenericPolyUI<> PolyUI;
