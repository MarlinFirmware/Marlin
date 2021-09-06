/******************
 * files_screen.h *
 ******************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#define FTDI_FILES_SCREEN
#define FTDI_FILES_SCREEN_CLASS FilesScreen

struct FilesScreenData {
  struct {
    uint8_t is_dir  : 1;
    uint8_t is_root : 1;
  } flags;
  uint8_t   selected_tag;
  uint8_t   num_page;
  uint8_t   cur_page;
  #if ENABLED(SCROLL_LONG_FILENAMES) && (FTDI_API_LEVEL >= 810)
    uint16_t  scroll_pos;
    uint16_t  scroll_max;
  #endif
};

class FilesScreen : public BaseScreen, public CachedScreen<FILES_SCREEN_CACHE, FILE_SCREEN_DL_SIZE> {
  private:
    static uint8_t  getTagForLine(uint8_t line) {return line + 2;}
    static uint8_t  getLineForTag(uint8_t tag)  {return  tag - 2;}
    static uint16_t getFileForTag(uint8_t tag);
    static uint16_t getSelectedFileIndex();

    inline static const char *getSelectedShortFilename() {return getSelectedFilename(false);}
    inline static const char *getSelectedLongFilename()  {return getSelectedFilename(true);}
    static const char *getSelectedFilename(bool longName);

    static void drawFileButton(int x, int y, int w, int h, const char *filename, uint8_t tag, bool is_dir, bool is_highlighted);
    static void drawFileButton(const char *filename, uint8_t tag, bool is_dir, bool is_highlighted);
    static void drawFileList();
    static void drawHeader();
    static void drawArrows();
    static void drawFooter();
    static void drawSelectedFile();

    static void gotoPage(uint8_t);
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
    static void onMediaRemoved();
};
