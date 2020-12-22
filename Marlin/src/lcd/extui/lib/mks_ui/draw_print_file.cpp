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

#include "../../../../sd/cardreader.h"
#include "../../../../inc/MarlinConfig.h"

static lv_obj_t *scr;
extern lv_group_t*  g;

static lv_obj_t *buttonPageUp, *buttonPageDown, *buttonBack,
                *buttonGcode[FILE_BTN_CNT], *labelPageUp[FILE_BTN_CNT], *buttonText[FILE_BTN_CNT];

enum {
  ID_P_UP = 7,
  ID_P_DOWN,
  ID_P_RETURN
};

int8_t curDirLever = 0;
LIST_FILE list_file;
DIR_OFFSET dir_offset[10];

extern uint8_t public_buf[512];
extern char public_buf_m[100];

uint8_t sel_id = 0;

#if ENABLED(SDSUPPORT)

  static uint8_t search_file() {
    int valid_name_cnt = 0;

    list_file.Sd_file_cnt = 0;

    if (curDirLever != 0) card.cd(list_file.curDirPath);
    else card.cdroot(); 

    const uint16_t fileCnt = card.get_num_Files();

    for (uint16_t i = 0; i < fileCnt; i++) {
      if (list_file.Sd_file_cnt == list_file.Sd_file_offset) {
        const uint16_t nr = SD_ORDER(i, fileCnt);
        card.getfilename_sorted(nr);

        list_file.IsFolder[valid_name_cnt] = card.flag.filenameIsDir;
        strcpy(list_file.file_name[valid_name_cnt], list_file.curDirPath);
        strcat_P(list_file.file_name[valid_name_cnt], PSTR("/"));
        strcat(list_file.file_name[valid_name_cnt], card.filename);
        strcpy(list_file.long_name[valid_name_cnt], card.longest_filename());

        valid_name_cnt++;
        if (valid_name_cnt == 1)
          dir_offset[curDirLever].cur_page_first_offset = list_file.Sd_file_offset;
        if (valid_name_cnt >= FILE_NUM) {
          dir_offset[curDirLever].cur_page_last_offset = list_file.Sd_file_offset;
          list_file.Sd_file_offset++;
          break;
        }
        list_file.Sd_file_offset++;
      }
      list_file.Sd_file_cnt++;
    }
    return valid_name_cnt;
  }

#endif // SDSUPPORT

bool have_pre_pic(char *path) {
  #if ENABLED(SDSUPPORT)
    char *ps1, *ps2, *cur_name = strrchr(path, '/');
    if (!cur_name) return false;
    card.openFileRead(cur_name);
    card.read(public_buf, 512);
    ps1 = strstr((char *)public_buf, ";simage:");
    card.read(public_buf, 512);
    ps2 = strstr((char *)public_buf, ";simage:");
    card.closefile();
    if (ps1 || ps2) return true;
  #endif

  return false;
}

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  uint8_t i, file_count = 0;
  
  if (obj->mks_obj_id == ID_P_UP) {
    if (dir_offset[curDirLever].curPage > 0) {
      list_file.Sd_file_cnt = 0;

      if (dir_offset[curDirLever].cur_page_first_offset >= FILE_NUM)
        list_file.Sd_file_offset = dir_offset[curDirLever].cur_page_first_offset - FILE_NUM;

      #if ENABLED(SDSUPPORT)
        file_count = search_file();
      #endif
      if (file_count != 0) {
        dir_offset[curDirLever].curPage--;
        lv_clear_print_file();
        disp_gcode_icon(file_count);
      }
    }
  }
  else if (obj->mks_obj_id == ID_P_DOWN) {
    if (dir_offset[curDirLever].cur_page_last_offset > 0) {
      list_file.Sd_file_cnt    = 0;
      list_file.Sd_file_offset = dir_offset[curDirLever].cur_page_last_offset + 1;
      #if ENABLED(SDSUPPORT)
        file_count = search_file();
      #endif
      if (file_count != 0) {
        dir_offset[curDirLever].curPage++;
        lv_clear_print_file();
        disp_gcode_icon(file_count);
      }
      if (file_count < FILE_NUM)
        dir_offset[curDirLever].cur_page_last_offset = 0;
    }
  }
  else if (obj->mks_obj_id == ID_P_RETURN) {
    if (curDirLever > 0) {
      int8_t *ch = (int8_t *)strrchr(list_file.curDirPath, '/');
      if (ch) {
        *ch = 0;
        #if ENABLED(SDSUPPORT)
          card.cdup();
        #endif
        dir_offset[curDirLever].curPage               = 0;
        dir_offset[curDirLever].cur_page_first_offset = 0;
        dir_offset[curDirLever].cur_page_last_offset  = 0;
        curDirLever--;
        list_file.Sd_file_offset = dir_offset[curDirLever].cur_page_first_offset;
        #if ENABLED(SDSUPPORT)
          file_count = search_file();
        #endif
        lv_clear_print_file();
        disp_gcode_icon(file_count);
      }
    }
    else {
      lv_clear_print_file();
      lv_draw_ready_print();
    }
  }
  else {
    for (i = 0; i < FILE_BTN_CNT; i++) {
      if (obj->mks_obj_id == (i + 1)) {
        if (list_file.file_name[i][0] != 0) {
          if (list_file.IsFolder[i]) {
            strcpy(list_file.curDirPath, list_file.file_name[i]);
            curDirLever++;
            list_file.Sd_file_offset = dir_offset[curDirLever].cur_page_first_offset;
            #if ENABLED(SDSUPPORT)
              file_count = search_file();
            #endif
            lv_clear_print_file();
            disp_gcode_icon(file_count);
          }
          else {
            sel_id = i;
            lv_clear_print_file();
            lv_draw_dialog(DIALOG_TYPE_PRINT_FILE);
          }
          break;
        }
      }
    }
  }
}

