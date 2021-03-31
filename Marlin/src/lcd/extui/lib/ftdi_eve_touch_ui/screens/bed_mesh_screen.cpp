/***********************
 * bed_mesh_screen.cpp *
 ***********************/

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

#ifdef FTDI_BED_MESH_SCREEN

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

constexpr static BedMeshScreenData &mydata = screen_data.BedMeshScreen;
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

void BedMeshScreen::drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, ExtUI::bed_mesh_t data, uint8_t opts, float autoscale_max) {
  constexpr uint8_t rows       = GRID_MAX_POINTS_Y;
  constexpr uint8_t cols       = GRID_MAX_POINTS_X;

  #define VALUE(X,Y)         (data ? data[X][Y] : 0)
  #define ISVAL(X,Y)         (data ? !isnan(VALUE(X,Y)) : true)
  #define HEIGHT(X,Y)        (ISVAL(X,Y) ? (VALUE(X,Y) - val_min) * scale_z : 0)

  // Compute the mean, min and max for the points

  float   val_mean = 0;
  float   val_max  = -INFINITY;
  float   val_min  =  INFINITY;
  uint8_t val_cnt  = 0;

  if (data && (opts & USE_AUTOSCALE)) {
    for (uint8_t y = 0; y < rows; y++) {
      for (uint8_t x = 0; x < cols; x++) {
        if (ISVAL(x,y)) {
          const float val = VALUE(x,y);
          val_mean += val;
          val_max   = max(val_max, val);
          val_min   = min(val_min, val);
          val_cnt++;
        }
      }
    }
  }
  if (val_cnt)
    val_mean /= val_cnt;
  else {
    val_mean = 0;
    val_min  = 0;
    val_max  = 0;
  }

  const float scale_z = ((val_max == val_min) ? 1 : 1/(val_max - val_min)) * autoscale_max;

  /**
   * The 3D points go through a 3D graphics pipeline to determine the final 2D point on the screen.
   * This is written out as a stack of macros that each apply an affine transformation to the point.
   * At compile time, the compiler should be able to reduce these expressions.
   *
   * The last transformation in the chain (TRANSFORM_5) is initially set to a no-op so we can measure
   * the dimensions of the grid, but is later replaced with a scaling transform that scales the grid
   * to fit.
   */

  #define TRANSFORM_5(X,Y,Z)  (X), (Y)                                                                   // No transform
  #define TRANSFORM_4(X,Y,Z)  TRANSFORM_5((X)/(Z),(Y)/-(Z), 0)                                           // Perspective
  #define TRANSFORM_3(X,Y,Z)  TRANSFORM_4((X), (Z), (Y))                                                 // Swap Z and Y
  #define TRANSFORM_2(X,Y,Z)  TRANSFORM_3((X), (Y) + 2.5, (Z) - 1)                                       // Translate
  #define TRANSFORM(X,Y,Z)    TRANSFORM_2(float(X)/(cols-1) - 0.5, float(Y)/(rows-1)  - 0.5, (Z))        // Normalize

  // Compute the bounding box for the grid prior to scaling. Do this at compile-time by
  // transforming the four corner points via the transformation equations and finding
  // the min and max for each axis.

  constexpr float bounds[][3]  = {{TRANSFORM(0     , 0     , 0)},
                                  {TRANSFORM(cols-1, 0     , 0)},
                                  {TRANSFORM(0     , rows-1, 0)},
                                  {TRANSFORM(cols-1, rows-1, 0)}};
  #define APPLY(FUNC, AXIS) FUNC(FUNC(bounds[0][AXIS], bounds[1][AXIS]), FUNC(bounds[2][AXIS], bounds[3][AXIS]))
  constexpr float grid_x       = APPLY(min,0);
  constexpr float grid_y       = APPLY(min,1);
  constexpr float grid_w       = APPLY(max,0) - grid_x;
  constexpr float grid_h       = APPLY(max,1) - grid_y;
  constexpr float grid_cx      = grid_x + grid_w/2;
  constexpr float grid_cy      = grid_y + grid_h/2;

  // Figure out scale and offset such that the grid fits within the rectangle given by (x,y,w,h)

  const float scale_x          = float(w)/grid_w;
  const float scale_y          = float(h)/grid_h;
  const float center_x         = x + w/2;
  const float center_y         = y + h/2;

  // Now replace the last transformation in the chain with a scaling operation.

  #undef  TRANSFORM_5
  #define TRANSFORM_6(X,Y,Z)  (X)*16, (Y)*16                                                  // Scale to 1/16 pixel units
  #define TRANSFORM_5(X,Y,Z)  TRANSFORM_6( center_x + ((X) - grid_cx) * scale_x, \
                                           center_y + ((Y) - grid_cy) * scale_y, 0)           // Scale to bounds

  // Draw the grid

  const uint16_t basePointSize = min(w,h) / max(cols,rows);

  CommandProcessor cmd;
  cmd.cmd(SAVE_CONTEXT())
     .cmd(TAG_MASK(false))
     .cmd(SAVE_CONTEXT());

  for (uint8_t y = 0; y < rows; y++) {
    for (uint8_t x = 0; x < cols; x++) {
      if (ISVAL(x,y)) {
       const bool hasLeftSegment  = x < cols - 1 && ISVAL(x+1,y);
       const bool hasRightSegment = y < rows - 1 && ISVAL(x,y+1);
       if (hasLeftSegment || hasRightSegment) {
         cmd.cmd(BEGIN(LINE_STRIP));
         if (hasLeftSegment)  cmd.cmd(VERTEX2F(TRANSFORM(x + 1, y    , HEIGHT(x + 1, y    ))));
         cmd.cmd(                     VERTEX2F(TRANSFORM(x    , y    , HEIGHT(x    , y    ))));
         if (hasRightSegment) cmd.cmd(VERTEX2F(TRANSFORM(x    , y + 1, HEIGHT(x    , y + 1))));
       }
      }
    }

    if (opts & USE_POINTS) {
      const float sq_min = sq(val_min - val_mean);
      const float sq_max = sq(val_max - val_mean);
      cmd.cmd(POINT_SIZE(basePointSize * 2));
      cmd.cmd(BEGIN(POINTS));
      for (uint8_t x = 0; x < cols; x++) {
        if (ISVAL(x,y)) {
          if (opts & USE_COLORS) {
            const float   val_dev  = VALUE(x, y) - val_mean;
            const uint8_t neg_byte = sq(val_dev) / (val_dev < 0 ? sq_min : sq_max) * 0xFF;
            const uint8_t pos_byte = 255 - neg_byte;
            cmd.cmd(COLOR_RGB(pos_byte, pos_byte, 0xFF));
          }
          cmd.cmd(VERTEX2F(TRANSFORM(x, y, HEIGHT(x, y))));
        }
      }
      if (opts & USE_COLORS) {
        cmd.cmd(RESTORE_CONTEXT())
           .cmd(SAVE_CONTEXT());
      }
    }
  }
  cmd.cmd(RESTORE_CONTEXT())
     .cmd(TAG_MASK(true));

  if (opts & USE_TAGS) {
    cmd.cmd(COLOR_MASK(false, false, false, false))
       .cmd(POINT_SIZE(basePointSize * 10))
       .cmd(BEGIN(POINTS));
    for (uint8_t y = 0; y < rows; y++) {
      for (uint8_t x = 0; x < cols; x++) {
        const uint8_t tag = pointToTag(x, y);
        cmd.tag(tag).cmd(VERTEX2F(TRANSFORM(x, y, HEIGHT(x, y))));
      }
    }
    cmd.cmd(COLOR_MASK(true, true, true, true));
  }

  if (opts & USE_HIGHLIGHT) {
    const uint8_t tag = mydata.highlightedTag;
    xy_uint8_t pt;
    if (tagToPoint(tag, pt)) {
      cmd.cmd(COLOR_A(128))
         .cmd(POINT_SIZE(basePointSize * 6))
         .cmd(BEGIN(POINTS))
         .tag(tag).cmd(VERTEX2F(TRANSFORM(pt.x, pt.y, HEIGHT(pt.x, pt.y))));
    }
  }
  cmd.cmd(END());
  cmd.cmd(RESTORE_CONTEXT());
}

