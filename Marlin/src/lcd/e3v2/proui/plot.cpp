/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * DWIN Single var plot
 * Author: Miguel A. Risco-Castillo
 * Version: 2.2.3
 * Date: 2023/01/29
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "dwin_defines.h"

#if PROUI_TUNING_GRAPH

#include "dwin.h"
#include "../../marlinui.h"
#include "plot.h"

#define Plot_Bg_Color RGB( 1, 12,  8)

Plot plot;

uint16_t graphpoints, r, x2, y2 = 0;
frame_rect_t graphframe = {0};
float scale = 0;

void Plot::draw(const frame_rect_t &frame, const_celsius_float_t max, const_float_t ref/*=0*/) {
  graphframe = frame;
  graphpoints = 0;
  scale = frame.h / max;
  x2 = frame.x + frame.w - 1;
  y2 = frame.y + frame.h - 1;
  r = round((y2) - ref * scale);
  DWINUI::drawBox(1, Plot_Bg_Color, frame);
  for (uint8_t i = 1; i < 4; i++) if (i * 60 < frame.w) dwinDrawVLine(COLOR_LINE, i * 60 + frame.x, frame.y, frame.h);
  DWINUI::drawBox(0, COLOR_WHITE, DWINUI::extendFrame(frame, 1));
  dwinDrawHLine(COLOR_RED, frame.x, r, frame.w);
}

void Plot::update(const_float_t value) {
  if (!scale) return;
  const uint16_t y = round((y2) - value * scale);
  if (graphpoints < graphframe.w) {
    dwinDrawPoint(COLOR_YELLOW, 1, 1, graphpoints + graphframe.x, y);
  }
  else {
    dwinFrameAreaMove(1, 0, 1, Plot_Bg_Color, graphframe.x, graphframe.y, x2, y2);
    if ((graphpoints % 60) == 0) dwinDrawVLine(COLOR_LINE, x2 - 1, graphframe.y + 1, graphframe.h - 2);
    dwinDrawPoint(COLOR_RED, 1, 1, x2 - 1, r);
    dwinDrawPoint(COLOR_YELLOW, 1, 1, x2 - 1, y);
  }
  graphpoints++;
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    ui.refresh_backlight_timeout();
  #endif
}

#endif // PROUI_TUNING_GRAPH
#endif // DWIN_LCD_PROUI
