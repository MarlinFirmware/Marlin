/************
 * colors.h *
 ************/

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

#pragma once

namespace Theme {

  #define COLOR_CORRECTION(rgb)  ( \
    (uint32_t((((rgb) & 0xFF0000) >> 16) * 1.00) << 16) | \
    (uint32_t((((rgb) & 0x00FF00) >>  8) * 1.00) <<  8) | \
    (uint32_t((((rgb) & 0x0000FF) >>  0) *  .75) <<  0))

  #define COLOR_BLEND(a,b,f) COLOR_CORRECTION( \
    (uint32_t((((a) & 0xFF0000) >> 16) *    f + (((b) & 0xFF0000) >> 16) * (1-f))  << 16) | \
    (uint32_t((((a) & 0x00FF00) >>  8) *    f + (((b) & 0x00FF00) >>  8) * (1-f))  <<  8) | \
    (uint32_t((((a) & 0x0000FF) >>  0) *    f + (((b) & 0x0000FF) >>  0) * (1-f))  <<  0))

  constexpr uint32_t lulzbot_bg          = 0xDEEA5C;
  constexpr uint32_t lulzbot_fg          = 0xC1D82F;

  constexpr uint32_t lulzbot_green       = COLOR_BLEND(0xC1DB2F,0x788814,0.33);

  #ifndef LULZBOT_USE_BIOPRINTER_UI
    constexpr uint32_t theme_darkest    = COLOR_CORRECTION(0x444444);
    constexpr uint32_t theme_dark       = COLOR_CORRECTION(0x777777);

    constexpr uint32_t bg_color         = theme_darkest;
    constexpr uint32_t bg_text_disabled = theme_dark;
    constexpr uint32_t bg_text_enabled  = 0xFFFFFF;
    constexpr uint32_t bg_normal        = theme_darkest;

    constexpr uint32_t fg_normal        = theme_dark;
    constexpr uint32_t fg_action        = lulzbot_green;
    constexpr uint32_t fg_disabled      = bg_color;
  #else
    constexpr uint32_t theme_darkest    = 0x545923;
    constexpr uint32_t theme_dark       = lulzbot_bg;

    constexpr uint32_t bg_color         = 0xFFFFFF;
    constexpr uint32_t bg_text_disabled = 0x333333;
    constexpr uint32_t bg_text_enabled  = theme_darkest;
    constexpr uint32_t bg_normal        = theme_dark;

    constexpr uint32_t fg_normal        = theme_darkest;
    constexpr uint32_t fg_action        = theme_dark;
    constexpr uint32_t fg_disabled      = 0xEFEFEF;

    constexpr uint32_t shadow_rgb       = 0xE0E0E0;
    constexpr uint32_t fill_rgb         = lulzbot_fg;
    constexpr uint32_t stroke_rgb       = theme_darkest;
    constexpr uint32_t syringe_rgb      = 0xF1F6C0;
  #endif

  constexpr uint32_t x_axis        = COLOR_CORRECTION(0xFF0000);
  constexpr uint32_t y_axis        = COLOR_CORRECTION(0x00BB00);
  constexpr uint32_t z_axis        = COLOR_CORRECTION(0x0000FF);
  #ifndef LULZBOT_USE_BIOPRINTER_UI
  constexpr uint32_t e_axis        = COLOR_CORRECTION(0x777777);
  constexpr uint32_t feedrate      = COLOR_CORRECTION(0x777777);
  constexpr uint32_t other         = COLOR_CORRECTION(0x777777);
  #else
  constexpr uint32_t e_axis        = 0x000000;
  constexpr uint32_t feedrate      = 0x000000;
  constexpr uint32_t other         = 0x000000;
  #endif

  // Status screen
  constexpr uint32_t progress      = theme_dark;
  constexpr uint32_t status_msg    = theme_dark;
  constexpr uint32_t fan_speed     = COLOR_CORRECTION(0x3771CB);
  constexpr uint32_t temp          = COLOR_CORRECTION(0x892ca0);
  constexpr uint32_t axis_label    = theme_dark;

  constexpr uint32_t disabled_icon = 0x101010;

  // Calibration Registers Screen
  constexpr uint32_t transformA    = 0x3010D0;
  constexpr uint32_t transformB    = 0x4010D0;
  constexpr uint32_t transformC    = 0x5010D0;
  constexpr uint32_t transformD    = 0x6010D0;
  constexpr uint32_t transformE    = 0x7010D0;
  constexpr uint32_t transformF    = 0x8010D0;
  constexpr uint32_t transformVal  = 0x104010;

  constexpr btn_colors disabled_btn = {.bg = bg_color,      .grad = fg_disabled, .fg = fg_disabled,  .rgb = fg_disabled };
  constexpr btn_colors normal_btn   = {.bg = fg_action,     .grad = 0xFFFFFF,    .fg = fg_normal,    .rgb = 0xFFFFFF };
  constexpr btn_colors action_btn   = {.bg = bg_color,      .grad = 0xFFFFFF,    .fg = fg_action,    .rgb = 0xFFFFFF };
  constexpr btn_colors red_btn      = {.bg = 0xFF5555,      .grad = 0xFFFFFF,    .fg = 0xFF0000,     .rgb = 0xFFFFFF };
  constexpr btn_colors ui_slider    = {.bg = theme_darkest, .grad = 0xFFFFFF,    .fg = theme_dark,   .rgb = lulzbot_green };
  constexpr btn_colors ui_toggle    = {.bg = theme_darkest, .grad = 0xFFFFFF,    .fg = theme_dark,   .rgb = 0xFFFFFF };

  // Temperature color scale

  const rgb_t cool_rgb (  0,   0,   0);
  const rgb_t low_rgb  (128,   0,   0);
  const rgb_t med_rgb  (255, 128,   0);
  const rgb_t high_rgb (255, 255, 128);
};
