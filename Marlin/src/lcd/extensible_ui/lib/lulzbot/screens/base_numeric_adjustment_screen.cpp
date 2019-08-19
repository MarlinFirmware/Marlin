/**************************************
 * base_numeric_adjustment_screen.cpp *
 **************************************/

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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"
#include "screen_data.h"

using namespace FTDI;
using namespace Theme;

#ifdef TOUCH_UI_PORTRAIT
  #define GRID_COLS 13
  #define GRID_ROWS 10
#else
  #define GRID_COLS 18
  #define GRID_ROWS  7
#endif

BaseNumericAdjustmentScreen::widgets_t::widgets_t(draw_mode_t what) : _what(what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true));
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(font_medium)
       .colors(action_btn)
    #ifdef TOUCH_UI_PORTRAIT
       .tag(1).button( BTN_POS(1,10), BTN_SIZE(13,1), F("Back"))
    #else
       .tag(1).button( BTN_POS(15,7), BTN_SIZE(4,1),  F("Back"))
    #endif
       .colors(normal_btn);
  }

  _line = 1;
  _units = PSTR("");
}

BaseNumericAdjustmentScreen::widgets_t &BaseNumericAdjustmentScreen::widgets_t::precision(uint8_t decimals, precision_default_t initial) {
  _decimals = decimals;
  if (screen_data.BaseNumericAdjustmentScreen.increment == 0) {
    screen_data.BaseNumericAdjustmentScreen.increment = 243 + (initial - DEFAULT_LOWEST) - _decimals;
  }
  return *this;
}

void BaseNumericAdjustmentScreen::widgets_t::heading(const char *label) {
  CommandProcessor cmd;
  cmd.font(font_medium).cmd(COLOR_RGB(bg_text_enabled));
  if (_what & BACKGROUND) {
    #ifdef TOUCH_UI_PORTRAIT
      cmd.tag(0).fgcolor(bg_color).button( BTN_POS(1, _line), BTN_SIZE(12,1), progmem_str(label), OPT_FLAT);
    #else
      cmd.tag(0).fgcolor(bg_color).button( BTN_POS(5, _line), BTN_SIZE(8,1),  progmem_str(label), OPT_FLAT);
    #endif
  }

  _line++;
}

#ifdef TOUCH_UI_PORTRAIT
  #ifdef TOUCH_UI_800x480
    #undef EDGE_R
    #define EDGE_R 20
  #else
    #undef EDGE_R
    #define EDGE_R 10
  #endif
#endif

void BaseNumericAdjustmentScreen::widgets_t::_draw_increment_btn(uint8_t, const uint8_t tag) {
  CommandProcessor  cmd;
  const char        *label = PSTR("?");
  uint8_t            pos;
  uint8_t &          increment = screen_data.BaseNumericAdjustmentScreen.increment;

  if (increment == 0) {
    increment = tag; // Set the default value to be the first.
  }

  switch (tag) {
    case 240: label = PSTR(   ".001"); pos = _decimals - 3; break;
    case 241: label = PSTR(   ".01" ); pos = _decimals - 2; break;
    case 242: label = PSTR(  "0.1"  ); pos = _decimals - 1; break;
    case 243: label = PSTR(  "1"    ); pos = _decimals + 0; break;
    case 244: label = PSTR( "10"    ); pos = _decimals + 1; break;
    default:  label = PSTR("100"    ); pos = _decimals + 2; break;
  }

  cmd.tag(tag)
     .colors(increment == tag ? action_btn : normal_btn)
  #ifdef TOUCH_UI_PORTRAIT
     .font(font_small);
  #else
     .font(font_medium);
  #endif
  switch (pos) {
    #ifdef TOUCH_UI_PORTRAIT
      case 0: cmd.button( BTN_POS(5,_line), BTN_SIZE(2,1), progmem_str(label)); break;
      case 1: cmd.button( BTN_POS(7,_line), BTN_SIZE(2,1), progmem_str(label)); break;
      case 2: cmd.button( BTN_POS(9,_line), BTN_SIZE(2,1), progmem_str(label)); break;
    #else
      case 0: cmd.button( BTN_POS(15,2),    BTN_SIZE(4,1), progmem_str(label)); break;
      case 1: cmd.button( BTN_POS(15,3),    BTN_SIZE(4,1), progmem_str(label)); break;
      case 2: cmd.button( BTN_POS(15,4),    BTN_SIZE(4,1), progmem_str(label)); break;
    #endif
  }
  cmd.colors(normal_btn);
}


