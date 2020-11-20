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

enum {
  ID_CN = 1,
  ID_T_CN,
  ID_EN,
  ID_RU,
  ID_ES,
  ID_FR,
  ID_IT,
  ID_L_RETURN
};

#define SELECTED    1
#define UNSELECTED  0

static void disp_language(uint8_t language, uint8_t state);

extern lv_group_t *g;
static lv_obj_t *scr;
static lv_obj_t *buttonCN, *buttonT_CN, *buttonEN, *buttonRU;
static lv_obj_t *buttonES, *buttonFR, *buttonIT;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_CN:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonCN, "F:/bmp_simplified_cn_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonCN);
      gCfgItems.language = LANG_SIMPLE_CHINESE;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_T_CN:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonT_CN, "F:/bmp_traditional_cn_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonT_CN);
      gCfgItems.language = LANG_COMPLEX_CHINESE;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_EN:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonEN, "F:/bmp_english_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonEN);
      gCfgItems.language = LANG_ENGLISH;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_RU:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonRU, "F:/bmp_russian_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonRU);
      gCfgItems.language = LANG_RUSSIAN;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_ES:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonES, "F:/bmp_spanish_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonES);
      gCfgItems.language = LANG_SPANISH;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_FR:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonFR, "F:/bmp_french_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonFR);
      gCfgItems.language = LANG_FRENCH;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_IT:
      disp_language(gCfgItems.language, UNSELECTED);
      lv_imgbtn_set_src_both(buttonIT, "F:/bmp_italy_sel.bin");
      lv_obj_refresh_ext_draw_pad(buttonIT);
      gCfgItems.language = LANG_ITALY;
      update_spi_flash();
      disp_language_init();
      break;
    case ID_L_RETURN:
      buttonCN   = nullptr;
      buttonT_CN = nullptr;
      buttonEN   = nullptr;
      buttonRU   = nullptr;
      buttonES   = nullptr;
      buttonFR   = nullptr;
      buttonFR   = nullptr;
      buttonIT   = nullptr;
      lv_clear_language();
      lv_draw_set();
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

  lv_obj_set_event_cb_mks(obj, event_handler, id, "", 0);
  lv_imgbtn_set_src_both(obj, public_buf_l);

  if (state == UNSELECTED) lv_obj_refresh_ext_draw_pad(obj);
}

void lv_draw_language(void) {
  scr = lv_screen_create(LANGUAGE_UI);
  // Create image buttons
  buttonCN = lv_big_button_create(scr, "F:/bmp_simplified_cn.bin", language_menu.chinese_s, INTERVAL_V, titleHeight, event_handler, ID_CN);
  lv_obj_clear_protect(buttonCN, LV_PROTECT_FOLLOW);
  buttonT_CN = lv_big_button_create(scr, "F:/bmp_traditional_cn.bin", language_menu.chinese_t, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_T_CN);
  buttonEN = lv_big_button_create(scr, "F:/bmp_english.bin", language_menu.english, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_EN);
  buttonRU = lv_big_button_create(scr, "F:/bmp_russian.bin", language_menu.russian, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_RU);
  buttonES = lv_big_button_create(scr, "F:/bmp_spanish.bin", language_menu.spanish, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_ES);
  buttonFR = lv_big_button_create(scr, "F:/bmp_french.bin", language_menu.french, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_FR);
  buttonIT = lv_big_button_create(scr, "F:/bmp_italy.bin", language_menu.italy, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_IT);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_L_RETURN);
  disp_language(gCfgItems.language, SELECTED);
}

void lv_clear_language() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