uint8_t BedMeshScreen::pointToTag(uint8_t x, uint8_t y) {
  return y * (GRID_MAX_POINTS_X) + x + 10;
}

bool BedMeshScreen::tagToPoint(uint8_t tag, xy_uint8_t &pt) {
  if (tag < 10) return false;
  pt.x = (tag - 10) % (GRID_MAX_POINTS_X);
  pt.y = (tag - 10) / (GRID_MAX_POINTS_X);
  return true;
}

void BedMeshScreen::onEntry() {
  mydata.allowEditing = true;
  mydata.highlightedTag = 0;
  mydata.zAdjustment = 0;
  mydata.count = GRID_MAX_POINTS;
  mydata.message = mydata.MSG_NONE;
  BaseScreen::onEntry();
}

float BedMeshScreen::getHighlightedValue(bool nanAsZero) {
  xy_uint8_t pt;
  if (tagToPoint(mydata.highlightedTag, pt)) {
    const float val = ExtUI::getMeshPoint(pt);
    return (isnan(val) && nanAsZero) ? 0 : val;
  }
  return NAN;
}

void BedMeshScreen::setHighlightedValue(float value) {
  xy_uint8_t pt;
  if (tagToPoint(mydata.highlightedTag, pt))
    ExtUI::setMeshPoint(pt, value);
}

