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

#include "draw_ready_print.h"
#include "draw_tool.h"
#include <lv_conf.h>
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"
#include "tft_lvgl_configuration.h"
#include "mks_hardware_test.h"
#include "draw_ui.h"

#include <lvgl.h>

#include "../../../../module/temperature.h"
#include "../../../../inc/MarlinConfig.h"

#include <stdio.h>

//static lv_obj_t *buttonPrint, *buttonTool, *buttonSet;
extern lv_group_t*  g;
static lv_obj_t *scr;
#if ENABLED(MKS_TEST)
  uint8_t curent_disp_ui = 0;
#endif

enum {
  ID_TOOL = 1,
  ID_SET,
  ID_PRINT
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_TOOL:
      lv_clear_ready_print();
      lv_draw_tool();
      break;
    case ID_SET:
      lv_clear_ready_print();
      lv_draw_set();
      break;
    case ID_PRINT:
      lv_clear_ready_print();
      lv_draw_print_file();
      break;
  }
}

lv_obj_t *limit_info, *det_info;
lv_style_t limit_style, det_style;
void disp_Limit_ok() {
  limit_style.text.color.full = 0xFFFF;
  lv_obj_set_style(limit_info, &limit_style);
  lv_label_set_text(limit_info, "Limit:ok");
}
void disp_Limit_error() {
  limit_style.text.color.full = 0xF800;
  lv_obj_set_style(limit_info, &limit_style);
  lv_label_set_text(limit_info, "Limit:error");
}

void disp_det_ok() {
  det_style.text.color.full = 0xFFFF;
  lv_obj_set_style(det_info, &det_style);
  lv_label_set_text(det_info, "det:ok");
}
void disp_det_error() {
  det_style.text.color.full = 0xF800;
  lv_obj_set_style(det_info, &det_style);
  lv_label_set_text(det_info, "det:error");
}

lv_obj_t *e1, *e2, *e3, *bed;
void mks_disp_test() {
  char buf[30] = {0};
  //lv_obj_t *label_tool2 = lv_label_create_empty(scr);
  //lv_obj_set_pos(label_tool, 20, 50);
  sprintf_P(buf, PSTR("e1:%d"), (int)thermalManager.temp_hotend[0].celsius);
  lv_label_set_text(e1, buf);
  #if HAS_MULTI_HOTEND
    sprintf_P(buf, PSTR("e2:%d"), (int)thermalManager.temp_hotend[1].celsius);
    lv_label_set_text(e2, buf);
  #endif

  //sprintf_P(buf, PSTR("e3:%d"), (int)thermalManager.temp_hotend[2].celsius);
  //lv_label_set_text(e3, buf);
  #if HAS_HEATED_BED
    sprintf_P(buf, PSTR("bed:%d"), (int)thermalManager.temp_bed.celsius);
    lv_label_set_text(bed, buf);
  #endif
}