void lv_draw_print_file(void) {
  uint8_t file_count;

  curDirLever = 0;
  dir_offset[curDirLever].curPage = 0;

  list_file.Sd_file_offset = 0;
  list_file.Sd_file_cnt = 0;

  ZERO(dir_offset);
  ZERO(list_file.IsFolder);
  ZERO(list_file.curDirPath);

  list_file.Sd_file_offset = dir_offset[curDirLever].cur_page_first_offset;
  #if ENABLED(SDSUPPORT)
    card.mount();
    file_count = search_file();
  #endif
  disp_gcode_icon(file_count);
}
static char test_public_buf_l[FILE_NUM][SHORT_NAME_LEN + strlen("S:/") + 1];
void disp_gcode_icon(uint8_t file_num) {
  uint8_t i;

  scr = lv_screen_create(PRINT_FILE_UI, "");

  // Create image buttons
  buttonPageUp   = lv_imgbtn_create(scr, "F:/bmp_pageUp.bin", OTHER_BTN_XPIEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_P_UP);
  buttonPageDown = lv_imgbtn_create(scr, "F:/bmp_pageDown.bin", OTHER_BTN_XPIEL * 3 + INTERVAL_V * 4, titleHeight + OTHER_BTN_YPIEL + INTERVAL_H, event_handler, ID_P_DOWN);
  buttonBack     = lv_imgbtn_create(scr, "F:/bmp_back.bin", OTHER_BTN_XPIEL * 3 + INTERVAL_V * 4, titleHeight + OTHER_BTN_YPIEL * 2 + INTERVAL_H * 2, event_handler, ID_P_RETURN);

  // Create labels on the image buttons
  for (i = 0; i < FILE_BTN_CNT; i++) {
    
    if (i >= file_num) break;

    #ifdef TFT35
      buttonGcode[i] = lv_imgbtn_create(scr, nullptr);

      lv_imgbtn_use_label_style(buttonGcode[i]);
      lv_obj_clear_protect(buttonGcode[i], LV_PROTECT_FOLLOW);
      lv_btn_set_layout(buttonGcode[i], LV_LAYOUT_OFF);

      ZERO(public_buf_m);
      cutFileName((char *)list_file.long_name[i], 16, 8, (char *)public_buf_m);

      if (list_file.IsFolder[i]) {
        lv_obj_set_event_cb_mks(buttonGcode[i], event_handler, (i + 1), "", 0);
        lv_imgbtn_set_src_both(buttonGcode[i], "F:/bmp_dir.bin");
        if (i < 3)
          lv_obj_set_pos(buttonGcode[i], BTN_X_PIXEL * i + INTERVAL_V * (i + 1), titleHeight);
        else
          lv_obj_set_pos(buttonGcode[i], BTN_X_PIXEL * (i - 3) + INTERVAL_V * ((i - 3) + 1), BTN_Y_PIXEL + INTERVAL_H + titleHeight);

        labelPageUp[i] = lv_label_create(buttonGcode[i], public_buf_m);
        lv_obj_align(labelPageUp[i], buttonGcode[i], LV_ALIGN_IN_BOTTOM_MID, 0, -5);
      }
      else {
        if (have_pre_pic((char *)list_file.file_name[i])) {

          char *cur_name = strrchr(list_file.file_name[i], '/');
          test_public_buf_l[i][0] = '\0';
          strcat(test_public_buf_l[i],"S:");
          strcat(test_public_buf_l[i],cur_name);
          char *temp = strstr(test_public_buf_l[i],".GCO");
          if (temp) strcpy(temp, ".bin");
          lv_obj_set_event_cb_mks(buttonGcode[i], event_handler, (i + 1), "", 0);
          lv_imgbtn_set_src_both(buttonGcode[i], test_public_buf_l[i]);
          if (i < 3) {
            lv_obj_set_pos(buttonGcode[i], BTN_X_PIXEL * i + INTERVAL_V * (i + 1) + FILE_PRE_PIC_X_OFFSET, titleHeight + FILE_PRE_PIC_Y_OFFSET);
            buttonText[i] = lv_btn_create(scr, nullptr);

            lv_btn_use_label_style(buttonText[i]);
            lv_obj_clear_protect(buttonText[i], LV_PROTECT_FOLLOW);
            lv_btn_set_layout(buttonText[i], LV_LAYOUT_OFF);
            lv_obj_set_pos(buttonText[i], BTN_X_PIXEL * i + INTERVAL_V * (i + 1) + FILE_PRE_PIC_X_OFFSET, titleHeight + FILE_PRE_PIC_Y_OFFSET + 100);
            lv_obj_set_size(buttonText[i], 100, 40);
          }
          else {
            lv_obj_set_pos(buttonGcode[i], BTN_X_PIXEL * (i - 3) + INTERVAL_V * ((i - 3) + 1) + FILE_PRE_PIC_X_OFFSET, BTN_Y_PIXEL + INTERVAL_H + titleHeight + FILE_PRE_PIC_Y_OFFSET);
            buttonText[i] = lv_btn_create(scr, nullptr);

            lv_btn_use_label_style(buttonText[i]);
            lv_obj_clear_protect(buttonText[i], LV_PROTECT_FOLLOW);
            lv_btn_set_layout(buttonText[i], LV_LAYOUT_OFF);
            lv_obj_set_pos(buttonText[i], BTN_X_PIXEL * (i - 3) + INTERVAL_V * ((i - 3) + 1) + FILE_PRE_PIC_X_OFFSET, BTN_Y_PIXEL + INTERVAL_H + titleHeight + FILE_PRE_PIC_Y_OFFSET + 100);
            lv_obj_set_size(buttonText[i], 100, 40);
          }
          labelPageUp[i] = lv_label_create(buttonText[i], public_buf_m);
          lv_obj_align(labelPageUp[i], buttonText[i], LV_ALIGN_IN_BOTTOM_MID, 0, 0);
        }
        else {
          lv_obj_set_event_cb_mks(buttonGcode[i], event_handler, (i + 1), "", 0);
          lv_imgbtn_set_src_both(buttonGcode[i], "F:/bmp_file.bin");
          if (i < 3)
            lv_obj_set_pos(buttonGcode[i], BTN_X_PIXEL * i + INTERVAL_V * (i + 1), titleHeight);
          else
            lv_obj_set_pos(buttonGcode[i], BTN_X_PIXEL * (i - 3) + INTERVAL_V * ((i - 3) + 1), BTN_Y_PIXEL + INTERVAL_H + titleHeight);

          labelPageUp[i] = lv_label_create(buttonGcode[i], public_buf_m);
          lv_obj_align(labelPageUp[i], buttonGcode[i], LV_ALIGN_IN_BOTTOM_MID, 0, -5);
        }
      }
      #if HAS_ROTARY_ENCODER
        if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonGcode[i]);
      #endif

    #else // !TFT35
    #endif // !TFT35
  }
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonPageUp);
      lv_group_add_obj(g, buttonPageDown);
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

