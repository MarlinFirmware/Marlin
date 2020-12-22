/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../../inc/MarlinConfigPre.h"

#if BOTH(HAS_TFT_LVGL_UI, TOUCH_SCREEN_CALIBRATION)

#include "draw_ui.h"
#include "draw_touch_calibration.h"
#include <lv_conf.h>

#include "../../../../inc/MarlinConfig.h"
#include "../../../tft_io/touch_calibration.h"
#include "SPI_TFT.h"

static lv_obj_t *scr;
static lv_obj_t *status_label;

static void event_handler(lv_obj_t *obj, lv_event_t event);

enum {
  ID_TC_RETURN = 1
};

static void drawCross(uint16_t x, uint16_t y, uint16_t color) {
  SPI_TFT.tftio.set_window(x - 15, y, x + 15, y);
  SPI_TFT.tftio.WriteMultiple(color, 31);
  SPI_TFT.tftio.set_window(x, y - 15, x, y + 15);
  SPI_TFT.tftio.WriteMultiple(color, 31);
}

void lv_update_touch_calibration_screen() {
  uint16_t x, y;

  calibrationState calibration_stage = touch_calibration.get_calibration_state();
  if (calibration_stage == CALIBRATION_NONE) {
    // start and clear screen
    calibration_stage = touch_calibration.calibration_start();
  }
  else {
    // clear last cross
    x = touch_calibration.calibration_points[_MIN(calibration_stage - 1, CALIBRATION_BOTTOM_RIGHT)].x;
    y = touch_calibration.calibration_points[_MIN(calibration_stage - 1, CALIBRATION_BOTTOM_RIGHT)].y;
    drawCross(x, y, LV_COLOR_BACKGROUND.full);
  }

  const char *str = nullptr;
  if (calibration_stage < CALIBRATION_SUCCESS) {
    // handle current state
    switch (calibration_stage) {
      case CALIBRATION_TOP_LEFT: str = GET_TEXT(MSG_TOP_LEFT); break;
      case CALIBRATION_BOTTOM_LEFT: str = GET_TEXT(MSG_BOTTOM_LEFT); break;
      case CALIBRATION_TOP_RIGHT:  str = GET_TEXT(MSG_TOP_RIGHT); break;
      case CALIBRATION_BOTTOM_RIGHT: str = GET_TEXT(MSG_BOTTOM_RIGHT); break;
      default: break;
    }

    x = touch_calibration.calibration_points[calibration_stage].x;
    y = touch_calibration.calibration_points[calibration_stage].y;
    drawCross(x, y, LV_COLOR_WHITE.full);
  }
  else {
    // end calibration
    str = calibration_stage == CALIBRATION_SUCCESS ? GET_TEXT(MSG_CALIBRATION_COMPLETED) : GET_TEXT(MSG_CALIBRATION_FAILED);
    touch_calibration.calibration_end();
    lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_TC_RETURN);
  }

  // draw current message
  lv_label_set_text(status_label, str);
  lv_obj_align(status_label, nullptr, LV_ALIGN_CENTER, 0, 0);
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_TC_RETURN:
      TERN_(MKS_TEST, curent_disp_ui = 1);
      lv_clear_touch_calibration_screen();
      draw_return_ui();
      break;
  }
}

void lv_draw_touch_calibration_screen() {
  scr = lv_screen_create(TOUCH_CALIBRATION_UI, "");

  status_label = lv_label_create(scr, "");
  lv_obj_align(status_label, nullptr, LV_ALIGN_CENTER, 0, 0);

  lv_refr_now(lv_refr_get_disp_refreshing());

  lv_update_touch_calibration_screen();
}

void lv_clear_touch_calibration_screen() {
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && TOUCH_SCREEN_CALIBRATION
