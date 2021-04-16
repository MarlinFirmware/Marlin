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

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../module/temperature.h"
#include "../../../../gcode/queue.h"
#include "../../../../inc/MarlinConfig.h"

static lv_obj_t *scr;
extern lv_group_t *g;
static lv_obj_t *buttonType, *buttonStep, *buttonSpeed;
static lv_obj_t *labelType;
static lv_obj_t *labelStep;
static lv_obj_t *labelSpeed;
static lv_obj_t *tempText;
static lv_obj_t *ExtruText;

enum {
  ID_E_ADD = 1,
  ID_E_DEC,
  ID_E_TYPE,
  ID_E_STEP,
  ID_E_SPEED,
  ID_E_RETURN
};

static int32_t extrudeAmount;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_E_ADD:
      if (thermalManager.degHotend(uiCfg.extruderIndex) >= EXTRUDE_MINTEMP) {
        sprintf_P((char *)public_buf_l, PSTR("G91\nG1 E%d F%d\nG90"), uiCfg.extruStep, 60 * uiCfg.extruSpeed);
        queue.inject(public_buf_l);
        extrudeAmount += uiCfg.extruStep;
        disp_extru_amount();
      }
      break;
    case ID_E_DEC:
      if (thermalManager.degHotend(uiCfg.extruderIndex) >= EXTRUDE_MINTEMP) {
        sprintf_P((char *)public_buf_l, PSTR("G91\nG1 E%d F%d\nG90"), 0 - uiCfg.extruStep, 60 * uiCfg.extruSpeed);
        queue.enqueue_one_now(public_buf_l);
        extrudeAmount -= uiCfg.extruStep;
        disp_extru_amount();
      }
      break;
    case ID_E_TYPE:
      if (ENABLED(HAS_MULTI_EXTRUDER)) {
        if (uiCfg.extruderIndex == 0) {
          uiCfg.extruderIndex = 1;
          queue.inject_P(PSTR("T1"));
        }
        else {
          uiCfg.extruderIndex = 0;
          queue.inject_P(PSTR("T0"));
        }
      }
      else
        uiCfg.extruderIndex = 0;

      extrudeAmount = 0;
      disp_hotend_temp();
      disp_ext_type();
      disp_extru_amount();
      break;
    case ID_E_STEP:
      switch (abs(uiCfg.extruStep)) {
        case  1: uiCfg.extruStep = 5; break;
        case  5: uiCfg.extruStep = 10; break;
        case 10: uiCfg.extruStep = 1; break;
        default: break;
      }
      disp_ext_step();
      break;
    case ID_E_SPEED:
      switch (uiCfg.extruSpeed) {
        case  1: uiCfg.extruSpeed = 10; break;
        case 10: uiCfg.extruSpeed = 20; break;
        case 20: uiCfg.extruSpeed = 1; break;
        default: break;
      }
      disp_ext_speed();
      break;
    case ID_E_RETURN:
      clear_cur_ui();
      draw_return_ui();
      break;
  }
}

void lv_draw_extrusion() {
  scr = lv_screen_create(EXTRUSION_UI);
  // Create image buttons
  lv_obj_t *buttonAdd = lv_big_button_create(scr, "F:/bmp_in.bin", extrude_menu.in, INTERVAL_V, titleHeight, event_handler, ID_E_ADD);
  lv_obj_clear_protect(buttonAdd, LV_PROTECT_FOLLOW);
  lv_big_button_create(scr, "F:/bmp_out.bin", extrude_menu.out, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_E_DEC);

  buttonType = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_E_TYPE);
  buttonStep = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_E_STEP);
  buttonSpeed = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_E_SPEED);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonType);
      lv_group_add_obj(g, buttonStep);
      lv_group_add_obj(g, buttonSpeed);
    }
  #endif

  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_E_RETURN);

  // Create labels on the image buttons
  labelType = lv_label_create_empty(buttonType);
  labelStep = lv_label_create_empty(buttonStep);
  labelSpeed = lv_label_create_empty(buttonSpeed);

  disp_ext_type();
  disp_ext_step();
  disp_ext_speed();

  tempText = lv_label_create_empty(scr);
  lv_obj_set_style(tempText, &tft_style_label_rel);
  disp_hotend_temp();

  ExtruText = lv_label_create_empty(scr);
  lv_obj_set_style(ExtruText, &tft_style_label_rel);
  disp_extru_amount();
}

void disp_ext_type() {
  if (uiCfg.extruderIndex == 1) {
    lv_imgbtn_set_src_both(buttonType, "F:/bmp_extru2.bin");
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, extrude_menu.ext2);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
  else {
    lv_imgbtn_set_src_both(buttonType, "F:/bmp_extru1.bin");
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, extrude_menu.ext1);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_ext_speed() {
  if (uiCfg.extruSpeed == 20)
    lv_imgbtn_set_src_both(buttonSpeed, "F:/bmp_speed_high.bin");
  else if (uiCfg.extruSpeed == 1)
    lv_imgbtn_set_src_both(buttonSpeed, "F:/bmp_speed_slow.bin");
  else
    lv_imgbtn_set_src_both(buttonSpeed, "F:/bmp_speed_normal.bin");

  if (gCfgItems.multiple_language) {
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
  sprintf(buf, extrude_menu.temp_value, (int)thermalManager.degHotend(uiCfg.extruderIndex), (int)thermalManager.degTargetHotend(uiCfg.extruderIndex));
  strcpy(public_buf_l, extrude_menu.temper_text);
  strcat(public_buf_l, buf);
  lv_label_set_text(tempText, public_buf_l);
  lv_obj_align(tempText, nullptr, LV_ALIGN_CENTER, 0, -50);
}

void disp_extru_amount() {
  char buf1[10] = {0};

  public_buf_l[0] = '\0';

  if (extrudeAmount < 999 && extrudeAmount > -99)
    sprintf(buf1, extrude_menu.count_value_mm, extrudeAmount);
  else if (extrudeAmount < 9999 && extrudeAmount > -999)
    sprintf(buf1, extrude_menu.count_value_cm, extrudeAmount / 10);
  else
    sprintf(buf1, extrude_menu.count_value_m, extrudeAmount / 1000);
  strcat(public_buf_l, uiCfg.extruderIndex == 0 ? extrude_menu.ext1 : extrude_menu.ext2);
  strcat(public_buf_l, buf1);

  lv_label_set_text(ExtruText, public_buf_l);
  lv_obj_align(ExtruText, nullptr, LV_ALIGN_CENTER, 0, -75);
}

void disp_ext_step() {
  if (uiCfg.extruStep == 1)
    lv_imgbtn_set_src_both(buttonStep, "F:/bmp_step1_mm.bin");
  else if (uiCfg.extruStep == 5)
    lv_imgbtn_set_src_both(buttonStep, "F:/bmp_step5_mm.bin");
  else if (uiCfg.extruStep == 10)
    lv_imgbtn_set_src_both(buttonStep, "F:/bmp_step10_mm.bin");

  if (gCfgItems.multiple_language) {
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
