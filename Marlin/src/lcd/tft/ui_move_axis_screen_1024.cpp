/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if HAS_UI_1024x600

#include "ui_common.h"

#if ENABLED(MOVE_AXIS_SCREEN)

#include "../marlinui.h"
#include "../menu/menu.h"
#include "../../libs/numtostr.h"

#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/planner.h"
#include "../../module/motion.h"

#if ENABLED(BABYSTEP_ZPROBE_OFFSET)
  #include "../../feature/babystep.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#define BTN_WIDTH 64
#define BTN_HEIGHT 52
#define X_MARGIN 20
#define Y_MARGIN 15

#define CUR_STEP_VALUE_WIDTH 104
void drawCurStepValue() {
  tft_string.set(ftostr52sp(motionAxisState.currentStepSize));
  tft_string.add(F("mm"));
  tft.canvas(motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(tft_string.center(CUR_STEP_VALUE_WIDTH), 0, COLOR_AXIS_HOMED, tft_string);
}

#if HAS_Z_AXIS
  void drawCurZSelection() {
    tft_string.set('Z');
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, tft_string.width(), 34);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_text(0, 0, Z_BTN_COLOR, tft_string);

    tft.queue.sync();

    tft_string.set(F("Offset"));
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y + 34, tft_string.width(), 34);
    tft.set_background(COLOR_BACKGROUND);
    #if HAS_BED_PROBE
      if (motionAxisState.z_selection == Z_SELECTION_Z_PROBE)
        tft.add_text(0, 0, Z_BTN_COLOR, tft_string);
    #endif
  }
#endif

#if HAS_EXTRUDERS
  void drawCurESelection() {
    tft.canvas(motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, BTN_WIDTH, BTN_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set('E');
    tft.add_text(0, 0, E_BTN_COLOR, tft_string);
    tft.add_text(tft_string.width(), 0, E_BTN_COLOR, ui8tostr3rj(motionAxisState.e_selection));
  }
#endif

void drawMessage_P(PGM_P const msg) {
  tft.canvas(X_MARGIN, TFT_HEIGHT - Y_MARGIN - 34, TFT_HEIGHT / 2, 34);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_STATUS_MESSAGE, msg);
}

