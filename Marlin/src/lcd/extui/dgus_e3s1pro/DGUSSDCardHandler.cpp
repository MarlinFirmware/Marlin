/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(DGUS_LCD_UI_E3S1PRO)

#include "../../../sd/cardreader.h"
#include "DGUSSDCardHandler.h"

char DGUS_SDCardHandler::filenames[5][LONG_FILENAME_LENGTH] = {0};

void DGUS_SDCardHandler::setFilename(file_t file, const char* filename, bool isDir) {
  const uint8_t fileIndex = DGUS_FILE_TO_INDEX(file);
  filenames[fileIndex][0] = '\0';
  if (filename) strcat(filenames[fileIndex], filename);
  if (isDir) strcat(filenames[fileIndex], "/");
}

#endif // DGUS_LCD_UI_E3S1PRO
