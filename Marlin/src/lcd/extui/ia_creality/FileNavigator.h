/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/* ****************************************
 * lcd/extui/ia_creality/FileNavigator.h
 * ****************************************
 * Extensible_UI implementation for Creality DWIN
 * 10SPro, Max, CRX and others
 * Based on implementations for Anycubic Chiron and Nextion by Nick Wells and Skorpi08
 * Written by Insanity Automation
 * ***************************************/

#include "../ui_api.h"

#define MAX_FOLDER_DEPTH   4    // Limit folder depth TFT has a limit for the file path
#define MAX_PATH_LEN      16 * MAX_FOLDER_DEPTH // Maximum number of characters in a SD file path
#define DISPLAY_FILES      4

class FileNavigator {
  public:
    FileNavigator();
    static void reset();
    static void getFiles(uint16_t);
    static void upDIR();
    static void changeDIR(char *);
    static void refresh();
    static char* getCurrentDirPath();
    static uint8_t  folderdepth;
    static uint16_t currentindex;
    static bool getIndexisDir(uint16_t);
    const char *getIndexName(uint16_t);
    static uint16_t maxFiles();
  private:
    static ExtUI::FileList filelist;
    static char     currentDirPath[MAX_PATH_LEN];
    static uint16_t lastindex;
};

extern FileNavigator filenavigator;
