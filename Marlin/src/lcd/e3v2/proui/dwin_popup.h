/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 4.1.1
 * Date: 2023/07/12
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
#pragma once

#include "dwin.h"
#include "dwinui.h"

typedef void (*popupDrawFunc_t)();
typedef void (*popupClickFunc_t)();
typedef void (*popupChangeFunc_t)(const bool state);
extern popupDrawFunc_t popupDraw;

void drawSelectHighlight(const bool sel, const uint16_t ypos);
inline void drawSelectHighlight(const bool sel) { drawSelectHighlight(sel, 280); };
void dwinPopupContinue(const uint8_t icon, FSTR_P const fmsg1, FSTR_P const fmsg2);
void dwinPopupConfirmCancel(const uint8_t icon, FSTR_P const fmsg2);
void gotoPopup(const popupDrawFunc_t fnDraw, const popupClickFunc_t fnClick=nullptr, const popupChangeFunc_t fnChange=nullptr);
void hmiPopup();

inline void drawPopupBkgd() {
  dwinDrawRectangle(1, hmiData.colorPopupBg, 14, 60, 258, 330);
  dwinDrawRectangle(0, hmiData.colorHighlight, 14, 60, 258, 330);
}

template<typename T, typename U>
void dwinDrawPopup(const uint8_t icon, T amsg1=nullptr, U amsg2=nullptr, uint8_t button=0) {
  uint8_t xpos = 81;
  switch (icon) {
    case 17 ... 24: xpos = 96; break;
    case 78 ... 81: xpos = 101; break;
  }
  DWINUI::clearMainArea();
  drawPopupBkgd();
  if (icon) DWINUI::drawIcon(icon, xpos, 105);
  if (amsg1) DWINUI::drawCenteredString(hmiData.colorPopupTxt, 210, amsg1);
  if (amsg2) DWINUI::drawCenteredString(hmiData.colorPopupTxt, 240, amsg2);
  if (button) DWINUI::drawButton(button, 86, 280);
}

template<typename T, typename U>
void dwinShowPopup(const uint8_t icon, T amsg1=nullptr, U amsg2=nullptr, uint8_t button=0) {
  dwinDrawPopup(icon, amsg1, amsg2, button);
  dwinUpdateLCD();
}