void lv_draw_ready_print(void) {
  char buf[30] = {0};
  lv_obj_t *buttonTool;

  disp_state_stack._disp_index = 0;
  ZERO(disp_state_stack._disp_state);
  scr = lv_screen_create(PRINT_READY_UI, "");
  //lv_obj_set_hidden(scr, true);

  if (mks_test_flag == 0x1E) {
    //(void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

    // Create image buttons
    //buttonPrint = lv_imgbtn_create(scr, nullptr);
    buttonTool = lv_imgbtn_create(scr, "F:/bmp_tool.bin", event_handler, ID_TOOL);

    lv_obj_set_pos(buttonTool, 360, 180);

    //buttonSet = lv_imgbtn_create(scr, nullptr);
    //lv_obj_set_pos(buttonSet, 180, 90);
    //lv_obj_set_pos(buttonPrint, 340, 90);

    //lv_obj_set_pos(buttonTool, SIMPLE_FIRST_PAGE_GRAP+1, (TFT_HEIGHT-BTN_Y_PIXEL)/2+2);
    //lv_obj_set_pos(buttonSet, BTN_X_PIXEL+SIMPLE_FIRST_PAGE_GRAP*2+1, (TFT_HEIGHT-BTN_Y_PIXEL)/2+2);
    //lv_obj_set_pos(buttonPrint, BTN_X_PIXEL*2+SIMPLE_FIRST_PAGE_GRAP*3+1, (TFT_HEIGHT-BTN_Y_PIXEL)/2+2);

    // Create labels on the image buttons
    //lv_btn_set_layout(buttonPrint, LV_LAYOUT_OFF);
    //lv_btn_set_layout(buttonSet, LV_LAYOUT_OFF);

    //lv_obj_t *label_print = lv_label_create_empty(buttonPrint);
    //lv_obj_t *label_set = lv_label_create_empty(buttonSet);
    lv_obj_t *label_tool = lv_label_create_empty(buttonTool);
    if (gCfgItems.multiple_language) {
      //lv_label_set_text(label_print, main_menu.print);
      //lv_obj_align(label_print, buttonPrint, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      //lv_label_set_text(label_set, main_menu.set);
      //lv_obj_align(label_set, buttonSet, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      //lv_label_set_style(label_tool, LV_BTN_STATE_PR, &tft_style_label_pre);
      //lv_label_set_style(label_tool, LV_BTN_STATE_REL, &tft_style_label_rel);
      lv_label_set_text(label_tool, main_menu.tool);
      lv_obj_align(label_tool, buttonTool, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }

    #if 1
      e1 = lv_label_create_empty(scr);
      lv_obj_set_pos(e1, 20, 20);
      sprintf_P(buf, PSTR("e1:  %d"), (int)thermalManager.temp_hotend[0].celsius);
      lv_label_set_text(e1, buf);
      #if HAS_MULTI_HOTEND
        e2 = lv_label_create_empty(scr);
        lv_obj_set_pos(e2, 20, 45);
        sprintf_P(buf, PSTR("e1:  %d"), (int)thermalManager.temp_hotend[1].celsius);
        lv_label_set_text(e2, buf);
      #endif

      //e3 = lv_label_create_empty(scr);
      //lv_obj_set_pos(e3, 20, 70);
      //sprintf_P(buf, PSTR("e1:  %d"), (int)thermalManager.temp_hotend[2].celsius);
      //lv_label_set_text(e3, buf);

      #if HAS_HEATED_BED
        bed = lv_label_create_empty(scr);
        lv_obj_set_pos(bed, 20, 95);
        sprintf_P(buf, PSTR("bed:  %d"), (int)thermalManager.temp_bed.celsius);
        lv_label_set_text(bed, buf);
      #endif

      limit_info = lv_label_create_empty(scr);

      lv_style_copy(&limit_style, &lv_style_scr);
      limit_style.body.main_color.full = 0X0000;
      limit_style.body.grad_color.full = 0X0000;
      limit_style.text.color.full      = 0Xffff;
      lv_obj_set_style(limit_info, &limit_style);

      lv_obj_set_pos(limit_info, 20, 120);
      lv_label_set_text(limit_info, " ");

      det_info = lv_label_create_empty(scr);

      lv_style_copy(&det_style, &lv_style_scr);
      det_style.body.main_color.full = 0X0000;
      det_style.body.grad_color.full = 0X0000;
      det_style.text.color.full      = 0Xffff;
      lv_obj_set_style(det_info, &det_style);

      lv_obj_set_pos(det_info, 20, 145);
      lv_label_set_text(det_info, " ");
    #endif // if 1

  }
  else {
    lv_big_button_create(scr, "F:/bmp_tool.bin", main_menu.tool, 20, 90, event_handler, ID_TOOL);
    lv_big_button_create(scr, "F:/bmp_set.bin", main_menu.set, 180, 90, event_handler, ID_SET);
    lv_big_button_create(scr, "F:/bmp_printing.bin", main_menu.print, 340, 90, event_handler, ID_PRINT);
  }
}

void lv_clear_ready_print() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable == true) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
