/****************************
 * bed_mesh_view_screen.cpp *
 ****************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2020                                        *
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"
#include "../screen_data.h"

#ifdef FTDI_BED_MESH_VIEW_SCREEN

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

constexpr static BedMeshViewScreenData &mydata = screen_data.BedMeshViewScreen;
constexpr static float gaugeThickness = 0.25;

#if ENABLED(TOUCH_UI_PORTRAIT)
  #define GRID_COLS 3
  #define GRID_ROWS 10

  #define MESH_POS    BTN_POS(1, 2), BTN_SIZE(3,5)
  #define MESSAGE_POS BTN_POS(1, 7), BTN_SIZE(3,1)
  #define Z_LABEL_POS BTN_POS(1, 8), BTN_SIZE(1,1)
  #define Z_VALUE_POS BTN_POS(2, 8), BTN_SIZE(2,1)
  #define OKAY_POS    BTN_POS(1,10), BTN_SIZE(3,1)
#else
  #define GRID_COLS 5
  #define GRID_ROWS 5

  #define MESH_POS    BTN_POS(1,1), BTN_SIZE(3,5)
  #define MESSAGE_POS BTN_POS(4,1), BTN_SIZE(2,1)
  #define Z_LABEL_POS BTN_POS(4,2), BTN_SIZE(2,1)
  #define Z_VALUE_POS BTN_POS(4,3), BTN_SIZE(2,1)
  #define OKAY_POS    BTN_POS(4,5), BTN_SIZE(2,1)
#endif

static float meshGetter(uint8_t x, uint8_t y, void*) {
  xy_uint8_t pos;
  pos.x = x;
  pos.y = y;
  return ExtUI::getMeshPoint(pos);
}

void BedMeshViewScreen::onEntry() {
  mydata.highlight.x = -1;
  mydata.count = GRID_MAX_POINTS;
  mydata.message = nullptr;
  BaseScreen::onEntry();
}

void BedMeshViewScreen::drawHighlightedPointValue() {
  CommandProcessor cmd;
  cmd.font(Theme::font_medium)
     .cmd(COLOR_RGB(bg_text_enabled))
     .text(Z_LABEL_POS, GET_TEXT_F(MSG_MESH_EDIT_Z))
     .font(font_small);

  if (mydata.highlight.x != -1)
    draw_adjuster_value(cmd, Z_VALUE_POS, ExtUI::getMeshPoint(mydata.highlight), GET_TEXT_F(MSG_UNITS_MM), 4, 3);

  cmd.colors(action_btn)
     .tag(1).button(OKAY_POS, GET_TEXT_F(MSG_BUTTON_OKAY))
     .tag(0);

  if (mydata.message) cmd.text(MESSAGE_POS, mydata.message);
}

void BedMeshViewScreen::onRedraw(draw_mode_t what) {
  #define _INSET_POS(x,y,w,h) x + min(w,h)/10, y + min(w,h)/10, w - min(w,h)/5, h - min(w,h)/5
  #define INSET_POS(pos) _INSET_POS(pos)

  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true));
    drawMeshBackground(cmd, INSET_POS(MESH_POS));
  }

  if (what & FOREGROUND) {
    const float progress = sq(float(mydata.count) / GRID_MAX_POINTS);
    if (progress >= 1.0)
      drawHighlightedPointValue();
    drawMeshForeground(cmd, INSET_POS(MESH_POS), meshGetter, nullptr, pointToTag(mydata.highlight.x, mydata.highlight.y), progress);
  }
}

bool BedMeshViewScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); return true;
    default: return tagToPoint(tag, mydata.highlight);
  }
  return true;
}

void BedMeshViewScreen::onMeshUpdate(const int8_t, const int8_t, const float) {
  if (AT_SCREEN(BedMeshViewScreen)) {
    onRefresh();
    ExtUI::yield();
  }
}

void BedMeshViewScreen::onMeshUpdate(const int8_t x, const int8_t y, const ExtUI::probe_state_t state) {
  switch (state) {
    case ExtUI::G29_START:
      mydata.message = nullptr;
      mydata.count = 0;
      break;
    case ExtUI::G29_FINISH:
      if (mydata.count == GRID_MAX_POINTS && ExtUI::getMeshValid())
        mydata.message = GET_TEXT_F(MSG_BED_MAPPING_DONE);
      else
        mydata.message = GET_TEXT_F(MSG_BED_MAPPING_INCOMPLETE);
      mydata.count = GRID_MAX_POINTS;
      break;
    case ExtUI::G26_START:
      GOTO_SCREEN(BedMeshViewScreen);
      mydata.message = nullptr;
      mydata.count = 0;
      break;
    case ExtUI::G26_FINISH:
      GOTO_SCREEN(StatusScreen);
      break;
    case ExtUI::G29_POINT_START:
    case ExtUI::G26_POINT_START:
      mydata.highlight.x = x;
      mydata.highlight.y = y;
      break;
    case ExtUI::G29_POINT_FINISH:
    case ExtUI::G26_POINT_FINISH:
      mydata.count++;
      break;
  }
  BedMeshViewScreen::onMeshUpdate(x, y, 0);
}

void BedMeshViewScreen::doProbe() {
  GOTO_SCREEN(BedMeshViewScreen);
  mydata.count = 0;
  injectCommands_P(PSTR(BED_LEVELING_COMMANDS));
}

void BedMeshViewScreen::doMeshValidation() {
  mydata.count = 0;
  GOTO_SCREEN(StatusScreen);
  injectCommands_P(PSTR("M75\nG28 O\nM117 Heating...\nG26 R X0 Y0\nG27\nM77"));
}

void BedMeshViewScreen::show() {
  injectCommands_P(PSTR("G29 L1"));
  GOTO_SCREEN(BedMeshViewScreen);
}

#endif // FTDI_BED_MESH_VIEW_SCREEN
