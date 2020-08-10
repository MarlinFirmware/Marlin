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

#if ENABLED(TFT_LVGL_UI_SPI)
  #include "SPI_TFT.h"
#endif

#include "tft_lvgl_configuration.h"

#include "pic_manager.h"

#include "draw_ui.h"
#include "mks_hardware_test.h"

#include <SPI.h>

#include "../../../../MarlinCore.h"
#include "../../../../sd/cardreader.h"
#include "../../../../module/motion.h"
#include "../../../../module/planner.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../../../feature/pause.h"
#endif

W25QXXFlash W25QXX;
CFG_ITMES gCfgItems;
UI_CFG uiCfg;
DISP_STATE_STACK disp_state_stack;
DISP_STATE disp_state = MAIN_UI;
DISP_STATE last_disp_state;
PRINT_TIME print_time;
value_state value;

uint32_t To_pre_view;
uint8_t gcode_preview_over;
uint8_t flash_preview_begin;
uint8_t default_preview_flg;
uint32_t size = 809;
uint16_t row;
uint8_t temperature_change_frequency;
uint8_t printing_rate_update_flag;

extern uint8_t once_flag;
extern uint8_t sel_id;
extern uint8_t public_buf[512];
extern uint8_t bmp_public_buf[17 * 1024];

extern void LCD_IO_WriteData(uint16_t RegValue);

lv_point_t line_points[4][2] = {
  {{PARA_UI_POS_X, PARA_UI_POS_Y + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y + PARA_UI_SIZE_Y}},
  {{PARA_UI_POS_X, PARA_UI_POS_Y*2 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y*2 + PARA_UI_SIZE_Y}},
  {{PARA_UI_POS_X, PARA_UI_POS_Y*3 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y*3 + PARA_UI_SIZE_Y}},
  {{PARA_UI_POS_X, PARA_UI_POS_Y*4 + PARA_UI_SIZE_Y}, {TFT_WIDTH, PARA_UI_POS_Y*4 + PARA_UI_SIZE_Y}}
};
void gCfgItems_init() {
  gCfgItems.multiple_language = MULTI_LANGUAGE_ENABLE;
  #if 1 // LCD_LANGUAGE == en
    gCfgItems.language = LANG_ENGLISH;
  #elif LCD_LANGUAGE == zh_CN
    gCfgItems.language = LANG_SIMPLE_CHINESE;
  #elif LCD_LANGUAGE == zh_TW
    gCfgItems.language = LANG_COMPLEX_CHINESE;
  #elif LCD_LANGUAGE == jp_kana
    gCfgItems.language = LANG_JAPAN;
  #elif LCD_LANGUAGE == de
    gCfgItems.language = LANG_GERMAN;
  #elif LCD_LANGUAGE == fr
    gCfgItems.language = LANG_FRENCH;
  #elif LCD_LANGUAGE == ru
    gCfgItems.language = LANG_RUSSIAN;
  #elif LCD_LANGUAGE == ko_KR
    gCfgItems.language = LANG_KOREAN;
  #elif LCD_LANGUAGE == tr
    gCfgItems.language = LANG_TURKISH;
  #elif LCD_LANGUAGE == es
    gCfgItems.language = LANG_SPANISH;
  #elif LCD_LANGUAGE == el
    gCfgItems.language = LANG_GREEK;
  #elif LCD_LANGUAGE == it
    gCfgItems.language = LANG_ITALY;
  #elif LCD_LANGUAGE == pt
    gCfgItems.language = LANG_PORTUGUESE;
  #endif
  gCfgItems.leveling_mode    = 0;
  gCfgItems.from_flash_pic   = 0;
  gCfgItems.curFilesize      = 0;
  gCfgItems.finish_power_off = 0;
  gCfgItems.pause_reprint    = 0;
  gCfgItems.pausePosX        = -1;
  gCfgItems.pausePosY        = -1;
  gCfgItems.pausePosZ        = 5;
  W25QXX.SPI_FLASH_BufferRead((uint8_t *)&gCfgItems.spi_flash_flag, VAR_INF_ADDR, sizeof(gCfgItems.spi_flash_flag));
  if (gCfgItems.spi_flash_flag == GCFG_FLAG_VALUE) {
    W25QXX.SPI_FLASH_BufferRead((uint8_t *)&gCfgItems, VAR_INF_ADDR, sizeof(gCfgItems));
  }
  else {
    gCfgItems.spi_flash_flag = GCFG_FLAG_VALUE;
    W25QXX.SPI_FLASH_SectorErase(VAR_INF_ADDR);
    W25QXX.SPI_FLASH_BufferWrite((uint8_t *)&gCfgItems, VAR_INF_ADDR, sizeof(gCfgItems));
  }

  uiCfg.F[0] = 'N';
  uiCfg.F[1] = 'A';
  uiCfg.F[2] = 'N';
  uiCfg.F[3] = 'O';
  W25QXX.SPI_FLASH_BlockErase(REFLSHE_FLGA_ADD + 32 - 64*1024);
  W25QXX.SPI_FLASH_BufferWrite(uiCfg.F,REFLSHE_FLGA_ADD,4);
}

void gCfg_to_spiFlah() {
  W25QXX.SPI_FLASH_SectorErase(VAR_INF_ADDR);
  W25QXX.SPI_FLASH_BufferWrite((uint8_t *)&gCfgItems, VAR_INF_ADDR, sizeof(gCfgItems));
}

void ui_cfg_init() {
  uiCfg.curTempType         = 0;
  uiCfg.curSprayerChoose    = 0;
  uiCfg.stepHeat            = 10;
  uiCfg.leveling_first_time = 0;
  uiCfg.para_ui_page        = 0;
  uiCfg.extruStep           = 5;
  uiCfg.extruSpeed          = 10;
  uiCfg.move_dist           = 1;
  uiCfg.moveSpeed           = 3000;
  uiCfg.stepPrintSpeed      = 10;
}

void update_spi_flash() {
  W25QXX.init(SPI_QUARTER_SPEED);
  W25QXX.SPI_FLASH_SectorErase(VAR_INF_ADDR);
  W25QXX.SPI_FLASH_BufferWrite((uint8_t *)&gCfgItems, VAR_INF_ADDR, sizeof(gCfgItems));
}

