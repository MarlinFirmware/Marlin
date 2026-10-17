/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_E3V2_TOUCH)

#include "DGUSSDCardHandler.h"
#include "../ui_api.h"

char DGUS_SDCardHandler::filenames[DGUS_FILE_COUNT][DGUS_FILENAME_LEN + 1];
char DGUS_SDCardHandler::shortnames[DGUS_FILE_COUNT][13];
uint8_t DGUS_SDCardHandler::fileCount = 0;
int8_t DGUS_SDCardHandler::selected = -1;

void DGUS_SDCardHandler::reset() {
  fileCount = 0;
  selected = -1;
  for (uint8_t i = 0; i < DGUS_FILE_COUNT; ++i) filenames[i][0] = shortnames[i][0] = '\0';

  #if HAS_MEDIA
    if (!ExtUI::isMediaMounted()) return;

    ExtUI::FileList fileList;
    while (!fileList.isAtRootDir()) fileList.upDir();

    const uint16_t entries = fileList.count();
    for (uint16_t pos = 0; pos < entries && fileCount < DGUS_FILE_COUNT; ++pos) {
      if (!fileList.seek(pos, true) || fileList.isDir()) continue;
      strlcpy(filenames[fileCount], fileList.longFilename(), sizeof(filenames[0]));
      strlcpy(shortnames[fileCount], fileList.shortFilename(), sizeof(shortnames[0]));
      ++fileCount;
    }
  #endif
}

bool DGUS_SDCardHandler::select(const uint8_t index) {
  if (index >= fileCount) return false;
  selected = index;
  return true;
}

#endif // DGUS_LCD_UI_E3V2_TOUCH
