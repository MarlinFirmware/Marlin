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

#define Plot_Bg_Color RGB(1, 12, 8)

Plot plot;

void Plot::draw(const frame_rect_t &frame, const_celsius_float_t max, const_celsius_float_t ref/*=0*/) {
  plot.graphframe = frame;
  plot.graphpoints = 0;
  plot.scale = frame.h / max;
  plot.x2 = frame.x + frame.w - 1;
  plot.y2 = frame.y + frame.h - 1;
  plot.r = LROUND((plot.y2) - ref * plot.scale);
  DWINUI::drawBox(1, Plot_Bg_Color, frame);
  for (uint8_t i = 1; i < 4; i++) if (i * 60 < frame.w) dwinDrawVLine(COLOR_LINE, i * 60 + frame.x, frame.y, frame.h);
  DWINUI::drawBox(0, COLOR_WHITE, DWINUI::extendFrame(frame, 1));
  dwinDrawHLine(COLOR_RED, frame.x, plot.r, frame.w);
}

void Plot::update(const_celsius_float_t value) {
  if (!plot.scale) return;
  const uint16_t y = LROUND((plot.y2) - value * plot.scale);
  if (plot.graphpoints < plot.graphframe.w) {
    if (plot.graphpoints < 1) {
      dwinDrawPoint(COLOR_YELLOW, 1, 1, plot.graphframe.x, y);
    }
    else {
      dwinDrawLine(COLOR_YELLOW, plot.graphpoints + plot.graphframe.x - 1, plot.yP, plot.graphpoints + plot.graphframe.x, y);
    }
  }
  else {
    dwinFrameAreaMove(1, 0, 1, Plot_Bg_Color, plot.graphframe.x, plot.graphframe.y, plot.x2, plot.y2);
    if ((plot.graphpoints % 60) == 0) dwinDrawVLine(COLOR_LINE, plot.x2 - 1, plot.graphframe.y + 1, plot.graphframe.h - 2);
    dwinDrawPoint(COLOR_RED, 1, 1, plot.x2 - 1, plot.r);
    dwinDrawLine(COLOR_YELLOW, plot.x2 - 2, plot.yP, plot.x2 - 1, y);
  }
  plot.yP = y;
  plot.graphpoints++;
  TERN_(HAS_BACKLIGHT_TIMEOUT, ui.refresh_backlight_timeout());
}

#endif // PROUI_TUNING_GRAPH
#endif // DWIN_LCD_PROUI