uint32_t lv_open_gcode_file(char *path) {
  #if ENABLED(SDSUPPORT)
    uint32_t *ps4;
    uint32_t pre_sread_cnt = 0;
    char *cur_name;

    cur_name = strrchr(path, '/');
    if (!cur_name) return 0;
    public_buf[511] = 0;
    card.openFileRead(cur_name);
    card.read(public_buf, 512);
    ps4 = (uint32_t *)strstr((char *)public_buf, ";simage:");
    // Ignore the beginning message of gcode file
    if (ps4) {
      pre_sread_cnt = (uint32_t)ps4 - (uint32_t)((uint32_t *)(&public_buf[0]));
      card.setIndex(pre_sread_cnt);
    }
    return pre_sread_cnt;
  #endif // SDSUPPORT
}

int ascii2dec_test(char *ascii) {
  int result = 0;
  if (ascii == 0) return 0;

  if (*(ascii) >= '0' && *(ascii) <= '9')
    result = *(ascii) - '0';
  else if (*(ascii) >= 'a' && *(ascii) <= 'f')
    result = *(ascii) - 'a' + 0x0A;
  else if (*(ascii) >= 'A' && *(ascii) <= 'F')
    result = *(ascii) - 'A' + 0x0A;
  else
    return 0;

  return result;
}