lv_style_t tft_style_scr;
lv_style_t tft_style_label_pre;
lv_style_t tft_style_label_rel;
lv_style_t style_line;
lv_style_t style_para_value_pre;
lv_style_t style_para_value_rel;

lv_style_t style_num_key_pre;
lv_style_t style_num_key_rel;

lv_style_t style_num_text;
void tft_style_init() {
  lv_style_copy(&tft_style_scr, &lv_style_scr);
  tft_style_scr.body.main_color   = LV_COLOR_BACKGROUND;
  tft_style_scr.body.grad_color   = LV_COLOR_BACKGROUND;
  tft_style_scr.text.color        = LV_COLOR_TEXT;
  tft_style_scr.text.sel_color    = LV_COLOR_TEXT;
  tft_style_scr.line.width        = 0;
  tft_style_scr.text.letter_space = 0;
  tft_style_scr.text.line_space   = 0;

  lv_style_copy(&tft_style_label_pre, &lv_style_scr);
  lv_style_copy(&tft_style_label_rel, &lv_style_scr);
  tft_style_label_pre.body.main_color = LV_COLOR_BACKGROUND;
  tft_style_label_pre.body.grad_color = LV_COLOR_BACKGROUND;
  tft_style_label_pre.text.color      = LV_COLOR_TEXT;
  tft_style_label_pre.text.sel_color  = LV_COLOR_TEXT;
  tft_style_label_rel.body.main_color = LV_COLOR_BACKGROUND;
  tft_style_label_rel.body.grad_color = LV_COLOR_BACKGROUND;
  tft_style_label_rel.text.color      = LV_COLOR_TEXT;
  tft_style_label_rel.text.sel_color  = LV_COLOR_TEXT;
  tft_style_label_pre.text.font       = TERN(HAS_SPI_FLASH_FONT, &gb2312_puhui32, LV_FONT_DEFAULT);
  tft_style_label_rel.text.font       = TERN(HAS_SPI_FLASH_FONT, &gb2312_puhui32, LV_FONT_DEFAULT);
  tft_style_label_pre.line.width        = 0;
  tft_style_label_rel.line.width        = 0;
  tft_style_label_pre.text.letter_space = 0;
  tft_style_label_rel.text.letter_space = 0;
  tft_style_label_pre.text.line_space   = -5;
  tft_style_label_rel.text.line_space   = -5;
  lv_style_copy(&style_para_value_pre, &lv_style_scr);
  lv_style_copy(&style_para_value_rel, &lv_style_scr);
  style_para_value_pre.body.main_color = LV_COLOR_BACKGROUND;
  style_para_value_pre.body.grad_color = LV_COLOR_BACKGROUND;
  style_para_value_pre.text.color      = LV_COLOR_BLACK;
  style_para_value_pre.text.sel_color  = LV_COLOR_BLACK;
  style_para_value_rel.body.main_color = LV_COLOR_BACKGROUND;
  style_para_value_rel.body.grad_color = LV_COLOR_BACKGROUND;
  style_para_value_rel.text.color      = LV_COLOR_BLACK;
  style_para_value_rel.text.sel_color  = LV_COLOR_BLACK;
  style_para_value_pre.text.font       = TERN(HAS_SPI_FLASH_FONT, &gb2312_puhui32, LV_FONT_DEFAULT);
  style_para_value_rel.text.font       = TERN(HAS_SPI_FLASH_FONT, &gb2312_puhui32, LV_FONT_DEFAULT);
  style_para_value_pre.line.width        = 0;
  style_para_value_rel.line.width        = 0;
  style_para_value_pre.text.letter_space = 0;
  style_para_value_rel.text.letter_space = 0;
  style_para_value_pre.text.line_space   = -5;
  style_para_value_rel.text.line_space   = -5;
  lv_style_copy(&style_num_key_pre, &lv_style_scr);
  lv_style_copy(&style_num_key_rel, &lv_style_scr);
  style_num_key_pre.body.main_color = LV_COLOR_KEY_BACKGROUND;
  style_num_key_pre.body.grad_color = LV_COLOR_KEY_BACKGROUND;
  style_num_key_pre.text.color      = LV_COLOR_TEXT;
  style_num_key_pre.text.sel_color  = LV_COLOR_TEXT;
  style_num_key_rel.body.main_color = LV_COLOR_KEY_BACKGROUND;
  style_num_key_rel.body.grad_color = LV_COLOR_KEY_BACKGROUND;
  style_num_key_rel.text.color      = LV_COLOR_TEXT;
  style_num_key_rel.text.sel_color  = LV_COLOR_TEXT;
  #if HAS_SPI_FLASH_FONT
    style_num_key_pre.text.font = &gb2312_puhui32;
    style_num_key_rel.text.font = &gb2312_puhui32;
  #else
    style_num_key_pre.text.font = LV_FONT_DEFAULT;
    style_num_key_rel.text.font = LV_FONT_DEFAULT;
  #endif

  style_num_key_pre.line.width        = 0;
  style_num_key_rel.line.width        = 0;
  style_num_key_pre.text.letter_space = 0;
  style_num_key_rel.text.letter_space = 0;
  style_num_key_pre.text.line_space   = -5;
  style_num_key_rel.text.line_space   = -5;
  lv_style_copy(&style_num_text, &lv_style_scr);

  style_num_text.body.main_color   = LV_COLOR_WHITE;
  style_num_text.body.grad_color   = LV_COLOR_WHITE;
  style_num_text.text.color        = LV_COLOR_BLACK;
  style_num_text.text.sel_color    = LV_COLOR_BLACK;
  style_num_text.text.font         = TERN(HAS_SPI_FLASH_FONT, &gb2312_puhui32, LV_FONT_DEFAULT);
  style_num_text.line.width        = 0;
  style_num_text.text.letter_space = 0;
  style_num_text.text.line_space   = -5;

  lv_style_copy(&style_line, &lv_style_plain);
  style_line.line.color   = LV_COLOR_MAKE(0x49, 0x54, 0xff);
  style_line.line.width   = 1;
  style_line.line.rounded = 1;
}

#define MAX_TITLE_LEN 28

char public_buf_m[100] = {0};
char public_buf_l[30];

