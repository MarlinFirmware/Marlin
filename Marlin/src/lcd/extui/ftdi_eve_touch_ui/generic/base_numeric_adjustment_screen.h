/************************************
 * base_numeric_adjustment_screen.h *
 ************************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
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

#define FTDI_BASE_NUMERIC_ADJ_SCREEN
#define FTDI_BASE_NUMERIC_ADJ_SCREEN_CLASS BaseNumericAdjustmentScreen

struct BaseNumericAdjustmentScreenData {
  uint8_t increment;
};

class BaseNumericAdjustmentScreen : public BaseScreen {
  public:
    enum precision_default_t {
      DEFAULT_LOWEST,
      DEFAULT_MIDRANGE,
      DEFAULT_HIGHEST
    };

  protected:
    class widgets_t {
      private:
        draw_mode_t _what;
        uint8_t     _line;
        uint32_t    _color;
        uint8_t     _decimals;
        FSTR_P      _units;
        enum style_t {
          BTN_NORMAL,
          BTN_ACTION,
          BTN_TOGGLE,
          BTN_DISABLED,
          TEXT_AREA,
          TEXT_LABEL
        } _style;

      protected:
        void _draw_increment_btn(CommandProcessor &, uint8_t line, const uint8_t tag);
        void _button(CommandProcessor &, uint8_t tag, int16_t x, int16_t y, int16_t w, int16_t h, FSTR_P, bool enabled = true, bool highlight = false);
        void _button_style(CommandProcessor &cmd, style_t style);
      public:
        widgets_t(draw_mode_t);

        widgets_t &color(uint32_t color)       {_color = color; return *this;}
        widgets_t &units(FSTR_P units)         {_units = units; return *this;}
        widgets_t &draw_mode(draw_mode_t what) {_what  = what;  return *this;}
        widgets_t &precision(uint8_t decimals, precision_default_t = DEFAULT_HIGHEST);

        void heading           (FSTR_P label);
        void adjuster_sram_val (uint8_t tag,  FSTR_P label, const char *value,  bool is_enabled = true);
        void adjuster          (uint8_t tag,  FSTR_P label, const char *value,  bool is_enabled = true);
        void adjuster          (uint8_t tag,  FSTR_P label, float value=0,      bool is_enabled = true);
        void button            (uint8_t tag,  FSTR_P label,                     bool is_enabled = true);
        void text_field        (uint8_t tag,  FSTR_P label, const char *value,  bool is_enabled = true);
        void two_buttons       (uint8_t tag1, FSTR_P label1,
                                uint8_t tag2, FSTR_P label2,                    bool is_enabled = true);
        void toggle            (uint8_t tag,  FSTR_P label,                     bool value, bool is_enabled = true);
        void home_buttons      (uint8_t tag);
        void increments        ();
    };

    static float getIncrement();

  public:
    static void onEntry();
    static bool onTouchEnd(uint8_t tag);
};
