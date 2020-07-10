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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_LVGL_UI)

#include "draw_ready_print.h"
#include "draw_tool.h"
#include "lv_conf.h"
#include "lvgl.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"
#include "tft_lvgl_configuration.h"
#include "mks_hardware_test.h"
#include "draw_ui.h"

#include <stdio.h>

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"

//static lv_obj_t *buttonPrint,*buttonTool,*buttonSet;
static lv_obj_t * scr;
#if ENABLED(MKS_TEST)
  uint8_t curent_disp_ui = 0;
#endif

#define ID_TOOL   1
#define ID_SET    2
#define ID_PRINT  3

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_TOOL:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(MKS_TEST)
          curent_disp_ui = 2;
        #endif
        lv_obj_del(scr);
        lv_draw_tool();
      }
      break;
    case ID_SET:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_obj_del(scr);
        lv_draw_set();
      }
      break;
    case ID_PRINT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_obj_del(scr);
        lv_draw_print_file();
      }
      break;
  }
}


#if ENABLED(MKS_TEST)

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
  void disp_test() {
    char buf[30] = {0};
    //lv_obj_t * label_tool2 = lv_label_create(scr, NULL);
    //lv_obj_set_pos(label_tool,20,50);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "e1:%d", (int)thermalManager.temp_hotend[0].celsius);
    lv_label_set_text(e1, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "e2:%d", (int)thermalManager.temp_hotend[1].celsius);
    lv_label_set_text(e2, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "e3:%d", (int)thermalManager.temp_hotend[2].celsius);
    lv_label_set_text(e3, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "bed:%d", (int)thermalManager.temp_bed.celsius);
    lv_label_set_text(bed, buf);
  }

#endif // MKS_TEST