void BedMeshScreen::moveToHighlightedValue() {
  xy_uint8_t pt;
  if (tagToPoint(mydata.highlightedTag, pt))
    ExtUI::moveToMeshPoint(pt, gaugeThickness + mydata.zAdjustment);
}

void BedMeshScreen::adjustHighlightedValue(float increment) {
  mydata.zAdjustment += increment;
  moveToHighlightedValue();
}

void BedMeshScreen::saveAdjustedHighlightedValue() {
  if (mydata.zAdjustment) {
    BedMeshScreen::setHighlightedValue(BedMeshScreen::getHighlightedValue(true) + mydata.zAdjustment);
    mydata.zAdjustment = 0;
  }
}

void BedMeshScreen::changeHighlightedValue(uint8_t tag) {
  if (mydata.allowEditing) saveAdjustedHighlightedValue();
  mydata.highlightedTag = tag;
  if (mydata.allowEditing) moveToHighlightedValue();
}

void BedMeshScreen::drawHighlightedPointValue() {
  CommandProcessor cmd;
  cmd.font(Theme::font_medium)
     .colors(normal_btn)
     .text(Z_LABEL_POS, GET_TEXT_F(MSG_MESH_EDIT_Z))
     .font(font_small);

  if (mydata.allowEditing)
    draw_adjuster(cmd, Z_VALUE_POS, 2, getHighlightedValue(true) + mydata.zAdjustment, GET_TEXT_F(MSG_UNITS_MM), 4, 3);
  else
    draw_adjuster_value(cmd, Z_VALUE_POS, getHighlightedValue(true) + mydata.zAdjustment, GET_TEXT_F(MSG_UNITS_MM), 4, 3);

  cmd.colors(action_btn)
     .tag(1).button(OKAY_POS, GET_TEXT_F(MSG_BUTTON_OKAY))
     .tag(0);

  switch (mydata.message) {
    case mydata.MSG_MESH_COMPLETE:   cmd.text(MESSAGE_POS, GET_TEXT_F(MSG_BED_MAPPING_DONE)); break;
    case mydata.MSG_MESH_INCOMPLETE: cmd.text(MESSAGE_POS, GET_TEXT_F(MSG_BED_MAPPING_INCOMPLETE)); break;
    default: break;
  }
}

