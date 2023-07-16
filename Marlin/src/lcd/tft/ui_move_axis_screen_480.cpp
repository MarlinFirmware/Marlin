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

#if HAS_UI_480x320 || HAS_UI_480x272

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

#if ENABLED(TFT_COLOR_UI_PORTRAIT)
  #define MOVE_AXIS_TOTAL_ROWS_HEIGHT (7 * BTN_HEIGHT + FONT_LINE_HEIGHT + 12)
  #define MOVE_AXIS_MARGIN_REGIONS 6
  #define MOVE_AXIS_MARGIN_SIZE ((TFT_HEIGHT - MOVE_AXIS_TOTAL_ROWS_HEIGHT) / MOVE_AXIS_MARGIN_REGIONS)
#endif

#define Z_SELECTION_Z 1
#define Z_SELECTION_Z_PROBE -1

struct {
  #if HAS_X_AXIS
    xy_int_t xValuePos;
  #endif
  #if HAS_Y_AXIS
    xy_int_t yValuePos;
  #endif
  #if HAS_Z_AXIS
    xy_int_t zValuePos, zTypePos;
    int z_selection = Z_SELECTION_Z;
  #endif
  #if HAS_EXTRUDERS
    xy_int_t eValuePos, eNamePos;
    uint8_t e_selection = 0;
  #endif
  xy_int_t stepValuePos;
  float currentStepSize = 10.0;
  bool blocked = false;
  char message[32];
} motionAxisState;

#define BTN_WIDTH 64
#define BTN_HEIGHT 52
#define X_MARGIN 20
#define Y_MARGIN 15
#define Y_OFFSET TERN(HAS_UI_480x272, 28, 34)

static void quick_feedback() {
  #if HAS_CHIRP
    ui.chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
    #if ALL(HAS_MARLINUI_MENU, HAS_BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #elif HAS_MARLINUI_MENU
      delay(10);
    #endif
  #endif
}

#define CUR_STEP_VALUE_WIDTH 104
static void drawCurStepValue() {
  tft_string.set(ftostr52sp(motionAxisState.currentStepSize));
  tft_string.trim();
  tft_string.add(F("mm"));
  tft.canvas(motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_AXIS_HOMED, tft_string);
}

#if HAS_Z_AXIS
  static void drawCurZSelection() {
    tft_string.set('Z');
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, tft_string.width(), FONT_LINE_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_text(0, 0, Z_BTN_COLOR, tft_string);

    tft.queue.sync();

    tft_string.set(F("Offset"));
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y + Y_OFFSET, tft_string.width(), Y_OFFSET TERN_(HAS_UI_480x272, - 10));
    tft.set_background(COLOR_BACKGROUND);
    #if HAS_BED_PROBE
      if (motionAxisState.z_selection == Z_SELECTION_Z_PROBE)
        tft.add_text(0, 0, Z_BTN_COLOR, tft_string);
    #endif
  }
#endif

#if HAS_EXTRUDERS
  static void drawCurESelection() {
    tft.canvas(motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, BTN_WIDTH, BTN_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set('E');
    tft_string.add(utostr3(motionAxisState.e_selection));
    tft.add_text(tft_string.center(BTN_WIDTH), 0, E_BTN_COLOR, tft_string);
  }
#endif

static void drawMessage(PGM_P const msg) {
  #if ENABLED(TFT_COLOR_UI_PORTRAIT)
    tft.canvas(X_MARGIN, TFT_HEIGHT - 2 * MOVE_AXIS_MARGIN_SIZE - BTN_HEIGHT - FONT_LINE_HEIGHT, TFT_WIDTH - X_MARGIN * 2, FONT_LINE_HEIGHT);
  #else
    tft.canvas(X_MARGIN, TFT_HEIGHT - Y_MARGIN - Y_OFFSET, TFT_HEIGHT / 2, Y_OFFSET);
  #endif
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_YELLOW, msg);
}

static void drawMessage(FSTR_P const fmsg) { drawMessage(FTOP(fmsg)); }

static void drawAxisValue(const AxisEnum axis) {
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
  tft.canvas(pos.x, pos.y, BTN_WIDTH + X_MARGIN, BTN_HEIGHT TERN_(HAS_UI_480x272, / 2));
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(ftostr52sp(value));
  tft_string.trim();
  tft.add_text(tft_string.center(BTN_WIDTH + X_MARGIN), 0, color, tft_string);
}

