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
#pragma once

/**
 * lcd/extui/anycubic_chiron/FileNavigator.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "chiron_tft_defs.h"
#include "../ui_api.h"

using namespace ExtUI;

namespace Anycubic {

  class FileNavigator {
    public:
      static FileList filelist;

      FileNavigator();
      static void reset();
      static void getFiles(uint16_t, panel_type_t, uint8_t filesneeded=4);
      static void upDIR();
      static void changeDIR(const char *);
      static void sendFile(panel_type_t);
      static void refresh();
      static void skiptofileindex(uint16_t);
    private:
      static uint16_t lastpanelindex;
      static uint16_t currentindex;
      static uint8_t  folderdepth;
      static uint16_t currentDirIndex[MAX_FOLDER_DEPTH];
      static char     currentDirPath[MAX_PATH_LEN + 1];
  };

  extern FileNavigator filenavigator;

}
