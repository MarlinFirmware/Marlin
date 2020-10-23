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

#if BOTH(HAS_TFT_LVGL_UI, HAS_BED_PROBE)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../module/probe.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_OFFSET_RETURN   1
#define ID_OFFSET_X        2
#define ID_OFFSET_Y        3
#define ID_OFFSET_Z        4

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_OFFSET_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_auto_level_offset_settings();
        draw_return_ui();
      }
      break;
    case ID_OFFSET_X:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = x_offset;
        lv_clear_auto_level_offset_settings();
        lv_draw_number_key();
      }
      break;
    case ID_OFFSET_Y:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = y_offset;
        lv_clear_auto_level_offset_settings();
        lv_draw_number_key();
      }
      break;
    case ID_OFFSET_Z:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = z_offset;
        lv_clear_auto_level_offset_settings();
        lv_draw_number_key();
      }
      break;
  }
}

void lv_draw_auto_level_offset_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL;
  lv_obj_t *buttonXValue = NULL, *labelXValue = NULL;
  lv_obj_t *buttonYValue = NULL, *labelYValue = NULL;
  lv_obj_t *buttonZValue = NULL, *labelZValue = NULL;
  lv_obj_t * line1 = NULL, * line2 = NULL, * line3 = NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != NOZZLE_PROBE_OFFSET_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = NOZZLE_PROBE_OFFSET_UI;
  }
  disp_state = NOZZLE_PROBE_OFFSET_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.OffsetConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y + 10, machine_menu.Xoffset);

  buttonXValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_OFFSET_X);
  labelXValue = lv_label_create_empty(buttonXValue);

  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1, line_points[0]);

  lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10, machine_menu.Yoffset);

  buttonYValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_OFFSET_Y);
  labelYValue = lv_label_create_empty(buttonYValue);

  line2 = lv_line_create(scr, NULL);
  lv_ex_line(line2, line_points[1]);

  (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 10, machine_menu.Zoffset);

  buttonZValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_OFFSET_Z);
  labelZValue = lv_label_create_empty(buttonZValue);

  line3 = lv_line_create(scr, NULL);
  lv_ex_line(line3, line_points[2]);

  buttonBack = lv_btn_create_back(scr, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE, event_handler, ID_OFFSET_RETURN);
  label_Back = lv_label_create_empty(buttonBack);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonXValue);
      lv_group_add_obj(g, buttonYValue);
      lv_group_add_obj(g, buttonZValue);
      lv_group_add_obj(g, buttonBack);
    }
  #endif

  if (gCfgItems.multiple_language) {
    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), TERN(HAS_PROBE_XY_OFFSET, probe.offset.x, 0));
    lv_label_set_text(labelXValue, public_buf_l);
    lv_obj_align(labelXValue, buttonXValue, LV_ALIGN_CENTER, 0, 0);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), TERN(HAS_PROBE_XY_OFFSET, probe.offset.y, 0));
    lv_label_set_text(labelYValue, public_buf_l);
    lv_obj_align(labelYValue, buttonYValue, LV_ALIGN_CENTER, 0, 0);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), probe.offset.z);
    lv_label_set_text(labelZValue, public_buf_l);
    lv_obj_align(labelZValue, buttonZValue, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }
}

void lv_clear_auto_level_offset_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && HAS_BED_PROBE