void drawAxisValue(const AxisEnum axis) {
  const float value = (
    TERN_(HAS_BED_PROBE, axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE ? probe.offset.z :)
    ui.manual_move.axis_value(axis)
  );
  xy_int_t pos;
  uint16_t color;
  switch (axis) {
    #if HAS_X_AXIS
      case X_AXIS: pos = motionAxisState.xValuePos; color = X_BTN_COLOR; break;
    #endif
    #if HAS_Y_AXIS
      case Y_AXIS: pos = motionAxisState.yValuePos; color = Y_BTN_COLOR; break;
    #endif
    #if HAS_Z_AXIS
      case Z_AXIS: pos = motionAxisState.zValuePos; color = Z_BTN_COLOR; break;
    #endif
    #if HAS_EXTRUDERS
      case E_AXIS: pos = motionAxisState.eValuePos; color = E_BTN_COLOR; break;
    #endif
    default: return;
  }
  tft.canvas(pos.x, pos.y, BTN_WIDTH + X_MARGIN, BTN_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(ftostr52sp(value));
  tft.add_text(0, 0, color, tft_string);
}

void drawBtn(const int x, const int y, const char *label, intptr_t data, const MarlinImage img, uint16_t bgColor, const bool enabled/*=true*/) {
  drawBtn(x, y, label, data, imgBtn52Rounded, img, bgColor, enabled);
}

void MarlinUI::move_axis_screen() {
  // Reset
  defer_status_screen(true);
  motionAxisState.blocked = false;
  TERN_(TOUCH_SCREEN, touch.enable());

  ui.clear_lcd();

  TERN_(TOUCH_SCREEN, touch.clear());

  const bool busy = printingIsActive();

  // Babysteps during printing? Select babystep for Z probe offset
  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    if (busy) motionAxisState.z_selection = Z_SELECTION_Z_PROBE;
  #endif

  /**************************************************************************
   * Row 1: | [E-] | [Y-] | Current Y | [Z+] |
   *************************************************************************/

  int x = X_MARGIN, y = Y_MARGIN, spacing = 0;

  TERN_(HAS_EXTRUDERS, drawBtn(x, y, "E+", e_plus, imgUp, E_BTN_COLOR, !busy));

  spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;
  x += BTN_WIDTH + spacing;

  TERN_(HAS_Y_AXIS, drawBtn(x, y, "Y+", y_plus, imgUp, Y_BTN_COLOR, !busy));

  x += BTN_WIDTH;

  // Current Y
  #if HAS_Y_AXIS
    motionAxisState.yValuePos.set(x + 2, y);
    drawAxisValue(Y_AXIS);
  #endif

  x += spacing;
  #if HAS_Z_AXIS
    drawBtn(x, y, "Z+", z_plus, imgUp, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); // Only enabled when not busy or have baby step
  #endif

  /**************************************************************************
   * Row 2: | "Ex" | [X-] | [Home] | [X+] | "Z" |
   *************************************************************************/

  y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
  x = X_MARGIN;
  spacing = (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4;

  #if HAS_EXTRUDERS
    motionAxisState.eNamePos.set(x, y);
    drawCurESelection();
    TERN_(TOUCH_SCREEN, if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, BTN_HEIGHT, e_select));
  #endif

  x += BTN_WIDTH + spacing;

  TERN_(HAS_X_AXIS, drawBtn(x, y, "X-", x_minus, imgLeft, X_BTN_COLOR, !busy));

  x += BTN_WIDTH + spacing;

  #if ALL(HAS_X_AXIS, TOUCH_SCREEN)
    add_control(TFT_WIDTH / 2 - images[imgHome].width / 2, y - (images[imgHome].width - BTN_HEIGHT) / 2, BUTTON, do_home, imgHome, !busy);
  #endif

  x += BTN_WIDTH + spacing;
  const uint16_t xplus_x = x;

  TERN_(HAS_X_AXIS, drawBtn(x, y, "X+", x_plus, imgRight, X_BTN_COLOR, !busy));

  x += BTN_WIDTH + spacing;

  #if HAS_Z_AXIS
    motionAxisState.zTypePos.set(x, y);
    drawCurZSelection();
    #if ALL(HAS_BED_PROBE, TOUCH_SCREEN)
      if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, 34 * 2, z_select);
    #endif
  #endif

  /**************************************************************************
   * Row 3: | [E-] | Current X | [Y-] | [Z-] |
   *************************************************************************/

  y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
  x = X_MARGIN;
  spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;

  #if HAS_EXTRUDERS
    drawBtn(x, y, "E-", e_minus, imgDown, E_BTN_COLOR, !busy);
    motionAxisState.eValuePos.set(x, y + BTN_HEIGHT + 2);
    drawAxisValue(E_AXIS);
  #endif

  // Current X
  #if HAS_X_AXIS
    motionAxisState.xValuePos.set(BTN_WIDTH + (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4, y - 10);
    drawAxisValue(X_AXIS);
  #endif

  x += BTN_WIDTH + spacing;

  // Current Y
  TERN_(HAS_Y_AXIS, drawBtn(x, y, "Y-", y_minus, imgDown, Y_BTN_COLOR, !busy));

  x += BTN_WIDTH + spacing;

  // Current Z
  #if HAS_Z_AXIS
    drawBtn(x, y, "Z-", z_minus, imgDown, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); // Only enabled when not busy or have babystep
    motionAxisState.zValuePos.set(x, y + BTN_HEIGHT + 2);
    drawAxisValue(Z_AXIS);
  #endif

  /**************************************************************************
   * Row 4: | Step Size | [Disable Steppers] | [Back] |
   *************************************************************************/

  y = TFT_HEIGHT - Y_MARGIN - 32;
  x = TFT_WIDTH / 2 - CUR_STEP_VALUE_WIDTH / 2;
  motionAxisState.stepValuePos.set(x, y);
  if (!busy) {
    drawCurStepValue();
    TERN_(TOUCH_SCREEN, touch.add_control(BUTTON, x, y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT, step_size));
  }

  // Aligned with x+
  drawBtn(xplus_x, TFT_HEIGHT - Y_MARGIN - BTN_HEIGHT, "off", disable_steppers, imgCancel, COLOR_WHITE, !busy);

  TERN_(TOUCH_SCREEN, add_control(TFT_WIDTH - X_MARGIN - BTN_WIDTH, y, BACK, imgBack));
}

#endif // MOVE_AXIS_SCREEN
#endif // HAS_UI_1024x600