void titleText_cat(char *str, int strSize, char *addPart) {
  if (str == 0 || addPart == 0) return;
  if ((int)(strlen(str) + strlen(addPart)) >= strSize) return;
  strcat(str, addPart);
}

char *getDispText(int index) {

  ZERO(public_buf_l);

  switch (disp_state_stack._disp_state[index]) {
    case PRINT_READY_UI:
      strcpy(public_buf_l, main_menu.title);
      break;
    case PRINT_FILE_UI:
      strcpy(public_buf_l, file_menu.title);
      break;
    case PRINTING_UI:
      if (disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI
        #ifndef TFT35
          || disp_state_stack._disp_state[disp_state_stack._disp_index] == OPERATE_UI
          || disp_state_stack._disp_state[disp_state_stack._disp_index] == PAUSE_UI
        #endif
      )    strcpy(public_buf_l, common_menu.print_special_title);
      else strcpy(public_buf_l, printing_menu.title);
      break;
    case MOVE_MOTOR_UI:
      strcpy(public_buf_l, move_menu.title);
      break;
    case OPERATE_UI:
      if (disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI
        #ifndef TFT35
          || disp_state_stack._disp_state[disp_state_stack._disp_index] == OPERATE_UI
          || disp_state_stack._disp_state[disp_state_stack._disp_index] == PAUSE_UI
        #endif
      )    strcpy(public_buf_l, common_menu.operate_special_title);
      else strcpy(public_buf_l, operation_menu.title);
      break;

    case PAUSE_UI:
      if (disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI
        || disp_state_stack._disp_state[disp_state_stack._disp_index] == OPERATE_UI
        || disp_state_stack._disp_state[disp_state_stack._disp_index] == PAUSE_UI
      )    strcpy(public_buf_l, common_menu.pause_special_title);
      else strcpy(public_buf_l, pause_menu.title);
      break;

    case EXTRUSION_UI:
      strcpy(public_buf_l, extrude_menu.title);
      break;
    case CHANGE_SPEED_UI:
      strcpy(public_buf_l, speed_menu.title);
      break;
    case FAN_UI:
      strcpy(public_buf_l, fan_menu.title);
      break;
    case PRE_HEAT_UI:
      if ((disp_state_stack._disp_state[disp_state_stack._disp_index - 1] == OPERATE_UI))
           strcpy(public_buf_l, preheat_menu.adjust_title);
      else strcpy(public_buf_l, preheat_menu.title);
      break;
    case SET_UI:
      strcpy(public_buf_l, set_menu.title);
      break;
    case ZERO_UI:
      strcpy(public_buf_l, home_menu.title);
      break;
    case SPRAYER_UI: break;
    case MACHINE_UI: break;
    case LANGUAGE_UI:
      strcpy(public_buf_l, language_menu.title);
      break;
    case ABOUT_UI:
      strcpy(public_buf_l, about_menu.title);
      break;
    case LOG_UI: break;
    case DISK_UI:
      strcpy(public_buf_l, filesys_menu.title);
      break;
    case DIALOG_UI:
      strcpy(public_buf_l, common_menu.dialog_confirm_title);
      break;
    case WIFI_UI:
      strcpy(public_buf_l, wifi_menu.title);
      break;
    case MORE_UI:
    case PRINT_MORE_UI:
      strcpy(public_buf_l, more_menu.title);
      break;
    case FILAMENTCHANGE_UI:
      strcpy(public_buf_l, filament_menu.title);
      break;
    case LEVELING_UI:
    case MESHLEVELING_UI:
      strcpy(public_buf_l, leveling_menu.title);
      break;
    case BIND_UI:
      strcpy(public_buf_l, cloud_menu.title);
      break;
    case ZOFFSET_UI:
      strcpy(public_buf_l, zoffset_menu.title);
      break;
    case TOOL_UI:
      strcpy(public_buf_l, tool_menu.title);
      break;
    case WIFI_LIST_UI:
      //strcpy(public_buf_l, list_menu.title);
      break;
    case MACHINE_PARA_UI:
      strcpy(public_buf_l, MachinePara_menu.title);
      break;
    case BABY_STEP_UI:
      strcpy(public_buf_l, operation_menu.babystep);
      break;
    case EEPROM_SETTINGS_UI:
      strcpy(public_buf_l, eeprom_menu.title);
      break;
    default: break;
  }

  return public_buf_l;
}

char *creat_title_text() {
  int index     = 0;
  char *tmpText = 0;
  char tmpCurFileStr[20];

  ZERO(tmpCurFileStr);

  #if _LFN_UNICODE
    //cutFileName((TCHAR *)curFileName, 16, 16, (TCHAR *)tmpCurFileStr);
  #else
    cutFileName(list_file.long_name[sel_id], 16, 16, tmpCurFileStr);
  #endif

  ZERO(public_buf_m);

  while (index <= disp_state_stack._disp_index) {
    tmpText = getDispText(index);
    if ((*tmpText == 0) || (tmpText == 0)) {
      index++;
      continue;
    }

    titleText_cat(public_buf_m, sizeof(public_buf_m), tmpText);
    if (index < disp_state_stack._disp_index) titleText_cat(public_buf_m, sizeof(public_buf_m), (char *)">");

    index++;
  }

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] == PRINTING_UI
    /*|| disp_state_stack._disp_state[disp_state_stack._disp_index] == OPERATE_UI
    || disp_state_stack._disp_state[disp_state_stack._disp_index] == PAUSE_UI*/
  ) {
    titleText_cat(public_buf_m, sizeof(public_buf_m), (char *)":");
    titleText_cat(public_buf_m, sizeof(public_buf_m), tmpCurFileStr);
  }

  if (strlen(public_buf_m) > MAX_TITLE_LEN) {
    ZERO(public_buf_m);
    tmpText = getDispText(0);
    if (*tmpText != 0) {
      titleText_cat(public_buf_m, sizeof(public_buf_m), tmpText);
      titleText_cat(public_buf_m, sizeof(public_buf_m), (char *)">...>");
      tmpText = getDispText(disp_state_stack._disp_index);
      if (*tmpText != 0) titleText_cat(public_buf_m, sizeof(public_buf_m), tmpText);
    }
  }

  return public_buf_m;
}