static void moveAxis(const AxisEnum axis, const int8_t direction) {
  quick_feedback();

  #if HAS_EXTRUDERS
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(motionAxisState.e_selection)) {
      drawMessage(F("Too cold"));
      return;
    }
  #endif

  const float diff = motionAxisState.currentStepSize * direction;

  #if HAS_BED_PROBE

    if (axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE) {
      #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
        const int16_t babystep_increment = direction * BABYSTEP_SIZE_Z;
        const bool do_probe = DISABLED(BABYSTEP_HOTEND_Z_OFFSET) || active_extruder == 0;
        const float bsDiff = planner.mm_per_step[Z_AXIS] * babystep_increment,
                    new_probe_offset = probe.offset.z + bsDiff,
                    new_offs = TERN(BABYSTEP_HOTEND_Z_OFFSET
                      , do_probe ? new_probe_offset : hotend_offset[active_extruder].z - bsDiff
                      , new_probe_offset
                    );
        if (WITHIN(new_offs, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
          babystep.add_steps(Z_AXIS, babystep_increment);
          if (do_probe)
            probe.offset.z = new_offs;
          else
            TERN(BABYSTEP_HOTEND_Z_OFFSET, hotend_offset[active_extruder].z = new_offs, NOOP);
          drawMessage(NUL_STR); // Clear the error
          drawAxisValue(axis);
        }
        else
          drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));

      #else
        // Only change probe.offset.z
        probe.offset.z += diff;
        if (direction < 0 && current_position.z < Z_PROBE_OFFSET_RANGE_MIN) {
          current_position.z = Z_PROBE_OFFSET_RANGE_MIN;
          drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));
        }
        else if (direction > 0 && current_position.z > Z_PROBE_OFFSET_RANGE_MAX) {
          current_position.z = Z_PROBE_OFFSET_RANGE_MAX;
          drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));
        }
        else
          drawMessage(NUL_STR); // Clear the error

        drawAxisValue(axis);
      #endif
    }

  #endif // HAS_BED_PROBE

  if (!ui.manual_move.processing) {
    // Get motion limit from software endstops, if any
    float min, max;
    soft_endstop.get_manual_axis_limits(axis, min, max);

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS && TERN1(HAS_EXTRUDERS, axis != E_AXIS)) {
        max = SQRT(sq(float(PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const bool limited = ui.manual_move.apply_diff(axis, diff, min, max);
    #if IS_KINEMATIC
      UNUSED(limited);
    #else
      PGM_P const msg = limited ? GET_TEXT(MSG_LCD_SOFT_ENDSTOPS) : NUL_STR;
      drawMessage(msg);
    #endif

    ui.manual_move.soon(axis OPTARG(MULTI_E_MANUAL, motionAxisState.e_selection));
  }

  drawAxisValue(axis);
}

#if HAS_EXTRUDERS
  static void e_plus()  { moveAxis(E_AXIS, 1);  }
  static void e_minus() { moveAxis(E_AXIS, -1); }
#endif
#if HAS_X_AXIS
  static void x_minus() { moveAxis(X_AXIS, -1); }
  static void x_plus()  { moveAxis(X_AXIS, 1);  }
#endif
#if HAS_Y_AXIS
  static void y_plus()  { moveAxis(Y_AXIS, 1);  }
  static void y_minus() { moveAxis(Y_AXIS, -1); }
#endif
#if HAS_Z_AXIS
  static void z_plus()  { moveAxis(Z_AXIS, 1);  }
  static void z_minus() { moveAxis(Z_AXIS, -1); }
#endif

#if ENABLED(TOUCH_SCREEN)
  #if HAS_EXTRUDERS
    static void e_select() {
      if (++motionAxisState.e_selection >= EXTRUDERS)
        motionAxisState.e_selection = 0;
      quick_feedback();
      drawCurESelection();
      drawAxisValue(E_AXIS);
    }
  #endif

  static void do_home() {
    quick_feedback();
    drawMessage(GET_TEXT_F(MSG_LEVEL_BED_HOMING));
    queue.inject_P(G28_STR);
    // Disable touch until home is done
    touch.disable();
    TERN_(HAS_EXTRUDERS, drawAxisValue(E_AXIS));
    TERN_(HAS_X_AXIS, drawAxisValue(X_AXIS));
    TERN_(HAS_Y_AXIS, drawAxisValue(Y_AXIS));
    TERN_(HAS_Z_AXIS, drawAxisValue(Z_AXIS));
  }

  static void step_size() {
    motionAxisState.currentStepSize = motionAxisState.currentStepSize / 10.0;
    if (motionAxisState.currentStepSize < 0.0015) motionAxisState.currentStepSize = 10.0;
    quick_feedback();
    drawCurStepValue();
  }
#endif // TOUCH_SCREEN

#if ALL(HAS_BED_PROBE, TOUCH_SCREEN)
  static void z_select() {
    motionAxisState.z_selection *= -1;
    quick_feedback();
    drawCurZSelection();
    drawAxisValue(Z_AXIS);
  }
#endif

static void disable_steppers() {
  quick_feedback();
  queue.inject(F("M84"));
}

static void drawBtn(const int x, const int y, const char *label, intptr_t data, const MarlinImage img, uint16_t bgColor, const bool enabled=true) {
  const uint16_t width = images[imgBtn52Rounded].width,
                height = images[imgBtn52Rounded].height;

  if (!enabled) bgColor = COLOR_CONTROL_DISABLED;

  tft.canvas(x, y, width, height);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_image(0, 0, imgBtn52Rounded, bgColor, COLOR_BACKGROUND, COLOR_DARKGREY);

  // TODO: Make an add_text() taking a font arg
  if (label) {
    tft_string.set(label);
    tft_string.trim();
    tft.add_text(tft_string.center(width), height / 2 - tft_string.font_height() / 2, bgColor, tft_string);
  }
  else
    tft.add_image(0, 0, img, bgColor, COLOR_BACKGROUND, COLOR_DARKGREY);

  TERN_(TOUCH_SCREEN, if (enabled) touch.add_control(BUTTON, x, y, width, height, data));
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

  #if ENABLED(TFT_COLOR_UI_PORTRAIT)

    int y = MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 1st: | Button "E+" | Button "Y+" | Button "Z+" |
     *************************************************************************/

    TERN_(HAS_EXTRUDERS, drawBtn(TFT_WIDTH / 6     - BTN_WIDTH / 2, y, "E+", (intptr_t)e_plus, imgUp, E_BTN_COLOR, !busy));
    TERN_(HAS_Y_AXIS,    drawBtn(TFT_WIDTH / 2     - BTN_WIDTH / 2, y, "Y+", (intptr_t)y_plus, imgUp, Y_BTN_COLOR, !busy));
    TERN_(HAS_Z_AXIS,    drawBtn(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y, "Z+", (intptr_t)z_plus, imgUp, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)));

    y += BTN_HEIGHT + 4;

    /**************************************************************************
     * 2nd: | Label "Ex" (E0, E1, etc.) | Y value | Label "Z" |
     *************************************************************************/

    // Label "Ex" (E0, E1, etc.)
    #if HAS_EXTRUDERS
      motionAxisState.eNamePos.set(TFT_WIDTH / 6 - BTN_WIDTH / 2, y);
      drawCurESelection();
      #if ENABLED(TOUCH_SCREEN)
        if (!busy) touch.add_control(BUTTON, motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, BTN_WIDTH, BTN_HEIGHT, (intptr_t)e_select);
      #endif
    #endif

    // Y value
    #if HAS_Y_AXIS
      motionAxisState.yValuePos.set(TFT_WIDTH / 2 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(Y_AXIS);
    #endif

    // Label "Z"
    #ifdef HAS_Z_AXIS
      motionAxisState.zTypePos.set(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y);
      drawCurZSelection();
      #if ALL(HAS_BED_PROBE, TOUCH_SCREEN)
        if (!busy) touch.add_control(BUTTON, motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, BTN_WIDTH, FONT_LINE_HEIGHT * 2, (intptr_t)z_select);
      #endif
    #endif

    y += BTN_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 3rd: | Button "X-" | Button "Homing" | Button "X+" |
     *************************************************************************/

    #if HAS_X_AXIS
      drawBtn(TFT_WIDTH / 6 - BTN_WIDTH / 2, y, "X-", (intptr_t)x_minus, imgLeft, X_BTN_COLOR, !busy);
      TERN_(TOUCH_SCREEN, add_control(TFT_WIDTH / 2 - images[imgHome].width / 2, y - (images[imgHome].width - BTN_HEIGHT) / 2, BUTTON, (intptr_t)do_home, imgHome, !busy));
      drawBtn(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y, "X+", (intptr_t)x_plus, imgRight, X_BTN_COLOR, !busy);
    #endif

    y += BTN_HEIGHT + 4;

    /**************************************************************************
     * 4th: | X value |
     *************************************************************************/

    #ifdef HAS_X_AXIS
      motionAxisState.xValuePos.set(TFT_WIDTH / 6 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(X_AXIS);
    #endif

    y += BTN_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 5th: | Button "E-" | Button "Y-" | Button "Z-"
     *************************************************************************/

    TERN_(HAS_EXTRUDERS, drawBtn(TFT_WIDTH / 6 - BTN_WIDTH / 2, y, "E-", (intptr_t)e_minus, imgDown, E_BTN_COLOR, !busy));
    TERN_(HAS_Y_AXIS, drawBtn(TFT_WIDTH / 2 - BTN_WIDTH / 2, y, "Y-", (intptr_t)y_minus, imgDown, Y_BTN_COLOR, !busy));
    TERN_(HAS_Z_AXIS, drawBtn(TFT_WIDTH * 5 / 6 - BTN_WIDTH / 2, y, "Z-", (intptr_t)z_minus, imgDown, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)));

    y += BTN_HEIGHT + 4;

    /**************************************************************************
     * 6th: | Current Ex value | Z value |
     *************************************************************************/

    // Current Ex value
    #ifdef HAS_EXTRUDERS
      motionAxisState.eValuePos.set(TFT_WIDTH / 6 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(E_AXIS);
    #endif

    #ifdef HAS_Z_AXIS
      motionAxisState.zValuePos.set(TFT_WIDTH * 5 / 6 - (BTN_WIDTH + X_MARGIN) / 2, y);
      drawAxisValue(Z_AXIS);
    #endif

    y += BTN_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 7th: | Message - drawMessage(...) function |
     *************************************************************************/

    y += FONT_LINE_HEIGHT + MOVE_AXIS_MARGIN_SIZE;

    /**************************************************************************
     * 8th: | Step size | Button "Disable steppers" | Button "Back" |
     *************************************************************************/

    // Step size
    motionAxisState.stepValuePos.set(X_MARGIN, y + 12);
    if (!busy) {
      drawCurStepValue();
      TERN_(TOUCH_SCREEN, touch.add_control(BUTTON, motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y + 12, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT, (intptr_t)step_size));
    }

    // Button "Disable steppers"
    drawBtn(TFT_WIDTH / 2, y, "off", (intptr_t)disable_steppers, imgCancel, COLOR_WHITE, !busy);

    // Button "Back"
    TERN_(TOUCH_SCREEN, add_control(TFT_WIDTH * 5 / 6 - images[imgBack].width / 2, y + 12, BACK, imgBack));

  #else // !TFT_COLOR_UI_PORTRAIT

    // ROW 1 -> E- Y- CurY Z+
    int x = X_MARGIN, y = Y_MARGIN, spacing = 0;

    TERN_(HAS_EXTRUDERS, drawBtn(x, y, "E+", (intptr_t)e_plus, imgUp, E_BTN_COLOR, !busy));

    spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;
    x += BTN_WIDTH + spacing;

    TERN_(HAS_Y_AXIS, drawBtn(x, y, "Y+", (intptr_t)y_plus, imgUp, Y_BTN_COLOR, !busy));

    x += BTN_WIDTH;

    // Cur Y
    #if HAS_Y_AXIS
      motionAxisState.yValuePos.set(x + 2, y);
      drawAxisValue(Y_AXIS);
    #endif

    x += spacing;
    #if HAS_Z_AXIS
      drawBtn(x, y, "Z+", (intptr_t)z_plus, imgUp, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); // Only enabled when not busy or have baby step
    #endif

    // ROW 2 -> "Ex"  X-  HOME X+  "Z"
    y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
    x = X_MARGIN;
    spacing = (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4;

    #if HAS_EXTRUDERS
      motionAxisState.eNamePos.set(x, y);
      drawCurESelection();
      TERN_(TOUCH_SCREEN, if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, BTN_HEIGHT, (intptr_t)e_select));
    #endif

    x += BTN_WIDTH + spacing;

    TERN_(HAS_X_AXIS, drawBtn(x, y, "X-", (intptr_t)x_minus, imgLeft, X_BTN_COLOR, !busy));

    x += BTN_WIDTH + spacing; //imgHome is 64x64

    #if ALL(HAS_X_AXIS, TOUCH_SCREEN)
      add_control(TFT_WIDTH / 2 - images[imgHome].width / 2, y - (images[imgHome].width - BTN_HEIGHT) / 2, BUTTON, (intptr_t)do_home, imgHome, !busy);
    #endif

    x += BTN_WIDTH + spacing;
    const uint16_t xplus_x = x;

    TERN_(HAS_X_AXIS, drawBtn(x, y, "X+", (intptr_t)x_plus, imgRight, X_BTN_COLOR, !busy));

    x += BTN_WIDTH + spacing;

    #if HAS_Z_AXIS
      motionAxisState.zTypePos.set(x, y);
      drawCurZSelection();
      #if ALL(HAS_BED_PROBE, TOUCH_SCREEN)
        if (!busy) touch.add_control(BUTTON, x, y, BTN_WIDTH, Y_OFFSET * 2, (intptr_t)z_select);
      #endif
    #endif

    // ROW 3 -> E- CurX Y-  Z-
    y += BTN_HEIGHT + (TFT_HEIGHT - Y_MARGIN * 2 - 4 * BTN_HEIGHT) / 3;
    x = X_MARGIN;
    spacing = (TFT_WIDTH - X_MARGIN * 2 - 3 * BTN_WIDTH) / 2;

    #if HAS_EXTRUDERS
      drawBtn(x, y, "E-", (intptr_t)e_minus, imgDown, E_BTN_COLOR, !busy);
      motionAxisState.eValuePos.set(x, y + BTN_HEIGHT + 2);
      drawAxisValue(E_AXIS);
    #endif

    // Cur X
    #if HAS_X_AXIS
      motionAxisState.xValuePos.set(BTN_WIDTH + (TFT_WIDTH - X_MARGIN * 2 - 5 * BTN_WIDTH) / 4, y - 10);
      TERN_(HAS_X_AXIS, drawAxisValue(X_AXIS));
    #endif

    x += BTN_WIDTH + spacing;

    // Cur Y
    TERN_(HAS_Y_AXIS, drawBtn(x, y, "Y-", (intptr_t)y_minus, imgDown, Y_BTN_COLOR, !busy));

    x += BTN_WIDTH + spacing;

    // Cur Z
    #if HAS_Z_AXIS
      drawBtn(x, y, "Z-", (intptr_t)z_minus, imgDown, Z_BTN_COLOR, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); //only enabled when not busy or have baby step
      motionAxisState.zValuePos.set(x, y + BTN_HEIGHT + 2);
      drawAxisValue(Z_AXIS);
    #endif

    // ROW 4 -> step_size  disable steppers back
    y = TFT_HEIGHT - Y_MARGIN - TERN(HAS_UI_480x272, BTN_WIDTH / 2, 32);
    x = TFT_WIDTH / 2 - CUR_STEP_VALUE_WIDTH / 2;
    motionAxisState.stepValuePos.set(x, y);
    if (!busy) {
      drawCurStepValue();
      TERN_(TOUCH_SCREEN, touch.add_control(BUTTON, motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, BTN_HEIGHT, (intptr_t)step_size));
    }

    // Aligned with x+
    drawBtn(xplus_x, TFT_HEIGHT - Y_MARGIN - BTN_HEIGHT, "off", (intptr_t)disable_steppers, imgCancel, COLOR_WHITE, !busy);

    TERN_(TOUCH_SCREEN, add_control(TFT_WIDTH - X_MARGIN - BTN_WIDTH, y, BACK, imgBack));

  #endif // !TFT_COLOR_UI_PORTRAIT
}
#endif // MOVE_AXIS_SCREEN
#endif // HAS_UI_480x320 || HAS_UI_480x272
