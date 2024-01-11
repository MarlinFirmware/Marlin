/**
 * toolBar for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * version: 3.1.1
 * Date: 2023/09/12
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

#include "../../../inc/MarlinConfig.h"

#if ALL(DWIN_LCD_PROUI, HAS_TOOLBAR)

#include "dwin.h"
#include "toolbar.h"
#include "toolbar_def.h"
#include "menus.h"

const TBItem_t *TBItem;
ToolBar toolBar;

uint8_t ToolBar::OptCount() {
  return COUNT(TBItemA);
}

void onDrawTBItem(int8_t pos, int8_t line) {
  const bool focused = (checkkey == ID_Menu);
  const int8_t sel = toolBar.selected;
  const uint8_t tw = focused ? MENU_CHR_W * TBMaxCaptionWidth : 0;
  const uint8_t xoff = (DWIN_WIDTH - (B_XPOS * toolBar.count + tw)) / 2;
  const uint8_t xp = xoff + line * B_XPOS + (line > sel ? tw : 0);
  if (focused && (line == sel)) {
    dwinDrawBox(1, COLOR_BG_WINDOW, xp - 2, TBYPOS, B_XPOS, TBHEIGHT);
    DWINUI::drawString(xp + B_XPOS, B_YPOS + 1, getMenuItem(pos)->caption);
  }
  DWINUI::drawIcon(getMenuItem(pos)->icon, xp, B_YPOS);
};

void drawToolBar() {
  if (notCurrentMenu(&toolBar)) {
    for (uint8_t i = 0; i < TBMaxOpt; ++i) {
      TBGetItem(PRO_data.TBopt[i]);
      if (TBItem->icon) MENU_ITEM_F(TBItem->icon, TBItem->caption, onDrawTBItem, TBItem->onClick);
    }
    toolBar.onExit = &exitToolBar;
    toolBar.count = menuItemCount;
    currentMenu = &toolBar;
  }
  toolBar.draw();
}

void updateTBSetupItem(int8_t pos, uint8_t val) {
  TBGetItem(val);
  getMenuItem(pos)->icon = TBItem->icon ?: ICON_Info;
  getMenuItem(pos)->caption = TBItem->caption;
}

void drawTBSetupItem(bool focused) {
  const uint8_t line = currentMenu->line();
  const uint16_t ypos = MYPOS(line) + 1;
  DWINUI::drawBox(1, focused ? COLOR_BG_BLACK : hmiData.colorBackground, { 15, ypos, DWIN_WIDTH - 15, MLINE - 1 });
  onDrawMenuItem(currentMenu->selected, line);
  if (focused) DWINUI::drawChar(VALX + 24, MBASE(line), 18);
}

void TBGetItem(uint8_t item) {
  const uint8_t N = toolBar.OptCount() - 1;
  if (WITHIN(item, 1, N))
    TBItem = &TBItemA[item];
  else
    TBItem = &TBItemA[0];
}

#endif // ALL(DWIN_LCD_PROUI, HAS_TOOLBAR)