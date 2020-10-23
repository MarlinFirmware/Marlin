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

#include "../../../../inc/MarlinConfig.h"
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

extern lv_group_t * g;
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
        lv_imgbtn_set_src_both(buttonCN, "F:/bmp_simplified_cn_sel.bin");
        lv_obj_refresh_ext_draw_pad(buttonCN);
        gCfgItems.language = LANG_SIMPLE_CHINESE;
        update_spi_flash();
        disp_language_init();
      }
      break;
    case ID_T_CN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_imgbtn_set_src_both(buttonT_CN, "F:/bmp_traditional_cn_sel.bin");
        lv_obj_refresh_ext_draw_pad(buttonT_CN);
        gCfgItems.language = LANG_COMPLEX_CHINESE;
        update_spi_flash();
        disp_language_init();
      }
      break;
    case ID_EN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_imgbtn_set_src_both(buttonEN, "F:/bmp_english_sel.bin");
        lv_obj_refresh_ext_draw_pad(buttonEN);
        gCfgItems.language = LANG_ENGLISH;
        update_spi_flash();
        disp_language_init();
      }
      break;
    case ID_RU:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_imgbtn_set_src_both(buttonRU, "F:/bmp_russian_sel.bin");
        lv_obj_refresh_ext_draw_pad(buttonRU);
        gCfgItems.language = LANG_RUSSIAN;
        update_spi_flash();
        disp_language_init();
      }
      break;
    case ID_ES:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_imgbtn_set_src_both(buttonES, "F:/bmp_spanish_sel.bin");
        lv_obj_refresh_ext_draw_pad(buttonES);
        gCfgItems.language = LANG_SPANISH;
        update_spi_flash();
        disp_language_init();
      }
      break;
    case ID_FR:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_imgbtn_set_src_both(buttonFR, "F:/bmp_french_sel.bin");
        lv_obj_refresh_ext_draw_pad(buttonFR);
        gCfgItems.language = LANG_FRENCH;
        update_spi_flash();
        disp_language_init();
      }
      break;
    case ID_IT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        disp_language(gCfgItems.language, UNSELECTED);
        lv_imgbtn_set_src_both(buttonIT, "F:/bmp_italy_sel.bin");
        lv_obj_refresh_ext_draw_pad(buttonIT);
        gCfgItems.language = LANG_ITALY;
        update_spi_flash();
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
        lv_clear_language();
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
      strcpy_P(public_buf_l, PSTR("F:/bmp_simplified_cn"));
      obj = buttonCN;
      break;
    case LANG_COMPLEX_CHINESE:
      id = ID_T_CN;
      strcpy_P(public_buf_l, PSTR("F:/bmp_traditional_cn"));
      obj = buttonT_CN;
      break;
    case LANG_ENGLISH:
      id = ID_EN;
      strcpy_P(public_buf_l, PSTR("F:/bmp_english"));
      obj = buttonEN;
      break;
    case LANG_RUSSIAN:
      id = ID_RU;
      strcpy_P(public_buf_l, PSTR("F:/bmp_russian"));
      obj = buttonRU;
      break;
    case LANG_SPANISH:
      id = ID_ES;
      strcpy_P(public_buf_l, PSTR("F:/bmp_spanish"));
      obj = buttonES;
      break;
    case LANG_FRENCH:
      id = ID_FR;
      strcpy_P(public_buf_l, PSTR("F:/bmp_french"));
      obj = buttonFR;
      break;
    case LANG_ITALY:
      id = ID_IT;
      strcpy_P(public_buf_l, PSTR("F:/bmp_italy"));
      obj = buttonIT;
      break;
    default:
      id = ID_CN;
      strcpy_P(public_buf_l, PSTR("F:/bmp_simplified_cn"));
      obj = buttonCN;
      break;
  }

  if (state == SELECTED) strcat_P(public_buf_l, PSTR("_sel"));

  strcat_P(public_buf_l, PSTR(".bin"));

  lv_obj_set_event_cb_mks(obj, event_handler, id, NULL, 0);
  lv_imgbtn_set_src_both(obj, public_buf_l);

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

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create image buttons
  buttonCN = lv_imgbtn_create(scr, "F:/bmp_simplified_cn.bin", INTERVAL_V, titleHeight, event_handler, ID_CN);
  lv_obj_clear_protect(buttonCN, LV_PROTECT_FOLLOW);
  buttonT_CN = lv_imgbtn_create(scr, "F:/bmp_traditional_cn.bin", BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_T_CN);
  buttonEN = lv_imgbtn_create(scr, "F:/bmp_english.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_EN);
  buttonRU = lv_imgbtn_create(scr, "F:/bmp_russian.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_RU);
  buttonES = lv_imgbtn_create(scr, "F:/bmp_spanish.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_ES);
  buttonFR = lv_imgbtn_create(scr, "F:/bmp_french.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FR);
  buttonIT = lv_imgbtn_create(scr, "F:/bmp_italy.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_IT);
  buttonBack = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_L_RETURN);

  // Create labels on the image buttons
  lv_obj_t *label_CN   = lv_label_create_empty(buttonCN);
  lv_obj_t *label_T_CN = lv_label_create_empty(buttonT_CN);
  lv_obj_t *label_EN   = lv_label_create_empty(buttonEN);
  lv_obj_t *label_RU   = lv_label_create_empty(buttonRU);
  lv_obj_t *label_ES   = lv_label_create_empty(buttonES);
  lv_obj_t *label_FR   = lv_label_create_empty(buttonFR);
  lv_obj_t *label_IT   = lv_label_create_empty(buttonIT);
  lv_obj_t *label_Back = lv_label_create_empty(buttonBack);

  disp_language(gCfgItems.language, SELECTED);

  if (gCfgItems.multiple_language) {
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
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonCN);
      lv_group_add_obj(g, buttonT_CN);
      lv_group_add_obj(g, buttonEN);
      lv_group_add_obj(g, buttonRU);
      lv_group_add_obj(g, buttonES);
      lv_group_add_obj(g, buttonFR);
      lv_group_add_obj(g, buttonIT);
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

void lv_clear_language() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