#if HAS_GCODE_PREVIEW

  uint32_t gPicturePreviewStart = 0;

  void preview_gcode_prehandle(char *path) {
    #if ENABLED(SDSUPPORT)
      //uint8_t re;
      //uint32_t read;
      uint32_t pre_read_cnt = 0;
      uint32_t *p1;
      char *cur_name;

      gPicturePreviewStart = 0;
      cur_name             = strrchr(path, '/');
      card.openFileRead(cur_name);
      card.read(public_buf, 512);
      p1 = (uint32_t *)strstr((char *)public_buf, ";simage:");

      if (p1) {
        pre_read_cnt = (uint32_t)p1 - (uint32_t)((uint32_t *)(&public_buf[0]));

        To_pre_view              = pre_read_cnt;
        gcode_preview_over       = 1;
        gCfgItems.from_flash_pic = 1;
        update_spi_flash();
      }
      else {
        gcode_preview_over       = 0;
        default_preview_flg      = 1;
        gCfgItems.from_flash_pic = 0;
        update_spi_flash();
      }
      card.closefile();
    #endif
  }

  #if 1

    void gcode_preview(char *path, int xpos_pixel, int ypos_pixel) {
      #if ENABLED(SDSUPPORT)
        //uint8_t ress;
        //uint32_t write;
        volatile uint32_t i, j;
        volatile uint16_t *p_index;
        //int res;
        char *cur_name;

        cur_name = strrchr(path, '/');
        card.openFileRead(cur_name);

        if (gPicturePreviewStart <= 0) {
          while (1) {
            uint32_t br  = card.read(public_buf, 400);
            uint32_t* p1 = (uint32_t *)strstr((char *)public_buf, ";gimage:");
            if (p1) {
              gPicturePreviewStart += (uint32_t)p1 - (uint32_t)((uint32_t *)(&public_buf[0]));
              break;
            }
            else {
              gPicturePreviewStart += br;
            }
            if (br < 400) break;
          }
        }

        card.setIndex((gPicturePreviewStart + To_pre_view) + size * row + 8);
        #if ENABLED(TFT_LVGL_UI_SPI)
          SPI_TFT.SetWindows(xpos_pixel, ypos_pixel + row, 200, 1);
        #else
          ili9320_SetWindows(xpos_pixel, ypos_pixel + row, 200, 1);
          LCD_WriteRAM_Prepare();
        #endif

        j = i = 0;

        while (1) {
          card.read(public_buf, 400);
          for (i = 0; i < 400;) {
            bmp_public_buf[j] = ascii2dec_test((char*)&public_buf[i]) << 4 | ascii2dec_test((char*)&public_buf[i + 1]);
            i                += 2;
            j++;
          }
          if (j >= 400) break;
        }
        #if ENABLED(TFT_LVGL_UI_SPI)
          for (i = 0; i < 400; i += 2) {
            p_index  = (uint16_t *)(&bmp_public_buf[i]);
            if (*p_index == 0x0000) *p_index = LV_COLOR_BACKGROUND.full;
          }
          SPI_TFT.tftio.WriteSequence((uint16_t*)bmp_public_buf, 200);
        #else
          for (i = 0; i < 400;) {
            p_index = (uint16_t *)(&bmp_public_buf[i]);
            if (*p_index == 0x0000) *p_index = LV_COLOR_BACKGROUND.full; //gCfgItems.preview_bk_color;
            LCD_IO_WriteData(*p_index);
            i += 2;
          }
        #endif
        #if HAS_BAK_VIEW_IN_FLASH
          W25QXX.init(SPI_QUARTER_SPEED);
          if (row < 20) W25QXX.SPI_FLASH_SectorErase(BAK_VIEW_ADDR_TFT35 + row * 4096);
          W25QXX.SPI_FLASH_BufferWrite(bmp_public_buf, BAK_VIEW_ADDR_TFT35 + row * 400, 400);
        #endif
        row++;
        if (row >= 200) {
          size = 809;
          row  = 0;

          gcode_preview_over = 0;
          //flash_preview_begin = 1;

          card.closefile();

          /*
          if (gCurFileState.file_open_flag != 0xaa) {
            reset_file_info();
            res = f_open(file, curFileName, FA_OPEN_EXISTING | FA_READ);
            if (res == FR_OK) {
              f_lseek(file,PREVIEW_SIZE+To_pre_view);
              gCurFileState.file_open_flag = 0xaa;
              //bakup_file_path((uint8_t *)curFileName, strlen(curFileName));
              srcfp = file;
              mksReprint.mks_printer_state = MKS_WORKING;
              once_flag = 0;
            }
          }
          */
          char *cur_name;

          cur_name = strrchr(list_file.file_name[sel_id], '/');

          SdFile file;
          SdFile *curDir;
          card.endFilePrint();
          const char * const fname = card.diveToFile(true, curDir, cur_name);
          if (!fname) return;
          if (file.open(curDir, fname, O_READ)) {
            gCfgItems.curFilesize = file.fileSize();
            file.close();
            update_spi_flash();
          }

          card.openFileRead(cur_name);
          if (card.isFileOpen()) {
            feedrate_percentage = 100;
            //saved_feedrate_percentage = feedrate_percentage;
            planner.flow_percentage[0] = 100;
            planner.e_factor[0]        = planner.flow_percentage[0] * 0.01;
            if (EXTRUDERS == 2) {
              planner.flow_percentage[1] = 100;
              planner.e_factor[1]        = planner.flow_percentage[1] * 0.01;
            }
            card.startFileprint();
            TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
            once_flag = 0;
          }
          return;
        }
        card.closefile();
      #endif // SDSUPPORT
    }

  #else // if 1

    void gcode_preview(char *path, int xpos_pixel, int ypos_pixel) {
      #if ENABLED(SDSUPPORT)
        //uint8_t ress;
        //uint32_t write;
        volatile uint32_t i, j;
        volatile uint16_t *p_index;
        //int res;
        char *cur_name;
        uint16_t Color;

        cur_name = strrchr(path, '/');
        card.openFileRead(cur_name);

        card.setIndex((PREVIEW_LITTLE_PIC_SIZE + To_pre_view) + size * row + 8);
        #if ENABLED(TFT_LVGL_UI_SPI)
          SPI_TFT.SetWindows(xpos_pixel, ypos_pixel + row, 200, 1);
        #else
          ili9320_SetWindows(xpos_pixel, ypos_pixel + row, 200, 1);
          LCD_WriteRAM_Prepare();
        #endif

        j = 0;
        i = 0;

        while (1) {
          card.read(public_buf, 400);
          for (i = 0; i < 400;) {
            bmp_public_buf[j] = ascii2dec_test((char*)&public_buf[i]) << 4 | ascii2dec_test((char*)&public_buf[i + 1]);
            i += 2;
            j++;
          }

          //if (i > 800) break;
          //#ifdef TFT70
          //  if (j > 400) {
          //    f_read(file, buff_pic, 1, &read);
          //    break;
          //  }
          //#elif defined(TFT35)
          if (j >= 400)
            //f_read(file, buff_pic, 1, &read);
            break;
          //#endif

        }
        #if ENABLED(TFT_LVGL_UI_SPI)
          for (i = 0; i < 400;) {
            p_index = (uint16_t *)(&bmp_public_buf[i]);

            Color    = (*p_index >> 8);
            *p_index = Color | ((*p_index & 0xff) << 8);
            i       += 2;
            if (*p_index == 0x0000) *p_index = 0xC318;
          }
          SPI_TFT_CS_L;
          SPI_TFT_DC_H;
          SPI.dmaSend(bmp_public_buf, 400, true);
          SPI_TFT_CS_H;

        #else
          for (i = 0; i < 400;) {
            p_index = (uint16_t *)(&bmp_public_buf[i]);
            if (*p_index == 0x0000) *p_index = 0x18C3;
            LCD_IO_WriteData(*p_index);
            i = i + 2;
          }
        #endif
        W25QXX.init(SPI_QUARTER_SPEED);
        if (row < 20)
          W25QXX.SPI_FLASH_SectorErase(BAK_VIEW_ADDR_TFT35 + row * 4096);
        W25QXX.SPI_FLASH_BufferWrite(bmp_public_buf, BAK_VIEW_ADDR_TFT35 + row * 400, 400);
        row++;
        if (row >= 200) {
          size = 809;
          row  = 0;

          gcode_preview_over = 0;
          //flash_preview_begin = 1;

          card.closefile();

          /*
          if (gCurFileState.file_open_flag != 0xaa) {
            reset_file_info();
            res = f_open(file, curFileName, FA_OPEN_EXISTING | FA_READ);
            if (res == FR_OK) {
              f_lseek(file,PREVIEW_SIZE+To_pre_view);
              gCurFileState.file_open_flag = 0xaa;
              //bakup_file_path((uint8_t *)curFileName, strlen(curFileName));
              srcfp = file;
              mksReprint.mks_printer_state = MKS_WORKING;
              once_flag = 0;
            }
          }
          */
          char *cur_name;

          cur_name = strrchr(list_file.file_name[sel_id], '/');

          SdFile file;
          SdFile *curDir;
          card.endFilePrint();
          const char * const fname = card.diveToFile(true, curDir, cur_name);
          if (!fname) return;
          if (file.open(curDir, fname, O_READ)) {
            gCfgItems.curFilesize = file.fileSize();
            file.close();
            update_spi_flash();
          }

          card.openFileRead(cur_name);
          if (card.isFileOpen()) {
            feedrate_percentage = 100;
            //saved_feedrate_percentage = feedrate_percentage;
            planner.flow_percentage[0] = 100;
            planner.e_factor[0]        = planner.flow_percentage[0] * 0.01;
            if (EXTRUDERS == 2) {
              planner.flow_percentage[1] = 100;
              planner.e_factor[1]        = planner.flow_percentage[1] * 0.01;
            }
            card.startFileprint();
            TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
            once_flag = 0;
          }
          return;
        }
        card.closefile();
      #endif // SDSUPPORT
    }

  #endif // if 1

  void Draw_default_preview(int xpos_pixel, int ypos_pixel, uint8_t sel) {
    int index;
    int y_off = 0;

    for (index = 0; index < 10; index++) { // 200*200
      #if HAS_BAK_VIEW_IN_FLASH
        if (sel == 1) {
          flash_view_Read(bmp_public_buf, 8000); // 20k
        }
        else {
          default_view_Read(bmp_public_buf, DEFAULT_VIEW_MAX_SIZE / 10); // 20k
        }
      #else
        default_view_Read(bmp_public_buf, DEFAULT_VIEW_MAX_SIZE / 10); // 20k
      #endif

      #if ENABLED(TFT_LVGL_UI_SPI)
        SPI_TFT.SetWindows(xpos_pixel, y_off * 20 + ypos_pixel, 200, 20); // 200*200
        SPI_TFT.tftio.WriteSequence((uint16_t*)(bmp_public_buf), DEFAULT_VIEW_MAX_SIZE / 20);
      #else
        int x_off = 0;
        uint16_t temp_p;
        int i = 0;
        uint16_t *p_index;
        ili9320_SetWindows(xpos_pixel, y_off * 20 + ypos_pixel, 200, 20); // 200*200

        LCD_WriteRAM_Prepare();

        for (int _y = y_off * 20; _y < (y_off + 1) * 20; _y++) {
          for (x_off = 0; x_off < 200; x_off++) {
            if (sel == 1) {
              temp_p  = (uint16_t)(bmp_public_buf[i] | bmp_public_buf[i + 1] << 8);
              p_index = &temp_p;
            }
            else {
              p_index = (uint16_t *)(&bmp_public_buf[i]);
            }
            if (*p_index == 0x0000) *p_index = LV_COLOR_BACKGROUND.full; //gCfgItems.preview_bk_color;
            LCD_IO_WriteData(*p_index);
            i += 2;
          }
          if (i >= 8000) break;
        }
      #endif // TFT_LVGL_UI_SPI
      y_off++;
    }
    W25QXX.init(SPI_QUARTER_SPEED);
  }

  void disp_pre_gcode(int xpos_pixel, int ypos_pixel) {
    if (gcode_preview_over == 1) gcode_preview(list_file.file_name[sel_id], xpos_pixel, ypos_pixel);
    #if HAS_BAK_VIEW_IN_FLASH
      if (flash_preview_begin == 1) {
        flash_preview_begin = 0;
        Draw_default_preview(xpos_pixel, ypos_pixel, 1);
      }
    #endif
    #if HAS_GCODE_DEFAULT_VIEW_IN_FLASH
      if (default_preview_flg == 1) {
        Draw_default_preview(xpos_pixel, ypos_pixel, 0);
        default_preview_flg = 0;
      }
    #endif
  }