void BedMeshScreen::onRedraw(draw_mode_t what) {
  #define _INSET_POS(x,y,w,h) x + min(w,h)/10, y + min(w,h)/10, w - min(w,h)/5, h - min(w,h)/5
  #define INSET_POS(pos) _INSET_POS(pos)

  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true));

    // Draw the shadow and tags
    cmd.cmd(COLOR_RGB(Theme::bed_mesh_shadow_rgb));
    BedMeshScreen::drawMesh(INSET_POS(MESH_POS), nullptr, USE_POINTS | USE_TAGS);
    cmd.cmd(COLOR_RGB(bg_text_enabled));
  }

  if (what & FOREGROUND) {
    constexpr float autoscale_max_amplitude = 0.03;
    const bool gotAllPoints = mydata.count >= GRID_MAX_POINTS;
    if (gotAllPoints) {
      drawHighlightedPointValue();
    }
    CommandProcessor cmd;
    cmd.cmd(COLOR_RGB(Theme::bed_mesh_lines_rgb));
    const float levelingProgress = sq(float(mydata.count) / GRID_MAX_POINTS);
    BedMeshScreen::drawMesh(INSET_POS(MESH_POS), ExtUI::getMeshArray(),
      USE_POINTS | USE_HIGHLIGHT | USE_AUTOSCALE | (gotAllPoints ? USE_COLORS : 0),
      autoscale_max_amplitude * levelingProgress
    );
  }
}

bool BedMeshScreen::onTouchEnd(uint8_t tag) {
  constexpr float increment = 0.01;
  switch (tag) {
    case 1:
      saveAdjustedHighlightedValue();
      injectCommands_P(PSTR("G29 S1"));
      GOTO_PREVIOUS();
      return true;
    case 2: adjustHighlightedValue(-increment); break;
    case 3: adjustHighlightedValue( increment); break;
    default:
        if (tag >= 10)
            changeHighlightedValue(tag);
        else
            return false;
  }
  return true;
}

void BedMeshScreen::onMeshUpdate(const int8_t, const int8_t, const float) {
  if (AT_SCREEN(BedMeshScreen))
    onRefresh();
}

void BedMeshScreen::onMeshUpdate(const int8_t x, const int8_t y, const ExtUI::probe_state_t state) {
  switch (state) {
    case ExtUI::MESH_START:
      mydata.allowEditing = false;
      mydata.count = 0;
      mydata.message = mydata.MSG_NONE;
      break;
    case ExtUI::MESH_FINISH:
      if (mydata.count == GRID_MAX_POINTS && ExtUI::getMeshValid())
        mydata.message = mydata.MSG_MESH_COMPLETE;
      else
        mydata.message = mydata.MSG_MESH_INCOMPLETE;
      mydata.count = GRID_MAX_POINTS;
      break;
    case ExtUI::PROBE_START:
      mydata.highlightedTag = pointToTag(x, y);
      break;
    case ExtUI::PROBE_FINISH:
      mydata.count++;
      break;
  }
  BedMeshScreen::onMeshUpdate(x, y, 0);
}

void BedMeshScreen::startMeshProbe() {
  GOTO_SCREEN(BedMeshScreen);
  mydata.allowEditing = false;
  mydata.count = 0;
  injectCommands_P(PSTR(BED_LEVELING_COMMANDS));
}

void BedMeshScreen::showMesh() {
  GOTO_SCREEN(BedMeshScreen);
  mydata.allowEditing = false;
}

void BedMeshScreen::showMeshEditor() {
  SpinnerDialogBox::enqueueAndWait_P(ExtUI::isMachineHomed() ? F("M420 S1") : F("G28\nM420 S1"));
  // After the spinner, go to this screen.
  current_screen.forget();
  PUSH_SCREEN(BedMeshScreen);
}

#endif // FTDI_BED_MESH_SCREEN