static int gcode_file_read(uint8_t *data_buf, uint32_t & position) {
  struct refill 
  {
    char buf[200];
    uint32_t available;
    uint32_t rpos;
    uint32_t remain() const { return available - rpos; }
    bool need(uint32_t amount) { 
      if (!card.isFileOpen()) return false;
      if (amount > sizeof(buf)) return false;
      int toread = amount - remain();
      if (toread <= 0) return true; // It's available already
      available -= rpos;
      memmove(buf, &buf[rpos], available);
      rpos = 0;
      card.read(&buf[available], toread);
      available += toread;
      return true;
    }
    char * head(uint32_t fetchqty) {
      if (fetchqty + rpos > available && !need(fetchqty)) return 0;
      rpos += fetchqty;
      return &buf[rpos - fetchqty];
    }
    void revert(uint32_t qty) {
      if (qty < rpos) rpos -= qty;
      else rpos = 0;
    }

    refill() : available(0), rpos(0) {}
  };

  const size_t amountToRead = 200;
  uint16_t count = 0; 
  // We don't call setIndex in the loop below since it's seeking the file and card.read does not update the position
  // So instead we keep track of the read position by ourself
  uint32_t fileSize = card.getFileSize();
  
  // Clear the buffer with background color
  memset(data_buf, 0, amountToRead);

  refill tmp;
  static bool rescaledOdd = false;
  uint32_t prevPosition = position;
  
  if (position < 208) { // This is 400 bytes of hexadecimal data + strlen(;simage;)
    // Step 1 here, let's find the beginning of the file's data
    
    // Prefetch a lot of data at once
    if (!tmp.need(200)) {
      card.closefile();
      return -1;
    }

    char * p = tmp.head(1);
    bool skipSearchNL = true; // First search, we just skip new line 
    while(card.isFileOpen() && position < fileSize) {
      if (!skipSearchNL) {
        // Find next new line
        while (p && (*p != '\r' && *p != '\n') && position < fileSize) { p = tmp.head(1); position++; }
        if (!p) return -2; // Nothing can be done here
        // Check if it's followed by a ';'
        p = tmp.head(1);
      }
      if (p && *p != ';') { skipSearchNL = false; continue; }
      position++;

      // Check if we have a simage available now
      p = tmp.head(7);
      if (!p) return -3;
      if (memcmp(p, "simage:", 7) == 0) { position += 7; break; }
      if (memcmp(p, "gimage;", 7) == 0) return 1; 

      // Not found, let's continue searching
      tmp.revert(7);
    }
    if (!p) return -4;
  }
  // Now we should be on data, let's read as many as required
  // Step 2
  bool rescaleBy2 = false;
  while (count < amountToRead && position < fileSize)
  {
    char * p = tmp.head(2);
    if (!p) return -5;
    
    if (p[0] == '\r' || p[1] == '\r' || p[0] == '\n' || p[1] == '\n')
    {
      // End of line, let's check the exit condition here
      if (p[1] != '\n' && p[1] != '\r') tmp.revert(1);
      p = tmp.head(8);
      if (!p || memcmp(p, ";;gimage", 8) == 0) {
        // Step 3
        position += 2;
        return 1;
      }
      if (memcmp(p, "M10086 ;", 8) == 0) {
        // Here we have a problem. This is called for lines of 200 bytes, so 100 pixels (with 16bit / pixels)
        // Yet, the files above are only 50x50 so we can't just use that and expect it'll work
        // We might need to resize the output, but this is only a guess since the image size is not in the header (why ?)
        // So toggle the resize flag if we only processed half the width yet
        if (count == amountToRead / 2) {
          rescaleBy2 = true;
          break;
        }
        position += 9;
        // tmp.need(200); // Each line is 208 bytes long so it's perfectly safe now to fetch a complete line, or is it ?
        continue;
      }
    }
    data_buf[count++] = (char)(ascii2dec_test(&p[0]) << 4 | ascii2dec_test(&p[1]));
    position += 2;
  }
  if (rescaleBy2) {
    // We received a 50x50 icon and we were queried for a 100x50 icon, so resize the data now
    // We have to progress backward to avoid overwriting what we just converted
    uint16_t * db = (uint16_t*)data_buf;
    for (int i = count - 1; i > 0; i-= 2) {
      db[i] = db[i/2];
      db[i-1] = db[i/2];
    }
    // We need to re-decode the same line next time we are called, so let's remember it 
    if (!rescaledOdd) position = prevPosition;
    rescaledOdd ^= true;
  }

  return 0;
}

