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

#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../../module/settings.h"
#endif

#if HAS_BED_PROBE
  #include "../../../../module/probe.h"
#endif

extern lv_group_t * g;
static lv_obj_t * scr;

static lv_obj_t *labelV, *buttonV, * zOffsetText;

#define ID_BABY_STEP_X_P    1
#define ID_BABY_STEP_X_N    2
#define ID_BABY_STEP_Y_P    3
#define ID_BABY_STEP_Y_N    4
#define ID_BABY_STEP_Z_P    5
#define ID_BABY_STEP_Z_N    6
#define ID_BABY_STEP_DIST   7
#define ID_BABY_STEP_RETURN 8

static float babystep_dist=0.01;
static uint8_t has_adjust_z = 0;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  char baby_buf[30] = { 0 };
  switch (obj->mks_obj_id) {
    case ID_BABY_STEP_X_P:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        sprintf_P(baby_buf, PSTR("M290 X%.3f"),babystep_dist);
        gcode.process_subcommands_now_P(PSTR(baby_buf));
        has_adjust_z = 1;
      }
      break;
    case ID_BABY_STEP_X_N:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        sprintf_P(baby_buf, PSTR("M290 X%.3f"),((float)0 - babystep_dist));
        gcode.process_subcommands_now_P(PSTR(baby_buf));
        has_adjust_z = 1;
      }
      break;
    case ID_BABY_STEP_Y_P:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        sprintf_P(baby_buf, PSTR("M290 Y%.3f"), babystep_dist);
        gcode.process_subcommands_now_P(PSTR(baby_buf));
        has_adjust_z = 1;
      }
      break;
    case ID_BABY_STEP_Y_N:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        sprintf_P(baby_buf, PSTR("M290 Y%.3f"),((float)0 - babystep_dist));
        gcode.process_subcommands_now_P(PSTR(baby_buf));
        has_adjust_z = 1;
      }
      break;
    case ID_BABY_STEP_Z_P:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        sprintf_P(baby_buf, PSTR("M290 Z%.3f"), babystep_dist);
        gcode.process_subcommands_now_P(PSTR(baby_buf));
        has_adjust_z = 1;
      }
      break;
    case ID_BABY_STEP_Z_N:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        sprintf_P(baby_buf, PSTR("M290 Z%.3f"),((float)0 - babystep_dist));
        gcode.process_subcommands_now_P(PSTR(baby_buf));
        has_adjust_z = 1;
      }
      break;
    case ID_BABY_STEP_DIST:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (abs((int)(100 * babystep_dist)) == 1)
          babystep_dist = 0.05;
        else if (abs((int)(100 * babystep_dist)) == 5)
          babystep_dist = 0.1;
        else
          babystep_dist = 0.01;
        disp_baby_step_dist();
      }

      break;
    case ID_BABY_STEP_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (has_adjust_z == 1) {
          TERN_(EEPROM_SETTINGS, (void)settings.save());
          has_adjust_z = 0;
        }
        clear_cur_ui();
        draw_return_ui();
      }
      break;
  }
}

void lv_draw_baby_stepping(void) {
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != BABY_STEP_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = BABY_STEP_UI;
  }
  disp_state = BABY_STEP_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create an Image button
  lv_obj_t *buttonXI = lv_imgbtn_create(scr, "F:/bmp_xAdd.bin", INTERVAL_V, titleHeight, event_handler, ID_BABY_STEP_X_P);
  lv_obj_t *buttonXD = lv_imgbtn_create(scr, "F:/bmp_xDec.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_X_N);
  lv_obj_t *buttonYI = lv_imgbtn_create(scr, "F:/bmp_yAdd.bin", BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_BABY_STEP_Y_P);
  lv_obj_t *buttonYD = lv_imgbtn_create(scr, "F:/bmp_yDec.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_Y_N);
  lv_obj_t *buttonZI = lv_imgbtn_create(scr, "F:/bmp_zAdd.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_BABY_STEP_Z_P);
  lv_obj_t *buttonZD = lv_imgbtn_create(scr, "F:/bmp_zDec.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_Z_N);
             buttonV = lv_imgbtn_create(scr, NULL, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_BABY_STEP_DIST);
  lv_obj_t *buttonBack = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_RETURN);

  // Create labels on the image buttons
  lv_obj_t *labelXI = lv_label_create_empty(buttonXI);
  lv_obj_t *labelXD = lv_label_create_empty(buttonXD);
  lv_obj_t *labelYI = lv_label_create_empty(buttonYI);
  lv_obj_t *labelYD = lv_label_create_empty(buttonYD);
  lv_obj_t *labelZI = lv_label_create_empty(buttonZI);
  lv_obj_t *labelZD = lv_label_create_empty(buttonZD);
  labelV = lv_label_create_empty(buttonV);
  lv_obj_t *label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    lv_label_set_text(labelXI, move_menu.x_add);
    lv_obj_align(labelXI, buttonXI, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelXD, move_menu.x_dec);
    lv_obj_align(labelXD, buttonXD, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelYI, move_menu.y_add);
    lv_obj_align(labelYI, buttonYI, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelYD, move_menu.y_dec);
    lv_obj_align(labelYD, buttonYD, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelZI, move_menu.z_add);
    lv_obj_align(labelZI, buttonZI, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelZD, move_menu.z_dec);
    lv_obj_align(labelZD, buttonZD, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonXI);
      lv_group_add_obj(g, buttonXD);
      lv_group_add_obj(g, buttonYI);
      lv_group_add_obj(g, buttonYD);
      lv_group_add_obj(g, buttonZI);
      lv_group_add_obj(g, buttonZD);
      lv_group_add_obj(g, buttonV);
      lv_group_add_obj(g, buttonBack);
    }
  #endif

  disp_baby_step_dist();

  zOffsetText = lv_label_create(scr, 290, TITLE_YPOS, NULL);
  disp_z_offset_value();
}

void disp_baby_step_dist() {
  // char buf[30] = {0};
  if ((int)(100 * babystep_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_01.bin");
  else if ((int)(100 * babystep_dist) == 5)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_05.bin");
  else if ((int)(100 * babystep_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_1.bin");

  if (gCfgItems.multiple_language) {
    if ((int)(100 * babystep_dist) == 1) {
      lv_label_set_text(labelV, move_menu.step_001mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(100 * babystep_dist) == 5) {
      lv_label_set_text(labelV, move_menu.step_005mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(100 * babystep_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_z_offset_value() {
  char buf[20];
  sprintf_P(buf, PSTR("offset Z: %.3f"), (double)TERN(HAS_BED_PROBE, probe.offset.z, 0));
  lv_label_set_text(zOffsetText, buf);
}

void lv_clear_baby_stepping() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
