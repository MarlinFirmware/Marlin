/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669 [https://blog.cyril.by]
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

#if HAS_TFT_LVGL_UI && ENABLED(NEOPIXEL_LED)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../inc/MarlinConfig.h"
#include "../../../../gcode/queue.h"

extern lv_group_t *g;
static lv_obj_t *scr,*cpicker,*help,*ledsel,*sliderLbl;


#define CPICKER_SIZE  (TFT_HEIGHT - titleHeight - 100)

enum {
  ID_LEDSTRIP_RETURN = 1,
};

static void make_slider_label(int value) {
  if (!value) lv_label_set_text(sliderLbl, more_menu.ledstrip_all);
  else {
    sprintf_P(public_buf_l, PSTR("%d"), value);
    lv_label_set_text(sliderLbl, public_buf_l);
  }
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (obj == cpicker || obj == ledsel) {
    if(event == LV_EVENT_VALUE_CHANGED) {
      int16_t v = lv_slider_get_value(ledsel);
      if (obj == ledsel) make_slider_label(v);
      // Need to send the command for the strip now
      lv_color_t color = lv_cpicker_get_color(cpicker);

      sprintf_P(public_buf_l, PSTR("M150 I%d R%d U%d B%d"), (int)v - 1, (int)LV_COLOR_GET_R(color), (int)LV_COLOR_GET_G(color), (int)LV_COLOR_GET_B(color));
      queue.enqueue_one_now(public_buf_l);
    }
    return;
  }
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_ledstrip();
  switch (obj->mks_obj_id) {
    case ID_LEDSTRIP_RETURN:
      lv_draw_more();
      return;
  }
}

void lv_draw_ledstrip() {
  scr = lv_screen_create(LEDSTRIP_UI, more_menu.ledstrip);
  // Create a label as an help
  help = lv_label_create(scr, TITLE_XPOS, titleHeight - TITLE_YPOS * 2, more_menu.ledstrip_help);
  // Need to create both a slider and a color picker here
  cpicker = lv_cpicker_create(scr, NULL);
  lv_obj_set_size(cpicker, CPICKER_SIZE, CPICKER_SIZE);
  lv_obj_align(cpicker, NULL, LV_ALIGN_IN_TOP_MID, 0, titleHeight * 2 - TITLE_YPOS * 2);
  static lv_style_t style_knob;
  lv_cpicker_set_knob_colored(cpicker, true);
  lv_style_copy(&style_knob, lv_cpicker_get_style(cpicker, LV_CPICKER_STYLE_KNOB));
  style_knob.body.padding.top = 10;
  style_knob.body.padding.left = 10;
  style_knob.body.padding.right = 10;
  style_knob.body.padding.bottom = 10;
  lv_cpicker_set_style(cpicker, LV_CPICKER_STYLE_KNOB, &style_knob);
  static lv_style_t style_main;
  lv_style_copy(&style_main, lv_cpicker_get_style(cpicker, LV_CPICKER_STYLE_MAIN));
  style_main.line.width = 8;
  lv_cpicker_set_style(cpicker, LV_CPICKER_STYLE_MAIN, &style_main);

  lv_obj_set_event_cb(cpicker, event_handler);

  // Finally create a slider for selecting the LED to run
  ledsel = lv_slider_create(scr, NULL);
  lv_obj_set_width(ledsel, TFT_WIDTH - 16 * TITLE_XPOS - 70); // Size of the back button
  lv_obj_align(ledsel, cpicker, LV_ALIGN_OUT_BOTTOM_LEFT, (CPICKER_SIZE-TFT_WIDTH) / 2 + 8*TITLE_XPOS, 3*TITLE_YPOS);
  lv_slider_set_range(ledsel, 0, NEOPIXEL_PIXELS);
  lv_obj_set_event_cb(ledsel, event_handler);

  sliderLbl = lv_label_create(scr, more_menu.ledstrip_all);
  make_slider_label(0);
  lv_obj_set_auto_realign(sliderLbl, true);                          /*To keep center alignment when the width of the text changes*/
  lv_obj_align(sliderLbl, ledsel, LV_ALIGN_OUT_BOTTOM_MID, 0, TITLE_YPOS);

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X + 10, PARA_UI_BACL_POS_Y, event_handler, ID_LEDSTRIP_RETURN, true);
}

void lv_clear_ledstrip() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