void lv_gcode_file_read(uint8_t *data_buf, uint32_t unreliable_pos) {
  /* Example file content:
     --boundary_.oOo._F2003otnPwbPRz13/F4h0s+GUovex2zD
     Content-Disposition: form-data; name="file"; filename="SP_Mutter.gcode"

     ;simage:00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
     M10086 ;00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
     M10086 ;00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
     M10086 ;00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
     [...]
     M10086 ;
     ;;gimage:0000000000000000000000000000000000000000000000000000000000000000000000000
     [...]
     M10086 ;
     ;FLAVOR:Marlin
     ;TIME:1384
     [...]
  */
  // The idea is to fetch the small image (simage) from the comments
  // We'll be called multiple times without any possible storage :-/
  
  // So we have 3 steps to parse here
  // Step 1: Find the first "\r;simage:" code (drop any byte read meanwhile) 
  // Step 2: Extract all image data until end of line and skip header (`M10086 ;`) until we find a ';;' marker
  // Step 3: Skip ';;' marker and end/close the file here

  // Step 1 can only happen on first call, when card.file.curPosition() is 0
  // We *must* make progress to reach a simage/M10086 section so that next call will not be inside the header
  // Between call, we must be into the data itself (it's much easier this way)
  #if ENABLED(SDSUPPORT)
    uint32_t position = card.getIndex();
    int res = gcode_file_read(data_buf, position);
    if (res >= 0)
      card.setIndex(position);
    else card.closefile();

    // Fix the background color if required
    uint16_t * db = (uint16_t *)data_buf;
    for (size_t i = 0; i < 200 / sizeof(*db); i++)
      if (db[i] == 0x0000) db[i] = LV_COLOR_BACKGROUND.full;
  #endif
    


  #if ENABLED(SDSUPPORT) && 0
    uint16_t i = 0, j = 0, k = 0;
    uint16_t row_1    = 0;
    bool ignore_start = true;
    const size_t pic_width = 200;
    char temp_test[pic_width];
    volatile uint16_t *p_index;

    memset(public_buf, 0, pic_width);

    while (card.isFileOpen()) {
      if (ignore_start) card.read(temp_test, 8); // line start -> ignore
      card.read(temp_test, pic_width); // data
      // \r;;gimage: we got the bit img, so stop here
      if (temp_test[1] == ';') {
        card.closefile();
        break;
      }
      for (i = 0; i < pic_width;) {
        public_buf[row_1 * pic_width + 100 * k + j] = (char)(ascii2dec_test(&temp_test[i]) << 4 | ascii2dec_test(&temp_test[i + 1]));
        j++;
        i += 2;
      }

      uint16_t c = card.get();
      // check if we have more data or finished the line (CR)
      if (c == '\r') break;
      card.setIndex(card.getIndex());
      k++;
      j = 0;
      ignore_start = false;
    }
      for (i = 0; i < pic_width;) {
        p_index = (uint16_t *)(&public_buf[i]);

        i += 2;
        if (*p_index == 0x0000) *p_index = LV_COLOR_BACKGROUND.full;
      }
    memcpy(data_buf, public_buf, pic_width);
  #endif // SDSUPPORT
}