void lv_draw_ready_print(void) {
  #if ENABLED(MKS_TEST)
    char buf[30] = {0};
    lv_obj_t *buttonTool;

    static lv_style_t style_pr, style_rel;

    curent_disp_ui = 1;

    scr = lv_obj_create(NULL, NULL);

    static lv_style_t ready_style;

    lv_style_copy(&ready_style, &lv_style_scr);
    //ready_style.body.main_color.full = 0X18C3;
    //ready_style.body.grad_color.full = 0X18C3;
    ready_style.body.main_color.full = 0X0000;
    ready_style.body.grad_color.full = 0X0000;
    ready_style.text.color.full = 0Xffff;
    lv_obj_set_style(scr, &ready_style);
    lv_scr_load(scr);
    lv_obj_clean(scr);
    //lv_obj_set_hidden(scr,true);
    lv_refr_now(lv_refr_get_disp_refreshing());

    LV_IMG_DECLARE(bmp_pic);
    //LV_IMG_DECLARE(bmp_pic2);
    //LV_IMG_DECLARE(bmp_pic3);

    //scr = lv_obj_create(NULL, NULL);
    //lv_scr_load(scr);

    /*Create an Image button*/
    buttonTool = lv_imgbtn_create(scr, NULL);

    //lv_btn_setting(&style_pr,0x5d8f16,0x5d8f16);
    //lv_btn_setting(&style_rel,0x5d8f16,0x5d8f16);

    lv_obj_set_event_cb_mks(buttonTool, event_handler, 3, "bmp_Tool.bin", 0);
    //lv_imgbtn_set_src_mks(buttonTool, LV_BTN_STATE_REL, &bmp_pic,(uint8_t *)"bmp_tool.bin");
    lv_imgbtn_set_src(buttonTool, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonTool, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonTool, LV_BTN_STATE_PR, &style_pr);
    lv_imgbtn_set_style(buttonTool, LV_BTN_STATE_REL, &style_rel);

    lv_obj_set_pos(buttonTool, 360, 180);

    lv_btn_set_layout(buttonTool, LV_LAYOUT_OFF);

    lv_obj_t * label_tool = lv_label_create(buttonTool, NULL);

    lv_obj_set_pos(label_tool, 30, 100);
    lv_label_set_text(label_tool, "Back");

    e1 = lv_label_create(scr, NULL);
    lv_obj_set_pos(e1, 20, 20);
    sprintf(buf, "e1:  %d", (int)thermalManager.temp_hotend[0].celsius);
    lv_label_set_text(e1, buf);

    e2 = lv_label_create(scr, NULL);
    lv_obj_set_pos(e2, 20, 45);
    sprintf(buf, "e1:  %d", (int)thermalManager.temp_hotend[1].celsius);
    lv_label_set_text(e2, buf);

    e3 = lv_label_create(scr, NULL);
    lv_obj_set_pos(e3, 20, 70);
    sprintf(buf, "e1:  %d", (int)thermalManager.temp_hotend[2].celsius);
    lv_label_set_text(e3, buf);

    bed = lv_label_create(scr, NULL);
    lv_obj_set_pos(bed, 20, 95);
    sprintf(buf, "bed:  %d", (int)thermalManager.temp_bed.celsius);
    lv_label_set_text(bed, buf);

    limit_info = lv_label_create(scr, NULL);

    lv_style_copy(&limit_style, &lv_style_scr);
    limit_style.body.main_color.full = 0X0000;
    limit_style.body.grad_color.full = 0X0000;
    limit_style.text.color.full = 0Xffff;
    lv_obj_set_style(limit_info, &limit_style);

    lv_obj_set_pos(limit_info, 20, 120);
    lv_label_set_text(limit_info, " ");

    det_info = lv_label_create(scr, NULL);

    lv_style_copy(&det_style, &lv_style_scr);
    det_style.body.main_color.full = 0X0000;
    det_style.body.grad_color.full = 0X0000;
    det_style.text.color.full = 0Xffff;
    lv_obj_set_style(det_info, &det_style);

    lv_obj_set_pos(det_info, 20, 145);
    lv_label_set_text(det_info, " ");
  #else // !MKS_TEST
    lv_obj_t *buttonPrint, *buttonTool, *buttonSet;

    disp_state_stack._disp_index = 0;
    memset(disp_state_stack._disp_state, 0, sizeof(disp_state_stack._disp_state));
    disp_state_stack._disp_state[disp_state_stack._disp_index] = PRINT_READY_UI;

    disp_state = PRINT_READY_UI;

    scr = lv_obj_create(NULL, NULL);

    lv_obj_set_style(scr, &tft_style_scr);
    lv_scr_load(scr);
    lv_obj_clean(scr);
    //lv_obj_set_hidden(scr,true);
    lv_refr_now(lv_refr_get_disp_refreshing());

    LV_IMG_DECLARE(bmp_pic);

    //lv_obj_t * title = lv_label_create(scr, NULL);
    //lv_obj_set_style(title, &tft_style_lable_rel);
    //lv_obj_set_pos(title,TITLE_XPOS,TITLE_YPOS);
    //lv_label_set_text(title, creat_title_text());

    /*Create an Image button*/
    buttonPrint = lv_imgbtn_create(scr, NULL);
    buttonTool = lv_imgbtn_create(scr, NULL);
    buttonSet = lv_imgbtn_create(scr, NULL);

    lv_obj_set_event_cb_mks(buttonPrint, event_handler, ID_PRINT, "bmp_printing.bin", 0);
    //lv_imgbtn_set_src_mks(buttonPrint, LV_BTN_STATE_REL, &bmp_pic,(uint8_t *)"bmp_printing.bin");
    lv_imgbtn_set_src(buttonPrint, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonPrint, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonPrint, LV_BTN_STATE_PR, &tft_style_lable_pre);
    lv_imgbtn_set_style(buttonPrint, LV_BTN_STATE_REL, &tft_style_lable_rel);
    lv_obj_clear_protect(buttonPrint, LV_PROTECT_FOLLOW);
    #if 1
      lv_obj_set_event_cb_mks(buttonSet, event_handler, ID_SET, "bmp_Set.bin", 0);
      //lv_imgbtn_set_src_mks(buttonSet, LV_BTN_STATE_REL, &bmp_pic,(uint8_t *)"bmp_set.bin");
      lv_imgbtn_set_src(buttonSet, LV_BTN_STATE_REL, &bmp_pic);
      lv_imgbtn_set_src(buttonSet, LV_BTN_STATE_PR, &bmp_pic);
      lv_imgbtn_set_style(buttonSet, LV_BTN_STATE_PR, &tft_style_lable_pre);
      lv_imgbtn_set_style(buttonSet, LV_BTN_STATE_REL, &tft_style_lable_rel);

      lv_obj_set_event_cb_mks(buttonTool, event_handler, ID_TOOL, "bmp_Tool.bin", 0);
      //lv_imgbtn_set_src_mks(buttonTool, LV_BTN_STATE_REL, &bmp_pic,(uint8_t *)"bmp_tool.bin");
      lv_imgbtn_set_src(buttonTool, LV_BTN_STATE_REL, &bmp_pic);
      lv_imgbtn_set_src(buttonTool, LV_BTN_STATE_PR, &bmp_pic);
      lv_imgbtn_set_style(buttonTool, LV_BTN_STATE_PR, &tft_style_lable_pre);
      lv_imgbtn_set_style(buttonTool, LV_BTN_STATE_REL, &tft_style_lable_rel);
    #endif
    lv_obj_set_pos(buttonTool, 20, 90);
    lv_obj_set_pos(buttonSet, 180, 90);
    lv_obj_set_pos(buttonPrint, 340, 90);

    //lv_obj_set_pos(buttonTool,SIMPLE_FIRST_PAGE_GRAP+1,(TFT_HEIGHT-BTN_Y_PIXEL)/2+2);
    //lv_obj_set_pos(buttonSet,BTN_X_PIXEL+SIMPLE_FIRST_PAGE_GRAP*2+1,(TFT_HEIGHT-BTN_Y_PIXEL)/2+2);
    //lv_obj_set_pos(buttonPrint,BTN_X_PIXEL*2+SIMPLE_FIRST_PAGE_GRAP*3+1,(TFT_HEIGHT-BTN_Y_PIXEL)/2+2);

    /*Create a label on the Image button*/
    lv_btn_set_layout(buttonPrint, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonSet, LV_LAYOUT_OFF);
    lv_btn_set_layout(buttonTool, LV_LAYOUT_OFF);

    lv_obj_t * label_print = lv_label_create(buttonPrint, NULL);
    lv_obj_t * label_set = lv_label_create(buttonSet, NULL);
    lv_obj_t * label_tool = lv_label_create(buttonTool, NULL);
    if (gCfgItems.multiple_language != 0) {

      lv_label_set_text(label_print, main_menu.print);
      lv_obj_align(label_print, buttonPrint, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_set, main_menu.set);
      lv_obj_align(label_set, buttonSet, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      //lv_label_set_style(label_tool,LV_BTN_STATE_PR,&tft_style_lable_pre);
      //lv_label_set_style(label_tool,LV_BTN_STATE_REL,&tft_style_lable_rel);
      lv_label_set_text(label_tool, main_menu.tool);
      lv_obj_align(label_tool, buttonTool, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  #endif // !MKS_TEST
}

void lv_clear_ready_print() { lv_obj_del(scr); }

#endif // TFT_LVGL_UI
