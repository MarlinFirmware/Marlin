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
 * Version: 2.1.2
 * Date: 2022/11/20
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For commercial applications additional licenses can be requested
 */

#include "../../../inc/MarlinConfigPre.h"

#if BOTH(DWIN_LCD_PROUI, HAS_PIDPLOT)

#include "plot.h"
#include "../../../core/types.h"
#include "../../marlinui.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin.h"

#define Plot_Bg_Color RGB( 1, 12,  8)

PlotClass Plot;

uint16_t grphpoints, r, x2, y2 = 0;
frame_rect_t grphframe = {0};
float scale = 0;

void PlotClass::Draw(const frame_rect_t frame, const float max, const float ref) {
  grphframe = frame;
  grphpoints = 0;
  scale = frame.h / max;
  x2 = frame.x + frame.w - 1;
  y2 = frame.y + frame.h - 1;
  r = round((y2) - ref * scale);
  DWINUI::Draw_Box(1, Plot_Bg_Color, frame);
  for (uint8_t i = 1; i < 4; i++) if (i * 50 < frame.w) DWIN_Draw_VLine(Line_Color, i * 50 + frame.x, frame.y, frame.h);
  DWINUI::Draw_Box(0, Color_White, DWINUI::ExtendFrame(frame, 1));
  DWIN_Draw_HLine(Color_Red, frame.x, r, frame.w);
}

void PlotClass::Update(const float value) {
  if (!scale) return;
  uint16_t y = round((y2) - value * scale);
  if (grphpoints < grphframe.w) {
    DWIN_Draw_Point(Color_Yellow, 1, 1, grphpoints + grphframe.x, y);
  }
  else {
    DWIN_Frame_AreaMove(1, 0, 1, Plot_Bg_Color, grphframe.x, grphframe.y, x2, y2);
    if ((grphpoints % 50) == 0) DWIN_Draw_VLine(Line_Color, x2 - 1, grphframe.y + 1, grphframe.h - 2);
    DWIN_Draw_Point(Color_Red, 1, 1, x2 - 1, r);
    DWIN_Draw_Point(Color_Yellow, 1, 1, x2 - 1, y);
  }
  grphpoints++;
}

#endif // DWIN_LCD_PROUI && HAS_PIDPLOT
