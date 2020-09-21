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
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"

static lv_obj_t * scr;
extern lv_group_t*  g;
static lv_obj_t * buttoType, *buttonStep, *buttonSpeed;
static lv_obj_t *labelType;
static lv_obj_t *labelStep;
static lv_obj_t *labelSpeed;
static lv_obj_t * tempText;
static lv_obj_t * ExtruText;

#define ID_E_ADD    1
#define ID_E_DEC    2
#define ID_E_TYPE   3
#define ID_E_STEP   4
#define ID_E_SPEED  5
#define ID_E_RETURN 6

static int32_t extructAmount;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_E_ADD:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius >= EXTRUDE_MINTEMP) {
          queue.enqueue_now_P(PSTR("G91"));
          ZERO(public_buf_l);
          sprintf_P((char *)public_buf_l, PSTR("G1 E%d F%d"), uiCfg.extruStep, 60 * uiCfg.extruSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G90"));
          extructAmount += uiCfg.extruStep;
          disp_extru_amount();
        }
      }
      break;
    case ID_E_DEC:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius >= EXTRUDE_MINTEMP) {
          queue.enqueue_now_P(PSTR("G91"));
          ZERO(public_buf_l);
          sprintf_P((char *)public_buf_l, PSTR("G1 E%d F%d"), 0 - uiCfg.extruStep, 60 * uiCfg.extruSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G90"));
          extructAmount -= uiCfg.extruStep;
          disp_extru_amount();
        }
      }
      break;
    case ID_E_TYPE:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (ENABLED(HAS_MULTI_EXTRUDER)) {
          if (uiCfg.curSprayerChoose == 0) {
            uiCfg.curSprayerChoose = 1;
            queue.inject_P(PSTR("T1"));
          }
          else {
            uiCfg.curSprayerChoose = 0;
            queue.inject_P(PSTR("T0"));
          }
        }
        else
          uiCfg.curSprayerChoose = 0;

        extructAmount = 0;
        disp_hotend_temp();
        disp_ext_type();
        disp_extru_amount();
      }
      break;
    case ID_E_STEP:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        switch (abs(uiCfg.extruStep)) {
          case  1: uiCfg.extruStep = 5; break;
          case  5: uiCfg.extruStep = 10; break;
          case 10: uiCfg.extruStep = 1; break;
          default: break;
        }
        disp_ext_step();
      }
      break;
    case ID_E_SPEED:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        switch (uiCfg.extruSpeed) {
          case  1: uiCfg.extruSpeed = 10; break;
          case 10: uiCfg.extruSpeed = 20; break;
          case 20: uiCfg.extruSpeed = 1; break;
          default: break;
        }
        disp_ext_speed();
      }
      break;
    case ID_E_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        clear_cur_ui();
        draw_return_ui();
      }
      break;
  }
}