void BaseNumericAdjustmentScreen::widgets_t::increments() {
  if (_what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(bg_color)
       .tag(0)
    #ifdef TOUCH_UI_PORTRAIT
       .font(font_small).button( BTN_POS(1, _line),  BTN_SIZE(4,1), F("Increment:"), OPT_FLAT);
    #else
       .font(font_medium).button( BTN_POS(15,1),     BTN_SIZE(4,1), F("Increment:"), OPT_FLAT);
    #endif
  }

  if (_what & FOREGROUND) {
      _draw_increment_btn(_line+1, 245 - _decimals);
      _draw_increment_btn(_line+1, 244 - _decimals);
      _draw_increment_btn(_line+1, 243 - _decimals);
  }

  #ifdef TOUCH_UI_PORTRAIT
  _line++;
  #endif
}

void BaseNumericAdjustmentScreen::widgets_t::adjuster_sram_val(uint8_t tag, const char *label, const char *value, bool is_enabled) {
  CommandProcessor cmd;

  if (_what & BACKGROUND) {
    cmd.enabled(1)
       .font(font_small)
       .fgcolor(_color)            .tag(0).button( BTN_POS(5,_line), BTN_SIZE(5,1), F(""),               OPT_FLAT)
       .cmd(COLOR_RGB(bg_text_enabled))
       .fgcolor(bg_color) .tag(0).button( BTN_POS(1,_line), BTN_SIZE(4,1), (progmem_str) label, OPT_FLAT);
  }

  if (_what & FOREGROUND) {
    cmd.colors(normal_btn)
       .font(font_medium)
       .tag(is_enabled ? tag   : 0).enabled(is_enabled).button( BTN_POS(10,_line), BTN_SIZE(2,1),  F("-"))
       .tag(is_enabled ? tag+1 : 0).enabled(is_enabled).button( BTN_POS(12,_line), BTN_SIZE(2,1),  F("+"))
       .tag(0).font(font_small)                        .text  ( BTN_POS(5,_line),  BTN_SIZE(5,1),  is_enabled ? value : "-");
  }

  _line++;
}

void BaseNumericAdjustmentScreen::widgets_t::adjuster(uint8_t tag, const char *label, const char *value, bool is_enabled) {
  if (_what & BACKGROUND) {
    adjuster_sram_val(tag, label, nullptr);
  }

  if (_what & FOREGROUND) {
    char b[strlen_P(value)+1];
    strcpy_P(b,value);
    adjuster_sram_val(tag, label, b, is_enabled);
  }
}

void BaseNumericAdjustmentScreen::widgets_t::adjuster(uint8_t tag, const char *label, float value, bool is_enabled) {
  if (_what & BACKGROUND) {
    adjuster_sram_val(tag, label, nullptr);
  }

  if (_what & FOREGROUND) {
    char b[32];
    dtostrf(value, 5, _decimals, b);
    strcat_P(b, PSTR(" "));
    strcat_P(b, (const char*) _units);
    adjuster_sram_val(tag, label, b, is_enabled);
  }
}

void BaseNumericAdjustmentScreen::widgets_t::button(uint8_t tag, const char *label, bool is_enabled) {
  if (_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .tag(is_enabled ? tag   : 0)
       .enabled(is_enabled)
    #ifdef TOUCH_UI_PORTRAIT
       .font(font_small)
    #else
       .font(font_medium)
    #endif
    .button(BTN_POS(5,_line), BTN_SIZE(9,1), progmem_str(label));
  }

  _line++;
}

