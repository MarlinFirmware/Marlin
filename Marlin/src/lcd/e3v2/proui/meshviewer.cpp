/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Mesh Viewer for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 4.2.1
 * Date: 2023/05/05
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, HAS_MESH)

#include "../../../core/types.h"
#include "../../marlinui.h"
#include "dwin.h"
#include "dwin_popup.h"
#include "../../../feature/bedlevel/bedlevel.h"
#include "meshviewer.h"

#if ENABLED(USE_GRID_MESHVIEWER)
  #include "bedlevel_tools.h"
#endif

bool meshredraw;                            // Redraw mesh points
uint8_t sizex, sizey;                       // Mesh XY size
uint8_t rmax;                               // Maximum radius
#define margin 25                           // XY Margins
#define rmin 5                              // Minimum radius
#define zmin -20                            // rmin at z=-0.20
#define zmax  20                            // rmax at z= 0.20
#define width DWIN_WIDTH - 2 * margin
#define r(z) ((z - zmin) * (rmax - rmin) / (zmax - zmin) + rmin)
#define px(xp) (margin + (xp) * (width) / (sizex - 1))
#define py(yp) (30 + DWIN_WIDTH - margin - (yp) * (width) / (sizey - 1))

constexpr uint8_t meshfont = TERN(TJC_DISPLAY, font8x16, font6x12);

MeshViewerClass MeshViewer;

float MeshViewerClass::max, MeshViewerClass::min;

void MeshViewerClass::DrawMeshGrid(const uint8_t csizex, const uint8_t csizey) {
  sizex = csizex;
  sizey = csizey;
  rmax = _MIN(margin - 2, 0.5 * (width) / (sizex - 1));
  min = 100;
  max = -100;
  DWINUI::ClearMainArea();
  dwinDrawRectangle(0, HMI_data.SplitLine_Color, px(0), py(0), px(sizex - 1), py(sizey - 1));
  for (uint8_t x = 1; x < sizex - 1; ++x) dwinDrawVLine(HMI_data.SplitLine_Color, px(x), py(sizey - 1), width);
  for (uint8_t y = 1; y < sizey - 1; ++y) dwinDrawHLine(HMI_data.SplitLine_Color, px(0), py(y), width);
}

void MeshViewerClass::DrawMeshPoint(const uint8_t x, const uint8_t y, const float z) {
  const uint8_t fs = DWINUI::fontWidth(meshfont);
  const int16_t v = isnan(z) ? 0 : round(z * 100);
  NOLESS(max, z);
  NOMORE(min, z);
  const uint16_t color = DWINUI::RainbowInt(v, zmin, zmax);
  DWINUI::Draw_FillCircle(color, px(x), py(y), r(_MAX(_MIN(v,zmax),zmin)));
  TERN_(TJC_DISPLAY, delay(100));
  if (sizex < (ENABLED(TJC_DISPLAY) ? 8 : 9)) {
    if (v == 0) DWINUI::Draw_Float(meshfont, 1, 2, px(x) - 2*fs, py(y) - fs, 0);
    else DWINUI::Draw_Signed_Float(meshfont, 1, 2, px(x) - 3*fs, py(y) - fs, z);
  }
  else {
    char str_1[9];
    str_1[0] = '\0';
    switch (v) {
      case -999 ... -100:
        DWINUI::Draw_Signed_Float(meshfont, 1, 1, px(x) - 3*fs, py(y) - fs, z);
        break;
      case -99 ... -1:
        sprintf_P(str_1, PSTR("-.%02i"), -v);
        break;
      case 0:
        dwinDrawString(false, meshfont, DWINUI::textcolor, DWINUI::backcolor, px(x) - 4, py(y) - fs, "0");
        break;
      case 1 ... 99:
        sprintf_P(str_1, PSTR(".%02i"), v);
        break;
      case 100 ... 999:
        DWINUI::Draw_Signed_Float(meshfont, 1, 1, px(x) - 3 * fs, py(y) - fs, z);
        break;
    }
    if (str_1[0])
      dwinDrawString(false, meshfont, DWINUI::textcolor, DWINUI::backcolor, px(x) - 2 * fs, py(y) - fs, str_1);
  }
}

void MeshViewerClass::DrawMesh(const bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey) {
  DrawMeshGrid(csizex, csizey);
  for (uint8_t y = 0; y < csizey; ++y) {
    hal.watchdog_refresh();
    for (uint8_t x = 0; x < csizex; ++x) DrawMeshPoint(x, y, zval[x][y]);
  }
}

void MeshViewerClass::Draw(const bool withsave/*=false*/, const bool redraw/*=true*/) {
  Title.ShowCaption(GET_TEXT_F(MSG_MESH_VIEWER));
  #if ENABLED(USE_GRID_MESHVIEWER)
    DWINUI::ClearMainArea();
    bedLevelTools.viewer_print_value = true;
    bedLevelTools.Draw_Bed_Mesh(-1, 1, 8, 10 + TITLE_HEIGHT);
  #else
    if (redraw) DrawMesh(bedlevel.z_values, GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y);
    else DWINUI::Draw_Box(1, HMI_data.Background_Color, { 89, 305, 99, 38 });
  #endif
  if (withsave) {
    DWINUI::Draw_Button(BTN_Save, 26, 305);
    DWINUI::Draw_Button(BTN_Continue, 146, 305);
    Draw_Select_Highlight(hmiFlag.select_flag, 305);
  }
  else
    DWINUI::Draw_Button(BTN_Continue, 86, 305);

  #if ENABLED(USE_GRID_MESHVIEWER)
    bedLevelTools.Set_Mesh_Viewer_Status();
  #else
    char str_1[6], str_2[6] = "";
    ui.status_printf(0, F("Mesh minZ: %s, maxZ: %s"), dtostrf(min, 1, 2, str_1), dtostrf(max, 1, 2, str_2));
  #endif
}

void Draw_MeshViewer() { MeshViewer.Draw(true, meshredraw); }

void onClick_MeshViewer() { if (hmiFlag.select_flag) SaveMesh(); HMI_ReturnScreen(); }

void Goto_MeshViewer(const bool redraw) {
  meshredraw = redraw;
  if (leveling_is_valid()) Goto_Popup(Draw_MeshViewer, onClick_MeshViewer);
  else HMI_ReturnScreen();
}

#endif // DWIN_LCD_PROUI && HAS_MESH
