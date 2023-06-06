/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/nextion/FileNavigator.cpp
 * ****************************************
 * Extensible_UI implementation for Nextion
 * https://github.com/Skorpi08
 * ***************************************/

#include "nextion_tft_defs.h" // for MAX_PATH_LEN
#include "../ui_api.h"

using namespace ExtUI;

class FileNavigator {
  public:
    FileNavigator();
    static void  reset();
    static void  getFiles(uint16_t);
    static void  upDIR();
    static void  changeDIR(char *);
    static void  refresh();
    static char* getCurrentDirPath();
  private:
    static FileList filelist;
    static char     currentDirPath[MAX_PATH_LEN];
    static uint16_t lastindex;
    static uint8_t  folderdepth;
    static uint16_t currentindex;
};

extern FileNavigator filenavigator;
