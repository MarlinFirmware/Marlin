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

#if HAS_TFT_LVGL_UI

#include "lv_conf.h"
#include "draw_ui.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/planner.h"
#include "../../../../module/stepper/indirection.h"
#include "../../../../feature/tmc_util.h"
#include "../../../../gcode/gcode.h"
#include "../../../../module/planner.h"

#if BUTTONS_EXIST(EN1, EN2)

extern lv_group_t * g;
static lv_obj_t * scr;
static lv_obj_t * buttonEncoderState = NULL;
static lv_obj_t *labelEncoderState = NULL;

#define ID_ENCODER_RETURN 1
#define ID_ENCODER_STATE 2

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_ENCODER_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_encoder_settings();
        draw_return_ui();
      }
      break;
    case ID_ENCODER_STATE:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        if (gCfgItems.encoder_enable) {
          gCfgItems.encoder_enable = false;
          lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_REL, "F:/bmp_disable.bin");
          lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_PR, "F:/bmp_disable.bin");
          lv_label_set_text(labelEncoderState, machine_menu.disable);
          update_spi_flash();
        }
        else {
          gCfgItems.encoder_enable = true;
          lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_REL, "F:/bmp_enable.bin");
          lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_PR, "F:/bmp_enable.bin");
          lv_label_set_text(labelEncoderState, machine_menu.enable);
          update_spi_flash();
        }
      }
      break;
  }
}

void lv_draw_encoder_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL;
  lv_obj_t *labelEncoderTips = NULL;

  lv_obj_t * line1 = NULL;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != ENCODER_SETTINGS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = ENCODER_SETTINGS_UI;
  }
  disp_state = ENCODER_SETTINGS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.EncoderConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  labelEncoderTips = lv_label_create(scr, NULL);
  lv_obj_set_style(labelEncoderTips, &tft_style_label_rel);
  lv_obj_set_pos(labelEncoderTips, PARA_UI_POS_X, PARA_UI_POS_Y + 10);
  lv_label_set_text(labelEncoderTips, machine_menu.EncoderConfText);

  buttonEncoderState = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonEncoderState, PARA_UI_STATE_POS_X, PARA_UI_POS_Y + PARA_UI_STATE_V);
  if (gCfgItems.encoder_enable) {
    lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_REL, "F:/bmp_enable.bin");
    lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_PR, "F:/bmp_enable.bin");
  }
  else {
    lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_REL, "F:/bmp_disable.bin");
    lv_imgbtn_set_src(buttonEncoderState, LV_BTN_STATE_PR, "F:/bmp_disable.bin");
  }

  lv_obj_set_event_cb_mks(buttonEncoderState, event_handler, ID_ENCODER_STATE, NULL, 0);

  lv_imgbtn_set_style(buttonEncoderState, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonEncoderState, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonEncoderState, LV_LAYOUT_OFF);
  labelEncoderState = lv_label_create(buttonEncoderState, NULL);

  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1, line_points[0]);

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_ENCODER_RETURN, NULL, 0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_back70x40.bin");
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_back70x40.bin");
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.encoder_enable) {
    lv_label_set_text(labelEncoderState, machine_menu.enable);
    lv_obj_align(labelEncoderState, buttonEncoderState, LV_ALIGN_CENTER, 0, 0);
  }
  else {
    lv_label_set_text(labelEncoderState, machine_menu.disable);
    lv_obj_align(labelEncoderState, buttonEncoderState, LV_ALIGN_CENTER, 0, 0);
  }

  lv_label_set_text(label_Back, common_menu.text_back);
  lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonEncoderState);
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

void lv_clear_encoder_settings() {
  #if HAS_ROTARY_ENCODER
    lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // BUTTONS_EXIST(EN1, EN2)

#endif // HAS_TFT_LVGL_UI
