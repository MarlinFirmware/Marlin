/************
 * colors.h *
 ************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2019 - Cocoa Press                          *
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
  #ifdef TOUCH_UI_LULZBOT_BIO
    // The Lulzbot Bio uses the color PANTONE 2175C on the case silkscreen.
    // This translates to HSL(208°, 100%, 39%) as an accent color on the GUI.

    constexpr int   accent_hue          = 208;
    constexpr float accent_sat          = 0.5;

    constexpr uint32_t logo_bg_rgb      = 0xffffff;
    constexpr uint32_t logo_fill_rgb    = 0xffffff;
    constexpr uint32_t logo_stroke_rgb  = hsl_to_rgb(accent_hue, 1.0, 0.39);
  #else
    // The Lulzbot logo uses the color PANTONE 382c.
    // This translates to HSL(68°, 68%, 52%) as an accent color on the GUI.

    constexpr int   accent_hue          = 68;
    constexpr float accent_sat          = 0.68;

    constexpr uint32_t logo_bg_rgb      = hsl_to_rgb(accent_hue, 0.77, 0.64);
    constexpr uint32_t logo_fill_rgb    = hsl_to_rgb(accent_hue, 0.68, 0.52); // Lulzbot Green
    constexpr uint32_t logo_stroke_rgb  = 0x000000;
  #endif

  // Shades of accent color

  #ifdef TOUCH_UI_COCOA_PRESS
    constexpr uint32_t accent_color_1   = hsl_to_rgb(12.8,0.597,0.263); // Darkest
    constexpr uint32_t accent_color_2   = hsl_to_rgb(12.8,0.597,0.263);
    constexpr uint32_t accent_color_3   = hsl_to_rgb( 9.6,0.664,0.443);
    constexpr uint32_t accent_color_4   = hsl_to_rgb(16.3,0.873,0.537);
    constexpr uint32_t accent_color_5   = hsl_to_rgb(23.0,0.889,0.539);
    constexpr uint32_t accent_color_6   = hsl_to_rgb(23.0,0.889,0.539); // Lightest
  #else
    constexpr uint32_t accent_color_1   = hsl_to_rgb(accent_hue, accent_sat, 0.26); // Darkest
    constexpr uint32_t accent_color_2   = hsl_to_rgb(accent_hue, accent_sat, 0.39);
    constexpr uint32_t accent_color_3   = hsl_to_rgb(accent_hue, accent_sat, 0.52);
    constexpr uint32_t accent_color_4   = hsl_to_rgb(accent_hue, accent_sat, 0.65);
    constexpr uint32_t accent_color_5   = hsl_to_rgb(accent_hue, accent_sat, 0.78);
    constexpr uint32_t accent_color_6   = hsl_to_rgb(accent_hue, accent_sat, 0.91); // Lightest
  #endif

  // Shades of gray

  constexpr float gray_sat = 0.14;

  constexpr uint32_t gray_color_1       = hsl_to_rgb(accent_hue, gray_sat, 0.26); // Darkest
  constexpr uint32_t gray_color_2       = hsl_to_rgb(accent_hue, gray_sat, 0.39);
  constexpr uint32_t gray_color_3       = hsl_to_rgb(accent_hue, gray_sat, 0.52);
  constexpr uint32_t gray_color_4       = hsl_to_rgb(accent_hue, gray_sat, 0.65);
  constexpr uint32_t gray_color_5       = hsl_to_rgb(accent_hue, gray_sat, 0.78);
  constexpr uint32_t gray_color_6       = hsl_to_rgb(accent_hue, gray_sat, 0.91); // Lightest

  #if DISABLED(TOUCH_UI_LULZBOT_BIO) && DISABLED(TOUCH_UI_COCOA_PRESS)
    // Lulzbot TAZ Pro
    constexpr uint32_t theme_darkest    = gray_color_1;
    constexpr uint32_t theme_dark       = gray_color_2;

    constexpr uint32_t bg_color         = theme_darkest;
    constexpr uint32_t bg_text_disabled = theme_dark;
    constexpr uint32_t bg_text_enabled  = 0xFFFFFF;
    constexpr uint32_t bg_normal        = theme_darkest;

    constexpr uint32_t fg_normal        = theme_dark;
    constexpr uint32_t fg_action        = accent_color_2;
    constexpr uint32_t fg_disabled      = theme_darkest;
  #else
    // Lulzbot Bio
    constexpr uint32_t theme_darkest    = accent_color_1;
    constexpr uint32_t theme_dark       = accent_color_4;

    constexpr uint32_t bg_color         = 0xFFFFFF;
    constexpr uint32_t bg_text_disabled = gray_color_1;
    constexpr uint32_t bg_text_enabled  = accent_color_1;
    constexpr uint32_t bg_normal        = accent_color_4;

    constexpr uint32_t fg_normal        = accent_color_1;
    constexpr uint32_t fg_action        = accent_color_4;
    constexpr uint32_t fg_disabled      = gray_color_6;

    constexpr uint32_t shadow_rgb       = gray_color_6;
    constexpr uint32_t stroke_rgb       = accent_color_1;
    constexpr uint32_t fill_rgb         = accent_color_3;
    constexpr uint32_t syringe_rgb      = accent_color_5;
  #endif

  constexpr uint32_t x_axis             = 0xFF0000;
  constexpr uint32_t y_axis             = 0x00BB00;
  constexpr uint32_t z_axis             = 0x0000BF;
  constexpr uint32_t e_axis             = gray_color_2;
  constexpr uint32_t feedrate           = gray_color_2;
  constexpr uint32_t other              = gray_color_2;

  // Status screen
  constexpr uint32_t progress           = gray_color_2;
  constexpr uint32_t status_msg         = gray_color_2;
  constexpr uint32_t fan_speed          = 0x377198;
  constexpr uint32_t temp               = 0x892c78;
  constexpr uint32_t axis_label         = gray_color_2;

  constexpr uint32_t disabled_icon      = gray_color_1;

  // Calibration Registers Screen
  constexpr uint32_t transformA         = 0x3010D0;
  constexpr uint32_t transformB         = 0x4010D0;
  constexpr uint32_t transformC         = 0x5010D0;
  constexpr uint32_t transformD         = 0x6010D0;
  constexpr uint32_t transformE         = 0x7010D0;
  constexpr uint32_t transformF         = 0x8010D0;
  constexpr uint32_t transformVal       = 0x104010;

  constexpr btn_colors disabled_btn     = {.bg = bg_color,      .grad = fg_disabled, .fg = fg_disabled,  .rgb = fg_disabled };
  constexpr btn_colors normal_btn       = {.bg = fg_action,     .grad = 0xFFFFFF,    .fg = fg_normal,    .rgb = 0xFFFFFF };
  constexpr btn_colors action_btn       = {.bg = bg_color,      .grad = 0xFFFFFF,    .fg = fg_action,    .rgb = 0xFFFFFF };
  constexpr btn_colors red_btn          = {.bg = 0xFF5555,      .grad = 0xFFFFFF,    .fg = 0xFF0000,     .rgb = 0xFFFFFF };
  constexpr btn_colors ui_slider        = {.bg = theme_darkest, .grad = 0xFFFFFF,    .fg = theme_dark,   .rgb = accent_color_3 };
  constexpr btn_colors ui_toggle        = {.bg = theme_darkest, .grad = 0xFFFFFF,    .fg = theme_dark,   .rgb = 0xFFFFFF };

  // Temperature color scale

  const rgb_t cool_rgb (  0,   0,   0);
  const rgb_t low_rgb  (128,   0,   0);
  const rgb_t med_rgb  (255, 128,   0);
  const rgb_t high_rgb (255, 255, 128);
};
