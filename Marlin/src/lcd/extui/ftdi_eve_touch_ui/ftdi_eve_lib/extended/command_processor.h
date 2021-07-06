/***********************
 * command_processor.h *
 ***********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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

typedef struct {
  uint32_t bg;
  uint32_t grad;
  uint32_t fg;
  uint32_t rgb;
} btn_colors;

// Disable TOUCH_UI_FIT_TEXT on a case-by-case basis
namespace FTDI {
  constexpr uint16_t OPT_NOFIT = OPT_NOTICKS;
}

/**************************** Enhanced Command Processor **************************/

/* The CommandProcessor class wraps the CommandFifo with several features to make
 * defining user interfaces much easier.
 *
 *   - Implements chaining on all methods
 *   - Automatically adds text to button, toggle, text and keys.
 *   - Constrains all widgets to fit inside a box for ease of layout.
 *   - Font size is specified using a chained modifier.
 *   - Option argument is given the default OPT_3D value.
 */

class CommandProcessor : public CLCD::CommandFifo {
  public:
    static constexpr uint8_t STYLE_DISABLED = 0x80;

  private:
    static bool default_button_style_func(CommandProcessor &, uint8_t tag, uint8_t & /*style*/, uint16_t &options, bool) {
      if (tag != 0 && FTDI::EventLoop::get_pressed_tag() == tag) {
        options = FTDI::OPT_FLAT;
      }
      return false;
    }

    typedef bool btn_style_func_t(CommandProcessor &cmd, uint8_t tag, uint8_t &style, uint16_t &options, bool post);

    static btn_style_func_t  *_btn_style_callback;
    static bool is_tracking;
    int8_t  _font = 26, _tag = 0;
    uint8_t _style = 0;

  protected:
    // Returns the cannonical thickness of a widget (i.e. the height of a toggle element)
    uint16_t widget_thickness() {
      CLCD::FontMetrics fm(_font);
      return fm.height * 20.0/16;
    }

    FORCEDINLINE void linear_widget_box(int16_t &x, int16_t &y, int16_t &w, int16_t &h, bool tracker = false) {
      const uint16_t th = widget_thickness() / 2;
      if (w > h) {
        x += tracker ? th * 2.5 : th;
        y += (h - th) / 2;
        w -= tracker ? th * 5.0 : th * 2;
        h  = th;
      }
      else {
        x += (w - th) / 2;
        y += tracker ? th * 2.5 : th;
        w  = th;
        h -= tracker ? th * 5.0 : th * 2;
      }
    }

    FORCEDINLINE uint16_t circular_widget_box(int16_t &x, int16_t &y, int16_t &w, int16_t &h) {
      const uint16_t r = min(w,h) / 2;
      x += w / 2;
      y += h / 2;
      w  = 1;
      h  = 1;
      return r;
    }

  public:
    // Helper method for setting all colors at once
    inline CommandProcessor& colors(const btn_colors &colors) {
      cmd(FTDI::COLOR_RGB(colors.rgb))
        .gradcolor(colors.grad)
        .fgcolor(colors.fg)
        .bgcolor(colors.bg);
      return *this;
    }

