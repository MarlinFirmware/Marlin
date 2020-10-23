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

#include "../../../../module/planner.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_MANUAL_POS_RETURN   1
#define ID_MANUAL_POS_X1       2
#define ID_MANUAL_POS_Y1       3
#define ID_MANUAL_POS_X2       4
#define ID_MANUAL_POS_Y2       5
#define ID_MANUAL_POS_X3       6
#define ID_MANUAL_POS_Y3       7
#define ID_MANUAL_POS_X4       8
#define ID_MANUAL_POS_Y4       9
#define ID_MANUAL_POS_X5       10
#define ID_MANUAL_POS_Y5       11
#define ID_MANUAL_POS_DOWN     12
#define ID_MANUAL_POS_UP       13

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_MANUAL_POS_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_manual_level_pos_settings();
        draw_return_ui();
      }
      break;
    case ID_MANUAL_POS_X1:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x1;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y1:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y1;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X2:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x2;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y2:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y2;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X3:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x3;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y3:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y3;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X4:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x4;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y4:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y4;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X5:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y5;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y5:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y5;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_UP:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_manual_level_pos_settings();
        lv_draw_manual_level_pos_settings();
      }
      break;
    case ID_MANUAL_POS_DOWN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 1;
        lv_clear_manual_level_pos_settings();
        lv_draw_manual_level_pos_settings();
      }
      break;
  }
}

void lv_draw_manual_level_pos_settings(void) {
  char buf2[50];
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != MANUAL_LEVELING_POSIGION_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = MANUAL_LEVELING_POSIGION_UI;
  }
  disp_state = MANUAL_LEVELING_POSIGION_UI;

  scr = lv_obj_create(NULL, NULL);
  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);
  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.LevelingParaConfTitle);
  lv_refr_now(lv_refr_get_disp_refreshing());

  if (uiCfg.para_ui_page != 1) {
    sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[0][0]);
    sprintf_P(buf2, PSTR("%d"), gCfgItems.levelingPos[0][1]);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position1, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_MANUAL_POS_Y1, 0, buf2, ID_MANUAL_POS_X1, public_buf_l);

    sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[1][0]);
    sprintf_P(buf2, PSTR("%d"), gCfgItems.levelingPos[1][1]);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position2, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_MANUAL_POS_Y2, 1, buf2, ID_MANUAL_POS_X2, public_buf_l);

    sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[2][0]);
    sprintf_P(buf2, PSTR("%d"), gCfgItems.levelingPos[2][1]);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position3, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_MANUAL_POS_Y3, 2, buf2, ID_MANUAL_POS_X3, public_buf_l);

    sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[3][0]);
    sprintf_P(buf2, PSTR("%d"), gCfgItems.levelingPos[3][1]);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position4, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_MANUAL_POS_Y4, 3, buf2, ID_MANUAL_POS_X4, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_MANUAL_POS_DOWN, true);
  }
  else {
    sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[4][0]);
    sprintf_P(buf2, PSTR("%d"), gCfgItems.levelingPos[4][1]);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position4, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_MANUAL_POS_Y5, 0, buf2, ID_MANUAL_POS_X5, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_MANUAL_POS_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X + 10, PARA_UI_BACL_POS_Y, event_handler, ID_MANUAL_POS_RETURN, true);
}

void lv_clear_manual_level_pos_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
