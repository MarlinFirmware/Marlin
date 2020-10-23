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
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../gcode/queue.h"
#include "../../../../module/temperature.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_T_PRE_HEAT   1
#define ID_T_EXTRUCT    2
#define ID_T_MOV        3
#define ID_T_HOME       4
#define ID_T_LEVELING   5
#define ID_T_FILAMENT   6
#define ID_T_MORE       7
#define ID_T_RETURN     8

#if ENABLED(MKS_TEST)
  extern uint8_t curent_disp_ui;
#endif

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_T_PRE_HEAT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_preHeat();
      }
      break;
    case ID_T_EXTRUCT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_extrusion();
      }
      break;
    case ID_T_MOV:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_move_motor();
      }
      break;
    case ID_T_HOME:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_tool();
        lv_draw_home();
      }
      break;
    case ID_T_LEVELING:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          //queue.enqueue_one_P(PSTR("G28"));
          //queue.enqueue_one_P(PSTR("G29"));
          get_gcode_command(AUTO_LEVELING_COMMAND_ADDR,(uint8_t *)public_buf_m);
          public_buf_m[sizeof(public_buf_m)-1] = 0;
          queue.inject_P(PSTR(public_buf_m));
        #else
          uiCfg.leveling_first_time = 1;
          lv_clear_tool();
          lv_draw_manualLevel();
        #endif
      }
      break;
    case ID_T_FILAMENT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.desireSprayerTempBak = thermalManager.temp_hotend[uiCfg.curSprayerChoose].target;
        lv_clear_tool();
        lv_draw_filament_change();
      }
      break;
    case ID_T_MORE: break;
    case ID_T_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        TERN_(MKS_TEST, curent_disp_ui = 1);
        lv_clear_tool();
        lv_draw_ready_print();
      }
      break;
  }
}

void lv_draw_tool(void) {
  lv_obj_t *buttonPreHeat, *buttonExtrusion, *buttonMove, *buttonHome, *buttonLevel;
  lv_obj_t *buttonFilament;
  lv_obj_t *buttonBack;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != TOOL_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = TOOL_UI;
  }
  disp_state = TOOL_UI;

  scr = lv_obj_create(NULL, NULL);

  //static lv_style_t tool_style;

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create image buttons
  buttonPreHeat   = lv_imgbtn_create(scr, "F:/bmp_preHeat.bin", INTERVAL_V, titleHeight, event_handler, ID_T_PRE_HEAT);
  buttonExtrusion = lv_imgbtn_create(scr, "F:/bmp_extruct.bin", BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_T_EXTRUCT);
  buttonMove      = lv_imgbtn_create(scr, "F:/bmp_mov.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_T_MOV);
  buttonHome      = lv_imgbtn_create(scr, "F:/bmp_zero.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_T_HOME);
  buttonLevel     = lv_imgbtn_create(scr, "F:/bmp_leveling.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_T_LEVELING);
  buttonFilament  = lv_imgbtn_create(scr, "F:/bmp_filamentchange.bin",BTN_X_PIXEL+INTERVAL_V*2,BTN_Y_PIXEL+INTERVAL_H+titleHeight, event_handler,ID_T_FILAMENT);
  //buttonMore    = lv_imgbtn_createx(scr, NULL);
  //lv_obj_set_pos(buttonMore,BTN_X_PIXEL*2+INTERVAL_V*3, BTN_Y_PIXEL+INTERVAL_H+titleHeight);
  buttonBack      = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_T_RETURN);

  // Create labels on the image buttons
  lv_obj_t *labelPreHeat   = lv_label_create_empty(buttonPreHeat);
  lv_obj_t *labelExtrusion = lv_label_create_empty(buttonExtrusion);
  lv_obj_t *label_Move     = lv_label_create_empty(buttonMove);
  lv_obj_t *label_Home     = lv_label_create_empty(buttonHome);
  lv_obj_t *label_Level    = lv_label_create_empty(buttonLevel);
  lv_obj_t *label_Filament = lv_label_create_empty(buttonFilament);
  //lv_obj_t *label_More   = lv_label_create_empty(buttonMore);
  lv_obj_t *label_Back     = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    lv_label_set_text(labelPreHeat, tool_menu.preheat);
    lv_obj_align(labelPreHeat, buttonPreHeat, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelExtrusion, tool_menu.extrude);
    lv_obj_align(labelExtrusion, buttonExtrusion, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Move, tool_menu.move);
    lv_obj_align(label_Move, buttonMove, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Home, tool_menu.home);
    lv_obj_align(label_Home, buttonHome, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Level, tool_menu.TERN(AUTO_BED_LEVELING_BILINEAR, autoleveling, leveling));
    lv_obj_align(label_Level, buttonLevel, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Filament, tool_menu.filament);
    lv_obj_align(label_Filament, buttonFilament, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    //lv_label_set_text(label_More, tool_menu.more);
    //lv_obj_align(label_More, buttonMore, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonPreHeat);
      lv_group_add_obj(g, buttonExtrusion);
      lv_group_add_obj(g, buttonMove);
      lv_group_add_obj(g, buttonHome);
      lv_group_add_obj(g, buttonLevel);
      lv_group_add_obj(g, buttonFilament);
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

void lv_clear_tool() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