void lv_draw_extrusion(void) {
  lv_obj_t *buttonAdd, *buttonDec, *buttonBack;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != EXTRUSION_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = EXTRUSION_UI;
  }
  disp_state = EXTRUSION_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create image buttons
  buttonAdd   = lv_imgbtn_create(scr, NULL);
  buttonDec   = lv_imgbtn_create(scr, NULL);
  buttoType   = lv_imgbtn_create(scr, NULL);
  buttonStep  = lv_imgbtn_create(scr, NULL);
  buttonSpeed = lv_imgbtn_create(scr, NULL);
  buttonBack  = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonAdd, event_handler, ID_E_ADD, NULL, 0);
  lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_REL, "F:/bmp_in.bin");
  lv_imgbtn_set_src(buttonAdd, LV_BTN_STATE_PR, "F:/bmp_in.bin");
  lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonAdd, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonAdd, LV_PROTECT_FOLLOW);

  #if 1
    lv_obj_set_event_cb_mks(buttonDec, event_handler, ID_E_DEC, NULL, 0);
    lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_REL, "F:/bmp_out.bin");
    lv_imgbtn_set_src(buttonDec, LV_BTN_STATE_PR, "F:/bmp_out.bin");
    lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonDec, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttoType, event_handler, ID_E_TYPE, NULL, 0);
    lv_imgbtn_set_style(buttoType, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttoType, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonStep, event_handler, ID_E_STEP, NULL, 0);
    lv_imgbtn_set_style(buttonStep, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonStep, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonSpeed, event_handler, ID_E_SPEED, NULL, 0);
    lv_imgbtn_set_style(buttonSpeed, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonSpeed, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_E_RETURN, NULL, 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  #endif

  lv_obj_set_pos(buttonAdd, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonDec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
  lv_obj_set_pos(buttoType, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonStep, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonSpeed, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  // Create labels on the image buttons
  lv_btn_set_layout(buttonAdd, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonDec, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttoType, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonStep, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonSpeed, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  lv_obj_t *labelAdd   = lv_label_create(buttonAdd, NULL);
  lv_obj_t *labelDec   = lv_label_create(buttonDec, NULL);
  labelType             = lv_label_create(buttoType, NULL);
  labelStep             = lv_label_create(buttonStep, NULL);
  labelSpeed            = lv_label_create(buttonSpeed, NULL);
  lv_obj_t *label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(labelAdd, extrude_menu.in);
    lv_obj_align(labelAdd, buttonAdd, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelDec, extrude_menu.out);
    lv_obj_align(labelDec, buttonDec, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonAdd);
      lv_group_add_obj(g, buttonDec);
      lv_group_add_obj(g, buttoType);
      lv_group_add_obj(g, buttonStep);
      lv_group_add_obj(g, buttonSpeed);
      lv_group_add_obj(g, buttonBack);
    }
  #endif

  disp_ext_type();
  disp_ext_step();
  disp_ext_speed();

  tempText = lv_label_create(scr, NULL);
  lv_obj_set_style(tempText, &tft_style_label_rel);
  disp_hotend_temp();

  ExtruText = lv_label_create(scr, NULL);
  lv_obj_set_style(ExtruText, &tft_style_label_rel);
  disp_extru_amount();
}

void disp_ext_type() {
  if (uiCfg.curSprayerChoose == 1) {
    lv_imgbtn_set_src(buttoType, LV_BTN_STATE_REL, "F:/bmp_extru2.bin");
    lv_imgbtn_set_src(buttoType, LV_BTN_STATE_PR, "F:/bmp_extru2.bin");
    if (gCfgItems.multiple_language != 0) {
      lv_label_set_text(labelType, extrude_menu.ext2);
      lv_obj_align(labelType, buttoType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
  else {
    lv_imgbtn_set_src(buttoType, LV_BTN_STATE_REL, "F:/bmp_extru1.bin");
    lv_imgbtn_set_src(buttoType, LV_BTN_STATE_PR, "F:/bmp_extru1.bin");
    if (gCfgItems.multiple_language != 0) {
      lv_label_set_text(labelType, extrude_menu.ext1);
      lv_obj_align(labelType, buttoType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_ext_speed() {
  if (uiCfg.extruSpeed == 20) {
    lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_REL, "F:/bmp_speed_high.bin");
    lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_PR, "F:/bmp_speed_high.bin");
  }
  else if (uiCfg.extruSpeed == 1) {
  lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_REL, "F:/bmp_speed_slow.bin");
    lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_PR, "F:/bmp_speed_slow.bin");
  }
  else {
  lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_REL, "F:/bmp_speed_normal.bin");
    lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_PR, "F:/bmp_speed_normal.bin");
  }

  if (gCfgItems.multiple_language != 0) {
    if (uiCfg.extruSpeed == 20) {
      lv_label_set_text(labelSpeed, extrude_menu.high);
      lv_obj_align(labelSpeed, buttonSpeed, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if (uiCfg.extruSpeed == 1) {
      lv_label_set_text(labelSpeed, extrude_menu.low);
      lv_obj_align(labelSpeed, buttonSpeed, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else {
      lv_label_set_text(labelSpeed, extrude_menu.normal);
      lv_obj_align(labelSpeed, buttonSpeed, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_hotend_temp() {
  char buf[20] = {0};
  public_buf_l[0] = '\0';
  strcat(public_buf_l, extrude_menu.temper_text);
  sprintf(buf, extrude_menu.temp_value, (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].celsius,  (int)thermalManager.temp_hotend[uiCfg.curSprayerChoose].target);
  strcat(public_buf_l, buf);
  lv_label_set_text(tempText, public_buf_l);
  lv_obj_align(tempText, NULL, LV_ALIGN_CENTER, 0, -50);
}

void disp_extru_amount() {
  char buf1[10] = {0};

  public_buf_l[0] = '\0';

  if (extructAmount < 999 && extructAmount > -99) {
    sprintf(buf1, extrude_menu.count_value_mm, extructAmount);
    if (uiCfg.curSprayerChoose < 1)
      strcat(public_buf_l, extrude_menu.ext1);
    else
      strcat(public_buf_l, extrude_menu.ext2);
    strcat(public_buf_l, buf1);
  }
  else if (extructAmount < 9999 && extructAmount > -999) {
    sprintf(buf1, extrude_menu.count_value_cm, extructAmount / 10);
    if (uiCfg.curSprayerChoose < 1)
      strcat(public_buf_l, extrude_menu.ext1);
    else
      strcat(public_buf_l, extrude_menu.ext2);
    strcat(public_buf_l, buf1);
  }
  else {
    sprintf(buf1, extrude_menu.count_value_m, extructAmount / 1000);
    if (uiCfg.curSprayerChoose < 1)
      strcat(public_buf_l, extrude_menu.ext1);
    else
      strcat(public_buf_l, extrude_menu.ext2);
    strcat(public_buf_l, buf1);
  }

  lv_label_set_text(ExtruText, public_buf_l);
  lv_obj_align(ExtruText, NULL, LV_ALIGN_CENTER, 0, -75);
}

void disp_ext_step() {
  if (uiCfg.extruStep == 1) {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, "F:/bmp_step1_mm.bin");
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, "F:/bmp_step1_mm.bin");
  }
  else if (uiCfg.extruStep == 5) {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, "F:/bmp_step5_mm.bin");
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, "F:/bmp_step5_mm.bin");
  }
  else if (uiCfg.extruStep == 10) {
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_REL, "F:/bmp_step10_mm.bin");
    lv_imgbtn_set_src(buttonStep, LV_BTN_STATE_PR, "F:/bmp_step10_mm.bin");
  }

  if (gCfgItems.multiple_language != 0) {
    if (uiCfg.extruStep == 1) {
      lv_label_set_text(labelStep, extrude_menu.step_1mm);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if (uiCfg.extruStep == 5) {
      lv_label_set_text(labelStep, extrude_menu.step_5mm);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if (uiCfg.extruStep == 10) {
      lv_label_set_text(labelStep, extrude_menu.step_10mm);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void lv_clear_extrusion() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