#endif // HAS_GCODE_PREVIEW

void print_time_run() {
  static uint8_t lastSec = 0;

  if (print_time.seconds >= 60) {
    print_time.seconds = 0;
    print_time.minutes++;
    if (print_time.minutes >= 60) {
      print_time.minutes = 0;
      print_time.hours++;
    }
  }
  if (disp_state == PRINTING_UI) {
    if (lastSec != print_time.seconds) disp_print_time();
    lastSec = print_time.seconds;
  }
}

void GUI_RefreshPage() {
  if ((systick_uptime_millis % 1000) == 0) temperature_change_frequency = 1;
  if ((systick_uptime_millis % 3000) == 0) printing_rate_update_flag = 1;

  switch (disp_state) {
    case MAIN_UI:
      //lv_draw_ready_print();
      break;
    case EXTRUSION_UI:
      if (temperature_change_frequency == 1) {
        temperature_change_frequency = 0;
        disp_hotend_temp();
      }
      break;
    case PRE_HEAT_UI:
      if (temperature_change_frequency == 1) {
        temperature_change_frequency = 0;
        disp_desire_temp();
      }
      break;
    case PRINT_READY_UI:
      /*
      if (gCfgItems.display_style == 2) {
        if (temperature_change_frequency) {
          temperature_change_frequency = 0;
          disp_restro_state();
        }
      }
      */
      break;

    case PRINT_FILE_UI: break;

    case PRINTING_UI:
      if (temperature_change_frequency) {
        temperature_change_frequency = 0;
        disp_ext_temp();
        disp_bed_temp();
        disp_fan_speed();
        disp_print_time();
        disp_fan_Zpos();
      }
      if (printing_rate_update_flag || marlin_state == MF_SD_COMPLETE) {
        printing_rate_update_flag = 0;
        if (gcode_preview_over == 0) setProBarRate();
      }
      break;

    case OPERATE_UI:
      /*
      if (temperature_change_frequency == 1) {
        temperature_change_frequency = 0;
        disp_temp_operate();
      }

      setProBarRateOpera();
      */
      break;

    case PAUSE_UI:
      /*
      if (temperature_change_frequency == 1) {
        temperature_change_frequency = 0;
        disp_temp_pause();
      }
      */
      break;

    case FAN_UI:
      if (temperature_change_frequency == 1) {
        temperature_change_frequency = 0;
        disp_fan_value();
      }
      break;

    case MOVE_MOTOR_UI:
      /*
      if (mksReprint.mks_printer_state == MKS_IDLE) {
        if ((z_high_count==1)&&(temper_error_flg != 1)) {
          z_high_count = 0;
          {
            memset((char *)gCfgItems.move_z_coordinate, ' ', sizeof(gCfgItems.move_z_coordinate));
            GUI_DispStringAt((const char *)gCfgItems.move_z_coordinate, 380, TITLE_YPOS);
            sprintf_P((char *)gCfgItems.move_z_coordinate, PSTR("Z: %.3f"), current_position[Z_AXIS]);
            GUI_DispStringAt((const char *)gCfgItems.move_z_coordinate, 380, TITLE_YPOS);
          }
        }
      }
      */
      break;

    case WIFI_UI:
      /*
      if (wifi_refresh_flg == 1) {
        disp_wifi_state();
        wifi_refresh_flg = 0;
      }
      */
      break;
    case BIND_UI:
      /*refresh_bind_ui();*/
      break;

    case FILAMENTCHANGE_UI:
      /*
      if (temperature_change_frequency) {
        temperature_change_frequency = 0;
        disp_filament_sprayer_temp();
      }
      */
      break;
    case DIALOG_UI:
      /*filament_dialog_handle();
      wifi_scan_handle();*/
      break;
    case MESHLEVELING_UI:
      /*disp_zpos();*/
      break;
    case HARDWARE_TEST_UI:
      break;
    case WIFI_LIST_UI:
      /*
      if (wifi_refresh_flg == 1) {
        disp_wifi_list();
        wifi_refresh_flg = 0;
      }
      */
      break;
    case KEY_BOARD_UI:
      /*update_password_disp();
      update_join_state_disp();*/
      break;
    case TIPS_UI:
      /*
      switch (tips_type) {
        case TIPS_TYPE_JOINING:
          if (wifi_link_state == WIFI_CONNECTED && strcmp((const char *)wifi_list.wifiConnectedName, (const char *)wifi_list.wifiName[wifi_list.nameIndex]) == 0) {
            tips_disp.timer = TIPS_TIMER_STOP;
            tips_disp.timer_count = 0;
            Clear_Tips();
            tips_type = TIPS_TYPE_WIFI_CONECTED;
            draw_Tips();
          }
          if (tips_disp.timer_count >= 30) {
            tips_disp.timer = TIPS_TIMER_STOP;
            tips_disp.timer_count = 0;
            Clear_Tips();
            tips_type = TIPS_TYPE_TAILED_JOIN;
            draw_Tips();
          }
          break;
        case TIPS_TYPE_TAILED_JOIN:
          if (tips_disp.timer_count >= 3) {
            tips_disp.timer = TIPS_TIMER_STOP;
            tips_disp.timer_count = 0;
            last_disp_state = TIPS_UI;
            Clear_Tips();
            draw_Wifi_list();
          }
          break;
        case TIPS_TYPE_WIFI_CONECTED:
          if (tips_disp.timer_count >= 3) {
            tips_disp.timer = TIPS_TIMER_STOP;
            tips_disp.timer_count = 0;

            last_disp_state = TIPS_UI;
            Clear_Tips();
            draw_Wifi();
          }
          break;
        default: break;
      }
      */
      break;
    case BABY_STEP_UI:
      /*
      if (temperature_change_frequency == 1) {
        temperature_change_frequency = 0;
        disp_z_offset_value();
      }
      */
      break;
    default: break;
  }

  print_time_run();
}

