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
#include <string.h>

//static lv_obj_t *buttonMoveZ,*buttonTest,*buttonZ0,*buttonStop,*buttonReturn;

#define ID_CN       1
#define ID_T_CN     2
#define ID_EN       3
#define ID_RU       4
#define ID_ES       5
#define ID_FR       6
#define ID_IT       7
#define ID_L_RETURN 8

#define SELECTED    1
#define UNSELECTED  0

static void disp_language(uint8_t language, uint8_t state);

static lv_obj_t * scr;
static lv_obj_t *buttonCN, *buttonT_CN, *buttonEN, *buttonRU;
static lv_obj_t *buttonES, *buttonFR, *buttonIT, *buttonBack;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_CN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_obj_set_event_cb_mks(buttonCN, event_handler, ID_CN, "bmp_simplified_cn_sel.bin", 0);
        gCfgItems.language = LANG_SIMPLE_CHINESE;
        gCfg_to_spiFlah();
        disp_language_init();
      }
      break;
    case ID_T_CN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_obj_set_event_cb_mks(buttonT_CN, event_handler, ID_T_CN, "bmp_traditional_cn_sel.bin", 0);
        gCfgItems.language = LANG_COMPLEX_CHINESE;
        gCfg_to_spiFlah();
        disp_language_init();
      }
      break;
    case ID_EN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_obj_set_event_cb_mks(buttonEN, event_handler, ID_EN, "bmp_english_sel.bin", 0);
        gCfgItems.language = LANG_ENGLISH;
        gCfg_to_spiFlah();
        disp_language_init();
      }
      break;
    case ID_RU:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_obj_set_event_cb_mks(buttonRU, event_handler, ID_RU, "bmp_russian_sel.bin", 0);
        gCfgItems.language = LANG_RUSSIAN;
        gCfg_to_spiFlah();
        disp_language_init();
      }
      break;
    case ID_ES:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_obj_set_event_cb_mks(buttonES, event_handler, ID_ES, "bmp_spanish_sel.bin", 0);
        gCfgItems.language = LANG_SPANISH;
        gCfg_to_spiFlah();
        disp_language_init();
      }
      break;
    case ID_FR:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_obj_set_event_cb_mks(buttonFR, event_handler, ID_FR, "bmp_french_sel.bin", 0);
        gCfgItems.language = LANG_FRENCH;
        gCfg_to_spiFlah();
        disp_language_init();
      }
      break;
    case ID_IT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_obj_set_event_cb_mks(buttonIT, event_handler, ID_FR, "bmp_italy_sel.bin", 0);
        gCfgItems.language = LANG_ITALY;
        gCfg_to_spiFlah();
        disp_language_init();
      }
      break;
    case ID_L_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {

        buttonCN   = NULL;
        buttonT_CN = NULL;
        buttonEN   = NULL;
        buttonRU   = NULL;
        buttonES   = NULL;
        buttonFR   = NULL;
        buttonFR   = NULL;
        buttonIT   = NULL;
        buttonBack = NULL;

        lv_obj_del(scr);
        lv_draw_set();
      }
      break;

  }
}

static void disp_language(uint8_t language, uint8_t state) {
  uint16_t id;
  lv_obj_t *obj;

  public_buf_l[0] = '\0';

  switch (language) {
    case LANG_SIMPLE_CHINESE:
      id = ID_CN;
      strcpy_P(public_buf_l, PSTR("bmp_simplified_cn"));
      obj = buttonCN;
      break;
    case LANG_COMPLEX_CHINESE:
      id = ID_T_CN;
      strcpy_P(public_buf_l, PSTR("bmp_traditional_cn"));
      obj = buttonT_CN;
      break;
    case LANG_ENGLISH:
      id = ID_EN;
      strcpy_P(public_buf_l, PSTR("bmp_english"));
      obj = buttonEN;
      break;
    case LANG_RUSSIAN:
      id = ID_RU;
      strcpy_P(public_buf_l, PSTR("bmp_russian"));
      obj = buttonRU;
      break;
    case LANG_SPANISH:
      id = ID_ES;
      strcpy_P(public_buf_l, PSTR("bmp_spanish"));
      obj = buttonES;
      break;
    case LANG_FRENCH:
      id = ID_FR;
      strcpy_P(public_buf_l, PSTR("bmp_french"));
      obj = buttonFR;
      break;
    case LANG_ITALY:
      id = ID_IT;
      strcpy_P(public_buf_l, PSTR("bmp_italy"));
      obj = buttonIT;
      break;
    default:
      id = ID_CN;
      strcpy_P(public_buf_l, PSTR("bmp_simplified_cn"));
      obj = buttonCN;
      break;
  }

  if (state == SELECTED) strcat_P(public_buf_l, PSTR("_sel"));

  strcat_P(public_buf_l, PSTR(".bin"));

  lv_obj_set_event_cb_mks(obj, event_handler, id, public_buf_l, 0);

  if (state == UNSELECTED) lv_obj_refresh_ext_draw_pad(obj);
}