void BaseNumericAdjustmentScreen::widgets_t::text_field(uint8_t tag, const char *label, const char *value, bool is_enabled) {
  CommandProcessor cmd;

  if (_what & BACKGROUND) {
    cmd.enabled(1)
       .font(font_small)
       .cmd(COLOR_RGB(bg_text_enabled))
       .fgcolor(_color).tag(0).button( BTN_POS(5,_line), BTN_SIZE(9,1), F(""),               OPT_FLAT)
       .fgcolor(bg_color) .tag(0).button( BTN_POS(1,_line), BTN_SIZE(4,1), (progmem_str) label, OPT_FLAT);
  }

  if (_what & FOREGROUND) {
    cmd.colors(normal_btn)
       .font(font_medium)
       .tag(tag).font(font_small).text ( BTN_POS(5,_line), BTN_SIZE(9,1), is_enabled ? value : "-");
  }

  _line++;
}

void BaseNumericAdjustmentScreen::widgets_t::two_buttons(uint8_t tag1, const char *label1, uint8_t tag2, const char *label2, bool is_enabled) {
  if (_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.enabled(is_enabled)
    #ifdef TOUCH_UI_PORTRAIT
       .font(font_small)
    #else
       .font(font_medium)
    #endif
    .tag(is_enabled ? tag1: 0).button(BTN_POS(5,_line),   BTN_SIZE(4.5,1), progmem_str(label1))
    .tag(is_enabled ? tag2: 0).button(BTN_POS(9.5,_line), BTN_SIZE(4.5,1), progmem_str(label2));
  }

  _line++;
}

void BaseNumericAdjustmentScreen::widgets_t::toggle(uint8_t tag, const char *label, const char *text, bool value, bool is_enabled) {
  if (_what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(bg_color)
       .tag(0)
       .font(font_small)
    #ifdef TOUCH_UI_PORTRAIT
       .button( BTN_POS(1, _line), BTN_SIZE( 8,1), progmem_str(label), OPT_FLAT);
    #else
       .button( BTN_POS(1, _line), BTN_SIZE(10,1), progmem_str(label), OPT_FLAT);
    #endif
  }

  if (_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.tag(is_enabled ? tag   : 0)
       .enabled(is_enabled)
       .font(font_small)
       .colors(ui_toggle)
    #ifdef TOUCH_UI_PORTRAIT
      .toggle(BTN_POS( 9,_line), BTN_SIZE(5,1), progmem_str(text), value);
    #else
      .toggle(BTN_POS(10,_line), BTN_SIZE(4,1), progmem_str(text), value);
    #endif
  }

  _line++;
}

void BaseNumericAdjustmentScreen::widgets_t::home_buttons(uint8_t tag) {
  if (_what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.fgcolor(bg_color)
       .tag(0)
       .font(font_small)
       .button( BTN_POS(1, _line),  BTN_SIZE(4,1), F("Home:"), OPT_FLAT);
  }

  if (_what & FOREGROUND) {
    CommandProcessor cmd;
    cmd
    #ifdef TOUCH_UI_PORTRAIT
       .font(font_small)
    #else
       .font(font_medium)
    #endif
       .tag(tag+0).button(BTN_POS(5,_line),  BTN_SIZE(2,1), F("X"))
       .tag(tag+1).button(BTN_POS(7,_line),  BTN_SIZE(2,1), F("Y"))
       .tag(tag+2).button(BTN_POS(9,_line),  BTN_SIZE(2,1), F("Z"))
       .tag(tag+3).button(BTN_POS(11,_line), BTN_SIZE(3,1), F("All"));
  }

  _line++;
}

void BaseNumericAdjustmentScreen::onEntry() {
  screen_data.BaseNumericAdjustmentScreen.increment = 0; // This will force the increment to be picked while drawing.
  BaseScreen::onEntry();
}

bool BaseNumericAdjustmentScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:           GOTO_PREVIOUS();                            return true;
    case 240 ... 245: screen_data.BaseNumericAdjustmentScreen.increment = tag; break;
    default:          return current_screen.onTouchHeld(tag);
  }
  return true;
}

float BaseNumericAdjustmentScreen::getIncrement() {
  switch (screen_data.BaseNumericAdjustmentScreen.increment) {
    case 240: return   0.001;
    case 241: return   0.01;
    case 242: return   0.1;
    case 243: return   1.0;
    case 244: return  10.0;
    case 245: return 100.0;
    default:  return   0.0;
  }
}

#endif // LULZBOT_TOUCH_UI