    inline CommandProcessor& bitmap_size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height) {
      cmd(FTDI::BITMAP_SIZE(filter, wrapx, wrapy, width, height));
      #if FTDI_API_LEVEL >= 810
        if (FTDI::ftdi_chip >= 810)
          cmd(FTDI::BITMAP_SIZE_H(width >> 9, height >> 9));
      #endif
      return *this;
    }

    inline CommandProcessor& bitmap_layout(uint8_t format, uint16_t linestride, uint16_t height) {
      cmd(FTDI::BITMAP_LAYOUT(format, linestride, height));
      #if FTDI_API_LEVEL >= 810
        if (FTDI::ftdi_chip >= 810)
          cmd(FTDI::BITMAP_LAYOUT_H(linestride >> 10, height >> 9));
      #endif
      return *this;
    }

    inline CommandProcessor& set_button_style_callback(const btn_style_func_t *func) {
      _btn_style_callback = func ? func : default_button_style_func;
      return *this;
    }

    inline CommandProcessor& tag      (uint8_t  tag)              {_tag = tag; cmd(FTDI::TAG(tag)); return *this;}

    inline CommandProcessor& font     (int16_t  font)             {_font = font; return *this;}

    inline CommandProcessor& enabled  (bool enabled=false) {
      if (enabled)
        _style &= ~STYLE_DISABLED;
      else
        _style |= STYLE_DISABLED;
      return *this;
    }

    inline CommandProcessor& style    (uint8_t style) {
      _style = (_style & STYLE_DISABLED) | style;
      return *this;
    }

    // Wrap all the CommandFifo routines to allow method chaining

    inline CommandProcessor& cmd      (uint32_t cmd32)            {CLCD::CommandFifo::cmd(cmd32); return *this;}
    inline CommandProcessor& cmd      (void *data, uint16_t len)  {CLCD::CommandFifo::cmd(data, len); return *this;}
    inline CommandProcessor& execute()                            {CLCD::CommandFifo::execute(); return *this;}

    inline CommandProcessor& fgcolor  (uint32_t rgb)              {CLCD::CommandFifo::fgcolor(rgb); return *this;}
    inline CommandProcessor& bgcolor  (uint32_t rgb)              {CLCD::CommandFifo::bgcolor(rgb); return *this;}
    inline CommandProcessor& gradcolor(uint32_t rgb)              {CLCD::CommandFifo::gradcolor(rgb); return *this;}

    inline CommandProcessor& snapshot (uint32_t ptr)              {CLCD::CommandFifo::snapshot(ptr); return *this;}

    inline CommandProcessor& loadimage(uint32_t ptr, uint32_t options)
                                                                  {CLCD::CommandFifo::loadimage(ptr, options); return *this;}
    inline CommandProcessor& sketch   (int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t ptr, uint16_t format)
                                                                  {CLCD::CommandFifo::sketch(x, y, w, h, ptr, format); return *this;}
    inline CommandProcessor& screensaver  ()                      {CLCD::CommandFifo::screensaver(); return *this;}
    #if FTDI_API_LEVEL >= 810
    inline CommandProcessor& setbase  (uint8_t base)              {CLCD::CommandFifo::setbase(base); return *this;}
    #endif
    inline CommandProcessor& loadidentity ()                      {CLCD::CommandFifo::loadidentity(); return *this;}
    inline CommandProcessor& scale    (int32_t sx, int32_t sy)    {CLCD::CommandFifo::scale(sx,sy); return *this;}
    inline CommandProcessor& rotate   (int32_t a)                 {CLCD::CommandFifo::rotate(a); return *this;}
    inline CommandProcessor& translate(int32_t tx, int32_t ty)    {CLCD::CommandFifo::translate(tx,ty); return *this;}
    inline CommandProcessor& setmatrix ()                         {CLCD::CommandFifo::setmatrix(); return *this;}
    inline CommandProcessor& stop ()                              {CLCD::CommandFifo::stop(); return *this;}

    inline CommandProcessor& memzero  (uint32_t ptr, uint32_t size)
                                                                  {CLCD::CommandFifo::memzero(ptr, size); return *this;}
    inline CommandProcessor& memset   (uint32_t ptr, uint32_t val, uint32_t size)
                                                                  {CLCD::CommandFifo::memset(ptr, val, size); return *this;}
    inline CommandProcessor& memcpy   (uint32_t src, uint32_t dst, uint32_t size)
                                                                  {CLCD::CommandFifo::memcpy(src, dst, size); return *this;}
    inline CommandProcessor& memcrc   (uint32_t ptr, uint32_t num, uint32_t result)
                                                                  {CLCD::CommandFifo::memcrc(ptr, num, result); return *this;}
    inline CommandProcessor& memwrite (uint32_t ptr, uint32_t value)
                                                                  {CLCD::CommandFifo::memwrite(ptr, value); return *this;}
    inline CommandProcessor& inflate  (uint32_t ptr)
                                                                  {CLCD::CommandFifo::inflate(ptr); return *this;}
    inline CommandProcessor& getptr   (uint32_t result)
                                                                  {CLCD::CommandFifo::getptr(result); return *this;}
    inline CommandProcessor& getprops (uint32_t ptr, uint32_t width, uint32_t height)
                                                                  {CLCD::CommandFifo::getprops(ptr, width, height); return *this;}

    #if FTDI_API_LEVEL >= 810
    inline CommandProcessor& setbitmap (uint32_t ptr, uint16_t fmt, uint16_t w, uint16_t h)
                                                                  {CLCD::CommandFifo::setbitmap(ptr,fmt,w,h); return *this;}
    inline CommandProcessor& snapshot2 (uint32_t fmt, uint32_t ptr, int16_t x, int16_t y, uint16_t w, uint16_t h)
                                                                  {CLCD::CommandFifo::snapshot2(fmt,ptr,x,y,w,h); return *this;}
    inline CommandProcessor& mediafifo (uint32_t p, uint32_t s)   {CLCD::CommandFifo::mediafifo(p, s); return *this;}
    inline CommandProcessor& playvideo(uint32_t options)          {CLCD::CommandFifo::playvideo(options); return *this;}
    inline CommandProcessor& romfont(uint8_t font, uint8_t slot)  {CLCD::CommandFifo::romfont(font, slot); return *this;}
    #endif

    inline CommandProcessor& gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1)
                                                                  {CLCD::CommandFifo::gradient(x0,y0,rgb0,x1,y1,rgb1); return *this;}

    inline CommandProcessor& rectangle(int16_t x, int16_t y, int16_t w, int16_t h) {
      using namespace FTDI;
      CLCD::CommandFifo::cmd(BEGIN(RECTS));
      CLCD::CommandFifo::cmd(VERTEX2F(x * 16, y * 16));
      CLCD::CommandFifo::cmd(VERTEX2F((x + w) * 16, (y + h) * 16));
      return *this;
    }

    template<typename T>
    FORCEDINLINE CommandProcessor& toggle(int16_t x, int16_t y, int16_t w, int16_t h, T text, bool state, uint16_t options = FTDI::OPT_3D) {
      CLCD::FontMetrics fm(_font);
      const int16_t widget_h = fm.height * 20.0 / 16;
      //const int16_t outer_bar_r = widget_h / 2;
      //const int16_t knob_r      = outer_bar_r - 1.5;
      // The y coordinate of the toggle is the baseline of the text,
      // so we must introduce a fudge factor based on the line height to
      // actually center the control.
      const int16_t fudge_y = fm.height * 5 / 16;
      CLCD::CommandFifo::toggle(x + h / 2, y + (h - widget_h) / 2 + fudge_y, w - h, _font, options, state);
      CLCD::CommandFifo::str(text);
      return *this;
    }

    CommandProcessor& toggle2(int16_t x, int16_t y, int16_t w, int16_t h, progmem_str no, progmem_str yes, bool state, uint16_t options = FTDI::OPT_3D) {
      char text[strlen_P((const char *)no) + strlen_P((const char *)yes) + 2];
      strcpy_P(text, (const char *)no);
      strcat(text, "\xFF");
      strcat_P(text, (const char *)yes);
      return toggle(x, y, w, h, text, state, options);
    }

    // Contrained drawing routines. These constrain the widget inside a box for easier layout.
    // The FORCEDINLINE ensures that the code is inlined so that all the math is done at compile time.

    FORCEDINLINE CommandProcessor& track_linear(int16_t x, int16_t y, int16_t w, int16_t h, int16_t tag) {
      linear_widget_box(x, y, w, h, true);
      CLCD::CommandFifo::track(x, y, w, h, tag);
      is_tracking = true;
      return *this;
    }

    FORCEDINLINE CommandProcessor& track_circular(int16_t x, int16_t y, int16_t w, int16_t h, int16_t tag) {
      circular_widget_box(x,y, w, h);
      CLCD::CommandFifo::track(x, y, w, h, tag);
      is_tracking = true;
      return *this;
    }

    uint8_t track_tag (uint16_t &value) {
      if (is_tracking) {
        if (FTDI::EventLoop::is_touch_held()) {
          return CLCD::get_tracker(value);
        }
        else {
          CLCD::CommandFifo::track(0, 0, 0, 0, 0);
          CLCD::CommandFifo::execute();
          is_tracking = false;
        }
      }
      return 0;
    }

    FORCEDINLINE CommandProcessor& clock(int16_t x, int16_t y, int16_t w, int16_t h, int16_t hr, int16_t m, int16_t s, int16_t ms, uint16_t options = FTDI::OPT_3D) {
      const uint16_t r = circular_widget_box(x, y, w, h);
      CLCD::CommandFifo::clock(x, y, r, options, hr, m, s, ms);
      return *this;
    }

    FORCEDINLINE CommandProcessor& gauge(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t major, uint16_t minor, uint16_t val, uint16_t range, uint16_t options = FTDI::OPT_3D) {
      const uint16_t r = circular_widget_box(x, y, w, h);
      CLCD::CommandFifo::gauge(x, y, r, options, major, minor, val, range);
      return *this;
    }

    FORCEDINLINE CommandProcessor& dial(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t val, uint16_t options = FTDI::OPT_3D) {
      const uint16_t r = circular_widget_box(x, y, w, h);
      CLCD::CommandFifo::dial(x, y, r, options, val);
      return *this;
    }

    FORCEDINLINE CommandProcessor& slider(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t val, uint16_t range, uint16_t options = FTDI::OPT_3D) {
      linear_widget_box(x, y, w, h);
      CLCD::CommandFifo::slider(x, y, w, h, options, val, range);
      return *this;
    }

    FORCEDINLINE CommandProcessor& progress(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t val, uint16_t range, uint16_t options = FTDI::OPT_3D) {
      linear_widget_box(x, y, w, h);
      CLCD::CommandFifo::progress(x, y, w, h, options, val, range);
      return *this;
    }

    FORCEDINLINE CommandProcessor& scrollbar(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t val, uint16_t size, uint16_t range, uint16_t options = 0) {
      linear_widget_box(x, y, w, h);
      CLCD::CommandFifo::scrollbar(x, y, w, h, options, val, size, range);
      return *this;
    }

    void apply_text_alignment(int16_t &x, int16_t &y, int16_t w, int16_t h, uint16_t options) {
      using namespace FTDI;
      x += ((options & OPT_CENTERX) ? w/2 : ((options & OPT_RIGHTX) ? w : 0));
      y += ((options & OPT_CENTERY) ? h/2 : h);
    }

    // Reduce font size until text fits the enclosing box.
    template<typename T>
    int8_t apply_fit_text(int16_t w, int16_t h, T text) {
      using namespace FTDI;
      int8_t font = _font;
      #if ENABLED(TOUCH_UI_USE_UTF8)
        const bool is_utf8 = has_utf8_chars(text);
      #endif
      for (;font > 26;) {
        int16_t width, height;
        #if ENABLED(TOUCH_UI_USE_UTF8)
          if (is_utf8) {
            width  = get_utf8_text_width(text, font_size_t::from_romfont(font));
            height = font_size_t::from_romfont(font).get_height();
          }
          else
        #endif
          {
            CLCD::FontMetrics fm(font);
            width  = fm.get_text_width(text);
            height = fm.height;
          }
        if (width < w && height < h) break;
        font--;
      }
      return font;
    }

    CommandProcessor& number(int16_t x, int16_t y, int16_t w, int16_t h, int32_t n, uint16_t options = FTDI::OPT_CENTER) {
      using namespace FTDI;
      apply_text_alignment(x, y, w, h, options);
      CLCD::CommandFifo::number(x, y, _font, options, n);
      return *this;
    }

    template<typename T>
    uint16_t text_width(T text) {
      using namespace FTDI;
      #if ENABLED(TOUCH_UI_USE_UTF8)
        if (has_utf8_chars(text))
          return get_utf8_text_width(text, font_size_t::from_romfont(_font));
      #endif
      CLCD::FontMetrics fm(_font);
      return fm.get_text_width(text);
    }

    template<typename T>
    CommandProcessor& text(int16_t x, int16_t y, int16_t w, int16_t h, T text, uint16_t options = FTDI::OPT_CENTER) {
      using namespace FTDI;
      apply_text_alignment(x, y, w, h, options);
      #ifdef TOUCH_UI_FIT_TEXT
        const int8_t font = (options & OPT_NOFIT) ? _font : apply_fit_text(w, h, text);
      #else
        const int8_t font = _font;
      #endif
      #if ENABLED(TOUCH_UI_USE_UTF8)
        if (has_utf8_chars(text))
          draw_utf8_text(*this, x, y, text, font_size_t::from_romfont(font), options);
        else
      #endif
      {
        CLCD::CommandFifo::text(x, y, font, options);
        CLCD::CommandFifo::str(text);
      }
      return *this;
    }

    FORCEDINLINE CommandProcessor& icon(int16_t x, int16_t y, int16_t w, int16_t h, const FTDI::bitmap_info_t& info, const float scale = 1) {
      using namespace FTDI;
      cmd(BEGIN(BITMAPS));
      if (scale != 1) {
        cmd(BITMAP_TRANSFORM_A(uint32_t(float(256)/scale)));
        cmd(BITMAP_TRANSFORM_E(uint32_t(float(256)/scale)));
      }
      cmd(BITMAP_SIZE(info.filter, info.wrapx, info.wrapy, info.width*scale, info.height*scale));
      cmd(VERTEX2F((x + w/2 - info.width*scale/2)*16, (y + h/2 - info.height*scale/2)*16));
      if (scale != 1) {
        cmd(BITMAP_TRANSFORM_A(256));
        cmd(BITMAP_TRANSFORM_E(256));
      }
      return *this;
    }

    template<typename T>
    CommandProcessor& button(int16_t x, int16_t y, int16_t w, int16_t h, T text, uint16_t options = FTDI::OPT_3D) {
      using namespace FTDI;
      bool styleModified = false;
      if (_btn_style_callback) styleModified = _btn_style_callback(*this, _tag, _style, options, false);
      #ifdef TOUCH_UI_FIT_TEXT
        const int8_t font = (options & OPT_NOFIT) ? _font : apply_fit_text(w, h, text);
      #else
        const int8_t font = _font;
      #endif
      CLCD::CommandFifo::button(x, y, w, h, font, options);
      #if ENABLED(TOUCH_UI_USE_UTF8)
        if (has_utf8_chars(text)) {
          CLCD::CommandFifo::str(F(""));
          apply_text_alignment(x, y, w, h, OPT_CENTER);
          if (!(options & FTDI::OPT_FLAT)) {
            // Reproduce the black "shadow" the FTDI adds to the button label
            CLCD::CommandFifo::cmd(SAVE_CONTEXT());
            CLCD::CommandFifo::cmd(COLOR_RGB(0x00000));
            draw_utf8_text(*this, x-1, y-1, text, font_size_t::from_romfont(font), OPT_CENTER);
            CLCD::CommandFifo::cmd(RESTORE_CONTEXT());
          }
          // Draw the button label
          draw_utf8_text(*this, x, y, text, font_size_t::from_romfont(font), OPT_CENTER);
        }
        else
      #endif
        CLCD::CommandFifo::str(text);
      if (_btn_style_callback && styleModified) _btn_style_callback(*this, _tag, _style, options, true);
      return *this;
    }

    template<typename T>
    CommandProcessor& keys(int16_t x, int16_t y, int16_t w, int16_t h, T keys, uint16_t options = FTDI::OPT_3D) {
      CLCD::CommandFifo::keys(x, y, w, h, _font, options);
      CLCD::CommandFifo::str(keys);
      return *this;
    }

    FORCEDINLINE CommandProcessor& spinner(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t style = 0, uint16_t scale = 0) {
      circular_widget_box(x, y, w, h);
      CLCD::CommandFifo::spinner(x, y, style, scale);
      return *this;
    }
};