void lv_draw_language(void) {

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != LANGUAGE_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = LANGUAGE_UI;
  }
  disp_state = LANGUAGE_UI;

  scr = lv_obj_create(NULL, NULL);

  // static lv_style_t tool_style;

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_pic);

  /*Create an Image button*/
  buttonCN   = lv_imgbtn_create(scr, NULL);
  buttonT_CN = lv_imgbtn_create(scr, NULL);
  buttonEN   = lv_imgbtn_create(scr, NULL);
  buttonRU   = lv_imgbtn_create(scr, NULL);
  buttonES   = lv_imgbtn_create(scr, NULL);
  buttonFR   = lv_imgbtn_create(scr, NULL);
  buttonIT   = lv_imgbtn_create(scr, NULL);
  buttonBack = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonCN, event_handler, ID_CN, "bmp_simplified_cn.bin", 0);
  lv_imgbtn_set_src(buttonCN, LV_BTN_STATE_REL, &bmp_pic);
  lv_imgbtn_set_src(buttonCN, LV_BTN_STATE_PR, &bmp_pic);
  lv_imgbtn_set_style(buttonCN, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonCN, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonCN, LV_PROTECT_FOLLOW);
  #if 1
    lv_obj_set_event_cb_mks(buttonT_CN, event_handler, ID_T_CN, "bmp_traditional_cn.bin", 0);
    lv_imgbtn_set_src(buttonT_CN, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonT_CN, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonT_CN, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonT_CN, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonEN, event_handler, ID_EN, "bmp_english.bin", 0);
    lv_imgbtn_set_src(buttonEN, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonEN, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonEN, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonEN, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonRU, event_handler, ID_RU, "bmp_russian.bin", 0);
    lv_imgbtn_set_src(buttonRU, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonRU, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonRU, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonRU, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonES, event_handler, ID_ES, "bmp_spanish.bin", 0);
    lv_imgbtn_set_src(buttonES, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonES, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonES, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonES, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonFR, event_handler, ID_FR, "bmp_french.bin", 0);
    lv_imgbtn_set_src(buttonFR, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonFR, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonFR, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonFR, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonIT, event_handler, ID_IT, "bmp_italy.bin", 0);
    lv_imgbtn_set_src(buttonIT, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonIT, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonIT, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonIT, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_L_RETURN, "bmp_return.bin", 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  #endif // if 1

  lv_obj_set_pos(buttonCN, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonT_CN, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
  lv_obj_set_pos(buttonEN, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
  lv_obj_set_pos(buttonRU, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
  lv_obj_set_pos(buttonES, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonFR, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonIT, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  /*Create a label on the Image button*/
  lv_btn_set_layout(buttonCN, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonT_CN, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonEN, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonRU, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonES, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonFR, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonIT, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  lv_obj_t * label_CN   = lv_label_create(buttonCN, NULL);
  lv_obj_t * label_T_CN = lv_label_create(buttonT_CN, NULL);
  lv_obj_t * label_EN   = lv_label_create(buttonEN, NULL);
  lv_obj_t * label_RU   = lv_label_create(buttonRU, NULL);
  lv_obj_t * label_ES   = lv_label_create(buttonES, NULL);
  lv_obj_t * label_FR   = lv_label_create(buttonFR, NULL);
  lv_obj_t * label_IT   = lv_label_create(buttonIT, NULL);
  lv_obj_t * label_Back = lv_label_create(buttonBack, NULL);

  disp_language(gCfgItems.language, SELECTED);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(label_CN, language_menu.chinese_s);
    lv_obj_align(label_CN, buttonCN, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_T_CN, language_menu.chinese_t);
    lv_obj_align(label_T_CN, buttonT_CN, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_EN, language_menu.english);
    lv_obj_align(label_EN, buttonEN, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_RU, language_menu.russian);
    lv_obj_align(label_RU, buttonRU, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_ES, language_menu.spanish);
    lv_obj_align(label_ES, buttonES, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_FR, language_menu.french);
    lv_obj_align(label_FR, buttonFR, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_IT, language_menu.italy);
    lv_obj_align(label_IT, buttonIT, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
}

void lv_clear_language() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