void lv_close_gcode_file() {TERN_(SDSUPPORT, card.closefile());}

void lv_gcode_file_seek(uint32_t pos) {
  // Don't seek. We don't care, since the parser will find its way in the file anyway and it breaks the logic if being seeked 
  //card.setIndex(pos);
}

void cutFileName(char *path, int len, int bytePerLine, char *outStr) {
  #if _LFN_UNICODE
    TCHAR *tmpFile;
    TCHAR *strIndex1 = 0, *strIndex2 = 0, *beginIndex;
    TCHAR secSeg[10]   = {0};
    TCHAR gFileTail[4] = {'~', '.', 'g', '\0'};
  #else
    char *tmpFile;
    char *strIndex1 = 0, *strIndex2 = 0, *beginIndex;
    char secSeg[10] = {0};
  #endif

  if (path == 0 || len <= 3 || outStr == 0) return;

  tmpFile = path;
  #if _LFN_UNICODE
    strIndex1 = (WCHAR *)wcsstr((const WCHAR *)tmpFile, (const WCHAR *)'/');
    strIndex2 = (WCHAR *)wcsstr((const WCHAR *)tmpFile, (const WCHAR *)'.');
  #else
    strIndex1 = (char *)strrchr(tmpFile, '/');
    strIndex2 = (char *)strrchr(tmpFile, '.');
  #endif

  beginIndex = (strIndex1 != 0) ? strIndex1 + 1 : tmpFile;

  if (strIndex2 == 0 || (strIndex1 > strIndex2)) { // not gcode file
    #if _LFN_UNICODE
      if (wcslen(beginIndex) > len)
        wcsncpy(outStr, beginIndex, len);
      else
        wcscpy(outStr, beginIndex);
    #else
      if ((int)strlen(beginIndex) > len)
        strncpy(outStr, beginIndex, len);
      else
        strcpy(outStr, beginIndex);
    #endif
  }
  else { // gcode file
    if (strIndex2 - beginIndex > (len - 2)) {
      #if _LFN_UNICODE
        wcsncpy(outStr, (const WCHAR *)beginIndex, len - 3);
        wcscat(outStr, (const WCHAR *)gFileTail);
      #else
        strncpy(outStr, beginIndex, len - 4);
        strcat_P(outStr, PSTR("~.g"));
      #endif
    }
    else {
      #if _LFN_UNICODE
        wcsncpy(outStr, (const WCHAR *)beginIndex, strIndex2 - beginIndex + 1);
        wcscat(outStr, (const WCHAR *)&gFileTail[3]);
      #else
        strncpy(outStr, beginIndex, strIndex2 - beginIndex + 1);
        strcat_P(outStr, PSTR("g"));
      #endif
    }
  }

  #if _LFN_UNICODE
    if (wcslen(outStr) > bytePerLine) {
      wcscpy(secSeg, (const WCHAR *)&outStr[bytePerLine]);
      outStr[bytePerLine]     = '\n';
      outStr[bytePerLine + 1] = '\0';
      wcscat(outStr, (const WCHAR *)secSeg);
    }
  #else
    if ((int)strlen(outStr) > bytePerLine) {
      strcpy(secSeg, &outStr[bytePerLine]);
      outStr[bytePerLine]     = '\n';
      outStr[bytePerLine + 1] = '\0';
      strcat(outStr, secSeg);
    }
    else {
      strcat_P(outStr, PSTR("\n"));
    }
  #endif
}

void lv_clear_print_file() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
