/**
 * Mesh Viewer for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 5.2.1
 * Date: 2023/08/19
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

#if ALL(DWIN_LCD_PROUI, HAS_MESH) && DISABLED(PROUI_EX)

#include "../../../core/types.h"
#include "../../marlinui.h"
#include "dwin.h"
#include "dwin_popup.h"
#include "../../../feature/bedlevel/bedlevel.h"
#include "meshviewer.h"

bool meshredraw;                            // Redraw mesh points
uint8_t sizex, sizey;                       // Mesh XY size
uint8_t rmax;                               // Maximum radius
#define margin 25                           // XY Margins
#define rmin 5                              // Minimum radius
#define zmin -20                            // rmin at z=-0.20
#define zmax  20                            // rmax at z= 0.20
#define width DWIN_WIDTH - 2 * margin
#define r(z) ((z-zmin)*(rmax-rmin)/(zmax-zmin)+rmin)
#define px(xp) (margin + (xp)*(width)/(sizex - 1))
#define py(yp) (30 + DWIN_WIDTH - margin - (yp)*(width)/(sizey - 1))

#if ENABLED(TJC_DISPLAY)
  #define meshfont font8x16
#else
  #define meshfont font6x12
#endif

MeshViewer meshViewer;

float MeshViewer::max, MeshViewer::min;

void MeshViewer::drawBackground(const uint8_t csizex, const uint8_t csizey) {
  sizex = csizex;
  sizey = csizey;
  rmax = _MIN(margin - 2, 0.5*(width)/(sizex - 1));
  min = 100;
  max = -100;
  DWINUI::clearMainArea();
  dwinDrawRectangle(0, hmiData.colorSplitLine, px(0), py(0), px(sizex - 1), py(sizey - 1));
  for (uint8_t x = 1; x < sizex - 1; ++x) dwinDrawVLine(hmiData.colorSplitLine, px(x), py(sizey - 1), width);
  for (uint8_t y = 1; y < sizey - 1; ++y) dwinDrawHLine(hmiData.colorSplitLine, px(0), py(y), width);
}

void MeshViewer::drawPoint(const uint8_t x, const uint8_t y, const float z) {
  if (isnan(z)) return;
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    ui.refresh_backlight_timeout();
  #endif
  const uint8_t fs = DWINUI::fontWidth(meshfont);
  int16_t v = round(z * 100);
  NOLESS(max, z); NOMORE(min, z);
  const uint16_t color = DWINUI::rainbowInt(v, zmin, zmax);
  DWINUI::drawFillCircle(color, px(x), py(y), r(_MAX(_MIN(v,zmax),zmin)));
  if (sizex < (ENABLED(TJC_DISPLAY) ? 8 : 9)) {
    if (v == 0) dwinDrawString(false, meshfont, DWINUI::textColor, DWINUI::backColor, px(x) - 2*fs - 1, py(y) - fs, "0.00");
    else DWINUI::drawSignedFloat(meshfont, 1, 2, px(x) - 3*fs, py(y) - fs, z);
  }
  else {
    char str_1[9] = "0";
    switch (v) {
      case -99 ... -1:  // -.99 .. -.01 mm
        sprintf_P(str_1, PSTR("-.%02i"), -v);
        break;
      case 1 ... 99:  // .01 .. .99 mm
        sprintf_P(str_1, PSTR(".%02i"), v);
        break;
      case -999 ... -100:  // -9.9 .. -1.0 mm
      case  100 ...  999:  // 1.0 .. 9.9 mm
        dtostrf(z, 1, 1, str_1);
        break;
    }
    dwinDrawString(false, meshfont, DWINUI::textColor, DWINUI::backColor, px(x) - 2 * fs, py(y) - fs, str_1);
  }
  SERIAL_FLUSH();
  TERN_(TJC_DISPLAY, delay(100));
}

void MeshViewer::drawMesh(const bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey) {
  drawBackground(csizex, csizey);
  for (uint8_t y = 0; y < csizey; ++y) {
    hal.watchdog_refresh();
    for (uint8_t x = 0; x < csizex; ++x) drawPoint(x, y, zval[x][y]);
  }
}

void MeshViewer::drawViewer(const bool withsave/*=false*/, const bool redraw/*=true*/) {
  title.draw(GET_TEXT_F(MSG_MESH_VIEWER));
  if (redraw) drawMesh(bedlevel.z_values, GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y); // Draw complete mesh viewer
  else DWINUI::drawBox(1, hmiData.colorBackground, { 89, 305, 99, 38 }); // Erase "Continue" button
  if (withsave) {
    DWINUI::drawButton(BTN_Save, 26, 305);
    DWINUI::drawButton(BTN_Continue, 146, 305);
    drawSelectHighlight(hmiFlag.select_flag, 305);
  }
  else DWINUI::drawButton(BTN_Continue, 86, 305);
  char str_1[6], str_2[6] = "";
  ui.status_printf(0, F("Mesh minZ: %s, maxZ: %s"),
    dtostrf(min, 1, 2, str_1),
    dtostrf(max, 1, 2, str_2)
  );
}

void drawMeshViewer() { meshViewer.drawViewer(true, meshredraw); }

void onClick_MeshViewer() { if (hmiFlag.select_flag) saveMesh(); hmiReturnScreen(); }

void gotoMeshViewer(const bool redraw) {
  meshredraw = redraw;
  if (leveling_is_valid()) gotoPopup(drawMeshViewer, onClick_MeshViewer);
  else hmiReturnScreen();
}

#endif // DWIN_LCD_PROUI && HAS_MESH && DISABLED(PROUI_EX)