void clear_cur_ui() {
  last_disp_state = disp_state_stack._disp_state[disp_state_stack._disp_index];

  switch (disp_state_stack._disp_state[disp_state_stack._disp_index]) {
    case PRINT_READY_UI:
      //Get_Temperature_Flg = 0;
      lv_clear_ready_print();
      break;
    case PRINT_FILE_UI:
      lv_clear_print_file();
      break;
    case PRINTING_UI:
      lv_clear_printing();
      break;
    case MOVE_MOTOR_UI:
      lv_clear_move_motor();
      break;
    case OPERATE_UI:
      lv_clear_operation();
      break;
    case PAUSE_UI:
      //Clear_pause();
      break;
    case EXTRUSION_UI:
      lv_clear_extrusion();
      break;
    case PRE_HEAT_UI:
      lv_clear_preHeat();
      break;
    case CHANGE_SPEED_UI:
      lv_clear_change_speed();
      break;
    case FAN_UI:
      lv_clear_fan();
      break;
    case SET_UI:
      lv_clear_set();
      break;
    case ZERO_UI:
      lv_clear_home();
      break;
    case SPRAYER_UI:
      //Clear_Sprayer();
      break;
    case MACHINE_UI:
      //Clear_Machine();
      break;
    case LANGUAGE_UI:
      lv_clear_language();
      break;
    case ABOUT_UI:
      lv_clear_about();
      break;
    case LOG_UI:
      //Clear_Connect();
      break;
    case DISK_UI:
      //Clear_Disk();
      break;
    case WIFI_UI:
      //Clear_Wifi();
      break;
    case MORE_UI:
      //Clear_more();
      break;
    case FILETRANSFER_UI:
      //Clear_fileTransfer();
      break;
    case DIALOG_UI:
      lv_clear_dialog();
      break;
    case FILETRANSFERSTATE_UI:
      //Clear_WifiFileTransferdialog();
      break;
    case PRINT_MORE_UI:
      //Clear_Printmore();
      break;
    case LEVELING_UI:
      lv_clear_manualLevel();
      break;
    case BIND_UI:
      //Clear_Bind();
      break;
    case ZOFFSET_UI:
      //Clear_Zoffset();
      break;
    case TOOL_UI:
      lv_clear_tool();
      break;
    case MESHLEVELING_UI:
      //Clear_MeshLeveling();
      break;
    case HARDWARE_TEST_UI:
      //Clear_Hardwaretest();
      break;
    case WIFI_LIST_UI:
      //Clear_Wifi_list();
      break;
    case KEY_BOARD_UI:
      //Clear_Keyboard();
      break;
    case TIPS_UI:
      //Clear_Tips();
      break;
    case MACHINE_PARA_UI:
      lv_clear_machine_para();
      break;
    case MACHINE_SETTINGS_UI:
      lv_clear_machine_settings();
      break;
    case TEMPERATURE_SETTINGS_UI:
      //Clear_TemperatureSettings();
      break;
    case MOTOR_SETTINGS_UI:
      lv_clear_motor_settings();
      break;
    case MACHINETYPE_UI:
      //Clear_MachineType();
      break;
    case STROKE_UI:
      //Clear_Stroke();
      break;
    case HOME_DIR_UI:
      //Clear_HomeDir();
      break;
    case ENDSTOP_TYPE_UI:
      //Clear_EndstopType();
      break;
    case FILAMENT_SETTINGS_UI:
      //Clear_FilamentSettings();
      break;
    case LEVELING_SETTIGNS_UI:
      //Clear_LevelingSettings();
      break;
    case LEVELING_PARA_UI:
      //Clear_LevelingPara();
      break;
    case DELTA_LEVELING_PARA_UI:
      //Clear_DeltaLevelPara();
      break;
    case XYZ_LEVELING_PARA_UI:
      //Clear_XYZLevelPara();
      break;
    case MAXFEEDRATE_UI:
      lv_clear_max_feedrate_settings();
      break;
    case STEPS_UI:
      lv_clear_step_settings();
      break;
    case ACCELERATION_UI:
      lv_clear_acceleration_settings();
      break;
    case JERK_UI:
      #if HAS_CLASSIC_JERK
        lv_clear_jerk_settings();
      #endif
      break;
    case MOTORDIR_UI:
      //Clear_MotorDir();
      break;
    case HOMESPEED_UI:
      //Clear_HomeSpeed();
      break;
    case NOZZLE_CONFIG_UI:
      //Clear_NozzleConfig();
      break;
    case HOTBED_CONFIG_UI:
      //Clear_HotbedConfig();
      break;
    case ADVANCED_UI:
      lv_clear_advance_settings();
      break;
    case DOUBLE_Z_UI:
      //Clear_DoubleZ();
      break;
    case ENABLE_INVERT_UI:
      //Clear_EnableInvert();
      break;
    case NUMBER_KEY_UI:
      lv_clear_number_key();
      break;
    case BABY_STEP_UI:
      //Clear_babyStep();
      break;
    case PAUSE_POS_UI:
      lv_clear_pause_position();
      break;
      #if HAS_TRINAMIC_CONFIG
        case TMC_CURRENT_UI:
          lv_clear_tmc_current_settings();
          break;
      #endif
    case EEPROM_SETTINGS_UI:
      lv_clear_eeprom_settings();
      break;
      #if HAS_STEALTHCHOP
        case TMC_MODE_UI:
          lv_clear_tmc_step_mode_settings();
          break;
      #endif
    default:
      break;
  }
  //GUI_Clear();
}

