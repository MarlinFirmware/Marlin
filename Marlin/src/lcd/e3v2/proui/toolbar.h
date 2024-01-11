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

#pragma once

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TOOLBAR && DISABLED(PROUI_EX)
  #error "HAS_TOOLBAR requires PROUI_EX."
#endif

#include "dwin.h"
#include "menus.h"

#define TBHEIGHT 28
#define TBYOFFSET 20
#define TBYPOS (STATUS_Y - (TBHEIGHT + 5))
#define B_YPOS (TBYPOS + 5)
#define B_XPOS 24
#define TBMaxCaptionWidth 18

typedef struct {
  uint8_t icon = 0;
  FSTR_P caption = nullptr;
  void (*onClick)() = nullptr;
} TBItem_t;
extern const TBItem_t *TBItem;
extern const TBItem_t TBItemA[];

class ToolBar : public Menu {
public:
  void onScroll(bool dir) override;
  void draw() override;
  void (*onExit)() = nullptr;
  uint8_t OptCount();
};
extern ToolBar toolBar;

void TBGetItem(uint8_t item);
void onDrawTBItem(int8_t pos, int8_t line);
void updateTBSetupItem(int8_t pos, uint8_t val);
void drawTBSetupItem(bool focused);
void drawToolBar();
inline void redrawToolBar() { invalidateMenu(); return drawToolBar(); };
