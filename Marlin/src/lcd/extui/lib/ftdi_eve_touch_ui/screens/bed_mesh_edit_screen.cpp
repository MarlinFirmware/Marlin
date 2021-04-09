/****************************
 * bed_mesh_edit_screen.cpp *
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
#include "screens.h"
#include "screen_data.h"

#ifdef FTDI_BED_MESH_EDIT_SCREEN

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

constexpr static BedMeshEditScreenData &mydata = screen_data.BedMeshEditScreen;
constexpr static float gaugeThickness = 0.25;

#if ENABLED(TOUCH_UI_PORTRAIT)
  #define GRID_COLS 3
  #define GRID_ROWS 10

  #define MESH_POS    BTN_POS(1, 2), BTN_SIZE(3,5)
  #define MESSAGE_POS BTN_POS(1, 7), BTN_SIZE(3,1)
  #define Z_LABEL_POS BTN_POS(1, 8), BTN_SIZE(1,1)
  #define Z_VALUE_POS BTN_POS(2, 8), BTN_SIZE(2,1)
  #define BACK_POS    BTN_POS(1,10), BTN_SIZE(2,1)
  #define SAVE_POS    BTN_POS(3,10), BTN_SIZE(1,1)
#else
  #define GRID_COLS 5
  #define GRID_ROWS 5

  #define MESH_POS    BTN_POS(1,1), BTN_SIZE(3,5)
  #define MESSAGE_POS BTN_POS(4,1), BTN_SIZE(2,1)
  #define Z_LABEL_POS BTN_POS(4,2), BTN_SIZE(2,1)
  #define Z_VALUE_POS BTN_POS(4,3), BTN_SIZE(2,1)
  #define BACK_POS    BTN_POS(4,5), BTN_SIZE(1,1)
  #define SAVE_POS    BTN_POS(5,5), BTN_SIZE(1,1)
#endif

static float meshGetter(uint8_t x, uint8_t y, void*) {
  xy_uint8_t pos;
  pos.x = x;
  pos.y = y;
  return ExtUI::getMeshPoint(pos) + (mydata.highlight.x != -1 && mydata.highlight == pos ? mydata.zAdjustment : 0);
}

void BedMeshEditScreen::onEntry() {
  mydata.needSave = false;
  mydata.highlight.x = -1;
  mydata.zAdjustment = 0;
  BaseScreen::onEntry();
}

float BedMeshEditScreen::getHighlightedValue() {
  const float val = ExtUI::getMeshPoint(mydata.highlight);
  return (isnan(val) ? 0 : val) + mydata.zAdjustment;
}

void BedMeshEditScreen::setHighlightedValue(float value) {
  ExtUI::setMeshPoint(mydata.highlight, value);
}

void BedMeshEditScreen::moveToHighlightedValue() {
  if (ExtUI::getMeshValid()) {
    ExtUI::setLevelingActive(true);
    ExtUI::moveToMeshPoint(mydata.highlight, gaugeThickness + mydata.zAdjustment);
  }
}

void BedMeshEditScreen::adjustHighlightedValue(float increment) {
  if(mydata.highlight.x != -1) {
    mydata.zAdjustment += increment;
    moveToHighlightedValue();
    mydata.needSave = true;
  }
}

void BedMeshEditScreen::saveAdjustedHighlightedValue() {
  if (mydata.zAdjustment && mydata.highlight.x != -1) {
    setHighlightedValue(getHighlightedValue());
    mydata.zAdjustment = 0;
  }
}

bool BedMeshEditScreen::changeHighlightedValue(uint8_t tag) {
  saveAdjustedHighlightedValue();
  if (tagToPoint(tag, mydata.highlight)) {
    moveToHighlightedValue();
    return true;
  }
  return false;
}

void BedMeshEditScreen::drawHighlightedPointValue() {
  CommandProcessor cmd;
  cmd.font(Theme::font_medium)
     .colors(normal_btn)
     .text(Z_LABEL_POS, GET_TEXT_F(MSG_MESH_EDIT_Z))
     .font(font_small);
  if(mydata.highlight.x != -1)
    draw_adjuster(cmd, Z_VALUE_POS, 3, getHighlightedValue(), GET_TEXT_F(MSG_UNITS_MM), 4, 3);
  cmd.colors(mydata.needSave ? normal_btn : action_btn)
     .tag(1).button(BACK_POS, GET_TEXT_F(MSG_BUTTON_BACK))
     .colors(mydata.needSave ? action_btn : normal_btn)
     .enabled(mydata.needSave)
     .tag(2).button(SAVE_POS, GET_TEXT_F(MSG_TOUCHMI_SAVE));
}

void BedMeshEditScreen::onRedraw(draw_mode_t what) {
  #define _INSET_POS(x,y,w,h) x + min(w,h)/10, y + min(w,h)/10, w - min(w,h)/5, h - min(w,h)/5
  #define INSET_POS(pos) _INSET_POS(pos)

  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true));
    drawMeshBackground(cmd, INSET_POS(MESH_POS));
  }

  if (what & FOREGROUND) {
    drawHighlightedPointValue();
    drawMeshForeground(cmd, INSET_POS(MESH_POS), meshGetter, nullptr, pointToTag(mydata.highlight.x,mydata.highlight.y));
  }
}

bool BedMeshEditScreen::onTouchHeld(uint8_t tag) {
  constexpr float increment = 0.01;
  switch (tag) {
    case 3: adjustHighlightedValue(-increment); return true;
    case 4: adjustHighlightedValue( increment); return true;
  }
  return false;
}

bool BedMeshEditScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      // On Cancel, reload saved mesh, discarding changes
      GOTO_PREVIOUS();
      injectCommands_P(PSTR("G29 L1"));
      return true;
    case 2:
      saveAdjustedHighlightedValue();
      injectCommands_P(PSTR("G29 S1"));
      mydata.needSave = false;
      return true;
    case 3:
    case 4:
      return onTouchHeld(tag);
    default: return changeHighlightedValue(tag);
  }
  return true;
}

void BedMeshEditScreen::show() {
  // On entry, home if needed and save current mesh
  if (!ExtUI::isMachineHomed()) {
    SpinnerDialogBox::enqueueAndWait_P(F("G28\nG29 S1"));
    // After the spinner, go to this screen.
    current_screen.forget();
    PUSH_SCREEN(BedMeshEditScreen);
  } else {
    injectCommands_P(PSTR("G29 S1"));
    GOTO_SCREEN(BedMeshEditScreen);
  }
}

#endif // FTDI_BED_MESH_EDIT_SCREEN