void draw_return_ui() {
  if (disp_state_stack._disp_index > 0) {
    disp_state_stack._disp_index--;

    switch (disp_state_stack._disp_state[disp_state_stack._disp_index]) {
      case PRINT_READY_UI:
        lv_draw_ready_print();
        break;
      case PRINT_FILE_UI:
        lv_draw_print_file();
        break;
      case PRINTING_UI:
        if (gCfgItems.from_flash_pic == 1) flash_preview_begin = 1;
        else default_preview_flg = 1;
        lv_draw_printing();
        break;
      case MOVE_MOTOR_UI:
        lv_draw_move_motor();
        break;
      case OPERATE_UI:
        lv_draw_operation();
        break;

        #if 1
          case PAUSE_UI:
            //draw_pause();
            break;
        #endif

      case EXTRUSION_UI:
        lv_draw_extrusion();
        break;
      case PRE_HEAT_UI:
        lv_draw_preHeat();
        break;
      case CHANGE_SPEED_UI:
        lv_draw_change_speed();
        break;
      case FAN_UI:
        lv_draw_fan();
        break;
      case SET_UI:
        lv_draw_set();
        break;
      case ZERO_UI:
        lv_draw_home();
        break;
      case SPRAYER_UI:
        //draw_Sprayer();
        break;
      case MACHINE_UI:
        //draw_Machine();
        break;
      case LANGUAGE_UI:
        lv_draw_language();
        break;
      case ABOUT_UI:
        lv_draw_about();
        break;

        #if tan_mask
          case LOG_UI:
            //draw_Connect();
            break;
        #endif

      case CALIBRATE_UI:
        //draw_calibrate();
        break;
      case DISK_UI:
        //draw_Disk();
        break;
      case WIFI_UI:
        //draw_Wifi();
        break;
      case MORE_UI:
        //draw_More();
        break;
      case PRINT_MORE_UI:
        //draw_printmore();
        break;
      case FILAMENTCHANGE_UI:
        //draw_FilamentChange();
        break;
      case LEVELING_UI:
        lv_draw_manualLevel();
        break;
      case BIND_UI:
        //draw_bind();
        break;

        #if tan_mask
          case ZOFFSET_UI:
            //draw_Zoffset();
            break;
        #endif

      case TOOL_UI:
        lv_draw_tool();
        break;
      case MESHLEVELING_UI:
        //draw_meshleveling();
        break;
      case HARDWARE_TEST_UI:
        //draw_Hardwaretest();
        break;
      case WIFI_LIST_UI:
        //draw_Wifi_list();
        break;
      case KEY_BOARD_UI:
        //draw_Keyboard();
        break;
      case TIPS_UI:
        //draw_Tips();
        break;
      case MACHINE_PARA_UI:
        lv_draw_machine_para();
        break;
      case MACHINE_SETTINGS_UI:
        lv_draw_machine_settings();
        break;
      case TEMPERATURE_SETTINGS_UI:
        //draw_TemperatureSettings();
        break;
      case MOTOR_SETTINGS_UI:
        lv_draw_motor_settings();
        break;
      case MACHINETYPE_UI:
        //draw_MachineType();
        break;
      case STROKE_UI:
        //draw_Stroke();
        break;
      case HOME_DIR_UI:
        //draw_HomeDir();
        break;
      case ENDSTOP_TYPE_UI:
        //draw_EndstopType();
        break;
      case FILAMENT_SETTINGS_UI:
        //draw_FilamentSettings();
        break;
      case LEVELING_SETTIGNS_UI:
        //draw_LevelingSettings();
        break;
      case LEVELING_PARA_UI:
        //draw_LevelingPara();
        break;
      case DELTA_LEVELING_PARA_UI:
        //draw_DeltaLevelPara();
        break;
      case XYZ_LEVELING_PARA_UI:
        //draw_XYZLevelPara();
        break;
      case MAXFEEDRATE_UI:
        lv_draw_max_feedrate_settings();
        break;
      case STEPS_UI:
        lv_draw_step_settings();
        break;
      case ACCELERATION_UI:
        lv_draw_acceleration_settings();
        break;
      case JERK_UI:
        #if HAS_CLASSIC_JERK
          lv_draw_jerk_settings();
        #endif
        break;
      case MOTORDIR_UI:
        //draw_MotorDir();
        break;
      case HOMESPEED_UI:
        //draw_HomeSpeed();
        break;
      case NOZZLE_CONFIG_UI:
        //draw_NozzleConfig();
        break;
      case HOTBED_CONFIG_UI:
        //draw_HotbedConfig();
        break;
      case ADVANCED_UI:
        lv_draw_advance_settings();
        break;
      case DOUBLE_Z_UI:
        //draw_DoubleZ();
        break;
      case ENABLE_INVERT_UI:
        //draw_EnableInvert();
        break;
      case NUMBER_KEY_UI:
        lv_draw_number_key();
        break;
      case DIALOG_UI:
        //draw_dialog(DialogType);
        break;
      case BABY_STEP_UI:
        //draw_babyStep();
        break;
      case PAUSE_POS_UI:
        lv_draw_pause_position();
        break;
        #if HAS_TRINAMIC_CONFIG
          case TMC_CURRENT_UI:
            lv_draw_tmc_current_settings();
            break;
        #endif
      case EEPROM_SETTINGS_UI:
        lv_draw_eeprom_settings();
        break;
        #if HAS_STEALTHCHOP
          case TMC_MODE_UI:
            lv_draw_tmc_step_mode_settings();
            break;
        #endif
      default: break;
    }
  }
}

#if ENABLED(SDSUPPORT)

  void sd_detection() {
    static bool last_sd_status;
    const bool sd_status = IS_SD_INSERTED();
    if (sd_status != last_sd_status) {
      last_sd_status = sd_status;
      if (sd_status) card.mount(); else card.release();
    }
  }

#endif

void lv_ex_line(lv_obj_t * line, lv_point_t *points) {
  /*Copy the previous line and apply the new style*/
  lv_line_set_points(line, points, 2);     /*Set the points*/
  lv_line_set_style(line, LV_LINE_STYLE_MAIN, &style_line);
  lv_obj_align(line, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
}

extern volatile uint32_t systick_uptime_millis;

void print_time_count() {
  if ((systick_uptime_millis % 1000) == 0)
    if (print_time.start == 1) print_time.seconds++;
}

void LV_TASK_HANDLER() {
  //lv_tick_inc(1);
  lv_task_handler();
  if (mks_test_flag == 0x1e) mks_hardware_test();
  #if HAS_GCODE_PREVIEW
    disp_pre_gcode(2, 36);
  #endif
  GUI_RefreshPage();
  //sd_detection();
}

#endif // HAS_TFT_LVGL_UI
