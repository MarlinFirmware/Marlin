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

/**
 * lcd/extui/lib/FileNavigator.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2021 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "chiron_tft_defs.h"
#include "../../ui_api.h"

using namespace ExtUI;

namespace Anycubic {
  class FileNavigator {
    public:
      FileNavigator();
      void   reset();
      void   getFiles(uint16_t, panel_type_t, uint8_t filesneeded = 4);
      void   upDIR();
      void   changeDIR(const char *);
      void   sendFile(panel_type_t);
      void   refresh();
      void   skiptofileindex(uint16_t);

      static FileList  filelist;
    private:
      static uint16_t  lastpanelindex;
      static uint16_t  currentindex;
      static uint8_t   currentfolderdepth;
      static uint16_t  currentfolderindex[MAX_FOLDER_DEPTH];
      static char      currentfoldername[MAX_PATH_LEN];
  };
  extern FileNavigator filenavigator;
}
