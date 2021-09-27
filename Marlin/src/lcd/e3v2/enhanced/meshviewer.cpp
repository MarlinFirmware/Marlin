/**
 * DWIN Mesh Viewer
 * Author: Miguel A. Risco-Castillo
 * version: 2.5
 * Date: 2021/09/27
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
 */

#include "../../../inc/MarlinConfigPre.h"

#if HAS_LEVELING

#include "../../../core/types.h"
#include "../../marlinui.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin.h"
#include "meshviewer.h"
#include "../../../feature/bedlevel/bedlevel.h"

MeshViewerClass MeshViewer;

void MeshViewerClass::Draw() {
  const int8_t mx = 30;  // Margins
  const int8_t my = 30;
  const int16_t stx = (DWIN_WIDTH - 2 * mx) / (GRID_MAX_POINTS_X - 1);  // Steps
  const int16_t sty = (DWIN_WIDTH - 2 * my) / (GRID_MAX_POINTS_Y - 1);
  int8_t zmesh[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
  int8_t maxz =-127;
  int8_t minz = 127;
  #define px(xp) (mx + (xp)*stx)
  #define py(yp) (30 + DWIN_WIDTH - my - (yp)*sty)
  #define rm(z) ((((z) - minz) * 10 / _MAX(1, (maxz - minz))) + 10)
  #define DrawMeshValue(xp, yp, zv)  DWINUI::Draw_Signed_Float(font6x12, 1, 2, px(xp) - 12, py(yp) - 6, zv);
  #define DrawMeshHLine(yp) DWIN_Draw_HLine(HMI_data.SplitLine_Color, px(0), py(yp), DWIN_WIDTH - 2 * mx)
  #define DrawMeshVLine(xp) DWIN_Draw_VLine(HMI_data.SplitLine_Color, px(xp), py(GRID_MAX_POINTS_Y - 1), DWIN_WIDTH - 2 * my)
  for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
    for (uint8_t y = 0; y < GRID_MAX_POINTS_X; y++) {
      zmesh[x][y] = Z_VALUES(x,y) * 100;
      maxz = _MAX(zmesh[x][y], maxz);
      minz = _MIN(zmesh[x][y], minz);
    }
  }
  Title.ShowCaption(F("Mesh viewer"));
  DWINUI::ClearMenuArea();
  DWINUI::Draw_Icon(ICON_Continue_E, 86, 305);
  DWIN_Draw_Rectangle(0, HMI_data.SplitLine_Color, px(0), py(0), px(GRID_MAX_POINTS_X - 1), py(GRID_MAX_POINTS_Y - 1));
  for (uint8_t x = 1; x < GRID_MAX_POINTS_X - 1; x++) DrawMeshVLine(x);
  for (uint8_t y = 1; y < GRID_MAX_POINTS_Y - 1; y++) DrawMeshHLine(y);
  for (uint8_t y = 0; y < GRID_MAX_POINTS_X; y++) {
    watchdog_refresh();
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
      uint16_t color = DWINUI::RainbowInt(zmesh[x][y], _MIN(-5, minz), _MAX(5, maxz));
      DWINUI::Draw_FillCircle(color, px(x), py(y), rm(zmesh[x][y]));
      DrawMeshValue(x, y, Z_VALUES(x,y));
    }
  }
  char str_1[6], str_2[6] = "";
  ui.status_printf_P(0,PSTR("Mesh minZ: %s, maxZ: %s"),
    dtostrf((float)minz/100, 1, 2, str_1),
    dtostrf((float)maxz/100, 1, 2, str_2));
}

#endif // HAS_LEVELING
